//////////////////////////////////////////////////////////////////////
//
// $Id: StFlowTrackCollection.h,v 1.5 2000/05/26 21:29:35 posk Exp $
//
// Author: Raimond Snellings and Art Poskanzer
//////////////////////////////////////////////////////////////////////
//
// Description: part of StFlowEvent 
//   The Collection of Tracks is the main component of StFlowEvent
//
//////////////////////////////////////////////////////////////////////
//
// $Log: StFlowTrackCollection.h,v $
// Revision 1.5  2000/05/26 21:29:35  posk
// Protected Track data members from overflow.
//
// Revision 1.4  2000/05/12 22:42:05  snelling
// Additions for persistency and minor fix
//
// Revision 1.3  2000/05/11 20:00:40  posk
// Preparation for micro and nano DSTs.
//
// Revision 1.2  2000/03/15 23:28:55  posk
// Added StFlowSelection.
//
// Revision 1.1  2000/03/02 23:03:00  posk
// Changed extensions from .hh and .cc to .h and .cxx .
//
// Revision 1.4  2000/01/31 22:17:00  posk
// CC5 compliant.
//
// Revision 1.2  1999/11/30 18:52:56  snelling
// First modification for the new StEvent
//
// Revision 1.1  1999/11/11 23:09:00  posk
// Rearrangement of files.
//
// Revision 1.1  1999/11/04 19:02:09  snelling
// First check in of StFlowMaker. It contains the common code from
// StFlowTagMaker and StFlowAnalysisMaker.
//
//
//////////////////////////////////////////////////////////////////////

#ifndef StFlowTrackCollection_h
#define StFlowTrackCollection_h
#include "StArray.h"
#include "StFlowTrack.h"
//#include <vector>

//#if !defined(ST_NO_NAMESPACES)
//using std::vector;
//#endif

//typedef vector<StFlowTrack*>            StFlowTrackCollection;
//typedef vector<StFlowTrack*>::iterator  StFlowTrackIterator;
StCollectionDef(FlowTrack)
typedef StSPtrVecFlowTrack StFlowTrackCollection;

//typedef StFlowTrackIterator StFlowTrackIterator;

#endif

