/***************************************************************************
 *
 * $Id: FCFMaker.cxx,v 1.20 2004/05/10 17:33:35 tonko Exp $
 *
 * Author: Jeff Landgraf, BNL Feb 2002
 ***************************************************************************
 *
 * Description:  Offline version of DAQ cluster finder
 *               
 * Input:  TPC RAW table dataset
 * Output: tphit Table
 *
 ***************************************************************************
 *
 * $Log: FCFMaker.cxx,v $
 * Revision 1.20  2004/05/10 17:33:35  tonko
 * Fixed small bug with cl_id and data dump in saveClusters
 *
 * Revision 1.19  2004/04/21 20:30:54  tonko
 * Added back-annotation and misc. cleanup
 *
 * Revision 1.18  2004/03/22 16:41:11  tonko
 * Added output to FCFMaker if FCF_DEBUG_OUTPUT is defined
 *
 * Revision 1.17  2004/03/15 15:28:35  tonko
 * Added TrackIDs in FCF and cleaned the includes
 *
 * Revision 1.16  2004/03/10 21:55:54  jml
 * support for simulations in FCFMaker
 *
 * Revision 1.15  2004/02/15 05:45:38  perev
 * Increase size of broken array
 *
 * Revision 1.14  2004/02/03 20:05:20  jml
 * *** empty log message ***
 *
 * Revision 1.13  2004/01/27 18:38:18  jeromel
 * Change SetDAQFlag to overloaded SetMode
 *
 * Revision 1.12  2004/01/27 16:30:15  jml
 * added FCF_ to defines
 *
 * Revision 1.11  2004/01/26 22:46:54  jml
 * debugging to see gains/t0, only get gains/t0 once, cleaner logging
 *
 * Revision 1.10  2004/01/26 19:42:25  jml
 * blah
 *
 * Revision 1.9  2004/01/22 18:36:11  jml
 * more updates to the logging
 *
 * Revision 1.8  2004/01/22 14:42:59  jml
 * fixed the logging
 *
 * Revision 1.7  2004/01/22 14:20:36  jml
 * Added cluster reading
 *
 * Revision 1.6  2003/11/17 18:53:00  jml
 * Preliminary tests look good
 *
 * Revision 1.5  2003/11/12 15:59:27  tonko
 * Arranged the default flags
 *
 * Revision 1.4  2003/09/19 15:48:13  tonko
 * Skip row 13. Charge cut is default.
 *
 * Revision 1.3  2003/09/17 19:57:48  tonko
 * Changed name of the class from DaqClf to RTSClientFCF
 *
 * Revision 1.2  2003/09/17 19:03:59  tonko
 * Fixed a small warning in BuildCPP
 *
 * Revision 1.1  2003/09/17 18:22:26  tonko
 * First seemingly working set
 *
 * Revision 1.3  2003/09/16 15:35:40  tonko
 * Deleted misc. ifdef code
 *
 * Revision 1.2  2003/09/15 20:17:33  tonko
 * First compilable Tonkos version
 *
 * Revision 1.8  2002/11/26 21:22:03  jml
 * pad width --> 11 for inner sector
 *
 * Revision 1.7  2002/11/25 19:49:04  jml
 * Changed mintmbk, maxtmbk, ntmbk = 10 to fix problem with loss of high pt tracks
 *
 * Revision 1.6  2002/09/05 15:48:07  jml
 * Now fill the deconvolution flag to help dedx
 *
 * Revision 1.5  2002/08/22 21:31:33  jml
 * Installed new version of fcfClass
 * This version is frozen for the 2002-2003 run and will be running on the i960s
 * (assuming no new bugs are found).  Also set best values for several
 * control flags
 *
 * Revision 1.4  2002/03/20 16:41:54  jml
 * Added pad by pad t0 corrections controlled by flags
 * 	no flag    -- full pad by pad corrections
 * 	'nopadt0'  -- no pad by pad corrections
 * 	'avgpadt0' -- correct according to clusters pad
 *
 * Revision 1.3  2002/03/08 20:33:43  jml
 * Instantiate and write to StEvent if "ittf" flag is set.
 *
 * Revision 1.2  2002/03/02 15:45:07  jml
 * updated the CVS log comments
 *
 **************************************************************************/

#include "StBFChain.h"
#include "St_DataSetIter.h"
#include "St_DataSet.h"
#include "StDAQMaker/StDAQReader.h"
#include "tables/St_raw_sec_m_Table.h"
#include "tables/St_raw_pad_Table.h"
#include "tables/St_raw_row_Table.h"
#include "tables/St_raw_seq_Table.h"
#include "tables/St_tcl_tphit_Table.h"
#include "tables/St_type_shortdata_Table.h"
#include "tables/St_tpcGain_Table.h"
#include "StDaqLib/TPC/trans_table.hh"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include <TError.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <StMessMgr.h>
#include <StEvent.h>
#include <StEvent/StTpcHitCollection.h>
#include <StEvent/StTpcHit.h>
#include <StThreeVectorF.hh>



#include <rtsSystems.h>
#include <fcfClass.hh>
#include <TPC/padfinder.h>
#include <TPC/rowlen.h>

#include "FCFMaker.h"

#include "StDbUtilities/StCoordinates.hh"
#include "StDetectorDbMaker/StDetectorDbTpcRDOMasks.h"
#include "StDaqLib/TPC/fee_pin.h"
#include "StDAQMaker/StDAQReader.h"

ClassImp(StRTSClientFCFMaker);


#ifdef FCF_DEBUG_OUTPUT
static FILE *ff ;	// used for the cluster dump
#endif


#if !defined(__CINT__) || defined(__MAKECINT__)
#include "Riostream.h"
#include "StMaker.h"
#include "tables/St_g2t_tpc_hit_Table.h"
#else
class StMaker;
class St_g2t_tpc_hit;
class g2t_tpc_hit_st;
#endif

#ifdef FCF_DEBUG_OUTPUT
struct Hit_t {
  double x, y, z, charge ; 
};
static Hit_t Hit;
//________________________________________________________________________________
static Hit_t *getHitInfo(int sector=1, int row=1, int track_id=1) {
  StMaker *mk = StMaker::GetChain();
  if (mk) {
    StMaker *geant = mk->Maker("geant");
    if (geant) {
      St_g2t_tpc_hit *hit = (St_g2t_tpc_hit *) geant->DataSet("g2t_tpc_hit");
      if (hit) {
	Int_t N = hit->GetNRows();
	g2t_tpc_hit_st *tpc_hit = hit->GetTable();
	for (int i = 0; i < N; i++, tpc_hit++) {
	  Int_t volId = tpc_hit->volume_id;
	  Int_t isDet  = (volId/100000);
	  if (isDet) continue; // pseudo pad row
	  volId  -= (isDet)*100000;
	  Int_t iSector = volId/100;
	  if (iSector != sector) continue;
	  volId  -= (iSector)*100;
	  Int_t iPadrow = volId;
	  if (iPadrow != row) continue;
	  if (tpc_hit->track_p != track_id) continue;
	  Hit.x = tpc_hit->x[0];
	  Hit.y = tpc_hit->x[1];
	  Hit.z = tpc_hit->x[2];
	  Hit.charge = tpc_hit->de;
	  //cout << "Hit s/r/i " << sector << "/" << row << "/" << track_id
	  //     << "\t x/y/z/charge " <<  Hit.x << "/" << Hit.y << "/" << Hit.z 
	  //     << "/" << Hit.charge << endl;
	  return &Hit;
	}
      }
    }
  } 
  return 0;
}
#endif

// Tonko: added a static global which counts events i.e. calls to "Make"
static int Event_counter ;

static class fcfAfterburner fcf_after;

// The raw cluster data...
// Three contributions...

// [sector][padrow][result_buffer]
static u_int croat_out[24][45][(FCF_MAX_CLUSTERS + 2) * 2];
static u_int croat_simu_out[24][45][(FCF_MAX_CLUSTERS+2)* 2];

// [sector][rb][mz][result_buffer]
static u_int daq_file_out[24][6][3][(FCF_MAX_CLUSTERS + 2) * 2 * 6];


// points to the raw data contributing...

static j_uintptr simu_resptr[24][45][3];
static j_uintptr croat_resptr[24][45][3];
static j_uintptr daq_file_resptr[24][45][3];

StRTSClientFCFMaker::StRTSClientFCFMaker(const char *name):StMaker(name)
{
  gMessMgr->Debug() << "Constructor for StRTSClientFCFMaker()" << endm;
  fcf = NULL;
  mCTransform = NULL;
}

StRTSClientFCFMaker::~StRTSClientFCFMaker() 
{
  gMessMgr->Debug() << "Destructor for StRTSClientFCFMaker()" << endm;
  if(fcf != NULL)
  {
    delete fcf;
    fcf = NULL;
  }

  if(mCTransform != NULL)
  {
    delete mCTransform;
    mCTransform = NULL;
  }
}

StDAQReader *daqReader;
StTPCReader *tpcReader;

void StRTSClientFCFMaker::SetMode(Int_t mode)
{
  switch(mode) {
  case 0x0:
    ignoreFileClusters = true;
    ignoreRawData = true;
    break;

  case 0x1:
    ignoreFileClusters = true;
    ignoreRawData = false;
    break;

  case 0x2:
    ignoreFileClusters = false;
    ignoreRawData = true;
    break;

  case 0x3:
    ignoreFileClusters = false;
    ignoreRawData = false;
    break;
  }

  if(ignoreRawData)
    printf("FCFMaker::SetMode Not calculating clusters from raw data\n");
  else
    printf("FCFMaker::SetMode Will calculate clusters from raw data if present\n");

  if(ignoreFileClusters)
    printf("FCFMaker::SetMode Not reading clusters from data file\n");
  else
    printf("FCFMaker::SetMode Will read clusters from data file if present\n");
}

Int_t StRTSClientFCFMaker::Init()
{
  PrintInfo();

  daqReader = NULL;
  tpcReader = NULL;

  //
  //  Disable the command line options in favor of hardcoded 
  //  values.   Here is an example of how to make command
  //  line options though...
  //
  //   StBFChain *chain = (StBFChain *)GetChain();
  //
  //   char *x = chain->GetOptionString("prfin");
  //   if(x) sscanf(x,"%le",&mPrfin);
  //   else mPrfin = .26;
  //

  ignoreFileClusters = false;
  ignoreRawData = false;

  mDp = .1;             // hardcoded errors
  mDt = .2;
  mDperp = .1;

  splitRows = 1;        // split padrows as if real DAQ on i960s
  doT0Corrections = 1;  // do the t0 corrections
  doGainCorrections = 1; // done by St_tpcdaq_Maker - shouldn't be!!! Tonko
  doZeroTruncation = 0; // don't 
  fillDeconFlag = 1;

  mCreate_stevent = 0;  // Use  StEvent for ittf
  mFill_stevent = 0;
  mFill_tphit = 1;

  mStEvent = NULL;
  mT_tphit = NULL;

  // gMessMgr->Info() << "dpad=" <<mDp<<endm;
  // gMessMgr->Info() << "dperp="<<mDperp<<endm;
  // gMessMgr->Info() << "dt="<<mDt<<endm;
  gMessMgr->Info() << "splitRows="<<splitRows<<" (Are rows to be split as on i960's)"<<endm;

  // Croat initializations
  //
  // croat_adcOff & croat_cppOff are static
  //
  fcf = new fcfClass(TPC_ID,NULL);

  for(int i=0;i<FCF_MAX_PADS_EVER+1;i++)
  {
    croat_adcOff[i] = (unsigned int)(&croat_adc[i][0]) - (unsigned int)(&croat_adc[0][0]);
    croat_cppOff[i] = (unsigned int)(&croat_cpp[i][0]) - (unsigned int)(&croat_cpp[0][0]);
  }
  fcf->adcOff = croat_adcOff;
  fcf->cppOff = (short unsigned int *)croat_cppOff;
  fcf->maxClusters = FCF_MAX_CLUSTERS;

  memset(t0Corr, 0, sizeof(t0Corr));

#ifdef FCF_DEBUG_OUTPUT
  ff = fopen("fcf.dta","w") ;
//  fcf_annotate_f = fopen("fcf_shape.dta","w") ;
#endif

  for(int i=0;i<24;i++) {
    for(int j=0;j<45;j++) {
      for(int k=0;k<FCF_MAX_PADS_EVER+1;k++) gainCorr[i][j][k] = 64;   // == 1.0 !!
    }
  }

  // Tonko: added startFlags
  memset(startFlags,0,sizeof(startFlags)) ;
  fcf->startFlags = startFlags ;

  // Get TPC Parameters
  St_DataSet *tpc = GetDataBase("tpc");
  assert(tpc);
  St_DataSet *tsspars = tpc->Find("tsspars");
  assert(tsspars);
  m_tsspar = (St_tss_tsspar *)tsspars->Find("tsspar");
  assert(m_tsspar);
  
  tss_tsspar_st *tsspar = m_tsspar->GetTable();
  tsspar->threshold = 1;

  fcf_after.setVerbose(false);
  return StMaker::Init();
}

// Tonko: this should hold aaaall the gain/T0 correction tables
// so that we don't have to redo them every time...

Int_t StRTSClientFCFMaker::InitRun(int run)
{
  fprintf(stderr,"StRTSClientFCFMaker::InitRun called with run %u...\n",run) ;
  
  Event_counter = 0 ;	// clear the count of events

  St_DataSet *dr = GetDataSet("StDAQReader");
  if(dr) 
    daqReader = (StDAQReader *)(dr->GetObject());

  if(daqReader == NULL) {
    printf("FCFMaker::InitRun No daqReader available...\n");
  }

  for(int i=0;i<24;i++) {
    for(int j=0;j<45;j++) {
      getCorrections(i+1,j);   // sector from 1, row from 0
    }
  }

  return kStOK ;
}

Int_t StRTSClientFCFMaker::Make()
{


  Event_counter++ ;	// got one more event...
  clustercount = 0 ;	// initialize clustercount...

  PrintInfo();



#ifdef FCF_ANNOTATE_CLUSTERS
	// zap the annotation structures in case we use them...
	memset(fcfPixA,0,sizeof(fcfPixA)) ;
#endif

  printf("<FCFMaker::Make> Making event %d...\n",Event_counter);

  // Hack for now untill ittf is in more complete shape...
  if(mCreate_stevent)
  {
    
    if(mStEvent != NULL)
    {
      delete mStEvent;
      mStEvent = NULL;
    }

    St_DataSetIter ods(m_DataSet);
    mStEvent = new StEvent();
    ods.Add(mStEvent);
  }
  
  // Coordinate transformer
  if(!gStTpcDb)
  {
    gMessMgr->Error() << "There is no gStTpcDb pointer\n" << endm;
    exit(0);
  }

  // need a coordinate transformer...
  if(!mCTransform)
  {
    mCTransform = new StTpcCoordinateTransform(gStTpcDb);
  }

  mDriftVelocity = gStTpcDb->DriftVelocity();
  // gMessMgr->Info() << "The drift velocity used = " << mDriftVelocity << endm;

  if(mFill_tphit)
  {
    // Set up tphit dataset...If exists use old one, else create it.
    St_DataSetIter outputDataSet(m_DataSet);
  
    mT_tphit = (St_tcl_tphit *)outputDataSet("tphit");
    if(mT_tphit == NULL)
    {
      mT_tphit = new St_tcl_tphit("tphit",10);
      outputDataSet.Add(mT_tphit);
    }
  }

  if(mFill_stevent)
  {
    mTpcHitColl = new StTpcHitCollection();
    assert(mTpcHitColl);
  }


  // Get the clusters
  // 
  // These fill the following results:
  //
  //     croat_out[][][]     croat_resptr[][][]
  //     daq_file_out[][][]  daq_file_resptr[][][]
  //

  int n_daq_file_cl = -1;
  int n_croat_cl = -1;
  if(!ignoreFileClusters)
  {
    //printf("FCFMaker: reading daq file clusters\n");
    n_daq_file_cl = build_daq_file_clusters();
    printf("<FCFMaker::Make> done reading daq file clusters (%d found)\n",n_daq_file_cl);
  }
  if(!ignoreRawData)
  {
    //printf("FCFMaker: calculating clusters from raw data\n");
    n_croat_cl = build_croat_clusters();
    printf("<FCFMaker::Make> done calculating clusters from raw data (%d found)\n",n_croat_cl);
  }

  int n_burned_daq_file_cl=-1;
  int n_burned_croat_cl=-1;
  int use_daq_file_clusters;

  if(n_daq_file_cl != -1)
    use_daq_file_clusters=1;
  else
    use_daq_file_clusters=0;

  int mismatch_tot=0;
  for(int s=0;s<24;s++)
  {
    int mismatch_sector=0;
    int n_burned_croat_cl_sector=0;
    int n_burned_daq_file_cl_sector=0;

    for(int pr=0;pr<45;pr++)
    {
      fcfHit h;

      // First compare...
     
      if((n_croat_cl > 0) &&
	 (n_daq_file_cl > 0)) {
	if(hasSim) {
	  printf("FCFMaker: have simulated data as well as daq file clusters. disabling comparison between calculated and file clusters\n");
	}
	else {
	  int e = fcf_after.compare(daq_file_resptr[s][pr],
				    croat_resptr[s][pr]);
	  
	  mismatch_sector += e;
	  mismatch_tot += e;
	  
	  // 	if(e != 0) {
	  //  	  printf("FCFMaker: %d mismatches between daq_file & calculated clusters (s=%d, pr=%d)\n",e,s,pr);
	  //  	}
	}
      }
      
      // Do daq file cluster after burner
      if(n_daq_file_cl > 0) {
	if(n_burned_daq_file_cl == -1) 
	  n_burned_daq_file_cl=0;

	fcf_after.burn(daq_file_resptr[s][pr], NULL);

	while(fcf_after.next(&h)) {
	  n_burned_daq_file_cl++;
	  n_burned_daq_file_cl_sector++;

	  if(use_daq_file_clusters) {
	    saveCluster(h.pad,h.tm,h.f,h.c,h.p1,h.p2,h.t1,h.t2,pr,s+1,-1,0,0);
	  }
	}
      }

      // Do croat cluster after burner if have croat clusters...
      if(n_croat_cl > 0) {
	if(n_burned_croat_cl == -1) 
	  n_burned_croat_cl = 0;

	fcf_after.burn(croat_resptr[s][pr], simu_resptr[s][pr]);
		
	while(fcf_after.next(&h)) {
	  n_burned_croat_cl++;
	  n_burned_croat_cl_sector++;

	  if(!use_daq_file_clusters) {
	    saveCluster(h.pad,h.tm,h.f,h.c,h.p1,h.p2,h.t1,h.t2,pr,s+1,h.cl_id,h.id_simtrk, h.id_quality);
	  }
	}
      }
    }
    
    // If no compare this is not satisfied...
    if(mismatch_sector != 0) {
      printf("<FCFMaker::Make> There were mismatches between file & calculated clusters (sector=%02d mismatches=%6d nfile=%6d nraw=%6d)\n",s+1,mismatch_sector,n_burned_daq_file_cl_sector,n_burned_croat_cl_sector);
    }
  }

  if(n_burned_daq_file_cl > 0) {
    printf("<FCFMaker::Make> Merged %d of %d file clusters\n",n_daq_file_cl-n_burned_daq_file_cl,n_daq_file_cl);
  }

  if(n_burned_croat_cl > 0) {
    printf("<FCFMaker::Make> Merged %d of %d calculated clusters\n",n_croat_cl-n_burned_croat_cl,n_croat_cl);
  }

  // Poor mans comparison....
  if((n_burned_croat_cl == -1) && 
     (n_burned_daq_file_cl == -1)) 
  {
    printf("<FCFMaker::Make> No data available\n");
  }
  else if((n_burned_croat_cl >= 0) &&
	  (n_burned_daq_file_cl == -1)) 
  {
    printf("<FCFMaker::Make> Only raw data available.  No daq file clusters\n");
  }
  else if((n_burned_croat_cl == -1) &&
	  (n_burned_daq_file_cl >= 0)) 
  {
    printf("<FCFMaker::Make> Only daq file clusters available.  No raw data\n");
  }
  else 
  {
    if(n_burned_daq_file_cl != n_burned_croat_cl)
    {
      printf("*-------------------------------------------------------*\n");
      printf("* FCFMaker: both raw data and clusters exist, but the   *\n");
      printf("* FCFMaker: number of clusters is different             *\n");
      printf("* FCFMaker: file-> %6d,  computed-> %6d           *\n",
	     n_burned_daq_file_cl,
	     n_burned_croat_cl);      
      printf("* FCFMaker: Check calibrations etc....                  *\n");
      printf("* FCFMaker: Using clusters from datafile                *\n");
      printf("*-------------------------------------------------------*\n");
    }
    else {
      printf("*-------------------------------------------------------*\n");
      printf("* FCFMaker: both raw data and clusters exist            *\n");
      printf("* FCFMaker: they agree on %6d clusters             *\n",n_burned_croat_cl);
      if(mismatch_tot == 0) {
	printf("* FCFMaker: and the contents are equal!!!                 \n");
      }
      else {
	printf("* FCFMaker: but the contents are not equal!!!             \n");
      }
      printf("*-------------------------------------------------------*\n");
    }
  }

  // Save the hit collection to StEvent...
  if(mFill_stevent)
  {
    mStEvent->setTpcHitCollection(mTpcHitColl);
    mTpcHitColl = NULL;    // I don't control the pointer anymore...
  }

  return kStOK;
}

// Build cpp array from tables
// Note: padrows[1-13] point to the inner sectors pixel buffer 
//       padrows[14-45] point to the outer sectors pixel buffer

Int_t StRTSClientFCFMaker::BuildCPP(int nrows, raw_row_st *row, raw_pad_st *pad, raw_seq_st *seq, int sector)
{
  int i,j,k;
  int r,p;
  int offset;

  offset = -1 ;
  for(i=0;i<nrows;i++) {
    int pad_off = row[i].ipad;
    r = row[i].RowId;

    for(j=0;j<row[i].npad;j++) {
      int seq_off = (row[i].iseq + pad[pad_off + j].SeqOffset);
      p = pad[pad_off + j].PadId;


      offset = (row[i].ipixel +
		pad[pad_off + j].PadOffset);

//       if((sector == 1) &&
// 	 (r == 1)) {
// 	printf("FCF: s=%d r=%d p=%d nseq=%d\n",
// 	       sector,r,p,pad[pad_off+j].nseq);
//       }

      int raw_s=0;
      int merged_s=0;

      for(k=0;k<pad[pad_off + j].nseq;k++) {
	int tb = seq[seq_off+k].m + ((k>=pad[pad_off + j].SeqModBreak) ? 256 : 0);
	int n = seq[seq_off+k].i;

// 	if((sector==1) &&
// 	   (r == 1)) {
// 	  printf("FCF: (s=%d r=%d p=%d) seq[%d] tb=%d len=%d (m=%d i=%d smb=%d)\n",
// 		 sector,r,p,k,tb,n+1,seq[seq_off+k].m,seq[seq_off+k].i,pad[pad_off+j].SeqModBreak);
// 	}

	raw_s = k;
	
	if( (r>45) || (p>184) || (raw_s>31) ||
	    (r<1)  || (p<1)   || (raw_s<0)) {
	  gMessMgr->Error() << "got an illegal sequence row=" << r << ", pad=" << p << ", seq=" << raw_s << endm;
	}

// 	if(n==0) {
// 	  printf("FCFMaker: Got an illegal CPP of length 0 (sector=%d row=%d pad=%d sequence=%d\n",
// 		 sector,r,p,raw_s);
// 	}

	int domerge=0;
	if(merged_s > 0) {
	  if(tb == (cpp[r-1].r[p-1][merged_s-1].start_bin + 
		    cpp[r-1].r[p-1][merged_s-1].length)) {
	    
	    // Merging...
	    domerge = 1;
	  }
	}

	if(!domerge) {
	  cpp[r-1].r[p-1][merged_s].start_bin = tb;
	  cpp[r-1].r[p-1][merged_s].offset = offset;
	  cpp[r-1].r[p-1][merged_s].length = n+1;

	  merged_s++;
	}
	else {
	  if(cpp[r-1].r[p-1][merged_s-1].length % 31 != 0) {
	    printf("FCFMaker: What is going on? merging short sequence len=%d\n",cpp[r-1].r[p-1][merged_s-1].length);
	  }

	  cpp[r-1].r[p-1][merged_s-1].length += n+1;
	}
	offset += n+1;
      } 
    }
  }

  return offset;
}

StDaqClfCppRow *StRTSClientFCFMaker::GetCPPRow(int r, int i, StDaqClfCppRow *storage)
{
  if(splitRows) {   // split row up to 3 times as per i960
    int found = 0;

    memset(storage, 0xff, sizeof(StDaqClfCppRow));
    
    if(padfinder[r][i].mezz == 0) return NULL;   
    
    for(int p=padfinder[r][i].minpad;
	p<=padfinder[r][i].maxpad;
	p++) 
    {  
      for(int s=0;s<32;s++) 
      {
	if(cpp[r].r[p-1][s].start_bin == 0xffff) continue;
	storage->r[p-1][s] = cpp[r].r[p-1][s];
	found = 1;
      }
    }
    if(found)
      return storage;
    else 
    {
      return NULL;
    }
  }
  else {
    if(i==0) {
      memcpy(storage, &cpp[r], sizeof(StDaqClfCppRow));
      return storage;
    }
  }
  return 0;
}

// Copies from StTpcCoordinateTransform,
//   except pad and tb need not be integers...
double StRTSClientFCFMaker::lxFromPad(int row, double pad)
{
  double pitch = (row<14) ?
    gStTpcDb->PadPlaneGeometry()->innerSectorPadPitch() :
    gStTpcDb->PadPlaneGeometry()->outerSectorPadPitch();
 
  double pads2move = pad - (gStTpcDb->PadPlaneGeometry()->numberOfPadsAtRow(row))/2.;
  double dist2move = -pitch*(pads2move-.5);

  //dist2move = -pitch*(pads2move) ;
  return(dist2move);
}

double StRTSClientFCFMaker::lyFromRow(int row)
{
  return (gStTpcDb->PadPlaneGeometry()->radialDistanceAtRow(row));
}

// With offsets and t0 corrections
double StRTSClientFCFMaker::lzFromTB(double timeBin, int sector, int row, int pad)
{
  double tbWidth = (1./gStTpcDb->Electronics()->samplingFrequency());
  
  double zoffset ((row > 13) ? 
		    gStTpcDb->Dimensions()->zOuterOffset() :
		    gStTpcDb->Dimensions()->zInnerOffset());

  double t0zoffset=0.0;

  //   The padbypad t0 corrections are done inside the cluster finder...
  //   using the values in fcf->t0Corr
  //
  //   if(doPadT0Corrections)  
  //   {
  //     t0zoffset =
  //       gStTpcDb->DriftVelocity()*1e-6*
  //       (gStTpcDb->T0(sector)->getT0(row,pad)*tbWidth);
  //   }

  double z = 
    gStTpcDb->DriftVelocity()*1e-6*         //cm/s->cm/us
    (gStTpcDb->triggerTimeOffset()*1e6      // units are s
     + gStTpcDb->Electronics()->tZero()     // units are us 
     + (timeBin)*tbWidth ); 

  return(z - zoffset + t0zoffset);
}

void StRTSClientFCFMaker::getCorrections(int sector, int row)
{
  int pad;

  //printf("Getting corrections %d %d\n",sector,row);
  TDataSet *tpc_calib  = GetDataBase("Calibrations/tpc"); assert(tpc_calib); 
  St_tpcGain *gainObj = (St_tpcGain*) tpc_calib->Find("tpcGain"); assert(gainObj);

  assert(gainObj->GetNRows()==24);  
  tpcGain_st *gains = gainObj->GetTable(); assert(gains);
  assert(sector>=1&&sector<=24);
  
  static StDetectorDbTpcRDOMasks* mask=0;
  static int tRDOFromRowAndPad[45][182];
  if(!mask) {
    mask = StDetectorDbTpcRDOMasks::instance();
    assert(mask);
    for(int tiFee=0;tiFee<182;tiFee++) {
      for(int tiPin=0;tiPin<32;tiPin++) {
        if(row_vs_fee[tiFee][tiPin]!=0 && pad_vs_fee[tiFee][tiPin]!=0) {
          tRDOFromRowAndPad[(row_vs_fee[tiFee][tiPin]-1)]
	    [(pad_vs_fee[tiFee][tiPin]-1)]=
	    rdo_vs_fee[tiFee][tiPin];
        }
      }
    }
  }
  
  // NOTE: pad starts from 0 (as well as row)
  for(pad=0;pad<tpc_rowlen[row+1];pad++) {
    double gain, t0;

//    double pg=0; double po=0;

//    if(mask->isOn(sector,tRDOFromRowAndPad[row][pad]))
//      { 
//	po=1;
//      }
//    pg = gains[sector-1].Gain[row][pad];

    // The "gain" logic is this:
    //	If the RDO is ON - abide by the doGainCorrections flag
    //  If the RDO is OFF - kill the gain!

    if(mask->isOn(sector,tRDOFromRowAndPad[row][pad])) {	// RDO is ON
	if(doGainCorrections) {
		gain = gains[sector-1].Gain[row][pad] ;
	}
	else {
		gain = 1.0 ;
		// Tonko: HACK! to eliminate gain calc. in FCF to cross-check TCL
		// but _still_ kill bad channels
		// if(gains[sector-1].Gain[row][pad] < 0.001) gain = 0.0 ;
	}

    }
    else {
	gain = 0.0 ;	
    }

    
    // gainCorr starts from 1!
    gainCorr[sector-1][row][pad+1] = (int)(gain*64.0 + 0.5);

    // Tonko: added T0 correction here...
    // NOTE: it seems that getT0 wants row,pad to start from 1 whereas the previous Gain
    // 		started from 0 - need to crosscheck!
    if(doT0Corrections) {
      t0 = gStTpcDb->T0(sector)->getT0(row+1,pad+1);
    }
    else t0 = 0.0 ;

    // t0Corr starts from 1!
    t0Corr[sector-1][row][pad+1] = (short)(gain*fabs(t0)*64.0 + 0.5) ;	// this is convoluted with the gain!
    if(t0 < 0.0) t0Corr[sector-1][row][pad+1] *= -1 ;

#ifdef FCF_DEBUG_OUTPUT
//     fprintf(ff, "Gains: %d %d %d %1.3f %1.3f\n",
// 	    sector, row+1, pad+1, gain, t0);
#endif

  }
}


// Save the cluster
//
// Assumes that sector is from 1...24
//              r      is from 0...44
//
void StRTSClientFCFMaker::saveCluster(int cl_x, int cl_t, int cl_f, int cl_c, int p1, int p2, int t1, int t2, int r, int sector, int cl_id, int id_simtrk, int id_quality)
{
  tss_tsspar_st *tsspar = m_tsspar->GetTable();

  double lx = lxFromPad(r+1,(((double)(cl_x))/64.0));
  double ly = lyFromRow(r+1);
  double lz = lzFromTB((((double)(cl_t))/64.0), sector, r+1, (cl_x+32)/64);
  lz -= 3.0 * tsspar->tau * mDriftVelocity * 1.0e-6;   // correct for convolution lagtime


  StTpcLocalSectorCoordinate local(lx,ly,lz,sector);
  StTpcLocalCoordinate global;   // tpt does the local --> global (DB adjustments?)
  (*mCTransform)(local,global);

  // Use the tphit table structure to accumulate info...	
  tcl_tphit_st hit;
  memset(&hit,0,sizeof(hit));


  if(cl_id != -1) {
	hit.cluster = cl_id ;
	hit.id = cl_id;
  }
  else {
	hit.cluster = clustercount ;
	hit.id = clustercount;
  }
  clustercount++ ;	// increment per event counter...

  // Filling in the flag causes very bad tracking performance
  // for some events.  I don't know why.
  if(fillDeconFlag == 0)
    hit.flag = 0;     
  else
  {
    if( (cl_f & FCF_DOUBLE_PAD) ||
	(cl_f & FCF_DOUBLE_T))
    {
      hit.flag = 1;
    }
  }

  

  hit.row = (r+1) + sector * 100;

  // Tonko: move all to double.
  // BREAKS the equality with TCL but it's TCL's problem.
  double tmp_q ;

  tmp_q = (double)cl_c  ;
	
	
  //  hit.q = (float)cl_c;
  double gain = (r<13) ? tsspar->gain_in : tsspar->gain_out;
  double wire_coupling = (r<13) ? tsspar->wire_coupling_in : tsspar->wire_coupling_out;

  tmp_q *= ((double)tsspar->ave_ion_pot * (double)tsspar->scale)/(gain*wire_coupling) ;

  //fprintf(stderr,"%f goes to %e for row %d\n",(float)cl_c,tmp_q,r) ;

  hit.q = tmp_q;
  hit.x = global.position().x();
  hit.dx = mDp;
  hit.y = global.position().y();
  hit.dy = mDperp;
  hit.z = global.position().z();
  hit.dz = mDt;


  if(doZeroTruncation)
  {
    if((hit.z < 0) && (sector <=12))  // sector 1..12 have positive z
      return;
    if((hit.z > 0) && (sector > 12))
      return;
  }

  hit.minpad = p1;
  hit.maxpad = p2;

  hit.npads = p2 - p1 + 1 ;

  hit.mintmbk = t1;
  hit.maxtmbk = t2;

  hit.ntmbk = hit.maxtmbk - hit.mintmbk + 1;

  hit.nseq = hit.npads;  // at least...

  // Factors adjusted to match tcl
  hit.prf = hit.npads * ((r>=13) ? .1316 : .0636);
  hit.zrf = hit.ntmbk * .1059;

  hit.id_simtrk = id_simtrk;
  hit.id_quality = id_quality;


#ifdef FCF_DEBUG_OUTPUT
  struct Hit_t *ht = getHitInfo(hit.row/100,hit.row%100,hit.id_simtrk) ;
  
  fprintf(ff,"%d %d %d %d %d %d %f %f %f %f %d %d %d %d ",Event_counter, hit.row/100, hit.row%100, hit.id, hit.id_simtrk,hit.id_quality,
	 hit.x,hit.y,hit.z,hit.q*1000000.0,cl_f,cl_c,p2-p1+1,t2-t1+1) ;

  if(!ht) {
	fprintf(ff,"0.0 0.0 0.0 0.0\n") ;
  }
  else {
	fprintf(ff,"%f %f %f %f\n",ht->x,ht->y,ht->z,ht->charge*1000000.0) ;
  }

#ifdef FCF_ANNOTATE_CLUSTERS
  if(cl_id != -1) {	// there was a valid CLuster ID passed to this routing!
	int i, j ;

	int sec = hit.row/100 - 1 ;
	int row = hit.row%100 -1 ;

	for(i=0;i<192;i++) {
		for(j=0;j<512;j++) {
			if(fcfPixA[sec][row][i][j].cl_id == hit.id) {
				fprintf(ff,"  s %d %d %d %d %d\n",i+1,j,fcfPixA[sec][row][i][j].adc,
					fcfPixA[sec][row][i][j].cl_id,fcfPixA[sec][row][i][j].id_simtrk) ;
			}
		}
	}
  }
#endif
				
  // Raw....
  // This line is to compare with the output from special
  // Special uses a very strange pad origin:
  // |       |[pad 1]|[pad 2]|[pad 3]|......[pad n]
  // a   b   c   d   e       (f)
  //
  // The origin of the raw cluster finder is at "b" because it is a simple average of the pad numbers which start at 1
  // In special, the output is  (raw_pad + .5) which places the origin at a
  // lxFromPad assumes the origin is b
//   fprintf(ff,"%d %d %f %f %d %d %d %d %d %d\n",
// 	  sector,r+1,((double)cl_x)/64.0 + .5,((double)cl_t)/64.0 + .5, cl_c, cl_f, p1, p2, t1, t2) ;

//   // tpc coords...
//   if(sector==24 || sector==12 || sector==6 || sector==18) {
//     fprintf(ff, 
//  	    "%d %d %d %d %d "
//  	    "%d %e %e %e %e "
//  	    "%e %f %e %f %e "
//  	    "%f %e %e %e %e "
//  	    "%e %d %d %d %d "
//  	    "%d %d %d %d %d\n",
//  	    hit.cluster,
//  	    hit.flag,
//  	    hit.id,
//  	    hit.id_globtrk,
//  	    hit.track,
//  	    hit.truncTag,
//  	    hit.alpha,
//  	    hit.dalpha,
//  	    hit.lambda,
//  	    hit.q,
//  	    hit.dq,
//  	    hit.x,
//  	    hit.dx,
//  	    hit.y,
//  	    hit.dy,
// 	    hit.z,
//  	    hit.dz,
//  	    hit.phi,
//  	    hit.prf,
//  	    hit.zrf,
//  	    hit.dedx,
// 	    hit.row/100,
//  	    hit.row%100,
//  	    hit.nseq,
//  	    hit.npads,
//  	    hit.minpad,
//  	    hit.maxpad,
//  	    hit.ntmbk,
//  	    hit.mintmbk,
//  	    hit.maxtmbk);
//   }
#endif

  if(mFill_tphit)
  {
    filltphit(&hit);
  }

  if(mFill_stevent)
  {
    fillStEvent(&hit);
  }
}

void StRTSClientFCFMaker::fillStEvent(tcl_tphit_st *hit)
{
  assert(mStEvent);
  assert(mTpcHitColl);
  
  StThreeVectorF p(hit->x,hit->y,hit->z);
  StThreeVectorF e(hit->dx,hit->dy,hit->dz);
  
  unsigned int hw = 1;         // detid_tpc
  hw += (hit->row/100 << 4);   // sector
  hw += (hit->row%100 << 9);   // row
  hw += (hit->npads   << 15);  // npads
  hw += (hit->ntmbk   << 22);  // ntmbks...

  // Tonko: this didn't compile in Jun03 !?
  //StTpcHit *tpcHit = new StTpcHit(p,e,hw,hit->q);  
  //if(!mTpcHitColl->addHit(tpcHit))
  //{
  //  assert(false);
  //}
}

void StRTSClientFCFMaker::filltphit(tcl_tphit_st *hit)
{
  assert(mT_tphit);

  int nAlloc = mT_tphit->GetTableSize();
  int nUsed = mT_tphit->GetNRows();
  if(nUsed>nAlloc-10)
  {
    mT_tphit->ReAllocate(Int_t(nAlloc*1.2+10));
  }
  mT_tphit->AddAt(hit, nUsed);
}

int StRTSClientFCFMaker::runClusterFinder(j_uintptr *result_mz_ptr, 
					  u_int *result_buff, 
					  int sector,
					  int row,
					  StDaqClfCppRow *cppRow,
					  unsigned short *adc,
					  unsigned short *trk,
					  u_int *simu_result_buff,
					  j_uintptr *simu_mz_ptr)
{
  int total_clusters=0;

 //  static StDaqClfCppRow *cppRowStorage ;
  u_int charge_on_row = 0 ;	// I added this for misc. debugging and cross checks, Tonko.

  for(int i=0;i<3;i++) result_mz_ptr[i] = NULL;
  for(int i=0;i<3;i++) simu_mz_ptr[i] = NULL;


  //printf("s=%d r=%d\n",sector,row);
  // does both Gain & T0 corrections (depending on flags)
  fcf->t0Corr = t0Corr[sector-1][row];
  fcf->gainCorr = gainCorr[sector-1][row];

  fcf->sb = sector ;	// sector starts from 1 

  //cppRowStorage = &cpp[r] ;

  u_int *res_ptr = result_buff ;
  u_int *simu_res_ptr = simu_result_buff ;
  u_int *rows_count = result_buff ;
  u_int *croat_outp ;
  u_int nclusters ;

  *rows_count = 0 ;	// row count 0 as default...
	
  res_ptr++ ;	   // advance space...
  simu_res_ptr++;  // keep parallel with res_ptr...

  int i ;
  for(i=0;i<3;i++) {
    //
    // Get the CPP pointers
    //
    //cppRow = GetCPPRow(r,i,&cppRowStorage);
    //cppRow = cpp ;
    //if(!cppRow) continue;


    fcf->row = row+1;   // row starts from 1
    fcf->padStart = 1000000;
    fcf->padStop = 0;

    memset(startFlags,0,sizeof(startFlags)) ;
    int start, stop ;

    // We send one padrow to croat at a time.
    // If the "splitRow" flag is on, these padrows are split to simulate
    // assignment of pads to different i960's.
    if(!splitRows) {
      if(i>=1) break ;	// allow only one pass...

      start = fcf->padStart = 1 ;
      stop = fcf->padStop = tpc_rowlen[row+1] ;
    }
    else {	// broken row
      if(padfinder[row][i].rdo == 0) break ;	// no more row fragments
      start = fcf->padStart = padfinder[row][i].minpad ;
      stop = fcf->padStop = padfinder[row][i].maxpad ;
    }

    if(start == 1) fcf->startFlags[start] |= FCF_ROW_EDGE ;
    else fcf->startFlags[start] |= FCF_BROKEN_EDGE ;

    if(stop == tpc_rowlen[row+1]) fcf->startFlags[stop] |= FCF_ROW_EDGE ;
    else fcf->startFlags[stop] |= FCF_BROKEN_EDGE ;

    for(int k=start;k<=stop;k++) {
      if(fcf->gainCorr[k] == 0) {
	fcf->startFlags[k] |= FCF_DEAD_EDGE ;
	if((k-1)>=start) fcf->startFlags[k-1] |= FCF_DEAD_EDGE ;
	if((k+1)<=stop) fcf->startFlags[k+1] |= FCF_DEAD_EDGE ;
      }
    }

    if(hasSim) memset(&croat_trk[0][0], 0, sizeof(croat_trk));
    memset(&croat_adc[0][0], 0, sizeof(croat_adc));
    memset(&croat_cpp[0][0], 0xff, sizeof(croat_cpp));

    // Write the ADC array for this row...
    for(int pp=fcf->padStart;pp<=fcf->padStop;pp++)
    {
      for(int ss=0;ss<FCF_MAX_SEQ;ss++)
      {

//fprintf(ff,"Row %d: pp %d, ss %d: off 0x%X, len %d\n",row+1,pp,ss,cppRow->r[pp-1][ss].offset,cppRow->r[pp-1][ss].length) ;

	if(cppRow->r[pp-1][ss].offset == 0xffffffff) break;
	    
	for(int ii=0;ii<cppRow->r[pp-1][ss].length;ii++)
	{
	  int time = ii + cppRow->r[pp-1][ss].start_bin;
	  int pnt = ii + cppRow->r[pp-1][ss].offset;


	  // The situation is rather stupid:
	  // tpcdaq maker converts adcs to 10 bits
	  // fcf requires them in 8 bits even though the 
	  // FCF_10BIT... flag is set.  Convert back here...
	  croat_adc[pp][time] = log10to8_table[adc[pnt]];

	  charge_on_row += adc[pnt] ;

//fprintf(ff,"Row %d (%d), pad %d, time %d, adc10 %d, adc8 %d, adc10 %d\n",row+1,i,pp,time,adc[pnt], croat_adc[pp][time],log8to10_table[croat_adc[pp][time]]) ;

	  if(hasSim) {
	    croat_trk[pp][time] = trk[pnt];
	    
	    //printf("FCF: s=%d pr=%d pad=%d tb=%d adc=%d trk=%d\n",sector, row, pp, time, croat_adc[pp][time], croat_trk[pp][time]);
	  }

#ifdef FCF_DEBUG_OUTPUT
//	  if(sector==1 && row==0)
//	    fprintf(ff,"%d %d %d %d %d\n",sector,row+1,pp,time,log10to8_table[adc[pnt]]) ;
#endif
	}
      }
    }

    // Write the pointers for this row...
    for(int pp=fcf->padStart;pp<=fcf->padStop;pp++)
    {
      for(int ss=0;ss<FCF_MAX_SEQ;ss++)
      {
	if(cppRow->r[pp-1][ss].start_bin == 0xffff) break;
	    
	croat_cpp[pp][2*ss] = cppRow->r[pp-1][ss].start_bin;
	croat_cpp[pp][2*ss+1] = (cppRow->r[pp-1][ss].start_bin +
				 cppRow->r[pp-1][ss].length -1);
      }
    }

    if(hasSim) {
      fcf->simIn = (short *)croat_trk;
      fcf->simOut = simu_res_ptr;
    }

    u_int words = fcf->finder((u_char *)croat_adc, 
			      (u_short *)croat_cpp, 
			      (u_int *)res_ptr);

    //
    // Add results to tphit table
    //
    croat_outp = res_ptr;  
    u_int wrow = *croat_outp++;
    nclusters = *croat_outp++;

    if(words == 1)
    {
      wrow = row+1;
      nclusters = 0;
    }

    total_clusters += nclusters;
 
    // 	printf("i=%d  ",i); for(int jjj=0;jjj<i;jjj++) printf("  ");
    // 	printf("clust: s=%d r=%d (%d/%d %d/%d %d)\n",
    // 	       sectorIdx,
    // 	       r,
    // 	       fcf->padStart,
    // 	       padfinder[r][i].minpad,
    // 	       fcf->padStop,
    // 	       padfinder[r][i].maxpad,  
    // 	       nclusters);

    if((int)wrow != row+1)
    {
      gMessMgr->Error() << "Fatal error: padrow "<<wrow<<" does not match "<< row+1 << endm;
      exit(0);
    }

    if((nclusters * 2 + 2 != words) &&
       (nclusters != 0))
    {
      gMessMgr->Error() << "Fatal error: nclusters="<<nclusters<<" words="<<words<<endm;
      exit(0);
    }

    if(nclusters) {
      result_mz_ptr[i] = res_ptr ;

      if(hasSim)
	simu_mz_ptr[i] = simu_res_ptr;

      (*rows_count)++ ;
      res_ptr += 2+2*nclusters ;	// advance pointer
      simu_res_ptr += 2+2*nclusters;
    }
  }

//   printf("nclusters(raw) = %d 0x%x 0x%x 0x%x (0x%x)\n",
// 	 nclusters,
// 	 result_mz_ptr[0],
// 	 result_mz_ptr[1],
// 	 result_mz_ptr[2],
// 	 (u_int)result_buff);


#ifdef FCF_DEBUG_OUTPUT
//	fprintf(ff,"*** Row %2d: total charge (10bit): %u\n",row+1,charge_on_row) ;
#endif

  return total_clusters;
}


int StRTSClientFCFMaker::build_daq_file_clusters()
{
  //printf("FCFMaker: build_daq_file_clusters\n");
  memset(daq_file_resptr, 0, sizeof(daq_file_resptr));
  memset(daq_file_out, 0, sizeof(daq_file_out));

  int hasClusters=0;
  int nClusters=0;

  if(!daqReader) 
  {
    printf("FCFMaker: No daq reader\n");
    return -1;
  }

  tpcReader = daqReader->getTPCReader();
  if(!tpcReader) {
    printf("FCFMaker: No tpc reader\n");
    return -1;
  }

  u_int *tpcp = (u_int *)tpcReader->ptrTPCP;

  if(memcmp("TPCP", (char *)tpcp, 4) != 0) 
  {
    printf("FCFMaker: Bad tpcp bank (%s)\n",(char *)tpcp);
    return -1;
  }

  //  printf("FCFMaker: Got tpcp\n");

//   for(int i=0;i<34;i++) {
//     printf("FCFMaker: tpcp[%d] = 0x%x\n",i,tpcp[i]);
//   }

  int ncl_sector[24];
  memset(ncl_sector, 0, sizeof(ncl_sector));
  
  for(u_int sec=0;sec<24;sec += 2) 
  {
    int swap_tpcp = checkSwap(tpcp[5]);
    int off = swap32(swap_tpcp, tpcp[10+2*sec]);
    int len = swap32(swap_tpcp, tpcp[10+2*sec+1]);

    //  printf("FCFMaker: sec=%d off=%d len=%d\n",sec,off,len);

    if(len == 0) continue;

    u_int *tpcsecp = tpcp + off;
    if(memcmp("TPCSECP", (char *)tpcsecp, 7) != 0) 
    {
      printf("FCFMaker: Bad tpcsecp bank (%s)\n",(char *)tpcsecp);
      return -1;
    }
    int swap_tpcsecp = checkSwap(tpcsecp[5]);   
    off = swap32(swap_tpcsecp, tpcsecp[8]);  // SECLP bank offset
    int format = swap32(swap_tpcsecp, tpcsecp[6]);

    //printf("FCFMaker: --- off=%d format=%d\n",off,format);

    if(off==0) continue;
    if(format < 2) continue;
  
    u_int *tpcseclp = tpcsecp + off;
    if(memcmp("TPCSECLP", (char *)tpcseclp, 8) != 0)
    {
      printf("FCFMaker: Bad tpcseclp bank (%s)\n",(char *)tpcseclp);
      return -1;
    }
    u_int swap_tpcseclp = checkSwap(tpcseclp[5]);
    if((sec+1) != swap32(swap_tpcseclp, tpcseclp[3])) 
    {
      printf("FCFMaker: Bad tpcseclp sector %d vs %d\n",
	     swap32(swap_tpcseclp, tpcseclp[3]),
	     sec);

    }

    //printf("FCFMaker: Good tpcseclp, sector %d\n",sec+1);
    
    int sadd;
    for(int rb=0;rb<12;rb++) {
      sadd = 0;    // rb 6..11 are from next sector
      if(rb >= 6) sadd = 1;

      off = swap32(swap_tpcseclp, tpcseclp[10+2*rb]);
      len = swap32(swap_tpcseclp, tpcseclp[10+2*rb+1]);
      if(len == 0) continue;

      u_int *tpcrbclp = tpcseclp + off;
      if(memcmp("TPCRBCLP", tpcrbclp, 8) != 0) {
	printf("FCFMaker: Bad TPCRBCLP bank (%s)", (char *)tpcrbclp);
	return -1;
      }
      int swap_tpcrbclp = checkSwap(tpcrbclp[5]);
      
      for(int mz=0;mz<3;mz++) {
	off = swap32(swap_tpcrbclp, tpcrbclp[10+2*mz]);
	len = swap32(swap_tpcrbclp, tpcrbclp[10+2*mz+1]);
	if(len==0) continue;

	u_int *tpcmzcld = tpcrbclp + off;
	if(memcmp("TPCMZCLD", tpcmzcld, 8) != 0)
	{
	  printf("FCFMaker: Bad TPCMZCLD bank (%s)", (char *)tpcmzcld);
	  return -1;
	}

	//printf("\tGood TPCMZCLD s=%d rb=%d mz=%d\n", sec+sadd, rb%6, mz);

	// We are finally here!!!
	// This bank contains up to 6 contributions to be placed into
	// daq_file_out[][][] and daq_file_resptr[][][]
	int swap_tpcmzcld = checkSwap(tpcmzcld[5]);

	u_int *p = daq_file_out[sec+sadd][rb%6][mz];

	// Only copy payload....
	memcpy(p, &tpcmzcld[10], len*4 - 40);

	int n_padrows = swap32(swap_tpcmzcld, p[0]); 
	
	u_int *curr = &p[1];
	for(int i=0;i<n_padrows;i++) {
	  u_int s = sec+sadd;
	  u_int r = rb % 6;

	  u_int pr = swap32(swap_tpcmzcld, *curr);
	  if(pr > 45) {
	    printf("FCFMaker: Bad padrow %d\n",pr);
	  }

	  u_int ncl = swap32(swap_tpcmzcld, *(curr+1));
	  ncl_sector[s] += ncl;
	  nClusters += ncl;

	  //printf("FCFMaker: \t\s=%d r=%d pr=%d -- %d clusters (tot=%d)\n",s,r,pr,ncl,nClusters);

	  hasClusters = 1;

	  if(pr > 45) 
	  {
	    printf("FCFMaker: Bad padrow s=%d, rb=%d, mz=%d pr=%d\n",
		   s+1,r,mz,pr);
	    return -1;
	  }

	  // Add pointer to daq_file_resptr[]
	  j_uintptr *resptr = daq_file_resptr[s][pr-1];
	  int j;
	  for(j=0;j<3;j++) {
	    if(resptr[j] == NULL) break;
	  }
	  if(j >= 3) {
	    printf("FCFMaker: All three resptr already filled! s=%d, rb=%d mz=%d pr=%d\n",
		   s+1,r,mz,pr);
	    return -1;
	  }

	  resptr[j] = curr;

	  curr += ncl*2+2;
	}
      }
    }
  }

//   for(int s=0;s<24;s++) {
//     printf("FCFMaker: sec=%d clusters=%d read from daq file\n",s+1,ncl_sector[s]);
//   }

  if(hasClusters == 0)
    return -1;
  
  return nClusters;
}

bool StRTSClientFCFMaker::checkSwap(int x)
{
  return (x==0x04030201) ? false : true;
}

u_int StRTSClientFCFMaker::swap32(bool test, u_int x)
{
  if(!test) return x;
  else
  { 
    char *hh,temp[4];
    hh=(char*)(&x);
    temp[0]=hh[3]; temp[1]=hh[2]; temp[2]=hh[1]; temp[3]=hh[0];
    return *((unsigned int*)temp);
  }
}

int StRTSClientFCFMaker::build_croat_clusters()
{
  int nclusters=0;
  int haveAnyRaw=0;
  int haveRaw=0;

  St_DataSet *rawData;
  St_DataSet *sector;
  St_DataSet *geant;

  geant = (St_DataSet *)GetInputDS("geant");
  if(geant) {
    hasSim = 1;
    printf("<FCFMaker> event has simulation information\n");
  }
  else {
    hasSim = 0;
  }

  int sz;
  
  rawData = (St_DataSet *)GetInputDS("tpc_raw");
  if(!rawData) return -1;

  St_DataSetIter rawIter(rawData);

  // Loop over all sectors  
  int sectorIdx;
  for(sectorIdx=1;sectorIdx<=24;sectorIdx++) {
    haveRaw = 0;

    St_raw_row *Trow_in, *Trow_out;
    St_raw_pad *Tpad_in, *Tpad_out;
    St_raw_seq *Tseq_in, *Tseq_out;
    St_type_shortdata *Tadc_in, *Tadc_out;
    St_type_shortdata *Ttrk_in, *Ttrk_out;

    // c arrays for this sector
    raw_row_st *row_in, *row_out;
    raw_pad_st *pad_in, *pad_out;
    raw_seq_st *seq_in, *seq_out;
    unsigned short *adc_in, *adc_out;
    unsigned short *trk_in=NULL;
    unsigned short *trk_out=NULL;

    // look for the sector in the raw data...
    rawIter.Reset();
    while((sector = rawIter()) != NULL) {
      char sectorName[100];
      sprintf(sectorName, "Sector_%d", sectorIdx);

      if(strcmp(sector->GetName(), sectorName) != 0) continue;

      // Get the table structures...
      St_DataSetIter sectorIter(sector);

      Trow_in = (St_raw_row *)sectorIter.Find("raw_row_in");
      Trow_out = (St_raw_row *)sectorIter.Find("raw_row_out");
      Tpad_in = (St_raw_pad *)sectorIter.Find("raw_pad_in");
      Tpad_out = (St_raw_pad *)sectorIter.Find("raw_pad_out");
      Tseq_in = (St_raw_seq *)sectorIter.Find("raw_seq_in");
      Tseq_out = (St_raw_seq *)sectorIter.Find("raw_seq_out");
      Tadc_in = (St_type_shortdata *)sectorIter.Find("pixel_data_in");
      Tadc_out = (St_type_shortdata *)sectorIter.Find("pixel_data_out");
      Ttrk_in = (St_type_shortdata *)sectorIter.Find("pixel_indx_in");
      Ttrk_out = (St_type_shortdata *)sectorIter.Find("pixel_indx_out");

      // Get the c arrays for this sector
      row_in = Trow_in->GetTable();
      row_out = Trow_out->GetTable();
      pad_in = Tpad_in->GetTable();
      pad_out = Tpad_out->GetTable();
      seq_in = Tseq_in->GetTable();
      seq_out = Tseq_out->GetTable();
      adc_in = (unsigned short *)Tadc_in->GetTable();
      adc_out = (unsigned short *)Tadc_out->GetTable();

      if(hasSim) {
	trk_in = (unsigned short *)Ttrk_in->GetTable();
	trk_out = (unsigned short *)Ttrk_out->GetTable();

	//printf("<FCFMaker> Simulation table at 0x%x and 0x%x\n",(u_int)trk_in, (u_int)trk_out);
      }
      else {
	trk_in = NULL;
	trk_out = NULL;
      }

      haveRaw = 1;
      haveAnyRaw = 1;
    }

    //printf("Sector %d, raw=%d\n", sectorIdx, haveRaw);

    // Setup pointers for entire sector
    if(haveRaw) {

      memset(&cpp[0],0xff,sizeof(cpp));
      sz = 0;	
      int sz2;	

      sz2 = BuildCPP(Trow_in->GetNRows(), row_in, pad_in, seq_in, sectorIdx);
      if(sz2 == -1) printf("<StRTSClientFCFMaker::build_croat_clusters> No data for sector %d, inner\n", sectorIdx);
      else sz += sz2;

      sz2 = BuildCPP(Trow_out->GetNRows(), row_out, pad_out, seq_out, sectorIdx);
      if(sz2 == -1) printf("<StRTSClientFCFMaker::build_croat_clusters> No data for sector %d, outer\n", sectorIdx);
      else sz += sz2;
    }

    // Run Clusterfinder...for this sector
    for(int r=44;r>=0;r--) {
      // skip row 13!
      if(r==12) continue ;
      
      j_uintptr *raw_resptr = croat_resptr[sectorIdx-1][r];
      j_uintptr *sim_resptr = simu_resptr[sectorIdx-1][r];

      if(haveRaw) {

	nclusters +=
	  runClusterFinder(raw_resptr, 
			   croat_out[sectorIdx-1][r], 
			   sectorIdx,
			   r,
			   &cpp[r],
			   ((r<13) ? adc_in : adc_out),
			   ((r<13) ? trk_in : trk_out),
			   croat_simu_out[sectorIdx-1][r],
			   sim_resptr);


// 	printf("(raw)------>        0x%x 0x%x 0x%x (0x%x)\n",
// 	       (u_int)raw_resptr[0],
// 	       (u_int)raw_resptr[1],
// 	       (u_int)raw_resptr[2],
// 	       (u_int)croat_out);
      }
    }
  }

  // -1 if no raw data.
  // 0 if raw data but no clusters
  if(haveAnyRaw == 0) return -1;
  return nclusters;
}


