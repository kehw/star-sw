*
* $Id: parjet.F,v 1.1.1.2 2009/02/01 17:10:18 fisyak Exp $
*
* $Log: parjet.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:18  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:46:53  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:24  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.1.1.1  2002/07/24 15:56:24  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:37  hristov
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
*$ CREATE PARJET.FOR
*COPY PARJET
*
*=== parjet ===========================================================*
*
      SUBROUTINE PARJET(IHAD,I)
 
#include "geant321/dblprc.inc"
#include "geant321/dimpar.inc"
#include "geant321/iounit.inc"
#include "geant321/finpar.inc"
#include "geant321/part.inc"
#include "geant321/inpdat2.inc"
      IHAD=1
      NREF(1)=I
      PXF(1)=0.D0
      PYF(1)=0.D0
      PZF(1)=0.D0
      HEP(1)=AM(I)
      AMF(1)=AM(I)
      ICHF(1)=ICH(I)
      IBARF(1)=IBAR(I)
      ANF(1)=ANAME(I)
      RETURN
      END
