/***************************************************************************
 *
 * $Id: StEmcTpcFourPMaker.cxx,v 1.3 2004/10/13 15:32:33 mmiller Exp $
 * 
 * Author: Thomas Henry February 2003
 ***************************************************************************
 *
 * Description:  Maker which creates a list of Four Momentums from the TPC
 * and EMC corresponding to charged particles and photons, but subtracting
 * some of the energy deposited in the EMC by the charged particles.
 *
 ***************************************************************************
 *
 * Revision 1.0  2003/02/20 thenry
 *
 **************************************************************************/
using namespace std;
//std
#include <string>
#include <iostream>
#include <math.h>
#include <sys/times.h>

//STAR
#include "TFile.h"
#include "StChain.h"
#include "SystemOfUnits.h"

//StMuDstMaker
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuEmcCollection.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEmcUtil.h"

//StEmc
#include "StEmcClusterCollection.h"
#include "StEmcPoint.h"
#include "StEmcUtil/geometry/StEmcGeom.h"
#include "StEmcUtil/others/emcDetectorName.h"
#include "StEmcADCtoEMaker/StBemcData.h"

//StJetMaker
#include "StJetMaker/StMuTrackFourVec.h"
#include "StEmcTpcFourPMaker.h"

ClassImp(StEmcTpcFourPMaker)
  
    double PionAveDepRatio;
double KaonAveDepRatio;
double ProtonAveDepRatio;
double ElectronAveDepRatio;
double ChargedAveDep;

double SMDR;
double mSMDR;
double mHSMDR;
double mtwoPi;
double mme;	
double mmpr;
double mmpi;
double mmk;

TH1F* adcValHist;
long eventNum;
long latestrunNum;

// This is the Energy deposit function.  It calculates the amount of 
// energy deposited by a charged particle 
double StProjectedTrack::depE(void) 
{ 
    double coef_0 = ChargedAveDep*(1.0-probElectron);
    double coef_1 = probElectron*ElectronAveDepRatio +
	probPion*PionAveDepRatio + 
	probProton*ProtonAveDepRatio +
	probKaon*KaonAveDepRatio;
    cout << "coef_0: " << coef_0 << endl;
    cout << "coef_1: " << coef_1 << endl;
    return coef_0 + coef_1*E();
}

void StEmcTpcFourPMaker::SetDepRatios(double PIDR, double KDR, 
				      double PRDR, double EDR, double CAD)
{
    mPIDR = PIDR;
    mKDR = KDR;
    mPRDR = PRDR;
    mEDR = EDR;
    mCAD = CAD;
    SetDepRatios();
}

void StEmcTpcFourPMaker::SetDepRatios(void)
{
    PionAveDepRatio = mPIDR;
    KaonAveDepRatio = mKDR;
    ProtonAveDepRatio = mPRDR;
    ElectronAveDepRatio = mEDR;
    ChargedAveDep = mCAD;
}

StEmcTpcFourPMaker::StEmcTpcFourPMaker(const char* name, 
				       StMuDstMaker* uDstMaker, 
				       long pBins, long thBins, double pRad, double thRad, double rsqr,
				       StEmcADCtoEMaker* adcToEMaker) 
    : StFourPMaker(name, uDstMaker), radiussqr(rsqr), binmap(pBins, thBins, pRad, thRad), adc2E(adcToEMaker) {
    seconds = 0;
    muEmc = NULL;
    emc = NULL;
    SetDepRatios(0.2,0.2,0.2,1.0,0.0);
    maxHits = 4800;
    fakePoints.resize(maxHits);
    useType = Hits;
    EMCSanityThreshold = 200.0;
    maxPoints = 10000;
    minPointThreshold = .01;

    SMDR = 2.2625;
    mSMDR = 231.23;
    mHSMDR = 115.615;
    mtwoPi = M_PI*2.0;
    mme = .000511;	
    mmpr = .9383;
    mmpi = .1396;
    mmk = .4937;
    noAbortions = false;
    
}

Int_t StEmcTpcFourPMaker::Make() {
    SetDepRatios();
    cout <<" Start StEmcTpcFourPMaker :: "<< GetName() <<" mode="<<m_Mode<<endl;   
    binmap.clearall();

    aborted = true;
    // Construct tracks out of (primary) tracks and EMC points. 
    // Must calculate the eta shift of the EMC
    StMuDst* uDst = muDst->muDst();
    StMuEvent* uEvent = uDst->event();
    double mField = uEvent->magneticField()/10.0;
    cout << "mField: " << mField << endl;
    StThreeVectorF vertex = uEvent->primaryVertexPosition();
    binmap.setVertex(vertex);
    double SMDR = 2.2625;
    double etaShift = atan2(vertex.z()/100.0, SMDR);
    cout << "zVertex: " << vertex.z()/100.0 << endl;
    cout << "EtaShift: " << etaShift << endl;
    double HSMDR = SMDR/2.0;
    long nTracks = uDst->numberOfPrimaryTracks();
    numCoincidences = 0;
    sumPtTracks = 0.0;
    sumEMC = 0.0;
    sumSubtracted = 0.0;
    sumTheorySubtracted = 0.0;

    // Calculate trackEmcPhi (Phi of the track at the radius of the EMC SMD)
    // pt=BeR, pt=0.3BR, pt GeV/c, B Tesla, R meters, R = pt/(Be) = pt/(0.3B)
    cout <<"\tlooping on:\t"<<nTracks<<"\ttracks from MuDst"<<endl;
    int ntkept =0;
    int badflag, ftpc, loweta, higheta, badr, badhits;
    badflag = ftpc = loweta = higheta = badr = badhits = 0;
    
    for(int i = 0; i < nTracks; i++)	{
	StMuTrack* track = uDst->primaryTracks(i);
	if(track->flag() < 0) {
	    //cout <<"skipping track:\t"<<i<<"\twith flag:\t"<<track->flag()<<endl;
	    ++badflag;
	    continue;
	}
	if (track->topologyMap().trackFtpcEast()==true || track->topologyMap().trackFtpcWest()==true) {
	    //cout <<"skipping track:\t"<<i<<"\twhich is from FTPC"<<endl;
	    ++ftpc;
	    continue;
	}
	if(track->eta() < GetEtaLow()) {
	    //cout <<"skipping track:\t"<<i<<"\twith eta:\t"<<track->eta()<<"\twich is less than:\t"<<GetEtaLow()<<endl;
	    ++loweta;
	    continue;
	}
	if(track->eta() > GetEtaHigh()) {
	    //cout <<"skipping track:\t"<<i<<"\twith eta:\t"<<track->eta()<<"\twich is more than:\t"<<GetEtaHigh()<<endl;
	    ++higheta;
	    continue;
	}
	double pt = track->pt();
	double R = pt/(0.3*fabs(mField));
	if(R < HSMDR) {// just forget the track if it doesn't get to EMC radius.
	    //cout <<"skipping track:\t"<<i<<"\twith R:\t"<<R<<"\twich is less than:\t"<<HSMDR<<"\t with pt:\t"<<pt<<"\tusing b:\t"<<mField<<endl;
	    ++badr;
	    continue;
	}
	if(static_cast<double>(track->nHits())/static_cast<double>(track->nHitsPoss()) < .51) {
	    //cout <<"skipping track:\t"<<i<<"\twith nHits:\t"<<track->nHits()<<"\tand nHitsPoss:\t"<<track->nHitsPoss()<<endl;
	    ++badhits;
	    continue;
	}
	sumPtTracks += pt;
	binmap.insertTrack(track, i);
	++ntkept;
    }
    cout <<"skipped "<<badflag<<" for flag, "<<ftpc<<" for ftpc, "<<loweta<<" for loweta, "<<higheta<<" for higheta, "
	 <<badr<<" for badr, "<<badhits<<" for hits"<<endl;
    
    cout <<"Added:\t"<<ntkept<<"\ttracks to the binmap"<<endl;

    
    // Retreive the points
    StEmcCollection *emc = NULL;
    if (!adc2E) {
	cout <<"StEmcTpcFourPMaker::Make().  ERROR. adc2E==0, undefined behavior, so we abort"<<endl;
	abort();
    }
    
    cout <<"use hits"<<endl;

    emc = adc2E->getEmcCollection();
    double twoPi = M_PI*2.0;
    int numHits = 0;
    StEmcGeom* geom = StEmcGeom::getEmcGeom(detname[0].Data());
      
    StBemcData* data = adc2E->getBemcData();
    numHits = data->NTowerHits;
    cout << "Number Hits: " << numHits<<endl;

    for(int hitId = 1; hitId <= maxHits; hitId++) {
	float eta, phi, energy;
	geom->getEtaPhi(hitId, eta, phi);
	if(data->TowerStatus[hitId-1] != 1) {
	    continue;
	}
	energy = data->TowerEnergy[hitId-1];

	while(phi < 0) phi += twoPi;
	while(phi > twoPi) phi -= twoPi;
	  
	sumEMC += energy;
	if(energy < 0.01) continue;
	  
	// now add a fake point to the binmap
	StMuEmcPoint& point = fakePoints[hitId];
	point.setEta(eta);
	point.setPhi(phi);
	point.setEnergy(energy);
	binmap.insertPoint(&point, hitId);
	//sumEMC += point.getEnergy();
    }

    // Now Bail if the energy is absurd
    cout <<"sumEMC:\t"<<sumEMC<<"\tGeV"<<endl;
    if(!noAbortions) {
	if(sumEMC >= EMCSanityThreshold) {
	    cout <<"StEmcTpcFourPMaker::Maker():\tsumEmc >= EMCSanityThreshold.  return"<<endl;
	    return kStOK;
	}
    }

    // Connect the points with the tracks when they are within radiussqr 
    binmap.correlate(radiussqr);
    numCoincidences = binmap.t2p.size(); // == binmap.pt2.size();
    cout << "Number Coincidences " << numCoincidences << endl;

    // Veto the guess of Tpc particle identification using the 
    // Point correlations
    for(trackToPoints::iterator trackit = binmap.t2p.begin(); 
	trackit != binmap.t2p.end(); ++trackit)
	{
	    // If the energy of the point is close to the energy of the track,
	    // then it is most likely an electron:
	    StMuTrack* track= (*trackit).first;
	    StProjectedTrack &pTrack = binmap.moddTracks[track];
	    StMuEmcPoint* point = (*trackit).second;
	    double trackE = pTrack.E();
	    double pointE = binmap.moddPoints[point].E();
	    double ediff = fabs(trackE - pointE);
	    if(ediff < 0.5*pointE)
		pTrack.probEIsOne();
	    if(trackE < 0.3*pointE)
		pTrack.probEIsZero();
	    break;
	}

    // It can't be an electron if there is no point:
    for(trackMap::iterator trackit = binmap.moddTracks.begin();
	trackit != binmap.moddTracks.end(); ++trackit)
	{
	    trackToPoints::iterator foundPoint = binmap.t2p.find((*trackit).first);
	    if(foundPoint == binmap.t2p.end())
		{
		    binmap.moddTracks[(*trackit).first].probEIsZero();
		    continue;
		}
	}

    numberPoints = 0;
    double maxEtValue = 0;
    for(pointMap::iterator point = binmap.moddPoints.begin(); 
	point != binmap.moddPoints.end(); ++point)
	{
	    pointMap::value_type &point_val = *point;
	    StCorrectedEmcPoint &cPoint = point_val.second;
	    double eta = cPoint.P().pseudoRapidity();
	    double et = cPoint.P().e()*sqrt(1.0-tanh(eta)*tanh(eta));
	    if(et > minPointThreshold)
		numberPoints++;
	    if(et > maxEtValue)
		maxEtValue = et;
	}
    if(!noAbortions)
	if(numberPoints > maxPoints) { // If there are too many points
	    cout <<"StEmcTpcFourPMaker::Make()\tERROR:\tnumberPoints > maxPoints.  return"<<endl;
	    return kStOK; // don't try to analyze this event
	}
    // If it is a hightower trigger event, but somehow the high tower
    // is missing from the points, skip the event to avoid weird emc biases.
    if(!noAbortions)
	{
	    const StTriggerId &trigger = uEvent->triggerIdCollection().nominal();
	    if(trigger.isTrigger(1101) || trigger.isTrigger(2201))
		if(maxEtValue < 2.5) {
		    cout <<"StEmcTpcFourPMaker::Make()\tERROR:\tmaxEtValue<2.5.  return"<<endl;
		    return kStOK;
		}
	    if(trigger.isTrigger(1102) || trigger.isTrigger(2202))
		if(maxEtValue < 3.5) {
		    cout <<"StEmcTpcFourPMaker::Make()\tERROR:\tmaxEtValue<3.5.  return"<<endl;
		    return kStOK;
		}
	}
    
    // Add TPC tracks
    long index = 0;
    tracks.clear();
    cout <<"binmap.moddTracks.size():\t"<<binmap.moddTracks.size()<<endl;
    
    for(trackMap::iterator track = binmap.moddTracks.begin();
	track != binmap.moddTracks.end(); ++track)
	{
	    trackMap::value_type &track_val = *track;
	    StMuTrackFourVec& newTrack = tPile[index++];
	    StProjectedTrack &pTrack = track_val.second;
	    newTrack.Init(pTrack.getTrack(), pTrack.P(), pTrack.getIndex(), kTpcId);
	    //cout <<"InitTrack kTpcId:\t"<<newTrack<<endl;
	    tracks.push_back(&newTrack);
	}


    // Now subtract the energy deposited by the tracks:
    StMuTrack* lasttrack = NULL;
    if(binmap.t2p.size() > 0)
	lasttrack = (*(binmap.t2p.begin())).first;
    double deposit = 0;
    if(lasttrack != NULL)
	deposit = (binmap.moddTracks[lasttrack]).depE();
    DistanceToPointMap pointsDist;
    for(trackToPoints::iterator trackit = binmap.t2p.begin(); 
	trackit != binmap.t2p.end(); ++trackit)
	{
	    if((*trackit).first != lasttrack)
		{
		    for(DistanceToPointMap::iterator d2p = pointsDist.begin();
			d2p != pointsDist.end(); ++d2p)
			{
			    StCorrectedEmcPoint& point = binmap.moddPoints[(*d2p).second];
			    if(point.E() > deposit)
				{
				    point.SubE(deposit);
				    sumSubtracted += deposit;
				    deposit = 0;
				    break;
				}
			    else
				{
				    deposit -= point.E();
				    sumSubtracted += point.E();
				    point.SetE(0);
				}
			}
		    pointsDist.clear();
		    lasttrack = (*trackit).first;
		    deposit = (binmap.moddTracks[lasttrack]).depE();
		    sumTheorySubtracted += deposit;
		}

	    StProjectedTrack& track = binmap.moddTracks[(*trackit).first];
	    StCorrectedEmcPoint& point = binmap.moddPoints[(*trackit).second];
	    pointsDist.insert(DistanceToPointMap::value_type
			      (binmap.trackPointRadiusSqr(track, point), 
			       (*trackit).second));
	}
    cout << "sumTheorySubtracted: " << sumTheorySubtracted << endl;
    cout << "sumSubtracted: " << sumSubtracted << endl;

    // Add neutral pion and eta tracks using the remaining energy in the 
    // reducedPointEnergies array - not coded

    // Add photon tracks using the remainder of the energy in thereducedPointEnergies array 
    for(pointMap::iterator point = binmap.moddPoints.begin(); point != binmap.moddPoints.end(); ++point) {
	pointMap::value_type &point_val = *point;
	StMuTrackFourVec& newTrack = tPile[index++];
	StCorrectedEmcPoint &cPoint = point_val.second;
	if(cPoint.P().e() > minPointThreshold)
	    {
		newTrack.Init(NULL, cPoint.P(), cPoint.getIndex(), kBarrelEmcTowerId);
		//cout <<"InitTrack kBarrelEmcTowerId:\t"<<newTrack<<endl;
		tracks.push_back(&newTrack);
	    }
    }  
    
    eventNum = uEvent->eventId();
    latestrunNum = uEvent->runNumber();
    aborted = false;
    cout <<"StEmcTpcFourPMaker::Maker()\tReturning kStOk"<<endl;
    return kStOk;
}

Int_t StEmcTpcFourPMaker::Finish()
{
    TString name("/star/rcf/pwg/spin/henry/spindst/adchists/");
    stringstream evnumstrstrm;
    evnumstrstrm << latestrunNum << "-" << eventNum << ".root";
    TString numstr(evnumstrstrm.str().c_str());
    name += numstr;
    return kStOk;
}

