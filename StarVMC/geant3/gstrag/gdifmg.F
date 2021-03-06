*
* $Id: gdifmg.F,v 1.1.1.2 2009/02/01 17:10:32 fisyak Exp $
*
* $Log: gdifmg.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:32  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:48:48  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:56  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.2  2003/11/28 11:23:56  brun
* New version of geant321 with all geant3 routines renamed from G to G3
*
* Revision 1.1.1.1  2002/07/24 15:56:27  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:41  hristov
* Separate distribution  of Geant3
*
* Revision 1.2  2002/05/13 12:40:58  hristov
* Dummy subroutines to avoid files with no code in
*
* Revision 1.1.1.1  1999/05/18 15:55:20  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:21:37  cernlib
* Geant
*
*
#include "geant321/pilot.h"
#if defined(CERNLIB_ASHO)
*CMZ :  3.21/02 29/03/94  15.41.25  by  S.Giani
*-- Author :
      SUBROUTINE G3DIFMG(DEL)
 
#include "geant321/gccuts.inc"
#include "geant321/gcasho.inc"
C-----------------------------------------------------------------------
C      This program calculates the final distribution coef-
C      ficients until the integral S is more or equal to L2.
C      In case of S < L2 or miscalculations due to FORTRAN
C      limits DONE = FALSE.
C-----------------------------------------------------------------------
C      Update history: 10-08-93
C
C-----------------------------------------------------------------------
      REAL BN(50),BNA(50),L2
      INTEGER FLMAX
      PARAMETER (DL2=.1)
      DATA CORMAX/80./
C-----------------------------------------------------------------------
C      Initialization steps
C-----------------------------------------------------------------------
      XX = 0.
      Z = 1.
      DO 10 K=1,NOSC
         BN(K) = BOSC(K)*IOSC(K)
         BNA(K) = BN(K)*(AOSC(K) - 1.)
         XX = XX + AOSC(K)*BOSC(K)
   10 CONTINUE
C
      FLMAX = 0
      CORR = CORMAX - XX
      CMGO(1) = EXP(-CORMAX)
      CMGO(2) = CMGO(1)*BNA(1)
      Z = Z-(CMGO(1)+CMGO(2))*EXP(CORR)
C-----------------------------------------------------------------------
C      Calculation of the current Ci
C-----------------------------------------------------------------------
      CALL GRNDM(RNDM,1)
 
      L2 = DL2
      IF (RNDM.GE.L2) THEN
         L2 = RNDM
         FLMAX = 1
      ENDIF
      DO 40 NMGO=3,NMGOMA
         CMGO(NMGO) = CMGO(NMGO - 1)*BNA(1)
         DO 30 J = 1,NMGO - 2
            M = NMGO - 1 - J
            X = BOSC(1)/J
            DO 20 K=2,NOSC
               IF(MOD(J+1,IOSC(K)).EQ.0) THEN
                  L = (J+1)/IOSC(K)
                  IF(L.EQ.1) THEN
                     X = X + BNA(K)
                  ELSE
                     X = X + BN(K)/(L-1)
                  ENDIF
               ENDIF
   20       CONTINUE
            CMGO(NMGO) = CMGO(NMGO) + X*CMGO(M)
   30    CONTINUE
         CMGO(NMGO) = CMGO(NMGO)/(NMGO-1)
         Z = Z - CMGO(NMGO)*EXP(CORR)
         IF (Z.LE.L2) GOTO 50
   40 CONTINUE
 
   50 IF (FLMAX.EQ.1) THEN
         DEL = (NMGO-1)*EOSC(1)
      ELSE
         DEL = (NMGO-1)*EOSC(1)*L2/RNDM
      ENDIF
      IF (DEL.GE.DCUTE*1E6) DEL = DCUTE*1E6
 
      END
#else
      SUBROUTINE GDIFMG_DUMMY
      END
#endif
