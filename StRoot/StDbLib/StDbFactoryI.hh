/***************************************************************************
 *
 * $Id: StDbFactoryI.hh,v 1.7 2000/01/27 05:54:33 porter Exp $
 *
 * Author: R. Jeff Porter
 ***************************************************************************
 *
 * Description:  StDbTable ctor & list holder for generic tables
 *
 ***************************************************************************
 *
 * $Log: StDbFactoryI.hh,v $
 * Revision 1.7  2000/01/27 05:54:33  porter
 * Updated for compiling on CC5 + HPUX-aCC + KCC (when flags are reset)
 * Fixed reConnect()+transaction model mismatch
 * added some in-code comments
 *
 * Revision 1.6  2000/01/10 20:37:53  porter
 * expanded functionality based on planned additions or feedback from Online work.
 * update includes:
 * 	1. basis for real transaction model with roll-back
 * 	2. limited SQL access via the manager for run-log & tagDb
 * 	3. balance obtained between enumerated & string access to databases
 * 	4. 3-levels of diagnostic output: Quiet, Normal, Verbose
 * 	5. restructured Node model for better XML support
 *
 * Revision 1.5  1999/12/28 21:31:42  porter
 * added 'using std::vector' and 'using std::list' for Solaris CC5 compilation.
 * Also fixed some warnings arising from the CC5 compiles
 *
 * Revision 1.4  1999/10/19 14:30:38  porter
 * modifications relevant to use with StDbBroker and future merging with
 * "params" database structure + some docs + suppressing diagnostics messages
 *
 * Revision 1.3  1999/09/30 02:06:05  porter
 * add StDbTime to better handle timestamps, modify SQL content (mysqlAccessor)
 * allow multiple rows (StDbTable), & Added the comment sections at top of
 * each header and src file
 *
 **************************************************************************/
#ifndef STDBFACTORYI_HH
#define STDBFACTORYI_HH

#include "StDbDefs.hh"
#include "StDbTableID.h"
#include <fstream.h>

class StDbTable;



#include <list>

#ifdef ST_NO_TEMPLATE_DEF_ARGS
typedef list<StDbTableID*, allocator<StDbTableID*> > IDList;
#else
#if !defined(ST_NO_NAMESPACES)
using std::list;
#endif
typedef list<StDbTableID*> IDList;
#endif


class StDbFactoryI {

protected:

  bool isloaded;
  StDbType mdbType;
  IDList mIDList;

  virtual void initIDList(); // Here's all the work in derived classes

public:

  StDbFactoryI() : isloaded(false), mdbType(dbStDb) {};
  virtual ~StDbFactoryI(){ deleteIDList();};
  virtual StDbType getDbType() { return mdbType; };

  virtual int findTableID(const char* tableName);
  virtual StDbTable* getDbTable(const char* tableName, int option);
  virtual void deleteIDList();
  virtual void readIDList(ifstream& is);

};

inline void
StDbFactoryI::initIDList() {
isloaded=true;
// cerr<< "StDbFactoryI:: Has No default Tables in Base Class"<< endl;
}


#endif







