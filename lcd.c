#include "MKL46Z4.h"
#include "lcd.h"

// This is from User manual
// PTC17	: LCD_37
// PTB21 	: LCD_17
// PTB7		: LCD_7
// PTB8 	: LCD_8
// PTE5 	: LCD_53
// PTC18 	: LCD_38
// PTB10 	: LCD_10
// PTB11 	: LCD_11
// PTD0 	: LCD_40
// PTE4 	: LCD_52
// PTB23 	: LCD_19
// PTB19	: LCD_18
const char PIN_TABLE[] = {37, 17, 7, 8, 53, 38, 10, 11, 40, 52, 19, 18};

// Convert from Character to 7 segment Waveform
// Each character have CHARa and CHARb
char ASCII_TO_WF[] =
{
        (SEGD + SEGE + SEGF + !SEGG), (SEGC + SEGB + SEGA),       // Char = 0
        (!SEGD + SEGE + SEGF + !SEGG), (!SEGC + !SEGB + !SEGA),   // Char = 1
        (SEGD + SEGE + !SEGF + SEGG), (!SEGC + SEGB + SEGA),      // Char = 2
        (SEGD + SEGE + SEGF + SEGG), (!SEGC + !SEGB + SEGA),      // Char = 3
        (!SEGD + SEGE + SEGF + SEGG), (SEGC + !SEGB + !SEGA),     // Char = 4
        (SEGD + !SEGE + SEGF + SEGG), (SEGC + !SEGB + SEGA),      // Char = 5
        (SEGD + !SEGE + SEGF + SEGG), (SEGC + SEGB + SEGA),       // Char = 6
        (SEGD + SEGE + SEGF + !SEGG), (!SEGC + !SEGB + !SEGA),    // Char = 7
        (SEGD + SEGE + SEGF + SEGG), (SEGC + SEGB + SEGA),        // Char = 8
        (SEGD + SEGE + SEGF + SEGG), (SEGC + !SEGB + SEGA),       // Char = 9
		(!SEGD + !SEGE + !SEGF + !SEGG), (!SEGC + !SEGB + !SEGA), // Char = ':' (Blank)
};

void LCD_Init(void) {
	// Clock for LCD and PORT B, C, D, E
	SIM->SCGC5 |= SIM_SCGC5_SLCD_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
    // 1 << 19 | 1 << 10 | 1 << 11 | 1 << 12 | 1 << 13;

    // Enable Multiple Purpose CLock for LCD Alternative clock source
	MCG->C1 |= MCG_C1_IRCLKEN_MASK;		// 1 << 1;
	
    // Clock source (Multipurpose Clock Generator) and duty circle for 4 BP control
    LCD->GCR = LCD_GCR_SOURCE_MASK | LCD_GCR_DUTY(3);		// LCD->GCR = 1 << 6 | 0x03;
	
    // Enable LCD pins and Configure BackPlanes
	// Clear PEN and BPEN
    LCD->PEN[0] = 0x0;
    LCD->PEN[1] = 0x0;
    LCD->BPEN[0] = 0x0;
    LCD->BPEN[1] = 0x0;
	
	// Enable PIN for frontplane and backplane
	LCD->PEN[1] |= 0x20;			// PIN 37 for CHAR1a
	LCD->PEN[0] |= 0x20000;			// PIN 17 for CHAR1b
	LCD->PEN[0] |= 0x80;			// PIN 7 for CHAR2a
	LCD->PEN[0] |= 0x100;			// PIN 8 for CHAR2b
	LCD->PEN[1] |= 0x200000;		// PIN 53 for CHAR3a
	LCD->PEN[1] |= 0x40;			// PIN 38 for CHAR3b
	LCD->PEN[0] |= 0x400;			// PIN 10 for CHAR4a
	LCD->PEN[0] |= 0x800;			// PIN 11 for CHAR4b
	LCD->PEN[1] |= 0x100;			// PIN 40 for COM0
	LCD->PEN[1] |= 0x100000;		// PIN 52 for COM1
	LCD->PEN[0] |= 0x80000;			// PIN 19 for COM2
	LCD->PEN[0] |= 0x40000;			// PIN 18 for COM3
	
	// Enable backplane for COMs
	LCD->BPEN[1] |= 0x100;			// PIN 40 for COM0 
	LCD->BPEN[1] |= 0x100000;		// PIN 52 for COM1
	LCD->BPEN[0] |= 0x80000;		// PIN 19 for COM2
	LCD->BPEN[0] |= 0x40000;		// PIN 18 for COM3
	
	// Turn on all backplane
	LCD->WF8B[40] = 0x01;	//  Enable the 1st CHAR backplane
	LCD->WF8B[52] = 0x02;	//  Enable the 2nd CHAR backplane
	LCD->WF8B[19] = 0x04;	//  Enable the 3rd CHAR backplane
	LCD->WF8B[18] = 0x08;	//  Enable the 4th CHAR backplane
	
	// Enable LCD
	LCD->GCR |= LCD_GCR_LCDEN_MASK;	 // 1 << 7; 			
}

void LCD_WriteString(char *string) {
    char length = 0;
    while (length < 4 && *string)
    {
        LCD_WriteChar(*string++, length);
        length++;
    }

    if (length < 4)
    {	
        while (length++ < 4)
            LCD_WriteChar(':', length++);
    }
}

void LCD_WriteChar(char character, int LCD_CharPosition) {
	// Invalid position
	if (LCD_CharPosition >= 4){
		return;
	}
	
	// Invalid character -> replace with blank
    if (character < '0' || character > '9') {
        character = ':';
	}
	
	// Get position in ASCII to WF table
    int index = (character - '0') * 2;
	
	// CHARa (D, E, G, F) and CHARb(DP, C, B, A) Write
	char char_to_wf;
	int position;
    for (int i = 0; i < 2; i ++)
    {
		// Write to CHARa then CHARb
        position = (LCD_CharPosition) * 2 + i;
		
		// Get waveform value
        char_to_wf = ASCII_TO_WF[index + i];
		
		// Configure the waveform for CHARxa or CHARxb
        LCD->WF8B[PIN_TABLE[position]] = char_to_wf;
    }
}
