// $Id: St2011pubSpinMaker.h,v 1.2 2012/07/12 20:49:21 balewski Exp $
//
//*-- Author : Jan Balewski, MIT


#ifndef STAR_St2011pubSpinMaker
#define STAR_St2011pubSpinMaker

/*!
 *                                                                     
 * \class  St2011pubSpinMaker

 * \author Jan Balewski, MIT
 * \date   August 2009
 * \brief  spin sorting of W's 
 *
 *
 *
 */                                                                      

#ifndef StMaker_H
#include "StMaker.h"
#endif
#include <TString.h>

class St2011WMaker;

class St2011pubSpinMaker : public StMaker {
 private:

  float par_QPTlow,par_QPThighET0,par_QPThighET1, par_QPThighA ,par_QPThighB; // cuts to drop questionable reco charge charges
  float par_leptonEta1, par_leptonEta2; // narrow the range
  int par_useNoEEMC;

  St2011WMaker *wMK; // W-algo maker with all data
  TString core; // name attached to all histos

  // histograms
  TObjArray *HList;
  enum {mxHA=32}; TH1 * hA[mxHA];
  
  void initHistos();
  void bXingSort();  
 public: 
  St2011pubSpinMaker(const char *name="2011pubSpin");
  virtual       ~St2011pubSpinMaker(){};
  virtual Int_t  Init();
  virtual Int_t  InitRun  (int runumber);
  virtual Int_t  Make();
  void setHList(TObjArray * x){HList=x;}
  void setEta(float x, float y) { par_leptonEta1=x; par_leptonEta2=y;};
  void setQPT(float x){  par_QPTlow=x;}
  void setNoEEMC() {par_useNoEEMC=1;}

  void attachWalgoMaker(St2011WMaker *mk) { wMK=mk;}
  virtual Int_t FinishRun(int runumber);


  /// Displayed on session exit, leave it as-is please ...
  virtual const char *GetCVS() const {
    static const char cvs[]="Tag $Name:  $ $Id: St2011pubSpinMaker.h,v 1.2 2012/07/12 20:49:21 balewski Exp $ built "__DATE__" "__TIME__ ; 
    return cvs;
  }

  ClassDef(St2011pubSpinMaker,0)   //StAF chain virtual base class for Makers
};

#endif


// $Log: St2011pubSpinMaker.h,v $
// Revision 1.2  2012/07/12 20:49:21  balewski
// added spin info(star: bx48, bx7, spin4) and maxHtDSM & BTOW to Wtree
// removed dependence of spinSortingMaker from muDst
// Now Wtree can be spin-sorted w/o DB
// rdMu.C & readWtree.C macros modified
// tested so far on real data run 11
// lot of misc. code shuffling
//
// Revision 1.1  2011/02/10 20:33:25  balewski
// start
//
