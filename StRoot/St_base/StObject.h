// $Id: StObject.h,v 1.10 2000/09/30 17:48:27 perev Exp $
// $Log: StObject.h,v $
// Revision 1.10  2000/09/30 17:48:27  perev
// Zombies cons and loop for stru vector
//
// Revision 1.9  2000/09/15 15:11:58  perev
// Zombie for StEvent
//
// Revision 1.8  2000/07/30 01:49:03  perev
// StObject vers restored
//
// Revision 1.6  2000/04/20 14:24:09  perev
// StArray fixes
//
// Revision 1.5  2000/04/18 02:57:26  perev
// StEvent browse
//
// Revision 1.4  1999/11/15 23:09:10  perev
// Streamer for StrArray and auto remove
//
// Revision 1.3  1999/10/30 19:36:02  perev
// Added clone() to StObject
//
// Revision 1.2  1999/06/23 20:31:04  perev
// StArray I/O + browser
//
// Revision 1.1  1999/04/30 13:15:56  fisyak
// Ad StObject, modification StArray for StRootEvent
//

#ifndef STAR_StObject
#define STAR_StObject
 
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// StObject class is a base class to implement StEvent                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObject.h"
class StObject : public TObject {
  public:
  virtual ~StObject();
  virtual void Browse(TBrowser *b);
  virtual Bool_t IsFolder() const;
  virtual TObject *clone() const {return 0;}
  Int_t   isZombie() const {return IsZombie();}
  virtual void makeZombie(){MakeZombie();}
  ClassDef(StObject,2) // Base class for StEvent
};
#endif
