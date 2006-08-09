#include <iostream>
#include <stdexcept>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "StEvent.h"
#include "StMcEvent/StMcEvent.hh"
#include "StEventTypes.h"
#include "StDbUtilities/StGlobalCoordinate.hh"
#include "Sti/Base/Factory.h"
#include "Sti/StiHit.h"
#include "Sti/StiHitContainer.h"
#include "Sti/StiDetector.h"
#include "Sti/StiDetectorBuilder.h"
//#include "Sti/StiMcTrack.h"
#include "Sti/StiTrackContainer.h"
#include "StiIstHitLoader.h"
#include "StMcEvent/StMcTrack.hh"
#include "StMcEvent/StMcIstHit.hh"
#include "StMcEvent/StMcIstHitCollection.hh"
#include "StMcEvent/StMcIstLayerHitCollection.hh"

StiIstHitLoader::StiIstHitLoader()
    : StiHitLoader<StEvent,StiDetectorBuilder>("IstHitLoader")
{}

StiIstHitLoader::StiIstHitLoader(StiHitContainer* hitContainer,
				 Factory<StiHit>*hitFactory,
				 StiDetectorBuilder * detector)
    : StiHitLoader<StEvent,StiDetectorBuilder>("IstHitLoader",hitContainer,hitFactory,detector)
{evNum=0;}

StiIstHitLoader::~StiIstHitLoader()
{}

void StiIstHitLoader::loadHits(StEvent* source,
			       Filter<StiTrack> * trackFilter,
			       Filter<StiHit> * hitFilter)
{
    n=0;
    //cout << "  n = " << n << endl;
    cout << "StiIstHitLoader::loadHits(StEvent*) -I- Started" << endl;
    if (!_detector)
	throw runtime_error("StiIstHitLoader::loadHits(StEvent*) - FATAL - _detector==0");
    if(!_hitContainer)
	throw runtime_error("StiIstHitLoader::loadHits(StEvent*) - FATAL - _hitContainer==0");

    //StSPtrVecHit* istHits = source->hitCollection("Ist");
    StRnDHitCollection *col = source->rndHitCollection();
    if (!col) {
	cout <<"StiIstHitLoader::loadHits\tERROR:\tcol==0"<<endl;
	cout <<"You must not have pixelFastSim in your chain"<<endl;
	cout <<"will return with no action taken"<<endl;
	return;
    }
    StSPtrVecRnDHit& vec = col->hits();

    cout <<"StiIstHitLoader: Ist Hits: "<<vec.size()<<endl;
    
    for(unsigned int j=0; j<vec.size(); j++) {

	//StRnDHit* hit = dynamic_cast<StRndHit*>((*istHits)[j]);
	StRnDHit* hit = vec[j];
	assert(hit);

	if (hit->detector()!=kIstId) continue;
	if(hit->extraByte0()==1){
	  cout <<"retrieve detector"<<endl;
	  StiDetector* detector = _detector->getDetector(hit->layer()-1, hit->ladder()-1);
	  if (!detector) cout <<"no detector found for hit:\t"<<*hit<<endl;
	  assert(detector);
	  cout <<"add hit to detector:\t"<<detector->getName()<<endl;
	
	  StiHit * stiHit = _hitFactory->getInstance();
	  if(!stiHit) throw runtime_error("StiIstHitLoader::loadHits(StEvent*) -E- stiHit==0");
	  stiHit->reset();
	  stiHit->setGlobal(detector, hit, hit->position().x(),hit->position().y(),hit->position().z(),hit->charge());
	  _hitContainer->add( stiHit );
	}
    }
    
    cout << "StiIstHitLoader::loadHits(StEvent*) -I- Done" << endl;
}

/// Load the MC hits of the Ist detector associated with the given source, and StMcTrack
/// Call once per MC track - but currently only able to load from source->istHitCollection()
/// so... use a cached hit pointer to remember whether an event has already been loaded.
/*
void StiIstHitLoader::loadMcHits(StMcEvent* source,
				 bool useMcAsRec,
				 Filter<StiTrack> * trackFilter,
				 Filter<StiHit> * hitFilter,
				 StMcTrack & stMcTrack,
				 StiMcTrack & stiMcTrack)
{
  if (evNum==source->eventNumber()) return;
  else evNum=source->eventNumber();

  cout << "StiIstHitLoader::loadMcHits(StMcEvent*) -I- Started" << endl;
  if (!_detector)  throw runtime_error("StiIstHitLoader::loadMcHits(StMcEvent*) -F- _detector==0");
  if(!_mcHitContainer) throw runtime_error("StiIstHitLoader::loadMcHits(StMcEvent*) -F- _mcHitContainer==0");
  if(!_mcTrackFactory) throw runtime_error("StiIstHitLoader::loadMcHits() -F- _mcTrackFactory==0");
  if (!_mcTrackContainer) throw runtime_error("StiIstHitLoader::loadMcHitss() -F- _mcTrackContainer==0");
  if(!_hitFactory) throw runtime_error("StiIstHitLoader::loadMcHits(StMcEvent*) -F- _hitFactory==0");
  cout << "StiIstHitLoader::loadMcHits() -I- Loading"<<endl;

  if (n>0) return;
  n++;
  StMcIstHitCollection* allIstHitCol = source->istHitCollection();
  //cout <<   "ist: Number of Layers" << allIstHitCol->numberOfLayers() << endl;
  for(int tiLayer=0; tiLayer<allIstHitCol->numberOfLayers(); tiLayer++)
    {
      cout << "StiIstHitLoader::loadMcHits(StMcEvent*) -I- layer " << tiLayer << endl;
      StMcIstLayerHitCollection* istHitCol = allIstHitCol->layer(tiLayer);
      StSPtrVecMcIstHit& hits =  istHitCol->hits();
      
      int nHitCount = 0;
      StiDetector* detector;
      
      for (vector<StMcIstHit*>::const_iterator iterHit = hits.begin();iterHit != hits.end();iterHit++)
	{
	  StMcIstHit *hit = *iterHit;
	  if(!hit){//something's wrong
	    cout<<"null hit"<<endl;
	    continue;
	  }
	  nHitCount++;
	  //cout << "guck: " << tiLayer << "   " << hit->layer()-1 << endl;
	  detector = _detector->getDetector(hit->layer()-1, hit->ladder()-1);
	  StiHit * stiHit = _hitFactory->getInstance();
	  if(!stiHit) throw runtime_error("StiIstHitLoader::loadHits(StEvent*) -E- stiHit==0");
	  stiHit->reset();
	  stiHit->setGlobal(detector, 0, hit->position().x(),hit->position().y(),hit->position().z(),hit->dE());
	  _hitContainer->add( stiHit );
	  //stiMcTrack.addHit( stiHit );
	}
      cout << "StiIstHitLoader::loadMcHits(StEvent*) -I-  number of hits uploaded, after: " << _hitContainer->size() << endl;
      cout << "StiIstHitLoader::loadMcHits(StEvent*) -I- Done" << endl;
      cout<<"nHitCount\t"<<nHitCount<<endl;
    }
}
*/
