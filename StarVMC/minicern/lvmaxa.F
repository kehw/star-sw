*
* $Id: lvmaxa.F,v 1.1 2005/06/09 00:05:15 fisyak Exp $
*
* $Log: lvmaxa.F,v $
* Revision 1.1  2005/06/09 00:05:15  fisyak
* Add stuff for reconstruction
*
* Revision 1.1  2005/05/26 22:28:11  fisyak
* Add stuff necessary for reconstruction packages
*
* Revision 1.1.1.1  1996/02/15 17:50:12  mclareni
* Kernlib
*
*
#include "kerngen/pilot.h"
      FUNCTION LVMAXA (A,N)
C
C CERN PROGLIB# F121    LVMAXA          .VERSION KERNFOR  1.0   710701
C ORIG. 01/07/71
C
      DIMENSION A(*)
C
C
      IF (N.LE.0)  GO TO 100
      J= 1
      AMAX = 0.
      DO 9 I= 1,N
      CA= ABS (A(I))
      IF (CA.LE.AMAX)  GO TO 9
      AMAX= CA
      J= I
    9 CONTINUE
      LVMAXA = J
      RETURN
  100 LVMAXA = 0
      RETURN
      END
