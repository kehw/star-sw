/*!
 * \class StEmcRawHit 
 * \author Akio Ogawa, Jan 2000
 */
/***************************************************************************
 *
 * $Id: StEmcRawHit.h,v 2.6 2002/02/22 22:56:47 jeromel Exp $
 *
 * Author: Akio Ogawa, Jan 2000
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StEmcRawHit.h,v $
 * Revision 2.6  2002/02/22 22:56:47  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.5  2001/04/05 04:00:35  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.4  2001/03/24 03:34:46  perev
 * clone() -> clone() const
 *
 * Revision 2.3  2000/07/28 19:49:28  akio
 * Change in Detector Id for Endcap SMD
 *
 * Revision 2.2  2000/05/22 19:21:54  akio
 * Bug fix, add delta into EMcPoint, wider bits for Eta in RawHit
 *
 * Revision 2.1  2000/02/23 17:34:14  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StEmcRawHit_hh
#define StEmcRawHit_hh

#include "StObject.h"
#include "StEnumerations.h"

class StEmcRawHit : public StObject {
public:
    StEmcRawHit();
    StEmcRawHit(StDetectorId, unsigned int, unsigned int, unsigned int, unsigned int);
    StEmcRawHit(StDetectorId, unsigned int, unsigned int, unsigned int, unsigned int, float);
    StEmcRawHit(const StEmcRawHit&);
    // StEmcRawHit& operator=(const StEmcRawHit&); use default
    ~StEmcRawHit();
    
    StDetectorId    detector() const;
    unsigned int    module() const;
    unsigned int    eta() const;
    int             sub() const;
    unsigned int    calibrationType() const;
    unsigned int    adc() const;
    float           energy() const;
    
    void setId(StDetectorId, unsigned int, unsigned int, unsigned int);
    void setCalibrationType(const unsigned int);
    void setAdc(const unsigned int);
    void setEnergy(const float);
    
protected:
    unsigned int    bits(unsigned int, unsigned int) const;
    
protected:
    UInt_t    mId;
    UInt_t    mAdc;
    Float_t   mEnergy;
    
    StObject* clone() const;
    ClassDef(StEmcRawHit,1)
};
#endif


