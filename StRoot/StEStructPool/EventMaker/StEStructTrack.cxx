/**********************************************************************
 *
 * $Id: StEStructTrack.cxx,v 1.9 2010/09/02 21:26:29 prindle Exp $
 *
 * Author: Jeff Porter merge of code from Aya Ishihara and Jeff Reid
 *
 **********************************************************************
 *
 * Description:  Persistent track information
 *
 **********************************************************************/

#include "StEStructTrack.h"
#include "StPhysicalHelix.hh"
#include "SystemOfUnits.h"

#ifndef ST_NO_NAMESPACES
using namespace units;
#endif

ClassImp(StEStructTrack)

  StEStructTrack::StEStructTrack(StEStructTrack *track) : TObject(), mHelix(0,0,0,StThreeVectorD(), -1) {
  mPx = track->Px();
  mPy = track->Py();
  mPz = track->Pz();

  mEta = track->Eta();
  mPhi = track->Phi();

  mBxPrimary = track->BxPrimary();
  mByPrimary = track->ByPrimary();
  mBzPrimary = track->BzPrimary();

  mBxGlobal = track->BxGlobal();
  mByGlobal = track->ByGlobal();
  mBzGlobal = track->BzGlobal();

  mPIDe_dEdx  = track->PIDe_dEdx();
  mPIDpi_dEdx = track->PIDpi_dEdx();
  mPIDp_dEdx  = track->PIDp_dEdx();
  mPIDk_dEdx  = track->PIDk_dEdx();
  mPIDd_dEdx  = track->PIDd_dEdx();

  mPIDe_ToF  = track->PIDe_ToF();
  mPIDpi_ToF = track->PIDpi_ToF();
  mPIDp_ToF  = track->PIDp_ToF();
  mPIDk_ToF  = track->PIDk_ToF();
  mPIDd_ToF  = track->PIDd_ToF();

  mChi2 = track->Chi2();
  mBeta = track->beta();
  mDedx = track->Dedx();
  mAssignedMass=track->AssignedMass();

  mNFitPoints = track->NFitPoints();
  mNFoundPoints = track->NFoundPoints();
  mNMaxPoints = track->NMaxPoints();

  mDetectorID = track->DetectorID();
  mFlag = track->Flag();

  mCharge = track->Charge();

  mMap[0] = track->TopologyMapData(0);
  mMap[1] = track->TopologyMapData(1);
  mTPCNHits = track->TopologyMapTPCNHits();

  mHelix = track->Helix();

  //
  // check to see if one can complete ... requires event level information
  // such as bfield. If so, complete and set, if not set incomplete.
  //
 if(track->isComplete()){
    FillTransientData();
    mStartPos=track->StartPos();
    FillTpcReferencePoints();
    mIsComplete=true;
 } else {
   mIsComplete=false;
 }
}

//----------------------------------------------------------
void StEStructTrack::FillTransientData(){

  evalPt();
  evalPtot();
  evalYt();
  evalXt();
  evalCurvature();
  evalFourMomentum();
  evalPID();
  evalMass();
  
};

//----------------------------------------------------------
void StEStructTrack::evalYt(){
  float _r=mPt/0.13957;
  mYt = log(sqrt(1+_r*_r)+_r);

  mytbin=(int) floor((mYt-1.0)/0.5);
  if(mytbin>6)mytbin=6;  // Why 6?
  if(mytbin<0)mytbin=0;
};

//----------------------------------------------------------
void StEStructTrack::evalXt(){
  //
  // cut and paste from Aya's code
  //
  // Aya calls this Xmt, it doesn't seem to be what she used for the XtXt paper, so I'll use the standard instead
  /*float PionMass = 0.139;
  float Temperature = 0.25;
  float Minimum = (1+(PionMass/Temperature))*exp(-PionMass/Temperature);
  float mtOnly = sqrt((mPx*mPx)+(mPy*mPy)+PionMass*PionMass);
  mXt=1-(1+(mtOnly/Temperature))*exp(-mtOnly/Temperature)/Minimum;*/

  float PionMass = 0.139;
  float mt = Mt(PionMass);
  mXt = 1 - exp( -1*(mt-PionMass)/0.4 );

};


//----------------------------------------------------------
void StEStructTrack::evalPID(){
    // Combine dEdx and ToF information
    // Our numbering: (should we maybe be more forward thinking, with room for e, Lambda, K0??)
    //     pi = 1
    //     K  = 2
    //     p  = 3
    float pi = fabs(mPIDpi_dEdx);
    float k  = fabs(mPIDk_dEdx);
    float p  = fabs(mPIDp_dEdx);
    bool dpi     = false;
    bool dK      = false;
    bool dp      = false;
    mPID_dEdx = false;
    if (fabs(pi)<2  && fabs(k)>2 && fabs(p)>2) {
        dpi = true;
        mPID_dEdx = true;
    } else if (fabs(pi)>2  && fabs(k)<2 && fabs(p)>2) {
        dK = true;
        mPID_dEdx = true;
    } else if (fabs(pi)>2  && fabs(k)>2 && fabs(p)<2) {
        dp = true;
        mPID_dEdx = true;
    }

    // Something like 30% of tracks have no ToF?
    // Is a ToF hit the is not within a PID band more important than a missing ToF hit?
    double tnpi = mPIDpi_ToF;
    double tnK  = mPIDk_ToF;
    double tnp  = mPIDp_ToF;
    bool tpi    = false;
    bool tK     = false;
    bool tp     = false;
    bool mPID_ToF = false;
    if (fabs(tnpi)<2  && fabs(tnK)>2 && fabs(tnp)>2) {
        tpi = true;
        mPID_ToF = true;
    } else if (fabs(tnpi)>2  && fabs(tnK)<2 && fabs(tnp)>2) {
        tK = true;
        mPID_ToF = true;
    } else if (fabs(tnpi)>2  && fabs(tnK)>2 && fabs(tnp)<2) {
        tp = true;
        mPID_ToF = true;
    }
 
    if (mPID_dEdx && mPID_ToF) {
        if (dpi && tpi) {
            mPID = 1;
        } else if (dK && tK) {
            mPID = 2;
        } else if (dp && tp) {
            mPID = 3;
        } else {
            mPID = 0;
        }
    } else if (mPID_dEdx) {
        if (dpi) {
            mPID = 1;
        } else if (dK) {
            mPID = 2;
        } else if (dp) {
            mPID = 3;
        } else {
            mPID = 0;
        }
    } else {
        if (tpi) {
            mPID = 1;
        } else if (tK) {
            mPID = 2;
        } else if (tp) {
            mPID = 3;
        } else {
            mPID = 0;
        }
    }
};

//----------------------------------------------------------
void StEStructTrack::evalCurvature(){
    // store helix curvature.
    // Seems that curvature from helix is _not_ signed.
    // Sign of curvature is -helicity. (When magnetic field along +Z direction
    // helicity of a positive particle is negative.)
    mCurvature = -mHelix.h()*fabs(mHelix.curvature());
};


//----------------------------------------------------------
void StEStructTrack::evalFourMomentum(const float mass){

  float lMass=mass;
  if(lMass<=0)lMass=0.13957;

  mFourMomentum.setPx(mPx);
  mFourMomentum.setPy(mPy);
  mFourMomentum.setPz(mPz);
  mFourMomentum.setE(sqrt(mPt*mPt+mPz*mPz+lMass*lMass));

}

//----------------------------------------------------------
void StEStructTrack::FillTpcReferencePoints(){
  // Uses fitted helix to calculate intersection points in the TPC

  static StThreeVectorF WestEnd(0.,0.,200.);
  static StThreeVectorF EastEnd(0.,0.,-200.);
  static StThreeVectorF EndCapNormal(0.,0.,1.0);

  // In this use, pathLength(r) returns the helix path length to the intersection of a cylinder with radius r.
  // There are 2 mathematical solutions, so both are returned in the pairD class  where first < second.
  // If the first is <0 it is unphysical, so we would use the second.

  // The exit point is a special case, we need to find if the track exited the side or endcaps of TPC
  pairD candidates = mHelix.pathLength(200.0);
  double sideLength = (candidates.first > 0) ? candidates.first : candidates.second;
  double endLength = mHelix.pathLength(WestEnd,EndCapNormal);
  int endcap;
  if (endLength < 0.0) {
      endcap = +1;
      endLength = mHelix.pathLength(EastEnd,EndCapNormal);
  } else {
      endcap = -1;
  }
  double firstExitLength = endLength;
  if (endLength > sideLength) {
      mEndCapOuter = 0;
      firstExitLength = sideLength;
  } else {
      mEndCapOuter = endcap;
  }
  mNominalTpcExitPoint = mHelix.at(firstExitLength);

  candidates = mHelix.pathLength(50.0);
  sideLength = (candidates.first > 0) ? candidates.first : candidates.second;
  mNominalTpcEntrancePoint = mHelix.at(sideLength);

  // With cuts |\eta| < 1 and |V_z| < 50cm all tracks should cross 127cm radius before
  // intersecting endcap. (Need to double check this is true for lowest momentum helix we accept.)
  mMidTPCRadius = 127.0;
  candidates = mHelix.pathLength(mMidTPCRadius);
  sideLength = (candidates.first > 0) ? candidates.first : candidates.second;
  mMidTpcPoint = mHelix.at(sideLength);

  // Add OuterMid point at 163.5. This is to help with my crossing cut for LS tracks that have different pt.
  // Possible track may have excited via endcap at this radius.
  mOuterMidTPCRadius = 163.5;
  candidates = mHelix.pathLength(mOuterMidTPCRadius);
  sideLength = (candidates.first > 0) ? candidates.first : candidates.second;
  firstExitLength = endLength;
  if (endLength > sideLength) {
      mEndCapOuterMid = 0;
      firstExitLength = sideLength;
  } else {
      mEndCapOuterMid = endcap;
  }
  mOuterMidTpcPoint = mHelix.at(firstExitLength);

  // Store maximum radius this track will get to. Not all tracks get to specific radii.
  double curve = mHelix.curvature();
  if (curve > 0.000001) {
      mMaxRadius = 2/curve;
  } else {
      mMaxRadius = 99999.0;   // Arbitrary number bigger than anything we care about.
  }
  // Store radius of track intersection with endcap.
  StThreeVectorF vendcap = mHelix.at(endLength);
  mEndCapRadius = sqrt(vendcap.x()*vendcap.x() + vendcap.y()*vendcap.y());

  mIsComplete=true;  // finished with calculations

}


//----------------------------------------------------------
//  older stuff ... should look at some time in the future ...
//----------------------------------------------------------

Float_t StEStructTrack::Pt() const { return mPt; };
Float_t StEStructTrack::Ptot() const { return mPtot; };

Float_t StEStructTrack::Mt(Float_t mass) const { 
  return sqrt((mPt*mPt)+(mass*mass)); 
}

Float_t StEStructTrack::E(Float_t mass) const { 
  return sqrt((mPt*mPt)+(mPz*mPz)+(mass*mass)); 
}

Float_t StEStructTrack::Yt(Float_t mass) const { 
  if (0 == mass) {
      return mYt;
  } else {
      Float_t E = this->E(mass);
      return 0.5*log((E+mPt)/(E-mPt));
  }
}

Float_t StEStructTrack::Eta(Float_t mass) const {
  if (0 == mass) {
      return mEta;
  } else {
      return this->Rapidity(mass);
  }
}

Float_t StEStructTrack::Rapidity(Float_t mass) const { 
  Float_t E = this->E(mass);
  return 0.5*log((E+mPz)/(E-mPz)); 
}

Float_t StEStructTrack::Dca() const { 
  return (sqrt((mBxPrimary*mBxPrimary)+(mByPrimary*mByPrimary)+(mBzPrimary*mBzPrimary))); 
}

Float_t StEStructTrack::DcaPrimary() const { 
  return (sqrt((mBxPrimary*mBxPrimary)+(mByPrimary*mByPrimary)+(mBzPrimary*mBzPrimary))); 
}

Float_t StEStructTrack::DcaGlobal() const { 
  return (sqrt((mBxGlobal*mBxGlobal)+(mByGlobal*mByGlobal)+(mBzGlobal*mBzGlobal))); 
}

/**********************************************************************
 *
 * $Log: StEStructTrack.cxx,v $
 * Revision 1.9  2010/09/02 21:26:29  prindle
 * Track: Added ToF pid information, modify dEdx, add combined pid code.
 *
 * Revision 1.8  2010/03/02 21:47:18  prindle
 *   Support to retrieve track radius when it crosses endplate
 *   Add way to retrieve centrality
 *
 * Revision 1.7  2008/12/02 23:45:48  prindle
 * Added curvature and calculation of OuterMidTpcPoint.
 *
 * Revision 1.6  2006/02/22 22:06:07  prindle
 * Removed all references to multRef (?)
 *
 * Revision 1.5  2005/09/14 17:21:19  msd
 * Simplified helix fitting by taking helix from mudst instead of calculating from scratch
 *
 * Revision 1.4  2005/07/07 19:31:13  fisyak
 * Add default for mHelix
 *
 * Revision 1.3  2005/03/03 01:32:03  porter
 * fixed a bug setting 4-momentum and added data (+accessors)
 * to the track class
 *
 * Revision 1.2  2004/06/28 23:24:11  chunhuih
 *
 * added 'const' specification to some member functions, including some of the
 * return types, so that they can be used by a const StEStructTrack object.
 *
 * Revision 1.1  2003/10/15 18:20:51  porter
 * initial check in of Estruct Analysis maker codes.
 *
 *
 *********************************************************************/











