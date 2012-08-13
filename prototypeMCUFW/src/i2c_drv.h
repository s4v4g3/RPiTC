/***************************************************************************//**

   @file     i2c_drv.h
   @date     8/12/2012

   @brief    Declaration of I2C (SMBus) driver

*******************************************************************************/
#ifndef __I2C_DRV_H__
#define __I2C_DRV_H__

/*******************************************************************************
* INCLUDE FILES:
*******************************************************************************/
#include "platform_cfg.h"
#include "types.h"


/*******************************************************************************
* CONSTANTS:
*******************************************************************************/
#define I2C_INPUT_BUFFER_SIZE   8
#define I2C_OUTPUT_BUFFER_SIZE  8

/*******************************************************************************
* TYPES:
*******************************************************************************/

/*******************************************************************************
* GLOBAL VARIABLES:
*******************************************************************************/
extern SEGMENT_VARIABLE(_gI2C_InputBuffer[I2C_INPUT_BUFFER_SIZE], u8, SEG_IDATA);
extern SEGMENT_VARIABLE(_gI2C_OutputBuffer[I2C_OUTPUT_BUFFER_SIZE], u8, SEG_IDATA);
extern tBoolean _gI2C_DataReady;

/*******************************************************************************
* MACROS:
*******************************************************************************/
#define I2C_INPUTBUFFER  (_gI2C_InputBuffer)
#define I2C_OUTPUTBUFFER (_gI2C_OutputBuffer)
#define I2C_GETDATAREADY()  (_gI2C_DataReady)
#define I2C_CLEARDATAREADY() {_gI2C_DataReady = kFalse;}
/*******************************************************************************
* FUNCTIONS:
*******************************************************************************/
void I2C_Init();




#endif // __I2C_DRV_H__