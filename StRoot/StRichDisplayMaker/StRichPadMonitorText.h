/***************************************************************
 * $Id: StRichPadMonitorText.h,v 2.0 2000/08/09 16:28:04 gans Exp $
 *
 * Description:
 *  Text window filled by clickable actions onthe padmonitor
 *  Runs only in ROOT
 *
 ***************************************************************
 *
 * $Log: StRichPadMonitorText.h,v $
 * Revision 2.0  2000/08/09 16:28:04  gans
 * Created New Maker for all drawable objects.
 *
 * Revision 1.1  2000/02/29 18:18:59  lasiuk
 * Initial Revision
 *
 ***************************************************************/
#ifdef __ROOT__
#ifndef ST_RICH_PADMONITOR_TEXT_H
#define ST_RICH_PADMONITOR_TEXT_H

#include "TPaveLabel.h"
#include "StRichDrawableTPad.h"

class StRichPadMonitorText {
public:
    StRichPadMonitorText();

    ~StRichPadMonitorText();
    
    //StRichPadMonitorText(const StRichPadMonitorText&);
    //StRichPadMonitorText& operator=(const StRichPadMonitorText&);

    void drawText(StRichDrawableTPad* tpad);
    void removeText();

private:
    TPaveLabel* myLabel;
};

#endif /* _H */
#endif /* ROOT */
