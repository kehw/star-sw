*
* $Id: csfres.inc,v 1.1 2018/11/19 20:43:51 perev Exp $
*
* $Log: csfres.inc,v $
* Revision 1.1  2018/11/19 20:43:51  perev
* 64bits add files
*
* Revision 1.1.1.1  1996/02/26 17:16:35  mclareni
* Comis
*
*
#ifndef CERNLIB_COMIS_CSFRES_INC
#define CERNLIB_COMIS_CSFRES_INC
*
*
* csfres.inc
*
      DOUBLE PRECISION DCSRES
      COMPLEX          CXSRES
      COMMON/CSFRES/RCSRES,RCSPAD
      EQUIVALENCE (DCSRES,RCSRES),(DCSRES,ICSRES),(DCSRES,CXSRES)

#endif
