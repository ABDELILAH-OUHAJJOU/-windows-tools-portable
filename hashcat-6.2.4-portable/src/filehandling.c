/**
 * Author......: See docs/credits.txt
 * License.....: MIT
 */

#include "common.h"
#include "types.h"
#include "memory.h"
#include "shared.h"
#include "filehandling.h"

#if defined (__CYGWIN__)
// workaround for zlib with cygwin build
int _wopen (const char *path, int oflag, ...)
{
  va_list ap;
  va_start (ap, oflag);
  int r = open (path, oflag, ap);
  va_end (ap);
  return r;
}
#endif

bool hc_fopen (HCFILE *fp, const char *path, const char *mode)
{
  if (fp == NULL || path == NULL || mode == NULL) return false;

  /* cleanup */
  fp->fd       = -1;
  fp->pfp      = NULL;
  fp->gfp      = NULL;
  fp->ufp      = NULL;
  fp->bom_size = 0;
  fp->path     = NULL;
  fp->mode     = NULL;

  int oflag = -1;

  int fmode = S_IRUSR|S_IWUSR;

  if (strncmp (mode, "a", 1) == 0 || strncmp (mode, "ab", 2) == 0)
  {
    oflag = O_WRONLY | O_CREAT | O_APPEND;

    #if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
    if (strncmp (mode, "ab", 2) == 0) oflag |= O_BINARY;
    #endif
  }
  else if (strncmp (mode, "r", 1) == 0 || strncmp (mode, "rb", 2) == 0)
  {
    oflag = O_RDONLY;
    fmode = -1;

    #if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
    if (strncmp (mode, "rb", 2) == 0) oflag |= O_BINARY;
    #endif
  }
  else if (strncmp (mode, "w", 1) == 0 || strncmp (mode, "wb", 2) == 0)
  {
    oflag = O_WRONLY | O_CREAT | O_TRUNC;

    #if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
    if (strncmp (mode, "wb", 2) == 0) oflag |= O_BINARY;
    #endif
  }
  else
  {
    // ADD more strncmp to handle more "mode"
    return false;
  }

  unsigned char check[8] = { 0 };

  bool is_gzip = false;
  bool is_zip  = false;

  int fd_tmp = open (path, O_RDONLY);

  if (fd_tmp != -1)
  {
    lseek (fd_tmp, 0, SEEK_SET);

    if (read (fd_tmp, check, sizeof (check)) > 0)
    {
      if (check[0] == 0x1f && check[1] == 0x8b && check[2] == 0x08)                     is_gzip = true;
      if (check[0] == 0x50 && check[1] == 0x4b && check[2] == 0x03 && check[3] == 0x04) is_zip  = true;

      // compressed files with BOM will be undetected!

      if (is_gzip == false && is_zip == false)
      {
        fp->bom_size = hc_string_bom_size (check);
      }
    }

    close (fd_tmp);
  }

  if (fmode == -1)
  {
    fp->fd = open (path, oflag);
  }
  else
  {
    fp->fd = open (path, oflag, fmode);
  }

  if (fp->fd == -1) return false;

  if (is_zip == false)
  {
    if (is_gzip)
    {
      if ((fp->gfp = gzdopen (fp->fd, mode)) == NULL) return false;
    }
    else
    {
      if ((fp->pfp = fdopen (fp->fd, mode)) == NULL) return false;

      if (fp->bom_size)
      {
        // atm just skip bom

        const int nread = fread (check, sizeof (char), fp->bom_size, fp->pfp);

        if (nread != fp->bom_size) return false;
      }
    }
  }
  else
  {
    if ((fp->ufp = unzOpen64 (path)) == NULL) return false;

    if (unzOpenCurrentFile (fp->ufp) != UNZ_OK) return false;
  }

  fp->path = path;
  fp->mode = mode;

  return true;
}

bool hc_fopen_raw (HCFILE *fp, const char *path, const char *mode)
{
  if (fp == NULL || path == NULL || mode == NULL) return false;

  /* cleanup */
  fp->fd       = -1;
  fp->pfp      = NULL;
  fp->gfp      = NULL;
  fp->ufp      = NULL;
  fp->bom_size = 0;
  fp->path     = NULL;
  fp->mode     = NULL;

  int oflag = -1;

  int fmode = S_IRUSR|S_IWUSR;

  if (strncmp (mode, "a", 1) == 0 || strncmp (mode, "ab", 2) == 0)
  {
    oflag = O_WRONLY | O_CREAT | O_APPEND;

    #if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
    if (strncmp (mode, "ab", 2) == 0) oflag |= O_BINARY;
    #endif
  }
  else if (strncmp (mode, "r", 1) == 0 || strncmp (mode, "rb", 2) == 0)
  {
    oflag = O_RDONLY;
    fmode = -1;

    #if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
    if (strncmp (mode, "rb", 2) == 0) oflag |= O_BINARY;
    #endif
  }
  else if (strncmp (mode, "w", 1) == 0 || strncmp (mode, "wb", 2) == 0)
  {
    oflag = O_WRONLY | O_CREAT | O_TRUNC;

    #if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
    if (strncmp (mode, "wb", 2) == 0) oflag |= O_BINARY;
    #endif
  }
  else
  {
    // ADD more strncmp to handle more "mode"
    return false;
  }

  if (fmode == -1)
  {
    fp->fd = open (path, oflag);
  }
  else
  {
    fp->fd = open (path, oflag, fmode);
  }

  if (fp->fd == -1) return false;

  if ((fp->pfp = fdopen (fp->fd, mode)) == NULL) return false;

  fp->path = path;
  fp->mode = mode;

  return true;
}

size_t hc_fread (void *ptr, size_t size, size_t nmemb, HCFILE *fp)
{
  size_t n = -1;

  if (fp == NULL) return n;

  if (fp->gfp)
  {
    n = gzfread (ptr, size, nmemb, fp->gfp);
  }
  else if (fp->ufp)
  {
    unsigned s = size * nmemb;

    n = unzReadCurrentFile (fp->ufp, ptr, s);
  }
  else if (fp->pfp)
  {
    #if defined (_WIN)

    // 4 GB fread () limit for windows systems ?
    // see: https://social.msdn.microsoft.com/Forums/vstudio/en-US/7c913001-227e-439b-bf07-54369ba07994/fwrite-issues-with-large-data-write

    #define GIGABYTE (1024u * 1024u * 1024u)

    if (((size * nmemb) / GIGABYTE) < 4)
    {
      n = fread (ptr, size, nmemb, fp->pfp);
    }
    else
    {
      if ((size / GIGABYTE) > 3) return -1;

      size_t elements_max  = (3u * GIGABYTE) / size;
      size_t elements_left = nmemb;

      size_t off = 0;

      n = 0;

      while (elements_left > 0)
      {
        size_t elements_cur = elements_max;

        if (elements_left < elements_max) elements_cur = elements_left;

        size_t ret = fread (ptr + off, size, elements_cur, fp->pfp);

        if (ret != elements_cur) return -1;

        n   += ret;
        off += ret * size;

        elements_left -= ret;
      }
    }
    #else
    n = fread (ptr, size, nmemb, fp->pfp);
    #endif
  }

  return n;
}

size_t hc_fwrite (const void *ptr, size_t size, size_t nmemb, HCFILE *fp)
{
  size_t n = -1;

  if (fp == NULL) return n;

  if (fp->gfp)
  {
    n = gzfwrite (ptr, size, nmemb, fp->gfp);
  }
  else if (fp->ufp)
  {
  }
  else if (fp->pfp)
  {
    #if defined (_WIN)

    // 4 GB fwrite () limit for windows systems ?
    // see: https://social.msdn.microsoft.com/Forums/vstudio/en-US/7c913001-227e-439b-bf07-54369ba07994/fwrite-issues-with-large-data-write

    #define GIGABYTE (1024u * 1024u * 1024u)

    if (((size * nmemb) / GIGABYTE) < 4)
    {
      n = fwrite (ptr, size, nmemb, fp->pfp);
    }
    else
    {
      if ((size / GIGABYTE) > 3) return -1;

      size_t elements_max  = (3u * GIGABYTE) / size;
      size_t elements_left = nmemb;

      size_t off = 0;

      n = 0;

      while (elements_left > 0)
      {
        size_t elements_cur = elements_max;

        if (elements_left < elements_max) elements_cur = elements_left;

        size_t ret = fwrite (ptr + off, size, elements_cur, fp->pfp);

        if (ret != elements_cur) return -1;

        n   += ret;
        off += ret * size;

        elements_left -= ret;
      }
    }
    #else
    n = fwrite (ptr, size, nmemb, fp->pfp);
    #endif
  }

  return n;
}

int hc_fseek (HCFILE *fp, off_t offset, int whence)
{
  int r = -1;

  if (fp == NULL) return r;

  if (fp->gfp)
  {
    r = gzseek (fp->gfp, offset, whence);
  }
  else if (fp->ufp)
  {
    /*
    // untested and not used in wordlist engine
    zlib_filefunc64_32_def *d = NULL;
    if (whence == SEEK_SET)
    {
      r = ZSEEK64 (*d, fp->ufp, offset, ZLIB_FILEFUNC_SEEK_SET);
    }
    else if (whence == SEEK_CUR)
    {
      r = ZSEEK64 (*d, fp->ufp, offset, ZLIB_FILEFUNC_SEEK_CUR);
    }
    else if (whence == SEEK_END)
    {
      r = ZSEEK64 (*d, fp->ufp, offset, ZLIB_FILEFUNC_SEEK_END);
    }
    // or
    // r = unzSetOffset (fp->ufp, offset);
    */
  }
  else if (fp->pfp)
  {
    r = fseeko (fp->pfp, offset, whence);
  }

  return r;
}

void hc_rewind (HCFILE *fp)
{
  if (fp == NULL) return;

  if (fp->gfp)
  {
    gzrewind (fp->gfp);
  }
  else if (fp->ufp)
  {
    unzGoToFirstFile (fp->ufp);
  }
  else if (fp->pfp)
  {
    rewind (fp->pfp);
  }
}

int hc_fstat (HCFILE *fp, struct stat *buf)
{
  if (fp == NULL || buf == NULL || fp->fd == -1) return -1;

  return fstat (fp->fd, buf);
}

off_t hc_ftell (HCFILE *fp)
{
  off_t n = 0;

  if (fp == NULL) return -1;

  if (fp->gfp)
  {
    n = (off_t) gztell (fp->gfp);
  }
  else if (fp->ufp)
  {
    n = unztell (fp->ufp);
  }
  else if (fp->pfp)
  {
    n = ftello (fp->pfp);
  }

  return n;
}

int hc_fputc (int c, HCFILE *fp)
{
  int r = -1;

  if (fp == NULL) return r;

  if (fp->gfp)
  {
    r = gzputc (fp->gfp, c);
  }
  else if (fp->ufp)
  {
  }
  else if (fp->pfp)
  {
    r = fputc (c, fp->pfp);
  }

  return r;
}

int hc_fgetc (HCFILE *fp)
{
  int r = -1;

  if (fp == NULL) return r;

  if (fp->gfp)
  {
    r = gzgetc (fp->gfp);
  }
  else if (fp->ufp)
  {
    unsigned char c = 0;

    if (unzReadCurrentFile (fp->ufp, &c, 1) == 1) r = (int) c;
  }
  else if (fp->pfp)
  {
    r = fgetc (fp->pfp);
  }

  return r;
}

char *hc_fgets (char *buf, int len, HCFILE *fp)
{
  char *r = NULL;

  if (fp == NULL) return r;

  if (fp->gfp)
  {
    r = gzgets (fp->gfp, buf, len);
  }
  else if (fp->ufp)
  {
    if (unzReadCurrentFile (fp->ufp, buf, len) > 0) r = buf;
  }
  else if (fp->pfp)
  {
    r = fgets (buf, len, fp->pfp);
  }

  return r;
}

int hc_vfprintf (HCFILE *fp, const char *format, va_list ap)
{
  int r = -1;

  if (fp == NULL) return r;

  if (fp->gfp)
  {
    r = gzvprintf (fp->gfp, format, ap);
  }
  else if (fp->ufp)
  {
  }
  else if (fp->pfp)
  {
    r = vfprintf (fp->pfp, format, ap);
  }

  return r;
}

int hc_fprintf (HCFILE *fp, const char *format, ...)
{
  int r = -1;

  if (fp == NULL) return r;

  va_list ap;

  va_start (ap, format);

  if (fp->gfp)
  {
    r = gzvprintf (fp->gfp, format, ap);
  }
  else if (fp->ufp)
  {
  }
  else if (fp->pfp)
  {
    r = vfprintf (fp->pfp, format, ap);
  }

  va_end (ap);

  return r;
}

int hc_fscanf (HCFILE *fp, const char *format, void *ptr)
{
  if (fp == NULL) return -1;

  char buf[HCBUFSIZ_TINY];

  char *b = hc_fgets (buf, HCBUFSIZ_TINY - 1, fp);

  if (b == NULL)
  {
    return -1;
  }

  sscanf (b, format, ptr);

  return 1;
}

int hc_feof (HCFILE *fp)
{
  int r = -1;

  if (fp == NULL) return r;

  if (fp->gfp)
  {
    r = gzeof (fp->gfp);
  }
  else if (fp->ufp)
  {
    r = unzeof (fp->ufp);
  }
  else if (fp->pfp)
  {
    r = feof (fp->pfp);
  }

  return r;
}

void hc_fflush (HCFILE *fp)
{
  if (fp == NULL) return;

  if (fp->gfp)
  {
    gzflush (fp->gfp, Z_SYNC_FLUSH);
  }
  else if (fp->ufp)
  {
  }
  else if (fp->pfp)
  {
    fflush (fp->pfp);
  }
}

void hc_fsync (HCFILE *fp)
{
  if (fp == NULL) return;

  if (fp->pfp)
  {
#if defined (_WIN)
    HANDLE h = (HANDLE) _get_osfhandle (fp->fd);

    FlushFileBuffers (h);
#else
    fsync (fp->fd);
#endif
  }
}

void hc_fclose (HCFILE *fp)
{
  if (fp == NULL) return;

  if (fp->gfp)
  {
    gzclose (fp->gfp);
  }
  else if (fp->ufp)
  {
    unzCloseCurrentFile (fp->ufp);

    unzClose (fp->ufp);

    close (fp->fd);
  }
  else if (fp->pfp)
  {
    fclose (fp->pfp);
  }

  fp->fd = -1;
  fp->pfp = NULL;
  fp->gfp = NULL;
  fp->ufp = NULL;

  fp->path = NULL;
  fp->mode = NULL;
}

size_t fgetl (HCFILE *fp, char *line_buf, const size_t line_sz)
{
  size_t line_truncated = 0;

  size_t line_len = 0;

  while (!hc_feof (fp))
  {
    const int c = hc_fgetc (fp);

    if (c == EOF) break;

    if (line_len == line_sz)
    {
      line_truncated++;

      continue;
    }

    line_buf[line_len] = (char) c;

    line_len++;

    if (c == '\n') break;
  }

  if (line_truncated > 0)
  {
    fprintf (stderr, "\nOversized line detected! Truncated %" PRIu64 " bytes\n", (u64) line_truncated);
  }

  if (line_len == 0) return 0;

  while (line_len)
  {
    if (line_buf[line_len - 1] == '\n')
    {
      line_len--;

      continue;
    }

    if (line_buf[line_len - 1] == '\r')
    {
      line_len--;

      continue;
    }

    break;
  }

  line_buf[line_len] = 0;

  return line_len;
}

u64 count_lines (HCFILE *fp)
{
  u64 cnt = 0;

  char *buf = (char *) hcmalloc (HCBUFSIZ_LARGE + 1);

  char prev = '\n';

  while (!hc_feof (fp))
  {
    size_t nread = hc_fread (buf, sizeof (char), HCBUFSIZ_LARGE, fp);

    if (nread < 1) continue;

    for (size_t i = 0; i < nread; i++)
    {
      if (prev == '\n') cnt++;

      prev = buf[i];
    }
  }

  hcfree (buf);

  return cnt;
}

size_t in_superchop (char *buf)
{
  size_t len = strlen (buf);

  while (len)
  {
    if (buf[len - 1] == '\n')
    {
      len--;

      buf[len] = 0;

      continue;
    }

    if (buf[len - 1] == '\r')
    {
      len--;

      buf[len] = 0;

      continue;
    }

    break;
  }

  return len;
}

size_t superchop_with_length (char *buf, const size_t len)
{
  size_t new_len = len;

  while (new_len)
  {
    if (buf[new_len - 1] == '\n')
    {
      new_len--;

      buf[new_len] = 0;

      continue;
    }

    if (buf[new_len - 1] == '\r')
    {
      new_len--;

      buf[new_len] = 0;

      continue;
    }

    break;
  }

  return new_len;
}
