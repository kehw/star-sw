/***************************************************************
 * $Id: StRichPadMonitor.h,v 1.4 2000/04/05 16:02:13 lasiuk Exp $
 * Description:
 *  First aTtempt at a simple Pad Monitor.
 *  Runs only in ROOT
 *
 ***************************************************************
 *
 * $Log: StRichPadMonitor.h,v $
 * Revision 1.4  2000/04/05 16:02:13  lasiuk
 * GEANT info now drawable
 *
 * Revision 1.3  2000/03/13 21:50:35  lasiuk
 * coordinates
 *
 * Revision 1.2  2000/02/29 18:19:38  lasiuk
 * Split needed classes into individual files
 *
 * Revision 1.1  2000/02/12 21:57:57  lasiuk
 * Initial revision
 ***************************************************************/
#ifdef __ROOT__
#ifndef ST_RICH_PADMONITOR_H
#define ST_RICH_PADMONITOR_H

#include <vector>
#ifndef ST_NO_NAMESPACES
using std::vector;
#endif

#include "TObject.h"
#include "TCanvas.h"
#include "TObjArray.h"
//#include "St_Node.h"
#include "TText.h"
#include "TPaveLabel.h"

#include "TColor.h"

#include "StRichG2TInfo.h"

#include "StRchMaker/StRichHit.h"

class StRichPadMonitorText;
class StRichGeometryDb;
class StRichCoordinateTransform;
class StRichSinglePixel;
class StRichSingleMCPixel;
class StRichDrawableTPad;

class StRichPadMonitor : public TObject {
public:
    static StRichPadMonitor* getInstance(StRichGeometryDb*);
    
    ~StRichPadMonitor();

    //StRichPadMonitor(const StRichPadMonitor&) {/*nopt*/}
    //StRichPadMonitor& operator=(const StRichPadMonitor&) {/*nopt*/}

    void clearAll();
    void clearPads();
    void clearG2T();
    void clearHits();

    void drawPads();
    void addPad(StRichSinglePixel*);

    void drawPad(const StRichSingleMCPixel&);
    void drawPad(const StRichSinglePixel&);
    void drawG2T(const StRichG2TInfo&);
    void drawGeantGroup(int track, int color=1);
    void drawHit(StRichHit*);

    // Controls
    //void removeGeantPoints();
    //void drawGeantPoints();
    
    void update();
    
    void calculatePadPosition(const StRichSinglePixel* pad,
			      double* xl, double* yl, double* xu, double* yu);

    // Ring stuff
    void addInnerRingPoint(double x, double y);
    void addOuterRingPoint(double x, double y);
    void drawRing();
    
protected:
    StRichPadMonitor(StRichGeometryDb*);

    void    drawColorBox();
    Color_t GetColorAttribute(double amp);
    
private:
    TCanvas*    mRichCanvas;
    TObjArray   mAllFilledPads;
    TObjArray   mColorBoxes;
    TObjArray   mTextLabels;
    TObjArray   mControls;
    TObjArray   mG2TSegments;
    TObjArray   mHits;
    
    TObjArray   mRingPoints;
    TObjArray   mORingPoints;
    vector<double>     mXPoints;
    vector<double>     mYPoints;
    vector<double>     mXOPoints;
    vector<double>     mYOPoints;
    //St_Node *mHall;

    StRichPadMonitorText*      mTextWindow;
    
    StRichCoordinateTransform* mTransform;
    StRichGeometryDb*          mGeometryDb;
    
    double mRowPitch;
    double mPadPitch;
    double mPadWidth;
    double mPadLength;

    short mColorMode;
private:
    static StRichPadMonitor* mInstance;
};

#endif  // ST_RICH_PADMONITOR_H
#endif  // __ROOT__
