/***************************************************************************//**

   @file     types.h
   @date     8/12/2012

   @brief    Definition of common types

*******************************************************************************/
#ifndef __TYPES_H__
#define __TYPES_H__

/*******************************************************************************
* INCLUDE FILES:
*******************************************************************************/
#include "platform_cfg.h"

typedef VARIABLE_SEGMENT_POINTER(tCodeBytePtr, u8, SEG_CODE);
typedef VARIABLE_SEGMENT_POINTER(tXDataBytePtr, u8, SEG_XDATA);
typedef VARIABLE_SEGMENT_POINTER(tDataBytePtr, u8, SEG_DATA);
typedef VARIABLE_SEGMENT_POINTER(tIDataBytePtr, u8, SEG_IDATA);
typedef VARIABLE_SEGMENT_POINTER(tCodeWordPtr, u16, SEG_CODE);
typedef VARIABLE_SEGMENT_POINTER(tXDataWordPtr, u16, SEG_XDATA);
typedef VARIABLE_SEGMENT_POINTER(tDataWordPtr, u16, SEG_DATA);
typedef VARIABLE_SEGMENT_POINTER(tIDataWordPtr, u16, SEG_IDATA);
typedef VARIABLE_SEGMENT_POINTER(tCodeDWordPtr, u32, SEG_CODE);
typedef VARIABLE_SEGMENT_POINTER(tXDataDWordPtr, u32, SEG_XDATA);
typedef VARIABLE_SEGMENT_POINTER(tDataDWordPtr, u32, SEG_DATA);
typedef VARIABLE_SEGMENT_POINTER(tIDataDWordPtr, u32, SEG_IDATA);
typedef VARIABLE_SEGMENT_POINTER(tCodeVoidPtr, void, SEG_CODE);
typedef VARIABLE_SEGMENT_POINTER(tXDataVoidPtr, void, SEG_XDATA);
typedef VARIABLE_SEGMENT_POINTER(tDataVoidPtr, void, SEG_DATA);
typedef VARIABLE_SEGMENT_POINTER(tIDataVoidPtr, void, SEG_IDATA);
															  

#endif