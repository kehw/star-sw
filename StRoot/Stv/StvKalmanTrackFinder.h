/// \File StvKalmanTrackFinder.h
/// \author Victor Perev 01/2010
#ifndef StvKalmanTrackFinder_HH
#define StvKalmanTrackFinder_HH
#include "StvTrackFinder.h"

class THelix3d;
class StvSeedFinder;
class StvDiver;
class StvPars;
class StvTrack;
class StvDiver;
class StvHitter;
class StvHitVector;
class StvNodePars;
class StvFitDers;
class StvFitErrs;
class StvHitCounter;
class StvKonst_st;
/// \class StvKalmanTrackFinder

class StvKalmanTrackFinder : public StvTrackFinder
{
public:
  StvKalmanTrackFinder(const char *name="KalmanTrackFinder");
   ~StvKalmanTrackFinder(){;}
 void   SetCons(const StvKonst_st*);
   int	FindTracks();
   int	Refit(int idir);
   int  FindTrack(int idir);
StvNode *MakeDcaNode(StvTrack *tk);
   int	FindPrimaries(const StvHits &vtxs);
   int  Swim(int idir,int opt, const double target[3]
            ,const StvNodePars *inpPar,const StvFitErrs *inpErr
            ,      StvNodePars *outPar,      StvFitErrs *outErr
            ,      StvFitDers  *derivFit);
   void	Reset();
   void	Clear(const char *opt="");

protected:
   void CountHits(const StvTrack *tk);
   

protected:
char mBeg[1];
int mHitIdTruth;
int mMinHits;		/*Min number of hits allowed MidEta*/
int mNorHits;		/*Normal number of hits allowed MidEta*/
int mGoodHits;		/*Min umber of Good hits in track MidEta*/
const StvKonst_st *mKons;
    StvHitCounter *mHitCounter;
    StvSeedFinder *mSeedFinder;
const THelixTrack *mSeedHelx;
      StvTrack    *mCurrTrak;
StvDiver *mDive;
StvHitter *mHitter;
char mEnd[1];
private:
ClassDef(StvKalmanTrackFinder,0);
};

#endif
