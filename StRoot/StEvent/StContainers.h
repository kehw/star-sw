/***************************************************************************
 *
 * $Id: StContainers.h,v 2.18 2003/10/25 00:15:02 fisyak Exp $
 *
 * Author: Thomas Ullrich, Oct 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StContainers.h,v $
 * Revision 2.18  2003/10/25 00:15:02  fisyak
 * remove StPhmdHit and StPhmdCluster added by mistake
 *
 * Revision 2.16  2003/05/21 18:22:46  ullrich
 * Major Revision of ToF classes (F. Geurts)
 *
 * Revision 2.15  2002/12/20 22:34:42  ullrich
 * Added containers for PMD hits and clusters.
 *
 * Revision 2.14  2001/12/01 15:39:19  ullrich
 * Added macros for StDetectorState.
 *
 * Revision 2.13  2001/11/10 23:53:22  ullrich
 * Added calibration vertices.
 *
 * Revision 2.12  2001/10/01 19:40:31  ullrich
 * Added StTofData.
 *
 * Revision 2.11  2001/08/02 01:27:09  ullrich
 * Added containers for StL3AlgorithmInfo.
 *
 * Revision 2.10  2001/04/16 20:29:46  ullrich
 * Added Tof containers.
 *
 * Revision 2.9  2001/03/14 02:29:17  ullrich
 * Added StPsd container.
 *
 * Revision 2.8  2000/11/25 11:52:23  lasiuk
 * addition of Rich Photon Info
 *
 * Revision 2.7  2000/09/28 10:56:35  ullrich
 * Added Rich PID classes.
 *
 * Revision 2.6  2000/06/19 01:32:15  perev
 *  Thomas StEvent branches added
 *
 * Revision 2.5  2000/05/22 21:42:44  ullrich
 * Add RICH related classes.
 *
 * Revision 2.4  2000/03/23 22:24:06  akio
 * Initial version of Emc Point, and Inclusion of track pointers
 *
 * Revision 2.3  2000/02/23 17:35:57  ullrich
 * Changes due to the addition of the EMC to StEvent
 *
 * Revision 2.2  2000/01/05 16:02:23  ullrich
 * SSD hits added to StEvent.
 *
 * Revision 2.1  1999/10/28 22:06:19  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StContainers_hh
#define StContainers_hh
#include "StArray.h"

class StObject;
class StCalibrationVertex;
class StDetectorState;
class StHit;
class StPhmdHit;
class StPhmdCluster;
class StPrimaryTrack;
class StPrimaryVertex;
class StPsd;
class StSvtHit;
class StSsdHit;
class StTpcHit;
class StFtpcHit;
class StRichHit;
class StRichPixel;
class StRichPid;
class StRichPhotonInfo;
class StRichCluster;
class StRichMCInfo;
class StEmcRawHit;
class StEmcCluster;
class StEmcPoint;
class StTofHit;
class StTofSlat;
class StTofCell;
class StTofData;
class StTrack;
class StTrackNode;
class StTrackPidTraits;
class StTrackDetectorInfo;
class StV0Vertex;
class StXiVertex;
class StKinkVertex;
class StL3AlgorithmInfo;

StCollectionDef(Object)
StCollectionDef(CalibrationVertex)
StCollectionDef(DetectorState)
StCollectionDef(Hit)
StCollectionDef(PrimaryTrack)
StCollectionDef(PrimaryVertex)
StCollectionDef(Psd)
StCollectionDef(SvtHit)
StCollectionDef(SsdHit)
StCollectionDef(TpcHit)
StCollectionDef(FtpcHit)
StCollectionDef(RichHit)
StCollectionDef(RichPixel)
StCollectionDef(RichPid)
StCollectionDef(RichPhotonInfo)
StCollectionDef(RichCluster)
StCollectionDef(RichMCInfo)
StCollectionDef(EmcRawHit)
StCollectionDef(EmcCluster)
StCollectionDef(EmcPoint)
StCollectionDef(TofHit)
StCollectionDef(TofSlat)
StCollectionDef(TofCell)
StCollectionDef(TofData)
StCollectionDef(Track)
StCollectionDef(TrackNode)
StCollectionDef(TrackPidTraits)
StCollectionDef(TrackDetectorInfo)
StCollectionDef(V0Vertex)
StCollectionDef(XiVertex)
StCollectionDef(KinkVertex)
StCollectionDef(L3AlgorithmInfo)
StCollectionDef(PhmdHit)
StCollectionDef(PhmdCluster)

#endif
