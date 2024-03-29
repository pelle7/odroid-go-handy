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
// System object class                                                      //
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// This class provides the glue to bind of of the emulation objects         //
// together via peek/poke handlers and pass thru interfaces to lower        //
// objects, all control of the emulator is done via this class. Update()    //
// does most of the work and each call emulates one CPU instruction and     //
// updates all of the relevant hardware if required. It must be remembered  //
// that if that instruction involves setting SPRGO then, it will cause a    //
// sprite painting operation and then a corresponding update of all of the  //
// hardware which will usually involve recursive calls to Update, see       //
// Mikey SPRGO code for more details.                                       //
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

#define SYSTEM_CPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"

extern void lynx_decrypt(unsigned char * result, const unsigned char * encrypted, const int length);

int lss_read_ulong(ULONG* dest, int varsize, int varcount,LSS_FILE *fp)
{
    ULONG copysize;
    ULONG rc;
   copysize=varsize*varcount;
   if((fp->index + copysize) > fp->index_limit) copysize=fp->index_limit - fp->index;
   memcpy(&rc,fp->memptr+fp->index,copysize);
   fp->index+=copysize;
   *dest = rc;
   return copysize;
}

int lss_read(void* dest,int varsize, int varcount,LSS_FILE *fp)
{
   ULONG copysize;
   copysize=varsize*varcount;
   if((fp->index + copysize) > fp->index_limit) copysize=fp->index_limit - fp->index;
   memcpy(dest,fp->memptr+fp->index,copysize);
   fp->index+=copysize;
   return copysize;
}

void _splitpath(const char* path, char* drv, char* dir, char* name, char* ext)
{
   const char* end; /* end of processed string */
   const char* p;   /* search pointer */
   const char* s;   /* copy pointer */

   /* extract drive name */
   if (path[0] && path[1]==':') {
      if (drv) {
         *drv++ = *path++;
         *drv++ = *path++;
         *drv = '\0';
      }
   } else if (drv)
      *drv = '\0';

   /* search for end of string or stream separator */
   for(end=path; *end && *end!=':'; )
      end++;

   /* search for begin of file extension */
   for(p=end; p>path && *--p!='\\' && *p!='/'; )
      if (*p == '.') {
         end = p;
         break;
      }

   if (ext)
      for(s=end; (*ext=*s++); )
         ext++;

   /* search for end of directory name */
   for(p=end; p>path; )
      if (*--p=='\\' || *p=='/') {
         p++;
         break;
      }

   if (name) {
      for(s=p; s<end; )
         *name++ = *s++;

      *name = '\0';
   }

   if (dir) {
      for(s=path; s<p; )
         *dir++ = *s++;

      *dir = '\0';
   }
}

   CSystem::CSystem(const char* gamefile, const char* romfile, bool useEmu)
:mCart(NULL),
   mRom(NULL),
   mMemMap(NULL),
   mRam(NULL),
   mCpu(NULL),
   mMikie(NULL),
    mSusie(NULL),
    mEEPROM(NULL)
{
   printf("CSystem: 001\n");
#ifdef MY_GLOBAL_SYSTEM_VARS_CPU
   //gSystemVars = MY_MEM_ALLOC_FAST_EXT(systemvars, sizeof(systemvars)/4, 4);
   gSystemVars = MY_MEM_ALLOC_FAST_EXT(systemvars, sizeof(systemvars), 1);
#endif
   gAudioEnabledPointer = &SYSTEM_VAR(gAudioEnabled);
   SYSTEM_VAR(gAudioBuffer) = MY_MEM_ALLOC_FAST_EXT(UBYTE, HANDY_AUDIO_BUFFER_SIZE, 1);

#ifdef _LYNXDBG
   mpDebugCallback=NULL;
   mDebugCallbackObject=0;
#endif

   // Select the default filetype
   UBYTE *filememory=NULL;
   UBYTE *howardmemory=NULL;
   ULONG filesize=0;
   ULONG howardsize=0;
   /*
#ifndef MY_MEM_MODE_V2
#ifdef MY_MEM_MODE
    mMemoryHandlers = (CLynxBase**)malloc(sizeof(CLynxBase*) * (SYSTEM_SIZE - MY_MEM_START));
#else
   mMemoryHandlers = (CLynxBase**)malloc(sizeof(CLynxBase*) * SYSTEM_SIZE);
#endif
   if (!mMemoryHandlers) {
      printf("OUT OF MEMORY (1)!\n");
      return;
   }
#endif
   */
   printf("CSystem: 002\n");
   
   mFileType=HANDY_FILETYPE_ILLEGAL;
   if(strcmp(gamefile,"")==0)
   {
      // No file
      filesize=0;
      filememory=NULL;
   }
   else
   {
      // Open the file and load the file
      FILE	*fp;

      // Open the cartridge file for reading
      if((fp=fopen(gamefile,"rb"))==NULL)
      {
         fprintf(stderr, "Invalid Cart.\n");
      }

      // How big is the file ??
      fseek(fp,0,SEEK_END);
      filesize=ftell(fp);
      fseek(fp,0,SEEK_SET);
      filememory=MY_MEM_NEW_SLOW(UBYTE, filesize);
      if (!filememory) {
         printf("OUT OF MEMORY (2)!\n");
         return;
      }

      if(fread(filememory,sizeof(char),filesize,fp)!=filesize)
      {
         fprintf(stderr, "Invalid Cart (filesize).\n");
         MY_MEM_ALLOC_FREE(filememory);
      }

      fclose(fp);
   }

   // Now try and determine the filetype we have opened
   if(filesize)
   {
      char clip[11];
      memcpy(clip,filememory,11);
      clip[4]=0;
      clip[10]=0;

      if(!strcmp(&clip[6],"BS93")) mFileType=HANDY_FILETYPE_HOMEBREW;
      else if(!strcmp(&clip[0],"LYNX")) mFileType=HANDY_FILETYPE_LNX;
      else if(!strcmp(&clip[0],LSS_VERSION_OLD)) mFileType=HANDY_FILETYPE_SNAPSHOT;
      else if(filesize==128*1024 || filesize==256*1024 || filesize==512*1024)
      {
         fprintf(stderr, "Invalid Cart (type). but 128/256/512k size -> set to RAW and try to load raw rom image\n");
         mFileType=HANDY_FILETYPE_RAW;
         //MY_MEM_ALLOC_FREE(filememory);// WHY????? -> crash!
      }else{
         fprintf(stderr, "Invalid Cart (type). -> set to RAW and try to load raw rom image\n");
      }
   }

   mCycleCountBreakpoint=0xffffffff;

   // Create the system objects that we'll use

   // Attempt to load the cartridge errors caught above here...

   mRom = new CRom(romfile,useEmu);

   // An exception from this will be caught by the level above
   mEEPROM = new CEEPROM();

   switch(mFileType)
   {
      case HANDY_FILETYPE_RAW:
      case HANDY_FILETYPE_LNX:
#ifdef MY_GLOBAL_SYSTEM_VARS_CPU_MEMBER
         mCart = new CCart(filememory,filesize, gSystemVars);
#else
        mCart = new CCart(filememory,filesize);
#endif
         if(mCart->CartHeaderLess())
         {// veryvery strange Howard Check CANNOT work, as there are two different loader-less card types...
          // unclear HOW this should do anything useful...
            FILE	*fp;
            char drive[3],dir[256],cartgo[256];
            mFileType=HANDY_FILETYPE_HOMEBREW;
            _splitpath(romfile,drive,dir,NULL,NULL);
            strcpy(cartgo,drive);
            strcat(cartgo,dir);
            strcat(cartgo,"howard.o");

            // Open the howard file for reading
            if((fp=fopen(cartgo,"rb"))==NULL)
            {
               fprintf(stderr, "Invalid Cart.\n");
               MY_MEM_ALLOC_FREE(filememory);
            }

            // How big is the file ??
            fseek(fp,0,SEEK_END);
            howardsize=ftell(fp);
            fseek(fp,0,SEEK_SET);
            howardmemory=MY_MEM_NEW_SLOW(UBYTE, filesize);

            if(fread(howardmemory,sizeof(char),howardsize,fp)!=howardsize)
            {
               MY_MEM_ALLOC_FREE(howardmemory);
               fprintf(stderr, "Invalid Cart.\n");
               MY_MEM_ALLOC_FREE(filememory);
            }

            fclose(fp);

            // Pass it to RAM to load
            mRam = new CRam(howardmemory,howardsize);
         }
         else
         {
            mRam = new CRam(0,0);
         }
         break;
      case HANDY_FILETYPE_HOMEBREW:
#ifdef MY_GLOBAL_SYSTEM_VARS_CPU_MEMBER
         mCart = new CCart(0,0, NULL);
#else
         mCart = new CCart(0,0);
#endif
         mRam = new CRam(filememory,filesize);
         break;
      case HANDY_FILETYPE_SNAPSHOT:
      case HANDY_FILETYPE_ILLEGAL:
      default:
#ifdef MY_GLOBAL_SYSTEM_VARS_CPU_MEMBER
         mCart = new CCart(0,0, NULL);
#else
         mCart = new CCart(0,0);
#endif
         mRam = new CRam(0,0);
         break;
   }

   // These can generate exceptions

   mMikie = new CMikie(*this);
   mSusie = new CSusie(*this);

   // Instantiate the memory map handler

   mMemMap = new CMemMap(*this);

   // Now the handlers are set we can instantiate the CPU as is will use handlers on reset

   mCpu = new C65C02(*this);
   
#ifdef MY_GLOBAL_SYSTEM_VARS_CPU_MEMBER
   mCpu->gSystemVars = gSystemVars;
   mMikie->gSystemVars = gSystemVars;
   mSusie->gSystemVars = gSystemVars;
   mCart->gSystemVars = gSystemVars;
   mRam->gSystemVars = gSystemVars;
   mRom->gSystemVars = gSystemVars;
   gSystemVars->gAudioBufferPointer = &mMikie->gAudioBufferPointer;
#endif
   mMikie->gAudioBuffer = SYSTEM_VAR(gAudioBuffer);
   mMikie->mSusie = mSusie;
   mCpu->Reset();
   mRam->Reset();

   // Now init is complete do a reset, this will cause many things to be reset twice
   // but what the hell, who cares, I don't.....

   Reset();

   // If this is a snapshot type then restore the context

   if(mFileType==HANDY_FILETYPE_SNAPSHOT)
   {
      if(!ContextLoad(gamefile))
      {
         Reset();
         fprintf(stderr, "Invalid Snapshot.\n");
      }
   }
   if(filesize) { MY_MEM_ALLOC_FREE(filememory); }
   if(howardsize) { MY_MEM_ALLOC_FREE(howardmemory); }
   mEEPROM->SetEEPROMType(mCart->mEEPROMType);

   {
      char eepromfile[1024];
      strncpy(eepromfile, gamefile,1024-10);
      strcat(eepromfile,".eeprom");
      mEEPROM->SetFilename(eepromfile);
      printf("filename %d %s %s\n",mCart->mEEPROMType,gamefile,eepromfile);
      mEEPROM->Load();
   }
}

void CSystem::SaveEEPROM(void)
{
   if(mEEPROM!=NULL) mEEPROM->Save();
}

CSystem::~CSystem()
{
   // Cleanup all our objects

   if(mEEPROM!=NULL) delete mEEPROM;
   if(mCart!=NULL) delete mCart;
   if(mRom!=NULL) delete mRom;
   if(mRam!=NULL) delete mRam;
   if(mCpu!=NULL) delete mCpu;
   if(mMikie!=NULL) delete mMikie;
   if(mSusie!=NULL) delete mSusie;
   if(mMemMap!=NULL) delete mMemMap;
}

bool CSystem::IsZip(char *filename)
{
   UBYTE buf[2];
   FILE *fp;

   if((fp=fopen(filename,"rb"))!=NULL)
   {
      fread(buf, 2, 1, fp);
      fclose(fp);
      return(memcmp(buf,"PK",2)==0);
   }
   if(fp)fclose(fp);
   return FALSE;
}

void CSystem::HLE_BIOS_FE00(void)
{
    // Select Block in A
    C6502_REGS regs;
    mCpu->GetRegs(regs);
    mCart->SetShifterValue(regs.A);
    // we just put an RTS behind in fake ROM!
}

void CSystem::HLE_BIOS_FE19(void)
{
    // (not) initial jump from reset vector
    // Clear full 64k memory!
    mRam->Clear();

    // Set Load adresse to $200 ($05,$06)
    mRam->Poke(0x0005,0x00);
    mRam->Poke(0x0006,0x02);
    // Call to $FE00
    mCart->SetShifterValue(0);
    // Fallthrou $FE4A
    HLE_BIOS_FE4A();
}

void CSystem::HLE_BIOS_FE4A(void)
{
    UWORD addr=mRam->Peek(0x0005) | (mRam->Peek(0x0006)<<8);

   // Load from Cart (loader blocks)
      unsigned char buff[256];// maximum 5 blocks
      unsigned char res[256];

        buff[0]=mCart->Peek0();
        int blockcount = 0x100 -  buff[0];

        for (int i = 1; i < 1+51*blockcount; ++i) // first encrypted loader
      {
         buff[i] = mCart->Peek0();
      }

      lynx_decrypt(res, buff, 51);

        for (int i = 0; i < 50*blockcount; ++i)
      {
          Poke_CPU(addr++, res[i]);
      }

    // Load Block(s), decode to ($05,$06)
    // jmp $200

   C6502_REGS regs;
   mCpu->GetRegs(regs);
   regs.PC=0x0200;
   mCpu->SetRegs(regs);
}

void CSystem::HLE_BIOS_FF80(void)
{
    // initial jump from reset vector ... calls FE19
    HLE_BIOS_FE19();
}

void CSystem::Reset(void)
{
   SYSTEM_VAR(gSystemCycleCount)=0;
   SYSTEM_VAR(gNextTimerEvent)=0;
   SYSTEM_VAR(gCPUBootAddress)=0;
#ifdef _LYNXDBG
   gBreakpointHit=FALSE;
#endif
   //gSingleStepMode=FALSE;
   //gSingleStepModeSprites=FALSE;
   SYSTEM_VAR(gSystemIRQ)=FALSE;
   SYSTEM_VAR(gSystemNMI)=FALSE;
   SYSTEM_VAR(gSystemCPUSleep)=FALSE;
   SYSTEM_VAR(gSystemHalt)=FALSE;

   //gThrottleLastTimerCount=0;
   //gThrottleNextCycleCheckpoint=0;

   // gTimerCount=0;

   MIKIE_AUDIO_POINTER_EXT=0;
   SYSTEM_VAR(gAudioLastUpdateCycle)=0;
//	memset(gAudioBuffer,128,HANDY_AUDIO_BUFFER_SIZE); // only for unsigned 8bit
	memset(SYSTEM_VAR(gAudioBuffer),0,HANDY_AUDIO_BUFFER_SIZE); // for unsigned 8/16 bit

#ifdef _LYNXDBG
   SYSTEM_VAR(gSystemHalt)=TRUE;
#endif

   mMemMap->Reset();
   mCart->Reset();
   mEEPROM->Reset();
   mRom->Reset();
   mRam->Reset();
   mMikie->Reset();
   mSusie->Reset();
   mCpu->Reset();

   // Homebrew hashup

   if(mFileType==HANDY_FILETYPE_HOMEBREW)
   {
      mMikie->PresetForHomebrew();

      C6502_REGS regs;
      mCpu->GetRegs(regs);
      regs.PC=(UWORD)SYSTEM_VAR(gCPUBootAddress);
      mCpu->SetRegs(regs);
   }else{
      if(!mRom->mValid)
      {
	  mMikie->PresetForHomebrew();
          mRom->mWriteEnable=true;

          mRom->Poke(0xFE00+0,0x8d);
          mRom->Poke(0xFE00+1,0x97);
          mRom->Poke(0xFE00+2,0xfd);
          mRom->Poke(0xFE00+3,0x60);// RTS

          mRom->Poke(0xFE19+0,0x8d);
          mRom->Poke(0xFE19+1,0x97);
          mRom->Poke(0xFE19+2,0xfd);
          
          mRom->Poke(0xFE4A+0,0x8d);
          mRom->Poke(0xFE4A+1,0x97);
          mRom->Poke(0xFE4A+2,0xfd);          

          mRom->Poke(0xFF80+0,0x8d);
          mRom->Poke(0xFF80+1,0x97);
          mRom->Poke(0xFF80+2,0xfd);          

          mRom->mWriteEnable=false;
      }
   }
}

bool CSystem::ContextSave(const char *context)
{
   FILE *fp;
   bool status=1;
   if((fp=fopen(context,"wb"))==NULL) return false;
   status = ContextSave(fp);
   fclose(fp);
   return status;
}

bool CSystem::ContextSave(FILE *fp)
{
   bool status=1;

   if(!fprintf(fp,LSS_VERSION)) status=0;

   // Save ROM CRC
   ULONG tmp;
   ULONG checksum=mCart->CRC32();
   if(!fwrite(&checksum,sizeof(ULONG),1,fp)) status=0;

   if(!fprintf(fp,"CSystem::ContextSave")) status=0;

   if(!fwrite(&mCycleCountBreakpoint,sizeof(ULONG),1,fp)) status=0;
   if(!fwrite(&SYSTEM_VAR(gSystemCycleCount),sizeof(ULONG),1,fp)) status=0;
   if(!fwrite(&SYSTEM_VAR(gNextTimerEvent),sizeof(ULONG),1,fp)) status=0;
   if(!fwrite(&SYSTEM_VAR(gCPUWakeupTime),sizeof(ULONG),1,fp)) status=0;
   if(!fwrite(&SYSTEM_VAR(gCPUBootAddress),sizeof(ULONG),1,fp)) status=0;
   if(!fwrite(&SYSTEM_VAR(gIRQEntryCycle),sizeof(ULONG),1,fp)) status=0;
#ifdef _LYNXDBG
   if(!fwrite(&gBreakpointHit,sizeof(ULONG),1,fp)) status=0;
#else
	if(!fwrite(&tmp,sizeof(ULONG),1,fp)) status=0; // gBreakpointHit
#endif
   if(!fwrite(&tmp,sizeof(ULONG),1,fp)) status=0; // gSingleStepMode
   if(!fwrite(&SYSTEM_VAR(gSystemIRQ),sizeof(ULONG),1,fp)) status=0;
   if(!fwrite(&SYSTEM_VAR(gSystemNMI),sizeof(ULONG),1,fp)) status=0;
   if(!fwrite(&SYSTEM_VAR(gSystemCPUSleep),sizeof(ULONG),1,fp)) status=0;
   if(!fwrite(&SYSTEM_VAR(gSystemCPUSleep_Saved),sizeof(ULONG),1,fp)) status=0;
   if(!fwrite(&SYSTEM_VAR(gSystemHalt),sizeof(ULONG),1,fp)) status=0;
   if(!fwrite(&tmp,sizeof(ULONG),1,fp)) status=0;//gThrottleMaxPercentage
   if(!fwrite(&tmp,sizeof(ULONG),1,fp)) status=0;//gThrottleLastTimerCount
   if(!fwrite(&tmp,sizeof(ULONG),1,fp)) status=0;//gThrottleNextCycleCheckpoint

   tmp=0;//gTimerCount;
   if(!fwrite(&tmp,sizeof(ULONG),1,fp)) status=0;

   // ** if(!fwrite(gAudioBuffer,sizeof(UBYTE),HANDY_AUDIO_BUFFER_SIZE,fp)) status=0;
   // ** if(!fwrite(&gAudioBufferPointer,sizeof(ULONG),1,fp)) status=0;
   if(!fwrite(&SYSTEM_VAR(gAudioLastUpdateCycle),sizeof(ULONG),1,fp)) status=0;

   // Save other device contexts
   if(!mMemMap->ContextSave(fp)) status=0;

   if(!mCart->ContextSave(fp)) status=0;

   // *** if(!mEEPROM->ContextSave(fp)) status=0;
   //	if(!mRom->ContextSave(fp)) status=0; We no longer save the system ROM
   if(!mRam->ContextSave(fp)) status=0;
   if(!mMikie->ContextSave(fp)) status=0;
   if(!mSusie->ContextSave(fp)) status=0;
   if(!mCpu->ContextSave(fp)) status=0;

   return status;
}

size_t	CSystem::MemoryContextSave(const char* tmpfilename, char *context)
{
   size_t ret = 0;

   if(!ContextSave(tmpfilename))
   {
      return 0;
   }

   FILE *fp;
   UBYTE *filememory = NULL;
   ULONG filesize = 0;

   if((fp = fopen(tmpfilename,"rb")) == NULL)
   {
      return 0;
   }

   fseek(fp,0,SEEK_END);
   filesize = ftell(fp);
   fseek(fp,0,SEEK_SET);

   if(NULL == context)
   {
      filememory = MY_MEM_NEW_SLOW(UBYTE, filesize);
   }
   else
   {
      filememory = (UBYTE*) context;
   }

   if(fread(filememory,sizeof(char),filesize,fp) == filesize)
   {
      ret = filesize;
   }

   fclose(fp);

   if(NULL == context)
   {
      MY_MEM_ALLOC_FREE(filememory);
   }

   remove(tmpfilename);

   return ret;
}

bool CSystem::MemoryContextLoad(const char *context, size_t size)
{
   LSS_FILE *fp;
   bool status=1;
   UBYTE *filememory=(UBYTE*)context;
   ULONG filesize=size;

   // Setup our read structure
   fp = new LSS_FILE;
   fp->memptr=filememory;
   fp->index=0;
   fp->index_limit=filesize;

   char teststr[100];
   // Check identifier
   if(!lss_read(teststr,sizeof(char),4,fp)) status=0;
   teststr[4]=0;

   if(strcmp(teststr,LSS_VERSION)==0 || strcmp(teststr,LSS_VERSION_OLD)==0)
   {
      bool legacy=FALSE;
      if(strcmp(teststr,LSS_VERSION_OLD)==0)
      {
         legacy=TRUE;
      }
      else
      {
         ULONG checksum;
         // Read CRC32 and check against the CART for a match
         lss_read(&checksum,sizeof(ULONG),1,fp);
         if(mCart->CRC32()!=checksum)
         {
            delete fp;
            fprintf(stderr, "[handy]LSS Snapshot CRC does not match the loaded cartridge image, aborting load.\n");
            return 0;
         }
      }

      // Check our block header
      if(!lss_read(teststr,sizeof(char),20,fp)) status=0;
      teststr[20]=0;
      if(strcmp(teststr,"CSystem::ContextSave")!=0) status=0;
      ULONG tmp;

      if(!lss_read(&mCycleCountBreakpoint,sizeof(ULONG),1,fp)) status=0;
      if(!lss_read(&SYSTEM_VAR(gSystemCycleCount),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read(&SYSTEM_VAR(gNextTimerEvent),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read(&SYSTEM_VAR(gCPUWakeupTime),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read(&SYSTEM_VAR(gCPUBootAddress),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read(&SYSTEM_VAR(gIRQEntryCycle),sizeof(ULONG),1,fp)) status=0;
#ifdef _LYNXDBG
      if(!lss_read(&gBreakpointHit,sizeof(ULONG),1,fp)) status=0;
#else
      if(!lss_read(&tmp,sizeof(ULONG),1,fp)) status=0;
#endif
      if(!lss_read(&tmp,sizeof(ULONG),1,fp)) status=0; // gSingleStepMode
      if(!lss_read(&SYSTEM_VAR(gSystemIRQ),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read(&SYSTEM_VAR(gSystemNMI),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read(&SYSTEM_VAR(gSystemCPUSleep),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read(&SYSTEM_VAR(gSystemCPUSleep_Saved),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read(&SYSTEM_VAR(gSystemHalt),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read(&tmp,sizeof(ULONG),1,fp)) status=0;//gThrottleMaxPercentage
      if(!lss_read(&tmp,sizeof(ULONG),1,fp)) status=0;//gThrottleLastTimerCount
      if(!lss_read(&tmp,sizeof(ULONG),1,fp)) status=0;//gThrottleNextCycleCheckpoint

      if(!lss_read(&tmp,sizeof(ULONG),1,fp)) status=0;
      // gTimerCount=tmp;

      //if(!lss_read(gAudioBuffer,sizeof(UBYTE),HANDY_AUDIO_BUFFER_SIZE,fp)) status=0;
      //if(!lss_read(&gAudioBufferPointer,sizeof(ULONG),1,fp)) status=0;
      if(!lss_read(&SYSTEM_VAR(gAudioLastUpdateCycle),sizeof(ULONG),1,fp)) status=0;

      if(!mMemMap->ContextLoad(fp)) status=0;
      // Legacy support
      if(legacy)
      {
         if(!mCart->ContextLoadLegacy(fp)) status=0;
         if(!mRom->ContextLoad(fp)) status=0;
      }
      else
      {
         if(!mCart->ContextLoad(fp)) status=0;
      }
      if(!mRam->ContextLoad(fp)) status=0;
      if(!mMikie->ContextLoad(fp)) status=0;
      if(!mSusie->ContextLoad(fp)) status=0;
      if(!mCpu->ContextLoad(fp)) status=0;
   }
   else
   {
      fprintf(stderr, "[handy]Not a recognised LSS file\n");
   }

   delete fp;

   return status;
}

bool CSystem::ContextLoad(const char *context)
{
    bool status=1;
    FILE *fp;
    if((fp=fopen(context,"rb"))==NULL) return false;
    status = ContextLoad(fp);
    fclose(fp);
    return status;
}

bool CSystem::ContextLoad(FILE *fp_full)
{
   LSS_FILE *fp;
   bool status=1;
   UBYTE *filememory=NULL;
   ULONG filesize=0;

   {
      // Just open an read into memory
      long offset = ftell(fp_full);
      fseek(fp_full,0,SEEK_END);
      filesize=ftell(fp_full) - offset;
      fseek(fp_full,offset,SEEK_SET);
      filememory=MY_MEM_NEW_SLOW(UBYTE, filesize);

      if(fread(filememory,sizeof(char),filesize,fp_full)!=filesize)
      {
         return 1;
      }
   }

   // Setup our read structure
   fp = new LSS_FILE;
   fp->memptr=filememory;
   fp->index=0;
   fp->index_limit=filesize;

   char teststr[100];
   // Check identifier
   if(!lss_read(teststr,sizeof(char),4,fp)) status=0;
   teststr[4]=0;

   if(strcmp(teststr,LSS_VERSION)==0 || strcmp(teststr,LSS_VERSION_OLD)==0)
   {
      bool legacy=FALSE;
      if(strcmp(teststr,LSS_VERSION_OLD)==0)
      {
         legacy=TRUE;
      }
      else
      {
         ULONG checksum;
         // Read CRC32 and check against the CART for a match
         lss_read(&checksum,sizeof(ULONG),1,fp);
         if(mCart->CRC32()!=checksum)
         {
            delete fp;
            MY_MEM_ALLOC_FREE(filememory);
            fprintf(stderr, "[handy]LSS Snapshot CRC does not match the loaded cartridge image, aborting load.\n");
            return 0;
         }
      }

      // Check our block header
      if(!lss_read(teststr,sizeof(char),20,fp)) status=0;
      teststr[20]=0;
      if(strcmp(teststr,"CSystem::ContextSave")!=0) status=0;
      ULONG tmp;

      if(!lss_read_ulong(&mCycleCountBreakpoint,sizeof(ULONG),1,fp)) status=0;
      if(!lss_read_ulong(&SYSTEM_VAR(gSystemCycleCount),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read_ulong(&SYSTEM_VAR(gNextTimerEvent),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read_ulong(&SYSTEM_VAR(gCPUWakeupTime),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read_ulong(&SYSTEM_VAR(gCPUBootAddress),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read_ulong(&SYSTEM_VAR(gIRQEntryCycle),sizeof(ULONG),1,fp)) status=0;
#ifdef _LYNXDBG
      if(!lss_read_ulong(&gBreakpointHit,sizeof(ULONG),1,fp)) status=0;
#else
      if(!lss_read_ulong(&tmp,sizeof(ULONG),1,fp)) status=0;
#endif
      if(!lss_read_ulong(&tmp,sizeof(ULONG),1,fp)) status=0; // gSingleStepMode
      if(!lss_read_ulong(&SYSTEM_VAR(gSystemIRQ),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read_ulong(&SYSTEM_VAR(gSystemNMI),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read_ulong(&SYSTEM_VAR(gSystemCPUSleep),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read_ulong(&SYSTEM_VAR(gSystemCPUSleep_Saved),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read_ulong(&SYSTEM_VAR(gSystemHalt),sizeof(ULONG),1,fp)) status=0;
      if(!lss_read_ulong(&tmp,sizeof(ULONG),1,fp)) status=0;//gThrottleMaxPercentage
      if(!lss_read_ulong(&tmp,sizeof(ULONG),1,fp)) status=0;//gThrottleLastTimerCount
      if(!lss_read_ulong(&tmp,sizeof(ULONG),1,fp)) status=0;//gThrottleNextCycleCheckpoint

      if(!lss_read(&tmp,sizeof(ULONG),1,fp)) status=0;
      // gTimerCount=tmp;

      // ** if(!lss_read(gAudioBuffer,sizeof(UBYTE),HANDY_AUDIO_BUFFER_SIZE,fp)) status=0;
      // ** if(!lss_read(&gAudioBufferPointer,sizeof(ULONG),1,fp)) status=0;
      if(!lss_read_ulong(&SYSTEM_VAR(gAudioLastUpdateCycle),sizeof(ULONG),1,fp)) status=0;

      if(!mMemMap->ContextLoad(fp)) status=0;
      // Legacy support
      if(legacy)
      {
         if(!mCart->ContextLoadLegacy(fp)) status=0;
         if(!mRom->ContextLoad(fp)) status=0;
      }
      else
      {
         if(!mCart->ContextLoad(fp)) status=0;
      }
      if(!mRam->ContextLoad(fp)) status=0;
      if(!mMikie->ContextLoad(fp)) status=0;
      if(!mSusie->ContextLoad(fp)) status=0;
      if(!mCpu->ContextLoad(fp)) status=0;
   }
   else
   {
      gError->Warning("Not a recognised LSS file");
   }

   delete fp;
   MY_MEM_ALLOC_FREE(filememory);

   return status;
}

#ifdef _LYNXDBG

void CSystem::DebugTrace(int address)
{
   char message[1024+1];
   int count=0;

   sprintf(message,"%08x - DebugTrace(): ",SYSTEM_VAR(gSystemCycleCount));
   count=strlen(message);

   if(address)
   {
      if(address==0xffff)
      {
         C6502_REGS regs;
         char linetext[1024];
         // Register dump
         GetRegs(regs);
         sprintf(linetext,"PC=$%04x SP=$%02x PS=0x%02x A=0x%02x X=0x%02x Y=0x%02x",regs.PC,regs.SP, regs.PS,regs.A,regs.X,regs.Y);
         strcat(message,linetext);
         count=strlen(message);
      }
      else
      {
         // The RAM address contents should be dumped to an open debug file in this function
         do
         {
            message[count++]=Peek_RAM(address);
         }
         while(count<1024 && Peek_RAM(address++)!=0);
      }
   }
   else
   {
      strcat(message,"CPU Breakpoint");
      count=strlen(message);
   }
   message[count]=0;

   // Callback to dump the message
   if(mpDebugCallback)
   {
      (*mpDebugCallback)(mDebugCallbackObject,message);
   }
}

void CSystem::DebugSetCallback(void (*function)(ULONG objref,char *message),ULONG objref)
{
   mDebugCallbackObject=objref;
   mpDebugCallback=function;
}


#endif
