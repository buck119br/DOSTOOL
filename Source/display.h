#ifndef __DISPLAY_H
#define __DISPLAY_H


// Resolution and Color mode initiation
void display_init(void)
{
	asm pushad;
	
	// al=3  80*25   16 color mode
	asm mov ax,0x3;
	asm int 0x10;
	
	// cancle blinking and enable all 16 colors
	asm mov ax,0x1003;
	asm mov bx,0;
	asm int 0x10;

	asm popad;
}


// Set font and background property of location-given rectangular region
//           col_start
// row_start ************
//           *          *
//           ************ row_end
//                      col_end       
void row_color_set(unsigned int row_start, unsigned int col_start, unsigned int row_end, unsigned int col_end, unsigned int font_col, unsigned int back_col)
{	
	unsigned int ax_temp = 0;
	unsigned int total_line = 0;
	unsigned int row_property = 0;
	unsigned int start_point = 0;
	unsigned int end_point = 0;
	
	total_line   = row_end - row_start + 1;
	total_line   = total_line & 0xff;
	ax_temp      = 0x700 + total_line;
	ax_temp      = ax_temp & 0xffff;
	back_col	 = (back_col << 4) & 0xf0;
	font_col	 = font_col & 0xf;
	row_property = back_col + font_col;
	row_property = row_property & 0xff;
	row_start	 = (row_start << 8) & 0xff00;
	col_start	 = col_start & 0xff;
	start_point  = row_start + col_start;
	start_point  = start_point & 0xffff;
	row_end		 = (row_end << 8) & 0xff00;
	col_end 	 = col_end & 0xff;
	end_point    = row_end + col_end;
	end_point    = end_point & 0xffff;

	asm pushad;
	asm mov ax,ax_temp;
	asm mov bh,row_property;
	asm mov cx,start_point;
	asm mov dx,end_point;
	asm int 0x10;
	asm popad;
}


// Zone 1 handle the Memu Bar
void display_zone1(void)
{	
	row_color_set(zone_start_1.row,zone_start_1.col,zone_end_1.row,zone_end_1.col,color_black,color_white);
	row_color_set(0,0,0,0,color_red,color_white);
	row_color_set(0,12,0,12,color_red,color_white);
	cursor_move (zone_start_1.row,zone_start_1.col);
	printf("%s",main_menu);
}


// Zone 2 handle the Name String
void display_zone2(void)
{	
	row_color_set(zone_start_2.row,zone_start_2.col,zone_end_2.row,zone_end_2.col,color_white,color_blue);
	cursor_move (2,3);
	printf("%s",content_name_string_zone2);
}


// Zone 3 handle row-coordinate or any thing else
void display_zone3(unsigned int mode)
{
	unsigned int a;
	
	switch (mode)
	{
		case 1:
			row_color_set(zone_start_3.row,zone_start_3.col,zone_end_3.row,zone_end_3.col,color_white,color_blue);
			break;

		case 2:
			row_color_set(zone_start_3.row,zone_start_3.col,zone_end_3.row,zone_end_3.col,color_light_red,color_blue);
			
			// Print highlight coordinate			
			if ((content_number < 100))
			{
				row_color_set(zone_start_3.row,2,zone_start_3.row,4,color_yellow,color_blue);
				cursor_move (zone_start_3.row, 2);
				switch(data_display_unit)
				{
					case 1:	printf("%03X",(highlight_unit_location.row - 5) * 16 + highlight_unit_location.col + pcie_page_counter * 256); break;
					case 2: printf("%03X",(highlight_unit_location.row - 5) * 16 + (highlight_unit_location.col*2) + pcie_page_counter * 256); break;
					case 3: printf("%03X",(highlight_unit_location.row - 5) * 16 + (highlight_unit_location.col*4) + pcie_page_counter * 256); break;
					default : break;
				}
			}
			else if ((content_number > 102) && (content_number < 106))
			{
				row_color_set(zone_start_3.row,2,zone_start_3.row,4,color_yellow,color_blue);
				cursor_move (zone_start_3.row, 2);
				switch(data_display_unit)
				{
					case 1:	printf("%03X",(highlight_unit_location.row - 5) * 16 + highlight_unit_location.col); break;
					case 2: printf("%03X",(highlight_unit_location.row - 5) * 16 + (highlight_unit_location.col*2)); break;
					case 3: printf("%03X",(highlight_unit_location.row - 5) * 16 + (highlight_unit_location.col*4)); break;
					default : break;
				}
			}
			else if ((content_number >= 106) && (content_number < 126))
			{
				row_color_set(zone_start_3.row,1,zone_start_3.row,4,color_yellow,color_blue);
				cursor_move (zone_start_3.row, 1);
				switch(data_display_unit)
				{
					case 1:	printf("%04X",(highlight_unit_location.row - 5) * 16 + highlight_unit_location.col + acpi_table_page_counter * 256); break;
					case 2: printf("%04X",(highlight_unit_location.row - 5) * 16 + (highlight_unit_location.col*2) + acpi_table_page_counter * 256); break;
					case 3: printf("%04X",(highlight_unit_location.row - 5) * 16 + (highlight_unit_location.col*4) + acpi_table_page_counter * 256); break;
					default : break;
				}
			}
			else if (content_number == 126)
			{
				row_color_set(zone_start_3.row,2,zone_start_3.row,4,color_yellow,color_blue);
				cursor_move (zone_start_3.row, 2);
				switch(data_display_unit)
				{
					case 1:	printf("%03X",(highlight_unit_location.row - 5) * 16 + highlight_unit_location.col + identify_word_page * 256); break;
					case 2: printf("%03X",(highlight_unit_location.row - 5) * 16 + (highlight_unit_location.col*2) + identify_word_page * 256); break;
					case 3: printf("%03X",(highlight_unit_location.row - 5) * 16 + (highlight_unit_location.col*4) + identify_word_page * 256); break;
					default : break;
				}
			}
						
			// Print row coordinate
			for (a=0;a<16;a++)
			{				
				if (content_number < 100)
				{
					cursor_move (a+5,2);
					printf("%03X",a*16 + pcie_page_counter * 256);
				}
				else if ((content_number >= 100) && (content_number < 106))
				{
					cursor_move (a+5,2);
					printf("%03X",a*16);
				}
				else if ((content_number >= 106) && (content_number < 126))
				{
					cursor_move (a+5,1);
					printf("%04X",a*16 + acpi_table_page_counter * 256);
				}
				else if (content_number == 126)
				{
					cursor_move (a+5,2);
					printf("%03X",a*16 + identify_word_page * 256);
				}
			}
			break;
		case 3:
			break;
		default : break;
	}
}



// Zone 4 handle col-coordinate and 256 bytes data, or anything else
void display_zone4(unsigned int mode)
{
	unsigned int a;
	
	switch (mode)
	{
		case 1:
			row_color_set(zone_start_4.row,zone_start_4.col,zone_end_4.row,zone_end_4.col,color_white,color_blue);
			break;
		case 2:
			row_color_set(zone_start_4.row,zone_start_4.col,zone_end_4.row,zone_end_4.col,color_light_gray,color_blue);
			row_color_set(zone_start_4.row,zone_start_4.col,zone_start_4.row,zone_end_4.col,color_light_red,color_blue);
			cursor_move(zone_start_4.row,zone_start_4.col);
			if ((content_number < 100) || ((content_number >= 103) && (content_number < 127)))
			{
				switch(data_display_unit)
				{
					case 1:	
						printf("00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F "); break;
					case 2: 
						printf(" 0100  0302  0504  0706  0908  0B0A  0D0C  0F0E "); break;
					case 3:
						printf("  03020100    07060504    0B0A0908    0F0E0D0C  "); break;
					default : break;
				}
			}
			else if ((content_number >= 100) && (content_number < 103))
			{
				printf("    EAX         EBX         ECX         EDX     ");
			}
			break;
		case 3:
			break;
		default : break;
	}
}


// Zone 5 handle the parsered information, or anything else
void display_zone5(void)
{
	row_color_set(zone_start_5.row,zone_start_5.col,zone_end_5.row,zone_end_5.col,color_white,color_blue);
}


// Zone 6 handle additional information
void display_zone6(unsigned int mode)
{
	unsigned int a;

	switch (mode)
	{
		case 1:
			row_color_set(zone_start_6.row,zone_start_6.col,zone_end_6.row,zone_end_6.col,color_white,color_blue);	
			cursor_move(22, 3);
			switch(content_number)
			{
				case 150:
					printf("INT 15H E820");
					break;
				default : break;
			}
			break;
		case 2:
			row_color_set(zone_start_6.row,zone_start_6.col,zone_end_6.row,zone_end_6.col,color_white,color_blue);
			cursor_move (22, 3);
			if (content_number < 100)
			{
				printf("Bus %02ld   Device %02ld   Function %02ld   Type ",
					pci_device_bdf[content_number].bus, pci_device_bdf[content_number].device, pci_device_bdf[content_number].function);
				if (pci_device_detail.device_type == 1)
				{
					printf("PCIE");
				}
				else
				{
					printf("PCI");
				}
			}
			else if ((content_number >= 106) && (content_number <= 125))
			{
				printf("Signature %s   Base_Addr %08lX",
						acpi_header_detail.signature,
						acpi_header_ba[content_number-106] + acpi_table_page_counter * 256);
			}
			else
			{
				switch(content_number)
				{
					case 100 : printf("Standard functions 0x00000000"); break;
					case 101 : printf("Extended functions 0x80000000"); break;
					case 102 : printf("Centaur functions 0xC0000000"); break;
					case 103 : printf("Base Address : %08lX - %08lX", memory_base_address, memory_base_address + 255); break;
					case 104 : printf("DIMM %d", spd_menu_function_number + 1); break;
					case 105 : printf("Base Address : %04lX - %04lX", io_space_base_address, io_space_base_address + 255); break;
					case 126 : printf("Port %d", sata_port_function_number); break;
					default : break; 
				}
			}
			break;
		case 3:
			break;
		default : break;
	}	
}


// Zone 7 handle status bar
void display_zone7(unsigned int mode)
{
	switch (mode)
	{
		case 1:
			row_color_set(zone_start_7.row,zone_start_7.col,zone_end_7.row,zone_end_7.col,color_black,color_green);
			cursor_move (zone_start_7.row, zone_start_7.col);
			if (content_number == 151)
			{
				printf("%s",memory_test_tail);
			}
			else
			{
				printf("%s",main_tail);
			}		
			break;
		case 2:
			row_color_set(zone_start_7.row,zone_start_7.col,zone_end_7.row,zone_end_7.col,color_black,color_green);
			cursor_move (zone_start_7.row, zone_start_7.col);
			printf("%s",main_tail);
			break;
		case 3:
			break;
		default : break;
	}
}


/***************************************************************
	The most significant UI display entrance
***************************************************************/
void refresh_main_interface(void)
{
	// background display mode definites the main interface display in which status
	// 1: zone 2-6: font-white, back-blue;
	//    (Welcome, Memory Map, Memory Test)
	// 2: zone 2,4,5,6: font-white, back-blue; 
	//    zone 3: font-red, back-blue;
	//    zone 4: first line font-red, back-blue; 
	unsigned int backgnd_display_mode;
	
	// Set backgound_display_mode
	if (content_number < 100)		// PCI config space
	{
		backgnd_display_mode = 2;
	}
	else if ((content_number >= 106) && (content_number <= 125))	// ACPI Table
	{
		backgnd_display_mode = 2;
	}
	else
	{
		switch(content_number)
		{	
			case 100: 	// Standard CPUID 
			case 101: 	// Extended CPUID
			case 102: 	// Centaur  CPUID
			case 103: 	// Memory
			case 104: 	// SPD
			case 105: 	// IO Space
			case 126:	// Identify Word
				backgnd_display_mode = 2; 
				break;
				
			case 150: 	// Memory Map
			case 151:   // Memory Test
			case 152: 	// CPU MSR
			case 153:	// Clock Frequecy
			case 199: 	// Welcome
			case 203:	// Memory Address Input
			case 205:	// IO Space Address Input
			case 251:   // Memory Test ESC
			case 252:	// CPU MSR Address Input
				backgnd_display_mode = 1; 
				break;
				
			default : break;	
		}
	}

	// Set the background and font property of zone X
	if (display_control.main_refresh == 1)
	{
		display_zone1();
		display_zone2();
		display_zone3(backgnd_display_mode);
		display_zone4(backgnd_display_mode);
		display_zone5();
		display_zone6(backgnd_display_mode);
		display_zone7(backgnd_display_mode);

		if ((content_number != 152) && (content_number != 153))
		{
			display_control.main_refresh = 0;
		}
	}

	// Data display : only output the real data(stored for data_storage_addr_table[x])
	if (content_number < 100)
	{
		content_display_mode_2(data_storage_addr_table);
		pci_cfg_spc_parser_display ();
	}
	else if ((content_number >= 106) && (content_number < 126))	// ACPI Table
	{
		content_display_mode_2(data_storage_addr_table);
	}
	else
	{
		switch(content_number)
		{
			case 100:
				content_display_mode_2(data_storage_addr_table);
				cpuid_parser_display ();
				break;
			case 101:
				content_display_mode_2(data_storage_addr_table);
				cpuid_parser_display ();
				break;
			case 102:
				content_display_mode_2(data_storage_addr_table);
				cpuid_parser_display ();
				break;
			case 103:
				content_display_mode_2(data_storage_addr_table);
				break;
			case 104:
				content_display_mode_2(data_storage_addr_table);
				spd_parser_display ();
				break;
			case 105:
				content_display_mode_2(data_storage_addr_table);
				break;	
			case 126:
				content_display_mode_2(data_storage_addr_table);
				identify_word_parser_display();
				break;
			case 150:
				memory_map_display(memory_map_page_current);
				break;
			case 151:
				memory_test();
				break;
			case 152:
				msr_display();
				break;
			case 153:
				clock_frequency_display();
				break;
			case 251:
				memory_test_display();
				break;
			case 199:
				welcome_interface_display();
				break;
			default : break;	
		}
	}

	// Unit transfer to binary window
	if ((backgnd_display_mode == 2) && (display_control.binary_xfer == 1))
	{
		unsigned int col_start;
		unsigned int bin_xfer[32];
		unsigned long dword_temp;
		unsigned int a;
		
		switch(data_display_unit)
		{
			// BYTE
			case 1:
				dword_temp = *(data_storage_addr_table + (highlight_unit_location.row - 5) * 4 + highlight_unit_location.col / 4);
				for (a=0; a<32; a++)
				{
					bin_xfer[a] = dword_temp % 2;
					dword_temp = dword_temp >> 1;
				}
				col_start = zone_start_4.col + highlight_unit_location.col * 3;
				row_color_set(highlight_unit_location.row+1, col_start, highlight_unit_location.row+2, col_start+7, color_black, color_yellow);
				row_color_set(highlight_unit_location.row+3, col_start+1, highlight_unit_location.row+3, col_start+8, color_black, color_black);
				row_color_set(highlight_unit_location.row+2, col_start+8, highlight_unit_location.row+3, col_start+8, color_black, color_black);
				cursor_move(highlight_unit_location.row+1, col_start);
				for (a=8*(highlight_unit_location.col%4+1); a>8*(highlight_unit_location.col%4); a--)
				{
					printf("%d", bin_xfer[a - 1]);
				}
				cursor_move(highlight_unit_location.row+2, col_start);
				printf("   |   |");
				break;

			// WORD
			case 2:
				dword_temp = *(data_storage_addr_table + (highlight_unit_location.row - 5) * 4 + highlight_unit_location.col / 2);
				for (a=0; a<32; a++)
				{
					bin_xfer[a] = dword_temp % 2;
					dword_temp = dword_temp >> 1;
				}
				col_start = zone_start_4.col + highlight_unit_location.col * 6 + 1;
				row_color_set(highlight_unit_location.row+1, col_start, highlight_unit_location.row+2, col_start+16, color_black, color_yellow);
				row_color_set(highlight_unit_location.row+3, col_start+1, highlight_unit_location.row+3, col_start+17, color_black, color_black);
				row_color_set(highlight_unit_location.row+2, col_start+17, highlight_unit_location.row+3, col_start+17, color_black, color_black);
				cursor_move(highlight_unit_location.row+1, col_start - 1);
				for (a=16*(highlight_unit_location.col%2+1); a>16*(highlight_unit_location.col%2); a--)
				{	
					if ((a % 8) == 0)
					{
						printf(" ");
					}
					printf("%d", bin_xfer[a - 1]);
				}
				cursor_move(highlight_unit_location.row+2, col_start);
				printf("   |   |    |   |");
				break;

			// DWORD
			case 3:
				dword_temp = *(data_storage_addr_table + (highlight_unit_location.row - 5) * 4 + highlight_unit_location.col);
				for (a=0; a<32; a++)
				{
					bin_xfer[a] = dword_temp % 2;
					dword_temp = dword_temp >> 1;
				}
				col_start = zone_start_4.col + highlight_unit_location.col * 12 + 2;
				row_color_set(highlight_unit_location.row+1, col_start, highlight_unit_location.row+2, col_start+34, color_black, color_yellow);
				row_color_set(highlight_unit_location.row+3, col_start+1, highlight_unit_location.row+3, col_start+35, color_black, color_black);
				row_color_set(highlight_unit_location.row+2, col_start+35, highlight_unit_location.row+3, col_start+35, color_black, color_black);
				cursor_move(highlight_unit_location.row+1, col_start - 1);
				for (a=32; a>0; a--)
				{	
					if ((a % 8) == 0)
					{
						printf(" ");
					}
					printf("%d", bin_xfer[a - 1]);
				}
				cursor_move(highlight_unit_location.row+2, col_start);
				printf("   |   |    |   |    |   |    |   |");
				break;

			default : break;
		}
	}
}


/***************************************************************
	256 bytes presentation 
	the most significant data presentation entrance
***************************************************************/
void content_display_mode_2(unsigned long * content)
{
	unsigned int content_temp_byte[256];
	unsigned int content_temp_word[128];
	unsigned long dword_temp;
	unsigned int dword_no;
	unsigned int word_no;
	unsigned int byte_no;
	unsigned int col_start;
	unsigned int row,col;
	
	// DWORD XFER TO BYTE AND WORD
	for (dword_no=0; dword_no<64; dword_no++)
	{
		dword_temp = content[dword_no];
		for (word_no=0; word_no<2; word_no++)
		{
			content_temp_word[dword_no*2 + word_no] = dword_temp & 0xffff;
			dword_temp = dword_temp >> 16;
		}
		
		dword_temp = content[dword_no];
		for (byte_no=0; byte_no<4; byte_no++)
		{
			content_temp_byte[dword_no*4 + byte_no] = dword_temp & 0xff;
			dword_temp = dword_temp >> 8;
		}
	}

	switch(data_display_unit)
	{
		// BYTE
		case 1:
			for(row=0; row<16; row++)
			{
				cursor_move (row+5,zone_start_4.col);
				for(col=0; col<16; col++)
				{
					col_start = zone_start_4.col + col * 3; 
					if ((highlight_unit_location.row == row+5) && (highlight_unit_location.col == col))
					{
						row_color_set (row+5, col_start, row+5, col_start+data_display_unit_width-1, color_black, color_yellow);
					}
					else
					{
						if (content_temp_byte[row*16+col] != 0)
						{
							row_color_set (row+5, col_start, row+5, col_start+data_display_unit_width-1, color_yellow, color_blue);
						}
					}	
					printf("%02X ",content_temp_byte[row*16+col]);
				}
			}
			break;

		// WORD
		case 2:
			for(row=0; row<16; row++)
			{
				cursor_move (row+5,zone_start_4.col);
				for(col=0; col<8; col++)
				{
					col_start = zone_start_4.col + col * 6 + 1;
					if ((highlight_unit_location.row == row+5) && (highlight_unit_location.col == col))
					{
						row_color_set (row+5, col_start, row+5, col_start+data_display_unit_width-1, color_black, color_yellow);
					}
					else
					{
						if (content_temp_word[row*8+col] != 0)
						{
							row_color_set (row+5, col_start, row+5, col_start+data_display_unit_width-1, color_yellow, color_blue);								
						}
					}
					printf(" %04X ",content_temp_word[row*8+col]);
				}
			}
			break;

		// DWORD	
		case 3:
			for(row=0; row<16; row++)
			{
				cursor_move (row+5,zone_start_4.col);
				for(col=0; col<4; col++)
				{
					col_start = zone_start_4.col + col * 12 + 2;
					if ((highlight_unit_location.row == row+5) && (highlight_unit_location.col == col))
					{
						row_color_set (row+5, col_start, row+5, col_start+data_display_unit_width-1, color_black, color_yellow);					
					}
					else
					{
						if (content[row*4+col] != 0)
						{
							row_color_set (row+5, col_start, row+5, col_start+data_display_unit_width-1, color_yellow, color_blue);
						}
					}
					printf("  %08lX  ",content[row*4+col]);
				}
			}
			break;
			
		default : break;	
	}
}


// PCI configuration space single device presentation
void F6_row_print(unsigned int device)
{
	pci_dev_header_scan(device);
	printf("%02d  %04lX   %04lX   %03ld  %02ld  %02ld  %02lX-%02lX-%02lX  %s",
		device+1, pci_device_detail.vendor_id, pci_device_detail.device_id,
		pci_device_bdf[device].bus, pci_device_bdf[device].device, pci_device_bdf[device].function,
		pci_device_detail.base_class, pci_device_detail.sub_class, pci_device_detail.interface_class,
		pci_device_detail.name_string);
}


// PCI configuration space UI 
void F6_interface(void)
{
	unsigned int a;
	
	// The user interface description
	row_color_set(zone_start_1.row,zone_start_1.col,zone_end_1.row,zone_end_1.col,color_black,color_white);
	row_color_set(zone_start_2.row,zone_start_2.col,zone_end_6.row,zone_end_6.col,color_white,color_black);
	row_color_set(zone_start_7.row,zone_start_7.col,zone_end_7.row,zone_end_7.col,color_black,color_green);
	cursor_move(zone_start_1.row,zone_start_1.col);
	printf("%s",F6_menu);
	if (max_pci_dev_quantity < 21)
	{
		for (a=0; a<max_pci_dev_quantity+1; a++)
		{
			cursor_move (a+2,0);
			F6_row_print(a);
		}
	}
	else
	{
		for (a=0;a<21;a++)
		{
			cursor_move (a+2,0);
			F6_row_print(a);
		}
	}
	cursor_move(zone_start_7.row,zone_start_7.col);
	printf("%s",F6_tail);	
}


// PCI configuration space refresh
void F6_row_operation (unsigned int position, unsigned int direction)
{
	unsigned int current_row;
	unsigned int previous_row;
	unsigned int max_f6_page;
	unsigned int current_f6_page;
	unsigned int a;

	current_f6_page = position / 21;
	max_f6_page = max_pci_dev_quantity / 21;
	current_row = position% 21 + 2;
	
	switch(direction)
	{
		// up
		case 0:

			// Boundary
			if ((position + 1) % 21 == 0)
			{			
				row_color_set(zone_start_2.row,zone_start_2.col,zone_end_6.row,zone_end_6.col,color_white,color_black);
				for (a = 0; a < 21; a++)
				{
					cursor_move (a+2,0);
					F6_row_print(current_f6_page*21+a);
				}
			}
			else 
			{
				previous_row = position% 21 + 3;
				row_color_set (previous_row,0,previous_row,79,color_white,color_black);
				cursor_move (previous_row,0);
				F6_row_print(position+1);
			}
			break;
			
		// down	
		case 1:	

			// Boundary
			if (position % 21 == 0)
			{
				row_color_set(zone_start_2.row,zone_start_2.col,zone_end_6.row,zone_end_6.col,color_white,color_black);

				// Last page
				if (current_f6_page == max_f6_page)
				{
					for (a = 0; a < max_pci_dev_quantity%21+1; a++)
					{
						cursor_move (a+2,0);
						F6_row_print(current_f6_page*21+a);
					}
				}
				else 
				{
					for (a = 0; a < 21; a++)
					{
						cursor_move (a+2,0);
						F6_row_print(current_f6_page*21+a);
					}	
				}
			}
			else
			{
				previous_row = position% 21 + 1;
				row_color_set (previous_row,0,previous_row,79,color_white,color_black);
				cursor_move (previous_row,0);
				F6_row_print(position-1);
			}
			break;
		default : break;	
	}

	row_color_set (current_row,0,current_row,79,color_black,color_yellow);
	cursor_move (current_row,0);
	F6_row_print(position);
}


void acpi_row_print(unsigned int device)
{
	acpi_header_detail_scan(device);
	printf("%02d     %s       %08lX    %06lX   %s", 	
								device + 1, 
								acpi_header_detail.signature, 
								acpi_header_ba[device],
								acpi_header_detail.length,
								acpi_header_detail.description);
}


void acpi_interface(void)
{
	unsigned int a;
	
	// The user interface description
	row_color_set(zone_start_1.row,zone_start_1.col,zone_end_1.row,zone_end_1.col,color_black,color_white);
	row_color_set(zone_start_2.row,zone_start_2.col,zone_end_6.row,zone_end_6.col,color_white,color_black);
	row_color_set(zone_start_7.row,zone_start_7.col,zone_end_7.row,zone_end_7.col,color_black,color_green);
	cursor_move(zone_start_1.row,zone_start_1.col);
	printf("%s",acpi_menu);
	for (a=0; a<max_acpi_table_quantity+1; a++)
	{
		cursor_move (a+2,0);
		acpi_row_print(a);
	}
	cursor_move(zone_start_7.row,zone_start_7.col);
	printf("%s",acpi_tail);
}


void acpi_row_operation(unsigned int position, unsigned int direction)
{
	unsigned int current_row;
	unsigned int previous_row;

	current_row = position + 2;

	if (direction == 0)
	{
		previous_row = current_row + 1;
	}
	else 
	{
		previous_row = current_row - 1;
	}
	
	row_color_set (current_row,0,current_row,79,color_black,color_yellow);
	cursor_move (current_row,0);
	acpi_row_print(position);
	row_color_set (previous_row,0,previous_row,79,color_white,color_black);
	cursor_move (previous_row,0);
	acpi_row_print(previous_row - 2);

}


void menu_interface(unsigned int s_row, unsigned int s_col, unsigned int e_row, unsigned int e_col)
{
	unsigned int row_no;
	unsigned int menu_length;
	
	menu_length = e_row - s_row + 1;

	switch(display_control.display_focus)
	{
		// Alt-F
		case 2:
			row_color_set (0,alt_f_menu_start.col,0,7,color_black,color_light_green);
			cursor_move (0,0);
			printf("Function");
			break;
		// Alt-R
		case 3:
			row_color_set (0,alt_r_menu_start.col,0,alt_r_menu_start.col+3,color_black,color_light_green);
			cursor_move (0,alt_r_menu_start.col);
			printf("Read");
			break;

		default : break;
	}

	row_color_set (s_row,s_col,e_row,e_col,color_white,color_dark_gray);
	row_color_set (e_row+1,s_col+1,e_row+1,e_col+2,color_black,color_black);
	if (menu_length == 1)
	{
		cursor_move (s_row, s_col);
		switch(display_control.display_focus)
		{
			case 9: printf("%s%04X - >>",hba_menu_content[0], host_bus_adapter[0].vid); break;
			default : break;
		}
	}
	else
	{
		row_color_set (s_row+1,e_col+1,e_row,e_col+2,color_black,color_black);
	
		for (row_no=0; row_no<menu_length; row_no++)
		{
			cursor_move (row_no + s_row, s_col);	
			switch(display_control.display_focus)
			{
				case  2: printf("%s", function_menu_content[row_no]); break;
				case  3: printf("%s", read_menu_content[row_no]); break;
				case  8: printf("%s", spd_menu_content[row_no]); break;
				case  9: printf("%s%04X - >>",hba_menu_content[row_no], host_bus_adapter[row_no].vid); break;
				case 10: printf("%s %c %c", sata_port_menu_content[row_no], sata_port_detect[row_no][0], sata_port_detect[row_no][1]); break;
				default : break;
			}		
		}
	}
}


void menu_row_operation(unsigned int position, unsigned int direction, unsigned int s_row, unsigned int s_col, unsigned int e_col)
{
	unsigned int current_row;
	unsigned int previous_row;
	unsigned int pre_row_switch = 1;

	current_row = position + s_row;
	if (direction == 0)
	{
		previous_row = current_row + 1;
	}
	else 
	{
		previous_row = current_row - 1;
	}

	row_color_set (current_row, s_col, current_row, e_col, color_black, color_yellow);
	cursor_move (current_row, s_col);
	switch(display_control.display_focus)
	{
		case  2: printf("%s", function_menu_content[position]); break;
		case  3: printf("%s", read_menu_content[position]); break;
		case  8: printf("%s", spd_menu_content[position]); break;
		case  9: printf("%s%04X - >>",hba_menu_content[position], host_bus_adapter[position].vid); break;
		case 10: printf("%s %c %c", sata_port_menu_content[position], sata_port_detect[position][0], sata_port_detect[position][1]);; break;
		default : break;
	}
	if ((display_control.display_focus == 9) && (max_hba_quantity == 0))
	{
		pre_row_switch = 0;
	}
	if (pre_row_switch == 1)
	{
		row_color_set (previous_row, s_col, previous_row, e_col, color_white, color_dark_gray);
		cursor_move (previous_row, s_col);
		switch(display_control.display_focus)
		{
			case  2: printf("%s", function_menu_content[previous_row - s_row]); break;
			case  3: printf("%s", read_menu_content[previous_row - s_row]); break;
			case  8: printf("%s", spd_menu_content[previous_row - s_row]); break;	
			case  9: printf("%s%04X - >>",hba_menu_content[previous_row - s_row], host_bus_adapter[previous_row - s_row].vid); break;
			case 10: printf("%s %c %c", sata_port_menu_content[previous_row - s_row], sata_port_detect[previous_row - s_row][0], sata_port_detect[previous_row - s_row][1]);; break;	
			default : break;
		}
	}
}


void welcome_interface_display(void)
{
	unsigned int row_no;

	row_color_set(2, 0, 2, 79, color_yellow, color_blue);
	
	for (row_no=0; row_no<20; row_no++)
	{
		cursor_move(row_no + 2,0);
		printf("%s",welcome_content[row_no]);
	}
}


void number_input_display (unsigned int function)
{
	unsigned int a;
	unsigned int col_start;

	// Data length judgement and presentation
	if (function < 150)
	{
		number_input_width = data_display_unit_width;

		switch (data_display_unit)
		{
			case 1: 
				col_start = zone_start_4.col + highlight_unit_location.col * 3;
				row_color_set (highlight_unit_location.row, col_start, highlight_unit_location.row, col_start+data_display_unit_width-1, color_white, color_red);
				break;
			case 2:
				col_start = zone_start_4.col + highlight_unit_location.col * 6 + 1;
				row_color_set (highlight_unit_location.row, col_start, highlight_unit_location.row, col_start+data_display_unit_width-1, color_white, color_red);
				break;
			case 3:
				col_start = zone_start_4.col + highlight_unit_location.col * 12 + 2;
				row_color_set (highlight_unit_location.row, col_start, highlight_unit_location.row, col_start+data_display_unit_width-1, color_white, color_red);			
				break;
			default : break;
		}

		cursor_move (highlight_unit_location.row, col_start);
		switch(number_input_width)
		{
			case 2: printf("%02lX", number_input_value); break;
			case 4: printf("%04lX", number_input_value); break;
			case 8: printf("%08lX", number_input_value); break;
			default : break;
		}
		cursor_move (highlight_unit_location.row, col_start + number_input_width - 1);
	}
	else if (function == 152)
	{
		row_color_set (msr_highlight_position * 4 + 10, 34, msr_highlight_position * 4 + 10, 41, color_white, color_red);
		cursor_move(msr_highlight_position * 4 + 10, 34);
		printf("%08lX",number_input_value);
		cursor_move(msr_highlight_position * 4 + 10, 41);
	}
	else 
	{
		switch (function)
		{
			case 203 : number_input_width = 8; break;
			case 205 : number_input_width = 4; break;
			case 252 : number_input_width = 8; break;
			default : break;
		}

		display_control.main_refresh = 1;
		refresh_main_interface();
		row_color_set (number_input_start.row, number_input_start.col, number_input_end.row, number_input_end.col, color_black, color_white);
		row_color_set (number_input_end.row+1,number_input_start.col+1,number_input_end.row+1,number_input_end.col+2,color_black,color_black);
		row_color_set (number_input_start.row+1,number_input_end.col+1,number_input_end.row,number_input_end.col+2,color_black,color_black);
		row_color_set (12, 43, 12, 42 + number_input_width, color_white, color_red);
		cursor_move (12, 29);
		printf("Input Value : ");				
	
		switch(number_input_width)
		{
			case 2: printf("%02lX", number_input_value); break;
			case 4: printf("%04lX", number_input_value); break;
			case 8: printf("%08lX", number_input_value); break;
			default : break;
		}
		cursor_move (12, 42 + number_input_width);
	}
}


void error_reminder (char * content)
{
	unsigned int string_length;

	display_control.main_refresh = 1;
	refresh_main_interface();
	string_length = strlen(content);
	if (string_length >= 30)
	{
		row_color_set (10, 39 - string_length / 2 - 2, 14, 40 + string_length / 2 + 2, color_red, color_white);
		row_color_set (15, 39 - string_length / 2 - 1, 15, 40 + string_length / 2 + 4, color_black, color_black);
		row_color_set (11, 40 + string_length / 2 + 3, 14, 40 + string_length / 2 + 4, color_black, color_black);
	}
	else 
	{
		row_color_set (10, 23, 14, 56, color_red, color_white);
		row_color_set (15, 24, 15, 58, color_black, color_black);
		row_color_set (11, 57, 14, 58, color_black, color_black);
	}
	cursor_move (11, 40 - string_length / 2);
	printf("%s",content);	
	cursor_move (13, 25);
	printf("Press ESC or Enter to continue");
}


void memory_map_display(unsigned int page)
{
	unsigned int a;
	unsigned int b;
	unsigned int max_seg;
	
	row_color_set (4, 0, 4, 79, color_light_red, color_blue);
	cursor_move (4,6);
	printf("NO |   Base_Address   |   End_Address    |        Type       |");
	if (page == memory_map_page_max)
	{
		max_seg = memory_map_counter[0]-memory_map_page_max*16;
	}
	else
	{
		max_seg = 16;
	}
	for (a=0; a<max_seg; a++)
	{
		cursor_move (a+5,6);
		printf("%02d |",page*16+a+1);	
		printf(" %08lX",memory_map[(page*16+a)*6+1]);
		printf("%08lX |",memory_map[(page*16+a)*6]);
		printf(" %08lX",memory_map[(page*16+a)*6+3]+memory_map[(page*16+a)*6+1]);
		printf("%08lX |",memory_map[(page*16+a)*6+2]+memory_map[(page*16+a)*6]-1);
		switch(memory_map[(page*16+a)*6+4])
		{
			case 1: printf(" Usable(norm) RAM  |"); 	break;
			case 2: printf(" Reserved-unusable |"); 	break;
			case 3: printf(" ACPI reclaimable  |"); 	break;
			case 4: printf(" ACPI NVS          |"); 	break;
			case 5: printf(" Contain Bad Mem   |"); 	break;
		}
	}
}


//Memory test title
void memory_test_display(void)
{	
	row_color_set (6, 0, 6, 79, color_light_red, color_blue);
	cursor_move(6,2);
	printf("|-----------Memory Segment-----------|    Pass    |    Fail    |   Status  |");
	cursor_move(7,2);
	printf("            --------------                ----         ----        ------");
	cursor_move(4,30);
	printf("Pass");
	cursor_move(4,3);
	printf("Testing:");
}


void message_window_display(unsigned int s_row, unsigned int s_col, unsigned int e_row, unsigned int e_col)
{
	row_color_set (s_row,s_col, s_row, e_col, color_white, color_red);
	row_color_set (s_row + 1, s_col, e_row, e_col, color_black, color_white);
	row_color_set (e_row+1, s_col+1, e_row+1, e_col+2,color_black,color_black);
	row_color_set (s_row+1, e_col+1, e_row, e_col+2,color_black,color_black);
}


void msr_display(void)
{
	unsigned long msr_temp;
	unsigned int  bit_xfer[32];
	unsigned int  a;

	// UI portray
	if (display_control.main_refresh == 1)
	{
		message_window_display(msr_display_start.row, msr_display_start.col, msr_display_end.row, msr_display_end.col);
		cursor_move(msr_display_start.row , msr_display_start.col + 1);
		printf("Model Specific Registers");
		cursor_move(8, msr_display_start.col + 1);
		printf("Address : %08lX", msr_base_address);
		display_control.main_refresh = 0;
	}

	// EAX
	row_color_set (msr_highlight_position * 4 + 10, 34, msr_highlight_position * 4 + 10, 41, color_black, color_green);
	cursor_move(10, msr_display_start.col + 1);
	printf("EAX : %08lX", msr_info[0]);

	// EAX transfer to binary
	msr_temp = msr_info[0];
	for (a=0; a<32; a++)
	{
		bit_xfer[a] = msr_temp % 2;
		msr_temp = msr_temp >> 1;
	}
	cursor_move(11, msr_display_start.col + 3);
	printf("H : ");
	for (a=0; a<8; a++)
	{
		printf("%d",bit_xfer[31 - a]);
	}
	printf(" ");
	for (a=0; a<8; a++)
	{
		printf("%d",bit_xfer[23 - a]);
	}
	cursor_move(12, msr_display_start.col + 3);
	printf("L : ");
	for (a=0; a<8; a++)
	{
		printf("%d",bit_xfer[15 - a]);
	}
	printf(" ");
	for (a=0; a<8; a++)
	{
		printf("%d",bit_xfer[7 - a]);
	}

	// EDX
	cursor_move(14, msr_display_start.col + 1);
	printf("EDX : %08lX", msr_info[1]);

	// EDX transfer to binary
	msr_temp = msr_info[1];
	for (a=0; a<32; a++)
	{
		bit_xfer[a] = msr_temp % 2;
		msr_temp = msr_temp >> 1;
	}
	cursor_move(15, msr_display_start.col + 3);
	printf("H : ");
	for (a=0; a<8; a++)
	{
		printf("%d",bit_xfer[31 - a]);
	}
	printf(" ");
	for (a=0; a<8; a++)
	{
		printf("%d",bit_xfer[23 - a]);
	}
	cursor_move(16, msr_display_start.col + 3);
	printf("L : ");
	for (a=0; a<8; a++)
	{
		printf("%d",bit_xfer[15 - a]);
	}
	printf(" ");
	for (a=0; a<8; a++)
	{
		printf("%d",bit_xfer[7 - a]);
	}
}


void clock_frequency_display(void)
{
	unsigned long out_temp = 0;
	unsigned int  a;
	unsigned long hpet_mctp;
	unsigned long divide;
		
	if (display_control.main_refresh == 1)
	{
		message_window_display(clk_freq_display_start.row, clk_freq_display_start.col, clk_freq_display_end.row, clk_freq_display_end.col);
		cursor_move(clk_freq_display_start.row , clk_freq_display_start.col + 5);
		printf("Clock Frequency");
		display_control.main_refresh = 0;
	}

	cursor_move (clk_freq_display_start.row + 2, clk_freq_display_start.col + 2);
	printf("RTC : 1 Hz");

	// PIT
	cursor_move (clk_freq_display_start.row + 4, clk_freq_display_start.col + 2);
	for (a=0; a<18; a++)
	{
		out_temp += clock_frequency.pit[a];
	}
	divide = 1000;
	printf("PIT : %ld KHz", (out_temp / divide) * 101 / 100);

	cursor_move (clk_freq_display_start.row + 6, clk_freq_display_start.col + 2);
	printf("ACPI: Unknown");

	cursor_move (clk_freq_display_start.row + 8, clk_freq_display_start.col + 2);
	printf("APIC: Unknown");

	// HPET
	hpet_mctp = memory_read(hpet_mmio_base_addr + 4) / 10000;
	divide = 100000000;
	cursor_move (clk_freq_display_start.row + 10, clk_freq_display_start.col + 2);
	printf("HPET: %ld KHz", divide / hpet_mctp);

	// TSC
	cursor_move (clk_freq_display_start.row + 12, clk_freq_display_start.col + 2);
	for (a=0; a<18; a++)
	{
		out_temp += clock_frequency.tsc[a];
	}
	divide = 1000;
	printf("TSC : %ld MHz ", out_temp / divide / divide * 101 / 100);
}


void spd_parser_display(void)
{
	unsigned int a;
	
	cursor_move (4,zone_start_5.col);
	printf("-----------------------");
	cursor_move (5,zone_start_5.col);
	printf("DRAM Type:%s",spd_info_parsered.dram_device_type);
	cursor_move (6,zone_start_5.col);
	printf("Module Type:%s",spd_info_parsered.module_type);
	cursor_move (7,zone_start_5.col);
	printf("Voltage:%s",spd_info_parsered.voltage);
	cursor_move (8,zone_start_5.col);
	printf("Bus Width:%d bit",spd_info_parsered.total_bus_width);
	cursor_move (9,zone_start_5.col);
	printf("Capacity:%f GB",spd_info_parsered.total_capacity);
	cursor_move (10,zone_start_5.col);
	printf("MTB:%f * %d",spd_info_parsered.mtb,spd_info_parsered.mtb_hex);
	cursor_move (11,zone_start_5.col);
	printf("FTB:%f * %d",spd_info_parsered.ftb,spd_info_parsered.ftb_hex);
	cursor_move (12,zone_start_5.col);
	printf("Tck:%f",spd_info_parsered.tck);
	cursor_move (13,zone_start_5.col);
	printf("Speed:%f",spd_info_parsered.speed);
	cursor_move (14,zone_start_5.col);
	printf("Serial Num:%08lX",spd_info_parsered.module_serial_number);
	cursor_move (15,zone_start_5.col);
	printf("Part:");
	for (a = 0; a < 18; a++)
	{
		printf("%c",spd_info_parsered.module_part_number[a]);
	}
	cursor_move (16,zone_start_5.col);
	printf("Module Manuf:%d -> %X",spd_info_parsered.module_manufacturer_lsb,spd_info_parsered.module_manufacturer_msb);
	cursor_move (17,zone_start_5.col);
	printf("DRAM Manuf:%d -> %X",spd_info_parsered.dram_manufacturer_lsb,spd_info_parsered.dram_manufacrurer_msb);
	cursor_move (18,zone_start_5.col);
	printf("Raw Card:%s",spd_info_parsered.raw_card);
}


void cpuid_parser_display(void)
{
	unsigned int a;
	
	cursor_move (4,zone_start_5.col);
	printf("-----------------------");
	cursor_move (5,zone_start_5.col);
	printf("VenID: ");
	for (a=0; a<12; a++)
	{
		printf("%c",cpuid_info_parsered.vendor_id[a]);
	}
	cursor_move (6,zone_start_5.col);
	printf("FMS: %lX",cpuid_info_parsered.fms);
	cursor_move (7,zone_start_5.col);
	printf("Count: %d",cpuid_info_parsered.count);
	cursor_move (8,zone_start_5.col);
	printf("Name String:");
	cursor_move (9,zone_start_5.col);
	for (a=0; a<24; a++)
	{
		printf("%c",cpuid_info_parsered.name_string[a]);
	}
	cursor_move (10,zone_start_5.col);
	for (a=24; a<48; a++)
	{
		printf("%c",cpuid_info_parsered.name_string[a]);
	}
	cursor_move (11,zone_start_5.col);
	printf("L1 cache: %ld + %ld KB", cpuid_info_parsered.cache_1_size, cpuid_info_parsered.cache_1_size);
	cursor_move (12,zone_start_5.col);
	printf("L2 cache: %ld KB", cpuid_info_parsered.cache_2_size);
	cursor_move (13,zone_start_5.col);
	printf("L3 cache: %ld MB", cpuid_info_parsered.cache_3_size);
	cursor_move (15,zone_start_5.col);
	printf("Phy addr size : %d bits",cpuid_info_parsered.phy_addr_size);
	cursor_move (16,zone_start_5.col);
	printf("Virt addr size: %d bits",cpuid_info_parsered.virt_addr_size);
	cursor_move (17,zone_start_5.col);
	printf("Feature Flags:");
	cursor_move (18,zone_start_5.col);
	printf("C D : %08lX %08lX",cpuid_info_parsered.feature_flags[0], cpuid_info_parsered.feature_flags[1]);
	cursor_move (19,zone_start_5.col);
	printf("Extended Feature Flags:");
	cursor_move (20,zone_start_5.col);
	printf("C D : %08lX %08lX",cpuid_info_parsered.extended_feature_flags[0], cpuid_info_parsered.extended_feature_flags[1]);
}


void pci_cfg_spc_parser_display(void)
{
	unsigned long tmp;
	unsigned long a = 0;
	
	cursor_move (4, zone_start_5.col);
	printf("Data Width : %02d bits", data_display_unit_width * 4);

	cursor_move (6, zone_start_5.col);
	printf("VID:DID = %04lX:%04lX", pci_device_detail.vendor_id, pci_device_detail.device_id);
	cursor_move (7, zone_start_5.col);
	printf("Rev ID        : %02X", pci_device_detail.rev_id);
	cursor_move (8, zone_start_5.col);
	printf("Int Line (IRQ): %02X", pci_device_detail.int_line);
	cursor_move (9, zone_start_5.col);
	printf("Int Pin       : %02X", pci_device_detail.int_pin);

	// Address Maps
	cursor_move (11, zone_start_5.col);
	printf("Address Maps:");
	for (a=0; a<6; a++)
	{
		cursor_move (12+a, zone_start_5.col);
		if (pci_device_detail.address_map_type[a] == 2)
		{
			printf("Undefined");
		}
		else
		{
			switch(pci_device_detail.address_map_type[a])
			{
				// Memory Map IO
				case 0:
					switch(pci_device_detail.address_map_addr_length[a])
					{
						case 32:
							printf("MMIO : %08lX",pci_device_detail.address_map_addr[a]);
							break;
						case 64:
							printf("MMIO : 00000000%08lX",pci_device_detail.address_map_addr[a]);
						default : break;
					}
					break;		

				// IO
				case 1:
					printf("IO   : %04X",pci_device_detail.address_map_addr[a]);
					break;
				default : break;	
			}
		}
	}

	// PCIE capability and status
	if (pci_device_detail.device_type == 1)
	{
		cursor_move(19, zone_start_5.col);
		printf("PCIE Link Cap: Gen%d x%d",pci_device_detail.pcie_max_link_speed, pci_device_detail.pcie_max_link_width);
		cursor_move(20, zone_start_5.col);
		printf("PCIE Link Sta: Gen%d x%d",pci_device_detail.pcie_current_link_speed, pci_device_detail.pcie_current_link_width);
	}

	cursor_move (5, zone_start_5.col);
	printf("Header Type : ");
	switch(pci_device_detail.header_type)
	{
		// PCI config space
		case 0x0 : 
			printf("PCI"); 
			break;

		// PCI TO PCI bridge architecture 	
		case 0x1 :
			printf("PCI-PCI"); 
			break;

		// PC Card Bus	
		case 0x2 : printf("Card bus"); break;
		default : break;
	}

}


void identify_word_parser_display(void)
{
	unsigned int a;

	cursor_move(4, zone_start_5.col);
	printf("-----------------------");
	cursor_move(5, zone_start_5.col);
	printf("Cylinders: %d", identify_device_information.cylinder);
	cursor_move(6, zone_start_5.col);
	printf("Headers  : %d", identify_device_information.headers);
	cursor_move(7, zone_start_5.col);
	printf("Sectors  : %d", identify_device_information.sectors);
	cursor_move(8, zone_start_5.col);
	printf("Que Depth: %d", identify_device_information.queue_depth);
	cursor_move(9, zone_start_5.col);
	if (sata_port_mem_reg[sata_port_function_number].aspd != 0)
	{
		printf("HST Allow SPD: Gen%d", sata_port_mem_reg[sata_port_function_number].aspd);
	}
	else 
	{
		printf("HST Allow SPD: No limit");
	}
	cursor_move(10,zone_start_5.col);
	printf("HST Currt SPD: Gen%d", sata_port_mem_reg[sata_port_function_number].cspd);
	cursor_move(11,zone_start_5.col);
	printf("DEV Suppt SPD: Gen%d", identify_device_information.sata_speed);

	cursor_move(15,zone_start_5.col);
	printf("Serial Number:");
	cursor_move(16, zone_start_5.col + 1);
	for (a=0; a<20; a++)
	{
		printf("%c", identify_device_information.serial_no[a]);
	}
	cursor_move(18, zone_start_5.col);
	printf("Model Number:");
	cursor_move(19, zone_start_5.col + 1);
	for (a=0; a<20; a++)
	{
		printf("%c", identify_device_information.model_number[a]);
	}
	cursor_move(20, zone_start_5.col + 1);
	for (a=20; a<40; a++)
	{
		printf("%c", identify_device_information.model_number[a]);
	}
}
	
#endif
