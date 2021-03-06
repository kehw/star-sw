// $Id: bfcread_hist_list_all.C,v 1.15 2006/08/15 21:42:40 jeromel Exp $ 
// $Log: bfcread_hist_list_all.C,v $
// Revision 1.15  2006/08/15 21:42:40  jeromel
// Fix rhic -> rhic.bnl.gov
//
// Revision 1.14  2000/07/26 19:53:45  lansdell
// made changes for creating new QA histograms
//
// Revision 1.13  2000/06/12 19:58:54  lansdell
// loaded global shared library
//
// Revision 1.12  2000/04/18 20:44:53  kathy
// St_DataSet,St_DataSetIter,St_Table classes are nowchanged to TDataSet,TDataSetIter,TTable
//
// Revision 1.11  2000/04/12 15:06:52  kathy
// changed all macros that read DSTs to load Tables from libraries: gen,sim,global,dst instead of ALL Tables (previously loaded St_Tables); currently, if you are using DEV to read a DST in NEW,PRO, you must comment out the loading of libtpc_Tables because of a mismatch with tpt_track table
//
// Revision 1.10  2000/01/19 16:29:51  kathy
// update macros to use default input files in /afs/rhic.bnl.gov/star/data/samples
//
// Revision 1.9  2000/01/05 22:12:03  kathy
// changed input file to current one
//
// Revision 1.8  1999/12/01 21:30:11  kathy
// added input TopDirTree to bfcread_hist* macros in order to tell which top level directory hist file has since sometimes its not bfcTree; cleaned up print statements in bfcread_dst*hist.C macros; two new macros bfcread_dst_*QA_outhistfile.C added which read dst file and book and fill histograms and write out a new *.hist.root file, instead of just sending hist to postscript - this new *.hist.root file can then be read into bfcread_hist*.C to look at it --- note that topdirtree is different!
//
// Revision 1.7  1999/11/30 19:23:05  kathy
// changed bfcread_dst*.C so that MakerHist is hardwired in instead of being input; wrote better documentation in bfcread_hist*.C so that it explains where top level directory is set
//
// Revision 1.6  1999/11/19 20:13:22  kathy
// cleaned up macros to remove uneccessary lines; also added info about new tables to QA* macros
//
// Revision 1.5  1999/11/03 21:35:35  kathy
// small fixes for use of StIOMaker - had it wrong before
//
// Revision 1.4  1999/11/03 19:02:54  kathy
// changes to default input files and output file names - needed by perl script for testing
//
// Revision 1.3  1999/11/03 17:13:00  kathy
// fixed macros so they use StIOMaker instead of StTreeMaker
//
// Revision 1.2  1999/09/21 15:07:03  kathy
// change to have notes on input values at top of each macro, also clean up notes on usage and remove the usage of method St_QA_Maker::SetPntrToHistUtil which is not going to be used now that I made St_QA_Maker totally independent of the histogram printing
//
// Revision 1.1  1999/09/20 20:09:02  kathy
// bfcread_hist_list_all now lists all histograms in hist.root file; bfcread_hist_list now only lists those that are in the Maker that is input; bfcread_hist_to_ps prints and draws the hist that are in the input Maker, bfcread_dst_QAhist.C reads .dst.root file - runs QA_Maker and prints and draws the QA histograms
//
//=======================================================================
// owner: Kathy Turner
// what it does: reads the *.hist.root file produced from a chain 
//               (such as bfc) and
//               then lists all histogram branches and the names and
//               titles of the histograms in the branches
//
// inputs: MainFile - *.hist.root file from bfc output
//         TopDirTree - top level directory tree in your input hist file
//                (this is 3rd argument of constructor for StTreeMaker that
//                 you probably used to write the *.hist.root file)
//           NOTE: if you ran bfc, then the TopDirTree = bfcTree !!// 
//
//=======================================================================

class StChain;
class TDataSet;

StChain *chain;
TDataSet *Event;

void bfcread_hist_list_all(
  const Char_t *MainFile=
     "/afs/rhic.bnl.gov/star/data/samples/gstar.hist.root",
  const Char_t *TopDirTree="bfcTree")
{
  cout << "bfcread_hist_list_all.C, input hist file = " 
       << MainFile << endl;
  cout << "bfcread_hist_list_all.C, top level directory in hist file = " 
       << TopDirTree << endl;
//
    gSystem->Load("St_base");
    gSystem->Load("StChain");
    gSystem->Load("StUtilities");
    gSystem->Load("StIOMaker");
    gSystem->Load("StarClassLibrary");
    gSystem->Load("libglobal_Tables");


// setup chain with IOMaker - can read in .dst.root, .dst.xdf files
  StIOMaker *IOMk = new StIOMaker("IO","r",MainFile,TopDirTree);
  IOMk->SetDebug();
  IOMk->SetIOMode("r");
  IOMk->SetBranch("*",0,"0");                 //deactivate all branches
//  IOMk->SetBranch("tpc_tracks",0,"r"); //activate tpc_tracks Branch
//  IOMk->SetBranch("geantBranch",0,"r"); //activate geant Branch
  IOMk->SetBranch("histBranch",0,"r"); //activate dst Branch


// --- now execute chain member functions
  IOMk->Init();
  IOMk->Clear();
  IOMk->Make();

// Now look at histograms:
    Event = IOMk->GetDataSet("hist");
      if (Event) {
	cout << "Here is list of all histograms: " << endl;
          Event->ls(9); 
      }

   TDataSetIter nextHistList(Event);
   St_ObjectSet *histContainer = 0;
   TList *dirList = 0;
   while (histContainer = (St_ObjectSet *)nextHistList()) {
     dirList = (TList *) histContainer->GetObject();
     TIter nextHist(dirList);
     TObject  *o = 0;
     cout << " --- " << histContainer->GetName() << endl;
     while (o= nextHist()) {
       cout << " Hist name: " << o->GetName() << " ==> Title: " 
	    << o->GetTitle() << endl; 
     }
   }

}



