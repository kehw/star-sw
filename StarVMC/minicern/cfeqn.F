*
* $Id: cfeqn.F,v 1.1 2005/06/09 00:05:09 fisyak Exp $
*
* $Log: cfeqn.F,v $
* Revision 1.1  2005/06/09 00:05:09  fisyak
* Add stuff for reconstruction
*
* Revision 1.1  2005/05/26 22:28:00  fisyak
* Add stuff necessary for reconstruction packages
*
* Revision 1.1.1.1  1996/02/15 17:49:03  mclareni
* Kernlib
*
*
#include "kernnum/pilot.h"
          SUBROUTINE          CFEQN(N,A,IDIM,IR,K,B)
          INTEGER             IR(*)
          COMPLEX             A(IDIM,*),B(IDIM,*),X,Y,TE
          CHARACTER*6         HNAME
#if !defined(CERNLIB_NUMCDBLE)
          COMPLEX             S21, S22, DOTF
          DOTF(X,Y,S21)  =  X*Y + S21
#endif
#if defined(CERNLIB_NUMCDBLE)
          COMPLEX*16          S21, S22, DC, DOTF
          DC(X)          =  DCMPLX(DBLE(REAL(X)),DBLE(AIMAG(X)))
          DOTF(X,Y,S21)  =  DC(X)*DC(Y) + S21
#endif
          DATA      HNAME               /  ' CFEQN'  /
#include "feqn.inc"
          RETURN
          END
