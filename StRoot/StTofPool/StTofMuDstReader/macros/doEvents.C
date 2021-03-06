#include "iostream.h"

class     StChain;
StChain  *chain=0;
class     St_db_Maker;
St_db_Maker *dbMk =0;
class StFileI;
StFileI *setFiles =0;

Int_t iEvt=0,istat=0,nEvents=0;
//		ProtoTypes

void doEvents(const Char_t *file="StRoot/StTofPool/StTofMuDstReader/macros/test.lis", const Char_t *outputname="test.root");

              
// ------------------ Here is the actual method -----------------------------------------
void doEvents(const Char_t *fileList, const Char_t *outputname)
{
  Int_t nEvents = 100;
  Int_t nfiles = 100;

  //
  // First load some shared libraries we need
  //
  if (gClassTable->GetID("TTable") < 0) {
    gSystem->Load("libStar");
    gSystem->Load("libPhysics");
  }  
  gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
  loadSharedLibraries();
  gSystem->Load("StarMagField");
  gSystem->Load("StMagF");
  gSystem->Load("StTpcDb");
//  gSystem->Load("StDbUtilities");
  gSystem->Load("StDaqLib");
  gSystem->Load("StDbBroker");
  gSystem->Load("StDetectorDbMaker");
  gSystem->Load("StDbUtilities");
  gSystem->Load("St_db_Maker");

  gSystem->Load("StEvent");
  gSystem->Load("StEventMaker");
  gSystem->Load("StarMagField");
  gSystem->Load("StTofUtil");   
  gSystem->Load("StTofMaker");  
 
  gSystem->Load("libtpc_Tables");
  gSystem->Load("libGeom");
  gSystem->Load("St_g2t"); 
  gSystem->Load("geometry");
  gSystem->Load("St_geant_Maker");

  gSystem->Load("StTofMuDstReader");
    //
    // Handling depends on whether file is a ROOT file or XDF file
    //
    chain  = new StChain("StChain");


    int NwGeant=5000000, IwType=0, NwPaw=0;
    St_geant_Maker *geantMk = new St_geant_Maker("geant",NwGeant,NwPaw,IwType);
    geantMk->LoadGeometry("detp geometry y2005F");
    geantMk->SetActive(kFALSE);
    

    delete setFiles; setFiles =0;
    if (fileList) {     //Normal case
      setFiles= new StFile();
    } else        {     //
      return;
    }
    ifstream* inputStream = new ifstream;
    inputStream->open(fileList);
    if (!(inputStream)) {
      cout << "can not open list file" << endl;
      return;
    }
    cout<<"Open file list: --- "<<fileList<<endl;

    char line[512];
    for (;inputStream->good();) {
      inputStream->getline(line,512);
      if  ( inputStream->good() ) {
	//cout<<" root file "<<line<<endl;
	TFile *ftmp = new TFile(line);
	 //----------
         if (!(ftmp->IsOpen())) {
           cout<<line<<" open failed ! not chained"<<endl;
           continue;   
         }
         if (ftmp->IsZombie()) {
           cout<<"sth. very wrong with "<<line<<", not chained "<<endl;
           continue;   
         }
         if (ftmp->TestBit(1024)) { 
           cout<<"revocer procedure applied to "<<line<<endl;
           continue;   
         } 
	 //--------------------------           
         if( ftmp && ftmp->IsOpen() && ftmp->GetNkeys()) {
	    cout << "add file " << line << endl;
	    setFiles->AddFile(line);
	 } else {
	     cout << " cannot open file " << line << endl;
    	 }
    	 delete ftmp;
       }
    }

   StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",fileList,"MuDst.root",nfiles);
   muDstMaker->SetStatus("*",0);
   muDstMaker->SetStatus("MuEvent",1);
   muDstMaker->SetStatus("PrimaryVertices",1);
   muDstMaker->SetStatus("PrimaryTracks",1);
   muDstMaker->SetStatus("TofRawData",1);
   muDstMaker->SetStatus("TofData",1);
   muDstMaker->SetStatus("TofHit",1);

   gSystem->Load("StTofMuDstReader");

   StTofMuDstEval *tofEval = new StTofMuDstEval("tofMuDstEval",muDstMaker);
   StTofMuDstReader *tofMaker = new StTofMuDstReader("tofMaker",outputname,muDstMaker);


    //
    // Initialize chain
    //
    Int_t iInit = chain->Init();
    if (iInit) chain->Fatal(iInit,"on init");
    chain->PrintInfo();
    
    //
    // Event loop
    //
    int istat = 0, i = 1;
EventLoop: if (i <= nEvents && istat != 2) {
   
   cout << endl << "============================ Event " << i
	<< " start ============================" << endl;
   
   chain->Clear();
   istat = chain->Make(i);
   if (istat == 2) 
     {cout << "Last  event processed. Status = " << istat << endl;}
   if (istat == 3) 
     {cout << "Error event processed. Status = " << istat << endl;}
   
   //   gObjectTable->Print();
   i++;
   goto EventLoop;
 }
    
    i--;
  cout<<endl<<"============================ Event "<<i<<" finish ============================"<<endl;

  //
  // Chain Finish
  //
  if (nEvents > 1) {
    chain->Finish();
  }


}





