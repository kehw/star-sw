/*!
 * \class StEstGlobalTrack 
 * \author Thomas Ullrich, Mar 2002
 */
/***************************************************************************
 *
 * $Id: StEstGlobalTrack.h,v 2.1 2002/04/19 13:29:28 jeromel Exp $
 *
 * Author: Thomas Ullrich, Mar 2002
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StEstGlobalTrack.h,v $
 * Revision 2.1  2002/04/19 13:29:28  jeromel
 * New include for SVT 2 tables scheme support. Forgot about it yesterday ...
 *
 **************************************************************************/
#ifndef StEstGlobalTrack_hh
#define StEstGlobalTrack_hh

#include "StGlobalTrack.h"

class StEstGlobalTrack : public StGlobalTrack {
public:
    StEstGlobalTrack();
    StEstGlobalTrack(const dst_track_st&);
    StEstGlobalTrack(const StEstGlobalTrack&);
    StEstGlobalTrack& operator=(const StEstGlobalTrack&);
    ~StEstGlobalTrack();

    StTrackType     type() const;

protected:
    StObject* clone() const;
    ClassDef(StEstGlobalTrack,1)
};
#endif
