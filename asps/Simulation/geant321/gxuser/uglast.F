*
* $Id: uglast.F,v 1.1.1.1 2004/01/15 00:12:18 potekhin Exp $
*
* $Log: uglast.F,v $
* Revision 1.1.1.1  2004/01/15 00:12:18  potekhin
*
* Revision 1.1.1.1  1997/11/03 15:30:51  atlascvs
* Importing CERNLIB version 08.21.
*
* Revision 1.1.1.1  1995/10/24 10:21:52  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.35  by  S.Giani
*-- Author :
      SUBROUTINE UGLAST
*
************************************************************************
*                                                                      *
*            Termination routine to print histograms and statistics    *
*                                                                      *
************************************************************************
#include "geant321/gcomis.inc"
*
*     -----------------------------------------------------------------
*
#if defined(CERNLIB_COMIS)
      IF(JULAST.NE.0)THEN
         CALL CSJCAL(JULAST,0,X,X,X,X,X,X,X,X,X,X)
         GO TO 99
      ENDIF
#endif
      CALL GLAST
*
*             Close HIGZ
*
      CALL IGEND
*
  99  END
 
