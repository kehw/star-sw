*
* $Id: getpar.F,v 1.1.1.2 2009/02/01 17:10:34 fisyak Exp $
*
* $Log: getpar.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:34  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:49:01  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:58  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.1.1.1  2002/07/24 15:56:27  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:43  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:21  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:21:56  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.48  by  S.Giani
*-- Author :
      SUBROUTINE GETPAR(ID,N,IERR)
C retrieve particle from MPSTOR common
#include "geant321/mconst.inc"
#include "geant321/mpstor.inc"
#include "geant321/minput.inc"
      IERR = 0
      NN = 0
      NS = 1
   10 CONTINUE
      IF(IDN(NS).EQ.ID) NN = NN + 1
      IF(N.EQ.NN) GOTO 20
      NS = NS + 1
      IF(NS.GT.NPSTOR) THEN
         WRITE(IOUT,'('' MICAP: Cant retrieve particle no. '',I3,      '
     +   //'          '' of type '',I3,''; End of data '')') N,ID
         IERR = 1
         RETURN
      ENDIF
      GOTO 10
   20 CONTINUE
      EP = EN(NS)
      UP = UN(NS)
      VP = VN(NS)
      WP = WN(NS)
      AMP = AMN(NS)
      ZMP = ZMN(NS)
      AGEP = AGEN(NS)
      MTP = MTN(NS)
      RETURN
      END
