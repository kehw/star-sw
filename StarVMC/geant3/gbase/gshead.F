*
* $Id: gshead.F,v 1.1.1.2 2009/02/01 17:10:19 fisyak Exp $
*
* $Log: gshead.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:19  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:46:59  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:25  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.2  2003/11/28 11:23:55  brun
* New version of geant321 with all geant3 routines renamed from G to G3
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
* Revision 1.1.1.1  1995/10/24 10:20:11  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.19  by  S.Giani
*-- Author :
      SUBROUTINE G3SHEAD (NWBUF,UBUF,IADR)
C.
C.    ******************************************************************
C.    *                                                                *
C     *       Stores User's informations in HEADER user bank.          *
C     *       IADR is the stating location in the bank                 *
C     *                                                                *
C     *       on output IADR is the new starting location              *
C.    *                                                                *
C.    *    ==>Called by : <USER>                                       *
C.    *       Author     M.Maire                                       *
C.    *                                                                *
C.    ******************************************************************
C.
#include "geant321/gcbank.inc"
#include "geant321/gcunit.inc"
#include "geant321/gcflag.inc"
*
      DIMENSION UBUF(1)
*
      IF (JHEAD.GT.0) THEN
*
         NDATA = IADR + NWBUF
         IF (LQ(JHEAD-1).EQ.0) THEN
            CALL MZBOOK (IXDIV,JHU,JHEAD,-1,'HEDU',0,0,NDATA,3,0)
         ELSE
            JHU = LQ(JHEAD-1)
            NFREE = IQ(JHU-1) - NDATA
            IF (NFREE.LT.0) CALL MZPUSH (IXDIV,JHU,0,-NFREE,'I')
         ENDIF
*
         DO  10 I=1,NWBUF
            Q(JHU+IADR+I) = UBUF(I)
   10    CONTINUE
*
         IADR = IADR + NWBUF
      ELSE
*
         IADR = 0
         WRITE(CHMAIL,1000) IEVENT
         CALL GMAIL(0,0)
      ENDIF
*
 1000 FORMAT(' **** GSHEAD: No Header bank in memory for ievent:',I7)
*
      END
