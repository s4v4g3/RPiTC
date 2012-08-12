/*---------------------------------------------------------------------------
;
;
;
;
; FILE NAME: C8051F350.H
; TARGET MCUs: C8051F350, F350, F351, F352, F353
; DESCRIPTION: Register/bit definitions for the C8051F350 product family.
;
; REVISION 1.1
;
;---------------------------------------------------------------------------*/

#include <REG52.h>

// TODO:  convert to use SFR(name, addr) and SBIT(name, addr, bit) macros
// to be compiler-agnostic

/*  BYTE Registers  */
sfr CKCON         = 0x8E; /* CLOCK CONTROL */
sfr PSCTL         = 0x8F; /* PROGRAM STORE R/W CONTROL */
sfr TMR3CN        = 0x91; /* TIMER/COUNTER 3CONTROL */
sfr TMR3RLL       = 0x92; /* TIMER/COUNTER 3 RELOAD LOW */
sfr TMR3RLH       = 0x93; /* TIMER/COUNTER 3 RELOAD HIGH */
sfr TMR3L         = 0x94; /* TIMER/COUNTER 3 LOW */
sfr TMR3H         = 0x95; /* TIMER/COUNTER 3 HIGH */
sfr IDA0          = 0x96; /* CURRENT MODE DAC0 LOW */
sfr SCON0         = 0x98; /* UART0 CONTROL */
sfr SBUF0         = 0x99; /* UART0 DATA BUFFER */
sfr ADC0DECL      = 0x9A; /* ADC0 DECIMATION LOW */
sfr ADC0DECH      = 0x9B; /* ADC0 DECIMATION HIGH */
sfr CPT0CN        = 0x9C; /* COMPARATOR0 CONTROL */
sfr CPT0MD        = 0x9D; /* COMPARATOR0 MODE SELECTION */
sfr CPT0MX        = 0x9F; /* COMPARATOR0 MUX SELECTION */
sfr SPI0CFG       = 0xA1; /* SPI CONFIGURATION */
sfr SPI0CKR       = 0xA2; /* SPI CLOCK RATE CONTROL */
sfr SPI0DAT       = 0xA3; /* SPI DATA */
sfr P0MDOUT       = 0xA4; /* PORT 0 OUTPUT MODE CONFIGURATION */
sfr P1MDOUT       = 0xA5; /* PORT 1 OUTPUT MODE CONFIGURATION */
sfr P2MDOUT       = 0xA6; /* PORT 2 OUTPUT MODE CONFIGURATION */
sfr CLKSEL        = 0xA9; /* CLOCK SELECT */
sfr EMI0CN        = 0xAA; /* EXTERNAL MEMORY INTERFACE CONTROL */
sfr ADC0CGL       = 0xAB; /* ADC0 GAIN CALIBRATION LOW */
sfr ADC0CGM       = 0xAC; /* ADC0 GAIN CALIBRATION MIDDLE */
sfr ADC0CGH       = 0xAD; /* ADC0 GAIN CALIBRATION HIGH */
sfr OSCXCN        = 0xB1; /* EXTERNAL OSCILLATOR CONTROL */
sfr OSCICN        = 0xB2; /* INTERNAL OSCILLATOR CONTROL */
sfr OSCICL        = 0xB3; /* INTERNAL OSCILLATOR CALIBRATION */
sfr FLSCL         = 0xB6; /* FLASH SCALE */
sfr FLKEY         = 0xB7; /* FLASH LOCK AND KEY  */
sfr IDA0CN        = 0xB9; /* CURRENT MODE DAC0 CONTROL */
sfr ADC0COL       = 0xBA; /* ADC0 OFFSET CALIBRATION LOW */
sfr ADC0COM       = 0xBB; /* ADC0 OFFSET CALIBRATION MIDDLE */
sfr ADC0COH       = 0xBC; /* ADC0 OFFSET CALIBRATION HIGH */
sfr ADC0BUF       = 0xBD; /* ADC0 BUFFER CONTROL */
sfr CLKMUL        = 0xBE; /* CLOCK MULTIPLIER */
sfr ADC0DAC       = 0xBF; /* ADC0 OFFSET DAC */
sfr SMB0CN        = 0xC0; /* SMBUS CONTROL */
sfr SMB0CF        = 0xC1; /* SMBUS CONFIGURATION */
sfr SMB0DAT       = 0xC2; /* SMBUS DATA */
sfr ADC0L         = 0xC3; /* ADC0 OUTPUT LOW */
sfr ADC0M         = 0xC4; /* ADC0 OUTPUT MIDDLE */
sfr ADC0H         = 0xC5; /* ADC0 OUTPUT HIGH */
sfr ADC0MUX       = 0xC6; /* ADC0 MULTIPLEXER */
sfr TMR2CN        = 0xC8; /* TIMER/COUNTER 2 CONTROL */
sfr TMR2RLL       = 0xCA; /* TIMER/COUNTER 2 RELOAD LOW */
sfr TMR2RLH       = 0xCB; /* TIMER/COUNTER 2 RELOAD HIGH */
sfr TMR2L         = 0xCC; /* TIMER/COUNTER 2 LOW */
sfr TMR2H         = 0xCD; /* TIMER/COUNTER 2 HIGH */
sfr REF0CN        = 0xD1; /* VOLTAGE REFERENCE CONTROL */
sfr P0SKIP        = 0xD4; /* PORT 0 SKIP */
sfr P1SKIP        = 0xD5; /* PORT 1 SKIP */
sfr IDA1CN        = 0xD7; /* CURRENT MODE DAC1 CONTROL */
sfr PCA0CN        = 0xD8; /* PCA CONTROL */
sfr PCA0MD        = 0xD9; /* PCA MODE  */
sfr PCA0CPM0      = 0xDA; /* PCA MODULE 0 MODE */
sfr PCA0CPM1      = 0xDB; /* PCA MODULE 1 MODE */
sfr PCA0CPM2      = 0xDC; /* PCA MODULE 2 MODE */
sfr IDA1          = 0xDD; /* CURRENT MODE DAC1 LOW */
sfr XBR0          = 0xE1; /* PORT I/O CROSSBAR CONTROL 0 */
sfr XBR1          = 0xE2; /* PORT I/O CROSSBAR CONTROL 1 */
sfr PFE0CN        = 0xE3; /* PREFETCH ENGINE CONTROL */
sfr IT01CF        = 0xE4; /* INT0/INT1 CONFIGURATION */
sfr EIE1          = 0xE6; /* EXTENDED INTERRUPT ENABLE 1 */
sfr ADC0STA       = 0xE8; /* ADC0 STATUS */
sfr PCA0CPL0      = 0xE9; /* PCA CAPTURE 0 LOW */
sfr PCA0CPH0      = 0xEA; /* PCA CAPTURE 0 HIGH */
sfr PCA0CPL1      = 0xEB; /* PCA CAPTURE 1 LOW */
sfr PCA0CPH1      = 0xEC; /* PCA CAPTURE 1 HIGH */
sfr PCA0CPL2      = 0xED; /* PCA CAPTURE 2 LOW */
sfr PCA0CPH2      = 0xEE; /* PCA CAPTURE 2 HIGH */
sfr RSTSRC        = 0xEF; /* RESET SOURCE CONFIGURATION/STATUS */
sfr P0MDIN        = 0xF1; /* PORT 0 INPUT MODE CONFIGURATION */
sfr P1MDIN        = 0xF2; /* PORT 1 INPUT MODE CONFIGURATION */
sfr ADC0MD        = 0xF3; /* ADC0 MODE */
sfr ADC0CN        = 0xF4; /* ADC0 CONTROL */
sfr EIP1          = 0xF6; /* EXTENDED INTERRUPT PRIORITY 1 */
sfr ADC0CLK       = 0xF7; /* ADC0 CLOCK */
sfr SPI0CN        = 0xF8; /* SPI CONTROL */
sfr PCA0L         = 0xF9; /* PCA COUNTER LOW */
sfr PCA0H         = 0xFA; /* PCA COUNTER HIGH */
sfr ADC0CF        = 0xFB; /* ADC0 CONFIGURATION */
sfr ADC0FL        = 0xFC; /* ADC0 FAST FILTER OUTPUT LOW */
sfr ADC0FM        = 0xFD; /* ADC0 FAST FILTER OUTPUT MIDDLE */
sfr ADC0FH        = 0xFE; /* ADC0 FAST FILTER OUTPUT HIGH */
sfr VDM0CN        = 0xFF; /* VDD MONITOR CONTROL */


/* Bit Definitions */

/* SCON0 0x98 */
sbit S0MODE =     0x9F ;  /* UART 0 MODE */
sbit MCE0 =       0x9D ;  /* UART 0 MCE */
sbit REN0 =       0x9C ;  /* UART 0 RX ENABLE */
sbit TB80 =       0x9B ;  /* UART 0 TX BIT 8 */
sbit RB80 =       0x9A ;  /* UART 0 RX BIT 8 */
sbit TI0 =        0x99 ;  /* UART 0 TX INTERRUPT FLAG */
sbit RI0 =        0x98 ;  /* UART 0 RX INTERRUPT FLAG */

/* IE 0xA8 */
sbit ESPI0 =      0xAE ;  /* SPI0 INTERRUPT ENABLE */
sbit ES0  =       0xAC ;  /* UART0 INTERRUPT ENABLE */

/* IP 0xB8 */
sbit PSPI0 =      0xBE ;  /* SPI0 PRIORITY */
sbit PS0 =        0xBC ;  /* UART0 PRIORITY */

/* SMB0CN 0xC0 */
sbit MASTER =     0xC7 ;  /* SMBUS 0 MASTER/SLAVE */
sbit TXMODE =     0xC6 ;  /* SMBUS 0 TRANSMIT MODE */
sbit STA  =       0xC5 ;  /* SMBUS 0 START FLAG */
sbit STO =        0xC4 ;  /* SMBUS 0 STOP FLAG */
sbit ACKRQ =      0xC3 ;  /* SMBUS 0 ACKNOWLEDGE REQUEST */
sbit ARBLOST =    0xC2 ;  /* SMBUS 0 ARBITRATION LOST */
sbit ACK =        0xC1 ;  /* SMBUS 0 ACKNOWLEDGE FLAG */
sbit SI =         0xC0 ;  /* SMBUS 0 INTERRUPT PENDING FLAG */

/* TMR2CN 0xC8 */
sbit TF2H =       0xCF ;  /* TIMER 2 HIGH BYTE OVERFLOW FLAG */
sbit TF2L =       0xCE ;  /* TIMER 2 LOW BYTE OVERFLOW FLAG */
sbit TF2LEN =     0xCD ;  /* TIMER 2 LOW BYTE INTERRUPT ENABLE */
sbit T2SPLIT =    0xCB ;  /* TIMER 2 SPLIT MODE ENABLE */
sbit T2XCLK =     0xC8 ;  /* TIMER 2 EXTERNAL CLOCK SELECT */

/* PSW 0xD0 */
sbit F1 =         0xD1 ;  /* USER FLAG 1 */

/* PCA0CN 0xD8 */
sbit CF =         0xDF ;  /* PCA 0 COUNTER OVERFLOW FLAG */
sbit CR =         0xDE ;  /* PCA 0 COUNTER RUN CONTROL BIT */
sbit CCF2 =       0xDA ;  /* PCA 0 MODULE 2 INTERRUPT FLAG */
sbit CCF1 =       0xD9 ;  /* PCA 0 MODULE 1 INTERRUPT FLAG */
sbit CCF0 =       0xD8 ;  /* PCA 0 MODULE 0 INTERRUPT FLAG */
                          /* ADC 0 WINDOW INTERRUPT FLAG */
/* ADC0STA 0xE8 */
sbit AD0BUSY =    0xEF ;  /* ADC 0 CONVERSION IN PROGRESS FLAG */
sbit AD0CBSY =    0xEE ;  /* ADC 0 CALIBRATION IN PROGRESS FLAG */
sbit AD0INT =     0xED ;  /* ADC 0 CONVERSION COMPLETE FLAG */
sbit AD0S3C =     0xEC ;  /* ADC 0 SINC3 FILTER ERROR FLAG */
sbit AD0FFC =     0xEB ;  /* ADC 0 FAST FILTER ERROR FLAG */
sbit AD0CALC =    0xEA ;  /* ADC 0 CALIBRATION COMPLETE FLAG */
sbit AD0ERR =     0xE9 ;  /* ADC 0 ERROR FLAG */
sbit AD0OVR =     0xE8 ;  /* ADC 0 OVERRUN FLAG */

/* SPI0CN 0xF8 */
sbit SPIF =       0xFF ;  /* SPI 0 INTERRUPT FLAG */
sbit WCOL =       0xFE ;  /* SPI 0 WRITE COLLISION FLAG */
sbit MODF =       0xFD ;  /* SPI 0 MODE FAULT FLAG */
sbit RXOVRN =     0xFC ;  /* SPI 0 RX OVERRUN FLAG */
sbit NSSMD1 =     0xFB ;  /* SPI 0 SLAVE SELECT MODE 1 */
sbit NSSMD0 =     0xFA ;  /* SPI 0 SLAVE SELECT MODE 0 */
sbit TXBMT =      0xF9 ;  /* SPI 0 TX BUFFER EMPTY FLAG */
sbit SPIEN =      0xF8 ;  /* SPI 0 SPI ENABLE */

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions
//-----------------------------------------------------------------------------

SFR16(DP     , 0x82);	                 // data pointer
SFR16(TMR3RL , 0x92);                    // Timer3 reload value
SFR16(TMR3   , 0x94);                    // Timer3 counter
SFR16(ADC0DEC, 0x9a);					 // ADC0 decimation 
SFR16(TMR2RL , 0xca);                    // Timer2 reload value
SFR16(TMR2   , 0xcc);                    // Timer2 counter
SFR16(PCA0CP0, 0xe9);                    // PCA0 Module 1 Capture/Compare
SFR16(PCA0CP1, 0xeb);                    // PCA0 Module 2 Capture/Compare
SFR16(PCA0CP2, 0xed);                    // PCA0 Module 2 Capture/Compare
SFR16(PCA0   , 0xf9);                    // PCA0 counter
