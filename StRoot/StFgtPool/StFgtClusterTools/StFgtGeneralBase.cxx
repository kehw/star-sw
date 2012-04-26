#include "StFgtGeneralBase.h"
#include "StRoot/StFgtUtil/geometry/StFgtGeom.h"
#include "StRoot/StEvent/StEvent.h"
#include "StRoot/StEvent/StFgtCollection.h"
#include "StRoot/StEvent/StFgtHitCollection.h"
#include "StRoot/StEvent/StFgtHit.h"

#include "StRoot/StMuDSTMaker/COMMON/StMuDst.h"
#include "StRoot/StMuDSTMaker/COMMON/StMuFgtStrip.h"
#include "StRoot/StMuDSTMaker/COMMON/StMuFgtCluster.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StarClassLibrary/StThreeVectorF.hh"

#include <set>

#define ONE_HIT_PER_QUAD


StFgtGeneralBase::StFgtGeneralBase(const Char_t* name): StMaker( name ), evtNr(0)
{
  pClusters=new vector<generalCluster>*[6];
  pClusters[0]=&clustersD1;
  pClusters[1]=&clustersD2;
  pClusters[2]=&clustersD3;
  pClusters[3]=&clustersD4;
  pClusters[4]=&clustersD5;
  pClusters[5]=&clustersD6;

  pStrips=new vector<generalStrip>[50]; //for each quad so we don't have to lookup so much...
  StFgtDbMaker *fgtDbMkr = static_cast< StFgtDbMaker* >( GetMakerInheritsFrom( "StFgtDbMaker" ) );
  if( !fgtDbMkr ){
    LOG_FATAL << "StFgtDb not provided and error finding StFgtDbMaker" << endm;
  }
    mDb = fgtDbMkr->getDbTables();
    if( !mDb ){
      LOG_FATAL << "StFgtDb not provided and error retrieving pointer from StFgtDbMaker '"
		<< fgtDbMkr->GetName() << endm;
    }
  chargeMaxAdcCorr=new TH2D("chargeMaxAdcCorr","chargeMaxAdcCorr",50,0,10000,50,0,1000);
    chargeMaxAdcIntCorr=new TH2D("chargeMaxIntAdcCorr","chargeMaxAdcIntCorr",50,0,10000,50,0,1000);
    hIpZEv=new TH1D("IP_ZEv","IP_ZEv",70,-150,150);

}
Int_t StFgtGeneralBase::Finish()
{
  TCanvas c;
  chargeMaxAdcCorr->Draw("colz");
  c.SaveAs("chargeMaxAdcCorr.png");
  chargeMaxAdcIntCorr->Draw("colz");
  c.SaveAs("chargeMaxAdcIntCorr.png");
  hIpZEv->Draw();
  c.SaveAs("ipZEv.png");
  cout <<"done saving" <<endl;
}
Int_t StFgtGeneralBase::Make()
{
  Int_t ierr=kStOk;
  evtNr++;
  clustersD1.clear();
  clustersD2.clear();
  clustersD3.clear();
  clustersD4.clear();
  clustersD5.clear();
  clustersD6.clear();
  //  cout << " clearing strips..." <<endl;
  for(int i=0;i<50;i++)
    {
      pStrips[i].clear();
    }
  //  cout << " done" <<endl;

  //  fillFromStEvent();
  fillFromMuDst();

  return ierr;
}


Int_t StFgtGeneralBase::fillFromStEvent()
{
   Int_t ierr = kStFatal;

   StEvent* eventPtr = 0;
   StFgtCollection *fgtCollectionPtr = 0;
   StFgtHitCollection *fgtHitColPtr = 0;

   eventPtr = (StEvent*)GetInputDS("StEvent");

   if( eventPtr ) {
      fgtCollectionPtr = eventPtr->fgtCollection();

      if( fgtCollectionPtr ){
         // got this far, so flag that this is the right input.
         ierr = kStOk;

         // loop over discs
         for( Int_t disc = 0; disc < kFgtNumDiscs; ++disc ){
            fgtHitColPtr = fgtCollectionPtr->getHitCollection( disc );

            if( fgtHitColPtr ){
               const StSPtrVecFgtHit& hitVec = fgtHitColPtr->getHitVec();
               StSPtrVecFgtHitConstIterator hitIter;
	       set<int> quadsHit;
               Int_t idx = 0;
               for( hitIter = hitVec.begin(); hitIter != hitVec.end(); ++hitIter, ++idx )
		 {
		   Short_t quad, disc, strip;
		   Char_t layer; 
		   Double_t posR=(*hitIter)->getPositionR();
		   Double_t posPhi=(*hitIter)->getPositionPhi();
		   Int_t geoId=(*hitIter)->getCentralStripGeoId();
		   Double_t discZ=StFgtGeom::getDiscZ(disc);
		   StFgtGeom::decodeGeoId((*hitIter)->getCentralStripGeoId(),disc, quad, layer, strip);
#ifdef ONE_HIT_PER_QUAD
		   if(quadsHit.find(quad)!=quadsHit.end())
		     {
		       continue;
		     }
		   quadsHit.insert(quad);
#endif

		   //				  Int_t clusterSizePhi=(*hitIter)->getStripWeightMap().size();
		   Int_t clusterSize=(*hitIter)->getStripWeightMap().size();
		   Double_t clusterCharge=(*hitIter)->charge();
		   pClusters[disc]->push_back(generalCluster(geoId,layer,discZ,posPhi,posR,quad,disc,strip, clusterSize, clusterCharge));
		   mapGeoId2Cluster[geoId]=((pClusters[disc]->size()-1));
		 }
            };
         };
      };
   };

   return ierr;

}

Int_t StFgtGeneralBase::fillFromMuDst()
{
  //    cout <<"general base, muDST filler" <<endl;
   Int_t ierr = kStFatal;

   // get pointer to input
   const StMuDst* muDst = (const StMuDst*)GetInputDS("MuDst");
  if( muDst )
    {
      StMuEvent *event = static_cast<StMuEvent*>(muDst->event());
      if( event ){
	const StThreeVectorF& v = event->primaryVertexPosition();
	
	//	ipZ=v.z();

	StEventInfo &info=event->eventInfo();
	int nPrimV=muDst->numberOfPrimaryVertices();
	if(1 && nPrimV>0) { // copy vertex info
	  StMuPrimaryVertex* V= muDst->primaryVertex(0);// select highest rank vertex
	  assert(V);
	  const StThreeVectorF &r=V->position();
	  vtxZ=r.z();
	  const StThreeVectorF &er=V->posError();
	  float rank=V->ranking();

	  //printf("primVert  Z=%.2f ±%.2f   rank=%5.1e  (nVert=%d) \n",r.z(),er.z() ,rank, nPrimV);
	  //	  myJFgtEve->vertRank=rank;
	  vtxRank=rank;
	  vtxZ=r.z();
	  if(rank>0) { // use the vertex
	    //   myJFgtEve->vertPos=TVector3(r.x(),r.y(),r.z());
	    //	    myJFgtEve->vertErr=TVector3(er.x(),er.y(),er.z());
	    hIpZEv->Fill(vtxZ);
	  }
	  else
	    {
	      //	      return false; //primary vertex is not good...
	    }
	}
	//	if(fabs(ipZ)>20)
	// return false;
	//	cout <<" found z vertex: " << ipZ <<endl;
      }
    }
  else
    {
      cout <<"no muDST!!!" <<endl;
    }
   if( muDst ){
      TClonesArray *fgtClusters = muDst->fgtArray( muFgtClusters );
      TClonesArray *fgtStrips=muDst->fgtArray(muFgtStrips);
      if( fgtClusters ){
         // flag this is the correct input
         ierr = kStOk;
	 //	 cout <<" got fgt clust" <<endl;
         Int_t nClusters = fgtClusters->GetEntriesFast();

         for( Int_t i = 0; i < nClusters; ++i ){
            StMuFgtCluster* clus = static_cast< StMuFgtCluster* >( (*fgtClusters)[i] );
            if( clus )
	      {
		//		cout<<"found cluster in muDST " << endl;
		   Int_t geoId=clus->getCentralStripGeoId();
		   //		   cout <<"cluster from muDst: geoId: " << geoId <<endl;
		   Short_t quad, disc, strip;
		   Char_t layer;
		   StFgtGeom::decodeGeoId(geoId,disc, quad, layer, strip);
		   Double_t posR=clus->getR();
		   Double_t posPhi=clus->getPhi();
		   Double_t discZ=StFgtGeom::getDiscZ(disc);
		   StFgtGeom::decodeGeoId(geoId,disc, quad, layer, strip);
		   Int_t clusterSize=clus->getNumStrips();
		   Double_t clusterCharge=clus->getCharge();
		   //		   cout <<"looking at geoID: " << geoId <<" r: " << posR <<" phi: " << posPhi <<" charge: " << clusterCharge <<" size: " << clusterSize <<endl;
		   //		   cout <<" disc: " << disc <<" quad: " << quad << " layer: " << layer <<endl;
		   pClusters[disc]->push_back(generalCluster(geoId,layer,discZ,posPhi,posR,quad,disc,strip,clusterSize,clusterCharge));
		   mapGeoId2Cluster[geoId]=((pClusters[disc]->size()-1));
	      }
		     //               addClus( i, clus->getCentralStripGeoId(), clus->getR(), clus->getPhi() );
         };
      }
            if(fgtStrips)
      //      if(false)
	{
	  //	  cout <<"got strip " <<endl;
	  ierr = kStOk;
         Int_t nStrips = fgtStrips->GetEntriesFast();
         for( Int_t i = 0; i < nStrips; ++i ){
            StMuFgtStrip* strip = static_cast< StMuFgtStrip* >( (*fgtStrips)[i] );
	    //	    cout <<"got strip" <<endl;
            if( strip )
	      {
		//		cout <<"got strip again" <<endl;
		Int_t geoId=strip->getGeoId();
		Int_t cSeedType=strip->getClusterSeedType();
		Double_t charge=strip->getCharge();
		Double_t chargeUncert=strip->getChargeUncert();
		Short_t quad, disc, stripI;
		Char_t layer;
		StFgtGeom::decodeGeoId(geoId,disc, quad, layer, stripI);
		Double_t ped=0.0; //get from DB
		Double_t pedErr=0.0; 
		Int_t rdo, arm, apv, chan; 
		mDb->getElecCoordFromGeoId(geoId, rdo,arm,apv,chan);
		Int_t elecId = StFgtGeom::encodeElectronicId( rdo, arm, apv, chan );
		ped = mDb->getPedestalFromElecId( elecId );
		pedErr = mDb->getPedestalSigmaFromElecId( elecId );

		if(quad<4)
		  {
		    pStrips[disc*4+quad].push_back(generalStrip(geoId,ped,pedErr,cSeedType,charge, chargeUncert));
		    Double_t maxAdc=-9999;
		    for(int j=0;j<7;j++)
		      {
			pStrips[disc*4+quad].back().adc[j]=strip->getAdc(j);
			if(strip->getAdc(j)>maxAdc)
			  maxAdc=strip->getAdc(j);

		      }

		    pStrips[disc*4+quad].back().maxAdc=maxAdc;
		    //		    	    cout <<"just set max Adc to " << maxAdc <<endl;
		    if(mapGeoId2Cluster.find(geoId)!=mapGeoId2Cluster.end())
		      {
			//have to find all strips...
			//			cout << "hoping to see " << geoId << " have: " << (*pClusters[disc])[mapGeoId2Cluster[geoId]].centralStripGeoId<<endl;
			(*pClusters[disc])[mapGeoId2Cluster[geoId] ].centerStripIdx=(pStrips[disc*4+quad].size()-1);
			(*pClusters[disc])[mapGeoId2Cluster[geoId] ].maxAdc=maxAdc;
			//			cout <<"maxAdc of cluster: "<< maxAdc <<" "<< geoId <<" center strip geo:" << (*pClusters[disc])[mapGeoId2Cluster[geoId] ].centralStripGeoId;
		      }
		  }
		
	      }
	 }
	 ///find the other strips for clusters....
	 //	 cout <<"finding the other strip " <<endl;
	 for(int iDx=0;iDx<6;iDx++)
	   {
	     vector<generalCluster>::iterator it=pClusters[iDx]->begin();
	     for(;it!=pClusters[iDx]->end();it++)
	       {
		 Int_t centerStripId=it->centerStripIdx;
		 //		 cout <<"looking at cluster with center strip id:" << it->centralStripGeoId <<endl;
		 if(centerStripId<0)
		   continue;
		 //		 cout << " strip geo id is " << pStrips[iDx*2+it->quad][centerStripId].geoId <<endl;
		 Int_t stripCounter=(centerStripId-1);
		 Double_t maxChargeInt=it->maxAdc;
		 //		 cout <<"max charge:" <<maxChargeInt<<endl;
		 Int_t oldGeoId=(pStrips[iDx*4+it->quad])[centerStripId].geoId;
		 //		 cout <<" going down " <<endl;
		 while(stripCounter>=0)     
		   {
		     Int_t seedType=(pStrips[iDx*4+it->quad])[stripCounter].seedType;
		     //		     cout <<"looking at geo id: " << (pStrips[iDx*2+it->quad])[stripCounter].geoId <<endl;
		     //		     cout <<"seedtype:" << seedType <<endl;
		     if(!((seedType==kFgtClusterPart)||(seedType==kFgtClusterEndUp)||(seedType==kFgtClusterEndDown)||(seedType==kFgtSeedType1)||(seedType==kFgtSeedType2)||(seedType==kFgtSeedType3)))
		       break;
		     //		     cout <<"seed fulfilled " << endl;
		     if(fabs(oldGeoId-(pStrips[iDx*4+it->quad])[stripCounter].geoId)>1)
		       break;

		     maxChargeInt+=(pStrips[iDx*4+it->quad])[stripCounter].maxAdc;
		     //		     		     cout <<"found another strip, " <<  (pStrips[iDx*4+it->quad])[stripCounter].geoId << " maxChargenow:" << maxChargeInt <<endl;
		     oldGeoId=(pStrips[iDx*4+it->quad])[stripCounter].geoId;
		     stripCounter--;
		   }
		 //and go up
		 stripCounter=(centerStripId+1);
		 //		 cout <<"going up:" << centerStripId <<endl;
		 while(stripCounter<=(pStrips[iDx*4+it->quad]).size())
		   {
		     Int_t seedType=(pStrips[iDx*4+it->quad])[stripCounter].seedType;
		     if(!((seedType==kFgtClusterPart)||(seedType==kFgtClusterEndUp)||(seedType==kFgtClusterEndDown)||(seedType==kFgtSeedType1)||(seedType==kFgtSeedType2)||(seedType==kFgtSeedType3)))
		       break;
		     if(fabs(oldGeoId-(pStrips[iDx*4+it->quad])[stripCounter].geoId)>1)
		       break;
		     //		     		     cout <<"found another strip, maxCharge now (up):" <<  (pStrips[iDx*4+it->quad])[stripCounter].geoId  <<" maxChargeInt "<<endl;
		     maxChargeInt+=(pStrips[iDx*4+it->quad])[stripCounter].maxAdc;
		     oldGeoId=(pStrips[iDx*4+it->quad])[stripCounter].geoId;
		     stripCounter++;
		   }
		 //		 cout <<"setting max adcInt to :" << maxChargeInt <<endl;
		 it->maxAdcInt=maxChargeInt;
		 chargeMaxAdcCorr->Fill(it->clusterCharge,it->maxAdc);
		 chargeMaxAdcIntCorr->Fill(it->clusterCharge,maxChargeInt);
	       }

	     //kFgtClusterPart kFgtClusterEndUp kFgtClusterEndDown kFgtSeedType1 kFgtSeedType2 kFgtSeedType3
	   }

	}
   }
   return ierr;
};

ClassImp(StFgtGeneralBase);
