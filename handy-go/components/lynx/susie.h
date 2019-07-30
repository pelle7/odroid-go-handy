//
// Copyright (c) 2004 K. Wilkins
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//

//////////////////////////////////////////////////////////////////////////////
//	                  Handy - An Atari Lynx Emulator                     //
//                          Copyright (c) 1996,1997                         //
//                                 K. Wilkins                               //
//////////////////////////////////////////////////////////////////////////////
// Susie object header file                                                 //
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// This header file provides the interface definition for the Suzy class    //
// which provides math and sprite support to the emulator                   //
//                                                                          //
//    K. Wilkins                                                            //
// August 1997                                                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
// Revision History:                                                        //
// -----------------                                                        //
//                                                                          //
// 01Aug1997 KW Document header added & class documented.                   //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#ifndef SUSIE_H
#define SUSIE_H

#ifdef TRACE_SUSIE

#define TRACE_SUSIE0(msg)					_RPT1(_CRT_WARN,"CSusie::"msg" (Time=%012d)\n",gSystemCycleCount)
#define TRACE_SUSIE1(msg,arg1)				_RPT2(_CRT_WARN,"CSusie::"msg" (Time=%012d)\n",arg1,gSystemCycleCount)
#define TRACE_SUSIE2(msg,arg1,arg2)			_RPT3(_CRT_WARN,"CSusie::"msg" (Time=%012d)\n",arg1,arg2,gSystemCycleCount)
#define TRACE_SUSIE3(msg,arg1,arg2,arg3)	_RPT4(_CRT_WARN,"CSusie::"msg" (Time=%012d)\n",arg1,arg2,arg3,gSystemCycleCount)

#else

#define TRACE_SUSIE0(msg)
#define TRACE_SUSIE1(msg,arg1)
#define TRACE_SUSIE2(msg,arg1,arg2)
#define TRACE_SUSIE3(msg,arg1,arg2,arg3)

#endif

class CSystem;

#define SUSIE_START		0xfc00
#define SUSIE_SIZE		0x100

#define SCREEN_WIDTH	160
#define SCREEN_HEIGHT	102

#define LINE_END		0x80

//
// Define button values
//

#define BUTTON_A		0x0001
#define BUTTON_B		0x0002
#define BUTTON_OPT2		0x0004
#define BUTTON_OPT1		0x0008
#define BUTTON_LEFT		0x0010
#define BUTTON_RIGHT	0x0020
#define BUTTON_UP		0x0040
#define BUTTON_DOWN		0x0080
#define BUTTON_PAUSE	0x0100

#define line_error 0
#define line_abs_literal 1
#define line_literal 2
#define line_packed 3
//enum {line_error=0,line_abs_literal,line_literal,line_packed};

#define math_finished 0
#define math_divide 1
#define math_multiply 2
#define math_init_divide 3
#define math_init_multiply 4
//enum {math_finished=0,math_divide,math_multiply,math_init_divide,math_init_multiply};

#define sprite_background_shadow 0
#define sprite_background_noncollide 1
#define sprite_boundary_shadow 2
#define sprite_boundary 3
#define sprite_normal 4
#define sprite_noncollide 5
#define sprite_xor_shadow 6
#define sprite_shadow 7
/*
enum {sprite_background_shadow=0,
   sprite_background_noncollide,
   sprite_boundary_shadow,
   sprite_boundary,
   sprite_normal,
   sprite_noncollide,
   sprite_xor_shadow,
   sprite_shadow};
*/
   
#ifndef MY_SUSIE_VARS
      #include "susie_defs.h"
#endif

#define MY_SUSIE_PIXEL_HLOOP
//#define SUSIE_SIGN_MODE // buggy
#define SUSIE_INLINE_PaintSprites
//#define SUSIE_INLINE_PROCESSPIXEL // negativ performance

#define SUSIE_INLINE_WRITEPIXEL
#define SUSIE_INLINE_READPIXEL
#define SUSIE_INLINE_READCOLLISION
#define SUSIE_INLINE_WRITECOLLISION

#define SUSIE_INLINE_LineInit
#define SUSIE_INLINE_LineGetPixel
#define SUSIE_INLINE_LineGetBits
#define SUSIE_INLINE_LineGetBits_V2

#define SUSIE_INLINE_DoMathDivide
#define SUSIE_INLINE_DoMathMultiply

#define RAM_PEEK(m)             (mRamPointer[(m)])
#define RAM_PEEKW(m)            (mRamPointer[(m)]+(mRamPointer[(m)+1]<<8))
#define RAM_POKE(m1,m2)         {mRamPointer[(m1)]=(m2);}

#define MY_GET_BITS(retval_bits, bits) \
     /* ULONG retval_bits; */ \
   if(mLinePacketBitsLeft<=bits) retval_bits = 0; \
   else \
   { \
   if(mLineShiftRegCount<bits) \
   { \
      mLineShiftReg<<=24; \
      mLineShiftReg|=RAM_PEEK(mTMPADR.Word++)<<16; \
      mLineShiftReg|=RAM_PEEK(mTMPADR.Word++)<<8; \
      mLineShiftReg|=RAM_PEEK(mTMPADR.Word++); \
      mLineShiftRegCount+=24; \
      cycles_used+=3*SPR_RDWR_CYC; \
   } \
   retval_bits=mLineShiftReg>>(mLineShiftRegCount-bits); \
   retval_bits&=(1<<bits)-1; \
   mLineShiftRegCount-=bits; \
   mLinePacketBitsLeft-=bits; \
   }

#include "system_vars.h"

class CSusie : public CLynxBase
{
   public:
      CSusie(CSystem& parent);
      ~CSusie();

      void	Reset(void);
      bool	ContextSave(FILE *fp);
      bool	ContextLoad(LSS_FILE *fp);

      UBYTE	Peek(ULONG addr);
      void	Poke(ULONG addr,UBYTE data);
      //ULONG	ReadCycle(void) {return 9;};
      //WriteCycleULONG	WriteCycle(void) {return 5;};
      ULONG	ObjectSize(void) {return SUSIE_SIZE;};

      inline void	SetButtonData(ULONG data) {mJOYSTICK.Byte=(UBYTE)data;mSWITCHES.Byte=(UBYTE)(data>>8);};
      inline ULONG	GetButtonData(void) {return mJOYSTICK.Byte+(mSWITCHES.Byte<<8);};

#ifdef SUSIE_INLINE_PaintSprites
    inline ULONG    PaintSprites(void) {
      #include "susie_paintsprites.h"
      }
#else
    ULONG    PaintSprites(void);
#endif

   public:
#ifdef SUSIE_INLINE_DoMathDivide
      inline void    DoMathDivide(void) {
      #include "susie_DoMathDivide.h"
      }
#else
      void	DoMathDivide(void);
#endif
#ifdef SUSIE_INLINE_DoMathMultiply
      inline void  DoMathMultiply(void) {
      #include "susie_DoMathMultiply.h"
      }
#else
      void	DoMathMultiply(void);
#endif

#ifdef SUSIE_INLINE_LineInit
#else
      ULONG	LineInit(ULONG voff);
#endif

#ifndef MY_SUSIE_PIXEL_HLOOP
#ifdef SUSIE_INLINE_LineGetPixel
      inline ULONG LineGetPixel() {
      #include "susie_LineGetPixel.h"
      }
#else
      ULONG	LineGetPixel(void);
#endif
#endif

#ifndef SUSIE_INLINE_LineGetBits_V2
#ifdef SUSIE_INLINE_LineGetBits
      inline ULONG LineGetBits(ULONG bits) {
      #include "susie_LineGetBits.h"
      }
#else
      ULONG	LineGetBits(ULONG bits);
#endif
#endif

#ifdef SUSIE_INLINE_PROCESSPIXEL
      inline void	ProcessPixel(ULONG hoff,ULONG pixel) {
      #include "susie_processpixel.h"
      }
#else
#ifndef MY_SUSIE_PIXEL_HLOOP
      void    ProcessPixel(ULONG hoff,ULONG pixel);
#endif
#endif
#ifdef SUSIE_INLINE_WRITEPIXEL
    inline void  WritePixel(ULONG hoff,ULONG pixel) {
    #include "susie_writepixel.h"
    }
#else
    void    WritePixel(ULONG hoff,ULONG pixel);
#endif

#ifdef SUSIE_INLINE_READPIXEL
    inline ULONG   ReadPixel(ULONG hoff) {
    #include "susie_readpixel.h"
    }
#else
    ULONG   ReadPixel(ULONG hoff);
#endif

#ifdef SUSIE_INLINE_WRITECOLLISION
    inline void    WriteCollision(ULONG hoff,ULONG pixel) {
    #include "susie_writecollision.h"
    }
#else
    void    WriteCollision(ULONG hoff,ULONG pixel);
#endif  

#ifdef SUSIE_INLINE_READCOLLISION
     inline ULONG ReadCollision(ULONG hoff) {
     #include "susie_readcollision.h"
     }
#else
      ULONG	ReadCollision(ULONG hoff);
#endif
   public:
#ifdef MY_GLOBAL_SYSTEM_VARS_CPU_MEMBER
      systemvars    *gSystemVars;
#endif
   private:
      CSystem&	mSystem;
#ifndef MY_SUSIE_VARS
      #define VAR_PREFIX
      #include "susie_vars.h"
      #undef VAR_PREFIX
#endif
};

#endif
