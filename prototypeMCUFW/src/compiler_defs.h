/***************************************************************************//**

   @file    compiler_defs.h
   @date    4/17/2012

   @brief   Compiler definitions

*******************************************************************************/


//-----------------------------------------------------------------------------
// Header File Preprocessor Directive
//-----------------------------------------------------------------------------

#ifndef __COMPILER_DEFS_H__
#define __COMPILER_DEFS_H__

#ifndef _MSC_VER
#ifndef __PLATFORM_CFG_H__
#error Must include platform_cfg.h instead of directly including compiler_defs.h
#endif
#endif
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

// SDCC - Small Device C Compiler
// http://sdcc.sourceforge.net

#if defined SDCC

# define ZERO(x)

# define SEG_GENERIC
# define SEG_FAR   __xdata
# define SEG_DATA  __data
# define SEG_NEAR  __data
# define SEG_IDATA __idata
# define SEG_XDATA __xdata
# define SEG_PDATA __pdata
# define SEG_CODE  __code
# define SEG_BDATA __bdata

# define SBIT(name, addr, bit)  __sbit  __at(addr+bit)                  name
# define SFR(name, addr)        __sfr   __at(addr)                      name
# define SFRX(name, addr)       xdata volatile unsigned char __at(addr) name
# define SFR16(name, addr)      __sfr16 __at(((addr+1U)<<8) | addr)     name
# define SFR16E(name, fulladdr) __sfr16 __at(fulladdr)                  name
# define SFR32(name, addr)      __sfr32 __at(((addr+3UL)<<24) | ((addr+2UL)<<16) | ((addr+1UL)<<8) | addr) name
# define SFR32E(name, fulladdr) __sfr32 __at(fulladdr)                  name

# define INTERRUPT(name, vector) void name (void) __interrupt (vector)
# define INTERRUPT_USING(name, vector, regnum) void name (void) __interrupt (vector) using (regnum)
# define INTERRUPT_PROTO(name, vector) void name (void) __interrupt (vector)
# define INTERRUPT_PROTO_USING(name, vector, regnum) void name (void) __interrupt (vector) using (regnum)

# define FUNCTION_USING(name, return_value, parameter, regnum) return_value name (parameter) using (regnum)
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) return_value name (parameter) using (regnum)
// Note: Parameter must be either 'void' or include a variable type and name. (Ex: char temp_variable)

// variable located in 'locsegment'
# define SEGMENT_VARIABLE(name, vartype, locsegment) locsegment vartype name
// pointer to data in 'targsegment'   (pointer var is located in default segment)
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) targsegment vartype * name
// pointer to data in 'targsegment'  where pointer var is located in locsegment
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) targsegment vartype * locsegment name
// pointer to data in default segment
# define SEGMENT_POINTER(name, vartype, locsegment) vartype * locsegment name

# define LOCATED_VARIABLE(name, vartype, locsegment, addr, init) locsegment __at (addr) vartype name = init
# define LOCATED_VARIABLE_NO_INIT(name, vartype, locsegment, addr) locsegment __at (addr) vartype name

// used with UU16
# define LSB 0
# define MSB 1

// used with UU32 (b0 is least-significant byte)
# define b0 0
# define b1 1
# define b2 2
# define b3 3

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;
typedef signed char S8;
typedef signed int S16;
typedef signed long S32;
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
typedef signed char i8;
typedef signed int i16;
typedef signed long i32;

typedef union UU16
{
   U16 U16;
   S16 S16;
   U8 U8[2];
   S8 S8[2];
} UU16;

typedef union UU32
{
   U32 U32;
   S32 S32;
   UU16 UU16[2];
   U16 U16[2];
   S16 S16[2];
   U8 U8[4];
   S8 S8[4];
} UU32;

// NOP () macro support
#define NOP() _asm NOP _endasm


//-----------------------------------------------------------------------------

// Raisonance (must be placed before Keil C51)
// http://www.raisonance.com

#elif defined __RC51__

//#error Raisonance C51 detected.

# define SEG_GENERIC generic     //SEG_GENERIC only applies to pointers in Raisonance, not variables.
# define SEG_FAR   xdata
# define SEG_DATA  data
# define SEG_NEAR  data
# define SEG_IDATA idata
# define SEG_XDATA xdata
# define SEG_PDATA pdata
# define SEG_CODE  code
# define SEG_BDATA bdata

# define SBIT(name, addr, bit)  at (addr+bit) sbit         name
# define SFR(name, addr)        sfr at addr                name
# define SFR16(name, addr)      sfr16 at addr              name
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

# define INTERRUPT(name, vector) void name (void) interrupt vector
# define INTERRUPT_USING(name, vector, regnum) void name (void) interrupt vector using regnum
# define INTERRUPT_PROTO(name, vector) void name (void)
# define INTERRUPT_PROTO_USING(name, vector, regnum) void name (void)

# define FUNCTION_USING(name, return_value, parameter, regnum) return_value name (parameter) using regnum
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) return_value name (parameter)
// Note: Parameter must be either 'void' or include a variable type and name. (Ex: char temp_variable)

# define SEGMENT_VARIABLE(name, vartype, locsegment) vartype locsegment name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) vartype targsegment * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) vartype targsegment * locsegment name
# define SEGMENT_POINTER(name, vartype, locsegment) vartype * locsegment name
# define LOCATED_VARIABLE(name, vartype, locsegment, addr, init) at addr locsegment vartype name
# define LOCATED_VARIABLE_NO_INIT(name, vartype, locsegment, addr) at addr locsegment vartype name


// used with UU16
# define LSB 1
# define MSB 0

// used with UU32 (b0 is least-significant byte)
# define b0 3
# define b1 2
# define b2 1
# define b3 0

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;
typedef signed char S8;
typedef signed int S16;
typedef signed long S32;
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
typedef signed char i8;
typedef signed int i16;
typedef signed long i32;

typedef union UU16
{
   U16 U16;
   S16 S16;
   U8 U8[2];
   S8 S8[2];
} UU16;

typedef union UU32
{
   U32 U32;
   S32 S32;
   UU16 UU16[2];
   U16 U16[2];
   S16 S16[2];
   U8 U8[4];
   S8 S8[4];
} UU32;

// NOP () macro support -- NOP is opcode 0x00
#define NOP() asm { 0x00 }


//-----------------------------------------------------------------------------


// Keil C51
// http://www.keil.com

#elif defined __C51__

//#error Keil C51 detected.
# define ZERO(x)

# define SEG_GENERIC
# define SEG_FAR   xdata
# define SEG_DATA  data
# define SEG_NEAR  data
# define SEG_IDATA idata
# define SEG_XDATA xdata
# define SEG_PDATA pdata
# define SEG_CODE  code
# define SEG_BDATA bdata

# define SBIT(name, addr, bit)  sbit  name = addr^bit
# define SFR(name, addr)        sfr   name = addr
# define SFR16(name, addr)      sfr16 name = addr
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

# define INTERRUPT(name, vector) void name (void) interrupt vector
# define INTERRUPT_USING(name, vector, regnum) void name (void) interrupt vector using regnum
# define INTERRUPT_PROTO(name, vector) void name (void)
# define INTERRUPT_PROTO_USING(name, vector, regnum) void name (void)

# define FUNCTION_USING(name, return_value, parameter, regnum) return_value name (parameter) using regnum
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) return_value name (parameter)
// Note: Parameter must be either 'void' or include a variable type and name. (Ex: char temp_variable)

# define SEGMENT_VARIABLE(name, vartype, locsegment) vartype locsegment name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) vartype targsegment * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) vartype targsegment * locsegment name
# define SEGMENT_POINTER(name, vartype, locsegment) vartype * locsegment name
# define LOCATED_VARIABLE_NO_INIT(name, vartype, locsegment, addr) vartype locsegment name _at_ addr

// used with UU16
# define LSB 1
# define MSB 0

// used with UU32 (b0 is least-significant byte)
# define b0 3
# define b1 2
# define b2 1
# define b3 0

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;
typedef signed char S8;
typedef signed int S16;
typedef signed long S32;
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
typedef signed char i8;
typedef signed int i16;
typedef signed long i32;
typedef bit tBoolean;


typedef union UU16
{
   U16 U16;
   S16 S16;
   U8 U8[2];
   S8 S8[2];
} UU16;

typedef union UU32
{
   U32 U32;
   S32 S32;
   UU16 UU16[2];
   U16 U16[2];
   S16 S16[2];
   U8 U8[4];
   S8 S8[4];
} UU32;

// NOP () macro support
extern void _nop_ (void);
#define NOP() _nop_()

//-----------------------------------------------------------------------------

// Hi-Tech 8051
// http://www.htsoft.com

#elif defined HI_TECH_C

# define SEG_GENERIC
# define SEG_FAR   far
# define SEG_DATA  data
# define SEG_NEAR  near
# define SEG_IDATA idata
# define SEG_XDATA xdata
# define SEG_PDATA pdata
# define SEG_CODE  code
# define SEG_BDATA bdata


# define SBIT(name, addr, thebit) static volatile bit name @ (addr + thebit)
# define SFR(name, addr)          static volatile unsigned char name @ addr
# define SFR16(name, addr)        static volatile unsigned int name @ addr
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

# define INTERRUPT(name, vector)       void name (void) interrupt vector
# define INTERRUPT_PROTO(name, vector)
# define INTERRUPT_USING(name, vector, regnum) void name (void) interrupt vector using regnum
# define INTERRUPT_PROTO_USING(name, vector, regnum)

# define FUNCTION_USING(name, return_value, parameter, regnum) /* not supported */
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) /* not supported */
// Note: Hi-Tech does not support functions using different register banks. Register
//       banks can only be specified in interrupts. If a function is called from
//       inside an interrupt, it will use the same register bank as the interrupt.

# define SEGMENT_VARIABLE(name, vartype, locsegment) locsegment vartype name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) targsegment vartype * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) targsegment vartype * locsegment name
# define LOCATED_VARIABLE(name, vartype, locsegment, addr, init) locsegment vartype name @ addr

// used with UU16
# define LSB 0
# define MSB 1

// used with UU32 (b0 is least-significant byte)
# define b0 0
# define b1 1
# define b2 2
# define b3 3

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;
typedef signed char S8;
typedef signed int S16;
typedef signed long S32;
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
typedef signed char i8;
typedef signed int i16;
typedef signed long i32;

typedef union UU16
{
   U16 U16;
   S16 S16;
   U8 U8[2];
   S8 S8[2];
} UU16;

typedef union UU32
{
   U32 U32;
   S32 S32;
   UU16 UU16[2];
   U16 U16[2];
   S16 S16[2];
   U8 U8[4];
   S8 S8[4];
} UU32;

// NOP () macro support
#define NOP() asm(" nop ")

//-----------------------------------------------------------------------------

// Tasking / Altium
// http://www.altium.com/tasking


#elif defined _CC51

# define SEG_GENERIC
# define SEG_FAR   _xdat
# define SEG_DATA  _data
# define SEG_NEAR  _data
# define SEG_IDATA _idat
# define SEG_XDATA _xdat
# define SEG_PDATA _pdat
# define SEG_CODE  _rom
# define SEG_BDATA _bdat

# define SBIT(name, addr, bit)  _sfrbit  name _at(addr+bit)
# define SFR(name, addr)        _sfrbyte name _at(addr)
# define SFRX(name, addr)       _xdat volatile unsigned char name _at(addr)
#if _CC51 > 71
# define SFR16(name, addr)      _sfrword _little name _at(addr)
#else
# define SFR16(name, addr)      /* not supported */
#endif
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

# define INTERRUPT(name, vector) _interrupt (vector) void name (void)
# define INTERRUPT_USING(name, vector, regnum) _interrupt (vector) _using(regnum) void name (void)
# define INTERRUPT_PROTO(name, vector) _interrupt (vector) void name (void)
# define INTERRUPT_PROTO_USING(name, vector, regnum) _interrupt (vector) _using(regnum) void name (void)

// When calling FUNCTION_USING in Tasking, the function must be called from an interrupt or Main which
// is also using the same register bank. If not, the compiler will generate an error.
# define FUNCTION_USING(name, return_value, parameter, regnum) _using(regnum) return_value name (parameter)
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) _using(regnum) return_value name (parameter)
// Note: Parameter must be either 'void' or include a variable type and name. (Ex: char temp_variable)

# define SEGMENT_VARIABLE(name, vartype, locsegment) vartype locsegment name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) vartype targsegment * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) vartype targsegment * locsegment name
# define LOCATED_VARIABLE(name, vartype, locsegment, addr, init) vartype locsegment name _at( addr )

// used with UU16
# define LSB 1
# define MSB 0

// used with UU32 (b0 is least-significant byte)
# define b0 3
# define b1 2
# define b2 1
# define b3 0

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;
typedef signed char S8;
typedef signed int S16;
typedef signed long S32;
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
typedef signed char i8;
typedef signed int i16;
typedef signed long i32;

typedef union UU16
{
   U16 U16;
   S16 S16;
   U8 U8[2];
   S8 S8[2];
} UU16;

typedef union UU32
{
   U32 U32;
   S32 S32;
   UU16 UU16[2];
   U16 U16[2];
   S16 S16[2];
   U8 U8[4];
   S8 S8[4];
} UU32;

// NOP () macro support
extern void _nop (void);
#define NOP() _nop()

//-----------------------------------------------------------------------------


// IAR 8051
// http://www.iar.com

#elif defined __ICC8051__

#include <stdbool.h>
#include <intrinsics.h>

# define SBIT(name, addr, bit)  __bit __no_init volatile bool name @ (addr+bit)
# define SFR(name, addr)        __sfr __no_init volatile unsigned char name @ addr
# define SFRX(name, addr)       __xdata __no_init volatile unsigned char name @ addr
# define SFR16(name, addr)      __sfr __no_init volatile unsigned int  name @ addr
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr) /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

# define SEG_GENERIC __generic
# define SEG_FAR  __xdata
# define SEG_DATA __data
# define SEG_NEAR __data
# define SEG_IDATA __idata
# define SEG_XDATA __xdata
# define SEG_PDATA __pdata
# define SEG_CODE  __code
# define SEG_BDATA __bdata

#define bit bool

# define _PPTOSTR_(x) #x
# define _PPARAM_(address) _PPTOSTR_(vector=address * 8 + 3)
# define _PPARAM2_(regbank) _PPTOSTR_(register_bank=regbank)
# define INTERRUPT(name, vector) _Pragma(_PPARAM_(vector)) __interrupt void name(void)
# define INTERRUPT_PROTO(name, vector)  __interrupt void name(void)
# define INTERRUPT_USING(name, vector, regnum) _Pragma(_PPARAM2_(regnum)) _Pragma(_PPARAM_(vector)) __interrupt void name(void)
# define INTERRUPT_PROTO_USING(name, vector, regnum) __interrupt void name(void)

# define FUNCTION_USING(name, return_value, parameter, regnum) /* not supported */
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) /* not supported */
// Note: IAR does not support functions using different register banks. Register
//       banks can only be specified in interrupts. If a function is called from
//       inside an interrupt, it will use the same register bank as the interrupt.

# define SEGMENT_VARIABLE(name, vartype, locsegment)  locsegment vartype name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) vartype targsegment  * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) vartype targsegment * locsegment name

# define LOCATED_VARIABLE(name, vartype, locsegment, addr, init) locsegment __no_init vartype name @ addr

// used with UU16
# define LSB 0
# define MSB 1

// used with UU32 (b0 is least-significant byte)
# define b0 0
# define b1 1
# define b2 2
# define b3 3

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;
typedef signed char S8;
typedef signed int S16;
typedef signed long S32;
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
typedef signed char i8;
typedef signed int i16;
typedef signed long i32;;

typedef union UU16
{
   U16 U16;
   S16 S16;
   U8 U8[2];
   S8 S8[2];
} UU16;

typedef union UU32
{
   U32 U32;
   S32 S32;
   UU16 UU16[2];
   U16 U16[2];
   S16 S16[2];
   U8 U8[4];
   S8 S8[4];
} UU32;


#define NOP() __no_operation();
//-----------------------------------------------------------------------------

// Crossware
// http://www.crossware.com

#elif defined _XC51_VER
# define SBIT(name, addr, bit)  _sfrbit  name = (addr+bit)
# define SFR(name, addr)        _sfr     name = addr
# define SFR16(name, addr)      _sfrword name = addr
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

//-----------------------------------------------------------------------------

// Wickenhäuser
// http://www.wickenhaeuser.de

#elif defined __UC__
# define SBIT(name, addr, bit)  unsigned char bit  name @ (addr+bit)
# define SFR(name, addr)        near unsigned char name @ addr
# define SFR16(name, addr)      /* not supported */
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

//-----------------------------------------------------------------------------

// Microsoft C
#elif defined _MSC_VER

# define ZERO(x) x = 0

#pragma warning(disable: 4214)
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef signed char S8;
typedef signed short S16;
typedef signed int S32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef u8 tBoolean;

# define SBIT(name, addr, bit)  volatile u8 name
# define SFR(name, addr)        volatile u8  name
# define SFRX(name, addr)       volatile u8  name
# define SFR16(name, addr)      volatile u16 name
# define SFR16E(name, fulladdr) volatile u16 name
# define SFR32(name, fulladdr)  volatile u32  name
# define SFR32E(name, fulladdr) volatile u32  name

# define SEG_GENERIC
# define SEG_FAR   
# define SEG_DATA  
# define SEG_NEAR  
# define SEG_IDATA 
# define SEG_XDATA 
# define SEG_PDATA 
# define SEG_CODE  
# define SEG_BDATA 

# define INTERRUPT(name, vector) void name (void)
# define INTERRUPT_USING(name, vector, regnum) void name (void)
# define INTERRUPT_PROTO(name, vector) void name (void)
# define INTERRUPT_PROTO_USING(name, vector, regnum) void name (void)

# define FUNCTION_USING(name, return_value, parameter, regnum) return_value name (parameter)
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) return_value name (parameter)
// Note: Parameter must be either 'void' or include a variable type and name. (Ex: char temp_variable)
# define SEGMENT_VARIABLE(name, vartype, locsegment) vartype locsegment name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) vartype targsegment * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) vartype targsegment * locsegment name
# define SEGMENT_POINTER(name, vartype, locsegment) vartype * locsegment name
# define LOCATED_VARIABLE_NO_INIT(name, vartype, locsegment, addr) vartype locsegment name

// used with UU16
# define LSB 1
# define MSB 0

// used with UU32 (b0 is least-significant byte)
# define b0 3
# define b1 2
# define b2 1
# define b3 0



typedef union UU16
{
   U16 U16;
   S16 S16;
   U8 U8[2];
   S8 S8[2];
} UU16;

typedef union UU32
{
   U32 U32;
   S32 S32;
   UU16 UU16[2];
   U16 U16[2];
   S16 S16[2];
   U8 U8[4];
   S8 S8[4];
} UU32;

// NOP () macro support
extern void _nop_ (void);
#define NOP() _nop_()

// Default
// Unknown compiler

#else
# warning unrecognized compiler
# define SBIT(name, addr, bit)  volatile bool           name
# define SFR(name, addr)        volatile unsigned char  name
# define SFRX(name, addr)       volatile unsigned char  name
# define SFR16(name, addr)      volatile unsigned short name
# define SFR16E(name, fulladdr) volatile unsigned short name
# define SFR32(name, fulladdr)  volatile unsigned long  name
# define SFR32E(name, fulladdr) volatile unsigned long  name

#endif


// Common definitions
# define kTrue  1
# define kFalse 0

//-----------------------------------------------------------------------------
// Header File PreProcessor Directive
//-----------------------------------------------------------------------------

#endif                                 // #define __COMPILER_DEFS_H__

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
