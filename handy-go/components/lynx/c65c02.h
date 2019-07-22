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
//                       Handy - An Atari Lynx Emulator                     //
//                          Copyright (c) 1996,1997                         //
//                                 K. Wilkins                               //
//////////////////////////////////////////////////////////////////////////////
// 65C02 Emulation class                                                    //
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// This class emulates a 65C02 processor. It is interfaced to the rest of   //
// the system via the PEEK/POKE macros and a number of global variables     //
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

#ifndef C65C02_H
#define C65C02_H

//#include <crtdbg.h>
//#define	TRACE_CPU
#include "myadd.h"

#ifdef TRACE_CPU

#define TRACE_CPU0(msg)					_RPT1(_CRT_WARN,"C65C02::"msg" (Time=%012d)\n",gSystemCycleCount)
#define TRACE_CPU1(msg,arg1)			_RPT2(_CRT_WARN,"C65C02::"msg" (Time=%012d)\n",arg1,gSystemCycleCount)
#define TRACE_CPU2(msg,arg1,arg2)		_RPT3(_CRT_WARN,"C65C02::"msg" (Time=%012d)\n",arg1,arg2,gSystemCycleCount)
#define TRACE_CPU3(msg,arg1,arg2,arg3)	_RPT4(_CRT_WARN,"C65C02::"msg" (Time=%012d)\n",arg1,arg2,arg3,gSystemCycleCount)

#else

#define TRACE_CPU0(msg)
#define TRACE_CPU1(msg,arg1)
#define TRACE_CPU2(msg,arg1,arg2)
#define TRACE_CPU3(msg,arg1,arg2,arg3)

#endif

//
// Handy definitions
//

#define NMI_VECTOR	0xfffa
#define BOOT_VECTOR	0xfffc
#define IRQ_VECTOR	0xfffe

#define MAX_CPU_BREAKPOINTS	8

//
// ACCESS MACROS
//

//#define CPU_PEEK(m)				(mSystem.Peek_CPU(m))
//#define CPU_PEEKW(m)			(mSystem.PeekW_CPU(m))
//#define CPU_POKE(m1,m2)			(mSystem.Poke_CPU(m1,m2))

#define CPU_PEEK(m)				(((m<0xfc00)?mRamPointer[m]:mSystem.Peek_CPU(m)))
#define CPU_PEEK_RAM(m)          (mRamPointer[m])
#define CPU_PEEK_SYSTEM(m)       (mSystem.Peek_CPU(m))
#define CPU_PEEKW(m)			(((m<0xfc00)?(mRamPointer[m]+(mRamPointer[m+1]<<8)):mSystem.PeekW_CPU(m)))
#define CPU_PEEKW_SYSTEM(m)  (mSystem.PeekW_CPU(m))
#define CPU_POKE(m1,m2)			{if(m1<0xfc00) mRamPointer[m1]=m2; else mSystem.Poke_CPU(m1,m2);}
#define CPU_POKE_RAM(m1,m2)      mRamPointer[m1]=m2;

#ifdef MY_CPU_V2
#define CPU_UPDATE_mPC_region   
#define CPU_UPDATE_mPC(a)     mPC = a; CPU_UPDATE_mPC_region;

//#define CPU_PEEK_mPC_CHECK   if ( (mPC<0xfc00) != mPC_region ) printf("%s: Something went wromg!\n", __func__);
#define CPU_PEEK_mPC_CHECK   
 
#else
#define CPU_PEEK_mPC             (((mPC<0xfc00)?mRamPointer[mPC]:mSystem.Peek_CPU(mPC)))
#define CPU_PEEKW_mPC            (((mPC<0xfc00)?(mRamPointer[mPC]+(mRamPointer[mPC+1]<<8)):mSystem.PeekW_CPU(mPC)))

#define CPU_UPDATE_mPC(a)     mPC = a
#define CPU_UPDATE_mPC_region
#define CPU_PEEK_mPC_CHECK     
#endif

//#define MY_REG_VARS WORD_ALIGNED_ATTR UWORD
#define MY_REG_VARS ULONG

enum
{
   illegal=0,
   accu,
   imm,
   absl,
   zp,
   zpx,
   zpy,
   absx,
   absy,
   iabsx,
   impl,
   rel,
   zrel,
   indx,
   indy,
   iabs,
   ind
};

typedef struct
{
   int PS;		// Processor status register   8 bits
   int A;		// Accumulator                 8 bits
   int X;		// X index register            8 bits
   int Y;		// Y index register            8 bits
   int SP;		// Stack Pointer               8 bits
   int Opcode;	// Instruction opcode          8 bits
   int Operand;// Intructions operand		  16 bits
   int PC;		// Program Counter            16 bits
   bool NMI;
   bool IRQ;
   bool WAIT;
#ifdef _LYNXDBG
   int	cpuBreakpoints[MAX_CPU_BREAKPOINTS];
#endif
}C6502_REGS;

#define CPU_INLINE_Update

//
// The CPU emulation macros
//
#include "c6502mak.h"
//
// The CPU emulation macros
//

class C65C02
{
   public:
      C65C02(CSystemBase& parent);
      ~C65C02();

   public:
      void Reset(void);
      bool ContextSave(FILE *fp);
      bool ContextLoad(LSS_FILE *fp);

#ifdef CPU_INLINE_Update
      inline void Update(void)
      {
#ifdef MY_CPU_V2
#include "c65c02_update2.h"
#else
#include "c65c02_update.h"
#endif
      }
#else
    void Update(void);
#endif

      //		inline void SetBreakpoint(ULONG breakpoint) {mPcBreakpoint=breakpoint;};

      inline void SetRegs(C6502_REGS &regs)
      {
         PS(regs.PS);
         mA=regs.A;
         mX=regs.X;
         mY=regs.Y;
         mSP=regs.SP;
         mOpcode=regs.Opcode;
         mOperand=regs.Operand;
         CPU_UPDATE_mPC( regs.PC );
         gSystemCPUSleep=regs.WAIT;
#ifdef _LYNXDBG
         for(int loop=0;loop<MAX_CPU_BREAKPOINTS;loop++)	mPcBreakpoints[loop]=regs.cpuBreakpoints[loop];
#endif
         gSystemNMI=regs.NMI;
         gSystemIRQ=regs.IRQ;
      }

      inline void GetRegs(C6502_REGS &regs)
      {
         regs.PS=PS();
         regs.A=mA;
         regs.X=mX;
         regs.Y=mY;
         regs.SP=mSP;
         regs.Opcode=mOpcode;
         regs.Operand=mOperand;
         regs.PC=mPC;
         regs.WAIT=(gSystemCPUSleep)?true:false;
#ifdef _LYNXDBG
         for(int loop=0;loop<MAX_CPU_BREAKPOINTS;loop++)	regs.cpuBreakpoints[loop]=mPcBreakpoints[loop];
#endif
         regs.NMI=(gSystemNMI)?true:false;
         regs.IRQ=(gSystemIRQ)?true:false;
      }

      inline int GetPC(void) { return mPC; }

      inline void xILLEGAL(void)
      {
         //fprintf(stderr, "C65C02::Update() - Illegal opcode (%02x) at PC=$%04x.",mOpcode,mPC);
         gSystemCycleCount++;
      }

   private:
      CSystemBase	&mSystem;

      // CPU Flags & status
      MY_REG_VARS mA;		// Accumulator                 8 bits
      MY_REG_VARS mX;		// X index register            8 bits
      MY_REG_VARS mY;		// Y index register            8 bits
      MY_REG_VARS mSP;		// Stack Pointer               8 bits
      MY_REG_VARS mOpcode;  // Instruction opcode          8 bits
      MY_REG_VARS mOperand; // Intructions operand		  16 bits
      MY_REG_VARS mPC;		// Program Counter            16 bits
      // ULONG mPC_region;
      // WORD_ALIGNED_ATTR UWORD mPC;

      MY_REG_VARS mN;		// N flag for processor status register
      MY_REG_VARS mV;		// V flag for processor status register
      MY_REG_VARS mB;		// B flag for processor status register
      MY_REG_VARS mD;		// D flag for processor status register
      MY_REG_VARS mI;		// I flag for processor status register
      MY_REG_VARS mZ;		// Z flag for processor status register
      MY_REG_VARS mC;		// C flag for processor status register

      MY_REG_VARS mIRQActive;

#ifdef _LYNXDBG
      int mPcBreakpoints[MAX_CPU_BREAKPOINTS];
      int mDbgFlag;
#endif
      UBYTE *mRamPointer;

      // Associated lookup tables

      // int mBCDTable[2][256];

      //
      // Opcode prototypes
      //

   //private:

      // Answers value of the Processor Status register
      inline int PS() const
      {
         UBYTE ps = 0x20;
         if(mN) ps|=0x80;
         if(mV) ps|=0x40;
         if(mB) ps|=0x10;
         if(mD) ps|=0x08;
         if(mI) ps|=0x04;
         if(mZ) ps|=0x02;
         if(mC) ps|=0x01;
         return ps;
      }


      // Change the processor flags to correspond to the given value
      inline void PS(int ps)
      {
         mN=ps&0x80;
         mV=ps&0x40;
         mB=ps&0x10;
         mD=ps&0x08;
         mI=ps&0x04;
         mZ=ps&0x02;
         mC=ps&0x01;
      }

};


#endif

