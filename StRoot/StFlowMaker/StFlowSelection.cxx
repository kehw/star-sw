////////////////////////////////////////////////////////////////////////////
//
// $Id: StFlowSelection.cxx,v 1.4 2000/05/26 21:29:32 posk Exp $
//
// Author: Art Poskanzer and Raimond Snellings, LBNL, Mar 2000
//
// Description:  Class for making selections
//
////////////////////////////////////////////////////////////////////////////
//
// $Log: StFlowSelection.cxx,v $
// Revision 1.4  2000/05/26 21:29:32  posk
// Protected Track data members from overflow.
//
// Revision 1.3  2000/05/11 20:00:38  posk
// Preparation for micro and nano DSTs.
//
// Revision 1.2  2000/03/28 23:21:04  posk
// Allow multiple instances of the AnalysisMaker.
//
// Revision 1.1  2000/03/15 23:28:53  posk
// Added StFlowSelection.
//
////////////////////////////////////////////////////////////////////////////

#include <iostream.h>
#include <stdlib.h>
#include "StFlowSelection.h"
#include "StFlowEvent.h"
#include "StFlowTrack.h"
#define PR(x) cout << "##### FlowSelection: " << (#x) << " = " << (x) << endl;

ClassImp(StFlowSelection)

//-----------------------------------------------------------------------


StFlowSelection::StFlowSelection() : mCentrality(0), mSubevent(-1) {
  // To make selections
  mNumber[0] = '\0';
  mPid[0] = '\0';
  mPidPart[0] = '\0';
}

//-----------------------------------------------------------------------

StFlowSelection::~StFlowSelection() {
}

//-----------------------------------------------------------------------

Bool_t StFlowSelection::Select(StFlowEvent* pFlowEvent) {
  // Returns kTRUE if the event is selected
  
  // Centrality
  if (mCentrality) {
    UInt_t cent = pFlowEvent->Centrality();
    if (mCentrality != cent) return kFALSE;
  }

  return kTRUE;
}

//-----------------------------------------------------------------------

Bool_t StFlowSelection::Select(StFlowTrack* pFlowTrack) {
  // Selects particles for event plane determination
  // Returns kTRUE if the track is selected

  // PID
  if (mPid[0] != '\0') {
    const Char_t* pid = pFlowTrack->Pid();
    if (strstr(pid, mPid)==0) return kFALSE;
  }
  
  // Selected for event plane
  if (!pFlowTrack->Select(mHarmonic, mSelection, mSubevent)) return kFALSE;
  
  return kTRUE;
}

//-----------------------------------------------------------------------

Bool_t StFlowSelection::SelectPart(StFlowTrack* pFlowTrack) {
  // Selects particles for correlation with the event plane
  // Returns kTRUE if the track is selected

  // PID
  if (mPidPart[0] != '\0') {
    const Char_t* pid = pFlowTrack->Pid();
    if (strstr(pid, mPidPart)==0) return kFALSE;
  }
  
  return kTRUE;
}

//-----------------------------------------------------------------------
