#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"
#include "myadd.h"
#include "c65c02.h"

C65C02::C65C02(CSystemBase& parent)
     :mSystem(parent)
  {
     TRACE_CPU0("C65C02()");
     // Compute the BCD lookup table
     /*
     for(UWORD t=0;t<256;++t)
     {
        mBCDTable[0][t]=((t >> 4) * 10) + (t & 0x0f);
        mBCDTable[1][t]=(((t % 100) / 10) << 4) | (t % 10);
     }
     */
#ifdef _LYNXDBG
         for(int loop=0;loop<MAX_CPU_BREAKPOINTS;loop++)    mPcBreakpoints[loop]=0xfffffff;
         mDbgFlag=0;
#endif
     Reset();

  }

C65C02::~C65C02()
  {
     TRACE_CPU0("~C65C02()");
  }

void C65C02::Reset(void)
  {
     TRACE_CPU0("Reset()");
     mRamPointer=mSystem.GetRamPointer();
     mA=0;
     mX=0;
     mY=0;
     mSP=0xff;
     mOpcode=0;
     mOperand=0;
     CPU_UPDATE_mPC( CPU_PEEKW_SYSTEM(BOOT_VECTOR) );
     mN=FALSE;
     mV=FALSE;
     mB=FALSE;
     mD=FALSE;
     mI=TRUE;
     mZ=TRUE;
     mC=FALSE;
     mIRQActive=FALSE;

     gSystemNMI=FALSE;
     gSystemIRQ=FALSE;
     gSystemCPUSleep=FALSE;
     gSystemCPUSleep_Saved=FALSE;
  }


  bool C65C02::ContextSave(FILE *fp)
  {
     TRACE_CPU0("ContextSave()");
     int mPS;
     mPS=PS();
     if(!fprintf(fp,"C6502::ContextSave")) return 0;
     if(!fwrite(&mA,sizeof(ULONG),1,fp)) return 0;
     if(!fwrite(&mX,sizeof(ULONG),1,fp)) return 0;
     if(!fwrite(&mY,sizeof(ULONG),1,fp)) return 0;
     if(!fwrite(&mSP,sizeof(ULONG),1,fp)) return 0;
     if(!fwrite(&mPS,sizeof(ULONG),1,fp)) return 0;
     if(!fwrite(&mPC,sizeof(ULONG),1,fp)) return 0;
     if(!fwrite(&mIRQActive,sizeof(ULONG),1,fp)) return 0;
     return 1;
  }

   bool C65C02::ContextLoad(LSS_FILE *fp)
  {
     TRACE_CPU0("ContextLoad()");
     int mPS;
     char teststr[100]="XXXXXXXXXXXXXXXXXX";
     if(!lss_read(teststr,sizeof(char),18,fp)) return 0;
     if(strcmp(teststr,"C6502::ContextSave")!=0) return 0;
     ULONG tmp;
     if(!lss_read(&tmp,sizeof(ULONG),1,fp)) return 0;
     mA = tmp;
     if(!lss_read(&tmp,sizeof(ULONG),1,fp)) return 0;
     mX = tmp;
     if(!lss_read(&tmp,sizeof(ULONG),1,fp)) return 0;
     mY = tmp;
     if(!lss_read(&tmp,sizeof(ULONG),1,fp)) return 0;
     mSP = tmp;
     if(!lss_read(&tmp,sizeof(ULONG),1,fp)) return 0;
     mPS = tmp;
     if(!lss_read(&tmp,sizeof(ULONG),1,fp)) return 0;
     CPU_UPDATE_mPC( tmp );
     if(!lss_read(&tmp,sizeof(ULONG),1,fp)) return 0;
     mIRQActive = tmp;
     PS(mPS);
     return 1;
  }

#ifndef CPU_INLINE_Update
void IRAM_ATTR C65C02::Update(void)
{
#include "c65c02_update.h"
}
#endif
