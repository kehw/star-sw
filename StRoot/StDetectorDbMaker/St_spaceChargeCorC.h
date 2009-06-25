#ifndef St_spaceChargeCorC_h
#define St_spaceChargeCorC_h

#include "TChair.h"
#include "tables/St_spaceChargeCor_Table.h"
#include "StDetectorDbRichScalers.h"
#include "StDetectorDbMagnet.h"
#include "StChain/StChain.h"
class St_spaceChargeCorC : public TChair {
 public:
  spaceChargeCor_st 	*Struct(Int_t i = 0) 	{return ((St_spaceChargeCor*) Table())->GetTable()+i;}
  UInt_t     	getNumRows()                	{return GetNRows();}
  Double_t 	fullFieldB(Int_t i = 0) 	{return Struct(i)->fullFieldB;}
  Double_t 	halfFieldB(Int_t i = 0) 	{return Struct(i)->halfFieldB;}
  Double_t 	zeroField(Int_t i = 0) 	        {return Struct(i)->zeroField;}
  Double_t 	halfFieldA(Int_t i = 0) 	{return Struct(i)->halfFieldA;}
  Double_t 	fullFieldA(Int_t i = 0) 	{return Struct(i)->fullFieldA;}
  Double_t 	satRate(Int_t i = 0) 	        {return Struct(i)->satRate;}
  Float_t 	factor(Int_t i = 0) 	        {return Struct(i)->factor;}
  Float_t 	detector(Int_t i = 0) 	        {return Struct(i)->detector;}
  Float_t 	offset(Int_t i = 0) 	        {return Struct(i)->offset;}
  Float_t 	getEWRatio(Int_t i = 0)	        {return Struct(i)->ewratio;}
  Double_t      getSpaceChargeCorrection(Double_t scaleFactor, Int_t i = 0){
    Double_t value = 0;
    if(scaleFactor < -.75 && scaleFactor > -1.25) value = fullFieldB(i);
    else if(scaleFactor < -0.25)	          value = halfFieldB(i);
    else if(scaleFactor < .25)	                  value = zeroField(i);
    else if(scaleFactor < 0.75)	                  value = halfFieldA(i);
    else if(scaleFactor < 1.25)	                  value = fullFieldA(i);
    return value;
  }
  Double_t getSpaceChargeCorrection(){return  getSpaceChargeCorrection(StDetectorDbMagnet::instance()->getScaleFactor());}
  Double_t getSpaceChargeCoulombs(Double_t scaleFactor){
    StDetectorDbRichScalers* scalers = StDetectorDbRichScalers::instance();
    if (! scalers ) return 0;
    Double_t zf = zeroField(0); // potential validity margin for scalers
    if (zf>0 && zf<1) scalers->setValidityMargin(zf);
    Double_t coulombs = 0;
    bool use_powers = (StMaker::GetChain()->GetDate() > 20090101);
    for (int row=0;row< (int) getNumRows();row++) {
      Double_t mult = 0;
      switch ((int) getSpaceChargeDetector(row)) {
        case (0) : mult = scalers->getMult(); break;
        case (1) : mult = scalers->getBBCX(); break;
        case (2) : mult = scalers->getZDCX(); break;
        case (3) : mult = scalers->getZDCEast()+scalers->getZDCWest(); break;
        case (4) : mult = scalers->getBBCEast()+scalers->getBBCWest(); break;
        case (5) : mult = scalers->getZDCEast(); break;
        case (6) : mult = scalers->getZDCWest(); break;
        case (7) : mult = scalers->getBBCEast(); break;
        case (8) : mult = scalers->getBBCWest(); break;
        case (9) : mult = scalers->getBBCYellowBkg(); break;
        case (10): mult = scalers->getBBCBlueBkg(); break;
        default  : mult = 0.;
      }
      Double_t saturation = getSpaceChargeSatRate(row);
      Double_t correction = getSpaceChargeCorrection(scaleFactor,row);
      Double_t factor     = getSpaceChargeFactor(row);
      Double_t offset     = getSpaceChargeOffset(row);
      Double_t intens = (mult < saturation) ? mult : saturation;
      if (use_powers) coulombs += ::pow(intens-offset,factor) * correction ;
      else coulombs += factor * (intens-offset) * correction ;
    }
    return coulombs;
  };
  Double_t getSpaceChargeCoulombs(){return getSpaceChargeCoulombs(StDetectorDbMagnet::instance()->getScaleFactor());}
  Double_t getSpaceChargeSatRate(Int_t i = 0) {return satRate(i);}
  Float_t  getSpaceChargeFactor(Int_t i = 0)  {return factor(i);}
  Float_t  getSpaceChargeDetector(Int_t i = 0){return detector(i);}
  Float_t  getSpaceChargeOffset(Int_t i = 0)  {return offset(i);}

 protected:
  St_spaceChargeCorC(St_spaceChargeCor *table=0) : TChair(table) {}
  virtual ~St_spaceChargeCorC() {}
 private:
  ClassDefChair(St_spaceChargeCor, spaceChargeCor_st )
  ClassDef(St_spaceChargeCorC,1) //C++ TChair for spaceChargeCor table class
};

class St_spaceChargeCorR1C : public St_spaceChargeCorC {
 public:
  static St_spaceChargeCorR1C* 	instance();
 protected:
  St_spaceChargeCorR1C(St_spaceChargeCor *table=0) : St_spaceChargeCorC(table) {}
  virtual ~St_spaceChargeCorR1C() {fgInstance = 0;}
 private:
  static St_spaceChargeCorR1C* fgInstance;
  ClassDef(St_spaceChargeCorR1C,1) //C++ TChair for spaceChargeCor table class
};
class St_spaceChargeCorR2C : public St_spaceChargeCorC {
 public:
  static St_spaceChargeCorR2C* 	instance();
 protected:
  St_spaceChargeCorR2C(St_spaceChargeCor *table=0) : St_spaceChargeCorC(table) {}
  virtual ~St_spaceChargeCorR2C() {fgInstance = 0;}
 private:
  static St_spaceChargeCorR2C* fgInstance;
  ClassDef(St_spaceChargeCorR2C,1) //C++ TChair for spaceChargeCor table class
};
#endif
