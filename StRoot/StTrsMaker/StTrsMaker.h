// $Id: StTrsMaker.h,v 1.1 1999/01/22 21:31:46 lasiuk Exp $
//
// $Log: StTrsMaker.h,v $
// Revision 1.1  1999/01/22 21:31:46  lasiuk
// Name change
//
// Revision 1.2  1999/01/22 11:43:09  lasiuk
// add output structures
//
//
#ifndef STAR_St_Trs_Maker
#define STAR_St_Trs_Maker

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// StTrsMaker virtual base class for Maker                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#ifndef StMaker_H
#include "StMaker.h"
#endif

// Dbs
class StTpcGeometry;
class StTpcSlowControl;
class StMagneticField;
class StTpcElectronics;
class StTrsDeDx;

// Processes 
class StTrsChargeTransporter;
class StTrsAnalogSignalGenerator;
class StTrsDigitalSignalGenerator;

// Containers
class StTrsWireHistogram;
class StTrsSector;
class StTrsDigitalSector;

// Output Data
//class StTpcRawDataEvent;

class StTrsMaker : public StMaker {
 private:
   Bool_t drawinit;
// static Char_t  m_VersionCVS = "$Id: StTrsMaker.h,v 1.1 1999/01/22 21:31:46 lasiuk Exp $";
// Int_t          m_mode;        // mode 1 = primaries;
// St_stk_stkpar *m_stk_stkpar;  //! pointer to stk parameters

#ifndef ST_NO_TEMPLATE_DEF_ARGS
   //vector<StTrsChargeSegment>  *mData; //
#else
   //vector<StTrsChargeSegment,allocator<StTrsChargeSegment> >  *mData; //
#endif

    // DataBases
    StTpcGeometry               *mGeometryDb; //!
    StTpcSlowControl            *mSlowControlDb; //!
    StMagneticField             *mMagneticFieldDb;//!
    StTpcElectronics            *mElectronicsDb; //!
    StTrsDeDx                   *mGasDb;//!
    
    // Processes 
    StTrsChargeTransporter      *mChargeTransporter;//!
    StTrsAnalogSignalGenerator  *mAnalogSignalGenerator;//!
    StTrsDigitalSignalGenerator *mDigitalSignalGenerator;//!
    
    // Container
    StTrsWireHistogram           *mWireHistogram;//!
    StTrsSector                  *mSector;//!
    StTrsDigitalSector           *mDigitalSector;//!

    //StTpcRawDataEvent          *mAllTheData;

protected:

public: 
    StTrsMaker(const char *name="tpc_raw", const char *title="event/raw_data/tpc");
   virtual       ~StTrsMaker();
   virtual Int_t Init();
   virtual Int_t  Make();
   virtual void   PrintInfo();
// virtual void Set_mode       (Int_t   m =      2){m_mode       = m;} // *MENU*
   ClassDef(StTrsMaker, 1)   //StAF chain virtual base class for Makers
};

#endif
