*
* $Id: qtofatal.inc,v 1.1.1.1 2004/07/17 20:01:58 perev Exp $
*
* $Log: qtofatal.inc,v $
* Revision 1.1.1.1  2004/07/17 20:01:58  perev
* STAR version of Geant321 TGeant3 etc
*
* Revision 1.1.1.1  2002/07/24 15:56:28  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:50  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:27  fca
* AliRoot sources
*
* Revision 1.1.1.1  1996/03/06 10:46:54  mclareni
* Zebra
*
*
#ifndef CERNLIB_ZEBRA_QTOFATAL_INC
#define CERNLIB_ZEBRA_QTOFATAL_INC
*
*
* qtofatal.inc
*
#if (defined(CERNLIB_QTRHOLL))&&(defined(CERNLIB_A6M))
      IQUEST(10)= NAMESR
      CALL ZFATAL
#elif defined(CERNLIB_QTRHOLL)
      IQUEST(9) = NAMESR(1)
      IQUEST(10)= NAMESR(2)
      CALL ZFATAL
#elif defined(CERNLIB_A6M)
      CALL UCTOH (NAMESR,IQUEST(10),6,6)
      CALL ZFATAL
#elif 1 
      CALL UCTOH (NAMESR,IQUEST(9),4,8)
      CALL ZFATAL
*  -------------   for ZEBRA FZ   ------------------------------

#endif
#endif
