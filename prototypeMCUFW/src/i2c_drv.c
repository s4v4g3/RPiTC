/***************************************************************************//**

   @file     i2c_drv.c
   @date     8/12/2012

   @brief    Implementation of I2C (SMBus) driver

*******************************************************************************/


/*******************************************************************************
* INCLUDE FILES:
*******************************************************************************/
#include "platform_cfg.h"
#include "i2c_drv.h"
#include "types.h"
#include "globals.h"


/*******************************************************************************
* CONSTANTS:
*******************************************************************************/
#define  I2C_FREQUENCY  100000            // Target I2C frequency
#define  I2C_WRITE      0x00              // SMBus WRITE command
#define  I2C_READ       0x01              // SMBus READ command
#define  I2C_SLAVE_ADDR (0x78<<1)         // Device addresses (7 bits,
                                          // msb is a don't care)
// Status vector - top 4 bits only
#define  I2C_SRADD      0x20           // (SR) slave address received
                                       //    (also could be a lost
                                       //    arbitration)
#define  I2C_SRSTO      0x10           // (SR) STOP detected while SR or ST,
                                       //    or lost arbitration
#define  I2C_SRDB       0x00           // (SR) data byte received, or
                                       //    lost arbitration
#define  I2C_STDB       0x40           // (ST) data byte transmitted
#define  I2C_STSTO      0x50           // (ST) STOP detected during a
                                       //    transaction; bus error
// End status vector definition



/*******************************************************************************
* TYPES:
*******************************************************************************/

/*******************************************************************************
* GLOBAL VARIABLES:
*******************************************************************************/
SEGMENT_VARIABLE(_gI2C_InputBuffer[I2C_INPUT_BUFFER_SIZE], u8, SEG_IDATA);
SEGMENT_VARIABLE(_gI2C_OutputBuffer[I2C_OUTPUT_BUFFER_SIZE], u8, SEG_IDATA);
tBoolean _gI2C_DataReady;

/*******************************************************************************
* MACROS:
*******************************************************************************/
/* This Macro section should contain Macro functions used only in this module */
/******************************************************************************/

/*******************************************************************************
* LOCAL VARIABLES:
*******************************************************************************/
/* All following variables declared here must be declared 'static' */


/*******************************************************************************
* FUNCTION PROTOTYPES (for local functions):
*******************************************************************************/
/* All following functions declared here must be declared 'static' */
static void Timer1_Init();
static void Timer3_Init();



/*******************************************************************************
*
*                       IMPLEMENTATION: Public functions
*
*******************************************************************************/
void I2C_Init()
{
	Timer1_Init();
	Timer3_Init();
	
	SMB0CF = 0x1D;                     // Use Timer1 overflows as SMBus clock
                                       // source;
                                       // Enable slave mode;
                                       // Enable setup & hold time
                                       // extensions;
                                       // Enable SMBus Free timeout detect;
                                       // Enable SCL low timeout detect;

	SMB0CF |= 0x80;                    // Enable SMBus;
	EIE1 |= 0x01;                      // Enable the SMBus interrupt	
}



/*******************************************************************************
*
*                       IMPLEMENTATION: Private functions
*
*******************************************************************************/
//-----------------------------------------------------------------------------
// Timer1_Init()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Timer1 configured as the SMBus clock source as follows:
// - Timer1 in 8-bit auto-reload mode
// - SYSCLK or SYSCLK / 4 as Timer1 clock source
// - Timer1 overflow rate => 3 * SMB_FREQUENCY
// - The resulting SCL clock rate will be ~1/3 the Timer1 overflow rate
// - Timer1 enabled
//
static void Timer1_Init (void)
{

// Make sure the Timer can produce the appropriate frequency in 8-bit mode
// Supported SMBus Frequencies range from 10kHz to 100kHz.  The CKCON register
// settings may need to change for frequencies outside this range.
#if ((SYSCLK/I2C_FREQUENCY/3) < 255)
   #define SCALE 1
      CKCON |= 0x08;                   // Timer1 clock source = SYSCLK
#elif ((SYSCLK/I2C_FREQUENCY/4/3) < 255)
   #define SCALE 4
      CKCON |= 0x01;
      CKCON &= ~0x0A;                  // Timer1 clock source = SYSCLK / 4
#endif

   TMOD = 0x20;                        // Timer1 in 8-bit auto-reload mode

   // Timer1 configured to overflow at 1/3 the rate defined by SMB_FREQUENCY
   TH1 = -(SYSCLK/I2C_FREQUENCY/SCALE/3);

   TL1 = TH1;                          // Init Timer1

   TR1 = 1;                            // Timer1 enabled
}

//-----------------------------------------------------------------------------
// Timer3_Init()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Timer3 configured for use by the SMBus low timeout detect feature as
// follows:
// - Timer3 in 16-bit auto-reload mode
// - SYSCLK/12 as Timer3 clock source
// - Timer3 reload registers loaded for a 25ms overflow period
// - Timer3 pre-loaded to overflow after 25ms
// - Timer3 enabled
//
static void Timer3_Init (void)
{
   TMR3CN = 0x00;                      // Timer3 configured for 16-bit auto-
                                       // reload, low-byte interrupt disabled

   CKCON &= ~0x40;                     // Timer3 uses SYSCLK/12

   TMR3RL = -(SYSCLK/12/40);           // Timer3 configured to overflow after
   TMR3 = TMR3RL;                      // ~25ms (for SMBus low timeout detect):
                                       // 1/.025 = 40

   EIE1 |= 0x80;                       // Timer3 interrupt enable
   TMR3CN |= 0x04;                     // Start Timer3
}
//-----------------------------------------------------------------------------
// I2C Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// I2C ISR state machine
// - Slave only implementation - no master states defined
// - All incoming data is written to global variable <SMB_data_IN>
// - All outgoing data is read from global variable <SMB_data_OUT>
//
void I2C_ISR (void) interrupt 7
{
	static unsigned char sent_byte_counter;
	static unsigned char rec_byte_counter;

	if (ARBLOST == 0)
	{
		switch (SMB0CN & 0xF0)           // Decode the SMBus status vector
		{
			// Slave Receiver: Start+Address received
			case  I2C_SRADD:
				STA = 0;                   // Clear STA bit
				sent_byte_counter = 1;     // Reinitialize the data counters
				rec_byte_counter = 1;
				if((SMB0DAT&0xFE) == (I2C_SLAVE_ADDR&0xFE)) // Decode address
				{                          // If the received address matches,
					ACK = 1;                // ACK the received slave address
					if((SMB0DAT&0x01) == I2C_READ) // If the transfer is a master READ,
					{
						// Prepare outgoing byte
						SMB0DAT = I2C_OUTPUTBUFFER[sent_byte_counter-1];
						sent_byte_counter++;
					}
				}
				else                       // If received slave address does not
				{  				           // match,
					ACK = 0;                // NACK received address
				}
				break;

			// Slave Receiver: Data received
			case  I2C_SRDB:
				if (rec_byte_counter < I2C_INPUT_BUFFER_SIZE)
				{
					// Store incoming data
					I2C_INPUTBUFFER[rec_byte_counter-1] = SMB0DAT;
					rec_byte_counter++;
					_gI2C_DataReady = kFalse;
					ACK = 1;                // ACK received data
				}
				else
				{
					// Store incoming data
					I2C_INPUTBUFFER[rec_byte_counter-1] = SMB0DAT;
					_gI2C_DataReady = kTrue;         // Indicate new data fully received
					ACK = 1;                // ACK received data
				}
				break;

			// Slave Receiver: Stop received while either a Slave Receiver or
			// Slave Transmitter
			case  I2C_SRSTO:
				STO = 0;                   // STO must be cleared by software when
										   // a STOP is detected as a slave
				break;

			// Slave Transmitter: Data byte transmitted
			case  I2C_STDB:
				if (ACK == 1)              // If Master ACK's, send the next byte
				{
					if (sent_byte_counter <= I2C_OUTPUT_BUFFER_SIZE)
					{
						// Prepare next outgoing byte
						SMB0DAT = I2C_OUTPUTBUFFER[sent_byte_counter-1];
						sent_byte_counter++;
					}
					else
					{
						SMB0DAT = 0;
					}
				}                          // Otherwise, do nothing
				break;
			
			// Slave Transmitter: Arbitration lost, Stop detected
			//
			// This state will only be entered on a bus error condition.
			// In normal operation, the slave is no longer sending data or has
			// data pending when a STOP is received from the master, so the TXMODE
			// bit is cleared and the slave goes to the SRSTO state.
			case  I2C_STSTO:
			
				STO = 0;                   // STO must be cleared by software when
										   // a STOP is detected as a slave
				break;

			// Default: all other cases undefined
			default:
				SMB0CF &= ~0x80;           // Reset communication
				SMB0CF |= 0x80;
				STA = 0;
				STO = 0;
				ACK = 0;
				break;
		}
	}
	// ARBLOST = 1, Abort failed transfer
	else
	{
		STA = 0;
		STO = 0;
		ACK = 0;
	}

	SI = 0;                             // Clear SMBus interrupt flag
}

//-----------------------------------------------------------------------------
// Timer3 Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// A Timer3 interrupt indicates an SMBus SCL low timeout.
// The SMBus is disabled and re-enabled here
//
void Timer3_ISR (void) interrupt 14
{
   SMB0CF &= ~0x80;                    // Disable SMBus
   SMB0CF |= 0x80;                     // Re-enable SMBus
   TMR3CN &= ~0x80;                    // Clear Timer3 interrupt-pending flag
}