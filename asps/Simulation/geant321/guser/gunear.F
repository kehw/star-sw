*
* $Id: gunear.F,v 1.1.1.1 2004/01/15 00:12:17 potekhin Exp $
*
* $Log: gunear.F,v $
* Revision 1.1.1.1  2004/01/15 00:12:17  potekhin
*
* Revision 1.1.1.1  1997/11/03 15:30:49  atlascvs
* Importing CERNLIB version 08.21.
*
* Revision 1.1.1.1  1995/10/24 10:21:46  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.25  by  S.Giani
*-- Author :
      SUBROUTINE GUNEAR(ISEARC, ICALL, X, JNEAR)
C.
C.    ******************************************************************
C.    *                                                                *
C.    *    User search                                                 *
C.    *       ISEARC to identify the given volume                      *
C.    *       ICALL  to identify the calling routine                   *
C.    *              1 GMEDIA like                                     *
C.    *              2 GNEXT like                                      *
C.    *       X      coordinates (+direction for ICALL=2)              *
C.    *       JNEAR  address of default list of neighbours             *
C.    *              (list to be overwriten by user)                   *
C.    *                                                                *
C.    *    Called by : GFTRAC, GINVOL, GTMEDI, GTNEXT, GNEXT, GMEDIA   *
C.    *                                                                *
C.    ******************************************************************
C.
      DIMENSION X(*)
C.    ------------------------------------------------------------------
*                                                             END GUNEAR
      END
