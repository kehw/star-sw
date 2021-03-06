*
* $Id: kpois.F,v 1.1.1.2 2009/02/01 17:10:18 fisyak Exp $
*
* $Log: kpois.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:18  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:46:51  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:24  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.1.1.1  2002/07/24 15:56:24  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:36  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:16  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:20:01  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.44  by  S.Giani
*-- Author :
*$ CREATE KPOIS.FOR
*COPY KPOIS
*
*=== kpois ============================================================*
*
      FUNCTION KPOIS(EXPLAM)
 
#include "geant321/dblprc.inc"
#include "geant321/dimpar.inc"
#include "geant321/iounit.inc"
      REAL RNDM(1)
C*** EXPLAM IS EXP(-LAMBDA)
C*** SQMPLING FROM POISSON DISTRIBUTION, AVERAGE LAMBDA
C*** PK = EXP(-LAMBDA)*LAMBDA**K/(K-FACTORIAL)
      Y=1.D0
      KPOISS=0
5     CONTINUE
      CALL GRNDM(RNDM,1)
      Y=Y*RNDM(1)
      IF (Y.LT.EXPLAM) GO TO 10
      KPOISS=KPOISS+1
      GO TO 5
10    CONTINUE
      KPOIS=KPOISS
      RETURN
      END
