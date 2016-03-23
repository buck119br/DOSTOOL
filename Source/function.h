#ifndef __FUNCTION_H
#define __FUNCTION_H


void interrupt counter(...)
{
	if (content_number == 153)
	{
		clock_frequency_read();
	}

	timer_counter++;
	if (timer_counter > 17)
	{	
		timer_counter = 0;	
	}		
	oldhandler();
}


// Move cursor to specific loaction
void cursor_move(unsigned int row, unsigned int col)
{
	asm pushad;

	// set cursor position 
	asm mov ah,0x2;
	asm mov bh,0;
	asm mov dh,row;
	asm mov dl,col;
	asm int 0x10;

	asm popad;
}


void delay(unsigned int time)
{
	_asm{
		pushad;

		mov cx,time;
	.delay_loop_short:	
		loop .delay_loop_short;

		popad;
	}
}


void number_input (unsigned int function)
{
	unsigned int a;
	unsigned int array_offset_addr;
	unsigned int physical_offset_addr;
	unsigned long dword_ret;

	// Main UI data modification
	if (function < 150)
	{
		switch (number_input_width)
		{
			case 2:
				array_offset_addr = ((highlight_unit_location.row - 5) * 4) + (highlight_unit_location.col / 4);
				break;
			case 4:
				array_offset_addr = ((highlight_unit_location.row - 5) * 4) + (highlight_unit_location.col / 2);
				break;
			case 8:
				array_offset_addr = ((highlight_unit_location.row - 5) * 4) + highlight_unit_location.col;
				break;
			default : break;
		}

		if (function < 100)
		{
			dword_ret = data_storage_array_write (array_offset_addr);
			physical_offset_addr = (highlight_unit_location.row - 5) * 16 + (highlight_unit_location.col / ((highlight_unit_max_col + 1) / 4) * 4);

			// PCI config space
			if (pcie_page_counter == 0)
			{
				unsigned long pci_base_addr;

				pci_base_addr = 0x80000000 + (pci_device_bdf[function].bus << 16) + (pci_device_bdf[function].device << 11) + (pci_device_bdf[function].function << 8);
				out_port(PCI_INDEX, pci_base_addr + physical_offset_addr);
				out_port(PCI_DATA, dword_ret);
			}

			// PCIE extended config space
			else
			{
				unsigned long pcie_base_addr;

				pcie_base_addr = pcie_extended_base_address + (pci_device_bdf[function].bus << 20) + (pci_device_bdf[function].device << 15) + (pci_device_bdf[function].function << 12) + pcie_page_counter * 256;
				memory_write(pcie_base_addr + physical_offset_addr, dword_ret);
			}
			
			display_control.display_focus = 1;
		}
		else 
		{
			switch (function)
			{			
				case 103:
					dword_ret = data_storage_array_write (array_offset_addr);
					physical_offset_addr = (highlight_unit_location.row - 5) * 16 + (highlight_unit_location.col / ((highlight_unit_max_col + 1) / 4) * 4);
					memory_write (memory_base_address + physical_offset_addr, dword_ret);
					display_control.display_focus = 1;
					break;
				
				case 105:
					if (data_display_unit == 1)
					{
						dword_ret = data_storage_array_write(array_offset_addr);
						physical_offset_addr = (highlight_unit_location.row - 5) * 16 + highlight_unit_location.col;
						out_port_byte (physical_offset_addr + io_space_base_address, number_input_value);
						display_control.display_focus = 1;
					}
					else
					{
						strcpy(error_reminder_content, "IO space should be writen by single byte");
						display_control.display_focus = 6;
					}
					break;
				
				default :
					display_control.display_focus = 1;
					break;
			}
		}
	}

	// Address Input Window
	else 
	{
		
		switch(function)
		{
			case 152:
				msr_info[msr_highlight_position] = number_input_value;
				msr_write(msr_base_address, msr_info[0], msr_info[1]);
				display_control.display_focus = 1;
				break;

			// Memory Address Input
			case 203:
				if (number_input_value > 0xfffffeff)
				{
					strcpy(error_reminder_content, "The address should be smaller than 0xFFFFFEFF");
					display_control.display_focus = 6;
				}
				else
				{
					memory_base_address = number_input_value;
					display_control.display_focus = 1;
					content_number = 103;
				}
				break;

			// IO Space Address Input
			case 205:
				if (number_input_value > 0xfeff)
				{
					strcpy(error_reminder_content, "The address should be smaller than 0xFEFF");
					display_control.display_focus = 6;
				}
				else
				{
					io_space_base_address = number_input_value;
					display_control.display_focus = 1;
					content_number = 105;
				}
				break;

			// CPU MSR Address Input
			case 252:
				if (number_input_value > 0xfffffffe)
				{
					strcpy(error_reminder_content, "The address should be smaller than 0xFFFFFFFE");
					display_control.display_focus = 6;
				}
				else
				{
					msr_base_address = number_input_value;
					msr_highlight_position = 0;
					display_control.display_focus = 1;
					content_number = 152;
				}
				break;
				
			default : break;
		}
	}
}


void memory_test(void)
{
	unsigned long  segment_256M;
	unsigned long  segment_25DW;
	unsigned long  memory_segment;
	unsigned int   memory_map_segment_num;
	unsigned long  memory_pass_counter;
	unsigned long  memory_error_counter;
	unsigned long  memory_add_reg;
	unsigned long  pass_signal_num;
	unsigned long  pass_persent;
	unsigned long  testing_start_segment;
	unsigned long  testing_end_segment;
	unsigned int   pattern_num;
	unsigned long  divide;
	char start_unit;
	char end_unit;
	char pass_signal[40];

	unsigned long random_segment;
	unsigned int  num;
	unsigned int row_num = 8;
	unsigned int error_row = 20;

	memory_test_display();

	memory_pass_counter = 0;
	memory_error_counter = 0;

	//search usable memory and < 4G
	for(memory_map_segment_num = 1; memory_map_segment_num < memory_map_counter[0]; memory_map_segment_num++)
	{
		if((memory_map[memory_map_segment_num*6+4] == 1) && (memory_map[memory_map_segment_num*6+1] == 0))
		{	
			cursor_move(row_num, 8);
			row_color_set(4, 35, 4, 79, color_white, color_blue);

			//divide into 256M segment
			for(segment_256M = 0; segment_256M < memory_map[memory_map_segment_num*6+2]; segment_256M += 0xFF00000)
			{
				memory_pass_counter = 0;
				memory_error_counter = 0;
				pass_signal_num = 0;
				pass_persent = 0;
				row_color_set(4, 35, 4, 79, color_white, color_blue);
				row_color_set(4, 11, 4, 29, color_white, color_blue);
				if(row_num == 18)
				{
					row_color_set(8, 0, 18, 79, color_white, color_blue);
					row_num = 8;
				}
				
				if((memory_map[memory_map_segment_num*6+2]-segment_256M)>0xFF00000)
				{
					memory_segment = 0xFF00000;
				}
				else if(memory_map[memory_map_segment_num*6+2]<0xFF00000)
				{
					memory_segment = memory_map[memory_map_segment_num*6+2];
				}
					else
					{
						memory_segment = memory_map[memory_map_segment_num*6+2]-segment_256M;
					}

				// judge unit is 'M' or 'K'
				if(((memory_map[memory_map_segment_num*6]+segment_256M)>>10) == 0)
				{
					testing_start_segment = memory_map[memory_map_segment_num*6]+segment_256M;
					start_unit = 'B';
				}
				else if(((memory_map[memory_map_segment_num*6]+segment_256M)>>20) == 0)
				{
					testing_start_segment = (memory_map[memory_map_segment_num*6]+segment_256M)>>10;
					start_unit = 'K';
				}
					else
					{
						testing_start_segment = (memory_map[memory_map_segment_num*6]+segment_256M)>>20;
						start_unit = 'M';
					}
				if((memory_map[memory_map_segment_num*6]+segment_256M+(memory_segment>>10)) == 0)
				{
					testing_end_segment= memory_map[memory_map_segment_num*6]+segment_256M+memory_segment-1;
					end_unit = 'B';
				}
				else if(((memory_map[memory_map_segment_num*6]+segment_256M+memory_segment)>>20) == 0)
				{
					testing_end_segment = (memory_map[memory_map_segment_num*6]+segment_256M+memory_segment-1)>>10;
					end_unit = 'B';
				}
					else
					{
						testing_end_segment = (memory_map[memory_map_segment_num*6]+segment_256M+memory_segment-1)>>20;
						end_unit = 'M';
					}
				
				cursor_move(row_num, 8);
				printf("%08lX   --   %08lX", memory_map[memory_map_segment_num*6]+segment_256M, memory_map[memory_map_segment_num*6]+segment_256M+memory_segment-1);
				cursor_move(row_num, 42);
				printf("%08ld", memory_pass_counter);
				cursor_move(row_num, 55);
				printf("%08ld", memory_error_counter);
				cursor_move(4, 13);
				printf("%ld%c -- %ld%c", testing_start_segment, start_unit, testing_end_segment, end_unit);

				//memory test 2 DW in every 25DW
				for(segment_25DW = 0; segment_25DW < memory_segment; segment_25DW += 100)
				{
					if(bioskey(1) != 0)
					{
						if(bioskey(0) == ESC)
						{
							goto mem_test_exit_;
						}
					}
					random_segment= 0;
					memory_add_reg = memory_map[memory_map_segment_num*6] + segment_256M + segment_25DW + (random_segment/4)*4;

					num = 0;
					for(pattern_num=0; pattern_num<16; pattern_num++)
					{
						memory_write(memory_add_reg, memory_test_pattern[pattern_num]);
						if(memory_read(memory_add_reg) == memory_test_pattern[pattern_num])
						{
							num++;
							if(num == 15)
							{
								memory_pass_counter++;
								cursor_move(row_num, 42);
								printf("%08ld", memory_pass_counter);
							}
						}
						else
						{
							memory_error_counter++;
							cursor_move(row_num, 55);
							printf("%08ld", memory_error_counter);
							if(error_row == 23)
							{
								row_color_set(20, 0, 22, 79, color_light_red, color_blue);
								error_row = 20;
							}
							cursor_move(error_row, 8);
							printf("%0l8X  failed", memory_add_reg);
							error_row++;
						}
					}

					//progress bar
					divide = 100;
					pass_persent = memory_pass_counter*100/(memory_segment/divide);
					divide = 40;
					pass_signal_num = memory_pass_counter*100/(memory_segment/divide);
					if(pass_persent > 100)
					{
						pass_persent = 100;
					}
					if(pass_signal_num > 40)
					{
						pass_signal_num = 40;
					}
					memset(pass_signal, 0, sizeof(pass_signal));
					while(pass_signal_num-- != 0)
					{
						strcat(pass_signal, "#");
					}
					cursor_move(4,35);
					printf("%2ld%%", pass_persent);
					cursor_move(4,39);
					printf("%s", pass_signal);
				}
				
			fail_:			
				if(memory_error_counter == 0)
				{
					cursor_move(row_num, 70);
					printf("Pass");
				}
				else
				{
					row_color_set(row_num, 70, row_num, 79, color_light_red, color_blue);
					cursor_move(row_num, 70);
					printf("Fail");
				}
				row_num++;
			}
		}
	}
	
	mem_test_exit_:
	content_number = 251;
	strcpy(content_name_string_zone2,"Memory Test Stop!");
}


#endif
