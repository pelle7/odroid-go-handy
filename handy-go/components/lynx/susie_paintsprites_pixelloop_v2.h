ULONG pixel = mLinePixel; // Much faster
if (hsign > 0)
{
switch(mSPRCTL0_Type)
{
      case sprite_background_shadow:
         #define PROCESS_PIXEL \
         WritePixel(hoff,pixel); \
         if(!mSPRCOLL_Collide && !mSPRSYS_NoCollide && pixel!=0x0e) \
         { \
            WriteCollision(hoff,mSPRCOLL_Number); \
         }
         #define EndWhile EndWhile01a
         #define LoopContinue LoopContinue01a
         
         #include "susie_paintsprites_pixelloop_v2a.h"
         break;
      case sprite_background_noncollide:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL WritePixel(hoff,pixel);
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile02a
         #define LoopContinue LoopContinue02a
         #include "susie_paintsprites_pixelloop_v2a.h"
         break;
      case sprite_noncollide:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL if(pixel!=0x00) WritePixel(hoff,pixel);
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile03a
         #define LoopContinue LoopContinue03a
         #include "susie_paintsprites_pixelloop_v2a.h"
         break;
      case sprite_boundary:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL \
         if(pixel!=0x00 && pixel!=0x0f) \
         { \
            WritePixel(hoff,pixel); \
         } \
         if(pixel!=0x00) \
         { \
            if(!mSPRCOLL_Collide && !mSPRSYS_NoCollide) \
            { \
               ULONG collision=ReadCollision(hoff); \
               if(collision>mCollision) \
               { \
                  mCollision=collision; \
               } \
               { \
                  WriteCollision(hoff,mSPRCOLL_Number); \
               } \
            } \
         }
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile04a
         #define LoopContinue LoopContinue04a
         #include "susie_paintsprites_pixelloop_v2a.h"
         break;
      case sprite_normal:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL \
         if(pixel!=0x00) \
         { \
            WritePixel(hoff,pixel); \
            if(!mSPRCOLL_Collide && !mSPRSYS_NoCollide) \
            { \
               ULONG collision=ReadCollision(hoff); \
               if(collision>mCollision) \
               { \
                  mCollision=collision; \
               } \
               { \
                  WriteCollision(hoff,mSPRCOLL_Number); \
               } \
            } \
         }
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile05a
         #define LoopContinue LoopContinue05a
         #include "susie_paintsprites_pixelloop_v2a.h"
         break;
      case sprite_boundary_shadow:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL \
         if(pixel!=0x00 && pixel!=0x0e && pixel!=0x0f) \
         { \
            WritePixel(hoff,pixel); \
         } \
         if(pixel!=0x00 && pixel!=0x0e) \
         { \
            if(!mSPRCOLL_Collide && !mSPRSYS_NoCollide) \
            { \
               ULONG collision=ReadCollision(hoff); \
               if(collision>mCollision) \
               { \
                  mCollision=collision; \
               } \
               { \
                  WriteCollision(hoff,mSPRCOLL_Number); \
               } \
            } \
         }
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile06a
         #define LoopContinue LoopContinue06a
         #include "susie_paintsprites_pixelloop_v2a.h"
         break;
      case sprite_shadow:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL \
         if(pixel!=0x00) \
         { \
            WritePixel(hoff,pixel); \
         } \
         if(pixel!=0x00 && pixel!=0x0e) \
         { \
            if(!mSPRCOLL_Collide && !mSPRSYS_NoCollide) \
            { \
               ULONG collision=ReadCollision(hoff); \
               if(collision>mCollision) \
               { \
                  mCollision=collision; \
               } \
               { \
                  WriteCollision(hoff,mSPRCOLL_Number); \
               } \
            } \
         }
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile07a
         #define LoopContinue LoopContinue07a
         #include "susie_paintsprites_pixelloop_v2a.h"
         break;
      case sprite_xor_shadow:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL \
         if(pixel!=0x00) \
         { \
            WritePixel(hoff,ReadPixel(hoff)^pixel); \
         } \
         if(pixel!=0x00 && pixel!=0x0e) \
         { \
            if(!mSPRCOLL_Collide && !mSPRSYS_NoCollide && pixel!=0x0e) \
            { \
               ULONG collision=ReadCollision(hoff); \
               if(collision>mCollision) \
               { \
                  mCollision=collision; \
               } \
               { \
                  WriteCollision(hoff,mSPRCOLL_Number); \
               } \
            } \
         }
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile08a
         #define LoopContinue LoopContinue08a
         #include "susie_paintsprites_pixelloop_v2a.h"
         break;
      default:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile09a
         #define LoopContinue LoopContinue09a
         #include "susie_paintsprites_pixelloop_v2a.h"
         break;
}
}
else
{
switch(mSPRCTL0_Type)
{
      case sprite_background_shadow:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL \
         WritePixel(hoff,pixel); \
         if(!mSPRCOLL_Collide && !mSPRSYS_NoCollide && pixel!=0x0e) \
         { \
            WriteCollision(hoff,mSPRCOLL_Number); \
         }
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile01b
         #define LoopContinue LoopContinue01b
         #include "susie_paintsprites_pixelloop_v2b.h"
         break;
      case sprite_background_noncollide:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL WritePixel(hoff,pixel);
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile02b
         #define LoopContinue LoopContinue02b
         #include "susie_paintsprites_pixelloop_v2b.h"
         break;
      case sprite_noncollide:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL if(pixel!=0x00) WritePixel(hoff,pixel);
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile03b
         #define LoopContinue LoopContinue03b
         #include "susie_paintsprites_pixelloop_v2b.h"
         break;
      case sprite_boundary:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL \
         if(pixel!=0x00 && pixel!=0x0f) \
         { \
            WritePixel(hoff,pixel); \
         } \
         if(pixel!=0x00) \
         { \
            if(!mSPRCOLL_Collide && !mSPRSYS_NoCollide) \
            { \
               ULONG collision=ReadCollision(hoff); \
               if(collision>mCollision) \
               { \
                  mCollision=collision; \
               } \
               { \
                  WriteCollision(hoff,mSPRCOLL_Number); \
               } \
            } \
         }
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile04b
         #define LoopContinue LoopContinue04b
         #include "susie_paintsprites_pixelloop_v2b.h"
         break;
      case sprite_normal:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL \
         if(pixel!=0x00) \
         { \
            WritePixel(hoff,pixel); \
            if(!mSPRCOLL_Collide && !mSPRSYS_NoCollide) \
            { \
               ULONG collision=ReadCollision(hoff); \
               if(collision>mCollision) \
               { \
                  mCollision=collision; \
               } \
               { \
                  WriteCollision(hoff,mSPRCOLL_Number); \
               } \
            } \
         }
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile05b
         #define LoopContinue LoopContinue05b
         #include "susie_paintsprites_pixelloop_v2b.h"
         break;
      case sprite_boundary_shadow:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL \
         if(pixel!=0x00 && pixel!=0x0e && pixel!=0x0f) \
         { \
            WritePixel(hoff,pixel); \
         } \
         if(pixel!=0x00 && pixel!=0x0e) \
         { \
            if(!mSPRCOLL_Collide && !mSPRSYS_NoCollide) \
            { \
               ULONG collision=ReadCollision(hoff); \
               if(collision>mCollision) \
               { \
                  mCollision=collision; \
               } \
               { \
                  WriteCollision(hoff,mSPRCOLL_Number); \
               } \
            } \
         }
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile06b
         #define LoopContinue LoopContinue06b
         #include "susie_paintsprites_pixelloop_v2b.h"
         break;
      case sprite_shadow:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL \
         if(pixel!=0x00) \
         { \
            WritePixel(hoff,pixel); \
         } \
         if(pixel!=0x00 && pixel!=0x0e) \
         { \
            if(!mSPRCOLL_Collide && !mSPRSYS_NoCollide) \
            { \
               ULONG collision=ReadCollision(hoff); \
               if(collision>mCollision) \
               { \
                  mCollision=collision; \
               } \
               { \
                  WriteCollision(hoff,mSPRCOLL_Number); \
               } \
            } \
         }
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile07b
         #define LoopContinue LoopContinue07b
         #include "susie_paintsprites_pixelloop_v2b.h"
         break;
      case sprite_xor_shadow:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL \
         if(pixel!=0x00) \
         { \
            WritePixel(hoff,ReadPixel(hoff)^pixel); \
         } \
         if(pixel!=0x00 && pixel!=0x0e) \
         { \
            if(!mSPRCOLL_Collide && !mSPRSYS_NoCollide && pixel!=0x0e) \
            { \
               ULONG collision=ReadCollision(hoff); \
               if(collision>mCollision) \
               { \
                  mCollision=collision; \
               } \
               { \
                  WriteCollision(hoff,mSPRCOLL_Number); \
               } \
            } \
         }
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile08b
         #define LoopContinue LoopContinue08b
         #include "susie_paintsprites_pixelloop_v2b.h"
         break;
      default:
         #undef PROCESS_PIXEL
         #define PROCESS_PIXEL
         
         #undef EndWhile
         #undef LoopContinue
         #define EndWhile EndWhile09b
         #define LoopContinue LoopContinue09b
         #include "susie_paintsprites_pixelloop_v2b.h"
         break;
}
}
