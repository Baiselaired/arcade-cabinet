 ______________
| HUB75E pinout|
| R1   | G1    |
| B1   | GND   |
| R2   | G2    |
| B2   | E     |
|  A   | B     |
|  C   | D     |
| CLK  | LAT   |
| OE   | GND   |
 --------------


/**************************************************************************************************
**                                         Working Pins                                          **
**************************************************************************************************/
// HUB75e -- the matrix
#define R1     26
#define G1     25
#define BL1    24
// Ground      //
#define R2     29
#define G2     28
#define BL2    27
#define PIN_E  A4     // **
#define PIN_A  A0
#define PIN_B  A1
#define PIN_C  A2
#define PIN_D  A3
#define CLK    11     // USE THIS ON ARDUINO MEGA
#define OE     9
#define LAT    10
// Ground      //

** Pin E is used for forwarding one matrix's input to another, relevant when chaining or for sizes 64x64 or larger. When not relevant, it can go really anywhere, ^
just be sure to give it a definition and plug it in somewhere it won't interact with much.


// These are just the documentation that WAS present
// It doesn't seem to be very accurate / usable

Default library pin configuration for the reference

#define R1 25
#define G1 26
#define BL1 27
#define R2 14
#define G2 12
#define BL2 13
#define CH_A 23
#define CH_B 19
#define CH_C 5
#define CH_D 17
#define CH_E -1 // assign to any available pin if using two panels or 64x64 panels with 1/32 scan
#define CLK 16
#define LAT 4
#define OE 15



Pin defs given in Matrix's MEGA hardware header

#define CLK 11
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3
#define E -1      // Would assumedly be A4, but is only used for daisy chaining

RGBs 1 and 2 are stated to require to be on the same 'port'


Pindefs that work for the demo:

// Pindef:
#define R1  25
#define G1  26
#define BL1 24
// Ground
#define R2  29
#define G2  28
#define BL2 27 
#define A   A0
#define B   A1
#define C   A2
#define D   A3
#define E   A4
#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
// Ground