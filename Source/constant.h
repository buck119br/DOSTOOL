#ifndef __CONSTANT_H
#define __CONSTANT_H


/**********************************************************
	PREPROCESSING
**********************************************************/

// BIOSKEY() return value return code table
// Combinition
#define ALT_F 		0X2100
#define ALT_R 		0X1300
#define CTRL_Q		0X1011
#define CTRL_PAGEUP		0X8400
#define CTRL_PAGEDOWN	0x7600

// Function Domain
#define F6			0X4000
#define F7			0X4100
#define F10			0X4400
#define UP			0X4800
#define DOWN		0X5000
#define LEFT		0X4B00
#define RIGHT		0X4D00
#define PAGEUP		0X4900
#define PAGEDOWN	0X5100
#define SPACE		0X3920
#define ESC			0X11B
#define ENTER		0X1C0D
#define BACKSPACE	0XE08
// Main Domain
#define NO1			0X231
#define NO2			0X332
#define NO3			0X433
#define NO4			0X534
#define NO5			0X635
#define NO6			0X736
#define NO7			0X837
#define NO8			0X938
#define NO9			0XA39
#define NO0			0XB30
#define CHARa		0X1E61
#define CHARb		0X3062
#define CHARc		0X2E63
#define CHARd		0X2064
#define CHARe		0X1265
#define CHARf		0X2166
#define CHARA		0X1E41
#define CHARB		0X3042
#define CHARC		0X2E43
#define CHARD		0X2044
#define CHARE		0X1245
#define CHARF		0X2146
// Number Domain
#define NUM1		0X4F31
#define NUM2		0X5032
#define NUM3		0X5133
#define NUM4		0X4B34
#define NUM5		0X4C35
#define NUM6		0X4D36
#define NUM7		0X4737
#define NUM8		0X4838
#define NUM9		0X4939
#define NUM0		0X5230

// PCI config space access
#define PCI_INDEX 	0xCF8
#define PCI_DATA  	0xCFC

//******************** 
//	Display
//********************

// color definitions
unsigned int color_black         = 0x0;	
unsigned int color_blue          = 0x1;
unsigned int color_green         = 0x2;
unsigned int color_cyan          = 0x3;
unsigned int color_red           = 0x4;
unsigned int color_magenta       = 0x5;
unsigned int color_brown         = 0x6;
unsigned int color_light_gray    = 0x7;
unsigned int color_dark_gray     = 0x8;
unsigned int color_light_blue    = 0x9;
unsigned int color_light_green   = 0xA;
unsigned int color_light_cyan    = 0xB;
unsigned int color_light_red     = 0xC;
unsigned int color_light_magenta = 0xD;
unsigned int color_yellow        = 0xE;
unsigned int color_white         = 0xF; 

// main_interface	           "<--------------------------------------80------------------------------------->"
char * main_menu    		 = "Function    Read                                                DOSTOOL Ver 2.0";
char * F6_menu    		 	 = "NO  VenID  DevID  Bus Dev Fun   Class                   Name                   ";
char * acpi_menu			 = "NO   Signature    BaseAddr    Length              Description                  "; 

char * main_tail  			 = "<Alt+F>-Function Menu <Alt+R>-Read Menu <Enter>-Input <F10>-Exit               ";
char * F6_tail    			 = "Press <Up> or <Down> to select item. <Enter> to read. <Esc> to quit            ";
char * acpi_tail			 = "Press <Up> or <Down> to select item. <Enter> to read. <Esc> to quit            ";
char * memory_map_tail  	 = "<Up> and <Down> to read upper or next page                                     ";
char * memory_test_tail 	 = "Press <ESC> to stop                                                            ";

// Welcome 
char welcome_content[20][80]=
{
  //"<--------------------------------------80------------------------------------->"
	"                              Dostool User Manual                              ",
	"                                                                               ",
	"  Feature                                                                      ",
	"  1. Read <Alt-R>                               2. Function <Alt-F>            ",
	"     |- PCI config space <F6>                      |- Memory Test (Up to 4GB)  ",
	"     |- CPUID                                      |-                          ",
	"     |- Memory Map(E820)                           |-                          ",
	"     |- Serial Presence Detect (Up to 8 DIMM)      |-                          ",
	"     |- IO space (Up to 0xFFFF R/W)                |-                          ",
	"     |- Memory (Up to 4GB R/W)                     |-                          ",
	"     |- ACPI configuration tables (XSDT)           |-                          ",
	"     |- CPU MSR (Single address R/W)               |- INT 19H                  ",
	"     |- IDE/AHCI Identify Word                                                 ",
	"     |- Clock Frequency                                                        ",
	"                                                                               ",
	"  Shortcuts |- <Space> : Toggle binary bitmap window                           ",
	"            |- <Enter> : Change the value of highlight (IO, Memory, MSR)       ",
	"            |- <F7>    : Change data presentation unit (BYTE, WORD, DWORD)     ",
	"            |- <F10>   : Quit                                                  ",
	"                                                   Contributors: Jason & Blue  "
};


// Function menu content
char function_menu_content[10][21]=
{
  //"                    ",20*char
	" Memory test ------ ",
	" CPU Monitor ------ ",
	" ------------------ ",
	" ------------------ ",
	" ------------------ ",
	" ------------------ ",
	" ------------------ ",
	" ------------------ ",
	" ------------------ ",
	" INT 19H ---------- "
};

char read_menu_content[15][21]=
{
  //"   			     ",20*char
	" PCI Config --- <F6 ",
	" CPUID ------------ ",
	" Memory Map ------- ",
	" SPD ----------- >> ",
	" IO Space --------- ",	
	" Memory ----------- ",
	" ACPI Table ------- ",
	" CPU MSR ---------- ",
	" Identify ------ >> ",
	" Clock Frequency -- ",
	" ------------------ ",
	" ------------------ ",
	" ------------------ ",
	" ------------------ ",
	" ------------------ "
};

char spd_menu_content[8][9]=
{
	" DIMM 1 ",
	" DIMM 2 ",
	" DIMM 3 ",
	" DIMM 4 ",
	" DIMM 5 ",
	" DIMM 6 ",
	" DIMM 7 ",
	" DIMM 8 "
};

char hba_menu_content[5][8]=
{
	" HBA 1 ",
	" HBA 2 ",
	" HBA 3 ",
	" HBA 4 ",
	" HBA 5 "
};

char sata_port_menu_content[8][8]=
{
	" PORT 1",
	" PORT 2",
	" PORT 3",
	" PORT 4",
	" PORT 5",
	" PORT 6",
	" PORT 7",
	" PORT 8"
};

// the starting and ending point of each area
struct point_location
{
	unsigned int row;
	unsigned int col;
};
struct point_location zone_start_1 = {0,0};
struct point_location zone_start_2 = {1,0};
struct point_location zone_start_3 = {4,0};
struct point_location zone_start_4 = {4,6};
struct point_location zone_start_5 = {4,56};
struct point_location zone_start_6 = {21,0};
struct point_location zone_start_7 = {24,0};
struct point_location zone_end_1 = {0,79};
struct point_location zone_end_2 = {3,79};
struct point_location zone_end_3 = {20,5};
struct point_location zone_end_4 = {20,55};
struct point_location zone_end_5 = {20,79};
struct point_location zone_end_6 = {23,79};
struct point_location zone_end_7 = {24,79};

struct point_location alt_f_menu_start 	= {1,0};
struct point_location alt_f_menu_end 	= {10,19};

struct point_location alt_r_menu_start 	= {1,12};
struct point_location alt_r_menu_end 	= {15,31};

struct point_location spd_menu_start	= {4,33};
struct point_location spd_menu_end		= {11,40};

struct point_location hba_menu_start	= {9,33};
struct point_location hba_menu_end		= {9,49};

struct point_location sata_port_menu_start	= {9,51};
struct point_location sata_port_menu_end	= {9,62};

struct point_location highlight_unit_location;

struct point_location number_input_start = {11,28};
struct point_location number_input_end	 = {13,51};

struct point_location msr_display_start	= {6,27};
struct point_location msr_display_end	= {17,52};

struct point_location clk_freq_display_start = {5,27};
struct point_location clk_freq_display_end   = {18,52};

struct point_location cpu_monitor_start = {6,27};
struct point_location cpu_monitor_end   = {17,52};


// Memory test pattern, same as Linux kernel memtest
unsigned long memory_test_pattern[16] =	{0xffffffff,
										 0x00000000,
										 0x55555555,
										 0xaaaaaaaa,
										 0x11111111,
										 0x22222222,
										 0x44444444,
										 0x88888888,
										 0x33333333,
										 0x66666666,
										 0x99999999,
										 0xcccccccc,
										 0x77777777,
										 0xbbbbbbbb,
										 0xdddddddd,
										 0xeeeeeeee};

// AHCI SATA Port Signature
#define	SATA_SIG_ATA	0x00000101	// SATA drive
#define	SATA_SIG_ATAPI	0xEB140101	// SATAPI drive
#define	SATA_SIG_SEMB	0xC33C0101	// Enclosure management bridge
#define	SATA_SIG_PM		0x96690101	// Port multiplier

#endif
