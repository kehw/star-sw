*
* $Id: mzlint.F,v 1.1.1.1 2004/01/12 23:49:39 potekhin Exp $
*
* $Log: mzlint.F,v $
* Revision 1.1.1.1  2004/01/12 23:49:39  potekhin
*
* Revision 1.1  2001/02/27 10:15:26  nevski
*  first working release
*
* Revision 1.6  2000/01/06 02:42:36  nevski
* elastic zebra updated
*
* Revision 1.5  1999/10/18 16:46:01  nevski
* pgf->g77
*
* Revision 1.4  1998/06/05 20:55:36  perev
* AGI commit
*
* Revision 1.3  1998/04/16 16:58:06  fisyak
* 2nd pass with gstar
*
*
#include "zebra/pilot.h"
*CMZ :          21/11/97  17.09.55  by  Pavel Nevski
*-- Author :
      SUBROUTINE MZLINT (IXSTOR,CHNAME,LAREA,LREF,LREFL)

C-    Set permanent link area, user called

#include "zebra/zbcd.inc"
#include "zebra/zstate.inc"
#include "zebra/zunit.inc"
#include "zebra/mqsys.inc"
C--------------    End CDE                             --------------
      DIMENSION    LAREA(9),LREF(9),LREFL(9),NAME(2)
      CHARACTER    *(*) CHNAME
#if (defined(CERNLIB_QTRHOLL))&&(!defined(CERNLIB_A6M))
      DIMENSION    NAMESR(2)
      DATA  NAMESR / 4HMZLI, 4HNT   /
#endif
#if (defined(CERNLIB_QTRHOLL))&&(defined(CERNLIB_A6M))
      DATA  NAMESR / 6HMZLINT /
#endif
#if !defined(CERNLIB_QTRHOLL)
      CHARACTER    NAMESR*8
      PARAMETER   (NAMESR = 'MZLINT  ')
#endif

#include "zebra/q_jbit.inc"
#include "zebra/q_sbit.inc"
#include "zebra/q_locf.inc"


#include "zebra/qtrace.inc"

#include "zebra/qstore.inc"
#if defined(CERNLIB_QDEBUG)
      IF (IQVSTA.NE.0)       CALL ZVAUTX
#endif

      LSYS  = LQSYSS(KQT+1)
      NWTAB = IQ(KQS+LSYS+1)
      LOCAR = LOCF (LAREA(1)) - LQSTOR
      JDES = LAREA(2)
      IF (JDES.LT.11)              GO TO 21
      IF (JDES.GE.NWTAB)           GO TO 21
      LSTO = LSYS + JDES
      IF (IQ(KQS+LSTO+1).NE.LOCAR)    GO TO 21
      NL = IQ(KQS+LSTO+2) - LOCAR
#if defined(CERNLIB_QDEBPRI)
      IF (NQLOGL.LT.2)             GO TO 19
      WRITE (IQLOG,9018) IQ(KQS+LSTO+4),IQ(KQS+LSTO+5),JQSTOR
 9018 FORMAT (1X/' MZLINT-  Re-Init of Link Area ',2A4,'  for Store',I3)
#endif

C--                Set link area active

   19 LAREA(1) = 7
      CALL VZERO (LAREA(3),NL-2)
#include "zebra/qtrace99.inc"
      RETURN

C------            Initialize for the first time

   21 IF (NWTAB+5.GT.IQ(KQS+LSYS-1))  THEN
          JQDIVI = JQDVSY
          CALL MZPUSH (-7,LSYS,0,100,'I')
          LQSYSS(KQT+1) = LSYS
        ENDIF

      LSTO = LSYS + NWTAB
      LOCR  = LOCF (LREF(1))  - LQSTOR
      LOCRL = LOCF (LREFL(1)) - LQSTOR
      NS = LOCR    - LOCAR
      NL = LOCRL+1 - LOCAR
      IF (NL.EQ.1)  THEN
          NS = NS + 1
          NL = NS
        ENDIF

      LOCARE = LOCAR + NL
      MODAR  = MSBIT1 (NS,31)

      NAME(1) = IQBLAN
      NAME(2) = IQBLAN
      N = MIN (8, LEN(CHNAME))
      IF (N.NE.0)  CALL UCTOH (CHNAME,NAME,4,N)

      IQ(KQS+LSTO+1) = LOCAR
      IQ(KQS+LSTO+2) = LOCARE
      IQ(KQS+LSTO+3) = MODAR
      IQ(KQS+LSTO+4) = NAME(1)
      IQ(KQS+LSTO+5) = NAME(2)

C--                Range of possible values for an origin-link

      IQTABV(KQT+13) = MIN (IQTABV(KQT+13), LOCAR+2)
      IQTABV(KQT+14) = MAX (IQTABV(KQT+14), LOCARE)

      NSM2 = NS - 2
      NLM2 = NL - 2
#if defined(CERNLIB_QPRINT)
      IF (NQLOGL.GE.1)
     +WRITE (IQLOG,9039) NAME,JQSTOR,NLM2,NSM2
 9039 FORMAT (1X/' MZLINT.  Initialize Link Area  ',2A4,'  for Store'
     F,I3,' NL/NS=',2I6)

#endif
#if defined(CERNLIB_QDEBUG)

C----              Check valid parameters

      IF (LOCR .LT.LOCAR)          GO TO 91
      IF (LOCRL.LT.LOCAR)          GO TO 91
      IF (NL.LT.NS)                GO TO 91

C------            Check overlap with existing stores

      KLA = KQS + LOCAR
      KLE = KQS + LOCARE

#endif
#if defined(CERNLIB_QDEVZE)
      IF (NQDEVZ.GE.7)
     +WRITE (IQLOG,9841) 4*LQSTOR, 4*LQATAB, 4*LQBTIS, 4*KLA
     +,           LQSTOR,LQATAB, LQBTIS,KLA
     +,           LQSTOR,LQATAB, LQBTIS,KLA
 9841 FORMAT (1X/' DEVZE MZLINT.  ',17X,'LQSTOR',17X,'LQATAB',
     F17X,'LQBTIS',20X,'KLA'
#endif
#if (defined(CERNLIB_QDEVZE))&&(!defined(CERNLIB_HEX))
     F/10X,'4* OCT',4O23/13X,'OCT',4O23/13X,'DEC',4I23)
#endif
#if (defined(CERNLIB_QDEVZE))&&(defined(CERNLIB_HEX))
     F/10X,'4* HEX',4Z23/13X,'HEX',4Z23/13X,'DEC',4I23)
#endif
#if defined(CERNLIB_QDEVZE)
      IF (NQDEVZ.GE.7)   WRITE (IQLOG,9842) KLA,KLE
 9842 FORMAT (16X,' KLA/KLE=',2I10)
#endif
#if defined(CERNLIB_QDEBUG)

      DO 47  JSTO=1,NQSTOR+1
      IF (NQALLO(JSTO).NE.0)       GO TO 47
      JT  = NQOFFT(JSTO)
      JS  = NQOFFS(JSTO)
      JSA = JS  - IQTABV(JT+2) + 1
      JSE = JS  + LQSTA(JT+21) + 1
      JTA = JT  + LQBTIS       + 1
      JTE = JTA + NQTSYS

#endif
#if defined(CERNLIB_QDEVZE)
      IF (NQDEVZ.GE.7)   WRITE (IQLOG,9843) JTA,JTE,  JSA,JSE
 9843 FORMAT (16X,' JTA/JTE=',2I10,'  JSA/JSE=',2I10)
#endif
#if defined(CERNLIB_QDEBUG)

      IF (KLE.GT.JTA .AND. KLA.LT.JTE)    GO TO 92
      IF (KLE.GT.JSA .AND. KLA.LT.JSE)    THEN
         IF (NQLOGL.GE.0) WRITE (IQLOG,9139) CHNAME,JSTO
 9139    FORMAT (' Link area ',a,' overlaps with store #',i3)
      ENDIF

C--                Check overlap with existing link areas

      L = JS+ LQSYSS(JT+1)
      N = IQ(L+1)
      IF (N.LT.12)                 GO TO 47

      DO 44  J=12,N,5
      JLA = JS + IQ(L+J)
      JLE = JS + IQ(L+J+1)

#endif
#if defined(CERNLIB_QDEVZE)
      IF (NQDEVZ.GE.7)   WRITE (IQLOG,9844) JLA,JLE
 9844 FORMAT (16X,' JLA/JLE=',2I10)
#endif
#if defined(CERNLIB_QDEBUG)

      IF (KLE.GT.JLA .AND. KLA.LT.JLE)    GO TO 94
   44 CONTINUE
   47 CONTINUE
#endif

C--                Success, register new link area

      LAREA(2) = NWTAB
      IQ(KQS+LSYS+1) = NWTAB + 5
      GO TO 19

C------            Error conditions

   94 NQCASE = 1
      NQFATA = 4
      IQUEST(21) = IQ(L+J+3)
      IQUEST(22) = IQ(L+J+4)
      IQUEST(23) = JLA + LQSTOR
      IQUEST(24) = LAREA(2)

   93 NQCASE = NQCASE + 1
   92 NQCASE = NQCASE + 1
      NQFATA = NQFATA + 3
      IQUEST(18) = JSTO - 1
      IQUEST(19) = NQPNAM(JT+1)
      IQUEST(20) = NQPNAM(JT+2)

   91 NQCASE = NQCASE + 1
      NQFATA = NQFATA + 7
      IQUEST(11) = NAME(1)
      IQUEST(12) = NAME(2)
      IQUEST(13) = LOCAR + LQSTOR
      IQUEST(14) = LOCR  + LQSTOR
      IQUEST(15) = LOCRL + LQSTOR
      IQUEST(16) = NSM2
      IQUEST(17) = NLM2
#include "zebra/qtofatal.inc"
*     prevent warning only
      L = MSBIT0 (NS,31)
      END
#include "zebra/qcardl.inc"
