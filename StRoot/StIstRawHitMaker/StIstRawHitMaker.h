/***************************************************************************
*
* $Id: StIstRawHitMaker.h,v 1.17 2015/12/27 18:28:32 smirnovd Exp $
*
* Author: Yaping Wang, March 2013
****************************************************************************
* Description:
* Reads/un-pack a DAQ or SFS file; Accesses calibration DBs;
* Marks out bad channels/chips; Creates and fills the IST containers.
* Two working modes included:
* 1) calibration mode: save raw ADC value only
* 2) non-calibration (physics) mode: access calibration datasets, signal-like
*    raw hit decision, full raw hit info., ...
***************************************************************************/

#ifndef StIstRawHitMaker_hh
#define StIstRawHitMaker_hh

#include "StRoot/St_base/Stypes.h"
#include "StRoot/StChain/StRTSBaseMaker.h"
#include "StIstUtil/StIstConsts.h"

#include <vector>
#include <string>
#include <list>
#include <map>
#include <assert.h>

class StIstCollection;

class StIstRawHitMaker : public StRTSBaseMaker
{
public:
   StIstRawHitMaker( const char *name = "ist_raw_hit" );
   ~StIstRawHitMaker();
   Int_t Init();
   Int_t InitRun(Int_t runNumber);
   Int_t Make();
   void Clear( Option_t *opts = "" );

   void setIsCalibrationMode( bool isCaliMode = false )   { mIsCaliMode = isCaliMode;};
   void setHitCut(float hitCut = 5.)			{ mHitCut = hitCut;        };
   void setCmnCorrection( bool doCmnCorrection = false )	{ mDoCmnCorrection = doCmnCorrection; };
   void setCmnCut(float cmnCut = 3.)			{ mCmnCut = cmnCut;        };
   /// 0 - All data; 1 - non-ZS data; 2 - ZS data; 3 - ZS first data
   void setDataType(int nDataType = 0)		{ mDataType = nDataType;   };
   void setDoEmbedding(Bool_t doIt) {mDoEmbedding = doIt;}
   UChar_t getDataType() {return mDataType;} // 0 - All data; 1 - non-ZS data; 2 - ZS data; 3 - ZS first data

   // Get CVS
   virtual const char *GetCVS() const {
      static const char cvs[] = "Tag $Name:  $ $Id: StIstRawHitMaker.h,v 1.17 2015/12/27 18:28:32 smirnovd Exp $ built " __DATE__ " " __TIME__  ;
      return cvs;
   }

protected:
   Bool_t mIsCaliMode;
   Bool_t mDoEmbedding;
   Bool_t mDoCmnCorrection;
   //control paramters
   Float_t mHitCut, mCmnCut, mChanMinRmsNoiseLevel, mChanMaxRmsNoiseLevel, mApvMaxCmNoiseLevel;
   UChar_t mALLdata, mADCdata, mZSdata, mDefaultTimeBin, mCurrentTimeBinNum;
   UShort_t mMinNumOfRawHits, mMaxNumOfRawHits;

   StIstCollection *mIstCollectionPtr;
   StIstCollection *mIstCollectionSimuPtr; //raw ADC container from simu data

   std::vector< float > mCmnVec; ///< APV chip geom. index, common mode (CM) noise
   std::vector< float > mPedVec; ///< Channel elec. index, pedestal
   std::vector< float > mRmsVec; ///< Channel elec. index, RMS noise
   std::vector< float > mGainVec; ///< Channel elec. index, gain
   std::vector< int > mMappingVec; ///< Channel elec. index to geometry ID mapping
   std::vector< unsigned char > mConfigVec; ///< APV chip configuration status indexed by geom. id

private:

   void FillRawHitCollectionFromAPVData(unsigned char dataFlag, int ntimebin, int counterAdcPerEvent[], float sumAdcPerEvent[], int apvElecId,
      int (&signalUnCorrected)[kIstNumApvChannels][kIstNumTimeBins],
      float (&signalCorrected)[kIstNumApvChannels][kIstNumTimeBins]);
   void FillRawHitCollectionFromSimData();

   Int_t mDataType; ///<  0=all, 1=adc only, 2=zs only

   ClassDef(StIstRawHitMaker, 0);
};

#endif
