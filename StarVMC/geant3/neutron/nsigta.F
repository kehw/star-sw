*
* $Id: nsigta.F,v 1.1.1.2 2009/02/01 17:10:35 fisyak Exp $
*
* $Log: nsigta.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:35  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:49:03  fisyak
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
* Revision 1.1.1.1  1995/10/24 10:21:58  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.48  by  S.Giani
*-- Author :
      SUBROUTINE NSIGTA(E,JMED,TSIG,D,ISIGTS,LSIGT)
C       THIS ROUTINE DETERMINES THE MACROSCOPIC TOTAL
C       CROSS SECTION FOR MEDIA MED
#include "geant321/mmicab.inc"
      DIMENSION D(*),ISIGTS(*),LSIGT(*)
      CALL GTMED(JMED,MED)
      TSIG=0.0
      L1=LSIGT(MED)
      LS1=ISIGTS(MED)+LMOX3
      LEN=L1/2
      CALL TBSPLT(D(LS1),E,LEN,TSIG)
      RETURN
      END
