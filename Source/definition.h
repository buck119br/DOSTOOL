#ifndef __DEFINITION_H
#define __DEFINITION_H

#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <bios.h>


/**********************************************************
***********************************************************
	VARIABLE DEFINITION
***********************************************************
**********************************************************/

//******************** 
//	Global Flags
//********************


// Global display control
struct 
{
	unsigned int main_refresh: 1;	// Main UI zone refresh flag, 0-OFF, 1-ON
	unsigned int other_refresh: 1;	// Interfaces except main UI, 0-OFF, 1-ON 
	unsigned int data_refresh: 1;	// This flag control timer_counter in order to control data refresh, 0-OFF, 1-ON
	unsigned int binary_xfer: 1;	// This flag enalbe/disable the Hex xfer to Binary function, 0-OFF, 1-ON
	unsigned int display_focus: 4;	// Define which interface the system is operating
									// 01 : Main Interface
									// 02 : Alt-F Menu
									// 03 : Alt-R Menu
									// 04 : F6 Interface
									// 05 : Number Input
									// 06 : Error Reminder
									// 07 : ACPI table select interface
									// 08 : SPD select menu
									// 09 : HBA select menu
									// 10 : SATA Port select menu
} display_control;


// Content_number represents what is showing or operating on main interface
// 000-099 	: The PCI device configuration space
// 100-102 	: CPUID
// 103		: Memory
// 104		: SPD
// 105 		: IO Space
// 106-125	: ACPI Table
// 126		: Identify Word
// 150		: Memory Map
// 151		: Memory Test
// 152		: CPU MSR
// 153		: Clock Frequency
// 199     	: The welcome interface
// 203		: Memory Address Input
// 205		: IO Space Address Input
// 251		: Memory Test suspend
// 252		: CPU MSR Address Input
unsigned int content_number;
unsigned int content_number_previous;

// This flag represents the vendor of the host bridge, such as Intel AMD or VIA
unsigned long host_vendor_id;

// PIT 18.2Hz
unsigned int timer_counter = 0;

// This flag represents the zone 4 boundary in mode 2
unsigned int highlight_unit_max_col;

// This flag represents the unit of the data display in the main interface(1:BYTE,2:WORD and 3:DWORD)
unsigned int data_display_unit;

// This flag represents the data width of each unit;
unsigned int data_display_unit_width;

// This flag stores the provisional value being operated
unsigned int number_input_width;
unsigned long number_input_value;

// This flag represents the maximum quantity of the PCI devices system has found
unsigned int max_pci_dev_quantity;

// This flag represents the maximum quantity of the ACPI tables system has configured 
unsigned int max_acpi_table_quantity;

// Menu function pointer
unsigned int f6_function_number;
unsigned int acpi_function_number;
unsigned int alt_f_function_number;
unsigned int alt_r_function_number;
unsigned int spd_menu_function_number;
unsigned int hba_function_number;
unsigned int sata_port_function_number;

// Menu direction flag: 0-up, 1-down
struct 
{
	unsigned int f6: 1;
	unsigned int acpi: 1;
	unsigned int alt_f: 1;
	unsigned int alt_r: 1;
	unsigned int spd: 1;
	unsigned int hba: 1;
	unsigned int sata: 1;
} menu_direction;


//******************** 
//	Data
//********************

// This table store the head of each data storage array
unsigned long * data_storage_addr_table;
unsigned long data_stotage_array_256b[64];

// This table store the function name of each content, which is displayed in zone 2
char content_name_string_zone2[30];

// This matrix stores the PCI config space 256 byte of each PCI device
// If array >= [100][64] memory will overflow, so I decided to use only one array and overwrite it again and again
unsigned long pcie_extended_base_address = 0;
unsigned int  pcie_page_counter = 0;

// This array stores what the error reminder is representing
unsigned char error_reminder_content[40];

// CPUID data storage array
unsigned long cpuid_info[3][64];

// Memory 256 byte data storage array base address
unsigned long memory_base_address;

// Serial Presence Detect data storage array base address
unsigned int  smb_base_addr;

// IO Space data storage array base address
unsigned long io_space_base_address;

// Memory map E820 data storage array and related flags
unsigned long memory_map[150];
unsigned int  memory_map_counter[1] = {0};
unsigned int  memory_map_page_max = 0;
unsigned int  memory_map_page_current = 0;

// ACPI Description table
unsigned long rsdp_base_address;
unsigned long xsdt_base_address;
unsigned int  acpi_table_page_counter = 0;

// CPU MSR data storage
unsigned long msr_info[2];
unsigned long msr_base_address;
unsigned int  msr_highlight_position;

// AHCI/IDE Identify word
unsigned int  max_hba_quantity;
unsigned int  max_sata_port_quantity;
unsigned int  sata_port_detect[8][2];
unsigned int  ide_port_info[4][2];
unsigned long ide_pci_base[4];
unsigned long identify_word[128];
unsigned int  identify_word_page;

// Clock Frequency
unsigned long hpet_mmio_base_addr;

// MACRO of pci device detail information
struct pci_dev_bdf_tag
{
	unsigned long bus;
	unsigned long device;
	unsigned long function;
};
struct pci_dev_bdf_tag pci_device_bdf[100];

struct
{
	unsigned long vendor_id;
	unsigned long device_id;
	unsigned long base_class;
	unsigned long sub_class;
	unsigned long interface_class;
	unsigned int  header_type;
	unsigned int  rev_id;
	unsigned int  int_line;
	unsigned int  int_pin;
	unsigned int  capability_list;
	unsigned int  device_type; // 0-PCI or 1-PCIE or 2-Nothing
	unsigned int  pcie_max_link_speed;
	unsigned int  pcie_max_link_width;
	unsigned int  pcie_current_link_speed;
	unsigned int  pcie_current_link_width;
	unsigned int  address_map_type[6];
	unsigned int  address_map_addr_length[6];
	unsigned long address_map_addr[6];
	char name_string[30];
} pci_device_detail;


// ACPI Description Header information
unsigned long acpi_header_ba[20];

struct
{
	char signature[5];	
	unsigned long length;
	char description[40];
} acpi_header_detail;


// DRAM SPD information
// Total memory capacity = SDRAM CAPACITY / 8 * PRIMARY BUS WIDTH / SDRAM WIDTH * RANKS
// Tck = Tck(MTB) * MTB + Tck(FTB) * FTB
struct
{
	char  dram_device_type[20];
	char  module_type[20];
	char  voltage[20];
	float total_capacity;
	float sdram_capacity;
	float ranks;
	float sdram_width;
	float primary_bus_width;
	int   total_bus_width;
	float ftb;
	int   ftb_hex;
	float mtb;
	int   mtb_hex;
	float tck;
	float speed;
	char  raw_card[20];
	unsigned int  module_manufacturer_lsb;
	unsigned int  module_manufacturer_msb;
	unsigned int  dram_manufacturer_lsb;
	unsigned int  dram_manufacrurer_msb;
	unsigned int  module_part_number[18];
	unsigned long module_serial_number;	
} spd_info_parsered;


// CPUID information
struct
{
	unsigned int  vendor_id[12];			// 0x00000000 : Read sequence EBX -> EDX -> ECX, LOW to HIGH
	unsigned long fms;						// 0x00000001 : EAX
	unsigned long count;					// 0x00000001 : EBX bit 16-23
	unsigned long feature_flags[2];			// 0x00000001 : ECX EDX
	unsigned long extended_feature_flags[2];// 0x80000001 : ECX EDX
	unsigned int  name_string[48];			// 0x80000002 + 0x80000003 + 0x80000004 : EAX -> EDX, LOW to HIGH
	unsigned long cache_1_size;				// 0x80000005 : ECX bit 24-31 kB(VIA)
	unsigned long cache_2_size;				// 0x80000006 : ECX bit 16-31 kB
	unsigned long cache_3_size;				// 
	unsigned int  phy_addr_size;			// 0x80000008 : EAX bit 0-7
	unsigned int  virt_addr_size;			// 0x80000008 : EAX bit 8-15
} cpuid_info_parsered;


// HBA Configuration and Memory Register
// The following registers is named according to the AHCI specification, but not all
struct HBA_REGISTER
{
	unsigned int  mode;	// 0:IDE(compatibility) 1:IDE(native) or 2:AHCI
	unsigned int  nhf;	// Native Host Bus Adapter or not
	unsigned int  vid;	// Vendor ID
	unsigned long abar;	// AHCI Base Address
	unsigned int  iss;	// Interface Speed Support
	unsigned int  np;	// Number of Ports
	unsigned long pi;	// Ports Implemented
	unsigned int  vs[3];// AHCI Version	
};
struct HBA_REGISTER host_bus_adapter[5];


// SATA Port AHCI Register
// The following registers is named according to the AHCI specification, but not all
struct SATA_PORT_REGISER
{
	unsigned long clb; 	// Command List Base Address
	unsigned long fb;	// FIS Base Address
	unsigned long sig;	// Signature
	unsigned int  cspd;	// Current Interface Speed
	unsigned int  det;	// Device Detection
	unsigned int  aspd; // Allowed Speed
};
struct SATA_PORT_REGISER sata_port_mem_reg[8];

// IDENTIFY DEVICE INFORMATION
struct
{
	unsigned int cylinder;			// Number of logical cylinders
	unsigned int headers;			// Number of logical headers
	unsigned int sectors;			// Number of logical sectors
	unsigned int serial_no[20];
	unsigned int model_number[40];
	unsigned int queue_depth;
	unsigned int sata_speed;
	unsigned int ata_speed;
} identify_device_information;

// Clock Frequency
struct
{
	unsigned long tsc[18];			// Time_Stamp Counter 
	unsigned long pit[19];			// Programmable Interval Timer
	unsigned long hpet;				// High Precision Event Timer
} clock_frequency;

/**********************************************************
***********************************************************
	FUNCTION DEFINITION
***********************************************************
**********************************************************/

//******************** 
//	Data.h
//********************
void out_port(unsigned int, unsigned long);
unsigned long in_port(unsigned int);
void out_port_word (unsigned int, unsigned int);
unsigned int in_port_word (unsigned int);
void out_port_byte(unsigned int, unsigned int);
unsigned int in_port_byte(unsigned int);

void scan_bdf(void);
void pci_dev_header_scan(unsigned int);
void pci_config_space_scan(unsigned int);

unsigned long rsdp_research(void);
unsigned long rsdp_locate(unsigned long, unsigned long);
void acpi_header_ba_scan(void);
void acpi_header_detail_scan(unsigned int);
void acpi_table_scan(unsigned int);

unsigned long memory_read(unsigned long);
void memory_write(unsigned long, unsigned long);
unsigned int memory_read_byte(unsigned long);
void memory_write_byte(unsigned long, unsigned long);
void memory_array_read(unsigned long);

void cpuid_scan(void);
void cpuid_read(unsigned long *, unsigned long, unsigned long);

void memory_map_E820(unsigned long *);

void spd_read(unsigned int);
unsigned int smbus_read_byte(unsigned int, unsigned int, unsigned int);

void io_space_read(unsigned long);

void msr_read(unsigned long, unsigned long *);
void msr_write(unsigned long, unsigned long, unsigned long);
void cpu_msr_scan(unsigned long);

void hba_register_scan(unsigned int);
void ide_io_port_set(unsigned int, unsigned int);
void sata_port_register_scan(void);
void ide_identify(unsigned int);
void ahci_identify(unsigned int);
void identify_word_scan(unsigned int);

void clock_frequency_read(void);
unsigned int pit_read(void);

unsigned long data_storage_array_write(unsigned int);


//******************** 
//	Display.h
//********************
void display_init(void);
void row_color_set(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void clear_screen(void);

void display_zone1(void);
void display_zone2(void);
void display_zone3(unsigned int);
void display_zone4(unsigned int);
void display_zone5(void);
void display_zone6(unsigned int);
void display_zone7(unsigned int);

void refresh_main_interface(void);
void content_display_mode_2(unsigned long *);

void F6_row_print(unsigned int);
void F6_interface(void);
void F6_row_operation(unsigned int, unsigned int);

void acpi_row_print(unsigned int);
void acpi_interface(void);
void acpi_row_operation(unsigned int, unsigned int);

void menu_interface(unsigned int, unsigned int, unsigned int, unsigned int);
void menu_row_operation(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);

void welcome_interface_display(void);

void number_input_display(unsigned int);

void error_reminder(char *);

void memory_map_display(unsigned int);

void memory_test_display(void);

void message_window_display(unsigned int, unsigned int, unsigned int, unsigned int);

void msr_display(void);

void clock_frequency_display(void);

void spd_parser_display(void);

void cpuid_parser_display(void);

void pci_cfg_spc_parser_display(void);

void identify_word_parser_display(void);


//******************** 
//	Function.h
//********************
void interrupt ( * oldhandler)(...);
void interrupt counter(...);
void cursor_move(unsigned int, unsigned int);
void delay(unsigned int);
void number_input(unsigned int);
void memory_test(void);


//******************** 
//	Parser.h
//********************
void bdf_parser (void);
void acpi_signature_parser(unsigned int);
void spd_parser(unsigned long *);
void cpuid_parser(void);
void pci_config_space_parser(void);
void identify_word_parser(void);


#endif
