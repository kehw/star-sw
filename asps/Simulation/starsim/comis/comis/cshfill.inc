*
* $Id: cshfill.inc,v 1.1 2018/11/19 20:43:51 perev Exp $
*
* $Log: cshfill.inc,v $
* Revision 1.1  2018/11/19 20:43:51  perev
* 64bits add files
*
* Revision 1.1.1.1  1996/02/26 17:16:36  mclareni
* Comis
*
*
#ifndef CERNLIB_COMIS_CSHFILL_INC
#define CERNLIB_COMIS_CSHFILL_INC
*
*
* cshfill.inc
*
      PARAMETER (KJMPHFI=200, KNMSHFI=5)
      CHARACTER *8 HFINMS
      COMMON/CSHFIJ/JMPHFI(KJMPHFI)
      COMMON/CSHFII/MODHFI,NCLHFI,JADHFI(KNMSHFI),NARGHFI(KNMSHFI)
      COMMON/CSHFIN/HFINMS(KNMSHFI)

#endif
