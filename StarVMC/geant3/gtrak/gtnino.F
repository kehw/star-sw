*
* $Id: gtnino.F,v 1.1.1.2 2009/02/01 17:10:33 fisyak Exp $
*
* $Log: gtnino.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:33  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:48:55  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:56  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.2  2003/11/28 11:23:56  brun
* New version of geant321 with all geant3 routines renamed from G to G3
*
* Revision 1.1.1.1  2002/07/24 15:56:26  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:42  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:21  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:21:44  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/04 01/11/94  16.25.59  by  S.Ravndal
*-- Author :
      SUBROUTINE G3TNINO
C.
C.    ******************************************************************
C.    *                                                                *
C.    *       Tracking routine for "GEANTINO" type tracks              *
C.    *                                                                *
C.    *    ==>Called by : G3TRACK                                      *
C.    *       Author    R.Brun, F.Bruyant                              *
C.    *                                                                *
C.    ******************************************************************
C.
#include "geant321/gconsp.inc"
#include "geant321/gctmed.inc"
#include "geant321/gctrak.inc"
#if defined(CERNLIB_DEBUG)
#include "geant321/gcunit.inc"
#endif
#if !defined(CERNLIB_SINGLE)
      PARAMETER (EPSMAC=1.E-6)
#endif
#if defined(CERNLIB_SINGLE)
      PARAMETER (EPSMAC=1.E-11)
#endif
C.
C.    ------------------------------------------------------------------
C.
      STEP = STEMAX
      IPROC = 103
      CALL GTNEXT
      IF (IGNEXT.NE.0) THEN
         STEP = SNEXT + PREC
         IPROC=0
         INWVOL= 2
         NMEC =  1
         LMEC(1)=1
*
* *** Linear transport
*
         DO 10 I = 1,3
            VECTMP  = VECT(I) +STEP*VECT(I+3)
            IF(VECTMP.EQ.VECT(I)) THEN
*
* *** Correct for machine precision
*
               IF(VECT(I+3).NE.0.) THEN
                  VECTMP = VECT(I)+ABS(VECT(I))*SIGN(1.,VECT(I+3))*
     +            EPSMAC
*                 IF(NMEC.GT.0) THEN
*                    IF(LMEC(NMEC).EQ.32) NMEC=NMEC-1
*                 ENDIF
*                 NMEC=NMEC+1
*                 LMEC(NMEC)=32
#if defined(CERNLIB_DEBUG)
                  WRITE(CHMAIL, 10000)
                  CALL GMAIL(0,0)
                  WRITE(CHMAIL, 10100) GEKIN, NUMED, STEP, SNEXT
                  CALL GMAIL(0,0)
10000 FORMAT(' Boundary correction in GTNINO: ',
     +       '    GEKIN      NUMED       STEP      SNEXT')
10100 FORMAT(31X,E10.3,1X,I10,1X,E10.3,1X,E10.3,1X)
#endif
               ENDIF
            ENDIF
            VECT(I) = VECTMP
   10    CONTINUE
      ELSE
         DO 20 I = 1,3
            VECT(I)  = VECT(I) +STEP*VECT(I+3)
   20    CONTINUE
      ENDIF
      SLENG = SLENG +STEP
*
* *** Update time of flight
*
      TOFG = TOFG +STEP/CLIGHT
*
      IF(IPROC.NE.0) THEN
         NMEC=NMEC+1
         LMEC(NMEC)=IPROC
      ENDIF
      END
