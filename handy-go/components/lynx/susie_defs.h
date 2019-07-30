#pragma once

// Define register typdefs

typedef struct 
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	High;
         UBYTE	Low;
#else
         UBYTE	Low;
         UBYTE	High;
#endif
      }Byte;
      UWORD	Word;
      //ULONG Word;
   };
   //UWORD    dummy;
}UUWORD;


typedef struct
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	Fc1:1;
         UBYTE	Fc2:1;
         UBYTE	Fc3:1;
         UBYTE	reserved:1;
         UBYTE	Ac1:1;
         UBYTE	Ac2:1;
         UBYTE	Ac3:1;
         UBYTE	Ac4:1;
#else
         UBYTE	Ac4:1;
         UBYTE	Ac3:1;
         UBYTE	Ac2:1;
         UBYTE	Ac1:1;
         UBYTE	reserved:1;
         UBYTE	Fc3:1;
         UBYTE	Fc2:1;
         UBYTE	Fc1:1;
#endif
      }Bits;
      UBYTE	Byte;
   };
}TSPRINIT;

typedef struct
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	Up:1;
         UBYTE	Down:1;
         UBYTE	Left:1;
         UBYTE	Right:1;
         UBYTE	Option1:1;
         UBYTE	Option2:1;
         UBYTE	Inside:1;
         UBYTE	Outside:1;
#else
         UBYTE	Outside:1;
         UBYTE	Inside:1;
         UBYTE	Option2:1;
         UBYTE	Option1:1;
         UBYTE	Right:1;
         UBYTE	Left:1;
         UBYTE	Down:1;
         UBYTE	Up:1;
#endif
      }Bits;
      UBYTE	Byte;
   };
}TJOYSTICK;

typedef struct
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	spare:5;
         UBYTE	Cart1IO:1;
         UBYTE	Cart0IO:1;
         UBYTE	Pause:1;
#else
         UBYTE	Pause:1;
         UBYTE	Cart0IO:1;
         UBYTE	Cart1IO:1;
         UBYTE	spare:5;
#endif
      }Bits;
      UBYTE	Byte;
   };
}TSWITCHES;

typedef struct
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	A;
         UBYTE	B;
         UBYTE	C;
         UBYTE	D;
#else
         UBYTE	D;
         UBYTE	C;
         UBYTE	B;
         UBYTE	A;
#endif
      }Bytes;
      struct
      {
#ifdef MSB_FIRST
         UWORD	AB;
         UWORD	CD;
#else
         UWORD	CD;
         UWORD	AB;
#endif
      }Words;
      ULONG	Long;
   };
}TMATHABCD;

typedef struct
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	E;
         UBYTE	F;
         UBYTE	G;
         UBYTE	H;
#else
         UBYTE	H;
         UBYTE	G;
         UBYTE	F;
         UBYTE	E;
#endif
      }Bytes;
      struct
      {
#ifdef MSB_FIRST
         UWORD	EF;
         UWORD	GH;
#else
         UWORD	GH;
         UWORD	EF;
#endif
      }Words;
      ULONG	Long;
   };
}TMATHEFGH;

typedef struct
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	J;
         UBYTE	K;
         UBYTE	L;
         UBYTE	M;
#else
         UBYTE	M;
         UBYTE	L;
         UBYTE	K;
         UBYTE	J;
#endif
      }Bytes;
      struct
      {
#ifdef MSB_FIRST
         UWORD	JK;
         UWORD	LM;
#else
         UWORD	LM;
         UWORD	JK;
#endif
      }Words;
      ULONG	Long;
   };
}TMATHJKLM;

typedef struct
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE	xx2;
         UBYTE	xx1;
         UBYTE	N;
         UBYTE	P;
#else
         UBYTE	P;
         UBYTE	N;
         UBYTE	xx1;
         UBYTE	xx2;
#endif
      }Bytes;
      struct
      {
#ifdef MSB_FIRST
         UWORD	xx1;
         UWORD	NP;
#else
         UWORD	NP;
         UWORD	xx1;
#endif
      }Words;
      ULONG	Long;
   };
}TMATHNP;
