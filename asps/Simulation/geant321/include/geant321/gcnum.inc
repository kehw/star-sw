*
* $Id: gcnum.inc,v 1.2 2004/03/01 17:26:25 fisyak Exp $
*
* $Log: gcnum.inc,v $
* Revision 1.2  2004/03/01 17:26:25  fisyak
* Get rid of staf
*
* Revision 1.1.1.1  1995/10/24 10:20:32  cernlib
* Geant
*
*
#ifndef CERNLIB_GEANT321_GCNUM_INC
#define CERNLIB_GEANT321_GCNUM_INC
*
*
* gcnum.inc
*
#if !defined(CERNLIB_INCLUDE)
      COMMON/GCNUM/NMATE ,NVOLUM,NROTM,NTMED,NTMULT,NTRACK,NPART
     +            ,NSTMAX,NVERTX,NHEAD,NBIT
      COMMON /GCNUMX/ NALIVE,NTMSTO
C
#if defined(CERNLIB_TYPE)
#include "geant321/gtnum.inc"
#endif
#endif
*
* gcnum.inc
*
#if defined(CERNLIB_INCLUDE)
*
#include "geant321/incbeg.inc"
     +   gcnum
#include "geant321/incend.inc"
#endif

#endif
