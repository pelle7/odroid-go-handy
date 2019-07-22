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


enum {line_error=0,line_abs_literal,line_literal,line_packed};
enum {math_finished=0,math_divide,math_multiply,math_init_divide,math_init_multiply};

enum {sprite_background_shadow=0,
   sprite_background_noncollide,
   sprite_boundary_shadow,
   sprite_boundary,
   sprite_normal,
   sprite_noncollide,
   sprite_xor_shadow,
   sprite_shadow};

// Define register typdefs

typedef struct 
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	High;
         UBYTE	Low;
#else
         UBYTE	Low;
         UBYTE	High;
#endif
      }Byte;
      UWORD	Word;
   };
}UUWORD;


typedef struct
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	Fc1:1;
         UBYTE	Fc2:1;
         UBYTE	Fc3:1;
         UBYTE	reserved:1;
         UBYTE	Ac1:1;
         UBYTE	Ac2:1;
         UBYTE	Ac3:1;
         UBYTE	Ac4:1;
#else
         UBYTE	Ac4:1;
         UBYTE	Ac3:1;
         UBYTE	Ac2:1;
         UBYTE	Ac1:1;
         UBYTE	reserved:1;
         UBYTE	Fc3:1;
         UBYTE	Fc2:1;
         UBYTE	Fc1:1;
#endif
      }Bits;
      UBYTE	Byte;
   };
}TSPRINIT;

typedef struct
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	Up:1;
         UBYTE	Down:1;
         UBYTE	Left:1;
         UBYTE	Right:1;
         UBYTE	Option1:1;
         UBYTE	Option2:1;
         UBYTE	Inside:1;
         UBYTE	Outside:1;
#else
         UBYTE	Outside:1;
         UBYTE	Inside:1;
         UBYTE	Option2:1;
         UBYTE	Option1:1;
         UBYTE	Right:1;
         UBYTE	Left:1;
         UBYTE	Down:1;
         UBYTE	Up:1;
#endif
      }Bits;
      UBYTE	Byte;
   };
}TJOYSTICK;

typedef struct
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	spare:5;
         UBYTE	Cart1IO:1;
         UBYTE	Cart0IO:1;
         UBYTE	Pause:1;
#else
         UBYTE	Pause:1;
         UBYTE	Cart0IO:1;
         UBYTE	Cart1IO:1;
         UBYTE	spare:5;
#endif
      }Bits;
      UBYTE	Byte;
   };
}TSWITCHES;

typedef struct
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	A;
         UBYTE	B;
         UBYTE	C;
         UBYTE	D;
#else
         UBYTE	D;
         UBYTE	C;
         UBYTE	B;
         UBYTE	A;
#endif
      }Bytes;
      struct
      {
#ifdef MSB_FIRST
         UWORD	AB;
         UWORD	CD;
#else
         UWORD	CD;
         UWORD	AB;
#endif
      }Words;
      ULONG	Long;
   };
}TMATHABCD;

typedef struct
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	E;
         UBYTE	F;
         UBYTE	G;
         UBYTE	H;
#else
         UBYTE	H;
         UBYTE	G;
         UBYTE	F;
         UBYTE	E;
#endif
      }Bytes;
      struct
      {
#ifdef MSB_FIRST
         UWORD	EF;
         UWORD	GH;
#else
         UWORD	GH;
         UWORD	EF;
#endif
      }Words;
      ULONG	Long;
   };
}TMATHEFGH;

typedef struct
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	J;
         UBYTE	K;
         UBYTE	L;
         UBYTE	M;
#else
         UBYTE	M;
         UBYTE	L;
         UBYTE	K;
         UBYTE	J;
#endif
      }Bytes;
      struct
      {
#ifdef MSB_FIRST
         UWORD	JK;
         UWORD	LM;
#else
         UWORD	LM;
         UWORD	JK;
#endif
      }Words;
      ULONG	Long;
   };
}TMATHJKLM;

typedef struct
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	xx2;
         UBYTE	xx1;
         UBYTE	N;
         UBYTE	P;
#else
         UBYTE	P;
         UBYTE	N;
         UBYTE	xx1;
         UBYTE	xx2;
#endif
      }Bytes;
      struct
      {
#ifdef MSB_FIRST
         UWORD	xx1;
         UWORD	NP;
#else
         UWORD	NP;
         UWORD	xx1;
#endif
      }Words;
      ULONG	Long;
   };
}TMATHNP;

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
      inline ULONG LineInit(ULONG voff) {
      #include "susie_LineInit.h"
      }
#else
      ULONG	LineInit(ULONG voff);
#endif
#ifdef SUSIE_INLINE_LineGetPixel
      inline ULONG LineGetPixel(void) {
      #include "susie_LineGetPixel.h"
      }
#else
      ULONG	LineGetPixel(void);
#endif
#ifdef SUSIE_INLINE_LineGetBits
      inline ULONG LineGetBits(ULONG bits) {
      #include "susie_LineGetBits.h"
      }
#else
      ULONG	LineGetBits(ULONG bits);
#endif

#ifdef SUSIE_INLINE_PROCESSPIXEL
      inline void	ProcessPixel(ULONG hoff,ULONG pixel) {
      #include "susie_processpixel.h"
      }
#else
      void    ProcessPixel(ULONG hoff,ULONG pixel);
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

   private:
      CSystem&	mSystem;
      ULONG cycles_used;

      UUWORD		mTMPADR;		// ENG
      UUWORD		mTILTACUM;		// ENG
      UUWORD		mHOFF;			// CPU
      UUWORD		mVOFF;			// CPU
      UUWORD		mVIDBAS;		// CPU
      UUWORD		mCOLLBAS;		// CPU
      UUWORD		mVIDADR;		// ENG
      UUWORD		mCOLLADR;		// ENG
      UUWORD		mSCBNEXT;		// SCB
      UUWORD		mSPRDLINE;		// SCB
      UUWORD		mHPOSSTRT;		// SCB
      UUWORD		mVPOSSTRT;		// SCB
      UUWORD		mSPRHSIZ;		// SCB
      UUWORD		mSPRVSIZ;		// SCB
      UUWORD		mSTRETCH;		// ENG
      UUWORD		mTILT;			// ENG
      UUWORD		mSPRDOFF;		// ENG
      UUWORD		mSPRVPOS;		// ENG
      UUWORD		mCOLLOFF;		// CPU
      UUWORD		mVSIZACUM;		// ENG
      UUWORD		mHSIZACUM;		//    K.s creation
      UUWORD		mHSIZOFF;		// CPU
      UUWORD		mVSIZOFF;		// CPU
      UUWORD		mSCBADR;		// ENG
      UUWORD		mPROCADR;		// ENG

      TMATHABCD	mMATHABCD;		// ENG
      TMATHEFGH	mMATHEFGH;		// ENG
      TMATHJKLM	mMATHJKLM;		// ENG
      TMATHNP		mMATHNP;		// ENG
      int			mMATHAB_sign;
      int			mMATHCD_sign;
      int			mMATHEFGH_sign;

      int			mSPRCTL0_Type;			// SCB
      int			mSPRCTL0_Vflip;
      int			mSPRCTL0_Hflip;
      int			mSPRCTL0_PixelBits;

      int			mSPRCTL1_StartLeft;		// SCB
      int			mSPRCTL1_StartUp;
      int			mSPRCTL1_SkipSprite;
      int			mSPRCTL1_ReloadPalette;
      int			mSPRCTL1_ReloadDepth;
      int			mSPRCTL1_Sizing;
      int			mSPRCTL1_Literal;

      int			mSPRCOLL_Number;		//CPU
      int			mSPRCOLL_Collide;

      int			mSPRSYS_StopOnCurrent;	//CPU
      int			mSPRSYS_LeftHand;
      int			mSPRSYS_VStretch;
      int			mSPRSYS_NoCollide;
      int			mSPRSYS_Accumulate;
      int			mSPRSYS_SignedMath;
      int			mSPRSYS_Status;
      int			mSPRSYS_UnsafeAccess;
      int			mSPRSYS_LastCarry;
      int			mSPRSYS_Mathbit;
      int			mSPRSYS_MathInProgress;

      ULONG		mSUZYBUSEN;		// CPU

      TSPRINIT	mSPRINIT;		// CPU

      ULONG		mSPRGO;			// CPU
      int			mEVERON;

      UBYTE		mPenIndex[16];	// SCB

      // Line rendering related variables

      ULONG		mLineType;
      ULONG		mLineShiftRegCount;
      ULONG		mLineShiftReg;
      ULONG		mLineRepeatCount;
      ULONG		mLinePixel;
      ULONG		mLinePacketBitsLeft;

      int			mCollision;
#ifdef MY_NO_STATIC
      int vquadoff;
      int hquadoff;
#endif

      UBYTE		*mRamPointer;

      ULONG		mLineBaseAddress;
      ULONG		mLineCollisionAddress;

      // Joystick switches

      TJOYSTICK	mJOYSTICK;
      TSWITCHES	mSWITCHES;
};

#endif
