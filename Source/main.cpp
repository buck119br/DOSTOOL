/****************************************************************************************
	Dostool Version 2.0

Note: This version, we focus on rebuilding the architecture of the system,
	  including display and data storage.
	  We intend to provide a 'dostool API' for our collegues
****************************************************************************************/

#include "definition.h"
#include "constant.h"
#include "data.h"
#include "display.h"
#include "function.h"
#include "parser.h"

int main(void)
{
	unsigned int table_no;
	unsigned int key_ret;

	// Global variable initialization
	display_control.main_refresh 	= 1;
	display_control.other_refresh	= 0;
	display_control.data_refresh 	= 1;
	display_control.binary_xfer		= 1;
	display_control.display_focus 	= 1;

	content_number = 199;
	content_number_previous = 199;

	data_display_unit = 1;
	data_display_unit_width = 2;

	highlight_unit_location.row = 5;
	highlight_unit_location.col = 0;

	// Data preread	
	scan_bdf();
	pci_dev_header_scan(0);
	memory_map_E820(memory_map);
	acpi_header_ba_scan();
	for (table_no=0; table_no<max_acpi_table_quantity+1; table_no++)
	{
		acpi_header_detail_scan(table_no);
	}

	// Display initialization
	display_init();
	refresh_main_interface ();

	oldhandler = getvect(0x1c);
	disable();
	setvect(0x1c,counter);
		
	while(1)
	{		
		//******************************************************************************************************************
		// The key_ret judge and handle part
		//******************************************************************************************************************
		if (bioskey(1) != 0)
		{	
			key_ret = 0;
			key_ret = bioskey(0);
			
			switch (data_display_unit)
			{
				case 1: highlight_unit_max_col = 15; break;
				case 2: highlight_unit_max_col = 7; break;
				case 3: highlight_unit_max_col = 3; break;
				default : break;
			}

			switch (display_control.display_focus)
			{
				//**********************************************
				// The main screen operation
				//**********************************************
				case 1:	
					switch (key_ret)
					{
						// Alt-F means to open Function menu
						case ALT_F:	
							display_control.display_focus = 2;
							alt_f_function_number = 0;
							menu_direction.alt_f = 0;
							menu_interface(alt_f_menu_start.row,alt_f_menu_start.col,alt_f_menu_end.row,alt_f_menu_end.col);
							menu_row_operation (alt_f_function_number,menu_direction.alt_f,alt_f_menu_start.row,alt_f_menu_start.col,alt_f_menu_end.col);
							break;
							
						// Alt-R means to open Read menu
						case ALT_R:	
							display_control.display_focus = 3;
							alt_r_function_number = 0;
							menu_direction.alt_r = 0;
							menu_interface(alt_r_menu_start.row,alt_r_menu_start.col,alt_r_menu_end.row,alt_r_menu_end.col);
							menu_row_operation (alt_r_function_number,menu_direction.alt_r,alt_r_menu_start.row,alt_r_menu_start.col,alt_r_menu_end.col);
							break;

						// F6 means to open PCI device select menu		
						case F6:							
							display_control.display_focus = 4;
							f6_function_number = 0;
							menu_direction.f6 = 0;
							scan_bdf ();
							F6_interface ();
							F6_row_operation (f6_function_number,menu_direction.f6);
							break;

						// Enter to switch to number inputing interface to input value
						case ENTER:
							if ((content_number < 100) || (content_number == 103) || (content_number == 105) || (content_number == 152))
							{
								display_control.display_focus = 5;
								content_number_previous = content_number;
								number_input_value = 0;
							}
							break;

						// F7 means to change display mode
						case F7:
							if (content_number < 150)
							{
								if (data_display_unit == 3)
								{
									data_display_unit = 1;
								}
								else
								{
									data_display_unit ++;
								}
								switch(data_display_unit)
								{
									case 1: 
										data_display_unit_width = 2; 
										highlight_unit_location.col = highlight_unit_location.col * 4;
										break;
									case 2: 
										data_display_unit_width = 4;
										highlight_unit_location.col = highlight_unit_location.col / 2;
										break;
									case 3: 
										data_display_unit_width = 8; 
										highlight_unit_location.col = highlight_unit_location.col / 2;
										break;
									default : break;
								}
							}
							break;

						// F10 means to exit
						case F10:
						case CTRL_Q:
							row_color_set(zone_start_1.row,zone_start_1.col,zone_end_7.row,zone_end_7.col,color_white,color_black);
							cursor_move(0,0);
							goto main_exit_;

						// Up means to move the highlight unit upwards or see the upper page
						case UP:
							if (content_number < 150)
							{
								if (highlight_unit_location.row <= 5)
								{
									highlight_unit_location.row = 20;
								}
								else 
								{
									highlight_unit_location.row --;
								}
							}	
							else if (content_number == 150)
							{
								if (memory_map_page_current == 0)
								{
									memory_map_page_current = 0;
								}
								else 
								{
									memory_map_page_current --;
								}
							} 
							else if (content_number == 152)
							{
								msr_highlight_position = (msr_highlight_position + 1) % 2;
							}
							break;

						// Down means to move the highlight unit downwoards or see the next page	
						case DOWN:	
							if (content_number < 150)
							{
								if (highlight_unit_location.row >= 20)
								{
									highlight_unit_location.row = 5;
								}
								else 
								{
									highlight_unit_location.row ++;
								}
							}
							else if (content_number == 150)
							{
								if (memory_map_page_current == memory_map_page_max)
								{
									memory_map_page_current = memory_map_page_max;
								}
								else 
								{
									memory_map_page_current ++;
								}
							}		
							else if (content_number == 152)
							{
								msr_highlight_position = (msr_highlight_position + 1) % 2;
							}
							break;

						// Left means to move the highlight unit left	
						case LEFT:
							if (content_number < 150)
							{
								if (highlight_unit_location.col == 0)
								{
									highlight_unit_location.col = highlight_unit_max_col;
								}
								else 
								{
									highlight_unit_location.col --;
								}
							}
							break;

						// Right means to move the highlight unit right	
						case RIGHT:
							if (content_number < 150)
							{
								if (highlight_unit_location.col >= highlight_unit_max_col)
								{
									highlight_unit_location.col = 0;
								}
								else 
								{
									highlight_unit_location.col ++;
								}
							}
							break;

						// Pageup means to move to previos device or page	
						case PAGEUP:
							if (content_number < 100)
							{
								if (content_number != 0)
								{
									content_number --;
									pcie_page_counter = 0;
								}
								else
								{
									content_number = max_pci_dev_quantity;
								}
							}
							else if ((content_number >= 100) && (content_number < 103))
							{
								if (content_number != 100)
								{
									content_number --;
								}
								else 
								{
									if (host_vendor_id == 0x1106 || host_vendor_id == 0x1d17)
									{
										content_number = 102;
									}
									else if (host_vendor_id == 0x8086)
									{
										content_number = 101;
									}
								}
							}
							else if (content_number == 103)
							{
								if (memory_base_address >= 0x100)
								{
									memory_base_address = memory_base_address - 0x100;
								}
							}
							else if (content_number == 105)
							{
								if (io_space_base_address >= 0x100)
								{
									io_space_base_address = io_space_base_address - 0x100;
								}
							}
							else if ((content_number >= 106) && (content_number <= 125))
							{
								if (content_number != 106)
								{
									content_number --;
									acpi_table_page_counter = 0;
								}
								else 
								{
									content_number = 106 + max_acpi_table_quantity;
								}
							}
							else if (content_number == 152)
							{
								if (msr_base_address >= 1)
								{
									msr_base_address --;
								}
							}
							break;

						// Pagedown means to move to next device or page	
						case PAGEDOWN:	
							if (content_number < 100)
							{
								if (content_number != max_pci_dev_quantity)
								{
									content_number ++;
									pcie_page_counter = 0;
								}
								else 
								{
									content_number = 0;
								}
								pci_config_space_scan (content_number);
							}
							else if ((content_number >= 100) && (content_number < 103))
							{
								unsigned int table_length;
								
								if (host_vendor_id == 0x1106)
								{
									table_length = 102;
								}
								else if (host_vendor_id == 0x8086)
								{
									table_length = 101;
								}
								if (content_number != table_length)
								{
									content_number ++;
								}
								else
								{
									content_number = 100;
								}
							}
							else if (content_number == 103)
							{
								if (memory_base_address <= 0xfffffdff)
								{
									memory_base_address = memory_base_address + 0x100;
								}
							}
							else if (content_number == 105)
							{
								if (io_space_base_address <= 0xfdff)
								{
									io_space_base_address = io_space_base_address + 0x100;
								}
							}
							else if ((content_number >= 106) && (content_number <= 125))
							{
								if (content_number != 106 + max_acpi_table_quantity)
								{
									content_number ++;
									acpi_table_page_counter = 0;
								}
								else
								{
									content_number = 106;
								}
							}
							else if (content_number == 152)
							{
								if (msr_base_address <= 0xfffffffe)
								{
									msr_base_address ++;
								}
							}
							break;

						// Upper page of data
						case CTRL_PAGEUP:
							if (content_number < 100)
							{
								if (pci_device_detail.device_type == 1)
								{
									if (pcie_page_counter > 0)
									{
										pcie_page_counter --;
									}
								}
							}
							else if ((content_number >= 106) && (content_number < 126))
							{
								if (acpi_table_page_counter > 0)
								{
									acpi_table_page_counter --;
								}
							}
							else if (content_number == 126)
							{
								if (identify_word_page == 1)
								{
									identify_word_page --;
								}
							}
							break;

						// Last page of data
						case CTRL_PAGEDOWN:
							if (content_number < 100)
							{
								if (pci_device_detail.device_type == 1)
								{
									if (pcie_page_counter < 15)
									{
										pcie_page_counter ++;
									}
								}
							}
							else if ((content_number >= 106) && (content_number < 126))
							{
								if ((acpi_table_page_counter + 1) * 256 < acpi_header_detail.length)
								{
									acpi_table_page_counter ++;
								}
							}
							else if (content_number == 126)
							{
								if (identify_word_page == 0)
								{
									identify_word_page ++;
								}
							}
							break;
						
						// Space to enable or disable the highlight unit detail	
						case SPACE:	
							if (content_number < 150)
							{
								display_control.binary_xfer = (display_control.binary_xfer + 1) % 2;
							}
							break;
						default : break;
					}
					break;

				//**********************************************
				// Alt F Menu operation
				//**********************************************	
				case 2:	
					switch (key_ret)
					{

						// Up means to select the upper item
						case UP:	
							if (alt_f_function_number == 0)
							{
								alt_f_function_number = 0;
							}
							else 
							{
								alt_f_function_number --;
							}
							menu_direction.alt_f = 0;
							break;

						// Down means to select the next item	
						case DOWN:	
							if (alt_f_function_number != 9)
							{
								alt_f_function_number ++;
							}
							menu_direction.alt_f = 1;
							break;

						// Right means to move to the Alt-R Menu
						case RIGHT:
							display_control.main_refresh = 1; 	
							refresh_main_interface ();
							display_control.display_focus = 3;
							alt_r_function_number = 0;
							menu_direction.alt_r = 0;
							menu_interface(alt_r_menu_start.row,alt_r_menu_start.col,alt_r_menu_end.row,alt_r_menu_end.col);
							menu_row_operation (alt_r_function_number,menu_direction.alt_r,alt_r_menu_start.row,alt_r_menu_start.col,alt_r_menu_end.col);
							break;

						// ESC means to exit the current menu, that is turn to main menu	
						case ESC: 	
							display_control.display_focus = 1;
							break;

						// Enter means to select the current item and enter the corresponding function	
						case ENTER:	
							switch(alt_f_function_number)
							{
								// Memory Test
								case 0:
									display_control.display_focus = 1;
									content_number = 151;
									break;

								case 1:
									display_control.display_focus = 1;
									content_number = 154;
									break;

								case 9:
									row_color_set(zone_start_1.row,zone_start_1.col,zone_end_7.row,zone_end_7.col,color_white,color_black);
									cursor_move(0,0);
									asm int 0x19;
									break;
								
								default:
									display_control.display_focus = 1;
									break;	
							}							
							break;
							
						default : break;
					}
					break;

				//**********************************************
				// Alt R Menu operation
				//**********************************************		
				case 3:	
					switch (key_ret)
					{
						// Up means to select the upper item
						case UP:	
							if (alt_r_function_number == 0)
							{
								alt_r_function_number = 0;
							}
							else
							{
								alt_r_function_number --;
							}
							menu_direction.alt_r = 0;
							break;

						// Down means to select the next item	
						case DOWN:	
							if (alt_r_function_number != 14)
							{
								alt_r_function_number ++;
							}
							menu_direction.alt_r = 1;
							break;

						// Left means to move to the Alt-F Menu	
						case LEFT: 	
							display_control.main_refresh = 1;
							refresh_main_interface ();			
							display_control.display_focus = 2;
							alt_f_function_number = 0;
							menu_direction.alt_f = 0;
							menu_interface(alt_f_menu_start.row,alt_f_menu_start.col,alt_f_menu_end.row,alt_f_menu_end.col);
							menu_row_operation (alt_f_function_number,menu_direction.alt_f,alt_f_menu_start.row,alt_f_menu_start.col,alt_f_menu_end.col);
							break;

						// ESC means to exit the current menu, that is turn to main menu	
						case ESC: 	
							display_control.display_focus = 1;
							break;

						// Enter means to select the current item and enter the corresponding function
						case ENTER:	
							switch (alt_r_function_number)
							{

								// PCI device list
								case 0x0: 
									display_control.display_focus = 4;
									f6_function_number = 0;
									menu_direction.f6 = 0;
									scan_bdf ();
									F6_interface ();
									F6_row_operation (f6_function_number,menu_direction.f6);
									break;

								// CPUID	
								case 0x1: 
									display_control.display_focus = 1;	
									content_number = 100;
									break;

								// Memory Map	
								case 0x2: 
									display_control.display_focus = 1;
									content_number = 150;
									break;

								// SPD	
								case 0x3: 
									display_control.display_focus = 8;
									spd_menu_function_number = 0;
									menu_direction.spd = 0;
									menu_interface(spd_menu_start.row,spd_menu_start.col,spd_menu_end.row,spd_menu_end.col);
									menu_row_operation (spd_menu_function_number,menu_direction.spd,spd_menu_start.row,spd_menu_start.col,spd_menu_end.col);
									break;

								// IO Space
								case 0x4: 
									display_control.display_focus = 5;
									content_number_previous = content_number;
									content_number = 205;
									number_input_value = 0;
									break;

								// Memory
								case 0x5:
									display_control.display_focus = 5;
									content_number_previous = content_number;
									content_number = 203;
									number_input_value = 0;
									break;

								// ACPI table
								case 0x6:
									display_control.display_focus = 7;
									acpi_function_number = 0;
									menu_direction.acpi = 0;
									acpi_interface();
									acpi_row_operation (acpi_function_number, menu_direction.acpi);
									break;	

								// CPU MSR
								case 0x7:
									display_control.display_focus = 5;
									content_number_previous = content_number;
									content_number = 252;
									number_input_value = 0;
									break;

								// Identify Word
								case 0x8:
									display_control.display_focus = 9;
									hba_function_number = 0;
									menu_direction.hba = 0;
									menu_interface(hba_menu_start.row,hba_menu_start.col,hba_menu_end.row+max_hba_quantity,hba_menu_end.col);
									menu_row_operation (hba_function_number,menu_direction.hba,hba_menu_start.row,hba_menu_start.col,hba_menu_end.col);
									break;

								// Clock Frequency
								case 0x9:
									display_control.display_focus = 1;
									content_number = 153;
									break;
									
								default :break;
							}
							break;
						default : break;
					}
					break;
					
				//**********************************************
				// F6 Menu operation
				//**********************************************	
				case 4:
					switch (key_ret)
					{
						// Up means to select the upper item
						case UP:	
							if (f6_function_number == 0)
							{
								f6_function_number = 0;
							}
							else 
							{
								f6_function_number --;
							}
							menu_direction.f6 = 0;
							break;

						// Down means to select the next item	
						case DOWN:	
							if (f6_function_number == max_pci_dev_quantity)
							{
								f6_function_number = max_pci_dev_quantity;
							}
							else 
							{
								f6_function_number ++;
							}
							menu_direction.f6 = 1;
							break;

						// ESC means to exit the current menu, that is turn to main menu	
						case ESC: 	
							display_control.display_focus = 1;
							break;

						// Enter means to select the current item and enter the corresponding function	
						case ENTER:	
							display_control.display_focus = 1;
							pcie_page_counter = 0;
							content_number = f6_function_number;
							break;
						default : break;
					}
					break;

				//**********************************************
				// Number input Interface
				//**********************************************
				case 5:
					switch(key_ret)
					{
						case NO0:
						case NUM0: 
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 0;
							}
							break;
						case NO1:
						case NUM1: 
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 1;
							}	
							break;
						case NO2:
						case NUM2: 
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 2;
							}	
							break;
						case NO3:
						case NUM3: 
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 3;
							}	
							break;
						case NO4:
						case NUM4: 
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 4;
							}	
							break;
						case NO5:
						case NUM5: 
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 5;
							} 	
							break;
						case NO6:
						case NUM6: 
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 6;
							}	
							break;
						case NO7:
						case NUM7: 
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 7;
							}
							break;
						case NO8:
						case NUM8: 
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 8;
							}	
							break;
						case NO9:
						case NUM9: 
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 9;
							}	
							break;
						case CHARa:
						case CHARA:	
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 10;
							}
							break;
						case CHARb:
						case CHARB:	
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 11;
							}
							break;
						case CHARc:
						case CHARC:	
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 12;
							}	
							break;
						case CHARd:
						case CHARD:	
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 13;
							}
							break;
						case CHARe:
						case CHARE:	
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 14;
							}
							break;
						case CHARf:
						case CHARF:	
							if ((number_input_value << ((8-number_input_width)*4)) <= 0xfffffff)
							{
								number_input_value = (number_input_value << 4) & 0xfffffff0;
								number_input_value += 15;
							}
							break;
						case BACKSPACE:
							number_input_value = (number_input_value >> 4) & 0xfffffff;
							break;
						case ESC:
							display_control.display_focus = 1;
							content_number = content_number_previous;
							break;
						case ENTER:
							number_input(content_number);
							break;
						default : break;
					}
					break;
				
				//**********************************************
				// Error Reminder Interface
				//**********************************************
				case 6:
					switch (key_ret)
					{
						case ENTER:
						case ESC:
							if ((content_number == 203) || (content_number == 205) || (content_number == 252))
							{
								content_number = content_number_previous;
							}
							display_control.display_focus = 1;
							break;
						default : break;
					}
					break;

				//**********************************************
				// ACPI Table selection operation
				//**********************************************	
				case 7:
					switch (key_ret)
					{
						// Up means to select the upper item
						case UP:	
							if (acpi_function_number == 0)
							{
								acpi_function_number = 0;
							}
							else 
							{
								acpi_function_number --;
							}
							menu_direction.acpi = 0;
							break;

						// Down means to select the next item	
						case DOWN:	
							if (acpi_function_number >= max_acpi_table_quantity)
							{
								acpi_function_number = max_acpi_table_quantity;
							}
							else 
							{
								acpi_function_number ++;
							}
							menu_direction.acpi = 1;
							break;

						// ESC means to exit the current menu, that is turn to main menu	
						case ESC: 	
							display_control.display_focus = 1;
							break;

						// Enter means to select the current item and enter the corresponding function	
						case ENTER:	
							display_control.display_focus = 1;
							acpi_table_page_counter = 0;
							content_number = 106 + acpi_function_number;
							break;
							
						default : break;
					}
					break;

				//**********************************************
				// SPD menu selection operation
				//**********************************************	
				case 8:
					switch (key_ret)
					{

						// Up means to select the upper item
						case UP:	
							if (spd_menu_function_number == 0)
							{
								spd_menu_function_number = 0;
							}
							else 
							{
								spd_menu_function_number --;
							}
							menu_direction.spd = 0;
							break;

						// Down means to select the next item	
						case DOWN:	
							if (spd_menu_function_number != 7)
							{
								spd_menu_function_number ++;
							}
							menu_direction.spd = 1;
							break;

						// Left&ESC means to turn back to Alt-R menu
						case LEFT:
						case ESC:
							display_control.main_refresh = 1;
							refresh_main_interface();
							display_control.display_focus = 3;
							menu_interface(alt_r_menu_start.row,alt_r_menu_start.col,alt_r_menu_end.row,alt_r_menu_end.col);
							menu_row_operation (alt_r_function_number,menu_direction.alt_r,alt_r_menu_start.row,alt_r_menu_start.col,alt_r_menu_end.col);
							break;

						// Enter means to select the current item and enter the corresponding function	
						case ENTER:	
							display_control.display_focus = 1;
							content_number = 104;
							break;	
						default : break;
					}
					break;

				//**********************************************
				// HBA selection operation
				//**********************************************	
				case 9:
					switch (key_ret)
					{

						// Up means to select the upper item
						case UP:	
							if (hba_function_number == 0)
							{
								hba_function_number = 0;
							}
							else 
							{
								hba_function_number --;
							}
							menu_direction.hba = 0;
							break;

						// Down means to select the next item	
						case DOWN:	
							if (hba_function_number != max_hba_quantity)
							{
								hba_function_number ++;
							}
							menu_direction.hba = 1;
							break;

						// Left&ESC means to turn back to Alt-R menu
						case LEFT:
						case ESC:
							display_control.main_refresh = 1;
							refresh_main_interface();
							display_control.display_focus = 3;
							menu_interface(alt_r_menu_start.row,alt_r_menu_start.col,alt_r_menu_end.row,alt_r_menu_end.col);
							menu_row_operation (alt_r_function_number,menu_direction.alt_r,alt_r_menu_start.row,alt_r_menu_start.col,alt_r_menu_end.col);
							break;

						// Enter means to select the current item and enter the corresponding function	
						case ENTER:
							hba_register_scan(hba_function_number);
							if (host_bus_adapter[hba_function_number].nhf == 1)
							{
								display_control.display_focus = 10;
								sata_port_function_number = 0;
								menu_direction.sata = 0;
								menu_interface(sata_port_menu_start.row+hba_function_number, sata_port_menu_start.col, sata_port_menu_end.row+max_sata_port_quantity+hba_function_number, sata_port_menu_end.col);
								menu_row_operation (sata_port_function_number, menu_direction.sata, sata_port_menu_start.row+hba_function_number, sata_port_menu_start.col,sata_port_menu_end.col);
							}
							break;	
						default : break;
					}
					break;

				//**********************************************
				// SATA port selection operation
				//**********************************************	
				case 10:
					switch (key_ret)
					{

						// Up means to select the upper item
						case UP:	
							if (sata_port_function_number == 0)
							{
								sata_port_function_number = 0;
							}
							else 
							{
								sata_port_function_number --;
							}
							menu_direction.sata = 0;
							break;

						// Down means to select the next item	
						case DOWN:	
							if (sata_port_function_number != max_sata_port_quantity)
							{
								sata_port_function_number ++;
							}
							menu_direction.sata = 1;
							break;

						// Left&ESC means to turn back to Alt-R menu
						case LEFT:
						case ESC:
							display_control.main_refresh = 1;
							refresh_main_interface();
							display_control.display_focus = 3;
							menu_interface(alt_r_menu_start.row,alt_r_menu_start.col,alt_r_menu_end.row,alt_r_menu_end.col);
							menu_row_operation (alt_r_function_number, menu_direction.alt_r, alt_r_menu_start.row, alt_r_menu_start.col,alt_r_menu_end.col);
							display_control.display_focus = 9;
							menu_interface(hba_menu_start.row,hba_menu_start.col,hba_menu_end.row+max_hba_quantity,hba_menu_end.col);
							menu_row_operation (hba_function_number, menu_direction.hba, hba_menu_start.row, hba_menu_start.col,hba_menu_end.col);
							break;

						// Enter means to select the current item and enter the corresponding function	
						case ENTER:	
							if (host_bus_adapter[hba_function_number].mode <= 1)
							{
								display_control.display_focus = 1;
								content_number = 126;
								identify_word_page = 0;
								identify_word_scan(sata_port_function_number);
							}
							else if (host_bus_adapter[hba_function_number].mode == 2)
							{
								if (sata_port_mem_reg[sata_port_function_number].det & 0x1 == 1)
								{
									display_control.display_focus = 1;
									content_number = 126;
									identify_word_page = 0;
									identify_word_scan(sata_port_function_number);
								}
								else
								{
									strcpy(error_reminder_content, "No Device Detected");
									display_control.display_focus = 6;
								}
							}						
							break;	
						default : break;
					}
					break;
					
				default : break;	
			}

			if (display_control.display_focus == 1)
			{
				display_control.main_refresh = 1;
				goto main_refresh_;
			}
			else
			{
				// Other_refresh_counter have to be large enough to make other interface look like static 
				display_control.other_refresh = 1;
			}
		}

			
		//******************************************************************************************************************
		// The display part
		//******************************************************************************************************************		
		switch(display_control.display_focus)
		{
			// The main interface
			case 1:
				if (timer_counter == 17)
				{
				main_refresh_:
					
					// Data Fetch 
					if (content_number < 100)
					{
						pci_config_space_scan(content_number);
					}
					else if ((content_number >= 100) && (content_number < 103))
					{
						cpuid_scan ();
					}
					else if ((content_number >= 106) && (content_number < 126))
					{
						acpi_table_scan(content_number - 106);
					}
					else
					{
						switch(content_number)
						{
							case 103:
								memory_array_read(memory_base_address);
								break;
							case 104:
								spd_read(spd_menu_function_number);
								strcpy(content_name_string_zone2,"Serial Presence Detect");
								data_storage_addr_table = data_stotage_array_256b;
								break;
							case 105:
								io_space_read(io_space_base_address);
								break;
							case 126:
								// DO NOT REFRESH the IDENTIFY WORD, IDE MODE NOT STABLE
								break;
							case 150:
								memory_map_E820(memory_map);
								memory_map_page_max = memory_map_counter[0] / 16;
								strcpy(content_name_string_zone2,"Memory Map");
								data_storage_addr_table = memory_map;
								break;
							case 151:
								strcpy(content_name_string_zone2,"Memory Test");
								break;
							case 152:
								cpu_msr_scan(msr_base_address);
								break;
							case 153:
								strcpy(content_name_string_zone2, "");
								break;
							case 154:
								cpu_monitor_info_read();
								break;
							default : break;
						}
					}

					// UI Refresh
					refresh_main_interface();
					cursor_move(2, 50);
				}
				break;

			// Alt-F menu	
			case 2:	
				if (display_control.other_refresh)
				{
					display_control.other_refresh = 0;
					menu_row_operation (alt_f_function_number, menu_direction.alt_f, alt_f_menu_start.row, alt_f_menu_start.col,alt_f_menu_end.col);
				}
				break;

			// Alt-R menu	
			case 3:	
				if (display_control.other_refresh)
				{
					display_control.other_refresh = 0;
					menu_row_operation (alt_r_function_number, menu_direction.alt_r, alt_r_menu_start.row, alt_r_menu_start.col,alt_r_menu_end.col);
				}
				break;

			// F6 menu	
			case 4:	
				if (display_control.other_refresh)
				{
					display_control.other_refresh = 0;
					F6_row_operation(f6_function_number, menu_direction.f6);
				}
				break;

			// Number input
			case 5:
				if (display_control.other_refresh)
				{
					display_control.other_refresh = 0;
					number_input_display(content_number);
				}
				break;

			// Error Remainder
			case 6:
				if (display_control.other_refresh)
				{
					display_control.other_refresh = 0;
					error_reminder (error_reminder_content);
				}
				break;

			// ACPI Table menu
			case 7:
				if (display_control.other_refresh)
				{
					display_control.other_refresh = 0;
					acpi_row_operation(acpi_function_number, menu_direction.acpi);
				}
				break;

			// SPD Menu
			case 8:
				if (display_control.other_refresh)
				{
					display_control.other_refresh = 0;
					menu_row_operation (spd_menu_function_number, menu_direction.spd, spd_menu_start.row, spd_menu_start.col,spd_menu_end.col);
				}
				break;

			// HBA Menu
			case 9:
				if (display_control.other_refresh)
				{
					display_control.other_refresh = 0;
					menu_row_operation (hba_function_number, menu_direction.hba, hba_menu_start.row, hba_menu_start.col,hba_menu_end.col);
				}
				break;
				
			// SATA Port menu
			case 10:
				if (display_control.other_refresh)
				{
					display_control.other_refresh = 0;
					menu_row_operation (sata_port_function_number, menu_direction.sata, sata_port_menu_start.row+hba_function_number, sata_port_menu_start.col,sata_port_menu_end.col);
				}
				break;
					
			default : break;
		}
	}
main_exit_:	
	setvect(0x1c,oldhandler);
	return 0;
}
