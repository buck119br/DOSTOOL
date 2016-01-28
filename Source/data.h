#ifndef __DATA_H
#define __DATA_H

// IO Port write by DWORD
void out_port (unsigned int destination_addr, unsigned long content)
{
	asm pushad;
	asm mov dx,destination_addr;
	asm lea bx,content;
	asm mov eax,[bx];
	asm out dx,eax;
	asm popad;
}


// IO Port read by DWORD
unsigned long in_port (unsigned int destination_addr)
{
	unsigned long ret_val;
	
	asm pushad;
	asm mov dx,destination_addr;
	asm lea bx,ret_val;
	asm in  eax,dx;
	asm mov [bx],eax;
	asm popad;

	return ret_val;
}


// IO Port write by WORD
void out_port_word (unsigned int destination_addr, unsigned int content)
{
	asm pushad;
	asm mov dx,destination_addr;
	asm lea bx,content;
	asm mov ax,[bx];
	asm out dx,ax;
	asm popad;
}


// IO Port read by WORD
unsigned int in_port_word (unsigned int destination_addr)
{
	unsigned int ret_val;
	
	asm pushad;
	asm mov dx,destination_addr;
	asm lea bx,ret_val;
	asm in  ax,dx;
	asm mov [bx],ax;
	asm popad;

	return ret_val;
}


// IO Port write by BYTE
void out_port_byte(unsigned int destination_addr, unsigned int content)
{
	asm pushad;
	asm mov dx,destination_addr;
	asm lea bx,content;
	asm mov al,[bx];
	asm out dx,al;
	asm popad;
}


// IO Port read by BYTE
unsigned int in_port_byte(unsigned int destination_addr)
{
	unsigned int ret_val;
	
	asm pushad;
	asm mov dx,destination_addr;
	asm lea bx,ret_val;
	asm in  al,dx;
	asm mov [bx],al;
	asm popad;

	return ret_val;
}


// Scan PCI config space HEADER, not the whole 256-byte configuration space
void scan_bdf(void)
{
	unsigned long bus;
	unsigned long device;
	unsigned long function;
	unsigned long reg_addr;
	unsigned long reg_ret;

	max_pci_dev_quantity = 0;
	max_hba_quantity = 0;

	// 256 buses, 32 devices, 8 functions, traversal
	for (bus=0; bus<256; bus++)
		for(device=0; device<32; device++)
			for(function=0; function<8; function++)
			{
				reg_addr = 0x80000000 + (bus << 16) + (device << 11) + (function << 8);
				out_port(PCI_INDEX, reg_addr);
				reg_ret = in_port (PCI_DATA);
				if (reg_ret != 0xffffffff)
				{
					pci_device_bdf[max_pci_dev_quantity].bus = bus;
					pci_device_bdf[max_pci_dev_quantity].device = device;
					pci_device_bdf[max_pci_dev_quantity].function = function;

					// Judge whether the device is single-function device
					if (function == 0)
					{
						reg_addr = (reg_addr & 0xfffffff0) + 0xc;
						out_port (PCI_INDEX, reg_addr);
						reg_ret = in_port (PCI_DATA);
						reg_ret = reg_ret >> 16;
						if ((reg_ret & 0x80) == 0)
						{
							function = 8;
						}
					}

					pci_dev_header_scan(max_pci_dev_quantity);

					// Judge whether device is IDE/RAID/AHCI controller
					if (((pci_device_detail.base_class == 1) && (pci_device_detail.sub_class == 1))
						|| ((pci_device_detail.base_class == 1) && (pci_device_detail.sub_class == 4))
						|| ((pci_device_detail.base_class == 1) && (pci_device_detail.sub_class == 6)))
					{
						// Controller working mode
						if (pci_device_detail.sub_class == 1)
						{
							reg_addr = 0x80000000 + (bus << 16) + (device << 11) + (function << 8);
							out_port(PCI_INDEX, reg_addr + 0x8);
							if (((in_port(PCI_DATA) >> 8) & 0x1) == 0)
							{
								host_bus_adapter[max_hba_quantity].mode = 0;
								ide_pci_base[max_hba_quantity] = reg_addr;
							}
							else if (((in_port(PCI_DATA) >> 8) & 0x1) == 1)
							{
								host_bus_adapter[max_hba_quantity].mode = 1;
								ide_pci_base[max_hba_quantity] = reg_addr;
							}
						}
						else 
						{
							host_bus_adapter[max_hba_quantity].mode = 2;
						}
						
						// Native HBA or PCIE Card
						if (bus == 0)
						{
							host_bus_adapter[max_hba_quantity].nhf = 1;
						}
						else 
						{
							host_bus_adapter[max_hba_quantity].nhf = 0;
						}
							
						host_bus_adapter[max_hba_quantity].vid = pci_device_detail.vendor_id;
						reg_addr = 0x80000000 + (bus << 16) + (device << 11) + (function << 8) + 0x24;
						out_port(PCI_INDEX, reg_addr);
						host_bus_adapter[max_hba_quantity].abar = in_port (PCI_DATA) & 0xfffffff0;
						max_hba_quantity ++;
					}


					max_pci_dev_quantity ++;
				}				
			}
	max_pci_dev_quantity --;
	max_hba_quantity --;
}


void pci_dev_header_scan(unsigned int device_no)
{
	unsigned long reg_addr;
	unsigned long reg_ret;

	reg_addr = 0x80000000 + (pci_device_bdf[device_no].bus << 16) + (pci_device_bdf[device_no].device << 11) + (pci_device_bdf[device_no].function << 8);
	out_port(PCI_INDEX, reg_addr);
	reg_ret = in_port (PCI_DATA);

	// Vendor ID & Device ID
	pci_device_detail.vendor_id = reg_ret & 0xffff;
	pci_device_detail.device_id = (reg_ret >> 16) & 0xffff;

	// Host Vendor ID
	if ((pci_device_bdf[device_no].bus == 0) && (pci_device_bdf[device_no].device == 0) && (pci_device_bdf[device_no].function == 0))
	{
		host_vendor_id = pci_device_detail.vendor_id;
	}

	// Class Code
	reg_addr += 0x8;
	out_port (PCI_INDEX, reg_addr);
	reg_ret = in_port (PCI_DATA);
	reg_ret = reg_ret >> 8;
	pci_device_detail.interface_class = reg_ret & 0xff;
	reg_ret = reg_ret >> 8;
	pci_device_detail.sub_class = reg_ret & 0xff;
	reg_ret = reg_ret >> 8;
	pci_device_detail.base_class = reg_ret & 0xff;

	bdf_parser ();
}


void pci_config_space_scan (unsigned int device_no)
{
	unsigned long reg_addr;
	unsigned long mem_base_addr;
	unsigned int  dword_no;

	pci_dev_header_scan(device_no);

	// PCI Configuration Space
	reg_addr = 0x80000000 + (pci_device_bdf[device_no].bus << 16) + (pci_device_bdf[device_no].device << 11) + (pci_device_bdf[device_no].function << 8);
	for (dword_no=0; dword_no<64; dword_no++)
	{
		out_port (PCI_INDEX, reg_addr);
		data_stotage_array_256b[dword_no] = in_port (PCI_DATA);
		reg_addr += 4;
	}
	strcpy(content_name_string_zone2, pci_device_detail.name_string);
	data_storage_addr_table = data_stotage_array_256b;
	pci_config_space_parser ();	

	// PCIE Extended Configuration Space
	if (pcie_page_counter > 0)
	{
		mem_base_addr = pcie_extended_base_address + (pci_device_bdf[device_no].bus << 20) + (pci_device_bdf[device_no].device << 15) + (pci_device_bdf[device_no].function << 12) + pcie_page_counter * 256;
		for (dword_no=0; dword_no<64; dword_no++)
		{	
			data_stotage_array_256b[dword_no] = 0;
			data_stotage_array_256b[dword_no] = memory_read(mem_base_addr + dword_no * 4);
		}
	}
	strcpy(content_name_string_zone2, pci_device_detail.name_string);
	data_storage_addr_table = data_stotage_array_256b;
}


// Root System Description Pointer Location
unsigned long rsdp_research(void)
{
	unsigned long addr;

	// EBDA 1KB
	addr = rsdp_locate(0x40E, 0x40E + 0x400);
	// BIOS read-onlu memory
	if (addr == 0)
	{
		addr = rsdp_locate(0xE0000, 0xFFFFF);
	}

	return addr;
}

// Search for "RSD PTR "
unsigned long rsdp_locate(unsigned long start_addr,unsigned long end_addr)
{
	unsigned long addr;
	char signature[9]={"RSD PTR "};
	unsigned int byte_no;
	unsigned int match_count;

	for (addr=start_addr; addr<end_addr; addr=addr+16)
	{
		match_count = 0;
		for (byte_no=0; byte_no<8; byte_no++)
		{
			if (memory_read_byte(addr + byte_no) == signature[byte_no])
			{
				match_count++;
			}
			if (match_count == 8)
			{
				return addr;
			}
		}
	}

	return 0;
}


// Scan XSDT & other ACPI Description Table header
void acpi_header_ba_scan(void)
{
	unsigned int table_no;

	// XSDT Address locate
	rsdp_base_address = rsdp_research();
	xsdt_base_address = memory_read(rsdp_base_address + 24);
	max_acpi_table_quantity = (memory_read(xsdt_base_address + 4) - 36) / 8;

	// XSDT header base address	
	acpi_header_ba[0] = xsdt_base_address;
	
	// ACPI table header base address
	for (table_no=1; table_no<max_acpi_table_quantity+1; table_no++)
	{
		acpi_header_ba[table_no] = memory_read(xsdt_base_address + 36 + 8 * (table_no - 1));		
	}
}


void acpi_header_detail_scan(unsigned int table_no)
{
	unsigned int byte_no;

	for(byte_no=0; byte_no<4; byte_no++)
	{
		acpi_header_detail.signature[byte_no] = memory_read_byte(acpi_header_ba[table_no] + byte_no);
	}
	acpi_header_detail.length = memory_read(acpi_header_ba[table_no] + 4);

	acpi_signature_parser(table_no);
}


// Whole ACPI Description Table Scan
void acpi_table_scan(unsigned int table_no)
{
	unsigned long addr;
	unsigned long start_addr;
	unsigned long table_length;

	acpi_header_detail_scan(table_no);

	start_addr = acpi_header_ba[table_no] + acpi_table_page_counter * 256;

	// Set table length, or storage table will overflow
	if (acpi_header_detail.length >= 256 * (acpi_table_page_counter + 1))
	{
		table_length = 256;
	}
	else 
	{
		table_length = acpi_header_detail.length - 256 * acpi_table_page_counter;
	}

	// All set to 0, because the table length is not always 256 bytes
	for (addr=0; addr<64; addr++)
	{
		data_stotage_array_256b[addr] = 0;
	}

	// Table scan
	for (addr=0; addr<table_length; addr=addr+4)
	{
		data_stotage_array_256b[addr / 4] = memory_read(start_addr + addr);
	}

	strcpy(content_name_string_zone2, "ACPI Description Table");
	data_storage_addr_table = data_stotage_array_256b;
}


// Memory Read by DWORD
unsigned long memory_read (unsigned long addr)
{
	unsigned long memret = 0;

	asm pushad;
	_asm{ 
		mov edi,addr;
		mov ax,0;	
		mov fs,ax;
		mov eax,fs:[edi];
		lea ebx,memret;
		mov [ebx],eax;
	}
	asm popad;
	
	return memret;
}


// Memory Write by DWORD
void memory_write (unsigned long addr, unsigned long content)
{	
	asm pushad;               // zhong da shou huo:  must do pushad and popad, or you will be fucked 
	_asm{
		mov edi, addr;
		mov ax,0;
		mov fs,ax;
		mov eax,content;
		mov fs:[edi],eax;
	}
	asm popad;
}


// Memory Read by BYTE
unsigned int memory_read_byte(unsigned long addr)
{
	unsigned int memret = 0;

	asm pushad;
	_asm{ 
		mov edi,addr;
		mov ax,0;	
		mov fs,ax;
		mov ax,fs:[edi];
		lea bx,memret;
		mov [bx],ax;
	}
	asm popad;

	memret = memret & 0xff;
	return memret;
}


// Memory Write by BYTE
void memory_write_byte(unsigned long addr,unsigned long content)
{
	content = content & 0xff;
	asm pushad;               // zhong da shou huo:  must do pushad and popad, or you will be fucked 
	_asm{
		mov edi, addr;
		mov ax,0;
		mov fs,ax;
		mov ax,content;
		mov fs:[edi],ax;
	}
	asm popad;
}


// Memory Read 256 bytes from address base_addr
void memory_array_read(unsigned long base_addr)
{
	unsigned int dword_no;
	unsigned int byte_no;
	unsigned int a;
	unsigned long mem_ret;

	memory_base_address = base_addr;
	for (dword_no=0; dword_no<64; dword_no++)
	{
		data_stotage_array_256b[dword_no] = 0;
		for (byte_no=0; byte_no<4; byte_no++)
		{
			mem_ret = memory_read (base_addr);
			mem_ret = mem_ret & 0xff;
			for(a=0; a<byte_no; a++)
			{
				mem_ret = mem_ret << 8;
			}
			data_stotage_array_256b[dword_no] += mem_ret;
			base_addr ++;
		}
	}
	strcpy(content_name_string_zone2, "Memory");
	data_storage_addr_table = data_stotage_array_256b;
}


// CPUID 0X00000000 0X80000000 0XC0000000 traversal
void cpuid_scan(void)
{
	unsigned int  function_type;
	unsigned long dword;
	unsigned long function_code;
	unsigned long cpuret[4];
	unsigned int  table_length;
	unsigned int  max_input_value;

	if (host_vendor_id == 0x1106)
	{
		table_length = 3;
	}
	else if (host_vendor_id == 0x8086)
	{
		table_length = 2;
	}
	
	for (function_type=0; function_type<table_length; function_type++)
	{		
		switch (function_type)
		{
			case 0: function_code = 0; break;
			case 1: function_code = 0x80000000; break;
			case 2: function_code = 0xC0000000; break;
			default: break;
		}

		cpuid_read(cpuret, function_code, 0);
		max_input_value = cpuret[0] & 0xf;

		for (dword=0; dword<16; dword++)
		{	
			cpuid_info[function_type][dword*4]   = 0;
			cpuid_info[function_type][dword*4+1] = 0;
			cpuid_info[function_type][dword*4+2] = 0;
			cpuid_info[function_type][dword*4+3] = 0;
		}
		
		for (dword=0; dword<max_input_value+1; dword++)
		{	
			cpuid_read(cpuret, function_code+dword, 0); 
			cpuid_info[function_type][dword*4]   = cpuret[0];
			cpuid_info[function_type][dword*4+1] = cpuret[1];
			cpuid_info[function_type][dword*4+2] = cpuret[2];
			cpuid_info[function_type][dword*4+3] = cpuret[3];
		}
	}

	cpuid_parser ();
	data_storage_addr_table = cpuid_info[content_number - 100];
	strcpy(content_name_string_zone2, "CPUID");
}


// Single function number CPUID Read
void cpuid_read(unsigned long *cpuid_pt, unsigned long cpuid_func, unsigned long para)
{
	cpuid_pt[0] = cpuid_func;
	cpuid_pt[2] = para;

	asm pushad;

	_asm{
		mov di, cpuid_pt;
    	db 0x66; mov cx, [di+8];    
    	db 0x66; mov ax, [di];    	
    	db 0x66; xor dx, dx;   		
    	db 0x66; xor bx, bx; 	    
    	db 0x0f; db 0xa2;    		
    	db 0x66; mov [di], ax;    	
    	db 0x66; mov [di+4], bx;    
    	db 0x66; mov [di+8], cx;    
    	db 0x66; mov [di+12], dx;   
	}
	
	asm popad;
}


// use the INT 0x15, eax= 0xE820 BIOS function to get a memory map
// inputs: es:di -> destination buffer for 24 byte entries
// outputs: bp = entry count, trashes all registers except esi
void memory_map_E820(unsigned long *mmap_entry)
{ 
	memory_map_counter[0] = 0;
	
	asm pushad;

_asm{
	.do_e820:
		mov di, mmap_entry;				// input address
		xor ebx, ebx;					// ebx must be 0 to start
		xor bp, bp;						// keep an entry count in bp
		mov edx, 0x534D4150;			// Place "SMAP" into edx
		mov eax, 0xE820;
		mov [es:di + 20], 1;			// force a valid ACPI 3.X entry
		mov ecx, 24;					// ask for 24 bytes
		int 0x15;
		jc short .memory_map_failed;	// carry set on first call means "unsupported function"
		mov edx, 0x534D4150;			// Some BIOSes apparently trash this register?
		cmp eax, edx;					// on success, eax must have been reset to "SMAP"
		jnz short .memory_map_failed;
		test ebx, ebx;					// ebx = 0 implies list is only 1 entry long (worthless)
		jz short .memory_map_failed;
		jmp short .jmpin;

	.e820lp:
		mov eax, 0xE820;				// eax, ecx get trashed on every int 0x15 call
		mov [es:di + 20], 1;			// force a valid ACPI 3.X entry
		mov ecx, 24;					// ask for 24 bytes again
		int 0x15;
		jc short .e820f;				// carry set means "end of list already reached"
		mov edx, 0x534D4150;			// repair potentially trashed register

	.jmpin:
		jcxz .skipent;					// skip any 0 length entries
		cmp cl, 20;						// got a 24 byte ACPI 3.X response?
		jbe short .notext;
		test [es:di + 20], 1;			// if so: is the "ignore this data" bit clear?
		jz short .skipent;	

	.notext:
		mov ecx, [es:di + 8];			// get lower uint32_t of memory region length
		or ecx, [es:di + 12];			// "or" it with upper uint32_t to test for zero
		jz .skipent;					// if length uint64_t is 0, skip entry
		inc bp;							// got a good entry: ++count, move to next storage spot
		add di, 24;

	.skipent:
		test ebx, ebx;					// if ebx resets to 0, list is complete
		jnz short .e820lp;

	.e820f:
		mov [memory_map_counter], bp;	// store the entry count
		clc; 							// there is "jc" on end of list to this point, so the carry must be cleared
		//ret; 							// emulator works well 
		     							// silicon does's work 2015-08-20
	.memory_map_failed:
		stc;							// "function unsupported" error exit
		//ret;
	}

	asm popad;
}


// Serial Presence Detect 256 bytes read, compatible with VIA and Intel chipset
void spd_read(unsigned int dimm_no)
{
	unsigned int device_no;
	unsigned int dword_no;
	unsigned int byte_no;
	unsigned int spd_addr;
	unsigned long smb_ret;
	unsigned long reg_ret;
	unsigned int a;

	switch(host_vendor_id)
	{
		// VIA Host bridge
		case 0x1106:
			// D17F0 OFFSET D0:D1
			smb_base_addr = 0x400;          
			spd_addr = 0xA0 + dimm_no * 2;
			for(dword_no=0; dword_no<64; dword_no++)
			{
				data_stotage_array_256b[dword_no] = 0;
				for (byte_no=0; byte_no<4; byte_no++)
				{
					smb_ret = smbus_read_byte (smb_base_addr, spd_addr, dword_no*4+byte_no);					
					smb_ret = smb_ret << (8 * byte_no);
					data_stotage_array_256b[dword_no] += smb_ret;
				}			
			}
			spd_parser (data_stotage_array_256b);
			break;

		// Intel Host bridge	
		case 0x8086:
			for (device_no=0; device_no<max_pci_dev_quantity; device_no++)
			{
				pci_dev_header_scan(device_no);
				// Find SMBus, OFFSET 0x20
				if ((pci_device_detail.base_class == 0xc) && (pci_device_detail.sub_class == 0x5))
				{
					out_port(PCI_INDEX, 0x80000000 + (pci_device_bdf[device_no].bus << 16) + (pci_device_bdf[device_no].device << 11) + (pci_device_bdf[device_no].function << 8) + 0x20);
					reg_ret = in_port (PCI_DATA);
					smb_base_addr = reg_ret & 0xfff0;
				}
			}
			          
			spd_addr = 0xA0 + dimm_no * 2;
			for(dword_no=0; dword_no<64; dword_no++)
			{
				data_stotage_array_256b[dword_no] = 0;
				for (byte_no=0; byte_no<4; byte_no++)
				{
					smb_ret = 0;
					smb_ret = smbus_read_byte(smb_base_addr, spd_addr, dword_no*4+byte_no);					
					smb_ret = smb_ret << (8 * byte_no);
					data_stotage_array_256b[dword_no] += smb_ret;
				}			
			}
			spd_parser (data_stotage_array_256b);
			break;

		default : break;
	}
}


/*************************************************************
	SMBus read by BYTE
	Input  : register index & device ID & SMB_BA
	Output : Value read by byte
	http://blog.csdn.net/wbcuc/article/details/6202074
*************************************************************/
unsigned int smbus_read_byte(unsigned int smb_ba, unsigned int devid, unsigned int regindex)
{
	unsigned int smb_ret;
	
	_asm{
		pushad;
		mov cl,regindex;	
		mov ch,devid;
		push cx;
		
		// judge if SMBus is busy with HST_STS -> HST_STS&0x1E ?= 0, NOT:0x1E>HST_STS;
		mov dx,smb_ba;
		in  al,dx;
		test al,0x1E;
		jz .chk_smb_rdy1;
		mov al,0x1E;
		out dx,al;	
		
	.chk_smb_rdy1:
		mov dx,smb_ba;
    	in  al,dx;	 
		test al,0x1;
		jnz .chk_smb_rdy1;

		//set XMIT_SLVA(SMBA+0x04),select device addr,0x01 means read;
		mov dx,smb_ba;
		add dl,0x4;
		inc ch; 
		mov al,ch;
		out dx,al;

	.chk_smb_rdy2:
		mov dx,smb_ba;
    	in  al,dx;	 
		test al,0x1;
		jnz .chk_smb_rdy2;

		//set HST_CMD(SMBA+0x03),select device register index
		mov dx,smb_ba;
		add dl,0x3;
		pop ax;
		out dx,al;
	
	.chk_smb_rdy3:
		mov dx,smb_ba;
    	in  al,dx;	 
		test al,0x1;
		jnz .chk_smb_rdy3;
	
		//set HST_CNT(SMBA_0x02),0x48,that means read by byte
		mov dx,smb_ba;
		add dl,0x2;
		mov al,0x48;
		out dx,al;

		mov cx,0xffff;
	.smb_io_delay:
		loop .smb_io_delay;

	.chk_smb_rdy4:
		mov dx,smb_ba;
    	in  al,dx;	 
		test al,0x1;
		jnz .chk_smb_rdy4;

		xor al,al;
		mov dx,smb_ba;
		in  al,dx;
		test al,0x2;
		jz .smb_exit;
	
	.chk_smb_rdy5:
		mov dx,smb_ba;
    	in  al,dx;	 
		test al,0x1;
		jnz .chk_smb_rdy5;
		
		//read from HST_D0(SMBA+0x05)
		mov dx,smb_ba;
		add dl,0x5;
		in  ax,dx;	
		
		lea di,smb_ret;
		mov [di],ax;
	.smb_exit:
		popad;
	}

	return smb_ret;
}


// IO Space 256 ports traversal from base_addr
void io_space_read (unsigned long base_addr)
{
	unsigned int dword_no;
	unsigned int byte_no;
	unsigned long io_ret;
	unsigned int a;

	for (dword_no=0; dword_no<64; dword_no++)
	{
		data_stotage_array_256b[dword_no] = 0;
		for (byte_no=0; byte_no<4; byte_no++)
		{
			io_ret = in_port_byte(base_addr);
			io_ret = io_ret & 0xff;
			for(a=0; a<byte_no; a++)
			{
				io_ret = io_ret << 8;
			}
			data_stotage_array_256b[dword_no] += io_ret;
			base_addr ++;
		}
	}
	strcpy(content_name_string_zone2, "IO Space");
	data_storage_addr_table = data_stotage_array_256b;
}


// Single address Model Specific Register Read
void msr_read(unsigned long addr, unsigned long *msr_entry)
{
	asm pushad;

	_asm{
		db 0x66; mov di, msr_entry;
    	db 0x66; mov cx, addr;
    	db 0x0f; db 0x32;    		
    	db 0x66; mov [di], ax;    	  
    	db 0x66; mov [di+4], dx;   
	}
	
	asm popad;
}


// Single address Model Specific Register Read
void msr_write(unsigned long addr, unsigned long content_a, unsigned long content_d)
{
	asm pushad;

	_asm{
		db 0x66; mov cx, addr;
		db 0x66; mov ax, content_a;
		db 0x66; mov dx, content_d;
		db 0x0f; db 0x30;
	}

	asm popad;
}


void cpu_msr_scan(unsigned long base_addr)
{
	msr_info[0] = 0;
	msr_info[1] = 0;
	msr_read(base_addr, msr_info);
	strcpy(content_name_string_zone2, "");
	data_storage_addr_table = msr_info;
}


void hba_register_scan(unsigned int hba_counter)
{
	unsigned int  port_no;
	unsigned int  a;
	unsigned long mem_ret;

	if (host_bus_adapter[hba_counter].nhf == 0)
		return;

	// IDE mode
	if (host_bus_adapter[hba_counter].mode <= 1)
	{
		max_sata_port_quantity = 3;
		for (port_no=0; port_no<max_sata_port_quantity + 1; port_no++)
		{
			ide_io_port_set(hba_counter, port_no);

			// Intel
			if (host_vendor_id == 0x8086)
			{
				unsigned int  sidpba = 0;
				unsigned long sidpba_ret;

				out_port (PCI_INDEX, ide_pci_base[hba_counter] + 0x24);
				sidpba = in_port(PCI_DATA) & 0xFFF0;
				out_port (sidpba, (port_no << 8) & 0xff00);
				sidpba_ret = in_port(sidpba + 4);
				if (sidpba_ret & 0x1)
				{
					sata_port_detect[port_no][0] = 0x2a;
					if ((sidpba_ret >> 8) & 0x1)
					{
						sata_port_detect[port_no][1] = 0x2a;
					}
					else
					{
						sata_port_detect[port_no][1] = 0x20;
					}
				}
				else 
				{
					sata_port_detect[port_no][0] = 0x20;
					sata_port_detect[port_no][1] = 0x20;
				}
			}

			// VIA
			else if (host_vendor_id == 0x1106)
			{
				unsigned long reg_ret;

				out_port (PCI_INDEX, ide_pci_base[hba_counter] + 0xA0);
				reg_ret = in_port (PCI_DATA);
				reg_ret = reg_ret >> (port_no * 8);
				if (reg_ret & 0x1)
				{
					sata_port_detect[port_no][0] = 0x2a;
					if ((reg_ret >> 1) & 0x1)
					{
						sata_port_detect[port_no][1] = 0x2a;
					}
					else
					{
						sata_port_detect[port_no][1] = 0x20;
					}
				}
				else 
				{
					sata_port_detect[port_no][0] = 0x20;
					sata_port_detect[port_no][1] = 0x20;
				}
			}
		}
	}

	// AHCI mode
	else if (host_bus_adapter[hba_counter].mode == 2)
	{
		// Number of Ports
		mem_ret = memory_read(host_bus_adapter[hba_counter].abar);
		host_bus_adapter[hba_counter].np = mem_ret & 0xf;
		
		// Interface Speed Support
		host_bus_adapter[hba_counter].iss = (mem_ret >> 20) & 0xf;

		// Ports Implemented
		host_bus_adapter[hba_counter].pi = memory_read(host_bus_adapter[hba_counter].abar + 0xc);	
		for (port_no=0; port_no<host_bus_adapter[hba_counter].np+1; port_no++)
		{
			mem_ret = host_bus_adapter[hba_counter].pi;
			if ((mem_ret & 0x1) == 1)
			{
				sata_port_detect[port_no][0] = 0x2a;
			}
			else
			{
				sata_port_detect[port_no][0] = 0x20;
			}
			mem_ret = mem_ret >> 1;
		}

		// AHCI Version
		mem_ret = memory_read(host_bus_adapter[hba_counter].abar + 0x10);
		for (a=0; a<3; a++)
		{
			host_bus_adapter[hba_counter].vs[2-a] = mem_ret & 0xff;
			mem_ret = mem_ret >> 8;
		}	

		max_sata_port_quantity = host_bus_adapter[hba_function_number].np;
		sata_port_register_scan();
	}
}


void ide_io_port_set(unsigned int hba_no, unsigned int sata_port)
{
	// IO port setting
	if (host_bus_adapter[hba_no].mode == 0)
	{
		ide_port_info[0][0] = 0x1F0;
		ide_port_info[1][0] = 0x1F0;
		ide_port_info[2][0] = 0x170;
		ide_port_info[3][0] = 0x170;
	}
	else if (host_bus_adapter[hba_no].mode == 1)
	{
		out_port(PCI_INDEX, ide_pci_base[hba_no] + 0x10);
		ide_port_info[0][0] = in_port(PCI_DATA) & 0xFFF0;
		ide_port_info[1][0] = in_port(PCI_DATA) & 0xFFF0;
		out_port(PCI_INDEX, ide_pci_base[hba_no] + 0x18);
		ide_port_info[2][0] = in_port(PCI_DATA) & 0xFFF0;
		ide_port_info[3][0] = in_port(PCI_DATA) & 0xFFF0;
	}

	// Device setting 
	ide_port_info[sata_port][1] = (sata_port % 2) << 4;
}


void sata_port_register_scan(void)
{
	unsigned int  port_no;
	unsigned long mem_ret;
	
	for (port_no=0; port_no<max_sata_port_quantity+1; port_no++)
	{	
		// Command List Base Address
		sata_port_mem_reg[port_no].clb = memory_read (host_bus_adapter[hba_function_number].abar + 0x100 + 0x80 * port_no) & 0xfffffc00;

		// FIS Base Address
		sata_port_mem_reg[port_no].fb = memory_read (host_bus_adapter[hba_function_number].abar + 0x100 + 0x80 * port_no + 0x8) & 0xffffff00;

		// Signature
		sata_port_mem_reg[port_no].sig = memory_read (host_bus_adapter[hba_function_number].abar + 0x100 + 0x80 * port_no + 0x24);
		
		// Device Detection
		mem_ret = memory_read (host_bus_adapter[hba_function_number].abar + 0x100 + 0x80 * port_no + 0x28);
		sata_port_mem_reg[port_no].det = mem_ret & 0xf;
		if (sata_port_mem_reg[port_no].det & 0x1 == 1)
		{
			sata_port_detect[port_no][1] = 0x2a;
		}
		else 
		{
			sata_port_detect[port_no][1] = 0x20;
		}

		// Current Interface Speed
		mem_ret = mem_ret >> 4;
		sata_port_mem_reg[port_no].cspd = mem_ret & 0xf;	

		// Allowed Speed
		mem_ret = memory_read (host_bus_adapter[hba_function_number].abar + 0x100 + 0x80 * port_no + 0x2c);
		mem_ret = (mem_ret >> 4) & 0xf;
		sata_port_mem_reg[port_no].aspd = mem_ret;
	}
}


void ide_identify(unsigned int sata_port)
{
	unsigned int  loop = 0;
	unsigned int  dword_no;

	// IDENTIFY PACKET DEVICE
	out_port_byte (ide_port_info[sata_port][0] + 6, ide_port_info[sata_port][1]);
	out_port_byte (ide_port_info[sata_port][0] + 7, 0xa1);
	while((in_port_byte(ide_port_info[sata_port][0] + 7) != 0x58) && (loop ++ < 0xffff));

	// IDENTIFY DEVICE
	if (in_port_byte(ide_port_info[sata_port][0] + 7) != 0x40)
	{
		out_port_byte (ide_port_info[sata_port][0] + 6, ide_port_info[sata_port][1]);
		out_port_byte (ide_port_info[sata_port][0] + 7, 0xec);
		loop = 0;
	}
	while((in_port_byte(ide_port_info[sata_port][0] + 7) != 0x58) && (loop ++ < 0xffff));

	for (dword_no=0; dword_no<128; dword_no++)
	{
		unsigned long dword_temp = 0;
		identify_word[dword_no] = 0;
		identify_word[dword_no] = in_port_word(ide_port_info[sata_port][0]) & 0xffff;
		dword_temp = in_port_word(ide_port_info[sata_port][0]) & 0xffff;
		identify_word[dword_no] += (dword_temp << 16) & 0xffff0000;
	}
}


void ahci_identify(unsigned int sata_port)
{
	unsigned long port_mem_reg_ba;
	unsigned long command_table_base_addr;
	unsigned long prdt_dba;
	unsigned int  mem_temp;
	unsigned int  dword_no;

	port_mem_reg_ba = host_bus_adapter[hba_function_number].abar + 0x100 + sata_port * 0x80;
	command_table_base_addr = memory_read (sata_port_mem_reg[sata_port].clb + 8);

	memory_write (sata_port_mem_reg[sata_port].clb, 0x10005);			// PxCLB.PRDTL > 1   PxCLB.CFL > 1   PxCLB.W > 0	
	memory_write (command_table_base_addr + 0x8c, 0x1ff);				// PRDT.DBC

	if (memory_read (port_mem_reg_ba + 0x24) == SATA_SIG_ATAPI) 		// ACMD
	{
		memory_write_byte (command_table_base_addr + 0x2, 0xA1);
		memory_write (command_table_base_addr + 0x40, 0x0);
		memory_write (command_table_base_addr + 0x44, 0x0);
		memory_write (command_table_base_addr + 0x48, 0x0);
		memory_write (command_table_base_addr + 0x4c, 0x0);
		memory_write_byte (command_table_base_addr + 0x40, 0xA1);
	}
	else																// CFIS
	{	
		memory_write_byte (command_table_base_addr, 0x27);				// CFIS.FIS_type > 0x27
		memory_write_byte (command_table_base_addr + 0x1, 0x80); 		// CFIS.C > 1
		memory_write_byte (command_table_base_addr + 0x2, 0xec);		// CFIS.command > ATA_CMD_IDENTIFY
	}
	memory_write_byte (command_table_base_addr + 0x7, sata_port); 		// CFIS.device > DEVICE	

	mem_temp = memory_read_byte (port_mem_reg_ba + 0x18);				// PxCMD
	memory_write_byte (port_mem_reg_ba + 0x18, mem_temp | 0x11);		// PxCMD.FRE > 1   PxCMD.ST > 1	
	memory_write_byte (port_mem_reg_ba + 0x38, 0x1);					// PxCI      > 1

	prdt_dba = memory_read (command_table_base_addr + 0x80);			// PRDT.DBA (Identify word base address)
	while (memory_read_byte(port_mem_reg_ba + 0x38) & 0x1);				// Chech completion: PxCI ?= 0
	delay(0xffff);														//This is a delay
	
	for (dword_no=0; dword_no<128; dword_no++)
	{
		identify_word[dword_no] = memory_read (prdt_dba + dword_no * 4);
	}
}


void identify_word_scan(unsigned int port_number)
{
	// IDE mode
	if (host_bus_adapter[hba_function_number].mode <= 1)
	{	
		ide_io_port_set (hba_function_number, port_number);
		ide_identify (port_number);
	}

	// AHCI mode
	else if (host_bus_adapter[hba_function_number].mode == 2)
	{
		ahci_identify (port_number);
	}
	
	identify_word_parser();
	strcpy(content_name_string_zone2, "Identify Word");
	data_storage_addr_table = identify_word + identify_word_page * 256;
}


void clock_frequency_read(void)
{
	unsigned long msr_temp[2];
	unsigned long dword_temp[2];
	unsigned long tsc_temp;
	unsigned long pit_temp;

	// PIT
	pit_temp = pit_read();
	clock_frequency.pit[timer_counter] = clock_frequency.pit[18] + 0x10000 - pit_temp;
	clock_frequency.pit[18] = pit_temp;

	// TSC
	msr_temp[0] = msr_info[0];
	msr_temp[1] = msr_info[1];
	msr_read (0x10, msr_info);
	dword_temp[0] = msr_info[0];
	dword_temp[1] = msr_temp[0];
	if (msr_info[1] > msr_temp[1])
	{
		tsc_temp = 0x80000000 + (dword_temp[0] >> 1) - (dword_temp[1] >> 1);
		tsc_temp = tsc_temp << 1;	
	}
	else if (msr_info[1] == msr_temp[1])
	{
		tsc_temp = msr_info[0] - msr_temp[0];
	}
	clock_frequency.tsc[timer_counter] = tsc_temp;

	strcpy(content_name_string_zone2, "");
}


unsigned int pit_read(void)
{
	unsigned int pit_ret;
	
	_asm{
		pushfd;
		cli;
		mov al, 00000000b;	//al = channel in bits 6 and 7, remaining bits clear
		out 0x43, al; 		//Send the latch command
 	
		in  al, 0x40;		//al = low byte of count
		mov ah, al;			//ah = low byte of count	
		in  al, 0x40;		//al = high byte of count
		rol ax, 8;			//al = low byte, ah = high byte (ax = current count)
		lea bx, pit_ret;
		mov [bx], ax;
		sti;
		popfd;
	}

	return pit_ret;
}


// Modify value of 64 DWORDS array according to highlight location 
unsigned long data_storage_array_write (unsigned int offset)
{
	unsigned long dword_temp;
	unsigned int  bit_drift_value;

	switch(number_input_width)
	{
		case 2:
			bit_drift_value = (highlight_unit_location.col % 4) * 8;
			dword_temp = ~ (0xff << bit_drift_value);
			dword_temp = dword_temp & (*(data_storage_addr_table + offset));
			dword_temp = dword_temp + (number_input_value << bit_drift_value);
			break;
		case 4:
			bit_drift_value = (highlight_unit_location.col % 2) * 16;
			dword_temp = ~ (0xffff << bit_drift_value);
			dword_temp = dword_temp & (*(data_storage_addr_table + offset));
			dword_temp = dword_temp + (number_input_value << bit_drift_value);
			break;
		case 8:
			dword_temp = number_input_value;
			break;
		default : break;
	}

	*(data_storage_addr_table + offset) = dword_temp;

	return dword_temp;
}

#endif
