/***************************************************************************//**

   @file     adc_drv.c
   @date     8/12/2012

   @brief    Template file for new source modules

*******************************************************************************/


/*******************************************************************************
* INCLUDE FILES:
*******************************************************************************/
#include "platform_cfg.h"
#include "adc_drv.h"
#include "globals.h"

/*******************************************************************************
* CONSTANTS:
*******************************************************************************/
#define MDCLK        2457600              // Modulator Clock (Hz)
#define OWR          1                    // desired Output Word Rate in Hz

/*******************************************************************************
* TYPES:
*******************************************************************************/

/*******************************************************************************
* GLOBAL VARIABLES:
*******************************************************************************/

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


/*******************************************************************************
*
*                       IMPLEMENTATION: Public functions
*
*******************************************************************************/
void ADC_Init()
{
	unsigned ADC0_decimation;

   REF0CN &= ~0x01;                       // disable internal vref
/*
   REF0CN |= 0x01;                        // (enable if using internal vref)
*/

   //ADC0CN = 0x10;                       // Bipolar output codes, GAIN=1
   ADC0CN = 0x00;                         // Unipolar output codes, GAIN=1

/*
   ADC0CF = 0x00;                         // interrupts upon SINC3 filter output
                                          // and uses internal VREF
*/
   ADC0CF = 0x04;                         // interrupts upon SINC3 filter output
                                          // and uses external VREF

   // Generate MDCLK for modulator.
   // Ideally MDCLK = 2.4576
   ADC0CLK = (SYSCLK/MDCLK)-1;
                                          // Ideally, MDCLK = 2.4576 MHz
//   ADC0DEC = 0x7FF;                     // set slowest OWR


   // program decimation rate for desired OWR

   ADC0_decimation = (u32) SYSCLK/ (u32) OWR /
                     (u32) (ADC0CLK+1)/(u32)128;

   ADC0_decimation--;

   ADC0DEC = ADC0_decimation;

   ADC0BUF = 0x00;                        // Turn off Input Buffers

   ADC0MD = 0x80;                         // Enable the ADC0 (IDLE Mode)

   ADC0MD |= 0x01;                        // Init Internal Full cal

   while (!AD0CALC);                      // Wait for calibration complete

   ADC0MD &= ~0x07;                       // clear bits (put ADC0 in IDLE
                                          // mode)

   AD0INT = 0;                            // clear pending sample indication	
}

void ADC_Configure(tADCInputSelection inputSel)
{
	// Set IDLE mode
	ADC0MD = 0x80;                         
	
	// configure input mux
	ADC0MUX = inputSel;
	
	// start single conversion
	ADC0MD = 0x82;	
}

tBoolean ADC_Read(tDataDWordPtr adcDataPtr)
{
	if (AD0INT)
	{
		AD0INT = 0;
		
		*adcDataPtr = ADC0H;
		*adcDataPtr <<= 16;
		*adcDataPtr |= ADC0L;
		*adcDataPtr |= ((u32)ADC0M << 8);
		
		return kTrue;
	}
	else
	{
		return kFalse;
	}	
}


/*******************************************************************************
*
*                       IMPLEMENTATION: Private functions
*
*******************************************************************************/