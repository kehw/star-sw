*
* $Id: gfsdpr.F,v 1.1.1.1 2004/01/15 00:12:13 potekhin Exp $
*
* $Log: gfsdpr.F,v $
* Revision 1.1.1.1  2004/01/15 00:12:13  potekhin
*
* Revision 1.1.1.1  1997/11/03 15:30:35  atlascvs
* Importing CERNLIB version 08.21.
*
* Revision 1.1.1.1  1995/10/24 10:21:25  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.21  by  S.Giani
*-- Author :
      SUBROUTINE GFSDPR(Z,NSHL,PRB)
C.
C.    ******************************************************************
C.    *                                                                *
C     *       Fetch probability of radiative shells decay              *
C.    *                                                                *
C.    *    ==>Called by : GFSHDC                                       *
C.    *       Author    J. Chwastowski                                 *
C.    *                                                                *
C.    ******************************************************************
C.
#include "geant321/gcpmxz.inc"
#include "geant321/gcphpr.inc"
      DIMENSION PRB(NSHL)
      IZ = Z
      DO 10 I = 1,NSHL
         PRB(I) = GFLUPR(I,IZ)
   10 CONTINUE
      END
