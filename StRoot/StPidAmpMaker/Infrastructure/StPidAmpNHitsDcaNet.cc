/***************************************************************************
 *
 * $Id: StPidAmpNHitsDcaNet.cc,v 1.7 2000/07/22 22:11:32 aihong Exp $
 *
 * Author: Aihong Tang & Richard Witt (FORTRAN Version),Kent State U.
 *         Send questions to aihong@cnr.physics.kent.edu
 ***************************************************************************
 *
 * Description:part of StPidAmpMaker package
 *             StPidAmpNHitsDcaNet is a basic unit for building a
 *             NHitsDca instance of StPidAmpChannelCollection
 ***************************************************************************
 *
 * $Log: StPidAmpNHitsDcaNet.cc,v $
 * Revision 1.7  2000/07/22 22:11:32  aihong
 * move some include files to StEventUtilities & change include path
 *
 * Revision 1.6  2000/07/12 15:38:35  aihong
 * update for real data
 *
 * Revision 1.5  2000/05/01 16:59:26  aihong
 * clean up
 *
 * Revision 1.4  2000/04/14 16:07:30  aihong
 * change BetheBlock to BetheBloch :-)
 *
 * Revision 1.3  2000/04/12 23:39:06  aihong
 * change heightExpected for deuteron
 *
 * Revision 1.2  2000/04/11 15:34:23  aihong
 * change to adapt dividing trks by channel for faster filling
 *
 * Revision 1.1  2000/04/09 16:24:51  aihong
 * adding into package
 *
 *
 **************************************************************************/


#include "TF1.h"

#include "StPidAmpMaker/Infrastructure/StPidAmpNHitsDcaNet.hh"
#include "StEventUtilities/StPidAmpConst.hh"
#include <fstream.h>

extern double Linear(double *,double *);
extern double MaxllBoltz(double *,double *);

//-------------------------------
StPidAmpNHitsDcaNet::StPidAmpNHitsDcaNet():StPidAmpNet(){

  StPidAmpNHitsDcaNet::funcAmpPt=&MaxllBoltz;
  StPidAmpNHitsDcaNet::funcResoPt=&Linear;
}
//-------------------------------
StPidAmpNHitsDcaNet::StPidAmpNHitsDcaNet(StPidAmpParticle def, StPidAmpChannelInfo channelInfo):StPidAmpNet(def, channelInfo){

  StPidAmpNHitsDcaNet::funcAmpPt=&MaxllBoltz;
  StPidAmpNHitsDcaNet::funcResoPt=&Linear;
}

//------------------------------
void StPidAmpNHitsDcaNet::fitBand(){

   double varyRange=0.05;


   TF1 *mBetheBlochFcn = new TF1 ("mBetheBlochFcn",funcBandPt, BandsBegin,BandsEnd,NBandParam);
  

   mBetheBlochFcn->SetParLimits(0,(mBandParams[0]-varyRange*fabs(mBandParams[0])),(mBandParams[0]+varyRange*fabs(mBandParams[0])));
   mBetheBlochFcn->SetParLimits(1,(mBandParams[1]-varyRange*fabs(mBandParams[1])),(mBandParams[1]+varyRange*fabs(mBandParams[1])));
   mBetheBlochFcn->SetParLimits(2,(mBandParams[2]-varyRange*fabs(mBandParams[2])),(mBandParams[2]+varyRange*fabs(mBandParams[2])));
          
   mBetheBlochFcn->SetParameter(3,double(mParticleType.charge()));
   mBetheBlochFcn->SetParameter(4,double(mParticleType.mass()));
   mBetheBlochFcn->SetParameter(5, double(CalibFactor));
   mBetheBlochFcn->SetParameter(6, double(Saturation));
   mBetheBlochFcn->SetParLimits(3, 1,1); //fixed.
   mBetheBlochFcn->SetParLimits(4, 1,1);
   mBetheBlochFcn->SetParLimits(5, 1,1);
   mBetheBlochFcn->SetParLimits(6, 1,1);

   if ((bandGraph()->GetN())>0){
   bandGraph()->Fit("mBetheBlochFcn","R"); //R means use the range in TF1.
   mBandParams.clear();
   for (int i=0; i<NBandParam; i++) 
   mBandParams.push_back(mBetheBlochFcn->GetParameter(i));
   }


   delete mBetheBlochFcn;


}



//------------------------------
void StPidAmpNHitsDcaNet::fitAPath(StPidAmpPath& path, StPidAmpTrkVector* trks){

     double varyRange4Height=0.35;  
     double varyRange4Center=0.2;
     double varyRange4Width =0.3;
     double heightExpected=(maxPoint(ampGraph(),true));
     double centerExpected=fabs(mParticleType.maxllPeakPos());
     double widthExpected;
     double midNhits=(mChannelInfo.cutVector())[0].midPoint();

     if ((mChannelInfo.cutVector())[2].lowEdge()>0.1)
            widthExpected =0.186*midNhits+6.77; //higher dca, wider the width.
     else   widthExpected =0.23*midNhits+5.99; 
    //the less nhits, the wider. here I use a linear func
     //to describe the width change with nhits.

     if (mParticleType.id()==2||mParticleType.id()==3) {
        //electron's amp center has strong dependence of NHits.
        centerExpected=maxPoint(ampGraph(),false);
       if   (maxPoint(ampGraph(),false)<0.1) {
	 heightExpected=heightExpected*0.25;
	 widthExpected =mParticleType.maxllWidth()*10.0;
       }  else widthExpected =mParticleType.maxllWidth()*2.25;
       
     }

     if (mParticleType.id()==8||mParticleType.id()==9)
	 heightExpected=heightExpected*1.4;

     if (mParticleType.id() == 14 || mParticleType.id()==15){
     //for far off primary vertex proton tracks,
     //the center shift to left when has for less nhits. (why ?) 
     //so the centerExpected could not be a fixed number.
       if ((mChannelInfo.cutVector())[2].lowEdge()>0.1){
         centerExpected=maxPoint(ampGraph(),false);
           if (mParticleType.id()==15)
           heightExpected=heightExpected*0.2;
       }
     }

     if (mParticleType.id()==11||mParticleType.id()==12) {
       //do not expect much off-prvx pions
       if ((mChannelInfo.cutVector())[2].lowEdge()>0.1)
     heightExpected=heightExpected*0.2;
     }

 
      if (mParticleType.id()==45) {
       if ((mChannelInfo.cutVector())[2].lowEdge()>0.1)
          heightExpected=heightExpected*1.0;
       else //do not expect deuteron comes from primary vertex.
	  heightExpected=heightExpected*1e-06;
     }    

   //now fit the path.
   TF1 *mMaxllBoltzFcn = 
   new TF1 ("mMaxllBoltzFcn",funcAmpPt, BandsBegin,BandsEnd,NAmpParam);
    
   mMaxllBoltzFcn->SetParLimits(0,heightExpected*(1.0-varyRange4Height),heightExpected*(1.0+varyRange4Height));
   mMaxllBoltzFcn->SetParLimits(1,centerExpected*(1.0-varyRange4Center),centerExpected*(1.0+varyRange4Center));
   mMaxllBoltzFcn->SetParLimits(2,widthExpected*(1.0-varyRange4Width),widthExpected*(1.0+varyRange4Width));

   mMaxllBoltzFcn->SetParameter(3,double(fabs(mParticleType.start())));
   mMaxllBoltzFcn->SetParLimits(3, 1,1); //fixed par.

    
   if (((path.pathGraph())->GetN())>0){
     (path.pathGraph())->Fit("mMaxllBoltzFcn","R");
     (path.pathParams())->clear();
   for (int i=0; i<NAmpParam; i++) 
   (path.pathParams())->push_back(mMaxllBoltzFcn->GetParameter(i));
   }

     delete mMaxllBoltzFcn;
}

//------------------------------
void StPidAmpNHitsDcaNet::fitAmp(StPidAmpTrkVector* trks){
     double varyRange4Height=0.25;  
     double varyRange4Center=0.3;
     double varyRange4Width =0.3;
     double heightExpected=(maxPoint(ampGraph(),true));
     double centerExpected=fabs(mParticleType.maxllPeakPos());
     double widthExpected;
     double midNhits=(mChannelInfo.cutVector())[0].midPoint();

     if ((mChannelInfo.cutVector())[2].lowEdge()>0.1)
            widthExpected =0.186*midNhits+6.77; //higher dca, wider the width.
     else   widthExpected =0.23*midNhits+5.99; 
    //the less nhits, the wider. here I use a linear func
     //to describe the width change with nhits.

     if (mParticleType.id()==2||mParticleType.id()==3) {
        //electron's amp center has strong dependence of NHits.
        centerExpected=maxPoint(ampGraph(),false);
       if   (maxPoint(ampGraph(),false)<0.1) {
	 widthExpected =mParticleType.maxllWidth()*20.0;
       }  else widthExpected =mParticleType.maxllWidth()*2.25;
       
     }

     if (mParticleType.id()==8||mParticleType.id()==9){
	 heightExpected=heightExpected*1.4;
	 if ((mChannelInfo.cutVector())[2].lowEdge()>0.1){
	   heightExpected=heightExpected*0.6;//reduce muon's contamination
	  widthExpected=widthExpected*0.6;
	 }
     }       



     if (mParticleType.id() == 14 || mParticleType.id()==15){
     //for far off primary vertex proton tracks,
     //the center shift to left when has for less nhits. (why ?) 
     //so the centerExpected could not be a fixed number.
       if ((mChannelInfo.cutVector())[2].lowEdge()>0.1){
         centerExpected=maxPoint(ampGraph(),false);
	 if (mParticleType.id()==15) {
	   heightExpected=heightExpected*0.5;//reduce noise contam.
           widthExpected=widthExpected*2.0;
	 }
       }
     }

     if (mParticleType.id()==11||mParticleType.id()==12) {
       //do not expect much off-prvx pions
       if ((mChannelInfo.cutVector())[2].lowEdge()>0.1){
           heightExpected=heightExpected*0.5;//reduce noise contam.
           widthExpected=widthExpected*2.0;
       }
     }

 
      if (mParticleType.id()==45) {
       if ((mChannelInfo.cutVector())[2].lowEdge()>0.1)
          heightExpected=heightExpected*1.0;
       else //do not expect deuteron comes from primary vertex.
	  heightExpected=heightExpected*1e-06;
     }    









   TF1 *mMaxllBoltzFcn = 
   new TF1 ("mMaxllBoltzFcn",funcAmpPt, BandsBegin,BandsEnd,NAmpParam);
    
   mMaxllBoltzFcn->SetParLimits(0,heightExpected*(1.0-varyRange4Height),heightExpected*(1.0+varyRange4Height));
   mMaxllBoltzFcn->SetParLimits(1,centerExpected*(1.0-varyRange4Center),centerExpected*(1.0+varyRange4Center));
   mMaxllBoltzFcn->SetParLimits(2,widthExpected*(1.0-varyRange4Width),widthExpected*(1.0+varyRange4Width));

   mMaxllBoltzFcn->SetParameter(3,double(fabs(mParticleType.start())));
   mMaxllBoltzFcn->SetParLimits(3, 1,1); //fixed par.
    
   if ((ampGraph()->GetN())>0){
     ampGraph()->Fit("mMaxllBoltzFcn","R");
     mAmpParams.clear();
   for (int i=0; i<NAmpParam; i++) 
   mAmpParams.push_back(mMaxllBoltzFcn->GetParameter(i));
   }


  delete mMaxllBoltzFcn;

}
//----------------------------------
void StPidAmpNHitsDcaNet::fitReso(){


  TF1 *mResoFcn = new TF1 ("mResoFcn",funcResoPt, BandsBegin,BandsEnd,NResoParam);

  /*   
         double adj=0.2;     
 
        switch(mParticleType.id()){

        case 2  :  mResoFcn->SetParLimits(0,0.16*(1.0-adj),0.16*(1.0+adj));
	              break;// ePlus
        case 3  :  mResoFcn->SetParLimits(0,0.16*(1.0-adj),0.16*(1.0+adj));
	              break;//eMinus
        case 8  :  mResoFcn->SetParLimits(0,0.12*(1.0-adj),0.12*(1.0+adj));
	              break;//piPlus
        case 9  :  mResoFcn->SetParLimits(0,0.12*(1.0-adj),0.12*(1.0+adj));
	              break;//piMinus
        case 11 :  mResoFcn->SetParLimits(0,0.08*(1.0-adj),0.135*(1.0+adj));
	              break;//kaonPlus
        case 12 :  mResoFcn->SetParLimits(0,0.08*(1.0-adj),0.135*(1.0+adj));
	              break;//kaonMinus
        case 14 :  mResoFcn->SetParLimits(0,0.07*(1.0-adj),0.12*(1.0+adj));
	              break;//proton
        case 15 :  mResoFcn->SetParLimits(0,0.07*(1.0-adj),0.12*(1.0+adj));
	              break;//proton
        case 45 :  mResoFcn->SetParLimits(0,0.14*(1.0-adj),0.14*(1.0+adj));
	              break;//deuteron
        default :  mResoFcn->SetParLimits(0,0.13*(1.0-adj),0.13*(1.0+adj));

	}

										  */

 mResoFcn->SetParLimits(1,-0.05517,0.055172414);
if (mParticleType.id()==14||mParticleType.id()==15)
 mResoFcn->SetParLimits(1,-0.06017,0.060172414);

        if ((linrGraph()->GetN())>0){
        linrGraph()->Fit("mResoFcn","R");
        mResoParams.clear();
	for (int i=0; i<NResoParam; i++) 
        mResoParams.push_back(mResoFcn->GetParameter(i));
	}

        delete mResoFcn;

}
//------------------------------
ostream& StPidAmpNHitsDcaNet::put(ostream& s) const{// for calling the right put() by operator.

  s<<endl;
  s<<name().c_str()<<endl;
 
  for (unsigned i=0; i<mBandParams.size(); i++) s<<mBandParams[i]<<" ";
  s<<endl;
  for (unsigned j=0; j<mAmpParams.size(); j++) s<<mAmpParams[j]<<" ";
  s<<endl;
  for (unsigned k=0; k<mResoParams.size(); k++) s<<mResoParams[k]<<" ";
  s<<endl;
    
  return s;
}
//----------------------------------
void StPidAmpNHitsDcaNet::fillPathFittedSlices(){

   StPidAmpPathIter pathIter;
   StPidAmpPath*    thePath;

   for (pathIter=pathVector()->begin(); pathIter!=pathVector()->end(); pathIter++){//path loop

   thePath=*pathIter;
   TF1 *mMaxllBoltzFcn = new TF1 ("mMaxllBoltzFcn",funcAmpPt, BandsBegin,BandsEnd,NAmpParam);
   
   for(int i=0; i<NAmpParam; i++) 
   mMaxllBoltzFcn->SetParameter(i,(*(thePath->pathParams()))[i]);
   
  

       
   StPidAmpSliceIter sliceIter;
   StPidAmpSlice*    theSlice;

   for (sliceIter=sliceVector()->begin(); sliceIter!=sliceVector()->end(); sliceIter++){//slice loop

   theSlice=*sliceIter;
   (theSlice->pathFittedSlice())->SetBinContent((thePath->index()+1), mMaxllBoltzFcn->Eval(fabs(theSlice->meanRig()),0,0));
   //path index begin with 0, bin # begin with 1.

   }
   
   delete mMaxllBoltzFcn, theSlice;

   }

   //now turn on StPidAmpSlice::mUsePathFitResult       
   StPidAmpSliceIter sliceIter;
   StPidAmpSlice* theSlice;
   for (sliceIter=sliceVector()->begin(); sliceIter!=sliceVector()->end(); sliceIter++){//slice loop
     theSlice=*sliceIter;
     theSlice->usePathFitResult();
   }


}





      



