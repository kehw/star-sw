/***************************************************************************
 *
 * $Id: StVecVpdCounter.hh,v 1.2 1999/01/15 22:54:21 wenaus Exp $
 *
 * Author: Thomas Ullrich, Jan 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StVecVpdCounter.hh,v $
 * Revision 1.2  1999/01/15 22:54:21  wenaus
 * version with constructors for table-based loading
 *
 * Revision 1.3  1999/03/04 15:57:10  wenaus
 * add std namespace for Sun CC5 compatibility
 *

 * version with constructors for table-based loading
 *
#define StVecVpdCounter_hh
#include <vector>
#if !defined(ST_NO_NAMESPACES)
using namespace std;
#endif
class StVpdCounter;
#ifdef ST_NO_TEMPLATE_DEF_ARGS
typedef vector<StVpdCounter, allocator<StVpdCounter> > StVecVpdCounter;
#else
typedef vector<StVpdCounter> StVecVpdCounter;
#endif

#endif
