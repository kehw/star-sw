/*
  Copyright(c) 2001, STAR  Experiment at BNL, All rights reserved.      
  
  Author: STAR Integrated Track Task Force                              
  Contributors are mentioned in the code where appropriate.
  
  Permission to use, copy, modify and distribute this software and its
  documentation strictly for non-commercial purposes is hereby granted 
  without fee, provided that the above copyright notice appears in all
  copies and that both the copyright notice and this permission notice
  appear in the supporting documentation. The authors make no claims 
  about the suitability of this software for any purpose. It is     
  provided "as is" without express or implied warranty.             
  
*/

/*
  \class StiKalmanTrackFinder  
  
  \author  Claude Pruneau, Wayne State University                        
  \date March 2001                                                    
  
  \note The Kalman Filter Code imbedded in this class was given
  to us gracioulsy by Jouri Belikov from the ALICE       
  collaboration. i.e. code reproduced with autorization. 
*/

//Std
#include <iostream.h>
#include <stdexcept>
#include <math.h>
using namespace std;

//Sti
#include "Parameter.h"
#include "EditableFilter.h"
#include "StiHitLoader.h"
#include "StiToolkit.h"
#include "StiKTNIterator.h"
#include "StiIOBroker.h"
#include "StiHit.h"
#include "StiDetector.h"
#include "StiPlacement.h"
#include "StiShape.h"
#include "StiPlanarShape.h"
#include "StiDetectorContainer.h"
#include "StiTrackContainer.h"
#include "StiTrack.h"
#include "StiTrackFinder.h"
#include "StiKalmanTrack.h"
#include "StiTrackSeedFinder.h"
#include "StiEvaluableTrackSeedFinder.h"
#include "StiCompositeSeedFinder.h"
#include "StiTrack.h"
#include "StiMcTrack.h"
#include "StiGui/StiRootDrawableMcTrack.h"
#include "StiKalmanTrackFinder.h"
#include "StiMaterialInteraction.h"
#include "StiTrackContainer.h"
#include "StiHitErrorCalculator.h"
#include "StiDrawableTrack.h"
#include "StMcEvent.hh"
#include "StMcTrack.hh"
#include "StMcContainers.hh"

ostream& operator<<(ostream&, const StiTrack&);

StiKalmanTrackFinder::StiKalmanTrackFinder(StiToolkit * userToolkit)
  : Observer(StiIOBroker::instance()),
    toolkit(0),
    trackFilter(0), 
    guiTrackFilter(0), 
    guiMcTrackFilter(0), 
    trackSeedFinder(0), 
    trackNodeFactory(0), 
    trackFactory(0), 
    mcTrackFactory(0), 
    detectorContainer(0), 
    hitContainer(0), 
    trackContainer(0),
    mcTrackContainer(0),
    pars(0),
    mode(StepByLayer),
    state(0),
    visitedDet(0),
    position(0),
    lastMove(0),
    chi2(0.),
    bestChi2(0.),
    track(0),
    sNode(0),
    tNode(0),
    bestNode(0),
    leadNode(0),
    sDet(0),
    tDet(0),
    leadDet(0),
    trackDone(true),
    scanningDone(true),
    hasHit(false),
    hasDet(false),
    trackMes(*Messenger::instance(MessageType::kTrackMessage))
{
  // none of the following instances are owned by this class.
  toolkit = userToolkit;
  StiIOBroker *  broker = toolkit->getIOBroker();
  Factory< Filter<StiTrack>  >* trackFilterFactory = toolkit->getTrackFilterFactory();
  trackFilter      = trackFilterFactory->getInstance();
  if (broker->useGui())
    {
      cout << "StiKalmanTrackFinder() - INFO - Instantiating gui filters" << endl;
      EditableFilter<StiTrack> * guiFilter;
      guiTrackFilter = trackFilterFactory->getInstance();
      guiFilter = static_cast< EditableFilter<StiTrack> * >(guiTrackFilter);
      guiFilter->setName("RecTrackFilter");
      guiFilter->add("Chi2Used", "Use Chi2",     false, false, 0,1,1,Parameter::Boolean, StiTrack::kChi2);
      guiFilter->add("Chi2Min",  "Minimum Chi2", 0., 0., 0., 100.,1,Parameter::Double, StiTrack::kChi2);
      guiFilter->add("Chi2Max",  "Maximum Chi2", 20., 20., 0., 100.,1,Parameter::Double, StiTrack::kChi2);
      
      guiFilter->add("PhiUsed",  "Use Phi",     false, false, 0,1,1,Parameter::Boolean, StiTrack::kPhi);
      guiFilter->add("PhiMin",   "Minimum Phi", 0.,   0.,  0., 6.3,2,Parameter::Double, StiTrack::kPhi);
      guiFilter->add("PhiMax",   "Maximum Phi", 6.3, 6.3, 0., 6.3,2,Parameter::Double, StiTrack::kPhi);
      
      guiFilter->add("PtUsed",   "Use Pt",     false, false, 0,1,1,Parameter::Boolean, StiTrack::kPt);
      guiFilter->add("PtMin",    "Minimum Pt", 0., 0., 0., 100.,2,Parameter::Double, StiTrack::kPt);
      guiFilter->add("PtMax",    "Maximum Pt", 10., 10., 0., 100.,2,Parameter::Double, StiTrack::kPt);
      
      guiFilter->add("PUsed",    "Use P",     false, false, 0,1,1,Parameter::Boolean, StiTrack::kP);
      guiFilter->add("PMin",     "Minimum P", 0., 0., 0., 100.,2,Parameter::Double, StiTrack::kP);
      guiFilter->add("PMax",     "Maximum P", 10., 10., 0., 100.,2,Parameter::Double, StiTrack::kP);
      
      guiFilter->add("EtaUsed",  "Use Eta",     false, false, 0,1,1,Parameter::Boolean, StiTrack::kPseudoRapidity);
      guiFilter->add("EtaMin",   "Minimum Eta", -1.5, -1.5, -10., 10.,2,Parameter::Double, StiTrack::kPseudoRapidity);
      guiFilter->add("EtaMax",   "Maximum Eta",  1.5,  1.5, -10., 10.,2,Parameter::Double, StiTrack::kPseudoRapidity);
      
      guiFilter->add("nPtsUsed", "Use nPts",     false, false, 0,1,1,Parameter::Boolean, StiTrack::kPointCount);
      guiFilter->add("nPtsMin",  "Minimum nPts", 0., 0., 0., 100.,1,Parameter::Integer, StiTrack::kPointCount);
      guiFilter->add("nPtsMax",  "Maximum nPts", 60., 60., 0., 100.,1,Parameter::Integer, StiTrack::kPointCount);
      
      guiFilter->add("nGapsUsed","Use nGaps",     false, false, 0,1,1,Parameter::Boolean, StiTrack::kGapCount);
      guiFilter->add("nGapsMin", "Minimum nGaps", 0., 0., 0., 100.,1,Parameter::Integer, StiTrack::kGapCount);
      guiFilter->add("nGapsMax", "Maximum nGaps", 60., 60., 0., 100.,1,Parameter::Integer, StiTrack::kGapCount);
      
      guiFilter->add("chargeUsed","Use Charge",     false, false, 0,1,1,Parameter::Boolean, StiTrack::kCharge);
      guiFilter->add("chargeMin", "Minimum Charge", -1., -1., -100.,   100.,1,Parameter::Integer, StiTrack::kCharge);
      guiFilter->add("chargeMax", "Maximum Charge",  1.,  1., -100.,   100.,1,Parameter::Integer, StiTrack::kCharge);
      Observer * obs = dynamic_cast<Observer *>(this);
      dynamic_cast<Subject*>(guiTrackFilter)->attach(obs);

      guiMcTrackFilter = trackFilterFactory->getInstance();
      guiFilter = static_cast<EditableFilter<StiTrack> * >(guiMcTrackFilter);
      guiFilter->setName("McTrackFilter");
      guiFilter->add("PhiUsed",  "Use Phi",     false, false, 0,1,1,Parameter::Boolean, StiTrack::kPhi);
      guiFilter->add("PhiMin",   "Minimum Phi", 0.,   0.,  0., 6.3,2,Parameter::Double, StiTrack::kPhi);
      guiFilter->add("PhiMax",   "Maximum Phi", 6.3, 6.3, 0., 6.3,2,Parameter::Double, StiTrack::kPhi);
      guiFilter->add("PtUsed",   "Use Pt",     false, false, 0,1,1,Parameter::Boolean, StiTrack::kPt);
      guiFilter->add("PtMin",    "Minimum Pt", 0., 0., 0., 100.,2,Parameter::Double, StiTrack::kPt);
      guiFilter->add("PtMax",    "Maximum Pt", 10., 10., 0., 100.,2,Parameter::Double, StiTrack::kPt);
      guiFilter->add("PUsed",    "Use P",     false, false, 0,1,1,Parameter::Boolean, StiTrack::kP);
      guiFilter->add("PMin",     "Minimum P", 0., 0., 0., 100.,2,Parameter::Double, StiTrack::kP);
      guiFilter->add("PMax",     "Maximum P", 10., 10., 0., 100.,2,Parameter::Double, StiTrack::kP);
      guiFilter->add("EtaUsed",  "Use Eta",     false, false, 0,1,1,Parameter::Boolean, StiTrack::kPseudoRapidity);
      guiFilter->add("EtaMin",   "Minimum Eta", -1.5, -1.5, -10., 10.,2,Parameter::Double, StiTrack::kPseudoRapidity);
      guiFilter->add("EtaMax",   "Maximum Eta",  1.5,  1.5, -10., 10.,2,Parameter::Double, StiTrack::kPseudoRapidity);
      guiFilter->add("nPtsUsed", "Use nPts",     false, false, 0,1,1,Parameter::Boolean, StiTrack::kPointCount);
      guiFilter->add("nPtsMin",  "Minimum nPts", 0., 0., 0., 100.,1,Parameter::Integer, StiTrack::kPointCount);
      guiFilter->add("nPtsMax",  "Maximum nPts", 60., 60., 0., 100.,1,Parameter::Integer, StiTrack::kPointCount);
      guiFilter->add("chargeUsed","Use Charge",     false, false, 0,1,1,Parameter::Boolean, StiTrack::kCharge);
      guiFilter->add("chargeMin", "Minimum Charge", -1., -1., -100.,   100.,1,Parameter::Integer, StiTrack::kCharge);
      guiFilter->add("chargeMax", "Maximum Charge",  1.,  1., -100.,   100.,1,Parameter::Integer, StiTrack::kCharge);
      dynamic_cast<Subject*>(guiMcTrackFilter)->attach(obs);
    }
  else
    cout << "StiKalmanTrackFinder() - INFO - No GUI filters selected" << endl;
  trackSeedFinder  = toolkit->getTrackSeedFinder();
  
  trackNodeFactory = toolkit->getTrackNodeFactory();
  detectorContainer = toolkit->getDetectorContainer();
  hitContainer = toolkit->getHitContainer();
  trackContainer = toolkit->getTrackContainer();
  mcTrackContainer = toolkit->getMcTrackContainer();
  //Turn off by default
  Messenger::instance()->clearRoutingBits(MessageType::kTrackMessage);
  setParameters(new StiKalmanTrackFinderParameters());
  mSubject->attach(this);
  getNewState();
  //cout << "StiKalmanTrackFinder::StiKalmanTrackFinder() - Done"<<endl;
}

StiKalmanTrackFinder::~StiKalmanTrackFinder()
{
  //progFlowMes <<"StiKalmanTrackFinder::~StiKalmanTrackFinder() - Begin/End"<<endl;
  if (mSubject) {
    mSubject->detach(this);
  }
}

void StiKalmanTrackFinder::getNewState()
{
  StiIOBroker *  broker = toolkit->getIOBroker();
  //cout <<"StiKalmanTrackFinder::getNewState()"<<endl;
  pars->setMCSCalculated(broker->ktfMcsCalculated()); //check
  pars->setElossCalculated(broker->ktfElossCalculated()); //check
  pars->setMaxChi2ForSelection(broker->ktfMaxChi2ForSelection());//check
  pars->setField(broker->ktfBField()); //check
  pars->setMassHypothesis(broker->ktfMassHypothesis()); //check
  
  pars->setMinContiguousHitCount(broker->ktfMinContiguousHitCount());  //check
  pars->setMaxNullCount(broker->ktfMaxNullCount()); //check
  pars->setMaxContiguousNullCount(broker->ktfMaxContiguousNullCount()); //check
  
  pars->setMinSearchWindow(broker->ktfMinSearchRadius()); //check
  pars->setMaxSearchWindow(broker->ktfMaxSearchRadius()); //check
  pars->setSearchWindowScale(broker->ktfSearchWindowScale()); //check

  if (broker->ktfUseHelixExtrapolation()==true)  {
    StiMaterialInteraction::setExtrapolationType(kHelixExtrapolation);
  }
  else {
    StiMaterialInteraction::setExtrapolationType(kLinearExtrapolation);
  }
}

void StiKalmanTrackFinder::reset()
{
  cout << "StiKalmanTrackFinder::reset() - INFO - Starting" <<endl;
  toolkit->getHitContainer()->clear();
  toolkit->getDetectorContainer()->reset();
  toolkit->getHitFactory()->reset();
  toolkit->getTrackFactory()->reset();
  toolkit->getMcTrackFactory()->reset();
  toolkit->getTrackNodeFactory()->reset();
  toolkit->getTrackContainer()->clear();
  toolkit->getMcTrackContainer()->clear();
  cout << "StiKalmanTrackFinder::reset() - INFO - Done" <<endl;
}

bool StiKalmanTrackFinder::isValid(bool debug) const
{
  bool val = trackSeedFinder && trackFilter && trackNodeFactory  && detectorContainer &&
    hitContainer && trackContainer; //
  //&& magField;
  
  if (debug) 
    {
      cout <<"\nStiTrackFinder::isValid()\n"<<endl;
      cout <<"trackSeedFinder:\t"<<trackSeedFinder<<endl;
      cout <<"trackFilter:\t\t"<<trackFilter<<endl;
      cout <<"trackNodeFactory:\t"<<trackNodeFactory<<endl;
      //cout <<"magField:\t\t"<<magField<<endl;
      cout <<"detectorContainer:\t"<<detectorContainer<<endl;
      cout <<"hitContainer:\t\t"<<hitContainer<<endl;
      cout <<"trackContainer:\t\t"<<trackContainer<<endl;
      cout <<"Returning:\t\t"<< val<< endl;
    }
  return val;
}

//Temporary patch, to test seed finder (MLM, 8/20/01)

bool StiKalmanTrackFinder::hasMore()
{
  return trackSeedFinder->hasMore();
}

/*! Find all tracks associated with the current event.
 <p>
 Algorithm: In a <b>while loop</b>, obtain track seeds from 
 current track seed finder and proceed to extend it through the
 detector. 
 <p>Found tracks are added to the track container if no track 
 filter is set or if they satisfy the track filter requirements. 
 */
void StiKalmanTrackFinder::findTracks()
{
  StiKalmanTrack * track;
  try 
    {
      while (trackSeedFinder->hasMore())
	{ 
	  // obtain track seed from seed finder
	  track = trackSeedFinder->next();
	  if (!track) 
	    break;
	  trackContainer->push_back(track);
	  try
	    {
	      track->find();
	      StiDrawableTrack * t = dynamic_cast<StiDrawableTrack *>(track);
	      if (t) 
		t->update();
	      
	      //if (pars->useTrackFilter && trackFilter->filter(track)) 
	      //trackContainer->push_back(track);
	      //else
	      //trackContainer->push_back(track);
	    }
	  catch (runtime_error & rte)
	    {
	      cout<< "StiKalmanTrackFinder::findTracks() - Run Time Error :" << rte.what() << endl;
	      //trackContainer->push_back(track);
	    }
	}
    }
  catch (exception & e) 
    {
      cout << "StiKalmanTrackFinder::findTracks() - Internal Error :" << e.what() << endl;
    }
  catch (...) 
    {
      cout << "StiKalmanTrackFinder::findTracks() - Caught unknwon exception."<<endl;
    }
}

/*! Fit all track produced by the track seed finder. 
 <p>
 This method is useful when the seed finder returns full tracks produced
 by a 3rd party track finder e.g. the tpt package.
 <p>Fitted tracks are added to the track container if no track 
 filter is set or if they satisfy the track filter requirements. 
 */
void StiKalmanTrackFinder::fitTracks()
{
  try 
    {
      track = 0;
      if (trackSeedFinder->hasMore()) 
	{ //Redundant check, but it protectes against naive calls
	  track = trackSeedFinder->next();
	  if (!track) 
	    throw runtime_error("StiKalmanTrackFinder::fitTracks() - trackSeedFinder returned track==0");
	  trackMes <<"\nStiKalmanTrackFinder::fitTracks()\t Got Valid track"<<endl;
					
	  track->fit(kOutsideIn); track->setFlag(0);
	  if (pars->useTrackFilter && trackFilter->filter(track)) 
	    trackContainer->push_back(track);
	  else
	    trackContainer->push_back(track);
       StiDrawableTrack * t = dynamic_cast<StiDrawableTrack *>(track);
       t->update();
       //track->update();  //This updates the track on the display
	  trackMes << "track parameters:";
	  trackMes << *track<<endl;
	}
      else 
	trackMes <<"\ttrackSeedFinder->hasMore()==false"<<endl;
    }
  catch (runtime_error & rte) 
    {
      trackMes << "StiKalmanTrackFinder::fitTracks() - Run Time Error :" << rte.what() << endl;
    }
  catch (exception & e) 
    {
      cout << "StiKalmanTrackFinder::fitTracks() - Internal Error :" << e.what() << endl;
    }
  catch (...) 
    {
      cout << "StiKalmanTrackFinder::fitTracks() - Caught unknwon exception."<<endl;
    }
}

/*! Extend all known tracks to primary vertex
  <p>
  Attempt an extension of all known tracks to the given primary vertex. If the extension is successfull, 
  the vertex is added to the track as a node. Node that in this implementation, it is assumed the
  track has been pruned and thus consists of a single node sequence (as opposed to a tree).
  <p>
  <ol>
  <li>Loop on all tracks currently stored in track container.</li>
  <li>It is assumed that the track does not already have a main vertex associated with it.</li>
  <li>Attempt extension to the given vertex by a call to "extendToMainVertex". 
  <li>If extension is successfull, the given vertex is added as node to the track.
  </ol>
  <p>
  <h3>Note</h3>
  Any exception thrown by "getInnerMostNode()" or "extendTrackToVertex()" are
  caught here and reported with "cout".
*/
void StiKalmanTrackFinder::extendTracksToVertex(StiHit* vertex)
{
  try 
    {
      track = 0;
      for (TrackMap::const_iterator it=trackContainer->begin(); 
	   it!=trackContainer->end(); 
	   ++it) 
	{
	  track = static_cast<StiKalmanTrack*>((*it).second);
	  try
	    {
	      track->extendToVertex(vertex);
	      // ------ track->update();
	    }
	  catch (runtime_error & rte) 
	    {
	      trackMes << "SKTF::extendTracksToVertex()"
		       << "- WARNING - Run Time Error while extending a track to main vertex."<<endl
		       << "Error Message:" << endl
		       << rte.what() << endl;
	    }
	}
    }
  catch (runtime_error & rte) 
    {
      trackMes << "StiKalmanTrackFinder::extendTracksToVertex() - Run Time Error :\n" << rte.what() << endl;
    }
  catch (logic_error & le) 
    {
      cout << "StiKalmanTrackFinder::extendTracksToVertex() - Logic Error :\n" << le.what() << endl;
    }
  catch (exception & e) 
    {
      cout << "StiKalmanTrackFinder::extendTracksToVertex() - Internal Error :\n" << e.what() << endl;
    }
  catch (...) 
    {
      cout << "StiKalmanTrackFinder::extendTracksToVertex() - Caught unknwon exception."<<endl;
    }
}



bool StiKalmanTrackFinder::find(StiTrack * t, int direction) // throws runtime_error, logic_error
{
  //-----------------------------------------------------------------
  // Find extension (track) to the given track seed
  // Return Ok      if operation was successful
  // Return Error   if given seed "t" is invalid
  //                or if input data are invalid or if some other 
  //                internal error has occured.
  //-----------------------------------------------------------------
  trackMes << "SKTF::find(StiTrack * t) - Beginning" << endl;
  track = dynamic_cast<StiKalmanTrack *> (t);
  if (!track) 
    {
      throw logic_error("SKTF::find()\t - ERROR - dynamic_cast<StiKalmanTrack *>  returned 0");
    }
  nAdded       = 0;
  state        = 0;
  trackDone    = false;
  scanningDone = true;
  sNode   = track->getLastNode(); 
  sDet    = sNode->getHit()->detector();
  tNode   = 0;    // target node
  tDet    = 0;
  leadDet = 0;
	
  if (sDet==0) 
    throw logic_error("SKTF::find(StiTrack*) - Error - sDet==0");
	
  leadDet = sDet;
  while (!trackDone) 
    {
      doInitLayer(direction); //cout<<"init layer done"<<endl;
      while (!scanningDone)
	{
	  doNextDetector();
	}
      doFinishLayer(); //cout <<"finished layer"<<endl;
    }
  // track->update();  //This updates the track on the display
  trackDone = true;
  return nAdded>0;
}

void StiKalmanTrackFinder::doInitLayer(int trackingDirection)
{
  if (trackDone || !scanningDone) return; // nothing to do
  detectorContainer->setToDetector(leadDet);
  StiDetector * currentDet = **detectorContainer;
  if (trackingDirection==kOutsideIn)
    detectorContainer->moveIn();
  else
    detectorContainer->moveOut();
  tDet = **detectorContainer;
  leadDet = tDet;
  if (tDet==0) 
    throw logic_error("SKTF::doInitLayer() ERROR - tDet==0");
  else if (tDet==currentDet) 
    {
      trackDone = true;
      return;
    }
  position     = 0;
  lastMove     = 0;
  hasDet = false;
  hasHit = false;
  scanningDone = false;
  bestChi2     = 1e50;
  bestNode     = 0;  
}


void StiKalmanTrackFinder::doNextDetector()
{
  if (trackDone || scanningDone) return;
  tNode = trackNodeFactory->getInstance();
  if (tNode==0) 
    throw logic_error("SKTF::doNextDetector()\t- ERROR - tNode==null");
  tNode->reset();
  try
    {
      position = tNode->propagate(sNode, tDet); 
    }
  catch (runtime_error & rte)
    {
      trackMes << "SKTF::doNextDetector() - RunTimeError: " << rte.what();
      scanningDone = true;
      trackDone = true;
      return;
    }
  trackMes << "TargetDet :"<<*tDet<<endl
	   << "TargetNode:"<<*tNode<<endl;
    
  if (position<=kEdgeZplus) 
    {
      hasDet = true;
      leadNode = tNode;
      leadNode->setDetector(tDet);
      if (tDet->isActive()) 
	{ // active vol, look for hits
	  if (position==kHit)
	    scanningDone = true;
	  //cout <<"dy,dz:"<< 3.*tNode->fC00 << "\t" << tNode->fC11 << endl;
	  hitContainer->setDeltaD(tNode->getWindowY());
	  hitContainer->setDeltaZ(tNode->getWindowZ());
	  hitContainer->setRefPoint(tNode->fX,
				    tDet->getPlacement()->getCenterRefAngle(),
				    tNode->fP0,
				    tNode->fP1);
	  bool hasMore = hitContainer->hasMore();
	  while (hasMore) 
	    {
	      StiHit * h = hitContainer->getHit();
	      //if (h->timesUsed()==0) { //MLM (9/27/02) //why does this break code?!
	      tNode->setHit(h);
	      // set the node error according to the ad hoc parametrization...
	      // use the crossing angle and the dip angle.
	      tNode->setError(toolkit->getHitErrorCalculator()->getHitError(h,tNode->crossAngle(),
									    tNode->pitchAngle()));
	      chi2 = tNode->evaluateChi2();
	      trackMes << "SKTF::followTrackAt()\t chi2:" << chi2  << endl;
	      if (chi2<pars->maxChi2ForSelection && chi2 < bestChi2)
		  {
		      trackMes << "SKTF::followTrackAt()\t selected hit - chi2:" << chi2 << endl;
		      hasHit = true;
		      bestChi2 = chi2;
		      bestNode = tNode;
		  }
	      hasMore = hitContainer->hasMore();
	      if (hasMore) // prepare new node
		  {
		      StiKalmanTrackNode * newNode = trackNodeFactory->getInstance();
		      if (newNode==0) 
			  throw logic_error("SKTF::followTrackAt()\t- ERROR - newNode==null");
		      newNode->reset();   
		      newNode->setState(tNode); // get everything from tNode
		      newNode->setDetector(tDet); // set the local pointer to tDet
		      tNode = newNode;  // not a memory leak because the factory handles the objects.... ;-)
		  }
	      //} //if (hit->timesUsed()==0)
	    } // searching best hit
	}
      else // projection in inactive volume, scanning is done
	  scanningDone = true;
    }
  else if (position==kFailed) 
      {
	  trackMes << "SKTF::doNextDetector()\t - position==kFailed" << endl;
	  scanningDone = true;
	  trackDone = true;
	  return;
      }


  if (!scanningDone)
    {
      StiDetector * nextDet;
      if (position==kEdgePhiPlus || position==kMissPhiPlus)
	{
	  if (lastMove>=0 )
	    {
	      trackMes << "SKTF::followTrackAt()\t- movePlusPhi()" << endl;
	      detectorContainer->movePlusPhi();   
	      nextDet = **detectorContainer;
	      if (nextDet==0 || tDet==nextDet) 
		scanningDone = true;
	      tDet = nextDet;
	      lastMove++;
	    }
	  else
	    {
	      scanningDone = true;
	      trackMes << "SKTF::followTrackAt()\t-position==kEdgePhiPlus||kMissPhiPlus - but no PlusPhi done" << endl;
	    }
      
	}
      else if (position==kEdgePhiMinus || position==kMissPhiMinus)
	{
	  if (lastMove<=0)
	    {
	      trackMes << "SKTF::followTrackAt()\t- moveMinusPhi()" << endl;
	      detectorContainer->moveMinusPhi();
	      nextDet = **detectorContainer;
	      if (nextDet==0 || tDet==nextDet) 
		{
		  //trackMes << "SKTF::followTrackAt() - ERROR  -  moveMinusPhi() >> tDet==sDet"  << endl;
		  scanningDone = true;
		}
	      tDet = nextDet;
	      lastMove--;
	    }
	  else
	    {
	      scanningDone = true;
	      trackMes << "SKTF::followTrackAt()\t-position==kEdgePhiMinus||kMissPhiMinus - but no MinusPhi done" << endl;
	    }
	}
      else
	{
	  trackMes <<  "SKTF::followTrackAt()\t- Scanning set to done" << endl;
	  scanningDone = true;
	}
    }
  if (abs(lastMove)>2) scanningDone = true;
  //printState();
}

void StiKalmanTrackFinder::printState()
{
  trackMes << "State:"<<state;
  if (scanningDone) 
    trackMes << "/Scanning Done";
  else
    trackMes << "/Scanning NOT Done";
  if (trackDone) 
    trackMes << "/Track Done"<<endl;
  else
    trackMes << "/Track NOT Done" << endl;
}

//_____________________________________________________________________________
void StiKalmanTrackFinder::doFinishLayer()
{
  if (trackDone) return;         // don't do this when the track is done
  if (!scanningDone) return;  // don't do this until scanning of layer is completed. 
  if (hasDet)
    {
      if (hasHit)
	{ 
	  bestNode->updateNode();
	  //StiHit * myHit = bestNode->getHit();
	  sNode->add(bestNode);
	  sNode = bestNode;
	  track->setLastNode(bestNode);
	  leadDet = bestNode->getDetector();
	  trackMes << "SKTF::doFinishLayer() - Adding node with hit in det:" 
		   << *leadDet << endl;
	  nAdded++;
	}
      else // no hit found
	{
	  trackMes << "=====no hit found==========================leadNode   is at:" 
		   << *leadNode << endl;
	  leadDet = leadNode->getDetector();
	  if (leadDet==0)
	    trackMes << "SKTF::doFinishLayer() - Fatal Error - leadDet==0" << endl;
	  else
	    trackMes << "SKTF::doFinishLayer() - Adding node WITHOUT hit in det:"  
		     << *leadDet << endl;
	  sNode->add(leadNode);
	  sNode = leadNode;
	  track->setLastNode(leadNode);
	  trackMes << "SKTF::doFinishLayer() "
		   << "               nullCount:" << leadNode->nullCount << endl
		   << "    contiguousNullCount :" << leadNode->contiguousNullCount << endl;
	  if (leadNode->nullCount>pars->maxNullCount ||
	      leadNode->contiguousNullCount>pars->maxContiguousNullCount)
	    trackDone = true;    
	}
    }
  else // no det crossing found
    {
      trackMes << "SKTF::doFinishLayer() - Layer Completed with no node added" << endl;
      //if (nullCount>maxNullCount ||contiguousNullCount>maxContiguousNullCount)
      // trackDone = true;
    }
  //printState();
}


/*! Get the number of tracks found by this finder for the current event.
  <p>
  This convenience method returns the number of tracks found by this finder
  for the current. The number of tracks is simply determined based on the size
  of the track container used by this finder. All tracks inserted in the container
  are counted, no quality cut is used. 
*/
int StiKalmanTrackFinder::getTrackFoundCount() const
{
  return trackContainer->size();
}

/*! Get the number of tracks satisfying the given track filter.
  <p>
  This convenience method returns the number of tracks found by this finder
  for the current that satisfy the give track filter. 
*/
int StiKalmanTrackFinder::getTrackFoundCount(Filter<StiTrack> * filter) const
{
  // reset filter counter to zero.
  filter->reset();
  // loop over all tracks and filter
  TrackMap::const_iterator it;
  for (it=trackContainer->begin(); 
       it!=trackContainer->end(); 
       ++it) 
    filter->filter((*it).second);
  return filter->getAcceptedCount();
}

/*! Get the number of track seeds found by the seed finder used by this finder for the current event.
  <p>
  This convenience method returns the number of track seeds found by the 
  seed finder used by this finder for the current. 
  Note:needs to be fixed.
*/
int StiKalmanTrackFinder::getTrackSeedFoundCount() const
{
  return trackContainer->size();
}

void StiKalmanTrackFinder::doNextTrackStep()
{
  try
    {
      switch (state)
	{
	case 0: trackMes<< "InitTrackSearch()"<< endl;
	  track = 0;
	  if (trackSeedFinder->hasMore()) 
	    { 
	      //Redundant check, but it protectes against naive calls
	      track = trackSeedFinder->next();
	      if (!track) 
		{
		  trackMes << "NO MORE TRACK SEEDS - EVENT COMPLETED" << endl;
		  return;
		}
	      sNode = track->getLastNode();
	      if (!sNode) 
		throw logic_error("SKTF::doNextTrackStep()\t - ERROR - track->getLastNode() returned 0");
	      if (!trackDone) return;
	      if (detectorContainer==0) 
		throw logic_error("SKTF::doNextTrackStep() - Error - detectorContainer==0");
	      tNode   = 0;    // target node
	      leadDet = 0;
	      trackDone = false;
	      scanningDone = true;
	      sDet  = sNode->getHit()->detector();
	      if (sDet==0) 
		throw logic_error("SKTF::doNextTrackStep() - Error - sDet==0");
	      tDet=0;
	      leadDet = sDet;		 
	    }
	  else 
	    {
	      trackMes <<"\ttrackSeedFinder->hasMore()==false"<<endl;
	    }
	  state++; break;
	case 1: doInitLayer(track->getTrackingDirection());
	  while (!scanningDone)
	    {
	      doNextDetector();
	    }
	  doFinishLayer();
	  if (trackDone)
	    state=0;
	  break;
	}
      trackMes << "Track state:" << state << endl;
       StiDrawableTrack * t = dynamic_cast<StiDrawableTrack *>(track);
       t->update();
       //track->update();
    }
  catch (runtime_error & rte)
    {
      trackMes << "StiKalmanTrackFinder::doNextTrackStep() - WARNING - RunTime Error Exception: "
	       << rte.what();
    }
  catch (exception & e)
    {
      cout << "StiKalmanTrackFinder::doNextTrackStep() - ERROR - Exception: " << e.what();
    }
  catch (...) 
    {
      cout << "StiKalmanTrackFinder::doNextTrackStep() - Caught unknwon exception."<<endl;
    }
}


void StiKalmanTrackFinder::findNextTrack()
{
  try 
    {
      track = 0;
      if (!trackSeedFinder)
	throw runtime_error("No Track seed finder instance available");
      if (trackSeedFinder->hasMore()) 
	{ 
	  track = 0;
	  track = trackSeedFinder->next();
	  //Redundant check, but it protectes against naive calls
	  if (!track)
	    throw runtime_error("TrackSeedFinder->next() returned 0");
	  track->find();
	  if (pars->useTrackFilter && trackFilter->filter(track)) {
	      trackContainer->push_back(track);
	      //this next bit is bad design.  casting doesn't count as polymorphism.  downcast == bad!
	      StiDrawableTrack * t = dynamic_cast<StiDrawableTrack *>(track);
	      if (t) {
		  t->update();
	      }
	  }
	  else {
	      trackContainer->push_back(track);
	      StiDrawableTrack * t = dynamic_cast<StiDrawableTrack *>(track);
	      if (t) {
		  t->update();
	      }
	  }
	} 
      else {
	  trackMes <<"StiKalmanTrackFinder::findNextTrack() - INFO - trackSeedFinder->hasMore()==false"<<endl;
      }
    }
  catch (runtime_error & rte) 
      {
	  trackMes << "StiKalmanTrackFinder::findNextTrack() - Run Time Error :\n" << rte.what() << endl;
      }
  catch (logic_error & le) 
      {
	  cout << "StiKalmanTrackFinder::findNextTrack() - Logic Error :\n" << le.what() << endl;
    }
  catch (exception & e) 
      {
	  cout << "StiKalmanTrackFinder::findNextTrack() - Internal Error :\n" << e.what() << endl;
      }	
  catch (...) 
      {
	  cout << "StiKalmanTrackFinder::findNextTrack() - Caught unknwon exception."<<endl;
      }
}

void StiKalmanTrackFinder::fitNextTrack()
{
  try 
    {
      track = 0;
      if (trackSeedFinder->hasMore()) 
	{ //Redundant check, but it protectes against naive calls
	  track = trackSeedFinder->next();
	  if (!track) 
	    throw runtime_error("\nStiKalmanTrackFinder::fitNextTrack() - trackSeedFinder->next() returned 0");
	  track->fit(kOutsideIn);
	  if (pars->useTrackFilter && trackFilter->filter(track)) 
	    trackContainer->push_back(track);
	  else
	    trackContainer->push_back(track);
       StiDrawableTrack * t = dynamic_cast<StiDrawableTrack *>(track);
       t->update();
       //	  track->update();  //This updates the track on the display
	  trackMes << "track parameters:" << trackMes << *track<<endl;
	}
      else 
	trackMes <<"\ttrackSeedFinder->hasMore()==false"<<endl;
    }
  catch (runtime_error & rte) 
    {
      trackMes << "StiKalmanTrackFinder::fitNextTrack() - Run Time Error :" << rte.what() << endl;
    }
  catch (exception & e) 
    {
      cout << "StiKalmanTrackFinder::fitNextTrack() - Internal Error :" << e.what() << endl;
    }
  catch (...)
    {
      cout << "StiKalmanTrackFinder::fitNextTrack() - Caught unknwon exception."<<endl;
    }
}

void StiKalmanTrackFinder::findNextTrackSegment(){}

void StiKalmanTrackFinder::update()
{
  //cout << "StiKalmanTrackFinder::update() - INFO - Starting."<<endl;
  TrackMap::const_iterator iter;
  StiDrawableTrack * t;

  StiToolkit::instance()->getDisplayManager()->reset();
  StiToolkit::instance()->getDisplayManager()->draw();
  StiToolkit::instance()->getDisplayManager()->update();
  // Monte Carlo Tracks
  if (mcTrackContainer)
    {
      //cout << "StiKalmanTrackFinder::update() - INFO - Looping on MC tracks."<<endl;
      for (iter=mcTrackContainer->begin();iter!=mcTrackContainer->end(); ++iter) 
	{
	   if (guiMcTrackFilter)
	     {
	       //cout << "StiKalmanTrackFinder::update() - INFO - Filter OK/";
	       if (guiMcTrackFilter->filter((*iter).second))
		 {
		   //cout <<"Passed"<<endl;
		   t = dynamic_cast<StiDrawableTrack *>((*iter).second);
		   if (t)
		     t->update();
		   else
		     cout << "StiKalmanTrackFinder::update() - WARNING - MC dynamic_cast failed." << endl;
		 }
	     }	
	   else
	     cout << "StiKalmanTrackFinder::update() - INFO - Filter NOK"<<endl;
	}
    }

  // Reconstructed tracks
  for (iter=trackContainer->begin();iter!=trackContainer->end();++iter) 
    {
      if (guiTrackFilter && guiTrackFilter->filter((*iter).second))
	{
	  t = dynamic_cast<StiDrawableTrack *>((*iter).second);
	  if (t)
	      t->update();
	  else
	    cout << "StiKalmanTrackFinder::update() - WARNING - Kalman dynamic_cast failed." << endl;
	}
    }
  StiToolkit::instance()->getDisplayManager()->draw();
  StiToolkit::instance()->getDisplayManager()->update();
  //cout << "StiKalmanTrackFinder::update() - INFO - Done."<<endl;
}

void StiKalmanTrackFinder::setEvent(StEvent * event, StMcEvent * mcEvent)
{
  cout << "StiKalmanTrackFinder::setEvent(StEvent*) - INFO - Starting"  << endl;
  toolkit->getHitLoader()->loadHits(event);
  //toolkit->getHitFiller()->setEvent(event);
  //toolkit->getHitFiller()->fillHits(toolkit->getHitContainer(), toolkit->getHitFactory());
  toolkit->getHitContainer()->sortHits();
  toolkit->getHitContainer()->update(); //uncommented, MLM, 9/27
  trackSeedFinder->reset();

  if (mcEvent)
    { 
      StiEvaluableTrackSeedFinder* temp;
      temp = dynamic_cast<StiEvaluableTrackSeedFinder*>(toolkit->getTrackSeedFinder());
      if (temp!=0) 
	temp->setEvent(mcEvent);

      // Store the pointers to StMcTrack in StiMcTrack
      StSPtrVecMcTrack & mcTracks = mcEvent->tracks();
      StiMcTrack * mcTrack;
      StMcTrackConstIterator iter;
      for (iter=mcTracks.begin();iter!=mcTracks.end();iter++)
	{
	  //cout << "Loading StMcTrack into mcTrackContainer" << endl;
	  mcTrack = toolkit->getMcTrackFactory()->getInstance();
	  mcTrack->setStMcTrack( (*iter) );
	  mcTrackContainer->add(mcTrack);
	}
    }
  cout << "StiKalmanTrackFinder::setEvent(StEvent*) - INFO - Done"  << endl;
}

void StiKalmanTrackFinder::changed(Subject* changedSubject)
{
  cout << "StiKalmanTrackFinder::changed(Subject* changedSubject)" << endl;
  getNewState();
  update();
}
