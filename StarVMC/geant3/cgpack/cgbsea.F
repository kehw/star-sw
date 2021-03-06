*
* $Id: cgbsea.F,v 1.1.1.2 2009/02/01 17:10:06 fisyak Exp $
*
* $Log: cgbsea.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:06  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:46:00  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:36:38  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.1.1.1  2002/07/24 15:56:27  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:17:53  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:04  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:19:42  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.31  by  S.Giani
*-- Author :
      SUBROUTINE CGBSEA(A,B,NB,JB)
************************************************************************
*                                                                      *
*     Name: CGBSEA                                                     *
*     Author: E. Chernyaev                       Date:    22.10.88     *
*                                                Revised:              *
*                                                                      *
*     Function: For face A search face with the same plane in object B *
*                                                                      *
*     References: none                                                 *
*                                                                      *
*     Input: A(*) - face                                               *
*            B(*) - CG-object                                          *
*     Input                                                            *
*     Output: NB  - current face number in object B                    *
*             JB  - current pointer in object B                        *
*                                                                      *
*     Errors: none                                                     *
*                                                                      *
************************************************************************
#include "geant321/cggpar.inc"
#include "geant321/cgdelt.inc"
      REAL      A(*),B(*),ABCD(4)
*-
      ABCD(1)= A(KCGAA)
      ABCD(2)= A(KCGBB)
      ABCD(3)= A(KCGCC)
      ABCD(4)= A(KCGDD)
      IF (NB .EQ. 0)            J = LCGHEA
      IF (NB .EQ. 0)            GOTO 100
      NEDGE  = B(JB+KCGNE)
      J      = JB + LCGFAC + NEDGE*LCGEDG
  100 NSTART = NB + 1
      NFACE  = B(KCGNF)
      IF (NSTART .GT. NFACE)    GOTO 998
      DO 120 NF=NSTART,NFACE
        IF (ABS(B(J+KCGAA)-ABCD(1)) .GT. EEWOR) GOTO 110
        IF (ABS(B(J+KCGBB)-ABCD(2)) .GT. EEWOR) GOTO 110
        IF (ABS(B(J+KCGCC)-ABCD(3)) .GT. EEWOR) GOTO 110
        IF (ABS(B(J+KCGDD)-ABCD(4)) .GT. EEWOR) GOTO 110
        NB     = NF
        JB     = J
        GOTO 999
  110   NEDGE  = B(J+KCGNE)
        J      = J + LCGFAC + NEDGE*LCGEDG
  120   CONTINUE
*
  998 NB     = 0
  999 RETURN
      END
