         //
         // NMI is currently unused by the lynx so lets save some time
         //
         //         Check NMI & IRQ status, prioritise NMI then IRQ
         //         if(mNMI)
         //         {
         //             // Mark the NMI as services
         //             mNMI=FALSE;
         //             mProcessingInterrupt++;
         //
         //             // Push processor status
         //             CPU_POKE(0x0100+mSP--,mPC>>8);
         //             CPU_POKE(0x0100+mSP--,mPC&0x00ff);
         //             CPU_POKE(0x0100+mSP--,PS());
         //
         //             // Pick up the new PC
         //             mPC=CPU_PEEKW(NMI_VECTOR);
         //         }

         //    fprintf(stderr, "cpu update\n");

         if(gSystemIRQ && !mI)
         {
            TRACE_CPU1("Update() IRQ taken at PC=%04x",mPC);
            // IRQ signal clearance is handled by CMikie::Update() as this
            // is the only source of interrupts

            // Push processor status
            PUSH(mPC>>8);
            PUSH(mPC&0xff);
            PUSH(PS()&0xef);        // Clear B flag on stack

            mI=TRUE;                // Stop further interrupts
            mD=FALSE;               // Clear decimal mode

            // Pick up the new PC
            //mPC=CPU_PEEKW(IRQ_VECTOR);
            CPU_UPDATE_mPC( CPU_PEEKW_SYSTEM(IRQ_VECTOR) );

            // Save the sleep state as an irq has possibly woken the processor
            gSystemCPUSleep_Saved=gSystemCPUSleep;
            gSystemCPUSleep=FALSE;

            // Log the irq entry time
            gIRQEntryCycle=gSystemCycleCount;

            // Clear the interrupt status line
            gSystemIRQ=FALSE;
         }

         //
         // If the CPU is asleep then skip to the next timer event
         //
         if(gSystemCPUSleep) return;

         // Fetch opcode
/*
         CPU_PEEK_mPC_CHECK
         mOpcode=CPU_PEEK_mPC;
         TRACE_CPU2("Update() PC=$%04x, Opcode=%02x",mPC,mOpcode);
         CPU_MPC_PL1;
         #include "c65c02_update2_switch.h"
*/
if (mPC<0xfc00)
{
//const UWORD mPC_region = 1;
#ifdef CPU_PEEK_mPC
#undef CPU_PEEK_mPC
#undef CPU_PEEKW_mPC
#endif

#define CPU_PEEK_mPC             mRamPointer[mPC]
#define CPU_PEEKW_mPC            (mRamPointer[mPC]+(mRamPointer[mPC+1]<<8))

         #include "c65c02_update2_switch.h"
}
else
{
//const UWORD mPC_region = 0;
#ifdef CPU_PEEK_mPC
#undef CPU_PEEK_mPC
#undef CPU_PEEKW_mPC
#endif

#define CPU_PEEK_mPC             mSystem.Peek_CPU(mPC)
#define CPU_PEEKW_mPC            mSystem.PeekW_CPU(mPC)

         #include "c65c02_update2_switch.h"
}         
         
#ifdef MY_DEBUG_OUT         
         cpu_update++;
         cpu_calls[mOpcode]++;
#endif


#ifdef _LYNXDBG

         // Trigger breakpoint if required

         for(int loop=0;loop<MAX_CPU_BREAKPOINTS;loop++)
         {
            if(mPcBreakpoints[loop]==mPC)
            {
               gBreakpointHit=TRUE;
               mSystem.DebugTrace(0);
            }
         }

         // Check code level debug features
         // back to back CPX ($Absolute)
         // on the 2nd Occurance we do some debug
         if(mOpcode==0xec)
         {
            if(mDbgFlag)
            {
               // We shoud do some debug now
               if(!mOperand)
               {
                  // Trigger a breakpoint
                  gBreakpointHit=TRUE;
                  // Generate a debug trail output
                  mSystem.DebugTrace(0);
               }
               else
               {
                  // Generate a debug trail output
                  mSystem.DebugTrace(mOperand);
               }
               mDbgFlag=0;
            }
            else
            {
               if(mOperand==0x5aa5) mDbgFlag=1; else mDbgFlag=0;
            }
         }
         else
         {
            mDbgFlag=0;
         }
#endif
