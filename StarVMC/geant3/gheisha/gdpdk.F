*
* $Id: gdpdk.F,v 1.1.1.2 2009/02/01 17:10:27 fisyak Exp $
*
* $Log: gdpdk.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:27  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:48:06  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:37  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.1.1.1  2002/07/24 15:56:27  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:39  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:18  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:21:01  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.39  by  S.Giani
*-- Author :
      FUNCTION GDPDK(A,B,C)
C
C *** DOUBLE PRECISION VERSION OF "PDK" ***
C *** NVE 29-MAR-1988 CERN GENEVA ***
C
C CALLED BY : PHASP
C ORIGIN : H.FESEFELDT (02-DEC-1986)
C
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C
C     GDPDK = SQRT(A*A+(B*B-C*C)**2/(A*A) - 2.0*(B*B+C*C))/2.0
C
      A2 = A*A
      B2 = B*B
      C2 = C*C
      IF(A2) 21,21,61
   61 CONTINUE
      ARG=A2+(B2-C2)**2/A2-2.0*(B2+C2)
      IF (ARG) 21,21,31
   21 GDPDK=0.0
      GOTO 41
   31 CONTINUE
      GDPDK = 0.5*SQRT(ABS(A2 + (B2-C2)**2/A2 - 2.0*(B2+C2)))
   41 CONTINUE
      RETURN
      END
