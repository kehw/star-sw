#ifndef STAR_StBFChain
#define STAR_StBFChain

//////////////////////////////////////////////////////////////////////////
/*!
                                                                      
 \class  StBFChain                                                            
 \author Yuri Fisyak
 \date   1999/07/29
                                                                      
 Class to control "BFC" chain                                         
                                                                      
 This class :                                                         
   - Initialises the run default parameters                           
   - Provides API to Set/Get run parameters                           
   - Creates the support lists (TClonesArrays) for the Event structure
   - Creates the physics objects makers                               

*/
//////////////////////////////////////////////////////////////////////////
#include "StChain.h"
#include "TFile.h"
#include "tables/St_Bfc_Table.h"
//_____________________________________________________________________

class St_XDFFile;
class StFileI;
class TObjArray;

class StBFChain : public StChain {
 private:
  St_XDFFile          *fXdfOut;   //! xdf output file if any
  TFile               *fTFile;    // TFile associated with the chain
  Bfc_st              *fBFC;      // Private chain
  StFileI             *fSetFiles; //
  TString             *fInFile;   //
  TString             *fFileOut;  //
  TString             *fXdfFile;  //
  Int_t               FDate;      // floating timestamp date
  Int_t               FTime;      // floating timestamp time (unused)
  
 public:
                       StBFChain(const char *name="bfc");
   virtual            ~StBFChain();
   virtual Int_t       Make(int number){ SetIventNumber(number); return StChain::Make(number);};
   virtual Int_t       Make(){return StChain::Make();};
   virtual Int_t       Load();      // *MENU*
   virtual Int_t       Instantiate();      // *MENU*
   virtual Int_t       AddAB (const Char_t *after="",const StMaker *maker=0,const Int_t Opt=1); 
   virtual Int_t       AddAfter  (const Char_t *after, const StMaker *maker) {return AddAB (after,maker);} 
   virtual Int_t       AddBefore (const Char_t *before,const StMaker *maker) {return AddAB (before,maker,-1);} 
   virtual Int_t       ParseString (const TString &tChain, TObjArray &Opt);
   void                SetFlags(const Char_t *Chain="gstar tfs"); // *MENU*
   void                Set_IO_Files(const Char_t *infile=0, const Char_t *outfile=0); // *MENU
   void                SetInputFile(const Char_t *infile=0);                          // *MENU
   void                SetGC(const Char_t *queue=
			     "-s;dst runc;"                     // list of components needed
			     "-q;numberOfPrimaryTracks>1500;"   // example of user query
			     );                                                       // *MENU

   void                SetOutputFile(const Char_t *outfile=0);                        // *MENU
   virtual Int_t       kOpt(const TString *Tag) const; 
   virtual Int_t       kOpt(const Char_t  *Tag) const; 
   virtual void        SetXdfOut(St_XDFFile *xdf=0) {fXdfOut = xdf;}
   virtual void        SetDbOptions();
   virtual void        SetGeantOptions();
   virtual void        SetTreeOptions();
   virtual void        SetOption(const Int_t k);
   virtual void        SetOption(const Char_t*  Opt) {SetOption(kOpt(Opt));}
   virtual void        SetOption(const TString* Opt) {SetOption(kOpt(Opt));}
   virtual void        SetOptionOff(const Char_t*  Opt) {SetOption(-kOpt(Opt));}
   virtual void        SetOptionOff(const TString* Opt) {SetOption(-kOpt(Opt));}
   virtual void        SetTFile(TFile *m) {fTFile = m;}
   virtual Int_t       Finish();
   virtual TFile      *GetTFile() {return fTFile;}
   virtual St_XDFFile *GetXdfOut() {return fXdfOut;}
   virtual Option_t*   GetOption() const{return TObject::GetOption();}
   virtual Bool_t      GetOption(const Int_t k)  const;
   virtual Bool_t      GetOption(const TString *Opt) const {return GetOption(kOpt(Opt));}
   virtual Bool_t      GetOption(const Char_t  *Opt) const {return GetOption(kOpt(Opt));}
   virtual const char *GetCVS() const {
       static const char cvs[]="Tag $Name:  $ $Id: StBFChain.h,v 1.21 2002/02/22 00:34:06 jeromel Exp $ built "__DATE__" "__TIME__ ; 
       return cvs;
   }
   /// StBFChain control class
   ClassDef(StBFChain, 2) 
};
#endif
