* $Id: cqlpha.F,v 1.1.1.1 2004/01/15 00:08:49 potekhin Exp $
* $Name:  $
#include <commons/config.h>
      SUBROUTINE CQLPHA
      IMPLICIT REAL *8  (A-H,O-Z)
C
#include "geant321/crandm.inc"
#include "gcalor/cbert3.inc"
#include "gcalor/cisob2.inc"
      SAVE
C
      UNIV = RANDC(ISEED)
      IF(VALUE3)300,10,140
   10 IF(UNIV-VALUE1)20,20,120
   20 IF(ISW(11))40,30,40
   30 PT(2)=5.0
      PT(26)=2.0
   40 PT(4)=PNMS
      PM(4)=PNMS
      UNIV = RANDC(ISEED)
      IF(UNIV-VALUE2)50,50,70
   50 PT(14)=4.0
   60 RETURN
   70 IF(ISW(11))110,80,110
   80 PT(26)=1.0
   90 PT(14)=5.0
  100 PT(16)=PNMS
      GOTO60
  110 PT(26)=2.0
      GOTO100
  120 PT(2)=4.0
      IF(ISW(11))100,130,100
  130 PT(14)=5.0
      GOTO110
  140 IF(UNIV-VALUE1)150,150,200
  150 PM(4)=PNMS
      IF(ISW(11))160,190,160
  160 PT(2)=5.0
  170 PT(16)=PNMS
  180 PT(4)=PNMS
      GOTO60
  190 PT(14)=5.0
      PT(26)=2.0
      GOTO170
  200 IF(UNIV-VALUE2)210,210,250
  210 PT(2)=4.0
      UNIV = RANDC(ISEED)
      IF(UNIV-VALUE3)240,240,220
  220 IF(ISW(11))50,230,50
  230 PT(26)=2.0
      GOTO50
  240 IF(ISW(11))110,90,110
  250 PM(4)=PNMS
      PT(4)=PNMS
      UNIV = RANDC(ISEED)
      IF(UNIV-.66666667)260,260,280
  260 IF(ISW(11))230,270,230
  270 PT(2)=5.0
      GOTO50
  280 IF(ISW(11))90,290,90
  290 PT(26)=2.0
      GOTO160
  300 IF(UNIV-VALUE1)310,310,340
  310 PM(4)=PNMS
      PT(4)=PNMS
      UNIV = RANDC(ISEED)
      IF(VALUE3+1.0)330,320,330
  320 IF(UNIV-.33333333)90,90,230
  330 PT(2)=5.0
      IF(UNIV-.33333333)110,110,50
  340 IF(UNIV-VALUE2)350,350,380
  350 PT(2)=4.0
      UNIV = RANDC(ISEED)
      IF(VALUE3+1.0)370,360,370
  360 IF(UNIV-.66666667)50,50,110
  370 IF(UNIV-.66666667)230,230,90
  380 PM(4)=PNMS
      PT(4)=PNMS
      IF(VALUE3+1.0)390,160,390
  390 PT(14)=5.0
      GOTO110
      END
