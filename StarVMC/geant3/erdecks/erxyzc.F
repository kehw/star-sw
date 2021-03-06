*
* $Id: erxyzc.F,v 1.1.1.2 2009/02/01 17:10:15 fisyak Exp $
*
* $Log: erxyzc.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:15  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:46:29  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:23  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.2  2003/11/28 11:23:55  brun
* New version of geant321 with all geant3 routines renamed from G to G3
*
* Revision 1.1.1.1  2002/07/24 15:56:26  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:36  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:15  fca
* AliRoot sources
*
* Revision 1.1.1.1  1996/03/06 15:37:35  mclareni
* Add geane321 source directories
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.49  by  S.Giani
*-- Author :
      SUBROUTINE ERXYZC
C.
C.    ******************************************************************
C.    *                                                                *
C.    *        Print track and volume parameters at current point      *
C.    *                                                                *
C.    *    ==>Called by : <USER,EUSTEP>                                *
C.    *       Author    R.Brun  *********                              *
C.    *                                                                *
C.    ******************************************************************
C.
#include "geant321/gckine.inc"
#include "geant321/gctrak.inc"
#include "geant321/gcvolu.inc"
#include "geant321/gcunit.inc"
#include "geant321/ertrio.inc"
*
      DIMENSION MECNAM(20)
      CHARACTER*4 KUN1,KUN2
*
      DO 10 I=1,NMEC
         MEC=LMEC(I)
         MECNAM(I)=NAMEC(MEC)
   10 CONTINUE
*
      IF (SLENG.LE.0.) THEN
         WRITE(CHMAIL,1000) (NAPART(I),I=1,5),NEPRED,CHOPTI
         CALL GMAIL(0,0)
         WRITE(CHMAIL,1100)
         CALL GMAIL(0,0)
      ENDIF
*
      R = SQRT(VECT(1)**2+VECT(2)**2)
      CALL G3EVKEV (DESTEP,DESU,KUN1)
      CALL G3EVKEV (GEKIN ,GEKU,KUN2)
      WRITE(CHMAIL,2000)(VECT(I),I=1,3),R,NAMES(NLEVEL),NUMBER(NLEVEL)
     +      ,SLENG,STEP,DESU,KUN1,GEKU,KUN2,(MECNAM(I),I=1,NMEC)
      CALL GMAIL(0,0)
C
 1000 FORMAT(' =====> starting GEANE tracking for  ',5A4,' NEPRED =',I3,
     +       '  with options : ',A8)
 1100 FORMAT('      X         Y         Z         R      NAME  NUMBER',
     +'   SLENG      STEP      DESTEP     GEKIN    MECHANISMS')
 2000 FORMAT(1X,4F10.4,2X,A4,2X,I4,2X,2F10.4,F7.1,A4,F9.3,A4,2X,
     +          6(A4,1X))
*
      END
