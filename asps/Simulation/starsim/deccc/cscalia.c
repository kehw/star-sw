/*
 * $Id: cscalia.c,v 1.1.2.1 2020/08/07 02:44:28 perev Exp $
 *
 * $Log: cscalia.c,v $
 * Revision 1.1.2.1  2020/08/07 02:44:28  perev
 * Get pointer
 *
 * Revision 1.1  2018/11/19 23:20:11  perev
 * 64bits new comis files added from /CERN
 *
 * Revision 1.1.1.1  1996/02/26 17:16:55  mclareni
 * Comis
 *
 */
#if 0
#include "comis/pilot.h"
#if defined(CERNLIB_ALPHA_OSF)
/*CMZ :  1.17/03 18/11/93  16.51.12  by  Vladimir Berezhnoi*/
/*-- Author :*/

#define cscali
#undef  cscali

#ifdef __osf__

#ifdef JMP_DEBUG
#define PRINTF(x)       printf x
#else
#define PRINTF(x)
#endif

/*
 * Note from Antonio Pastore - DJP
 *
 * This package HAS to be LINKED with the following options:
 *
 *	f77 -D 4000000 -T 2000000
 *
 */
unsigned long  csPoter( int token);
cscali_ (
	int *fptr,	/* Contains the address of target routine 	*/
	int *n,		/* number of aguments				*/
	unsigned int pp[])	/* array of pointers!				*/
{
	ptr = csPoter(*fptr);
	name = (int (*)())ptr;
        unsigned long p[15]={0};
        for (int i=0;i<*n;i++) {
          if (!pp[i]) continue;
          p[i] = getPntB(pp[i]);
	}
	switch (*n)
	{
	case 0:
		return((*name)());
	case 1:
		return((*name)(p[0]));
	case 2:
		return((*name)(p[0],p[1]));
	case 3:
		return((*name)(p[0],p[1],p[2]));
	case 4:
		return((*name)(p[0],p[1],p[2],p[3]));
	case 5:
		return((*name)(p[0],p[1],p[2],p[3],p[4]));
	case 6:
		return((*name)(p[0],p[1],p[2],p[3],p[4],p[5]));
	case 7:
		return((*name)(p[0],p[1],p[2],p[3],p[4],p[5],p[6]));
	case 8:
		return((*name)(p[0],p[1],p[2],p[3],p[4],p[5],p[6],
			p[7]));
	case 9:
		return((*name)(p[0],p[1],p[2],p[3],p[4],p[5],p[6],
			p[7],p[8]));
	case 10:
		return((*name)(p[0],p[1],p[2],p[3],p[4],p[5],p[6],
			p[7],p[8],p[9]));
	case 11:
		return((*name)(p[0],p[1],p[2],p[3],p[4],p[5],p[6],
			p[7],p[8],p[9],p[10]));
	case 12:
		return((*name)(p[0],p[1],p[2],p[3],p[4],p[5],p[6],
			p[7],p[8],p[9],p[10],p[11]));
	case 13:
		return((*name)(p[0],p[1],p[2],p[3],p[4],p[5],p[6],
			p[7],p[8],p[9],p[10],p[11],p[12]));
	case 14:
		return((*name)(p[0],p[1],p[2],p[3],p[4],p[5],p[6],
			p[7],p[8],p[9],p[10],p[11],p[12],p[13]));
	case 15:
		return((*name)(p[0],p[1],p[2],p[3],p[4],p[5],p[6],
			p[7],p[8],p[9],p[10],p[11],p[12],p[13],
			p[14]));
	case 16:
		return((*name)(p[0],p[1],p[2],p[3],p[4],p[5],p[6],
			p[7],p[8],p[9],p[10],p[11],p[12],p[13],
			p[14],p[15]));
	default:
		printf("\n More then 16 arguments in call users routine");
	}
}
#endif
#endif
#endif //0
