*
* $Id: chkzeb.F,v 1.1.1.2 2009/02/01 17:10:34 fisyak Exp $
*
* $Log: chkzeb.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:34  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:49:00  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:57  fisyak
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
* Revision 1.1.1.1  1995/10/24 10:21:55  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.48  by  S.Giani
*-- Author :
      SUBROUTINE CHKZEB(NW,IX)
C
C Check if NW words are available in ZEBRA division IX
C
C ZEBRA user communication common
      COMMON/ QUEST / IQUEST(100)
C
      CALL MZNEED(IX,NW,'G')
      IF(IQUEST(11).LT.0) THEN
         PRINT *,'******************************************'
         PRINT *,'*            G C A L O R                 *'
         PRINT *,'*   NOT enough space available in ZEBRA  *'
         PRINT '('' *  division '',I3,'' to store '',I8,               '
     +   //'            '' words  *'')',IX,NW
         PRINT *,'*                                        *'
         PRINT *,'*  INCREASE ZEBRA COMMON SIZE AND RERUN  *'
         PRINT *,'*                                        *'
         PRINT *,'*             RUN TERMINATED             *'
         PRINT *,'******************************************'
         STOP
      ENDIF
      RETURN
      END
