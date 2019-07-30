   switch(addr&0xff)
   {
      case (TIM0BKUP&0xff):
         mTIM_0_BKUP=data;
         TRACE_MIKIE2("Poke(TIM0BKUP,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (TIM1BKUP&0xff):
         mTIM_1_BKUP=data;
         TRACE_MIKIE2("Poke(TIM1BKUP,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (TIM2BKUP&0xff):
         mTIM_2_BKUP=data;
         TRACE_MIKIE2("Poke(TIM2BKUP,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (TIM3BKUP&0xff):
         mTIM_3_BKUP=data;
         TRACE_MIKIE2("Poke(TIM3BKUP,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (TIM4BKUP&0xff):
         mTIM_4_BKUP=data;
         TRACE_MIKIE2("Poke(TIM4BKUP,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (TIM5BKUP&0xff):
         mTIM_5_BKUP=data;
         TRACE_MIKIE2("Poke(TIM5BKUP,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (TIM6BKUP&0xff):
         mTIM_6_BKUP=data;
         TRACE_MIKIE2("Poke(TIM6BKUP,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (TIM7BKUP&0xff):
         mTIM_7_BKUP=data;
         TRACE_MIKIE2("Poke(TIM7BKUP,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;


      case (TIM0CTLA&0xff):
         Poke_TIM0CTLA(data);
         break;
      case (TIM1CTLA&0xff):
         Poke_TIM1CTLA(data);
         break;
      case (TIM2CTLA&0xff):
         Poke_TIM2CTLA(data);
         break;
      case (TIM3CTLA&0xff):
         Poke_TIM3CTLA(data);
         break;
      case (TIM4CTLA&0xff):
         Poke_TIM4CTLA(data);
         break;
      case (TIM5CTLA&0xff):
         Poke_TIM5CTLA(data);
         break;
      case (TIM6CTLA&0xff):
         Poke_TIM6CTLA(data);
         break;
      case (TIM7CTLA&0xff):
         Poke_TIM7CTLA(data);
         break;


      case (TIM0CNT&0xff):
         mTIM_0_CURRENT=data;
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         TRACE_MIKIE2("Poke(TIM0CNT ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (TIM1CNT&0xff):
         mTIM_1_CURRENT=data;
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         TRACE_MIKIE2("Poke(TIM1CNT ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (TIM2CNT&0xff):
         mTIM_2_CURRENT=data;
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         TRACE_MIKIE2("Poke(TIM2CNT ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (TIM3CNT&0xff):
         mTIM_3_CURRENT=data;
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         TRACE_MIKIE2("Poke(TIM3CNT ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (TIM4CNT&0xff):
         mTIM_4_CURRENT=data;
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         TRACE_MIKIE2("Poke(TIM4CNT ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (TIM5CNT&0xff):
         mTIM_5_CURRENT=data;
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         TRACE_MIKIE2("Poke(TIM5CNT ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (TIM6CNT&0xff):
         mTIM_6_CURRENT=data;
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         TRACE_MIKIE2("Poke(TIM6CNT ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (TIM7CNT&0xff):
         mTIM_7_CURRENT=data;
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         TRACE_MIKIE2("Poke(TIM7CNT ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;

      case (TIM0CTLB&0xff):
         mTIM_0_TIMER_DONE=data&0x08;
         mTIM_0_LAST_CLOCK=data&0x04;
         mTIM_0_BORROW_IN=data&0x02;
         mTIM_0_BORROW_OUT=data&0x01;
         TRACE_MIKIE2("Poke(TIM0CTLB ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         //         BlowOut();
         break;
      case (TIM1CTLB&0xff):
         mTIM_1_TIMER_DONE=data&0x08;
         mTIM_1_LAST_CLOCK=data&0x04;
         mTIM_1_BORROW_IN=data&0x02;
         mTIM_1_BORROW_OUT=data&0x01;
         TRACE_MIKIE2("Poke(TIM1CTLB ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         //         BlowOut();
         break;
      case (TIM2CTLB&0xff):
         mTIM_2_TIMER_DONE=data&0x08;
         mTIM_2_LAST_CLOCK=data&0x04;
         mTIM_2_BORROW_IN=data&0x02;
         mTIM_2_BORROW_OUT=data&0x01;
         TRACE_MIKIE2("Poke(TIM2CTLB ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         //         BlowOut();
         break;
      case (TIM3CTLB&0xff):
         mTIM_3_TIMER_DONE=data&0x08;
         mTIM_3_LAST_CLOCK=data&0x04;
         mTIM_3_BORROW_IN=data&0x02;
         mTIM_3_BORROW_OUT=data&0x01;
         TRACE_MIKIE2("Poke(TIM3CTLB ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         //         BlowOut();
         break;
      case (TIM4CTLB&0xff):
         mTIM_4_TIMER_DONE=data&0x08;
         mTIM_4_LAST_CLOCK=data&0x04;
         mTIM_4_BORROW_IN=data&0x02;
         mTIM_4_BORROW_OUT=data&0x01;
         TRACE_MIKIE2("Poke(TIM4CTLB ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         //         BlowOut();
         break;
      case (TIM5CTLB&0xff):
         mTIM_5_TIMER_DONE=data&0x08;
         mTIM_5_LAST_CLOCK=data&0x04;
         mTIM_5_BORROW_IN=data&0x02;
         mTIM_5_BORROW_OUT=data&0x01;
         TRACE_MIKIE2("Poke(TIM5CTLB ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         //         BlowOut();
         break;
      case (TIM6CTLB&0xff):
         mTIM_6_TIMER_DONE=data&0x08;
         mTIM_6_LAST_CLOCK=data&0x04;
         mTIM_6_BORROW_IN=data&0x02;
         mTIM_6_BORROW_OUT=data&0x01;
         TRACE_MIKIE2("Poke(TIM6CTLB ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         //         BlowOut();
         break;
      case (TIM7CTLB&0xff):
         mTIM_7_TIMER_DONE=data&0x08;
         mTIM_7_LAST_CLOCK=data&0x04;
         mTIM_7_BORROW_IN=data&0x02;
         mTIM_7_BORROW_OUT=data&0x01;
         TRACE_MIKIE2("Poke(TIM7CTLB ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         //         BlowOut();
         break;

      case (AUD0VOL&0xff):
         // Counter is disabled when volume is zero for optimisation
         // reasons, we must update the last use position to stop problems
         if(!mAUDIO_0_VOLUME && data)
      {
         mAUDIO_0_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
      }
         mAUDIO_0_VOLUME=(SBYTE)data;
         TRACE_MIKIE2("Poke(AUD0VOL,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD0SHFTFB&0xff):
         mAUDIO_0_WAVESHAPER&=0x001fff;
         mAUDIO_0_WAVESHAPER|=(ULONG)data<<13;
         TRACE_MIKIE2("Poke(AUD0SHFTB,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD0OUTVAL&0xff):
         mAUDIO_OUTPUT_0=data;
         TRACE_MIKIE2("Poke(AUD0OUTVAL,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD0L8SHFT&0xff):
         mAUDIO_0_WAVESHAPER&=0x1fff00;
         mAUDIO_0_WAVESHAPER|=data;
         TRACE_MIKIE2("Poke(AUD0L8SHFT,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD0TBACK&0xff):
         // Counter is disabled when backup is zero for optimisation
         // due to the fact that the output frequency will be above audio
         // range, we must update the last use position to stop problems
         if(!mAUDIO_0_BKUP && data)
      {
         mAUDIO_0_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
      }
         mAUDIO_0_BKUP=data;
         TRACE_MIKIE2("Poke(AUD0TBACK,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD0CTL&0xff):
         mAUDIO_0_ENABLE_RELOAD=data&0x10;
         mAUDIO_0_ENABLE_COUNT=data&0x08;
         mAUDIO_0_LINKING=data&0x07;
         mAUDIO_0_INTEGRATE_ENABLE=data&0x20;
         if(data&0x40) mAUDIO_0_TIMER_DONE=0;
         mAUDIO_0_WAVESHAPER&=0x1fefff;
         mAUDIO_0_WAVESHAPER|=(data&0x80)?0x001000:0x000000;
         if(data&0x48)
         {
            mAUDIO_0_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
            SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         }
         TRACE_MIKIE2("Poke(AUD0CTL,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD0COUNT&0xff):
         mAUDIO_0_CURRENT=data;
         TRACE_MIKIE2("Poke(AUD0COUNT,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD0MISC&0xff):
         mAUDIO_0_WAVESHAPER&=0x1ff0ff;
         mAUDIO_0_WAVESHAPER|=(data&0xf0)<<4;
         mAUDIO_0_BORROW_IN=data&0x02;
         mAUDIO_0_BORROW_OUT=data&0x01;
         mAUDIO_0_LAST_CLOCK=data&0x04;
         TRACE_MIKIE2("Poke(AUD0MISC,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;

      case (AUD1VOL&0xff):
         // Counter is disabled when volume is zero for optimisation
         // reasons, we must update the last use position to stop problems
         if(!mAUDIO_1_VOLUME && data)
      {
         mAUDIO_1_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
      }
         mAUDIO_1_VOLUME=(SBYTE)data;
         TRACE_MIKIE2("Poke(AUD1VOL,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD1SHFTFB&0xff):
         mAUDIO_1_WAVESHAPER&=0x001fff;
         mAUDIO_1_WAVESHAPER|=(ULONG)data<<13;
         TRACE_MIKIE2("Poke(AUD1SHFTFB,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD1OUTVAL&0xff):
         mAUDIO_OUTPUT_1=data;
         TRACE_MIKIE2("Poke(AUD1OUTVAL,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD1L8SHFT&0xff):
         mAUDIO_1_WAVESHAPER&=0x1fff00;
         mAUDIO_1_WAVESHAPER|=data;
         TRACE_MIKIE2("Poke(AUD1L8SHFT,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD1TBACK&0xff):
         // Counter is disabled when backup is zero for optimisation
         // due to the fact that the output frequency will be above audio
         // range, we must update the last use position to stop problems
         if(!mAUDIO_1_BKUP && data)
      {
         mAUDIO_1_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
      }
         mAUDIO_1_BKUP=data;
         TRACE_MIKIE2("Poke(AUD1TBACK,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD1CTL&0xff):
         mAUDIO_1_ENABLE_RELOAD=data&0x10;
         mAUDIO_1_ENABLE_COUNT=data&0x08;
         mAUDIO_1_LINKING=data&0x07;
         mAUDIO_1_INTEGRATE_ENABLE=data&0x20;
         if(data&0x40) mAUDIO_1_TIMER_DONE=0;
         mAUDIO_1_WAVESHAPER&=0x1fefff;
         mAUDIO_1_WAVESHAPER|=(data&0x80)?0x001000:0x000000;
         if(data&0x48)
         {
            mAUDIO_1_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
            SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         }
         TRACE_MIKIE2("Poke(AUD1CTL,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD1COUNT&0xff):
         mAUDIO_1_CURRENT=data;
         TRACE_MIKIE2("Poke(AUD1COUNT,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD1MISC&0xff):
         mAUDIO_1_WAVESHAPER&=0x1ff0ff;
         mAUDIO_1_WAVESHAPER|=(data&0xf0)<<4;
         mAUDIO_1_BORROW_IN=data&0x02;
         mAUDIO_1_BORROW_OUT=data&0x01;
         mAUDIO_1_LAST_CLOCK=data&0x04;
         TRACE_MIKIE2("Poke(AUD1MISC,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;

      case (AUD2VOL&0xff):
         // Counter is disabled when volume is zero for optimisation
         // reasons, we must update the last use position to stop problems
         if(!mAUDIO_2_VOLUME && data)
      {
         mAUDIO_2_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
      }
         mAUDIO_2_VOLUME=(SBYTE)data;
         TRACE_MIKIE2("Poke(AUD2VOL,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD2SHFTFB&0xff):
         mAUDIO_2_WAVESHAPER&=0x001fff;
         mAUDIO_2_WAVESHAPER|=(ULONG)data<<13;
         TRACE_MIKIE2("Poke(AUD2VSHFTFB,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD2OUTVAL&0xff):
         mAUDIO_OUTPUT_2=data;
         TRACE_MIKIE2("Poke(AUD2OUTVAL,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD2L8SHFT&0xff):
         mAUDIO_2_WAVESHAPER&=0x1fff00;
         mAUDIO_2_WAVESHAPER|=data;
         TRACE_MIKIE2("Poke(AUD2L8SHFT,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD2TBACK&0xff):
         // Counter is disabled when backup is zero for optimisation
         // due to the fact that the output frequency will be above audio
         // range, we must update the last use position to stop problems
         if(!mAUDIO_2_BKUP && data)
      {
         mAUDIO_2_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
      }
         mAUDIO_2_BKUP=data;
         TRACE_MIKIE2("Poke(AUD2TBACK,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD2CTL&0xff):
         mAUDIO_2_ENABLE_RELOAD=data&0x10;
         mAUDIO_2_ENABLE_COUNT=data&0x08;
         mAUDIO_2_LINKING=data&0x07;
         mAUDIO_2_INTEGRATE_ENABLE=data&0x20;
         if(data&0x40) mAUDIO_2_TIMER_DONE=0;
         mAUDIO_2_WAVESHAPER&=0x1fefff;
         mAUDIO_2_WAVESHAPER|=(data&0x80)?0x001000:0x000000;
         if(data&0x48)
         {
            mAUDIO_2_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
            SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         }
         TRACE_MIKIE2("Poke(AUD2CTL,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD2COUNT&0xff):
         mAUDIO_2_CURRENT=data;
         TRACE_MIKIE2("Poke(AUD2COUNT,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD2MISC&0xff):
         mAUDIO_2_WAVESHAPER&=0x1ff0ff;
         mAUDIO_2_WAVESHAPER|=(data&0xf0)<<4;
         mAUDIO_2_BORROW_IN=data&0x02;
         mAUDIO_2_BORROW_OUT=data&0x01;
         mAUDIO_2_LAST_CLOCK=data&0x04;
         TRACE_MIKIE2("Poke(AUD2MISC,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;

      case (AUD3VOL&0xff):
         // Counter is disabled when volume is zero for optimisation
         // reasons, we must update the last use position to stop problems
         if(!mAUDIO_3_VOLUME && data)
      {
         mAUDIO_3_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
      }
         mAUDIO_3_VOLUME=(SBYTE)data;
         TRACE_MIKIE2("Poke(AUD3VOL,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD3SHFTFB&0xff):
         mAUDIO_3_WAVESHAPER&=0x001fff;
         mAUDIO_3_WAVESHAPER|=(ULONG)data<<13;
         TRACE_MIKIE2("Poke(AUD3SHFTFB,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD3OUTVAL&0xff):
         mAUDIO_OUTPUT_3=data;
         TRACE_MIKIE2("Poke(AUD3OUTVAL,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD3L8SHFT&0xff):
         mAUDIO_3_WAVESHAPER&=0x1fff00;
         mAUDIO_3_WAVESHAPER|=data;
         TRACE_MIKIE2("Poke(AUD3L8SHFT,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD3TBACK&0xff):
         // Counter is disabled when backup is zero for optimisation
         // due to the fact that the output frequency will be above audio
         // range, we must update the last use position to stop problems
         if(!mAUDIO_3_BKUP && data)
      {
         mAUDIO_3_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
      }
         mAUDIO_3_BKUP=data;
         TRACE_MIKIE2("Poke(AUD3TBACK,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD3CTL&0xff):
         mAUDIO_3_ENABLE_RELOAD=data&0x10;
         mAUDIO_3_ENABLE_COUNT=data&0x08;
         mAUDIO_3_LINKING=data&0x07;
         mAUDIO_3_INTEGRATE_ENABLE=data&0x20;
         if(data&0x40) mAUDIO_3_TIMER_DONE=0;
         mAUDIO_3_WAVESHAPER&=0x1fefff;
         mAUDIO_3_WAVESHAPER|=(data&0x80)?0x001000:0x000000;
         if(data&0x48)
         {
            mAUDIO_3_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
            SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         }
         TRACE_MIKIE2("Poke(AUD3CTL,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD3COUNT&0xff):
         mAUDIO_3_CURRENT=data;
         TRACE_MIKIE2("Poke(AUD3COUNT,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (AUD3MISC&0xff):
         mAUDIO_3_WAVESHAPER&=0x1ff0ff;
         mAUDIO_3_WAVESHAPER|=(data&0xf0)<<4;
         mAUDIO_3_BORROW_IN=data&0x02;
         mAUDIO_3_BORROW_OUT=data&0x01;
         mAUDIO_3_LAST_CLOCK=data&0x04;
         TRACE_MIKIE2("Poke(AUD3MISC,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;

      case (ATTEN_A&0xff):
            mAUDIO_ATTEN_0 = data;
            TRACE_MIKIE2("Poke(ATTEN_A ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
            break;
      case (ATTEN_B&0xff):
            mAUDIO_ATTEN_1 = data;
            TRACE_MIKIE2("Poke(ATTEN_B ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
            break;
      case (ATTEN_C&0xff):
            mAUDIO_ATTEN_2 = data;
            TRACE_MIKIE2("Poke(ATTEN_C ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
            break;
      case (ATTEN_D&0xff):
            mAUDIO_ATTEN_3 = data;
            TRACE_MIKIE2("Poke(ATTEN_D ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
            break;
      case (MPAN&0xff):
            TRACE_MIKIE2("Poke(MPAN ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
            mPAN = data;
         break;

      case (MSTEREO&0xff):
            TRACE_MIKIE2("Poke(MSTEREO,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
            mSTEREO=data;
         //         if(!(mSTEREO&0x11) && (data&0x11))
         //         {
         //             mAUDIO_0_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
         //             SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         //         }
         //         if(!(mSTEREO&0x22) && (data&0x22))
         //         {
         //             mAUDIO_1_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
         //             SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         //         }
         //         if(!(mSTEREO&0x44) && (data&0x44))
         //         {
         //             mAUDIO_2_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
         //             SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         //         }
         //         if(!(mSTEREO&0x88) && (data&0x88))
         //         {
         //             mAUDIO_3_LAST_COUNT=SYSTEM_VAR(gSystemCycleCount);
         //             SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         //         }
         break;

      case (INTRST&0xff):
         data^=0xff;
         mTimerStatusFlags&=data;
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         // 22/09/06 Fix to championship rally, IRQ not getting cleared with edge triggered system
         SYSTEM_VAR(gSystemIRQ)=(mTimerStatusFlags&mTimerInterruptMask)?TRUE:FALSE;
         // 22/09/06 Fix to championship rally, IRQ not getting cleared with edge triggered system
         TRACE_MIKIE2("Poke(INTRST  ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;

      case (INTSET&0xff):
         TRACE_MIKIE2("Poke(INTSET  ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         mTimerStatusFlags|=data;
         // 22/09/06 Fix to championship rally, IRQ not getting cleared with edge triggered system
         SYSTEM_VAR(gSystemIRQ)=(mTimerStatusFlags&mTimerInterruptMask)?TRUE:FALSE;
         // 22/09/06 Fix to championship rally, IRQ not getting cleared with edge triggered system
         SYSTEM_VAR(gNextTimerEvent)=SYSTEM_VAR(gSystemCycleCount);
         break;

      case (SYSCTL1&0xff):
         TRACE_MIKIE2("Poke(SYSCTL1 ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         if(!(data&0x02))
         {
            Error();
         }
         mSystem.CartAddressStrobe((data&0x01)?TRUE:FALSE);
         if(mSystem.mEEPROM->Available()) mSystem.mEEPROM->ProcessEepromCounter(mSystem.mCart->GetCounterValue());
         break;

      case (MIKEYSREV&0xff):
         TRACE_MIKIE2("Poke(MIKEYSREV,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;

      case (IODIR&0xff):
         TRACE_MIKIE2("Poke(IODIR   ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         mIODIR=data;
         if(mSystem.mEEPROM->Available()) mSystem.mEEPROM->ProcessEepromIO(mIODIR,mIODAT);
         break;

      case (IODAT&0xff):
         TRACE_MIKIE2("Poke(IODAT   ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         mIODAT=data;
         mSystem.CartAddressData((mIODAT&0x02)?TRUE:FALSE);
         // Enable cart writes to BANK1 on AUDIN if AUDIN is set to output
         if(mIODIR&0x10) mSystem.mCart->mWriteEnableBank1=(mIODAT&0x10)?TRUE:FALSE;// there is no reason to use AUDIN as Write Enable or latch. private patch??? TODO
         if(mSystem.mEEPROM->Available()) mSystem.mEEPROM->ProcessEepromIO(mIODIR,mIODAT);
         break;

      case (SERCTL&0xff):
         TRACE_MIKIE2("Poke(SERCTL  ,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         mUART_TX_IRQ_ENABLE=(data&0x80)?true:false;
         mUART_RX_IRQ_ENABLE=(data&0x40)?true:false;
         mUART_PARITY_ENABLE=(data&0x10)?true:false;
         mUART_SENDBREAK=data&0x02;
         mUART_PARITY_EVEN=data&0x01;

         // Reset all errors if required
         if(data&0x08)
         {
            mUART_Rx_overun_error=0;
            mUART_Rx_framing_error=0;
         }

         if(mUART_SENDBREAK)
         {
            // Trigger send break, it will self sustain as long as sendbreak is set
            mUART_TX_COUNTDOWN=UART_TX_TIME_PERIOD;
            // Loop back what we transmitted
            ComLynxTxLoopback(UART_BREAK_CODE);
         }
         break;

      case (SERDAT&0xff):
         TRACE_MIKIE2("Poke(SERDAT ,%04x) at PC=%04x",data,mSystem.mCpu->GetPC());
         //
         // Fake transmission, set counter to be decremented by Timer 4
         //
         // ComLynx only has one output pin, hence Rx & Tx are shorted
         // therefore any transmitted data will loopback
         //
         mUART_TX_DATA=data;
         // Calculate Parity data
         if(mUART_PARITY_ENABLE)
         {
            // Calc parity value
            // Leave at zero !!
         }
         else
         {
            // If disabled then the PAREVEN bit is sent
            if(mUART_PARITY_EVEN) data|=0x0100;
         }
         // Set countdown to transmission
         mUART_TX_COUNTDOWN=UART_TX_TIME_PERIOD;
         // Loop back what we transmitted
         ComLynxTxLoopback(mUART_TX_DATA);
         break;

      case (SDONEACK&0xff):
         TRACE_MIKIE2("Poke(SDONEACK,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;
      case (CPUSLEEP&0xff):
         TRACE_MIKIE2("Poke(CPUSLEEP,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         //
         // We must do "cycles_used" cycles of the system with the CPU sleeping
         // to compensate for the sprite painting, Mikie update will autowake the
         // CPU at the right time.
         //
         {
            TRACE_MIKIE0("*********************************************************");
            TRACE_MIKIE0("****               CPU SLEEP STARTED                 ****");
            TRACE_MIKIE0("*********************************************************");
            ULONG cycles_used=mSusie->PaintSprites();
            SYSTEM_VAR(gCPUWakeupTime)=SYSTEM_VAR(gSystemCycleCount)+cycles_used;
            SetCPUSleep();
            TRACE_MIKIE2("Poke(CPUSLEEP,%02x) wakeup at cycle =%012d",data,SYSTEM_VAR(gCPUWakeupTime));
         }
         break;

      case (DISPCTL&0xff):
         TRACE_MIKIE2("Poke(DISPCTL,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         {
            TDISPCTL tmp;
            tmp.Byte=data;
            mDISPCTL_DMAEnable=tmp.Bits.DMAEnable;
            mDISPCTL_Flip=tmp.Bits.Flip;
            mDISPCTL_FourColour=tmp.Bits.FourColour;
            mDISPCTL_Colour=tmp.Bits.Colour;
         }
         break;
      case (PBKUP&0xff):
         TRACE_MIKIE2("Poke(PBKUP,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;

      case (DISPADRL&0xff):
         TRACE_MIKIE2("Poke(DISPADRL,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         mDisplayAddress&=0xff00;
         mDisplayAddress+=data;
         break;

      case (DISPADRH&0xff):
         TRACE_MIKIE2("Poke(DISPADRH,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         mDisplayAddress&=0x00ff;
         mDisplayAddress+=(data<<8);
         break;

      case (Mtest0&0xff):
      case (Mtest1&0xff):
         // Test registers are unimplemented
         // lets hope no programs use them.
         TRACE_MIKIE2("Poke(MTEST0/1,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
      case (Mtest2&0xff):
         // Test registers are unimplemented
         // lets hope no programs use them.
         gError->Warning("CMikie::Poke() - Write to MTEST2");
         TRACE_MIKIE2("Poke(MTEST2,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         break;

      case (0xfd97&0xff):{
         // This code is to intercept calls to the fake ROM
         int mPC = mSystem.mCpu->GetPC();
         if(mPC>=0xFE00 && mPC<0xFFF0){
            switch(mPC){
               case 0xFE00+3:
                  mSystem.HLE_BIOS_FE00();
                  break;
               case 0xFE19+3:
                  mSystem.HLE_BIOS_FE19();
                  break;
               case 0xFE4A+3:
                  mSystem.HLE_BIOS_FE4A();
                  break;
               case 0xFF80+3:
                  mSystem.HLE_BIOS_FF80();
                  break;
               default:
                  printf("ROM code missing...\n");
                  break;
               }
            }
         }; break;
      case (GREEN0&0xff):
      case (GREEN1&0xff):
      case (GREEN2&0xff):
      case (GREEN3&0xff):
      case (GREEN4&0xff):
      case (GREEN5&0xff):
      case (GREEN6&0xff):
      case (GREEN7&0xff):
      case (GREEN8&0xff):
      case (GREEN9&0xff):
      case (GREENA&0xff):
      case (GREENB&0xff):
      case (GREENC&0xff):
      case (GREEND&0xff):
      case (GREENE&0xff):
      case (GREENF&0xff):
         TRACE_MIKIE2("Poke(GREENPAL0-F,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         mPalette[addr&0x0f].Colours.Green=data&0x0f;
         break;

      case (BLUERED0&0xff):
      case (BLUERED1&0xff):
      case (BLUERED2&0xff):
      case (BLUERED3&0xff):
      case (BLUERED4&0xff):
      case (BLUERED5&0xff):
      case (BLUERED6&0xff):
      case (BLUERED7&0xff):
      case (BLUERED8&0xff):
      case (BLUERED9&0xff):
      case (BLUEREDA&0xff):
      case (BLUEREDB&0xff):
      case (BLUEREDC&0xff):
      case (BLUEREDD&0xff):
      case (BLUEREDE&0xff):
      case (BLUEREDF&0xff):
         TRACE_MIKIE2("Poke(BLUEREDPAL0-F,%02x) at PC=%04x",data,mSystem.mCpu->GetPC());
         mPalette[addr&0x0f].Colours.Blue=(data&0xf0)>>4;
         mPalette[addr&0x0f].Colours.Red=data&0x0f;
         break;

         // Errors on read only register accesses

      case (MAGRDY0&0xff):
      case (MAGRDY1&0xff):
      case (AUDIN&0xff):
      case (MIKEYHREV&0xff):
         TRACE_MIKIE3("Poke(%04x,%02x) - Poke to read only register location at PC=%04x",addr,data,mSystem.mCpu->GetPC());
         break;

         // Errors on illegal location accesses

      default:
         TRACE_MIKIE3("Poke(%04x,%02x) - Poke to illegal location at PC=%04x",addr,data,mSystem.mCpu->GetPC());
         break;
   }
