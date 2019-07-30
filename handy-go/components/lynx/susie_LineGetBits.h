#ifdef SUSIE_INLINE_LineGetBits_V2
    {
#else
   ULONG retval;
   #define SUSIE_INLINE_LineGetBits_RC retval 
#endif

ODROID_DEBUG_PERF_START()

   // Sanity, not really needed
   // if(bits>32) return 0;

   // Only return data IF there is enought bits left in the packet

   if(mLinePacketBitsLeft<=bits) return 0;// <= fixes issues with polygones e.g. "demo006"

   // Make sure shift reg has enough bits to fulfil the request

   if(mLineShiftRegCount<bits)
   {
      // This assumes data comes into LSB and out of MSB
      //        mLineShiftReg&=0x000000ff;  // Has no effect
      mLineShiftReg<<=24;
      mLineShiftReg|=RAM_PEEK(mTMPADR.Word++)<<16;
      mLineShiftReg|=RAM_PEEK(mTMPADR.Word++)<<8;
      mLineShiftReg|=RAM_PEEK(mTMPADR.Word++);

      mLineShiftRegCount+=24;

      // Increment cycle count for the read
      cycles_used+=3*SPR_RDWR_CYC;
   }

   // Extract the return value
   SUSIE_INLINE_LineGetBits_RC=mLineShiftReg>>(mLineShiftRegCount-bits);
   SUSIE_INLINE_LineGetBits_RC&=(1<<bits)-1;

   // Update internal vars;
   mLineShiftRegCount-=bits;
   mLinePacketBitsLeft-=bits;
ODROID_DEBUG_PERF_INCR(ODROID_DEBUG_PERF_SUSIE_LINEGETBITS)
#ifdef SUSIE_INLINE_LineGetBits_V2
    }
#else
   return retval;
#endif