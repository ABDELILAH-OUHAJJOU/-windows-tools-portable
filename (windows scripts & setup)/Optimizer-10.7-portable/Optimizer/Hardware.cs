﻿//using System;
//using System.Collections.Generic;
//using System.Linq;
//using System.Text;
//using System.Threading.Tasks;

//namespace Optimizer
//{
//    public class CPU
//    {
//        public string Name { get; set; }
//        public ByteSize L2CacheSize { get; set; }
//        public ByteSize L3CacheSize { get; set; }
//        public UInt32 Cores { get; set; }
//        public UInt32 Threads { get; set; }
//        public UInt32 LogicalCpus { get; set; }
//        public string Virtualization { get; set; }
//        public string DataExecutionPrevention { get; set; }
//        public string Stepping { get; set; }
//        public string Revision { get; set; }
//    }

//    public class RAM
//    {
//        public string BankLabel { get; set; }
//        public UInt64 Capacity { get; set; }
//        public string FormFactor { get; set; }
//        public string Manufacturer { get; set; }
//        public string MemoryType { get; set; }
//        public UInt32 Speed { get; set; }
//    }

//    public class VirtualMemory
//    {
//        public ByteSize TotalVirtualMemory { get; set; }
//        public ByteSize AvailableVirtualMemory { get; set; }
//        public ByteSize UsedVirtualMemory { get; set; }
//    }

//    public class GPU
//    {
//        public string Name { get; set; }
//        public ByteSize Memory { get; set; }
//        public UInt32 ResolutionX { get; set; }
//        public UInt32 ResolutionY { get; set; }
//        public UInt32 RefreshRate { get; set; }
//        public string DACType { get; set; }
//        public string VideoMemoryType { get; set; }
//    }

//    public class Disk
//    {
//        public UInt32 BytesPerSector { get; set; }
//        public string FirmwareRevision { get; set; }
//        public string MediaType { get; set; }
//        public string Model { get; set; }
//        public ByteSize Capacity { get; set; }
//    }

//    public class Volume
//    {
//        public UInt64 BlockSize { get; set; }
//        public ByteSize Capacity { get; set; }
//        public string Compressed { get; set; }
//        public string DriveLetter { get; set; }
//        public string DriveType { get; set; }
//        public string FileSystem { get; set; }
//        public ByteSize FreeSpace { get; set; }
//        public ByteSize UsedSpace { get; set; }
//        public string Indexing { get; set; }
//        public string Label { get; set; }
//    }

//    public class NetworkDevice
//    {
//        public string AdapterType { get; set; }
//        public string Manufacturer { get; set; }
//        public string ProductName { get; set; }
//        public string PhysicalAdapter { get; set; }
//        public string MacAddress { get; set; }
//        public string ServiceName { get; set; }
//    }

//    public class Keyboard
//    {
//        public string Name { get; set; }
//        public string Layout { get; set; }
//        public string Status { get; set; }
//        public UInt16 FunctionKeys { get; set; }
//        public string Locked { get; set; }
//    }

//    public class PointingDevice
//    {
//        public string Name { get; set; }
//        public string Manufacturer { get; set; }
//        public string Status { get; set; }
//        public UInt16 Buttons { get; set; }
//        public string Locked { get; set; }
//        public string HardwareType { get; set; }
//        public string PointingType { get; set; }
//        public string DeviceInterface { get; set; }
//    }

//    public class AudioDevice
//    {
//        public string ProductName { get; set; }
//        public string Manufacturer { get; set; }
//        public string Status { get; set; }
//    }

//    public class Motherboard
//    {
//        public string Model { get; set; }
//        public string Manufacturer { get; set; }
//        public string Chipset { get; set; }
//        public string Product { get; set; }
//        public string Version { get; set; }
//        public string Revision { get; set; }
//        public string SystemModel { get; set; }

//        public string BIOSName { get; set; }
//        public string BIOSManufacturer { get; set; }
//        public string BIOSVersion { get; set; }
//        public string BIOSBuildNumber { get; set; }
//    }
//}
