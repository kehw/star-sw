*
* $Id: sub3.F,v 1.1.1.2 2009/02/01 17:10:28 fisyak Exp $
*
* $Log: sub3.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:28  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:48:12  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:37  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.1.1.1  2002/07/24 15:56:27  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:40  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:19  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:21:06  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.38  by  S.Giani
*-- Author :
C
C *** VARIOUS VECTOR OPERATIONS ***
C
C COPIED FROM F14BLO.PAMLIB 23.4.82
C AUTHOR: V.BLOBEL (UNIVERSITY OF HAMBURG)
C DESYLIB
C
C *** BLANK COMMON REPLACED BY /VECUTY/ TO MATCH GEANT/GHEISHA CODE ***
C *** NOTE THAT P(10,100) HAS BECOME PV(10,200) DUE TO THIS         ***
C
C UN-USED ENTRIES REMOVED :
C     "PCOP" "PEXC" "PZER" "PWRT" "DOT4" "IMPU" "IMPULI" "ADD3"
C     "SUB3" "CROSS" "DOT" "SMUL" "NORZ" "PARPER" "PUNIT" "TRAP"
C
C *** ALL ENTRIES RE-WRITTEN AS SUBROUTINES USING ONLY NECESSARY ***
C *** "DOUBLE PRECISION" STMTS. AND ALL SPECIFIC FUNCTIONS HAVE  ***
C *** BEEN CHANGED TO THEIR GENERIC EQUIVALENCES                 ***
C *** NVE 29-MAR-1988 CERN GENEVA ***
C
C ORIGIN : H.FESEFELDT (22-JUNE-1984)
C
      SUBROUTINE SUB3(K,L,M)
C
C --- PV-ARRAY ---
#include "geant321/mxgkgh.inc"
#include "geant321/s_blankp.inc"
C
      DOUBLE PRECISION A
C
      DO 2 I=1,3
      A=PV(I,K)-PV(I,L)
      PV(I,M)=A
    2 CONTINUE
      RETURN
      END
