#ifndef __PARSER_H
#define __PARSER_H

void bdf_parser(void)
{
	switch(pci_device_detail.vendor_id)
	{
		case 0x4b3:
			strcpy(pci_device_detail.name_string, "IBM");break;
		case 0x46d: // Logitech
			strcpy(pci_device_detail.name_string, "Logitech");break;
		case 0xa5c: // Broadcom
		case 0x1166:
		case 0x14e4:
			strcpy(pci_device_detail.name_string, "Broadcom");break;	
		case 0xac8: // ASUS
			strcpy(pci_device_detail.name_string, "ASUS");break;							
		case 0xcf3: // TP-LINK
			strcpy(pci_device_detail.name_string, "TP-Link");break;
		case 0x1000:
		case 0x11c1:
		case 0x13c1:
			strcpy(pci_device_detail.name_string, "LSI");break;
		case 0x1002: // AMD
		case 0x1014:
		case 0x1022:
			strcpy(pci_device_detail.name_string, "AMD");break;
		case 0x102b: // Matrox
			strcpy(pci_device_detail.name_string, "Matrox");break;
		case 0x101e: // AMI
		case 0x1529:
			strcpy(pci_device_detail.name_string, "AMI");break;							
		case 0x1020: // Hitachi
		case 0x1054:
		case 0x1250:
		case 0x1388:
			strcpy(pci_device_detail.name_string, "Hitachi");break;													
		case 0x1025: // Acer
			strcpy(pci_device_detail.name_string, "Acer");break;							
		case 0x1028: // Dell
			strcpy(pci_device_detail.name_string, "Dell");break;							
		case 0x1033: // NEC
		case 0xa200:
			strcpy(pci_device_detail.name_string, "NEC");break;							
		case 0x104a: // STM
			strcpy(pci_device_detail.name_string, "STM");break;
		case 0x104C: // TI
			strcpy(pci_device_detail.name_string, "TI");break;
		case 0x106b:
			strcpy(pci_device_detail.name_string, "Apple");break;
		case 0x1078:
			strcpy(pci_device_detail.name_string, "Cyrix");break;
		case 0x108e:
			strcpy(pci_device_detail.name_string, "Sun");break;
		case 0x1095:
			strcpy(pci_device_detail.name_string, "Silicon Image");break;
		case 0x1099: // Sumsung Electronic
		case 0x10c3: // Sumsung Semiconductors
			strcpy(pci_device_detail.name_string, "Sumsung");break;
		case 0x10ca:
		case 0x119e:
			strcpy(pci_device_detail.name_string, "Fujitsu");break;
		case 0x10de: // NVIDIA
			strcpy(pci_device_detail.name_string, "NVIDIA");break;
		case 0x10ec: // Realtek
			strcpy(pci_device_detail.name_string, "Realtek");break;								
		case 0x1106: // VIA
			strcpy(pci_device_detail.name_string, "VIA");break;
		case 0x1114:
			strcpy(pci_device_detail.name_string, "Atmel");break;
		case 0x1137:
			strcpy(pci_device_detail.name_string, "Cisco");break;
		case 0x113c:
			strcpy(pci_device_detail.name_string, "Cyclone");break;
		case 0x115f:
			strcpy(pci_device_detail.name_string, "Maxtor");break;
		case 0x1179: // Toshiba
		case 0x13d7:
			strcpy(pci_device_detail.name_string, "Toshiba");break;
		case 0x117b: // LG
		case 0x122b:
			strcpy(pci_device_detail.name_string, "LG");break;
		case 0x1148:
		case 0x11ab: // Marvell
		case 0x1b4b:
			strcpy(pci_device_detail.name_string, "Marvell");break;
		case 0x11ad: // Lite-On
			strcpy(pci_device_detail.name_string, "Lite-On");break;
		case 0x11b7: // Motorola
			strcpy(pci_device_detail.name_string, "Motorola");break;
		case 0x1260:
			strcpy(pci_device_detail.name_string, "Intersil");break;
		case 0x12d1: // Huawei
			strcpy(pci_device_detail.name_string, "Huawei");break;	
		case 0x1363:
			strcpy(pci_device_detail.name_string, "Phoenix");break;
		case 0x13b5:
			strcpy(pci_device_detail.name_string, "ARM");break;
		case 0x13bd:
			strcpy(pci_device_detail.name_string, "SHARP");break;
		case 0x1462:
			strcpy(pci_device_detail.name_string, "MSI");break;
		case 0x153f:
			strcpy(pci_device_detail.name_string, "MIPS");break;
		case 0x1574:
			strcpy(pci_device_detail.name_string, "Fairchild");break;
		case 0x15b7: // Sandisk
			strcpy(pci_device_detail.name_string, "Sandisk");break;
		case 0x2646: // Kingston
			strcpy(pci_device_detail.name_string, "Kingston");break;	
		case 0x1344:
		case 0xc0a9: // Micron?/Crutial
			strcpy(pci_device_detail.name_string, "Micron/Crutial");break;
		case 0x5333:
			strcpy(pci_device_detail.name_string, "S3");break;
		case 0x1b21:
		case 0x6945:
			strcpy(pci_device_detail.name_string, "ASMedia");break;
		case 0x104d:
		case 0xa304: // Sony
			strcpy(pci_device_detail.name_string, "Sony");break;
		case 0x163c:
		case 0x8086: // Intel
		case 0x8087:
			strcpy(pci_device_detail.name_string, "Intel");break;
		case 0x9004:
		case 0x9005:
			strcpy(pci_device_detail.name_string, "Adaptec");break;
		default:
			strcpy(pci_device_detail.name_string, "Unknown");break;
	}

	switch(pci_device_detail.base_class)
	{
		case 0x0: // Device was built before Class Code definitions	
			switch (pci_device_detail.sub_class)
			{
				case 0x0: // Currently implemented devices
					strcat(pci_device_detail.name_string, " Implemented");
					break;
				case 0x1: // VGA-compatible device
					strcat(pci_device_detail.name_string, " VGA-compatible");
					break;	
				default:
					strcat(pci_device_detail.name_string, " Lagacy");
					break;
			}
			break;
		case 0x1: // Mass Storage Controller 
			switch (pci_device_detail.sub_class)
			{
				case 0x0: // SCSI bus controller
					strcat(pci_device_detail.name_string, " SCSI bus controller");
					break;
				case 0x1: // IDE controller
					strcat(pci_device_detail.name_string, " IDE controller");
					break;
				case 0x2: // Floppy disk controller
					strcat(pci_device_detail.name_string, " Floppy disk controller");
					break;
				case 0x3:
					strcat(pci_device_detail.name_string, " IPI bus controller");
					break;
				case 0x4:
					strcat(pci_device_detail.name_string, " RAID controller");
					break;
				case 0x5:
					strcat(pci_device_detail.name_string, " ATA controller");
					break;
				case 0x6:
					strcat(pci_device_detail.name_string, " SATA controller");
					break;
				case 0x7:
					strcat(pci_device_detail.name_string, " SAS controller");
					break;
				case 0x80:
					strcat(pci_device_detail.name_string, " Mass storage controller");
					break;
				default:
					strcat(pci_device_detail.name_string, " Mass storage controller");
					break;
			}
			break;
		case 0x2: // Network Controller 
			switch (pci_device_detail.sub_class)
			{
				case 0x0:
					strcat(pci_device_detail.name_string, " Ethernet controller");
					break;
				case 0x1:
					strcat(pci_device_detail.name_string, " Token Ring controller");
					break;
				case 0x2:
					strcat(pci_device_detail.name_string, " FDDI controller");
					break;
				case 0x3:
					strcat(pci_device_detail.name_string, " ATM controller");
					break;
				case 0x4:
					strcat(pci_device_detail.name_string, " ISDN controller");
					break;
				case 0x5:
					strcat(pci_device_detail.name_string, " WorldFip controller");
					break;
				case 0x6:
					strcat(pci_device_detail.name_string, " PICMG 2.14 Multi Computing");
					break;
				case 0x80:
					strcat(pci_device_detail.name_string, " network controller");
					break;
				default:
					strcat(pci_device_detail.name_string, " network controller");
					break;
			}
			break;
		case 0x3: // Display Controller 
			switch (pci_device_detail.sub_class)
			{
				case 0x0:
					strcat(pci_device_detail.name_string, " VGA controller");
					break;
				case 0x1:
					strcat(pci_device_detail.name_string, " XGA controller");
					break;
				case 0x2:
					strcat(pci_device_detail.name_string, " 3D controller");
					break;
				case 0x80:
					strcat(pci_device_detail.name_string, " display controller");
					break;
				default:
					strcat(pci_device_detail.name_string, " display controller");
					break;
			}
			break;
		case 0x4: // Multimedia Device 
			switch (pci_device_detail.sub_class)
			{
				case 0x0:
					strcat(pci_device_detail.name_string, " Video device");
					break;
				case 0x1:
					strcat(pci_device_detail.name_string, " Audio device");
					break;
				case 0x2:
					strcat(pci_device_detail.name_string, " Computer telephony device");
					break;
				case 0x80:
					strcat(pci_device_detail.name_string, " multimedia device");
					break;
				default:
					strcat(pci_device_detail.name_string, " multidedia device");
					break;
			}
			break;
		case 0x5: // Memory Controller 
			switch (pci_device_detail.sub_class)
			{
				case 0x0:
					strcat(pci_device_detail.name_string, " RAM controller");
					break;
				case 0x1:
					strcat(pci_device_detail.name_string, " Flash controller");
					break;
				case 0x80:
					strcat(pci_device_detail.name_string, " memory controller");
					break;
				default:
					strcat(pci_device_detail.name_string, " memory controller");
					break;
			}
			break;
		case 0x6: // Bridge Device 
			switch (pci_device_detail.sub_class)
			{
				case 0x0:
					strcat(pci_device_detail.name_string, " Host bridge");
					break;
				case 0x1:
					strcat(pci_device_detail.name_string, " ISA bridge");
					break;
				case 0x2:
					strcat(pci_device_detail.name_string, " EISA bridge");
					break;
				case 0x3:
					strcat(pci_device_detail.name_string, " MCA bridge");
					break;
				case 0x4:
					strcat(pci_device_detail.name_string, " PCI-to-PCI bridge");
					break;
				case 0x5:
					strcat(pci_device_detail.name_string, " PCMCIA bridge");
					break;
				case 0x6:
					strcat(pci_device_detail.name_string, " NuBus bridge");
					break;
				case 0x7:
					strcat(pci_device_detail.name_string, " CardBus bridge");
					break;
				case 0x8:
					strcat(pci_device_detail.name_string, " RACEway bridge");
					break;
				case 0x9:
					strcat(pci_device_detail.name_string, " Semi-transparent PCI-to-PCI");
					break;
				case 0xa:
					strcat(pci_device_detail.name_string, " InfiniBand-to-PCI bridge");
					break;
				case 0x80:
					strcat(pci_device_detail.name_string, " bridge device");
					break;
				default:
					strcat(pci_device_detail.name_string, " bridge device");
					break;
			}
			break;
		case 0x7: // Simple Communication Controller 
			switch (pci_device_detail.sub_class)
			{
				case 0x0:
					strcat(pci_device_detail.name_string, " serial controller");
					break;
				case 0x1:
					strcat(pci_device_detail.name_string, " Parallel port/controller");
					break;
				case 0x2:
					strcat(pci_device_detail.name_string, " Multiport serial controller");
					break;
				case 0x3:
					strcat(pci_device_detail.name_string, " Generic/Hayes modem");
					break;
				case 0x4:
					strcat(pci_device_detail.name_string, " GPIB controller");
					break;
				case 0x5:
					strcat(pci_device_detail.name_string, " Smart Card");
					break;
				case 0x80:
					strcat(pci_device_detail.name_string, " communications device");
					break;
				default:
					strcat(pci_device_detail.name_string, " communication device");
					break;
			}
			break;
		case 0x8: // Base System Peripherals
			switch (pci_device_detail.sub_class)
			{
				case 0x0:
					strcat(pci_device_detail.name_string, " PIC/APIC");
					break;
				case 0x1:
					strcat(pci_device_detail.name_string, " DMA controller");
					break;
				case 0x2:
					strcat(pci_device_detail.name_string, " System timer");
					break;
				case 0x3:
					strcat(pci_device_detail.name_string, " RTC controller");
					break;
				case 0x4:
					strcat(pci_device_detail.name_string, " PCI Hot-plug controller");
					break;
				case 0x5:
					strcat(pci_device_detail.name_string, " SD Host controller");
					break;
				case 0x80:
					strcat(pci_device_detail.name_string, " system peripheral");
					break;
				default:
					strcat(pci_device_detail.name_string, " system peripheral");
					break;
			}
			break;
		case 0x9: // Input Device 
			switch (pci_device_detail.sub_class)
			{
				case 0x0:
					strcat(pci_device_detail.name_string, " Keyboard controller");
					break;
				case 0x1:
					strcat(pci_device_detail.name_string, " Digitizer(pen)");
					break;
				case 0x2:
					strcat(pci_device_detail.name_string, " Mouse controller");
					break;
				case 0x3:
					strcat(pci_device_detail.name_string, " Scanner controller");
					break;
				case 0x4:
					strcat(pci_device_detail.name_string, " Gameport controller");
					break;
				case 0x80:
					strcat(pci_device_detail.name_string, " input controller");
					break;
				default:
					strcat(pci_device_detail.name_string, " Input device");
					break;
			}
			break;
		case 0xa: // Docking Stations
			switch (pci_device_detail.sub_class)
			{
				case 0x0:
					strcat(pci_device_detail.name_string, " Generic docking station");
					break;
				case 0x1:
					strcat(pci_device_detail.name_string, " docking station");
					break;
				default:
					strcat(pci_device_detail.name_string, " docking station");
					break;
			}
			break;
		case 0xb: // Processors
			switch (pci_device_detail.sub_class)
			{
				case 0x0:
					strcat(pci_device_detail.name_string, " 386");
					break;
				case 0x1:
					strcat(pci_device_detail.name_string, " 486");
					break;
				case 0x2:
					strcat(pci_device_detail.name_string, " Pentium");
					break;
				case 0x10:
					strcat(pci_device_detail.name_string, " Alpha");
					break;
				case 0x20:
					strcat(pci_device_detail.name_string, " PowerPC");
					break;
				case 0x30:
					strcat(pci_device_detail.name_string, " MIPS");
					break;
				case 0x40:
					strcat(pci_device_detail.name_string, " Co-processor");
					break;
				default:
					strcat(pci_device_detail.name_string, " Processor");
					break;
			}
			break;
		case 0xc: // Serial Bus Controller
			switch (pci_device_detail.sub_class)
			{
				case 0x0:
					strcat(pci_device_detail.name_string, " IEEE 1394");
					break;
				case 0x1:
					strcat(pci_device_detail.name_string, " ACCESS.bus");
					break;
				case 0x2:
					strcat(pci_device_detail.name_string, " SSA");
					break;
				case 0x3:
					switch (pci_device_detail.interface_class)
					{
						case 0x0:
							strcat(pci_device_detail.name_string, " UHCI controller");
							break;
						case 0x10:
							strcat(pci_device_detail.name_string, " OHCI controller");
							break;
						case 0x20:
							strcat(pci_device_detail.name_string, " EHCI controller");
							break;
						case 0x30:
							strcat(pci_device_detail.name_string, " xHCI controller");
							break;
						case 0x80:
							strcat(pci_device_detail.name_string, " USB controller");
							break;
						case 0xfe:
							strcat(pci_device_detail.name_string, " USB device");
							break;
						default:
							strcat(pci_device_detail.name_string, " USB controller");
							break;
					}
					break;
				case 0x4:
					strcat(pci_device_detail.name_string, " Fibre Channel");
					break;
				case 0x5:
					strcat(pci_device_detail.name_string, " SMBus");
					break;
				case 0x6:
					strcat(pci_device_detail.name_string, " InfiniBand");
					break;
				case 0x7:
					strcat(pci_device_detail.name_string, " IPMI Interface");
					break;
				case 0x8:
					strcat(pci_device_detail.name_string, " SERCOS Interface");
					break;
				case 0x9:
					strcat(pci_device_detail.name_string, " CANbus");
					break;
				default:
					strcat(pci_device_detail.name_string, " Serial Bus Controller");
					break;
			}
			break;
		case 0xd: // Wireless Controller
			switch (pci_device_detail.sub_class)
			{
				case 0x0:
					strcat(pci_device_detail.name_string, " iRDA controller");
					break;
				case 0x1:
					strcat(pci_device_detail.name_string, " Consumer IR controller");
					break;
				case 0x10:
					strcat(pci_device_detail.name_string, " RF controller");
					break;
				case 0x11:
					strcat(pci_device_detail.name_string, " Bluetooth");
					break;
				case 0x12:
					strcat(pci_device_detail.name_string, " Broadband");
					break;
				case 0x20:
					strcat(pci_device_detail.name_string, " Ethernet(802.11.a)");
					break;
				case 0x21:
					strcat(pci_device_detail.name_string, " Ethernet(802.11.b)");
					break;
				case 0x80:
					strcat(pci_device_detail.name_string, " wireless controller");
					break;
				default:
					strcat(pci_device_detail.name_string, " wireless controller");
					break;
			}
			break;
		case 0xe: // Intelligent IO Controller 
			switch (pci_device_detail.sub_class)
			{
				case 0x0:
					strcat(pci_device_detail.name_string, " Message FIFO");
					break;
				default:
					strcat(pci_device_detail.name_string, " IO controller");
					break;
			}
			break;
		case 0xf: // Satellite Communication
			switch (pci_device_detail.sub_class)
			{
				case 0x1:
					strcat(pci_device_detail.name_string, " TV Satellite");
					break;
				case 0x2:
					strcat(pci_device_detail.name_string, " Audio Satellite");
					break;
				case 0x3:
					strcat(pci_device_detail.name_string, " Voice Satellite");
					break;
				case 0x4:
					strcat(pci_device_detail.name_string, " Data Satellite");
					break;
				default:
					strcat(pci_device_detail.name_string, " Satallite");
					break;
			}
			break;
		case 0x10: // Encryption / Decryption controller
			switch (pci_device_detail.sub_class)
			{
				case 0x0:
					strcat(pci_device_detail.name_string, " Netword and computing en/decryption");
					break;
				case 0x1:
					strcat(pci_device_detail.name_string, " Entertainment en/decryption");
					break;
				case 0x80:
					strcat(pci_device_detail.name_string, " en/decryption");
					break;
				default:
					strcat(pci_device_detail.name_string, " en/decryption");
					break;
			}
			break;
		case 0x11: // Data accquisition signal processing controller
			switch (pci_device_detail.sub_class)
			{
				case 0x0:
					strcat(pci_device_detail.name_string, " DPIO modules");
					break;
				case 0x1:
					strcat(pci_device_detail.name_string, " Performance counters");
					break;
				case 0x10:
					strcat(pci_device_detail.name_string, " Time and frequency test/measurement");
					break;
				case 0x20:
					strcat(pci_device_detail.name_string, " Management card");
					break;
				case 0x80:
					strcat(pci_device_detail.name_string, " data/signal controller");
					break;
				default:
					strcat(pci_device_detail.name_string, " data/signal controller");
					break;
			}
			break;
		default: // Undefined base class
			strcat(pci_device_detail.name_string, " Undefiend Base Class");
			break;
	}
}


void acpi_signature_parser(unsigned int device)
{
	switch(memory_read(acpi_header_ba[device]))
	{
		case 0x43495041:	//APIC
			strcpy(acpi_header_detail.description, "Multiple APIC Description Table"); break;
		case 0x54524542:	//BERT
			strcpy(acpi_header_detail.description, "Boot Error Record Table"); break;
		case 0x54524742:	//BGRT
			strcpy(acpi_header_detail.description, "Boot Graphics Resource Table"); break;
		case 0x50455043:	//CPEP
			strcpy(acpi_header_detail.description, "Corrected Platform Error Polling Table"); break;
		case 0x54445344:	//DSDT
			strcpy(acpi_header_detail.description, "Differentiated System Description Table"); break;
		case 0x54444345:	//ECDT
			strcpy(acpi_header_detail.description, "Embedded Controller Boot Resource Table"); break;
		case 0x4a4e4945:	//EINJ
			strcpy(acpi_header_detail.description, "Error Injection Table"); break;
		case 0x54535245:	//ERST
			strcpy(acpi_header_detail.description, "Error Record Serialization Table"); break;
		case 0x50434146:	//FACP
			strcpy(acpi_header_detail.description, "Fixed ACPI Description Table"); break;
		case 0x53434146:	//FACS
			strcpy(acpi_header_detail.description, "Firmware ACPI Control Data"); break;
		case 0x54445046:	//FPDT
			strcpy(acpi_header_detail.description, "Firmware Performance Data Table"); break;
		case 0x54445447:	//GTDT
			strcpy(acpi_header_detail.description, "Generic Timer Description Table"); break;
		case 0x54534548:	//HEST
			strcpy(acpi_header_detail.description, "Hardware Error Source Table"); break;
		case 0x5443534d:	//MSCT
			strcpy(acpi_header_detail.description, "Maximun System Characteristics Table"); break;
		case 0x5453504d:	//MPST
			strcpy(acpi_header_detail.description, "Memory Power State Table"); break;
		case 0x784d454f:	//OEMx
			strcpy(acpi_header_detail.description, "OEM Specific Information Tables"); break;
		case 0x54544d50:	//PMTT
			strcpy(acpi_header_detail.description, "Platform Memory Topology Table"); break;
		case 0x54445350:	//PSDT
			strcpy(acpi_header_detail.description, "Persistent System Description Table"); break;
		case 0x46534152:	//RASF
			strcpy(acpi_header_detail.description, "ACPI RAS Feature Table"); break;
		case 0x54445352:	//RSDT
			strcpy(acpi_header_detail.description, "Root System Description Table"); break;
		case 0x54534253:	//SBST
			strcpy(acpi_header_detail.description, "Smart Battery Specification Table"); break;
		case 0x54494c53:	//SLIT
			strcpy(acpi_header_detail.description, "System Locality Distance Info Table"); break;
		case 0x54415253:	//SRAT
			strcpy(acpi_header_detail.description, "System Resource Affinity Table"); break;
		case 0x54445353:	//SSDT
			strcpy(acpi_header_detail.description, "Secondary System Description Table"); break;
		case 0x54445358:	//XSDT
			strcpy(acpi_header_detail.description, "Extended System Description Table"); break;
		case 0x544f4f42:	//BOOT
			strcpy(acpi_header_detail.description, "Simple Boot Flag Table"); break;
		case 0x54525343:	//CSRT
			strcpy(acpi_header_detail.description, "Core System Resource Table"); break;
		case 0x32474244:	//DBG2
			strcpy(acpi_header_detail.description, "Debug Port Table 2"); break;
		case 0x50474244:	//DBGP
			strcpy(acpi_header_detail.description, "Debug Port Table"); break;
		case 0x52414d44:	//DMAR
			strcpy(acpi_header_detail.description, "DMA Remapping Table"); break;
		case 0x4d545244:	//DRTM
			strcpy(acpi_header_detail.description, "Dynamic Root of Trust for Measurement"); break;
		case 0x54445445:	//ETDT
			strcpy(acpi_header_detail.description, "Event Timer Description Table"); break;
		case 0x54455048:	//HPET
			strcpy(acpi_header_detail.description, "IA-PC High Precision Event Timer Table");
			hpet_mmio_base_addr = memory_read(acpi_header_ba[device] + 0x2C); break;
		case 0x54464249:	//iBFT
			strcpy(acpi_header_detail.description, "iSCSI Boot Firmware Table"); break;
		case 0x53525649:	//IVRS
			strcpy(acpi_header_detail.description, "I/O Virtualization Reporting Structure"); break;
		case 0x4746434d:	//MCFG
			strcpy(acpi_header_detail.description, "PCIE Memory Mapped configuration"); 
			pcie_extended_base_address = memory_read(acpi_header_ba[device] + 0x2C); break;
		case 0x4948434d:	//MCHI
			strcpy(acpi_header_detail.description, "Management Controller Host Interface"); break;
		case 0x4d44534d:	//MSDM
			strcpy(acpi_header_detail.description, "Microsoft Data Management Table"); break;
		case 0x43494C53:	//SLIC
			strcpy(acpi_header_detail.description, "Software Listening Table Specification"); break;
		case 0x52435053:	//SPCR
			strcpy(acpi_header_detail.description, "Serial Port Console Redirection Table"); break;
		case 0x494D5053:	//SPMI
			strcpy(acpi_header_detail.description, "Several Platform Management Interface"); break;
		case 0x41504354:	//TCPA
			strcpy(acpi_header_detail.description, "Trusted Computing Platform Alliance"); break;
		case 0x324d5054:	//TPM2
			strcpy(acpi_header_detail.description, "Trusted Platform Module 2 Table"); break;
		case 0x49464555:	//UEFI
			strcpy(acpi_header_detail.description, "UEFI ACPI Data Table"); break;
		case 0x54454157:	//WAET
			strcpy(acpi_header_detail.description, "Windows ACPI Emulated Devices Table"); break;
		case 0x54414457:	//WDAT
			strcpy(acpi_header_detail.description, "Watch Dog Action Table"); break;
		case 0x54524457:	//WDRT
			strcpy(acpi_header_detail.description, "Watch Dog Resourece Table"); break;
		case 0x54425057:	//WPBT
			strcpy(acpi_header_detail.description, "Windows Platform Binary Table"); break;
		default : 			//Undefined
			strcpy(acpi_header_detail.description, "Undefined");break;
	}
}


void spd_parser(unsigned long * content)
{
	unsigned int spd_byte[256];
	unsigned int dword_no;
	unsigned int byte_no;
	unsigned long temp;
	float devidend,devisor;

	for (dword_no=0; dword_no<64; dword_no++)
	{
		temp = content[dword_no];
		for (byte_no=0; byte_no<4; byte_no++)
		{
			spd_byte[dword_no*4 + byte_no] = temp & 0xff;
			temp = temp >> 8;
		}
	}
	
	// DRAM deviece type
	switch (spd_byte[2] & 0xff)
	{
		case 0x1:
			strcpy(spd_info_parsered.dram_device_type,"Standard FPM DRAM");break;
		case 0x2:
			strcpy(spd_info_parsered.dram_device_type,"EDO");break;
		case 0x3:
			strcpy(spd_info_parsered.dram_device_type,"Pipelined Nibble");break;
		case 0x4:
			strcpy(spd_info_parsered.dram_device_type,"SDRAM");break;
		case 0x5:	
			strcpy(spd_info_parsered.dram_device_type,"ROM");break;
		case 0x6:
			strcpy(spd_info_parsered.dram_device_type,"DDR SGRAM");break;
		case 0x7:
			strcpy(spd_info_parsered.dram_device_type,"DDR SDRAM");break;
		case 0x8:
			strcpy(spd_info_parsered.dram_device_type,"DDR2 SDRAM");break;
		case 0x9:
			strcpy(spd_info_parsered.dram_device_type,"DDR2 SDRAM FB-DIMM");break;
		case 0xa:
			strcpy(spd_info_parsered.dram_device_type,"DDR2 SDRAM FB-DIMM");break;
		case 0xb:
			strcpy(spd_info_parsered.dram_device_type,"DDR3 SDRAM");break;
		default:
			strcpy(spd_info_parsered.dram_device_type,"Reserved");break;
	}

	// Module type
	switch (spd_byte[3] & 0xf)
	{
		case 0x0:
			strcpy(spd_info_parsered.module_type,"Undefined");break;
		case 0x1:
			strcpy(spd_info_parsered.module_type,"RDIMM");break;
		case 0x2:
			strcpy(spd_info_parsered.module_type,"UDIMM");break;
		case 0x3:
			strcpy(spd_info_parsered.module_type,"SO-DIMM");break;
		case 0x4:
			strcpy(spd_info_parsered.module_type,"Micro-DIMM");break;
		case 0x5:
			strcpy(spd_info_parsered.module_type,"Mini-RDIMM");break;
		case 0x6:
			strcpy(spd_info_parsered.module_type,"Mini-UDIMM");break;
		case 0x7:
			strcpy(spd_info_parsered.module_type,"Mini-CDIMM");break;
		case 0x8:
			strcpy(spd_info_parsered.module_type,"72b-SO-UDIMM");break;
		case 0x9:
			strcpy(spd_info_parsered.module_type,"72b-SO-RDIMM");break;
		case 0xa:
			strcpy(spd_info_parsered.module_type,"72b-SO-CDIMM");break;
		default:
			strcpy(spd_info_parsered.module_type,"Reserved");break;
	}

	// Module Nominal Voltage
	switch (spd_byte[6] & 0x7)
	{
		case 0x0:
			strcpy(spd_info_parsered.voltage,"1.5V");break;
		case 0x2:
			strcpy(spd_info_parsered.voltage,"1.35V,1.5V");break;
		case 0x6: 
			strcpy(spd_info_parsered.voltage,"1.2xV,1.35V,1.5V");break;
		case 0x7:
			strcpy(spd_info_parsered.voltage,"1.2xV,1.35V");break;
		default : break;
	}

	// SDRAM Density
	switch (spd_byte[4] & 0xf)
	{
		case 0x0:
			spd_info_parsered.sdram_capacity = 0.25;break;
		case 0x1:
			spd_info_parsered.sdram_capacity = 0.5;break;
		case 0x2:
			spd_info_parsered.sdram_capacity = 1;break;
		case 0x3:
			spd_info_parsered.sdram_capacity = 2;break;
		case 0x4:
			spd_info_parsered.sdram_capacity = 4;break;
		case 0x5:
			spd_info_parsered.sdram_capacity = 8;break;
		case 0x6:
			spd_info_parsered.sdram_capacity = 16;break;
		default : break;			
	}

	// Module Organization
	switch (spd_byte[7] & 0x7)
	{
		case 0x0:
			spd_info_parsered.sdram_width = 4;break;
		case 0x1:
			spd_info_parsered.sdram_width = 8;break;
		case 0x2:
			spd_info_parsered.sdram_width = 16;break;
		case 0x3:
			spd_info_parsered.sdram_width = 32;break;
		default : break;	
	}

	spd_byte[7] = spd_byte[7] >> 3;

	switch (spd_byte[7] & 0x7)
	{
		case 0x0:
			spd_info_parsered.ranks = 1;break;
		case 0x1:
			spd_info_parsered.ranks = 2;break;
		case 0x2:
			spd_info_parsered.ranks = 3;break;
		case 0x3:
			spd_info_parsered.ranks = 4;break;
		default : break;	
	}

	// Module Memory Bus Width
	switch (spd_byte[8] & 0x7)
	{
		case 0x0:
			spd_info_parsered.primary_bus_width = 8;break;
		case 0x1:
			spd_info_parsered.primary_bus_width = 16;break;
		case 0x2:
			spd_info_parsered.primary_bus_width = 32;break;
		case 0x3:
			spd_info_parsered.primary_bus_width = 64;break;
		default : break;		
	}

	spd_byte[8] = spd_byte[8] >> 3;

	switch (spd_byte[8] & 0x7)
	{
		case 0x0:
			spd_info_parsered.total_bus_width = 64;break;
		case 0x1:
			spd_info_parsered.total_bus_width = 72;break;
		default : break;
	}

	// Module total capacity
	spd_info_parsered.total_capacity = spd_info_parsered.sdram_capacity / 8 * spd_info_parsered.primary_bus_width / spd_info_parsered.sdram_width * spd_info_parsered.ranks;

	// Fine Timebase
	devisor = spd_byte[9] & 0xf;
	spd_byte[9] = spd_byte[9] >> 4;
	devidend = spd_byte[9] & 0xf;
	spd_info_parsered.ftb = devidend / devisor;

	// Medium Timebase
	devidend = spd_byte[10] & 0xff;
	devisor = spd_byte[11] & 0xff;
	spd_info_parsered.mtb = devidend / devisor;

	// Speed
	spd_info_parsered.mtb_hex = spd_byte[12] & 0xff;
	spd_info_parsered.ftb_hex = spd_byte[34] & 0xff;
	spd_info_parsered.tck = spd_info_parsered.mtb_hex * spd_info_parsered.mtb + spd_info_parsered.ftb_hex * spd_info_parsered.ftb;

	spd_info_parsered.speed = 1 / spd_info_parsered.tck; // There is a bug,"Floating point error:devide by 0"
	spd_info_parsered.speed = spd_info_parsered.speed * 2000;

	// Module Manufacturer ID
	spd_info_parsered.module_manufacturer_lsb = spd_byte[117] & 0x7f;
	spd_info_parsered.module_manufacturer_msb = spd_byte[118] & 0xff;

	// Module Serial Number
	spd_info_parsered.module_serial_number = 0;
	for (byte_no=122; byte_no<126; byte_no++)
	{
		spd_info_parsered.module_serial_number = spd_info_parsered.module_serial_number << 8;
		spd_info_parsered.module_serial_number &= 0xffffff00;
		spd_info_parsered.module_serial_number |= (spd_byte[byte_no] & 0xff);
	}

	// Module Part Number
	dword_no = 0;
	for (byte_no=128; byte_no<146; byte_no++)
	{
		spd_info_parsered.module_part_number[dword_no] = spd_byte[byte_no] & 0xff;
		dword_no++;	
	}

	// DRAM Manufacturer ID
	spd_info_parsered.dram_manufacturer_lsb = spd_byte[148] & 0x7f;
	spd_info_parsered.dram_manufacrurer_msb = spd_byte[149] & 0xff;
	
	// Raw Card
	switch (spd_byte[62] & 0x60)
	{
		case 0x0:
			strcpy(spd_info_parsered.raw_card,"Rev 0 - ");break;
		case 0x20:
			strcpy(spd_info_parsered.raw_card,"Rev 1 - ");break;
		case 0x40:
			strcpy(spd_info_parsered.raw_card,"Rev 2 - ");break;
		case 0x60:
			strcpy(spd_info_parsered.raw_card,"Rev 3 - ");break;
	}
	if ((spd_byte[62] & 0x80) == 0)
	{
		switch (spd_byte[62] & 0x1f)
		{
			case 0x0:
				strcat(spd_info_parsered.raw_card,"A");break;
			case 0x1:
				strcat(spd_info_parsered.raw_card,"B");break;
			case 0x2:
				strcat(spd_info_parsered.raw_card,"C");break;
			case 0x3:
				strcat(spd_info_parsered.raw_card,"D");break;
			case 0x4:
				strcat(spd_info_parsered.raw_card,"E");break;
			case 0x5:
				strcat(spd_info_parsered.raw_card,"F");break;
			case 0x6:
				strcat(spd_info_parsered.raw_card,"G");break;
			case 0x7:
				strcat(spd_info_parsered.raw_card,"H");break;
			case 0x8:
				strcat(spd_info_parsered.raw_card,"J");break;
			case 0x9:
				strcat(spd_info_parsered.raw_card,"K");break;
			case 0xA:
				strcat(spd_info_parsered.raw_card,"L");break;
			case 0xB:
				strcat(spd_info_parsered.raw_card,"M");break;
			case 0xC:
				strcat(spd_info_parsered.raw_card,"N");break;	
			case 0xD:
				strcat(spd_info_parsered.raw_card,"P");break;
			case 0xE:
				strcat(spd_info_parsered.raw_card,"R");break;
			case 0xF:
				strcat(spd_info_parsered.raw_card,"T");break;
			case 0x10:
				strcat(spd_info_parsered.raw_card,"U");break;
			case 0x11:
				strcat(spd_info_parsered.raw_card,"V");break;
			case 0x12:
				strcat(spd_info_parsered.raw_card,"W");break;	
			case 0x13:
				strcat(spd_info_parsered.raw_card,"Y");break;	
			case 0x14:
				strcat(spd_info_parsered.raw_card,"AA");break;	
			case 0x15:
				strcat(spd_info_parsered.raw_card,"AB");break;
			case 0x16:
				strcat(spd_info_parsered.raw_card,"AC");break;
			case 0x17:
				strcat(spd_info_parsered.raw_card,"AD");break;	
			case 0x18:
				strcat(spd_info_parsered.raw_card,"AE");break;
			case 0x19:
				strcat(spd_info_parsered.raw_card,"AF");break;
			case 0x1A:
				strcat(spd_info_parsered.raw_card,"AG");break;	
			case 0x1B:
				strcat(spd_info_parsered.raw_card,"AH");break;	
			case 0x1C:
				strcat(spd_info_parsered.raw_card,"AJ");break;
			case 0x1D:
				strcat(spd_info_parsered.raw_card,"AK");break;	
			case 0x1E:
				strcat(spd_info_parsered.raw_card,"AL");break;
			default : break;	
		}
	}
	else 
	{
		switch (spd_byte[62] & 0x1f)
		{
			case 0x0:
				strcat(spd_info_parsered.raw_card,"AM");break;
			case 0x1:
				strcat(spd_info_parsered.raw_card,"AN");break;
			case 0x2:
				strcat(spd_info_parsered.raw_card,"AP");break;
			case 0x3:
				strcat(spd_info_parsered.raw_card,"AR");break;
			case 0x4:
				strcat(spd_info_parsered.raw_card,"AT");break;
			case 0x5:
				strcat(spd_info_parsered.raw_card,"AU");break;
			case 0x6:
				strcat(spd_info_parsered.raw_card,"AV");break;
			case 0x7:
				strcat(spd_info_parsered.raw_card,"AW");break;
			case 0x8:
				strcat(spd_info_parsered.raw_card,"AY");break;
			case 0x9:
				strcat(spd_info_parsered.raw_card,"BA");break;
			case 0xA:
				strcat(spd_info_parsered.raw_card,"BB");break;
			case 0xB:
				strcat(spd_info_parsered.raw_card,"BC");break;
			case 0xC:
				strcat(spd_info_parsered.raw_card,"BD");break;	
			case 0xD:
				strcat(spd_info_parsered.raw_card,"BE");break;
			case 0xE:
				strcat(spd_info_parsered.raw_card,"BF");break;
			case 0xF:
				strcat(spd_info_parsered.raw_card,"BG");break;
			case 0x10:
				strcat(spd_info_parsered.raw_card,"BH");break;
			case 0x11:
				strcat(spd_info_parsered.raw_card,"BJ");break;
			case 0x12:
				strcat(spd_info_parsered.raw_card,"BK");break;	
			case 0x13:
				strcat(spd_info_parsered.raw_card,"BL");break;	
			case 0x14:
				strcat(spd_info_parsered.raw_card,"BM");break;	
			case 0x15:
				strcat(spd_info_parsered.raw_card,"BN");break;
			case 0x16:
				strcat(spd_info_parsered.raw_card,"BP");break;
			case 0x17:
				strcat(spd_info_parsered.raw_card,"BR");break;	
			case 0x18:
				strcat(spd_info_parsered.raw_card,"BT");break;
			case 0x19:
				strcat(spd_info_parsered.raw_card,"BU");break;
			case 0x1A:
				strcat(spd_info_parsered.raw_card,"BV");break;	
			case 0x1B:
				strcat(spd_info_parsered.raw_card,"BW");break;	
			case 0x1C:
				strcat(spd_info_parsered.raw_card,"BY");break;
			case 0x1D:
				strcat(spd_info_parsered.raw_card,"CA");break;	
			case 0x1E:
				strcat(spd_info_parsered.raw_card,"CB");break;
			default : break;	
		}
	}
}


void cpuid_parser(void)
{
	unsigned long dword_temp;
	unsigned long cache_way;
	unsigned long cache_partition;
	unsigned long cache_line_size;
	unsigned long cache_set;
	unsigned int  dword_no,byte_no;
	unsigned int  a;

	// Vendor ID parser
	a = 0;
	dword_temp = cpuid_info[0][1];
	for(byte_no=0; byte_no<4; byte_no++)
	{
		cpuid_info_parsered.vendor_id[a] = dword_temp & 0xff;
		dword_temp = dword_temp >> 8;
		a++;
	}
	dword_temp = cpuid_info[0][3];
	for(byte_no=0; byte_no<4; byte_no++)
	{
		cpuid_info_parsered.vendor_id[a] = dword_temp & 0xff;
		dword_temp = dword_temp >> 8;
		a++;
	}
	dword_temp = cpuid_info[0][2];
	for(byte_no=0; byte_no<4; byte_no++)
	{
		cpuid_info_parsered.vendor_id[a] = dword_temp & 0xff;
		dword_temp = dword_temp >> 8;
		a++;
	} 

	// Family, Model, Step
	cpuid_info_parsered.fms = cpuid_info[0][4];

	// Count
	dword_temp = cpuid_info[0][5];
	cpuid_info_parsered.count = (dword_temp >> 16) & 0xff;

	// Name string
	for (dword_no=0; dword_no<12; dword_no++)
	{
		dword_temp = cpuid_info[1][8+dword_no];
		for (byte_no=0; byte_no<4; byte_no++)
		{
			cpuid_info_parsered.name_string[dword_no*4+byte_no] = dword_temp & 0xff;
			dword_temp = dword_temp >> 8;
		}
	}

	// L1 L2 L3 Cache size
	if (host_vendor_id == 0x8086)
	{
		unsigned long temp[3];
		for (a=0; a<3; a++)
		{
			cpuid_read (cpuid_info[0] + 16, 0x4, a+1);
			dword_temp = cpuid_info[0][17];
			cache_line_size = dword_temp & 0xfff;
			cache_partition = (dword_temp >> 12) & 0x3ff;
			cache_way = (dword_temp >> 22) & 0x3ff;
			cache_set = cpuid_info[0][18];
			temp[a] = (cache_way + 1) * (cache_partition + 1) * (cache_line_size + 1) * (cache_set + 1);
		}
		cpuid_info_parsered.cache_1_size = temp[0] >> 10;
		cpuid_info_parsered.cache_2_size = temp[1] >> 10;
		cpuid_info_parsered.cache_3_size = temp[2] >> 20;
	}
	else if (host_vendor_id == 0x1106)
	{
		dword_temp = cpuid_info[1][22];
		cpuid_info_parsered.cache_1_size = (dword_temp >> 24) & 0xff;
		dword_temp = cpuid_info[1][26];
		cpuid_info_parsered.cache_2_size = (dword_temp >> 16) & 0xffff;
		cpuid_info_parsered.cache_3_size = 0;
	}

	// Physical and virtual address size
	dword_temp = cpuid_info[1][32];
	cpuid_info_parsered.phy_addr_size = dword_temp & 0xff;
	dword_temp = dword_temp >> 8;
	cpuid_info_parsered.virt_addr_size = dword_temp & 0xff;

	// Feature flags
	cpuid_info_parsered.feature_flags[0] = cpuid_info[0][6];
	cpuid_info_parsered.feature_flags[1] = cpuid_info[0][7];
	
	// Extended feature flags
	cpuid_info_parsered.extended_feature_flags[0] = cpuid_info[1][6];
	cpuid_info_parsered.extended_feature_flags[1] = cpuid_info[1][7];
	
}


void pci_config_space_parser(void)
{
	unsigned int pci_byte[256];
	unsigned int dword_no;
	unsigned int byte_no;
	unsigned long dword_temp;
	unsigned int cap_ptr_temp;
	unsigned int cap_id;

	for (dword_no=0; dword_no<64; dword_no++)
	{
		dword_temp = data_stotage_array_256b[dword_no];
		for (byte_no=0; byte_no<4; byte_no++)
		{
			pci_byte[dword_no*4+byte_no] = dword_temp & 0xff;
			dword_temp =  dword_temp >> 8;
		}
	}

	// Header Type
	pci_device_detail.header_type = pci_byte[0xE] & 0x7f;

	// Revision ID
	pci_device_detail.rev_id = pci_byte[0x8] & 0xff;

	// Interrupt Line
	pci_device_detail.int_line = pci_byte[0x3c] & 0xff;

	// Interrupt Pin
	pci_device_detail.int_pin = pci_byte[0x3d] & 0xff;

	// Address Maps
	for (dword_no=0; dword_no<6; dword_no++)
	{
		dword_temp = data_stotage_array_256b[dword_no + 4];

		if (dword_temp == 0)
		{
			pci_device_detail.address_map_type[dword_no] = 2;
		}
		else
		{
			pci_device_detail.address_map_type[dword_no] = dword_temp & 0x1;
			switch (pci_device_detail.address_map_type[dword_no])
			{
				// Memory Map IO	
				case 0 : 
					pci_device_detail.address_map_addr_length[dword_no] = (dword_temp & 0x6) / 2;
					switch(pci_device_detail.address_map_addr_length[dword_no])
					{
						case 0 : 
							pci_device_detail.address_map_addr_length[dword_no] = 32;
							break;
						case 2 : 
							pci_device_detail.address_map_addr_length[dword_no] = 64;
							break;
						default : break;						
					}
					pci_device_detail.address_map_addr[dword_no] = dword_temp & 0xfffffff0;
					break;	

				// IO
				case 1 : 
					pci_device_detail.address_map_addr[dword_no] = dword_temp & 0xfffffffc;
					pci_device_detail.address_map_addr_length[dword_no] = 32;
					break;
				default : break;				
			}
		}
	}

	// Capability List
	pci_device_detail.capability_list = pci_byte[6] & 0x10;

	// Capability Pointer
	if (pci_device_detail.capability_list == 0x10)
	{
		cap_ptr_temp = pci_byte[0x34];
		pci_device_detail.device_type = 0;
		while (cap_ptr_temp != 0)
		{
			cap_id = pci_byte[cap_ptr_temp];

			// PCIE capability parser
			if (cap_id == 0x10)
			{
				// PCIE link capability
				pci_device_detail.device_type = 1;
				pci_device_detail.pcie_max_link_speed = pci_byte[cap_ptr_temp + 0xC] & 0xF;
				pci_device_detail.pcie_max_link_width = (pci_byte[cap_ptr_temp + 0xC] & 0xF0) >> 4;
				pci_device_detail.pcie_max_link_width &= 0xF;
				pci_device_detail.pcie_max_link_width += (pci_byte[cap_ptr_temp + 0xD] & 0x3) << 4;
				// PCIE link status
				pci_device_detail.pcie_current_link_speed = pci_byte[cap_ptr_temp + 0x12] & 0xF;
				pci_device_detail.pcie_current_link_width = (pci_byte[cap_ptr_temp + 0x12] & 0xF0) >> 4;
				pci_device_detail.pcie_current_link_width &= 0xF;
				pci_device_detail.pcie_current_link_width += (pci_byte[cap_ptr_temp + 0x13] & 0x3) << 4;
			}
			cap_ptr_temp ++;
			cap_ptr_temp = pci_byte[cap_ptr_temp];
		}
	}
}


void identify_word_parser(void)
{
	unsigned int  dword_no;
	unsigned int  word_no;
	unsigned long dword_temp;
	unsigned int  word_temp[256];

	for (dword_no=0; dword_no<128; dword_no++)
	{
		dword_temp = identify_word[dword_no];
		for (word_no=0; word_no<2; word_no++)
		{
			word_temp[dword_no*2 + word_no] = dword_temp & 0xffff;
			dword_temp = dword_temp >> 16;
		}
	}

	identify_device_information.cylinder = word_temp[54];
	identify_device_information.headers  = word_temp[55];
	identify_device_information.sectors  = word_temp[56];
	identify_device_information.queue_depth = word_temp[75];

	switch(word_temp[76] & 0xf)
	{
		case 0x2: identify_device_information.sata_speed = 1; break;
		case 0x6: identify_device_information.sata_speed = 2; break;
		case 0xe: identify_device_information.sata_speed = 3; break;
	}

	for (word_no=0; word_no<10; word_no++)
	{
		identify_device_information.serial_no[word_no * 2 + 1] = word_temp[10 + word_no] & 0xff;
		identify_device_information.serial_no[word_no * 2] = (word_temp[10 + word_no] >> 8) & 0xff;
	}

	for (word_no=0; word_no<20; word_no++)
	{
		identify_device_information.model_number[word_no * 2 + 1] = word_temp[27 + word_no] & 0xff;
		identify_device_information.model_number[word_no * 2] = (word_temp[27 + word_no] >> 8) & 0xff;
	}
}
 
#endif
