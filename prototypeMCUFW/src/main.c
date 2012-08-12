/***************************************************************************//**

   @file     Template.c
   @date     mo/day/year

   @brief    Template file for new source modules

*******************************************************************************/


/*******************************************************************************
* INCLUDE FILES:
*******************************************************************************/
#include "platform_cfg.h"
#include "globals.h"
#include "math.h"
#include "i2c_drv.h"
#include "adc_drv.h"

/*******************************************************************************
* CONSTANTS:
*******************************************************************************/
#define NUM_TEMPERATURES  5

SEGMENT_VARIABLE(_sSteinhart[3], float, SEG_CODE) = {2.3067434e-4,2.3696596e-4,1.2636414e-7};
SEGMENT_VARIABLE(_sResistorVal, float, SEG_CODE) = 9.71e3;
SEGMENT_VARIABLE(_sADCMax, u32, SEG_CODE) = 0x00FFFFFF;
SEGMENT_VARIABLE(_sADCInputs[NUM_TEMPERATURES], u8, SEG_CODE) = {kAIn0VsGnd, kAIn1VsGnd, kAIn0VsGnd, kAIn1VsGnd, kTempSensor};

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
static SEGMENT_VARIABLE(_sTemperatures[NUM_TEMPERATURES], float, SEG_DATA);
static SEGMENT_VARIABLE(_sTemperatureIndex, u8, SEG_DATA);

/*******************************************************************************
* FUNCTION PROTOTYPES (for local functions):
*******************************************************************************/
/* All following functions declared here must be declared 'static' */
static void SYSCLK_Init();
static void PORT_Init();

// move these elsewhere ???
static void UpdateTemperatures();
static float ConvertADCValueToTemperature(u32 adcVal);

static void I2C_Process();


/*******************************************************************************
*
*                       IMPLEMENTATION: Public functions
*
*******************************************************************************/
void main()
{
	// disable watchdog timer
	// WDTE = 0 (clear watchdog timer enable)
	// This is done in startup code
	PCA0MD &= ~0x40; 
	
	SYSCLK_Init();
	PORT_Init();
	
	// ADC driver init
	ADC_Init();                      

	// initialize I2C
	I2C_Init();

	EA = 1;
	
	_sTemperatureIndex = 0;
	ADC_Configure(_sADCInputs[_sTemperatureIndex]);

	while(1)
	{
		UpdateTemperatures();

		I2C_Process();
	}
}


/*******************************************************************************
*
*                       IMPLEMENTATION: Private functions
*
*******************************************************************************/
static void SYSCLK_Init()
{
	// set internal oscillator for highest freq (2450000)
	// Note that the global constant SYSCLK must reflect
	// this setting
 	OSCICN = 0x83;

	// enable missing clock detector                         
	RSTSRC = 0x04;                         
}

static void PORT_Init()
{
	P0MDOUT = 0x00;                     // All P0 pins open-drain output
   
	XBR0 = 0x04;                        // Enable SMBus pins
	XBR1 = 0x40;                        // Enable crossbar and weak pull-ups

   	P0 = 0xFF;
}

static void UpdateTemperatures()
{
	// check if the ADC reading is available
	u32 adcVal;
	if (ADC_Read(&adcVal))
	{
		if (_sTemperatureIndex != 4)
		{
			_sTemperatures[_sTemperatureIndex] = ConvertADCValueToTemperature(adcVal);
		}
		else
		{
			// temp sensor reading
			_sTemperatures[_sTemperatureIndex] = adcVal;
		}
		
		if (++_sTemperatureIndex == NUM_TEMPERATURES)
		{
			_sTemperatureIndex = 0;
		}

		ADC_Configure(_sADCInputs[_sTemperatureIndex]);
	}	
}

static float ConvertADCValueToTemperature(u32 adcVal)
{
	float R, T, Temperature;
	R = _sResistorVal / ((_sADCMax / (float)adcVal) - 1.0f);

	R = log(R);
	T = 1.0f / ((_sSteinhart[2] * R * R + _sSteinhart[1]) * R + _sSteinhart[0]);
	Temperature = T - 273.15;
	Temperature = (Temperature * 9.0 / 5.0) + 32.0;

	return Temperature;
}

#define  CMD_READTEMPERATURE   0x55

static void I2C_Process()
{
	u8 command;
	// Check for i2c data and copy		
	if (I2C_GETDATAREADY())
	{
		I2C_CLEARDATAREADY();
		command = I2C_INPUTBUFFER[0];
		switch(command)
		{
			case CMD_READTEMPERATURE:
				{
					tDataDWordPtr dataPtr;
					u8 index = I2C_INPUTBUFFER[1];
					if (index >= NUM_TEMPERATURES)
					{
						index = NUM_TEMPERATURES-1;
					}

					dataPtr = (tDataDWordPtr)&_sTemperatures[index];

					I2C_OUTPUTBUFFER[0] = ((*dataPtr) & 0xFF000000) >> 24;
					I2C_OUTPUTBUFFER[1] = ((*dataPtr) & 0xFF0000) >> 16;
					I2C_OUTPUTBUFFER[2] = ((*dataPtr) & 0xFF00) >> 8;
					I2C_OUTPUTBUFFER[3] = ((*dataPtr) & 0xFF) >> 0;
				}	
				break;
		}

	}
}