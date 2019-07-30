         mOpcode=CPU_PEEK_mPC;
         TRACE_CPU2("Update() PC=$%04x, Opcode=%02x",mPC,mOpcode);
         CPU_MPC_PL1;
#ifdef ODROID_DEBUG_PERF_CPU_ALL_INSTR
         ODROID_DEBUG_PERF_START()
#endif

         // Execute Opcode

         switch(mOpcode)
         {

            //
            // 0x00
            //
            case 0x00:
               SYSTEM_CYCLE_ADD((1+(6*CPU_RDWR_CYC)));
               // IMPLIED
               xBRK();
               break;
            case 0x01:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xINDIRECT_X();
               xORA();
               break;
            case 0x02:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x03:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x04:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xZEROPAGE();
               xTSB();
               break;
            case 0x05:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xORA();
               break;
            case 0x06:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xZEROPAGE();
               xASL();
               break;
            case 0x07:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0x08:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               // IMPLIED
               xPHP();
               break;
            case 0x09:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xIMMEDIATE();
               xORA();
               break;
            case 0x0A:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xASLA();
               break;
            case 0x0B:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x0C:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xABSOLUTE();
               xTSB();
               break;
            case 0x0D:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xORA();
               break;
            case 0x0E:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xABSOLUTE();
               xASL();
               break;
            case 0x0F:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

               //
               // 0x10
               //
            case 0x10:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // RELATIVE (IN FUNCTION)
               xBPL();
               break;
            case 0x11:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xINDIRECT_Y();
               xORA();
               break;
            case 0x12:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xINDIRECT();
               xORA();
               break;
            case 0x13:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x14:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xZEROPAGE();
               xTRB();
               break;
            case 0x15:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xORA();
               break;
            case 0x16:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xASL();
               break;
            case 0x17:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0x18:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xCLC();
               break;
            case 0x19:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_Y();
               xORA();
               break;
            case 0x1A:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xINCA();
               break;
            case 0x1B:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x1C:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xABSOLUTE();
               xTRB();
               break;
            case 0x1D:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xORA();
               break;
            case 0x1E:
               SYSTEM_CYCLE_ADD((1+(6*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xASL();
               break;
            case 0x1F:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

               //
               // 0x20
               //
            case 0x20:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xABSOLUTE();
               xJSR();
               break;
            case 0x21:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xINDIRECT_X();
               xAND();
               break;
            case 0x22:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x23:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x24:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xBIT();
               break;
            case 0x25:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xAND();
               break;
            case 0x26:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xZEROPAGE();
               xROL();
               break;
            case 0x27:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0x28:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               // IMPLIED
               xPLP();
               break;
            case 0x29:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               xIMMEDIATE();
               xAND();
               break;
            case 0x2A:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xROLA();
               break;
            case 0x2B:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x2C:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xBIT();
               break;
            case 0x2D:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xAND();
               break;
            case 0x2E:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xABSOLUTE();
               xROL();
               break;
            case 0x2F:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

               //
               // 0x30
               //
            case 0x30:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // RELATIVE (IN FUNCTION)
               xBMI();
               break;
            case 0x31:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xINDIRECT_Y();
               xAND();
               break;
            case 0x32:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xINDIRECT();
               xAND();
               break;
            case 0x33:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x34:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xBIT();
               break;
            case 0x35:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xAND();
               break;
            case 0x36:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xROL();
               break;
            case 0x37:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0x38:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xSEC();
               break;
            case 0x39:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_Y();
               xAND();
               break;
            case 0x3A:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xDECA();
               break;
            case 0x3B:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x3C:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xBIT();
               break;
            case 0x3D:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xAND();
               break;
            case 0x3E:
               SYSTEM_CYCLE_ADD((1+(6*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xROL();
               break;
            case 0x3F:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

               //
               // 0x40
               //
            case 0x40:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               // Only clear IRQ if this is not a BRK instruction based RTI

               // B flag is on the stack cant test the flag
               UWORD tmp;
               PULL(tmp);
               PUSH (tmp);
               if(!(tmp&0x10))
               {
                  SYSTEM_VAR(gSystemCPUSleep)=SYSTEM_VAR(gSystemCPUSleep_Saved);

                  // If were in sleep mode then we need to push the
                  // wakeup counter along by the same number of cycles
                  // we have used during the sleep period
                  if(SYSTEM_VAR(gSystemCPUSleep))
                  {
                     SYSTEM_VAR(gCPUWakeupTime)+=SYSTEM_VAR(gSystemCycleCount)-SYSTEM_VAR(gIRQEntryCycle);
                  }
               }
               // IMPLIED
               xRTI();
               break;
            case 0x41:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xINDIRECT_X();
               xEOR();
               break;
            case 0x42:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x43:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x44:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x45:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xEOR();
               break;
            case 0x46:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xZEROPAGE();
               xLSR();
               break;
            case 0x47:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0x48:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               // IMPLIED
               xPHA();
               break;
            case 0x49:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               xIMMEDIATE();
               xEOR();
               break;
            case 0x4A:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xLSRA();
               break;
            case 0x4B:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x4C:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xABSOLUTE();
               xJMP();
               break;
            case 0x4D:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xEOR();
               break;
            case 0x4E:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xABSOLUTE();
               xLSR();
               break;
            case 0x4F:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

               //
               // 0x50
               //
            case 0x50:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // RELATIVE (IN FUNCTION)
               xBVC();
               break;
            case 0x51:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xINDIRECT_Y();
               xEOR();
               break;
            case 0x52:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xINDIRECT();
               xEOR();
               break;
            case 0x53:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x54:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x55:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xEOR();
               break;
            case 0x56:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xLSR();
               break;
            case 0x57:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0x58:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xCLI();
               break;
            case 0x59:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_Y();
               xEOR();
               break;
            case 0x5A:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               // IMPLIED
               xPHY();
               break;
            case 0x5B:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x5C:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x5D:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xEOR();
               break;
            case 0x5E:
               SYSTEM_CYCLE_ADD((1+(6*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xLSR();
               break;
            case 0x5F:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

               //
               // 0x60
               //
            case 0x60:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               // IMPLIED
               xRTS();
               break;
            case 0x61:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xINDIRECT_X();
               xADC();
               break;
            case 0x62:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x63:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x64:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xSTZ();
               break;
            case 0x65:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xADC();
               break;
            case 0x66:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xZEROPAGE();
               xROR();
               break;
            case 0x67:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0x68:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               // IMPLIED
               xPLA();
               break;
            case 0x69:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               xIMMEDIATE();
               xADC();
               break;
            case 0x6A:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xRORA();
               break;
            case 0x6B:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x6C:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xINDIRECT_ABSOLUTE();
               xJMP();
               break;
            case 0x6D:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xADC();
               break;
            case 0x6E:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xABSOLUTE();
               xROR();
               break;
            case 0x6F:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

               //
               // 0x70
               //
            case 0x70:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // RELATIVE (IN FUNCTION)
               xBVS();
               break;
            case 0x71:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xINDIRECT_Y();
               xADC();
               break;
            case 0x72:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xINDIRECT();
               xADC();
               break;
            case 0x73:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x74:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xSTZ();
               break;
            case 0x75:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xADC();
               break;
            case 0x76:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xROR();
               break;
            case 0x77:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0x78:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xSEI();
               break;
            case 0x79:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_Y();
               xADC();
               break;
            case 0x7A:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               // IMPLIED
               xPLY();
               break;
            case 0x7B:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x7C:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xINDIRECT_ABSOLUTE_X();
               xJMP();
               break;
            case 0x7D:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xADC();
               break;
            case 0x7E:
               SYSTEM_CYCLE_ADD((1+(6*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xROR();
               break;
            case 0x7F:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

               //
               // 0x80
               //
            case 0x80:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               // RELATIVE (IN FUNCTION)
               xBRA();
               break;
            case 0x81:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xINDIRECT_X();
               xSTA();
               break;
            case 0x82:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x83:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x84:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xSTY();
               break;
            case 0x85:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xSTA();
               break;
            case 0x86:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xSTX();
               break;
            case 0x87:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0x88:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xDEY();
               break;
            case 0x89:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               xIMMEDIATE();
               xBIT();
               break;
            case 0x8A:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xTXA();
               break;
            case 0x8B:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x8C:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xSTY();
               break;
            case 0x8D:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xSTA();
               break;
            case 0x8E:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xSTX();
               break;
            case 0x8F:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

               //
               // 0x90
               //
            case 0x90:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // RELATIVE (IN FUNCTION)
               xBCC();
               break;
            case 0x91:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xINDIRECT_Y();
               xSTA();
               break;
            case 0x92:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xINDIRECT();
               xSTA();
               break;
            case 0x93:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x94:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xSTY();
               break;
            case 0x95:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xSTA();
               break;
            case 0x96:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xZEROPAGE_Y();
               xSTX();
               break;
            case 0x97:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0x98:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xTYA();
               break;
            case 0x99:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xABSOLUTE_Y();
               xSTA();
               break;
            case 0x9A:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xTXS();
               break;
            case 0x9B:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0x9C:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xSTZ();
               break;
            case 0x9D:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xSTA();
               break;
            case 0x9E:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xSTZ();
               break;
            case 0x9F:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

               //
               // 0xA0
               //
            case 0xA0:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               xIMMEDIATE();
               xLDY();
               break;
            case 0xA1:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xINDIRECT_X();
               xLDA();
               break;
            case 0xA2:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               xIMMEDIATE();
               xLDX();
               break;
            case 0xA3:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xA4:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xLDY();
               break;
            case 0xA5:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xLDA();
               break;
            case 0xA6:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xLDX();
               break;
            case 0xA7:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0xA8:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xTAY();
               break;
            case 0xA9:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               xIMMEDIATE();
               xLDA();
               break;
            case 0xAA:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xTAX();
               break;
            case 0xAB:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xAC:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xLDY();
               break;
            case 0xAD: // often: 145k
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xLDA();
               break;
            case 0xAE:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xLDX();
               break;
            case 0xAF:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

               //
               // 0xB0
               //
            case 0xB0:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // RELATIVE (IN FUNCTION)
               xBCS();
               break;
            case 0xB1:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xINDIRECT_Y();
               xLDA();
               break;
            case 0xB2:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xINDIRECT();
               xLDA();
               break;
            case 0xB3:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xB4:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xLDY();
               break;
            case 0xB5:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xLDA();
               break;
            case 0xB6:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xZEROPAGE_Y();
               xLDX();
               break;
            case 0xB7:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0xB8:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xCLV();
               break;
            case 0xB9:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_Y();
               xLDA();
               break;
            case 0xBA:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xTSX();
               break;
            case 0xBB:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xBC:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xLDY();
               break;
            case 0xBD:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xLDA();
               break;
            case 0xBE:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_Y();
               xLDX();
               break;
            case 0xBF:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

               //
               // 0xC0
               //
            case 0xC0:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               xIMMEDIATE();
               xCPY();
               break;
            case 0xC1:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xINDIRECT_X();
               xCMP();
               break;
            case 0xC2:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xC3:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xC4:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xCPY();
               break;
            case 0xC5:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xCMP();
               break;
            case 0xC6:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xZEROPAGE();
               xDEC();
               break;
            case 0xC7:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0xC8:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xINY();
               break;
            case 0xC9:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               xIMMEDIATE();
               xCMP();
               break;
            case 0xCA:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xDEX();
               break;
            case 0xCB:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xWAI();
               break;
            case 0xCC:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xCPY();
               break;
            case 0xCD:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xCMP();
               break;
            case 0xCE:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xABSOLUTE();
               xDEC();
               break;
            case 0xCF:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

               //
               // 0xD0
               //
            case 0xD0: // often: 152k
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // RELATIVE (IN FUNCTION)
               xBNE();
               break;
            case 0xD1:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xINDIRECT_Y();
               xCMP();
               break;
            case 0xD2:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xINDIRECT();
               xCMP();
               break;
            case 0xD3:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xD4:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xD5:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xCMP();
               break;
            case 0xD6:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xDEC();
               break;
            case 0xD7:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0xD8:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xCLD();
               break;
            case 0xD9:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_Y();
               xCMP();
               break;
            case 0xDA:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               // IMPLIED
               xPHX();
               break;
            case 0xDB:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xSTP();
               break;
            case 0xDC:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xDD:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xCMP();
               break;
            case 0xDE:
               SYSTEM_CYCLE_ADD((1+(6*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xDEC();
               break;
            case 0xDF:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

               //
               // 0xE0
               //
            case 0xE0:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               xIMMEDIATE();
               xCPX();
               break;
            case 0xE1:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xINDIRECT_X();
               xSBC();
               break;
            case 0xE2:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xE3:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xE4:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xCPX();
               break;
            case 0xE5:
               SYSTEM_CYCLE_ADD((1+(2*CPU_RDWR_CYC)));
               xZEROPAGE();
               xSBC();
               break;
            case 0xE6:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xZEROPAGE();
               xINC();
               break;
            case 0xE7:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0xE8:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xINX();
               break;
            case 0xE9:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               xIMMEDIATE();
               xSBC();
               break;
            case 0xEA:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xNOP();
               break;
            case 0xEB:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xEC:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xCPX();
               break;
            case 0xED:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE();
               xSBC();
               break;
            case 0xEE:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xABSOLUTE();
               xINC();
               break;
            case 0xEF:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

               //
               // 0xF0
               //
            case 0xF0:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // RELATIVE (IN FUNCTION)
               xBEQ();
               break;
            case 0xF1:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xINDIRECT_Y();
               xSBC();
               break;
            case 0xF2:
               SYSTEM_CYCLE_ADD((1+(4*CPU_RDWR_CYC)));
               xINDIRECT();
               xSBC();
               break;
            case 0xF3:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xF4:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xF5:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xSBC();
               break;
            case 0xF6:
               SYSTEM_CYCLE_ADD((1+(5*CPU_RDWR_CYC)));
               xZEROPAGE_X();
               xINC();
               break;
            case 0xF7:
               // *** ILLEGAL ***
               xILLEGAL();
               break;

            case 0xF8:
               SYSTEM_CYCLE_ADD((1+(1*CPU_RDWR_CYC)));
               // IMPLIED
               xSED();
               break;
            case 0xF9:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_Y();
               xSBC();
               break;
            case 0xFA:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               // IMPLIED
               xPLX();
               break;
            case 0xFB:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xFC:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
            case 0xFD:
               SYSTEM_CYCLE_ADD((1+(3*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xSBC();
               break;
            case 0xFE:
               SYSTEM_CYCLE_ADD((1+(6*CPU_RDWR_CYC)));
               xABSOLUTE_X();
               xINC();
               break;
            case 0xFF:
               // *** ILLEGAL ***
               xILLEGAL();
               break;
         }

#ifdef ODROID_DEBUG_PERF_CPU_ALL_INSTR
         ODROID_DEBUG_PERF_INCR(0x0100 + (ULONG)mOpcode)
         ODROID_DEBUG_PERF_INCR(ODROID_DEBUG_PERF_CPU)
#endif
