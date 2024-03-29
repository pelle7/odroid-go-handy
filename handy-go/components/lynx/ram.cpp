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
// RAM emulation class                                                      //
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// This class emulates the system RAM (64KB), the interface is pretty       //
// simple: constructor, reset, peek, poke.                                  //
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


#define RAM_CPP

//#include <crtdbg.h>
//#define   TRACE_RAM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"
#include "ram.h"

CRam::CRam(UBYTE *filememory,ULONG filesize)
{
   HOME_HEADER	header;

   // Take a copy into the backup buffer for restore on reset
   mFileSize=filesize;
   printf("CRam::CRam\n");
#ifdef MY_RAM_IN_32BIT
   printf("CRam::CRam: Alloc\n");
   ULONG *tmp = MY_MEM_ALLOC_FAST_EXT(ULONG, RAM_SIZE, 4);
   printf("CRam::CRam: Alloc: %p\n", tmp);
   mRamData = (UBYTE*)tmp;
   // 8bit access crashes on mRamData
   for (int i = 0;i < RAM_SIZE; i++) {
    mRamData[i] = 0;
   }
#else
   mRamData = MY_MEM_ALLOC_FAST_EXT(UBYTE, RAM_SIZE, 1);
   for (int i = 0;i < RAM_SIZE; i++) {
    mRamData[i] = 0;
   }
#endif
   

   if(filesize)
   {
      // Take a copy of the ram data
      mFileData = MY_MEM_NEW_SLOW(UBYTE, mFileSize);
      memcpy(mFileData,filememory,mFileSize);

      // Sanity checks on the header
      memcpy(&header,mFileData,sizeof(HOME_HEADER));

      if(header.magic[0]!='B' || header.magic[1]!='S' || header.magic[2]!='9' || header.magic[3]!='3')
      {
         fprintf(stderr, "Invalid cart.\n");
      }
   }
   else
   {
      filememory=NULL;
   }
   // Reset will cause the loadup

   //Reset(); // Moved to system.cpp
}

CRam::~CRam()
{
   if(mFileSize)
   {
      MY_MEM_NEW_FREE(mFileData);
      mFileData=NULL;
   }
   MY_MEM_ALLOC_FREE(mRamData);
}

void CRam::Reset(void)
{
   // Open up the file

   if(mFileSize >= sizeof(HOME_HEADER))
   {
      HOME_HEADER	header;
      UBYTE tmp;
      int data_size;

      // Reverse the bytes in the header words
      memcpy(&header,mFileData,sizeof(HOME_HEADER));
      tmp=(header.load_address&0xff00)>>8;
      header.load_address=(header.load_address<<8)+tmp;
      tmp=(header.size&0xff00)>>8;
      header.size=(header.size<<8)+tmp;

      // Now we can safely read/manipulate the data
      header.load_address-=10;

      data_size = __min(int(header.size), (int)(mFileSize));
      memset(mRamData, 0x00, header.load_address);
      memcpy(mRamData+header.load_address, mFileData, data_size);
      memset(mRamData+header.load_address+data_size, 0x00, RAM_SIZE-header.load_address-data_size);
      SYSTEM_VAR(gCPUBootAddress)=header.load_address;
   } else {
      memset(mRamData, DEFAULT_RAM_CONTENTS, RAM_SIZE);
   }
}

void CRam::Clear(void)
{
   memset(mRamData, 0, RAM_SIZE);
}

bool CRam::ContextSave(FILE *fp)
{
   if(!fprintf(fp,"CRam::ContextSave")) return 0;
   if(!fwrite(mRamData,sizeof(UBYTE),RAM_SIZE,fp)) return 0;
   return 1;
}

bool CRam::ContextLoad(LSS_FILE *fp)
{
   char teststr[100]="XXXXXXXXXXXXXXXXX";
   if(!lss_read(teststr,sizeof(char),17,fp)) return 0;
   if(strcmp(teststr,"CRam::ContextSave")!=0) return 0;

   if(!lss_read(mRamData,sizeof(UBYTE),RAM_SIZE,fp)) return 0;
   mFileSize=0;
   return 1;
}

//END OF FILE
