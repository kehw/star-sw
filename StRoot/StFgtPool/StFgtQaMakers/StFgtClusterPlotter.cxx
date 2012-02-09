/////
#include "StFgtClusterPlotter.h"
#include "StRoot/StEvent/StFgtCollection.h"
#include "StRoot/StEvent/StFgtHitCollection.h"
#include "StRoot/StEvent/StFgtHit.h"
#include "StRoot/StFgtUtil/geometry/StFgtGeom.h"
#include <TH2D.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>



/** grab the cluster container, fill histograms

*/
Int_t StFgtClusterPlotter::Make()
{
   Int_t ierr = StFgtQaMaker::Make();
   cout <<"in plotter make " << endl;
 for(int iDx=0;iDx<kFgtNumDiscs;iDx++)
   {
     cout <<"trying to get disc " << iDx << endl;
     StFgtHitCollection* clusterCol=mFgtCollectionPtr->getHitCollection(iDx);

     if(clusterCol)
       {
	 cout <<"got collection, looking at hits ..  " <<endl;
	 const StSPtrVecFgtHit &hitVec=clusterCol->getHitVec();
	 StSPtrVecFgtHitConstIterator hitIter;
	 for(hitIter=hitVec.begin();hitIter != hitVec.end();hitIter++)
	   {
	     Int_t iq=(*hitIter)->getQuad();
	     Float_t phi=(*hitIter)->getPositionPhi();
	     Float_t r=(*hitIter)->getPositionR();
	     Int_t geoId=(*hitIter)->getCentralStripGeoId();
	     Float_t charge=(*hitIter)->charge();
	     Int_t numStrips=(*hitIter)->getStripWeightMap().size();
	     Short_t tDisc, tQuad,tStrip;
	     Char_t tLayer;
	     //get strip id from the geo id
	     StFgtGeom::decodeGeoId(geoId,tDisc,tQuad,tLayer,tStrip);     
	     //	     cout <<"filling charge with phi: " << phi << " r : " << r << " charge: " << charge <<" numStrips: " << numStrips<< " strip nr: " << tStrip<<endl;
	     //	     cout<<"disc nr: " << iDx <<" quad: " << iq << " numquads: " << kFgtNumQuads <<endl;
	     hCChargePosSpacePhi[iDx*kFgtNumQuads+iq]->Fill(phi,charge);
	     hCChargePosSpaceR[iDx*kFgtNumQuads+iq]->Fill(r,charge);
	     hClusSizePhi[iDx*kFgtNumQuads+iq]->Fill(phi,numStrips);
	     hClusSizePhi[iDx*kFgtNumQuads+iq]->Fill(phi,numStrips);
	     hClusSizeR[iDx*kFgtNumQuads+iq]->Fill(r,numStrips);
	     hCChargeElecSpace[iDx*kFgtNumQuads+iq]->Fill(tStrip,charge);
	     hClusSizeElecSpace[iDx*kFgtNumQuads+iq]->Fill(tStrip,numStrips);
	   }
       }
   }


 return ierr;
};

StFgtClusterPlotter::StFgtClusterPlotter( const Char_t* name)
{
  StFgtQaMaker( name, 0,0, "qName" );

};

StFgtClusterPlotter::~StFgtClusterPlotter()
{
  //delete histogram arrays
};


Int_t StFgtClusterPlotter::Finish(){
      gStyle->SetPalette(1);
  cout <<"cluster plotter finish funciton " <<endl;
   Int_t ierr = kStOk;
  TCanvas* cChargePhi=new TCanvas("chargePhi","chargePhi",850,1100);
  cChargePhi->Divide(kFgtNumDiscs,kFgtNumQuads);
  TCanvas* cChargeR=new TCanvas("chargeR","chargeR",850,1100);
  cChargeR->Divide(kFgtNumDiscs,kFgtNumQuads);
  TCanvas* cClusSizePhi=new TCanvas("clusSizePhi","clusSizePhi",850,1100);
  cClusSizePhi->Divide(kFgtNumDiscs,kFgtNumQuads);
  TCanvas* cClusSizeR=new TCanvas("clusSizeR","clusSizeR",850,1100);
  cClusSizeR->Divide(kFgtNumDiscs,kFgtNumQuads);
  TCanvas* cChargeElecSpace=new TCanvas("cChargeElecSpace","cChargeEledId",850,1100);
  cChargeElecSpace->Divide(kFgtNumDiscs,kFgtNumQuads);
  TCanvas* cClusSizeElecSpace=new TCanvas("clusSizeElecSpace","clusSizeEledId",850,1100);
  cClusSizeElecSpace->Divide(kFgtNumDiscs,kFgtNumQuads);



  for(Int_t iD=0;iD<kFgtNumDiscs;iD++)
    {
      for(Int_t iQ=0;iQ<kFgtNumQuads;iQ++)
	{
	  cout <<"drawing disc " << iD <<" quad " << iQ <<endl;
	  cChargePhi->cd(iD*kFgtNumQuads+iQ+1);
	  hCChargePosSpacePhi[iD*kFgtNumQuads+iQ]->Draw("colz");
	  cChargeR->cd(iD*kFgtNumQuads+iQ+1);
	  hCChargePosSpaceR[iD*kFgtNumQuads+iQ]->Draw("colz");
	  cClusSizePhi->cd(iD*kFgtNumQuads+iQ+1);
	  hClusSizePhi[iD*kFgtNumQuads+iQ]->Draw("colz");
	  cClusSizeR->cd(iD*kFgtNumQuads+iQ+1);
	  hClusSizeR[iD*kFgtNumQuads+iQ]->Draw("colz");
	  cChargeElecSpace->cd(iD*kFgtNumQuads+iQ+1);
	  hCChargeElecSpace[iD*kFgtNumQuads+iQ]->Draw("colz");
	  cClusSizeElecSpace->cd(iD*kFgtNumQuads+iQ+1);
	  hClusSizeElecSpace[iD*kFgtNumQuads+iQ]->Draw("colz");
	}
    }


  cout <<"saving .." <<endl;
  cChargePhi->SaveAs("cChargePhi.pdf");
  cChargePhi->SaveAs("cChargePhi.png");

  cChargeR->SaveAs("cChargeR.pdf");
  cChargeR->SaveAs("cChargeR.png");

  cClusSizePhi->SaveAs("cClusSizePhi.pdf");
  cClusSizePhi->SaveAs("cClusSizePhi.png");

  cClusSizeR->SaveAs("cClusSizeR.pdf");
  cClusSizeR->SaveAs("cClusSizeR.png");


  cChargeElecSpace->SaveAs("cClusSizeElecSpace.pdf");
  cChargeElecSpace->SaveAs("cClusSizeElecSpace.png");

  cClusSizeElecSpace->SaveAs("cClusSizeElecSpace.pdf");
  cClusSizeElecSpace->SaveAs("cClusSizeElecSpace.png");



   return ierr;
};


/**
construct histograms

*/
Int_t StFgtClusterPlotter::Init(){
   Int_t ierr = kStOk;
   hClusterCharge=new TH1D("clusterCharge","clusterCharge",100, 0, 1000);
   char buffer[100];
   hCChargePosSpacePhi=new TH2D*[kFgtNumDiscs*kFgtNumQuads];
   hCChargePosSpaceR=new TH2D*[kFgtNumDiscs*kFgtNumQuads];
   hClusSizePhi=new TH2D*[kFgtNumDiscs*kFgtNumQuads];
   hClusSizeR=new TH2D*[kFgtNumDiscs*kFgtNumQuads];
   hCChargeElecSpace=new TH2D*[kFgtNumDiscs*kFgtNumQuads];
   hClusSizeElecSpace=new TH2D*[kFgtNumDiscs*kFgtNumQuads];


   for(int iD=0;iD<kFgtNumDiscs;iD++)
     {
       for(int iQ=0;iQ<kFgtNumQuads;iQ++)
	 {
	   sprintf(buffer,"clusterChargeDisk%d_Quad%d_phi",iD,iQ);
	   hCChargePosSpacePhi[iD*kFgtNumQuads+iQ]=new TH2D(buffer,buffer,100,-4,4, 100, 0, 2000);
	   sprintf(buffer,"clusterChargeDisk%d_Quad%d_R",iD,iQ);
	   hCChargePosSpaceR[iD*kFgtNumQuads+iQ]=new TH2D(buffer,buffer,100,10,35, 100, 0, 2000);
	   sprintf(buffer,"clusterSizeDisk%d_Quad%d_phi",iD,iQ);
	   hClusSizePhi[iD*kFgtNumQuads+iQ]=new TH2D(buffer,buffer,100,-4,4, 20, 0, 20);
	   sprintf(buffer,"clusterSizeDisk%d_Quad%d_R",iD,iQ);
	   hClusSizeR[iD*kFgtNumQuads+iQ]=new TH2D(buffer,buffer,100,10,35, 20, 0, 20);
	   sprintf(buffer,"clusterSizeDisk%d_Quad%d_ElecSpace",iD,iQ);
	   hClusSizeElecSpace[iD*kFgtNumQuads+iQ]=new TH2D(buffer,buffer,100,0,1000, 20, 0, 20);
	   sprintf(buffer,"clusterChargeDisk%d_Quad%d_ElecSpace",iD,iQ);
	   hCChargeElecSpace[iD*kFgtNumQuads+iQ]=new TH2D(buffer,buffer,100,0,1000, 100, 0, 2000);
	 }
     }
   return ierr;
};
ClassImp(StFgtClusterPlotter);
