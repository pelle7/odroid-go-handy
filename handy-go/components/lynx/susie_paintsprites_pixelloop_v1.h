    ULONG pixel;
    // Now render an individual destination line
    while((pixel=LineGetPixel())!=LINE_END)
    {
       // This is allowed to update every pixel
       mHSIZACUM.Word+=mSPRHSIZ.Word;
       pixel_width=mHSIZACUM.Byte.High;
       mHSIZACUM.Byte.High=0;

       for(int hloop=0;hloop<pixel_width;hloop++)
       {
          // Draw if onscreen but break loop on transition to offscreen
          if(hoff>=0 && hoff<SCREEN_WIDTH)
          {
             ProcessPixel(hoff,pixel);
             onscreen=everonscreen=TRUE;
          }
          else
          {
             if(onscreen) break;
          }
          hoff+=hsign;
       }
    }
