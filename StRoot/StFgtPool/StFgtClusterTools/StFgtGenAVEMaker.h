///
#ifndef _ST_FGT_GEN_AGV_EFF_MAKER_
#define _ST_FGT_GEN_AGV_EFF_MAKER_

#include "StMaker.h"
//#include "StFgtQaMaker.h"
#include <TH2D.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include "StFgtGeneralBase.h"
//#include "StRoot/StEvent/StFgtCollection.h"


struct AVPoint
{
  Double_t x;
  Double_t y;
  Double_t z;
  Double_t r;
  Double_t phi;
  Double_t rCharge;
  Double_t phiCharge;
  Double_t rSize;
  Double_t phiSize;
  Int_t rMaxAdc;
  Int_t phiMaxAdc;
  Int_t rMaxAdcInt;
  Int_t phiMaxAdcInt;
  Int_t geoIdR;
  Int_t geoIdPhi;

  Int_t dID;
  Int_t quadID;
  AVPoint(){};
  AVPoint(Double_t mx, Double_t my, Double_t mz, Double_t mr, Double_t mPhi, Int_t mdID,Int_t mQuad,  Double_t mRCharge, Double_t mPhiCharge, Double_t mRSize, Double_t mPhiSize)
  {
    geoIdR=-1;
    geoIdPhi=-1;
    x=mx;
    y=my;
    z=mz;
    r=mr;
    phi=mPhi;
    dID=mdID;
    quadID=mQuad;
    rCharge=mRCharge;
    phiCharge=mPhiCharge;
    rSize=mRSize;
    phiSize=mPhiSize;
    rMaxAdc=-1;
    phiMaxAdc=-1;
    rMaxAdcInt=-1;
    phiMaxAdcInt=-1;


  }
};

struct AVTrack
{
  Double_t mx;
  Double_t my;
  Double_t ax;
  Double_t ay;
  Double_t ipZEv;
  Double_t ipZ;
  Double_t chi2;
  Double_t dca;
  Double_t trkZ;


  AVTrack(){};
  AVTrack(Double_t m_mx, Double_t m_my, Double_t m_ax, Double_t m_ay, Double_t m_ipZ=0.0,Double_t m_chi2=0.0)
  {
    mx=m_mx;
    my=m_my;
    ax=m_ax;
    ay=m_ay;
    ipZEv=m_ipZ;
    chi2=m_chi2;
    dca=-9999;
    trkZ=-9999;
    ipZ=-9999;
  }
};

class StFgtCollection;

class StFgtGenAVEMaker : public StFgtGeneralBase {
 public:
  StFgtGenAVEMaker(const Char_t* name="FgtGenAVEMaker");
  virtual ~StFgtGenAVEMaker();
   Int_t Init();
   Int_t Make();
   Int_t Finish();
   //   Bool_t checkPulse(StFgtHit* pClus);
   virtual const char *GetCVS() const
   {static const char cvs[]="Tag $Name:  $ $Id: StFgtGenAVEMaker.h,v 1.3 2012/04/26 20:08:18 avossen Exp $ built "__DATE__" "__TIME__ ; return cvs;}
 protected:
   ofstream* outTxtFile;
   Short_t getQuadFromCoo(Double_t x, Double_t y);
   pair<Double_t,Double_t> getChargeRatio(Float_t r, Float_t phi, Int_t iD, Int_t iq);
   Bool_t printArea(Float_t r, Float_t phi, Int_t iD, Int_t iq);
   Bool_t getTrack(vector<AVPoint>& points, Double_t ipZ);
   pair<double,double> getDca(  vector<AVTrack>::iterator it);
   vector<AVTrack> m_tracks;
   // for accessing the data
   StFgtCollection *mFgtCollectionPtr;
   Double_t getRPhiRatio(vector<generalCluster>::iterator hitIterBegin, vector<generalCluster>::iterator hitIterEnd);
   Double_t findClosestPoint(double xE, double yE, Int_t iD);
   Bool_t isSomewhatEff(Float_t r, Float_t phi, Int_t iD, Int_t iq);
   Double_t findClosestStrip(Char_t layer, double ord, Int_t iD, Int_t iQ);
   // for knowing what & how to plot


   // threshold, in units of # sigma above average
   Float_t mPedThres;
   //   Double_t getRPhiRatio(StSPtrVecFgtHitConstIterator hitIterBegin, StSPtrVecFgtHitConstIterator hitIterEnd);
   //   Double_t getRPhiRatio();
   TH2D** radioPlotsEff;
   TH2D** radioPlotsNonEff;

   TH2D** radioPlotsEffR;
   TH2D** radioPlotsNonEffR;

   TH2D** radioPlotsEffPhi;
   TH2D** radioPlotsNonEffPhi;

   TH2D** radioPlotsEffLoose;
   TH2D** radioPlotsNonEffLoose;


   TH1D** rPhiRatioPlots;
   TH1D** rEff;
   TH1D* hChargeAsym;
   TH1D* hChargeRatio;
   TH1D** rNonEff;
   TH2D* chargeRatioInEffDisk;
   TH2D* chargeAsymInEffDisk;
   TH2D* chargeCorrInEffDisk;
   TH2D* tpcFgtZVertexCorr;
   TH2D* tpcFgtZVertexCorr2;
   TH2D* tpcFgtZVertexCorr3;




   TH2D** chargeCorr;
   TH1D** h_clusterSizeR;
   TH1D** h_clusterSizePhi;
   TH1D** h_clusterChargeR;
   TH1D** h_clusterChargePhi;

   TH2D* hIp;
   TH1D* hIpZ;
   TH1D* hIpDca;


   TH1D* hTrkZ;
   TH1D* hChi2;
   TH1D* hBx;
   TH1D* hBy;
   TH1D* hResidua;

   TH1D* hMx;
   TH1D* hMy;

   TFile* myRootFile;
   int runningEvtNr;
   int hitCounter;
   int hitCounterR;


   //THD2** 


 private:   
      ClassDef(StFgtGenAVEMaker,1);

};

#endif

