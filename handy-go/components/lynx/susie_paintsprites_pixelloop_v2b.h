// hsign<0
    // Now render an individual destination line
    while(true)
    {
            if(!mLineRepeatCount)
           {
              // Normal sprites fetch their counts on a packet basis
              if(mLineType!=line_abs_literal)
              {
#ifdef SUSIE_INLINE_LineGetBits_V2
    ULONG literal;
    MY_GET_BITS(literal,1)
#else
                 ULONG literal=LineGetBits(1);
#endif
                 if(literal) mLineType=line_literal; else mLineType=line_packed;
              }
        
              // Pixel store is empty what should we do
              switch(mLineType)
              {
                 case line_abs_literal:
                    // This means end of line for us
                    mLinePixel=LINE_END;
                    goto EndWhile;
                 case line_literal:
#ifdef SUSIE_INLINE_LineGetBits_V2
    MY_GET_BITS(mLineRepeatCount,4)
#else                 
                    mLineRepeatCount=LineGetBits(4);
#endif
                    mLineRepeatCount++;
                    break;
                 case line_packed:
                    //
                    // From reading in between the lines only a packed line with
                    // a zero size i.e 0b00000 as a header is allowable as a packet end
                    //
#ifdef SUSIE_INLINE_LineGetBits_V2
    MY_GET_BITS(mLineRepeatCount,4)
#else
                    mLineRepeatCount=LineGetBits(4);
#endif
                    if(!mLineRepeatCount)
                    {
                       mLinePixel=LINE_END;
                       mLineRepeatCount++;
                       goto EndWhile;
                    }
#ifdef SUSIE_INLINE_LineGetBits_V2
else
{
    ULONG tmp;
    MY_GET_BITS(tmp,mSPRCTL0_PixelBits)
    pixel=mPenIndex[tmp];
}
#else
                    else
                       pixel=mPenIndex[LineGetBits(mSPRCTL0_PixelBits)];
#endif
                    mLineRepeatCount++;
                    break;
                 default:
                    pixel = 0;
                    goto LoopContinue;
              }
        
           }
        /*
           if(pixel==LINE_END)
           {
                printf("ERROR!\n");
                goto EndWhile;
           }
         */
              mLineRepeatCount--;
        
              switch(mLineType)
              {
                 case line_abs_literal:
#ifdef SUSIE_INLINE_LineGetBits_V2
    MY_GET_BITS(pixel,mSPRCTL0_PixelBits)
#else
                    pixel=LineGetBits(mSPRCTL0_PixelBits);
#endif
                    // Check the special case of a zero in the last pixel
                    if(!mLineRepeatCount && !pixel)
                    {
                       mLinePixel=LINE_END;
                       goto EndWhile;
                    }
                    else
                       pixel=mPenIndex[pixel];
                    break;
                 case line_literal:
#ifdef SUSIE_INLINE_LineGetBits_V2
    ULONG tmp;
    MY_GET_BITS(tmp,mSPRCTL0_PixelBits)
    pixel=mPenIndex[tmp];
#else
                    pixel=mPenIndex[LineGetBits(mSPRCTL0_PixelBits)];
#endif
                    break;
                 case line_packed:
                    break;
                 default:
                    pixel=0;
                    goto LoopContinue;
              }
           
    
    LoopContinue:;
       
       // This is allowed to update every pixel
       mHSIZACUM.Word+=mSPRHSIZ.Word;
       pixel_width=mHSIZACUM.Byte.High;
       mHSIZACUM.Byte.High=0;

       for(int hloop=0;hloop<pixel_width;hloop++)
       {
          // Draw if onscreen but break loop on transition to offscreen
          if(hoff>=0 && hoff<SCREEN_WIDTH)
          {
             //ProcessPixel(hoff,pixel);
             PROCESS_PIXEL
             onscreen=everonscreen=TRUE;
          }
          else
          {
             if(onscreen) break;
          }
          hoff--;
       }
    }
mLinePixel = pixel;
EndWhile:;
