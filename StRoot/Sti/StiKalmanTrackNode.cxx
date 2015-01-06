//StiKalmanTrack.cxx
/*
 * $Id: StiKalmanTrackNode.cxx,v 2.148.2.7 2015/01/06 00:38:43 perev Exp $
 *
 * /author Claude Pruneau
 *
 * $Log: StiKalmanTrackNode.cxx,v $
 * Revision 2.148.2.7  2015/01/06 00:38:43  perev
 * Cylindrical shape splitted into two: Full cylinder & sector of cylinder.
 * Full cylinder could be rotated many times, sector not.
 * Very detailed setting of _state removed, kept only valid or not
 * mMtx is used only fo error propagation, not error rotation
 * For error rotation used StiNodeErrs::rotate
 * StiKalmanTrackNode::isDca() check for dertector == 0 added
 * All inline rotations moved into methods rotate(...)
 *
 * Revision 2.148.2.6  2014/12/16 01:07:01  perev
 * -  assert(fabs(mFP._sinCA)<1e-3);
 * +  assert(fabs(mFP._sinCA)<1e-2);
 * According pure math, _sinCA here must be zero. Accounting rounding
 * errors it must be around 1e-10. But even 1e-3 is too small.
 * It is related to changes in mag field for curvature about 10cm
 * So I put 1e-2 (Victor)
 *
 * Revision 2.148.2.5  2014/12/13 03:13:52  perev
 * BugFix. sind ==> sind/cosd
 *
 * Revision 2.148.2.4  2014/12/12 17:58:19  perev
 * Decrease strongeness of assert. 1e-4 ==> 1e-3
 *
 * Revision 2.148.2.3  2014/12/08 04:39:27  perev
 * in propagate(xk
 * best solution saved into mygP and myFP, and only at the end the best is
 * into mgP and mFPcopied
 *
 * Revision 2.148.2.2  2014/11/13 19:35:23  perev
 * Remove over complicated and not used logics of "edges"
 * -  kEdgePhiPlus, kEdgeZminus, kEdgePhiMinus, kEdgeZplus,
 * -  kMissPhiPlus, kMissZminus, kMissPhiMinus, kMissZplus,
 * and use intead only:
 * +  kFailed = -3,         // could not find intersection
 * +  kTooFar = -2,
 * +  kEnded  = -1,
 * +  kHit    =  0,
 *
 * Revision 2.148.2.1  2014/10/29 19:27:05  perev
 *
 * Method propagate(double xk, ...) rewritten to account cases which was ommitted
 * in prevuius version as non important. It happened they are important.
 *
 * Part of non used code (under #if ) was removed
 *
 * Revision 2.148  2014/10/18 03:00:03  perev
 * Linear approximation in nudge if shift < 0.01/curvature
 * xyz was not updated in cyl case + THelix
 *
 * Revision 2.147  2014/10/17 19:47:27  perev
 * Method nudge rewritten. Logic was too complicated, as a result wrong assert happened.
 * Now numerous cases checked carefully.
 *
 * Revision 2.146  2014/10/16 22:28:45  perev
 * Method nudge() rewritten. For complicated cases THelixTrack used.
 * Check for kFarFromBeam putted back (Xin found). It was removed by mistake
 *
 * Revision 2.145  2014/10/14 02:29:50  perev
 * Method inside() added
 * Method locate() rewritten, accounting of any errors and edges removed
 *
 * Revision 2.144  2014/10/10 21:34:36  perev
 * check for cos>1 adde
 *
 * Revision 2.143  2014/10/08 00:57:51  perev
 * Assert added for tubes Rxy == Rnormal (bug #2915)
 * Check for Hz, Hz==>fabs(Hz) to account negative Hz
 *
 * In Propagate to volume added distinguishig for planar and cyl shapes.
 * Planar approach sometimes does not work for cyl shape. (bug #2915)
 * Old, over complicated, and often wrong logic removed
 *
 * In nudge(...) now if hit is provided, nudge to hit. If not, nudge to detector plane
 *
 * Revision 2.142  2014/09/30 15:44:51  perev
 * Added StELoss class to keep ELoss info
 *
 * Revision 2.141  2014/09/18 18:45:00  perev
 * Debug++
 * Using new cylCross method
 * More checks for out of region
 *
 * Revision 2.140  2014/09/05 21:55:29  perev
 * bug #2903  fixed. x0,x0p,x0Gas initialised now tp 1e11 instead of -1
 * Many asserts adde temporary. Some of them time consuming
 *
 * Revision 2.139  2014/08/27 01:33:59  perev
 * ::print bug fixed (printed local coordinates instead of global ones)
 *         added print of rxy and direction of track, outside +ve, inside -ve
 *
 * Revision 2.138  2014/08/22 16:25:20  perev
 * Fix old bug double counting of density
 * Fix ELoss bug. dEdX(density) ==> dEdX(density,material)
 * Save calculated ELoss in StiNode for technical analisys
 *
 * Revision 2.137  2014/06/03 16:48:38  genevb
 * Reduce visibility of inactive materials
 *
 * Revision 2.136  2013/04/10 22:09:01  fisyak
 * Roll back to version 04/04/2013
 *
 * Revision 2.134  2013/01/14 22:19:48  fisyak
 * Set Bz = 0 for laser tracks
 *
 * Revision 2.133  2011/11/21 17:05:26  fisyak
 * Correct no. of possible point for CA case
 *
 * Revision 2.132  2010/09/07 18:37:31  fisyak
 * Restore Sti logic before TPCCATracker
 *
 * Revision 2.131  2010/09/06 18:20:48  fisyak
 * Add TPCCATracker
 *
 * Revision 1.4  2010/08/23 21:56:28  ikulakov
 * Fix - alinghment bag.
 *
 * Revision 1.3  2010/08/04 19:46:43  ikulakov
 * Fix - edge (don't cut 2 cm from detector)
 *
 * Revision 1.2  2010/07/29 16:19:11  fisyak
 * GSI CA tracker
 *
 * Revision 2.130  2010/07/14 18:45:10  perev
 * MoreComments
 *
 * Revision 2.129  2010/04/03 04:02:29  perev
 * Account field=0
 *
 * Revision 2.128  2010/04/01 20:25:34  perev
 * Remove test to small error _cPP
 *
 * Revision 2.127  2010/04/01 00:27:33  perev
 * Zero fied = 2e-6
 *
 * Revision 2.126  2009/11/05 17:37:52  fine
 * remove the compilation warnings
 *
 * Revision 2.125  2009/10/18 22:48:58  perev
 * remove STAR LOG in print()
 *
 * Revision 2.124  2009/08/19 21:19:37  perev
 * getTime() is a const now
 *
 * Revision 2.123  2009/04/23 02:39:03  perev
 * GetTime defence sin <1 added
 *
 * Revision 2.122  2009/04/01 19:20:17  perev
 * Replace asserts to error condition
 *
 * Revision 2.121  2009/03/16 13:50:15  fisyak
 * Move out all Sti Chairs into StDetectorDb
 *
 * Revision 2.120  2008/12/26 15:18:00  fisyak
 * Enlarge fitting volume from 200 => 250 cm
 *
 * Revision 2.119  2008/06/11 22:04:37  fisyak
 * Add dead material
 *
 * Revision 2.118  2008/06/09 20:12:09  perev
 * BigStepBack
 *
 * Revision 2.115  2008/04/03 20:03:36  fisyak
 * Straighten out DB access via chairs
 *
 * Revision 2.114  2008/03/25 18:02:53  perev
 * remove field field from everythere
 *
 * Revision 2.113  2007/09/10 21:26:52  perev
 * getPt non positive bug fix. introduces 3 month ago
 *
 * Revision 2.112  2007/08/30 19:13:27  fine
 * replace the repmaining cout with LOG_DEBUG
 *
 * Revision 2.111  2007/08/16 20:21:24  fine
 * replace printf with logger
 *
 * Revision 2.110  2007/07/12 00:21:00  perev
 * Normal radius instead of layer one
 *
 * Revision 2.109  2007/06/25 19:31:52  perev
 * Init of _sinCA and _cosCA non zeros now
 *
 * Revision 2.108  2007/06/07 20:13:42  perev
 * BugFix in getPt()
 *
 * Revision 2.107  2007/06/06 04:03:03  perev
 * getTime() cleanup
 *
 * Revision 2.106  2007/04/30 19:53:16  fisyak
 * Correct time of flight calculation, add time of flight corrrection for Laser
 *
 * Revision 2.105  2007/03/21 17:47:24  fisyak
 * Time of Flight
 *
 * Revision 2.104  2006/12/24 02:16:36  perev
 * _inf=0 added in copy constructor
 *
 * Revision 2.103  2006/12/18 01:17:41  perev
 * Info block added and filled for pulls
 *
 * Revision 2.102  2006/10/16 20:29:35  fisyak
 * Clean up useless classes
 *
 * Revision 2.101  2006/10/09 15:47:07  fisyak
 * take out Central represantation, remove StiDedxCalculator
 *
 * Revision 2.100  2006/05/31 03:58:06  fisyak
 * Add Victor's dca track parameters, clean up
 *
 * Revision 2.99  2006/04/15 23:12:10  perev
 * Supress printout
 *
 * Revision 2.98  2006/04/07 18:01:56  perev
 * Back to the latest Sti
 *
 * Revision 2.96  2006/02/16 20:44:50  perev
 * assert changed back
 *
 * Revision 2.95  2006/02/15 19:07:18  perev
 * assrt in nudge cos < 1
 *
 * Revision 2.94  2006/02/14 18:10:41  perev
 * getGlobalHitErrors added+CleanUp
 *
 * Revision 2.93  2005/12/31 01:37:12  perev
 * Primary node perpendicular to track
 *
 * Revision 2.92  2005/12/20 00:41:21  perev
 * unassigned sind fixed(thanxYF)
 *
 * Revision 2.91  2005/12/18 23:41:46  perev
 * Dependency from StiKalmanTrackNode removed
 *
 * Revision 2.90  2005/12/08 22:05:45  perev
 * nudge assert replaced by print. But very strangeStiKalmanTrackNode.cxx
 *
 * Revision 2.89  2005/12/07 22:29:27  perev
 * Big Cleanup
 *
 * Revision 2.88  2005/08/09 14:55:34  perev
 * extend()/reduce() of node
 *
 * Revision 2.87  2005/08/04 03:52:54  perev
 * Cleanup
 *
 * Revision 2.86  2005/07/20 17:24:25  perev
 * Nudge actions in evaluateChi2 added
 *
 * Revision 2.85  2005/06/14 22:22:46  perev
 * Replace assert to error return
 *
 * Revision 2.84  2005/06/03 19:57:04  perev
 * Bug fix, violation of array size
 *
 * Revision 2.83  2005/06/02 17:27:41  perev
 * More weak assert in nudge()
 *
 * Revision 2.82  2005/05/31 16:47:56  perev
 * technical reorganization
 *
 * Revision 2.81  2005/05/12 18:10:04  perev
 * dL/dCurv more accurate
 *
 * Revision 2.80  2005/05/04 19:33:00  perev
 * Supress assert
 *
 * Revision 2.79  2005/04/30 20:45:18  perev
 * Less strong test for assert in propagateError
 *
 * Revision 2.78  2005/04/25 20:20:25  fisyak
 * replace assert by print out
 *
 * Revision 2.77  2005/04/12 14:35:39  fisyak
 * Add print out for dE/dx
 *
 * Revision 2.76  2005/04/11 22:48:30  perev
 * assert removed
 *
 * Revision 2.75  2005/04/11 17:33:55  perev
 * Wrong sorting accounted, check for accuracy inctreased
 *
 * Revision 2.74  2005/04/11 14:32:18  fisyak
 * Use gdrelx from GEANT for dE/dx calculation with accouning density effect
 *
 * Revision 2.73  2005/03/30 21:01:43  perev
 * asserts replaced to prints
 *
 * Revision 2.72  2005/03/28 05:52:40  perev
 * Reorganization of node container
 *
 * Revision 2.71  2005/03/24 19:28:35  perev
 * Switch off DerivTest
 *
 * Revision 2.70  2005/03/24 18:05:07  perev
 * Derivatives and their test fixed to eta==Psi model
 *
 * Revision 2.69  2005/03/19 00:20:33  perev
 * Assert for zero determinant ==> print
 *
 * Revision 2.68  2005/03/18 17:35:38  perev
 * some asserts removed
 *
 * Revision 2.67  2005/03/18 17:13:07  perev
 * assert in rotate fix
 *
 * Revision 2.66  2005/03/17 06:24:52  perev
 * A lot of changes. _eta now is Psi
 *
 * Revision 2.65  2005/02/25 17:05:41  perev
 * Scaling for errors added
 *
 * Revision 2.64  2005/02/19 20:23:37  perev
 * Cleanup
 *
 * Revision 2.63  2005/02/18 19:02:55  fisyak
 * Add debug print out for extendToVertex
 *
 * Revision 2.62  2005/02/17 23:19:02  perev
 * NormalRefangle + Error reseting
 *
 * Revision 2.61  2005/02/17 19:58:06  fisyak
 * Add debug print out flags
 *
 * Revision 2.60  2005/02/16 17:47:16  perev
 * assert in nudge 1==>5
 *
 * Revision 2.59  2005/02/07 18:33:42  fisyak
 * Add VMC dead material
 *
 * Revision 2.58  2005/01/20 16:51:32  perev
 * Remove redundant print
 *
 * Revision 2.57  2005/01/17 01:31:25  perev
 * New parameter model
 *
 * Revision 2.56  2005/01/06 00:59:41  perev
 * Initial errors tuned
 *
 * Revision 2.55  2005/01/04 01:37:47  perev
 * minor bug fix
 *
 * Revision 2.54  2004/12/23 18:15:46  perev
 * Cut for -ve cosCA added
 *
 * Revision 2.53  2004/12/14 17:10:17  perev
 * Propagate for 0 not called
 *
 * Revision 2.52  2004/12/13 22:52:23  perev
 * Off testError
 *
 * Revision 2.51  2004/12/13 20:01:38  perev
 * old version of testError temporary activated
 *
 * Revision 2.50  2004/12/12 01:34:24  perev
 * More smart testError, partial error reset
 *
 * Revision 2.49  2004/12/11 22:17:49  pruneau
 * new eloss calculation
 *
 * Revision 2.48  2004/12/11 04:31:36  perev
 * set of bus fixed
 *
 * Revision 2.47  2004/12/10 15:51:44  fisyak
 * Remove fudge factor from eloss calculation, add more debug printout and tests, reorder calculation of cov. matrix for low triangular form
 *
 * Revision 2.46  2004/12/08 16:56:16  fisyak
 * Fix sign in dE/dx; move from upper to lower triangular matrix convention (StEvent) for px,py,pz
 *
 * Revision 2.45  2004/12/05 00:39:07  fisyak
 * Add test suit for matrix manipulation debugging under overall CPPFLAGS=-DSti_DEBUG
 *
 * Revision 2.44  2004/12/01 14:04:57  pruneau
 * z propagation fix
 *
 * Revision 2.43  2004/11/24 17:59:26  fisyak
 * Set ionization potential for Ar in eloss calculateion instead 5
 *
 * Revision 2.42  2004/11/22 19:43:06  pruneau
 * commented out offending cout statement
 *
 * Revision 2.41  2004/11/22 19:23:20  pruneau
 * minor changes
 *
 * Revision 2.40  2004/11/10 21:46:02  pruneau
 * added extrapolation function; minor change to updateNode function
 *
 * Revision 2.39  2004/11/08 15:32:54  pruneau
 * 3 sets of modifications
 * (1) Changed the StiPlacement class to hold keys to both the radial and angle placement. Propagated the use
 * of those keys in StiSvt StiTpc StiSsd and all relevant Sti classes.
 * (2) Changed the StiKalmanTrackFinder::find(StiTrack*) function's algorithm for the navigation of the
 * detector volumes. The new code uses an iterator to visit all relevant volumes. The code is now more robust and compact
 * as well as much easier to read and maintain.
 * (3) Changed the chi2 calculation in StiKalmanTrack::getChi2 and propagated the effects of this change
 * in both StiTrackingPlots and StiStEventFiller classes.
 *
 * Revision 2.38  2004/10/27 03:25:49  perev
 * Version V3V
 *
 * Revision 2.37  2004/10/26 21:53:23  pruneau
 * No truncation but bad hits dropped
 *
 * Revision 2.36  2004/10/26 06:45:37  perev
 * version V2V
 *
 * Revision 2.35  2004/10/25 14:15:56  pruneau
 * various changes to improve track quality.
 *
 * Revision 2.34  2004/03/24 22:01:07  pruneau
 * Removed calls to center representation and replaced by normal representation
 *
 * Revision 2.33  2004/03/17 21:01:53  andrewar
 * Trapping for negative track error (^2) values _cYY and _cZZ. This should
 * be a temporary fix until the root of the problem is found. Problem seems
 * localized to trackNodes without hits.
 * Also trapping for asin(x), x>1 in ::length; point to point cord length
 * on the helix is greater than twice radius of curvature. This should also be
 * resovled.
 *
 * Revision 2.32  2004/01/30 21:40:21  pruneau
 * some clean up of the infinite checks
 *
 * Revision 2.31  2003/09/02 17:59:41  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 2.30  2003/08/13 21:04:21  pruneau
 * transfered relevant tracking pars to detector builders
 *
 * Revision 2.29  2003/08/02 08:23:10  pruneau
 * best performance so far
 *
 * Revision 2.28  2003/07/30 19:18:58  pruneau
 * sigh
 *
 * Revision 2.26  2003/07/15 13:56:19  andrewar
 * Revert to previous version to remove bug.
 *
 * Revision 2.24  2003/05/22 18:42:33  andrewar
 * Changed max eloss correction from 1% to 10%.
 *
 * Revision 2.23  2003/05/09 22:07:57  pruneau
 * Added protection to avoid 90deg tracks and ill defined eloss
 *
 * Revision 2.22  2003/05/09 14:57:20  pruneau
 * Synching
 *
 * Revision 2.21  2003/05/08 18:49:09  pruneau
 * fudge=1
 *
 * Revision 2.20  2003/05/07 03:01:39  pruneau
 * *** empty log message ***
 *
 * Revision 2.19  2003/05/03 14:37:22  pruneau
 * *** empty log message ***
 *
 * Revision 2.18  2003/05/01 20:46:47  pruneau
 * changed error parametrization
 *
 * Revision 2.17  2003/04/22 21:20:17  pruneau
 * Added hit filter
 * Tuning og finder pars
 * Tuning of KalmanTrackNode
 *
 * Revision 2.16  2003/04/17 22:49:36  andrewar
 * Fixed getPhase function to conform to StHelixModel convention.
 *
 * Revision 2.15  2003/03/31 17:18:56  pruneau
 * various
 *
 * Revision 2.14  2003/03/13 21:21:27  pruneau
 * getPhase() fixed. MUST inclde -helicity()*pi/2
 *
 * Revision 2.13  2003/03/13 18:59:13  pruneau
 * various updates
 *
 * Revision 2.12  2003/03/12 17:57:31  pruneau
 * Elss calc updated.
 *
 * Revision 2.11  2003/03/04 21:31:05  pruneau
 * Added getX0() and getGasX0() conveninence methods.
 *
 * Revision 2.10  2003/03/04 18:41:27  pruneau
 * Fixed StiHit to use global coordinates as well as locals.
 * Fixed Logic Bug in StiKalmanTrackFinder
 *
 * Revision 2.9  2003/03/04 15:25:48  andrewar
 * Added several functions for radlength calculation.
 *
 */

#include <Stiostream.h>
#include <stdexcept>
#include <math.h>
#include <stdio.h>
#include <assert.h>
using namespace std;

#include "StiHit.h"
#include "StiDetector.h"
#include "StiPlacement.h"
#include "StiMaterial.h"
#include "StiShape.h"
#include "StiPlanarShape.h"
#include "StiCylindricalShape.h"
#include "StiKalmanTrackNode.h"
#include "StiElossCalculator.h"
#include "StDetectorDbMaker/StiTrackingParameters.h"
#include "StDetectorDbMaker/StiKalmanTrackFinderParameters.h"
#include "StDetectorDbMaker/StiHitErrorCalculator.h"
#include "StiTrackNodeHelper.h"
#include "StiFactory.h"
#include "StiUtilities/StiDebug.h"

#include "TString.h"
#if ROOT_VERSION_CODE < 331013
#include "TCL.h"
#else
#include "TCernLib.h"
#endif
#include "THelixTrack.h"
#include "StThreeVector.hh"
#include "StThreeVectorF.hh"

#include "TRMatrix.h"
#include "TRVector.h"
#include "StarMagField.h"
#include "TMath.h"
#include "StMessMgr.h"

#define PrP(A)    { LOG_INFO << "\t" << (#A) << " = \t" << ( A ) }
#define PrPP(A,B) {LOG_INFO  << "=== StiKalmanTrackNode::" << (#A); PrP((B)); LOG_INFO << endm;}
// Local Track Model
//
// x[0] = y  coordinate
// x[1] = z  position along beam axis
// x[2] = (Psi)
// x[3] = C  (local) curvature of the track
// x[4] = tan(l) 

static const double kMaxEta = 1.25; // 72 degrees for laser tracks
static const double kMaxSinEta = sin(kMaxEta);
static const double kMaxCur = 0.2;
static const double kFarFromBeam = 10.;
static const Double_t kMaxZ = 250;
static const Double_t kMaxR = 250;
StiNodeStat StiKalmanTrackNode::mgP; 


static const int    idx33[3][3] = {{0,1,3},{1,2,4},{3,4,5}};
static const int    idx55[5][5] = 
  {{0,1,3,6,10},{1,2,4,7,11},{3,4,5, 8,12},{6,7, 8, 9,13},{10,11,12,13,14}};
static const int    idx55tpt[5][5] = 
  {{0,1,2,3, 4},{1,5,6,7, 8},{2,6,9,10,11},{3,7,10,12,13},{ 4, 8,11,13,14}};

static const int    idx66[6][6] =
  {{ 0, 1, 3, 6,10,15},{ 1, 2, 4, 7,11,16},{ 3, 4, 5, 8,12,17}
  ,{ 6, 7, 8, 9,13,18},{10,11,12,13,14,19},{15,16,17,18,19,20}};

bool StiKalmanTrackNode::useCalculatedHitError = true;
TString StiKalmanTrackNode::comment("Legend: \tE - extapolation\tM Multiple scattering\tV at Vertex\tB at beam\tR at Radius\tU Updated\n");
TString StiKalmanTrackNode::commentdEdx(""); 
//debug vars
//#define STI_ERROR_TEST
//#define STI_DERIV_TEST
#ifdef STI_DERIV_TEST
int    StiKalmanTrackNode::fDerivTestOn=0;   
#endif
#ifndef STI_DERIV_TEST
int    StiKalmanTrackNode::fDerivTestOn=-10;   
#endif

double StiKalmanTrackNode::fDerivTest[kNPars][kNPars];   
int gCurrShape=0;

void StiKalmanTrackNode::Break(int kase)
{
static int myBreak=-2005;
if (kase!=myBreak) return;
  LOG_DEBUG << Form("*** Break(%d) ***",kase)<< endm;
}		
/* bit mask for debug printout  
   0   => 1 - covariance and propagate matrices 
   1   => 2 - hit associated with the node
   2   => 4 - test matrix manipulation
   3   => 8 - test locate
 */
int StiKalmanTrackNode::_debug = 0;
int StiKalmanTrackNode::_laser = 0;

//______________________________________________________________________________
void StiKalmanTrackNode::reset()
{ 
static int myCount=0;
  StiTrackNode::reset();
  memset(_beg,0,_end-_beg+1);
  _ext=0; _inf=0;
  mId = ++myCount; 
  mHz = 999;
  StiDebug::Break(mId);
}
//______________________________________________________________________________
void StiKalmanTrackNode::unset()
{ 
  reduce();
  if (_inf) BFactory::Free(_inf); _inf=0;
}
//______________________________________________________________________________
void StiKalmanTrackNode::resetError(double fak)
{ 
static const double DY=0.3,DZ=0.3,DEta=0.03,DPti=1.,DTan=0.05;

  if (!fak) {
    mFE.reset();
    mFE._cYY=DY*DY;
    mFE._cZZ=DZ*DZ;
    mFE._cEE=DEta*DEta;
    mFE._cPP=DPti*DPti;
    mFE._cTT=DTan*DTan;
  } else {
    for (int i=0;i<kNErrs;i++) mFE.A[i] *=fak;
  }  
  mPE() = mFE;
}
//_____________________________________________________________
/// Set the Kalman state of this node to be identical 
/// to that of the given node.
/// This method is useful to initial the state of a node
/// while propagating a track.
//______________________________________________________________________________
void StiKalmanTrackNode::setState(const StiKalmanTrackNode * n)
{
  _state = n->_state;
  _alpha = n->_alpha;
  mFP = n->mFP;
  mFE = n->mFE;
//assert(mFE.sign()>0);///???
  nullCount = n->nullCount;
  contiguousHitCount = n->contiguousHitCount;
  contiguousNullCount = n->contiguousNullCount;
  setChi2(1e62);  
}

/**
   returns the node information
   double& alpha : angle of the local reference frame
   double& xRef  : refence position of this node in the local frame
   double x[6],  : state, for a definition, see the top of this file
   double cc[21] : error matrix of the state "x"
   double& chi2) : chi2 of the track at this node
*/
//______________________________________________________________________________
void StiKalmanTrackNode::get(double& alpha,
			     double& xRef,
			     double  x[kNPars], 
			     double  e[kNErrs], 
			     double& chi2)
{
  alpha = _alpha;
  xRef  = getRefPosition();
  memcpy(x,mFP.P,kNPars*sizeof(mFP.x()));
  memcpy(e,mFE.A,sizeof(mFE));
  chi2 = getChi2();
}

//______________________________________________________________________________
/*! Calculate/return the track transverse momentum
  <p>
  Calculate the track transverse momentum in GeV/c based on this node's track parameters.
  <p>
  The momentum is calculated based on the track curvature held by this node. A minimum
  curvature of 1e-12 is allowed. 
*/
//______________________________________________________________________________
double StiKalmanTrackNode::getPt() const
{
  assert(!isnan(mFP.ptin()));
  return (fabs(mFP.ptin())<1e-3) ? 1e3: 1./fabs(mFP.ptin());
}
//______________________________________________________________________________
void StiKalmanTrackNode::propagateCurv(const StiKalmanTrackNode *parent)
{
   mFP.ptin()=parent->mFP.ptin();
   mFP.curv()=getHz()*mFP.ptin();
} 
//______________________________________________________________________________
/*! Calculate/return the z component of mag field 
  <p>
  Calculate/return the z component of mag field
  <p>
  Field is calcualated via StarMagField class and cashed. 
*/
//______________________________________________________________________________
double StiKalmanTrackNode::getHz() const
{
  
static const double EC = 2.99792458e-4,ZEROHZ = 2e-6;
   if (fabs(mHz)<999) return mHz;
   if (! _laser) {
     double h[3];
     StarMagField::Instance()->BField(&(getGlobalPoint().x()),h);
     h[2] = EC*h[2];
     if (fabs(h[2]) < ZEROHZ) h[2]=ZEROHZ;
     mHz = h[2];
   } else mHz = ZEROHZ;
   assert(mHz);
   return mHz;
}
//______________________________________________________________________________
/*! Calculate/return track 3-momentum and error.
  <p>
  Calculate the 3-momentum of the track in the local reference frame.
  <P>
    
  <h3>Momentum Representation</h3>
  <TABLE BORDER="0" CELLPADDING="2" CELLSPACING="0" WIDTH="100%">
  <TR>
  <TD WIDTH="10%">p[0]</TD>
  <TD WIDTH="10%">px</TD>
  <TD WIDTH="50%">outward</TD>
  </TR>
  <TR>
  <TD WIDTH="10%">p[1]</TD>
  <TD WIDTH="10%">py</TD>
  <TD WIDTH="50%">along detector plane</TD>
  </TR>
  <TR>
  <TD WIDTH="10%">p[2]</TD>
  <TD WIDTH="10%">pz</TD>
  <TD WIDTH="50%">along beam direction</TD>
  </TR>
  </TABLE>
  <h3>Notes:</h3>
  <ol>
  <li>Throws runtime_error exception if |sin(phi)^2|>1.</li>
  <li>Bypasses error calculation if error array "e" is a null pointer.</li>
  </ol>
*/
//______________________________________________________________________________
void StiKalmanTrackNode::getMomentum(double p[3], double e[6]) const
{	
//	keep in mind that _eta == CA
//	keep in mind that pt == SomeCoef/rho
enum {jX=0,jY,jZ,jE,jP,jT};

  double pt = getPt();
  p[0] = pt*mFP._cosCA;
  p[1] = pt*mFP._sinCA;
  p[2] = pt*mFP.tanl();

// 		if e==0, error calculation is not needed, then return
  if (!e) return;

  double F[3][kNPars]; memset(F,0,sizeof(F));
  double dPtdPi = -pt*pt; if (mFP.ptin()<0) dPtdPi=-dPtdPi;
  F[jX][jE] = -pt    *mFP._sinCA;
  F[jX][jP] =  dPtdPi*mFP._cosCA;
  F[jX][jT] =  0;

  F[jY][jE] =  pt    *mFP._cosCA;
  F[jY][jP] =  dPtdPi*mFP._sinCA;
  F[jY][jT] =  0;
  
  F[jZ][jE] =  0;
  F[jZ][jP] =  dPtdPi*mFP.tanl();
  F[jZ][jT] =  pt;
  
  
  TCL::trasat(F[0],mFE.A,e,3,kNPars);  
}
//______________________________________________________________________________
/**
   returns the node information
   double x[6],  : state, for a definition, in radial implementation
                   rad  - radius at start (cm). See also comments
                   phi  - azimuthal angle  (in rad)      
                   z    - z-coord. (cm)                 
                   psi  - azimuthal angle of pT vector (in rads)     
                   tanl - tan(dip) =pz/pt               
                   curv - Track curvature (1/cm) 
   double cc[15] : error matrix of the state "x" rad is fixed
                       code definition adopted here, where:
   PhiPhi;
   ZPhi     ,ZZ;                       
   TanlPhi  ,TanlZ ,TanlTanl,                 
   PsiPhi   ,PsiZ  ,PsiTanl , PsiPsi ,           
   CurvPhi  ,CurvZ ,CurvTanl, CurvPsi, CurvCurv     

*/
//______________________________________________________________________________
void StiKalmanTrackNode::getGlobalRadial(double  x[6],double  e[15])
{
  enum {jRad=0,jPhi,jZ,jTan,jPsi,jCur, kX=0,kY,kZ,kE,kC,kT};
  double alpha,xRef,chi2;
  double xx[kNPars],ee[kNErrs];

  get(alpha,xRef,xx,ee,chi2);
  
  x[jRad] = sqrt(pow(xx[kX],2)+pow(xx[kY],2));
  x[jPhi] = atan2(xx[kY],xx[kX]) + alpha;
  x[jZ  ] = xx[kZ];
  x[jTan] = xx[kT];
  x[jPsi] = xx[kE] + alpha;
  x[jCur] = xx[kC];
  if (!e) return;

  double F[kNErrs][kNErrs]; memset(F,0,sizeof(F));
  F[jPhi][kX] = -1e5;
  F[jPhi][kY] =  1e5;
  if (fabs(xx[kY])>1e-5)  F[jPhi][kX] = -1./(xx[kY]);
  if (fabs(xx[kX])>1e-5)  F[jPhi][kY] =  1./(xx[kX]);
  F[jZ][kZ]   = 1.;
  F[jTan][kT] = 1;
  F[jPsi][kE] = 1;
  F[jCur][kC] = 1;
  memset(e,0,sizeof(*e)*15);
  for (int k1=0;k1<kNPars;k1++) {
  for (int k2=0;k2<kNPars;k2++) {
    double cc = mFE.A[idx66[k1][k2]];    
    for (int j1=jPhi;j1<= 5;j1++){
    for (int j2=jPhi;j2<=j1;j2++){
      e[idx55[j1-1][j2-1]]+= cc*F[j1][k1]*F[j2][k2];
  }}}}    
  
}
//______________________________________________________________________________
/**
   returns the node information in TPT representation
   double x[6],  : state, for a definition, in radial implementation
                   rad  - radius at start (cm). See also comments
                   phi  - azimuthal angle  (in rad)      
                   z    - z-coord. (cm)                 
                   psi  - azimuthal angle of pT vector (in rads)     
                   tanl - tan(dip) =pz/pt               
                   q/pt -  
   double cc[15] : error matrix of the state "x" rad is fixed
                       code definition adopted here, where:

                                                 Units
                       ______|________________|____________
                       phi*R |  0  1  2  3  4 |  deg*cm
                        z0   |  1  5  6  7  8 |    cm
                       tanl  |  2  6  9 10 11 |    1         covar(i)
                        psi  |  3  7 10 12 13 |   deg
                       q/pt  |  4  8 11 13 14 | e*1/(GeV/c)
                       -----------------------------------

                       and where phi  = atan2(y0,x0)*(180 deg/pi)
                                 R    = sqrt(x0*x0 + y0*y0)
                                 q/pt = icharge*invpt; (This is what the 
                                        radius of curvature actually
                                        determines)
PhiPhi PhiZ PhiTan PhiPsi PhiPt
       ZZ   ZTan   ZPsi     ZPt
            TanTan TanPsi TanPt
                   PsiPsi PsiPt
		           PtPt
*/
//______________________________________________________________________________
void StiKalmanTrackNode::getGlobalTpt(float  x[6],float  e[15])
{
  enum {jRad=0,jPhi,jZ,jTan,jPsi,jCur,jPt=jCur};
static const double DEG = 180./M_PI;
static       double fak[6] = {1,0,1,1,DEG,0};

  double xx[6],ee[15];
  getGlobalRadial(xx,ee);
  double pt = getPt();
  fak[jPhi] = DEG*xx[jRad];
  fak[jPt] = (double(getCharge())/pt)/xx[jCur];

  for (int i=0;i<6;i++) {x[i] = (float)(fak[i]*xx[i]);}
  if (!e) return;

  for (int j1=jPhi;j1<= 5;j1++){
  for (int j2=jPhi;j2<=j1;j2++){
    e[idx55tpt[j1-1][j2-1]] = (float)fak[j1]*fak[j2]*ee[idx55[j1-1][j2-1]];
  }}

}
//______________________________________________________________________________
double StiKalmanTrackNode::getPhase() const
{
  //! This function translates between ITTF helix parameters and
  //! StHelixModel phi. It is only used to fill StTrackGeometry.
  //! For a StPhysicalHelix, phi must be transformed by -h*pi/2.
  return getPsi()-getHelicity()*M_PI/2;

}
//______________________________________________________________________________
double StiKalmanTrackNode::getPsi() const
{
  return nice(mFP.eta()+_alpha);
}

//______________________________________________________________________________
/// returns momentum and its error matrix 
/// in cartesian coordinates in the _global_
/// ref frame of the experiment
/// p[0] = px
/// p[1] = py
/// p[2] = pz
/// Use lower triangular matrix
/// e[0] = px-px
/// e[1] = px-py
/// e[2] = py-py
/// e[3] = px-pz
/// e[4] = py-pz
/// e[5] = pz-pz

//______________________________________________________________________________
void StiKalmanTrackNode::getGlobalMomentum(double p[3], double e[6]) const
{	
  // first get p & e in the local ref frame
  enum {jXX=0,jXY,jYY};
  
  getMomentum(p,e);
  // now rotate the p & e in the global ref frame
  // for the time being, assume an azimuthal rotation 
  // by alpha is sufficient.
  // transformation matrix - needs to be set
  double px=p[0];
  double py=p[1];
  double cosAlpha = cos(_alpha);
  double sinAlpha = sin(_alpha);
  p[0] = cosAlpha*px - sinAlpha*py;
  p[1] = sinAlpha*px + cosAlpha*py;
  if (e==0) return;

    // original error matrix

  double cXX = e[jXX];
  double cXY = e[jXY];
  double cYY = e[jYY];
  double cc = cosAlpha*cosAlpha;
  double ss = sinAlpha*sinAlpha;
  double cs = cosAlpha*sinAlpha;
  e[jXX] = cc*cXX -   2.*cs*cXY + ss*cYY;
  e[jYY] = ss*cXX +   2.*cs*cXY + cc*cYY;
  e[jXY] = cs*cXX + (cc-ss)*cXY - cs*cYY;
}


//______________________________________________________________________________
/*! Steering routine that propagates the track encapsulated by the given node "pNode" to the given detector "tDet". 
	<p>
	The propagation involves the following steps.
 <OL>
 <LI>Extrapolation of the existing track to the next layer, by "transporting" the
     track a smaller radius.</LI>
 <LI>Determine if the extrapolation actually intersects an existing volume.</LI>
 <LI>Exit with status code if no intersection is found.</LI>
 <LI>Transport the error matrix to the new radius.</LI>
 <LI>If mcsCalculated==true, proceed to calculate MCS effects on the error matrix.</LI>
 <LI>if elossCalculated==true, proceed to calculate Eloss effects on the track parameters.</LI>
 </OL>
 <p>Currently, propagate can handle kPlanar and kCylindrical geometries only. An exception is thrown if other geometry shape are used.
*/
//______________________________________________________________________________

//______________________________________________________________________________
int StiKalmanTrackNode::propagate(StiKalmanTrackNode *pNode, 
				  const StiDetector * tDet,int dir)
{
static int nCall=0; nCall++;
StiDebug::Break(nCall);
  int position = 0;
  setState(pNode);
  setDetector(tDet);
  if (debug()) ResetComment(::Form("%30s ",tDet->getName().c_str()));

  StiPlacement * place = tDet->getPlacement();
//double nLayerRadius  = place->getLayerRadius ();
  double nNormalRadius = place->getNormalRadius();

  StiShape * sh = tDet->getShape();
  int shapeCode = sh->getShapeCode();
  double endVal= nNormalRadius,dAlpha=0;
  switch (shapeCode) {

  case kPlanar: 
    { //flat volume
      dAlpha = place->getNormalRefAngle();
      dAlpha = nice(dAlpha - _alpha);
      // bail out if the rotation fails...
      position = rotate(dAlpha);
      if (position) 			return -10;
    }
    					break;
  case kSector:
  case kCylindrical:;
    {
      double xy[2][3];
      position = cylCross(mFP.P,&(mFP._cosCA), mFP.curv(),endVal,dir,xy);
      if (!position) 			return -11;
      int sol = 0;
      for (sol=0;sol<2;sol++) {
        if (!inside(xy[sol][0],xy[sol][1],2|4)) continue;
        dAlpha = atan2(xy[sol][1],xy[sol][0]);
        if (shapeCode==kCylindrical) 	{
          position = rotate(dAlpha);	}
	else 				{
          endVal = xy[sol][0];
        }
        break;
      }
      if (sol>1) 			return -12;
    }
   					break;
  default: assert(0);
  }
   
  position = propagate(endVal,shapeCode,dir); 

  if (position) return position;
  assert(mFP.x() > 0.);
  propagateError();

  // Multiple scattering
  if (StiKalmanTrackFinderParameters::instance()->mcsCalculated() && getHz())  
    propagateMCS(pNode,tDet);
  if (debug() & 8) { PrintpT("M");}
  return position;
}
//______________________________________________________________________________
/*! Propagate the track encapsulated by pNode to the given vertex. Use this node
	to represent the track parameters at the vertex.
  <p>
  This method propagates the track from the given parent node
  "pNode" to the given vertex effectively calculating the
  location (x,y,z) of the track near the given vertex. It use "this" node
 to represent/hold the track parameters at the vertex.
 return true when the propagation is successfull and false otherwise.
<p>
*/
bool StiKalmanTrackNode::propagate(const StiKalmanTrackNode *parentNode, StiHit * vertex,int dir)
{
static int nCall=0; nCall++;
StiDebug::Break(nCall);
  
  setState(parentNode);
  TCircle tc(&mFP.x(),&mFP._cosCA,mFP.curv());
  double xy[2]; xy[0]=vertex->x(),xy[1]=vertex->y();
  double s = tc.Path(xy); tc.Move(s);
  if (fabs(mFP.curv())>0.01) {

    const double *pos = tc.Pos();
    double r21 = pow(pos[0]-xy[0],2)+pow(pos[1]-xy[1],2);
    double s2 = s - M_PI/fabs(mFP.curv());
    TCircle tc2(tc); tc2.Move(s2);
    pos = tc2.Pos();
    double r22 = pow(pos[0]-xy[0],2)+pow(pos[1]-xy[1],2);
    if (r22<r21) tc = tc2;
  }
  double ang = atan2(tc.Dir()[1],tc.Dir()[0]);
  vertex->rotate(ang);
  rotate(ang);
  int ians = propagate(vertex->x(),1,-1);
  if (ians && ians!=kEnded)    return false; // track does not reach vertex "plane"
////  if (ians) return false; ////????
  assert(fabs(mFP._sinCA)<1e-2);
  double qwe = (mFP.x()-vertex->x())*mFP._cosCA 
             + (mFP.y()-vertex->y())*mFP._sinCA;
  assert(fabs(qwe)<1e-4);
  propagateError();
  setHit(vertex);
  setDetector(0);
  return true;
}

//______________________________________________________________________________
///Propagate track from the given node to the beam line with x==0.
///Set the hit and detector pointers to null to manifest this is an extrapolation
bool StiKalmanTrackNode::propagateToBeam(const StiKalmanTrackNode *parentNode,int dir)
{
  StiHit myHit;
  int ians = propagate(parentNode, &myHit,-1);
  if (ians<kEnded) return false;
  setHit(0);
  return true;
}

//______________________________________________________________________________
///Extrapolate the track defined by the given node to the given radius.
///Return a negative value if the operation is impossible.
int StiKalmanTrackNode::propagateToRadius(StiKalmanTrackNode *pNode, double radius,int dir)
{
  int position = 0;
  setState(pNode);
  if (debug()) ResetComment(::Form("%30s ",pNode->getDetector()->getName().c_str()));
  position = propagate(radius,kCylindrical,-1);
  if (position) return position;
  propagateError();
  if (debug() & 8) { PrintpT("R");}
  _detector = 0;
  return position;
}


//______________________________________________________________________________
/*! Work method used to perform the tranport of "this" node from 
  its current "_x" position to the given position "xk". 
  Returns -1 if the propagation cannot be carried out, i.e.
  if the track curvature is such it cannot reach the desired 
  location.
  option == 0 Planar
  option == 1 Cylinder
 */

//______________________________________________________________________________
int  StiKalmanTrackNode::propagate(double xk, int option,int dir)
{
dir = -1; ///????????????????????????
static int nCall=0; nCall++;
StiDebug::Break(nCall);  
  double mySign =mFP.x()*mFP._cosCA+mFP.y()*mFP._sinCA; 


  mgP.x1=mFP.x();  mgP.y1=mFP.y(); mgP.cosCA1 =mFP._cosCA; mgP.sinCA1 =mFP._sinCA;
  double rho = mFP.curv();
  mgP.x2 = xk;

  mgP.dx=mgP.x2-mgP.x1;  
  if (fabs(mgP.dx) < 1e-5) 		return kHit;
  int myDir = (dir>=0) ? (2*dir-1):0;
  double dsin = mFP.curv()*mgP.dx;
  mgP.sinCA2=mgP.sinCA1 + dsin; 
//	Orientation is bad. Fit is non reliable
  if (fabs(mgP.sinCA2)>kMaxSinEta) 	return kFailed; 
  int nTry = 2;
  if (option==kCylindrical) { 	//cyl case
    nTry = 1;
    mgP.cosCA2 = mgP.y1*rho + mgP.cosCA1;
    if (fabs(mgP.cosCA2) > 1) { 
      mgP.cosCA2 =(mgP.cosCA2>0)? 1:-1; 
      mgP.sinCA2=0;}
    }
  else          { 	//planar case
    mgP.cosCA2   = ::sqrt((1.-mgP.sinCA2)*(1.+mgP.sinCA2));
  }
  double bestLen = 1e11;
  StiNodeStat mygP; 
  StiNodePars myFP;
  double myZ = mFP.z(),myPhi = mFP.phi();;
  int iAns=kFailed;
  for (int iCos=0;iCos<nTry;iCos++) {
    if (iCos) mgP.cosCA2 = -mgP.cosCA2;
    mgP.sumSin   = mgP.sinCA1+mgP.sinCA2;
    mgP.sumCos   = mgP.cosCA1+mgP.cosCA2;
    if (fabs(mgP.sumCos)<1e-5) continue;
    mgP.dy = mgP.dx*(mgP.sumSin/mgP.sumCos);
    mgP.y2 = mgP.y1+mgP.dy;
    mgP.dl0 = mgP.cosCA1*mgP.dx+mgP.sinCA1*mgP.dy;
    double sind = mgP.dl0*rho;
    double cosd = mgP.cosCA2*mgP.cosCA1+mgP.sinCA2*mgP.sinCA1;

    if (fabs(sind) < 0.02 && cosd>0) { //tiny angle
      mgP.dl = mgP.dl0*(1.+sind*sind*(1./6 + (sind*sind*3./40)));
    } else {
      mgP.dl = atan2(sind,cosd)/rho;
    }
    if (myDir &&  mgP.dl*myDir<0 ) 	continue;//if we know direction and it is wrong, next one
    if (fabs(mgP.dl)> bestLen) 	continue;//if it is bigger than previous, next one
//??    if (iAns < kTooFar) iAns = kTooFar;
    if (mgP.y2*mgP.y2+mgP.x2*mgP.x2>kMaxR*kMaxR)	continue;
    mFP.z()  = myZ + mgP.dl*mFP.tanl();
    if (fabs(mFP.z()) > kMaxZ) 	continue;
    mFP.x()  = mgP.x2;
    mFP.y()  = mgP.y2;
    if (locate())		continue;
    mFP.eta() = nice(myPhi+rho*mgP.dl);  	/*VP*/
    mFP._sinCA   = mgP.sinCA2;
    mFP._cosCA   = mgP.cosCA2;
    bestLen = fabs(mgP.dl);   
    mygP = mgP;
    myFP = mFP;
    iAns = kHit;
  }  
  if (bestLen >= 1e10) 		return kFailed;
  iAns = kHit;
  mFP = myFP;
  mgP = mygP;
  if (fabs(mFP._cosCA)>0.999 || fabs(mFP._sinCA)>0.999) mFP.ready();
  mFP.hz()      = getHz();
  assert(fabs(mFP.hz()) > 1e-7);
  if (fabs(mFP.hz()) > 1e-10) 	{ mFP.curv() = mFP.hz()*mFP.ptin();}
  else 				{ mFP.curv() = 1e-6 ;}
  mFP.ready();
  mPP() = mFP;
  if (mFP.x()> kFarFromBeam) {
    if ((mFP.x()*mFP._cosCA+mFP.y()*mFP._sinCA)*mySign<0)	return kEnded; 
  }
  return 0;
}




//______________________________________________________________________________
int StiKalmanTrackNode::nudge(StiHit *hitp)
{
  enum { kTooFar = 33};

  StiHit *hit = hitp;
  double deltaX = 0,rN=0,sCA2,cCA2,deltaY,deltaL;
  int kase = 0,kaze = 0,shapeCode=0;		// 0=shift accounting deltaX, 1=use THelixTrack

  do {
   if ( hit) 			{kase = 1; break;}
   if ( !_detector && _hit)	{kase = 1; hit = _hit; break;}
   if ( !_detector) return 0;
   shapeCode =_detector->getShape()->getShapeCode();
   rN = _detector->getPlacement()->getNormalRadius();
   if ( shapeCode ==1) 		{kase = 2; break;}
   kase = 3;
  } while(0);

  switch(kase) {
    case 1: { deltaX = hit->x()-mFP.x(); break;}
    case 2: { deltaX = rN-mFP.x()      ; break;}
    case 3: {
      double t = 0.5*(rN*rN-mFP.rxy2())/(mFP.x()*mFP._cosCA+mFP.y()*mFP._sinCA);
      deltaX = mFP._cosCA*t;
      if (fabs(t) > 0.1*rN || fabs(t*mFP.curv()) > 0.01) kaze = 1; break;}
    default: { assert(0 && "Wrong Node");}
  }//end switch
  
  if (fabs(deltaX)>kTooFar)	kaze = 1;
  if (fabs(deltaX) <1.e-5) 	return  0;

  switch(kaze) {
  case 0: {// easy way
    double deltaS = mFP.curv()*(deltaX);
    sCA2 = mFP._sinCA + deltaS;
    if (fabs(sCA2)>0.99) 		return -2;
    cCA2= sqrt((1.-sCA2)*(1.+sCA2));
    if (mFP._cosCA <0) cCA2 = -cCA2;
    deltaY = deltaX*(mFP._sinCA+sCA2)/(mFP._cosCA+cCA2);
    deltaL = deltaX*mFP._cosCA + deltaY*mFP._sinCA;
    double sind = deltaL*mFP.curv();
    deltaL = asin(sind)/mFP.curv();
    double deltaZ = mFP.tanl()*(deltaL);
    mFP._sinCA = sCA2;
    mFP._cosCA = cCA2;
    mFP.x()   += deltaX;
    mFP.y()   += deltaY;
    mFP.z()   += deltaZ;
    mFP.eta() += deltaL*mFP.curv();
    break;}

  case 1: {// hard way, use THelixTrack
 
    THelixTrack hlx(mFP.P,&mFP._cosCA,mFP.curv()); 
    double surf[7]={0};
    int nSurf=0;
    switch(shapeCode) {
      case 0:;case kPlanar:
        surf[0]=mFP.x()+deltaX;surf[1]=-1;nSurf=4;break;
      case kCylindrical: 
      case kSector: 
        surf[0]=rN*rN; surf[4]=-1; surf[5]=-1; nSurf=7;break;
      default: assert(0 && "Wrong shape code"); 
    }//end switch

    double x[3];
    deltaL = hlx.Path(999.,surf,nSurf,x,0,1);
    if (fabs(deltaL) >=999) 		return -2;
//??    mFP.phi() += deltaL*mFP.curv();
    mFP.eta() += deltaL*mFP.curv();
    memcpy(mFP.P,x,sizeof(x));
    mFP.ready();
    break;}
  }

  mPP() = mFP;
  return 0;
}
//______________________________________________________________________________
/// Make propagation matrix 
/// \note This method must be called ONLY after a call to the propagate method.
void StiKalmanTrackNode::propagateMtx()
{  
//  	fYE == dY/dEta
  double fYE= mgP.dx*(1.+mgP.cosCA1*mgP.cosCA2+mgP.sinCA1*mgP.sinCA2)/(mgP.sumCos*mgP.cosCA2);
//	fEC == dEta/dRho
  double  fEC = mgP.dx/mgP.cosCA2;
//	fYC == dY/dRho
  double fYC=(mgP.dy*mgP.sinCA2+mgP.dx*mgP.cosCA2)/mgP.sumCos*fEC;
// 	fZC == dZ/dRho
  double dang = mgP.dl*mFP.curv();
  double C2LDX = mgP.dl*mgP.dl*(
                   0.5*mgP.sinCA2*pow((1+pow(dang/2,2)*sinX(dang/2)),2) +
                   mgP.cosCA2*dang*sinX(dang));

  double fZC = mFP.tanl()*C2LDX/mgP.cosCA2;
//	fZE == dZ/dEta
  double dLdEta = mgP.dy/mgP.cosCA2;
  double fZE =  mFP.tanl()*dLdEta;

//  	fZT == dZ/dTanL; 
  double fZT= mgP.dl; 
  double hz = getHz(); fEC *=hz; fYC*=hz; fZC*=hz;

  mMtx().reset();
//  X related derivatives
  mMtx().A[0][0] = -1;
  mMtx().A[1][0] = -mgP.sinCA2/mgP.cosCA2; 
  mMtx().A[2][0] = -mFP.tanl() /mgP.cosCA2;
  mMtx().A[3][0] = -mFP.curv() /mgP.cosCA2;

  mMtx().A[1][3]=fYE; mMtx().A[1][4]=fYC; mMtx().A[2][3]=fZE;
  mMtx().A[2][4]=fZC; mMtx().A[2][5]=fZT; mMtx().A[3][4]=fEC;
}



//______________________________________________________________________________
/// Propagate the track error matrix
/// \note This method must be called ONLY after a call to the propagate method.
void StiKalmanTrackNode::propagateError()
{  
  static int nCall=0; nCall++;
  StiDebug::Break(nCall);

  propagateMtx();
  mFE.recov();
  errPropag6(mFE.A,mMtx().A,kNPars);
  mFE.recov();
  mFE._cXX = mFE._cYX= mFE._cZX = mFE._cEX = mFE._cPX = mFE._cTX = 0;
// now set hiterrors
   if (_hit) setHitErrors();

// set state node is ready
  mPE() = mFE;
}

//______________________________________________________________________________
/*! Calculate the effect of MCS on the track error matrix.
  <p>
  The track is assumed to propagate from (x0,y0,z0) to (mgP.x1,y1,z1). The calculation
  is performed for the given mass hypothesis which given a momentum determines the
  speed "beta" of the particle. The calculation of the average scattering angle
  is delegated to the function "mcs2". The calculation of energy loss is done
  by the function eloss.
 */

/*!Calulates length between center of this node and provided node, which
  is assumed to be on the same helix. Have to use global coords, since 
  nodes may not be in the same detector volume.

  \returns (double) length
*/
//delta(mgP.dx,dy,dz) = here - there
double StiKalmanTrackNode::pathLToNode(const StiKalmanTrackNode * const oNode)
{
  const StThreeVector<double> delta = 
    getGlobalPoint() - oNode->getGlobalPoint();
  double R = getCurvature();
  // s = 2c * asin( t/(2c)); t=::sqrt(mgP.dx^2+dy^2+dz^2)
  return length(delta, R);
}

//______________________________________________________________________________
inline double StiKalmanTrackNode::length(const StThreeVector<double>& delta, double curv)
{
  
  double m = delta.perp();
  double as = 0.5*m*fabs(curv);
  if (as >= 1) {
    if (as > 1.1)
    cout << "StiKalmanTrackNode::length m = " << m << " curv = " << curv << " as = " << as << " illegal. reset to 1" << endl;
    as = 0.99;
  }
  double lxy=0;
  if (fabs(as)<0.01) { lxy = m*(1.+as*as/24);}
  else               { lxy = 2.*asin(as)/fabs(curv);}
  return sqrt(lxy*lxy+delta.z()*delta.z());
}

//______________________________________________________________________________
/*! Calculate the increment of chi2 caused by the addition of this node to the track.
  <p>
  Uses the track extrapolation to "_x", and hit position to evaluate and return the 
  increment to the track chi2.
  The chi2 is not stored internally in this node. 
  <p>
  <h3>Notes</h3>
  <ol>
  <li>Use full error matrices.</li>
  <li>Return increment in chi2 implied by the node/hit assocition.</li>
  <li>Throws an exception if numerical problems arise.</li>
  </ol>
*/
double StiKalmanTrackNode::evaluateChi2(const StiHit * hit) 
{
  double r00, r01,r11;
  //If required, recalculate the errors of the detector hits.
  //Do not attempt this calculation for the main vertex.
  double dsin =mFP.curv()*(hit->x()-mFP.x());
  if (fabs(mFP._sinCA+dsin)>0.99   )	return 1e41;
  if (fabs(mFP.eta())       >kMaxEta) 	return 1e41;
  if (fabs(mFP.curv())      >kMaxCur)    return 1e41;
  if (mHrr.hYY>1000*mFE._cYY
   && mHrr.hZZ>1000*mFE._cZZ)		return 1e41;
  setHitErrors(hit);
  r00=mHrr.hYY+mFE._cYY;
  r01=mHrr.hZY+mFE._cZY;  
  r11=mHrr.hZZ+mFE._cZZ;

#ifdef Sti_DEBUG
  TRSymMatrix R(2,
		r00,
		r01, r11);
#endif
  _det=r00*r11 - r01*r01;
  if (_det<r00*r11*1.e-5) {
    LOG_DEBUG << Form("StiKalmanTrackNode::evalChi2 *** zero determinant %g",_det)<< endm;
    return 1e60;
  }
  double tmp=r00; r00=r11; r11=tmp; r01=-r01;  
  double deltaX = hit->x()-mFP.x();
  double deltaL = deltaX/mFP._cosCA;
  double deltaY = mFP._sinCA *deltaL;
  double deltaZ = mFP.tanl()  *deltaL;
  double dyt=(mFP.y()-hit->y()) + deltaY;
  double dzt=(mFP.z()-hit->z()) + deltaZ;
  double cc= (dyt*r00*dyt + 2*r01*dyt*dzt + dzt*r11*dzt)/_det;

#ifdef Sti_DEBUG
  if (debug() & 4) {
    TRSymMatrix G(R,TRArray::kInverted);
    TRVector r(2,hit->y()-mFP.y(),hit->z()-mFP.z());
    Double_t chisq = G.Product(r,TRArray::kATxSxA);
    Double_t diff = chisq - cc;
    Double_t sum  = chisq + cc;
    if (diff > 1e-7 || (sum > 2. && (2 * diff ) / sum > 1e-7)) {
      LOG_DEBUG << "Failed:\t" << chisq << "\t" << cc << "\tdiff\t" << diff << endm;
    }
  }
#endif
  if (debug() & 8) {comment += Form(" chi2 = %6.2f",cc);}
  return cc;
}
//______________________________________________________________________________
int StiKalmanTrackNode::isEnded() const
{

   if(fabs(mFP.eta() )<=kMaxEta) return 0;
   return 1;   
}		
//______________________________________________________________________________
int StiKalmanTrackNode::isDca() const
{
   return (!_detector && fabs(mFP.x())<=0);   
}		
		
//______________________________________________________________________________
/*! Calculate the effect of MCS on the track error matrix.
  <p>
  The track is assumed to propagate from (x0,y0,z0) to (x1,y1,z1). The calculation
  is performed for the given mass hypothesis which given a momentum determines the
  speed "beta" of the particle. The calculation of the average scattering angle
  is delegated to the function "mcs2". The calculation of energy loss is done
  by the function eloss.
 */
//______________________________________________________________________________
void StiKalmanTrackNode::propagateMCS(StiKalmanTrackNode * previousNode, const StiDetector * tDet)
{  
static int nCall=0; nCall++;
  propagateCurv(previousNode);
  double pt = getPt();
  if (pt>=1e2) {
    mPP() = mFP; mPE() = mFE;
    return;
  }
  double relRadThickness;
  // Half path length in previous node
  double pL1=0,pL2=0,pL3=0,d1=0,d2=0,d3=0,dxEloss=0;
  if (previousNode->inside()) 	pL1=previousNode->pathlength()/2.;
  // Half path length in this node
  if (inside()) 		pL3=pathlength()/2.;
  // Gap path length
  pL2= pathLToNode(previousNode);
  if (pL1<0) pL1=0;
  if (pL2<0) pL2=0;
  if (pL3<0) pL3=0;
  double x0p =1e11,x0Gas=1e11,x0=1e11;

  if (pt > 0.350 && TMath::Abs(getHz()) < 1e-3) pt = 0.350;
  double p2=(1.+mFP.tanl()*mFP.tanl())*pt*pt;
  double m=StiKalmanTrackFinderParameters::instance()->massHypothesis();
  double m2=m*m;
  double e2=p2+m2;
  double beta2=p2/e2;


  const StiDetector 		*preDet = previousNode->getDetector();
  const StiMaterial 		*preMat = preDet->getMaterial();
  const StiElossCalculator	*preLos = preMat->getElossCalculator();
  d1 =(preLos) ? preLos->calculate(1.,m, beta2):0;
  x0p = preMat->getX0();

  const StiMaterial 		*curMat = tDet->getMaterial();
  const StiElossCalculator	*curLos = curMat->getElossCalculator();
  d3 =(curLos) ? curLos->calculate(1.,m, beta2):0;
  x0 = curMat->getX0();
  double sign = (mgP.dx>0)? 1:-1;

//		Gas of detector is placed UNDER of it
  const StiMaterial		*gasMat = (sign>0)? tDet->getGas() : preDet->getGas();
  x0Gas = gasMat->getX0();
  const StiElossCalculator	*gasLos = gasMat->getElossCalculator();
  d2 =(gasLos) ? gasLos->calculate(1.,m, beta2):0;

  pL2=pL2-pL1-pL3; if (pL2<0) pL2=0;
  relRadThickness = pL1/x0p+pL2/x0Gas+pL3/x0;
  dxEloss         =  d1*pL1+ d2*pL2  + d3*pL3;

  //cout << " m2:"<<m2<<" p2:"<<p2<<" beta2:"<<beta2;
  double theta2=mcs2(relRadThickness,beta2,p2);
  //cout << " theta2:"<<theta2;
  double pti = mFP.ptin(), tanl = mFP.tanl(); 
  
  double cos2Li = (1.+ tanl*tanl);  // 1/cos(lamda)**2
 
  mFE._cEE += cos2Li 		*theta2;
  mFE._cPP += tanl*tanl*pti*pti	*theta2;
  mFE._cTP += pti*tanl*cos2Li	*theta2;
  mFE._cTT += cos2Li*cos2Li	*theta2;
assert(mFE._cEE>0);
assert(mFE._cPP>0);
assert(mFE._cTT>0);

//assert(mFE.sign()>0); ///??? 

  double dE = sign*dxEloss;
//		save detLoss and gasLoss for investigation only
  
  mELoss[0].mELoss = 2*d3*pL3;
  mELoss[0].mLen   = 2*pL3;
  mELoss[0].mDens  = curMat->getDensity();
  mELoss[0].mX0    = x0;
  mELoss[1].mELoss = 2*d2*pL2;
  mELoss[1].mLen   = 2*pL2;
  mELoss[1].mDens  = gasMat->getDensity();
  mELoss[1].mX0    = x0Gas;
 
 if (fabs(dE)>0)
    {
      double correction =1. + ::sqrt(e2)*dE/p2;
      if (correction>1.1) correction = 1.1;
      else if (correction<0.9) correction = 0.9;
      mFP.curv() = mFP.curv()*correction;
      mFP.ptin() = mFP.ptin()*correction;
    }
    mPP() = mFP; mPE() = mFE;
}
//______________________________________________________________________________
/*! Update the track parameters using this node.
  <p>
  This method uses the hit contained by node to update the track 
  parameters contained by this node and thus complete the propagation
  of this track to the location x=_x.
  <p>
  <OL>
  <li>Throw a runtime_error exception if no hit is actually associated with this node.</li>
  <li>Compute the measurement error matrix "r". Invert it.
  <li>Update the measurement matrix "k" and calculate updated curvature, eta, and pitch.
  <li>Update track error matrix.</li>
  </OL>
  <p>
  <h3>Notes</h3>
  <ol>
  <li>Throw logic_error if no hit is associated with this node.</li>
  <li>Throw runtime_error if determinent of "r" matrix is null.
  </ol>
*/
//______________________________________________________________________________
int StiKalmanTrackNode::updateNode() 
{
static int nCall=0; nCall++;
//assert(mFE.sign()>0); ///??? 
  double r00,r01,r11;
  r00 = mHrr.hYY + mFE._cYY;
  r01 = mHrr.hZY + mFE._cZY;
  r11 = mHrr.hZZ + mFE._cZZ;
  _det=r00*r11 - r01*r01;
  if (!finite(_det) || _det<(r00*r11)*1.e-5) {
    LOG_DEBUG << Form("StiKalmanTrackNode::updateNode *** zero determinant %g",_det)
    << endm;
    return -11;
  }
  // inverse matrix
  double tmp=r00; r00=r11/_det; r11=tmp/_det; r01=-r01/_det;
  // update error matrix
  double k00=mFE._cYY*r00+mFE._cZY*r01, k01=mFE._cYY*r01+mFE._cZY*r11;
  double k10=mFE._cZY*r00+mFE._cZZ*r01, k11=mFE._cZY*r01+mFE._cZZ*r11;
  double k20=mFE._cEY*r00+mFE._cEZ*r01, k21=mFE._cEY*r01+mFE._cEZ*r11;
  double k30=mFE._cPY*r00+mFE._cPZ*r01, k31=mFE._cPY*r01+mFE._cPZ*r11;
  double k40=mFE._cTY*r00+mFE._cTZ*r01, k41=mFE._cTY*r01+mFE._cTZ*r11;
  double dyt  = getHit()->y() - mFP.y();
  double dzt  = getHit()->z() - mFP.z();
  double dp3  = k30*dyt + k31*dzt;
  double dp2  = k20*dyt + k21*dzt;
  double dp4  = k40*dyt + k41*dzt;
  double eta  = nice(mFP.eta() + dp2);
  if (fabs(eta)>kMaxEta) return -14;
  double pti  = mFP.ptin() + dp3;
  if (fabs(pti) < 1e-3) pti=1e-3;
  double cur  = pti*getHz();
  if (fabs(cur)>kMaxCur) return -16;
  assert(finite(cur));
  double tanl = mFP.tanl() + dp4;
  // update Kalman state
   double p0 = mFP.y() + k00*dyt + k01*dzt;
//VP  mFP.y() += k00*dy + k01*dz;
  if (fabs(p0)>kMaxR) 
    {
      LOG_DEBUG << "updateNode()[1] -W- _y:"<<mFP.y()<<" _z:"<<mFP.z()<<endm;
      return -12;
    }
  double p1 = mFP.z() + k10*dyt + k11*dzt;
  if (fabs(p1)>kMaxZ) 
    {
      LOG_DEBUG << "updateNode()[2] -W- _y:"<<mFP.y()<<" _z:"<<mFP.z()<<endm;
      return -13;
    }
  //mFP.tanl() += k40*dyt + k41*dzt;
  //  double sinCA  =  sin(eta);
  // The following test introduces a track propagation error but happens
  // only when the track should be aborted so we don't care...
  mFP.y()  = p0;
  mFP.z()  = p1;
  mFP.eta()   = eta;
  mFP.ptin()  = pti;
  mFP.curv()  = cur;
  mFP.tanl()  = tanl;
  mFP.ready();
  assert(!_detector || mFP.x() > 0.);
  mFP.hz() = getHz();

// update error matrix
  double c00=mFE._cYY;                       
  double c10=mFE._cZY, c11=mFE._cZZ;                 
  double c20=mFE._cEY, c21=mFE._cEZ;//, c22=mFE._cEE;           
  double c30=mFE._cPY, c31=mFE._cPZ;//, c32=mFE._cPE, c33=mFE._cPP;     
  double c40=mFE._cTY, c41=mFE._cTZ;//, c42=mFE._cTE, c43=mFE._cTP, c44=mFE._cTT;
  mFE._cYY-=k00*c00+k01*c10;
  mFE._cZY-=k10*c00+k11*c10;mFE._cZZ-=k10*c10+k11*c11;
  mFE._cEY-=k20*c00+k21*c10;mFE._cEZ-=k20*c10+k21*c11;mFE._cEE-=k20*c20+k21*c21;
  mFE._cPY-=k30*c00+k31*c10;mFE._cPZ-=k30*c10+k31*c11;mFE._cPE-=k30*c20+k31*c21;mFE._cPP-=k30*c30+k31*c31;
  mFE._cTY-=k40*c00+k41*c10;mFE._cTZ-=k40*c10+k41*c11;mFE._cTE-=k40*c20+k41*c21;mFE._cTP-=k40*c30+k41*c31;mFE._cTT-=k40*c40+k41*c41;
//assert(mFE.sign()>0); ///??? 

  if (mFE._cYY >= mHrr.hYY || mFE._cZZ >= mHrr.hZZ) {
    LOG_DEBUG << Form("StiKalmanTrackNode::updateNode *** _cYY >= hYY || _cZZ >= hZZ %g %g %g %g"
          ,mFE._cYY,mHrr.hYY,mFE._cZZ,mHrr.hZZ)<< endm;
    return -14;
  }
  if (mFE.check()) return -14;

  if (debug() & 8) PrintpT("U");

  return 0; 
}

//______________________________________________________________________________
/*! Rotate this node track representation azymuthally by given angle.
  <p>
  This method rotates by an angle alpha the track representation 
  held by this node. 
  <h3>Notes</h3>
  <ol>
  <li>The rotation is bound between -M_PI and M_PI.</li>
  <li>Throws runtime_error if "(_y-y0)*_curv>=0" in order to avoid math exception.</li>
  <li>Avoid undue rotations as they are CPU intensive...</li>
  </ol>
*/
int StiKalmanTrackNode::rotate (double alpha) //throw ( Exception)
{
  mMtx().A[0][0]=0;
  if (fabs(alpha)<1.e-6) return 0;
  _alpha += alpha;
  _alpha = nice(_alpha);

  StiNodePars *myPars[3]= {&mFP,0,0};
  if (_ext) myPars[1]= &(_ext->mPP);
  if (_inf) myPars[2]= &(_inf->mPP);

  StiNodeErrs *myErrs[3]= {&mFE,0,0};
  if (_ext) myErrs[1]= &(_ext->mPE);
  if (_inf) myErrs[2]= &(_inf->mPE);

  for (int jk=0;jk<3;jk++) {
    if (!myPars[jk]) 				continue;
    StiNodePars &myFP = *myPars[jk];
    StiNodeErrs &myFE = *myErrs[jk];
    if (fabs(myFP.x())+fabs(myFP.y())<=0)	continue;
    myFP.rotate(alpha);

    if (myFE._cYY<=0) continue;
    myFE.rotate(alpha,myFP);

  //cout << " myFP._sinCA:"<<myFP._sinCA<<endl;
    assert(fabs(myFP._sinCA)<=1.);
    assert(fabs(myFP._cosCA)<=1.);
  }

  memset(mMtx().A,0,sizeof(mMtx()));
  mPP() = mFP;
  return 0;
}


//_____________________________________________________________________________
/// print to the ostream "os" the parameters of this node 
/// and all its children recursively
ostream& operator<<(ostream& os, const StiKalmanTrackNode& n)
{
  const StiDetector *det = n.getDetector();
  if (det) os  <<"Det:"<<n.getDetector()->getName();
  else     os << "Det:UNknown";
  os << " a:" << 180*n._alpha/M_PI<<" degs"
     << "\tx:" << n.mFP.x()
     << " p0:" << n.mFP.y() 
     << " p1:" << n.mFP.z() 
     << " p2:" << n.mFP.eta() 
     << " p3:" << n.mFP.ptin() 
     << " p4:" << n.mFP.tanl()
     << " c00:" <<n.mFE._cYY<< " c11:"<<n.mFE._cZZ 
     << " pT:" << n.getPt() << endl;
  if (n.debug() & 2) {
    StiHit * hit = n.getHit();
    if (hit) os << "\thas hit with chi2 = " << n.getChi2()
		<< " n:"<<n.hitCount
		<< " null:"<<n.nullCount
		<< endl<<"\t hit:"<<*hit;
  }
  else os << endl;
  return os;
}

//______________________________________________________________________________
double StiKalmanTrackNode::getWindowY()
{	  
  const StiDetector * detector = getDetector();
  const StiTrackingParameters * parameters = detector->getTrackingParameters();
  double searchWindowScale = parameters->getSearchWindowScale();
  double minSearchWindow   = parameters->getMinSearchWindow();
  double maxSearchWindow   = parameters->getMaxSearchWindow();

  const StiHitErrorCalculator * calc = detector->getHitErrorCalculator();
  double myEyy,myEzz;
  calc->calculateError(&mFP,myEyy,myEzz);
  double window = searchWindowScale*::sqrt(mFE._cYY+myEyy);
  if      (window<minSearchWindow) window = minSearchWindow;
  else if (window>maxSearchWindow) window = maxSearchWindow;
  return window;
}

//_____________________________________________________________________________
double StiKalmanTrackNode::getWindowZ()
{	 
  const StiDetector * detector = getDetector();
  const StiTrackingParameters * parameters = detector->getTrackingParameters();
  double searchWindowScale = parameters->getSearchWindowScale();
  double minSearchWindow   = parameters->getMinSearchWindow();
  double maxSearchWindow   = parameters->getMaxSearchWindow();

  const StiHitErrorCalculator * calc = detector->getHitErrorCalculator();
  double myEyy,myEzz;
  calc->calculateError(&mFP,myEyy,myEzz);
  
  double window = searchWindowScale*::sqrt(mFE._cZZ+myEzz); 
  if      (window<minSearchWindow) window = minSearchWindow;
  else if (window>maxSearchWindow) window = maxSearchWindow;
  return window;
}

//______________________________________________________________________________
StThreeVector<double> StiKalmanTrackNode::getHelixCenter() const
{
  if (mFP.curv()==0) throw logic_error("StiKalmanTrackNode::getHelixCenter() -F- _curv==0 ");
  double xt0 = mFP.x()-mFP._sinCA/mFP.curv();   /*VP*/
  double yt0 = mFP.y()+mFP._cosCA/(mFP.curv());
  double zt0 = mFP.z()+mFP.tanl()*asin(mFP._sinCA)/mFP.curv();
  double cosAlpha = cos(_alpha);
  double sinAlpha = sin(_alpha);
  return (StThreeVector<double>(cosAlpha*xt0-sinAlpha*yt0,sinAlpha*xt0+cosAlpha*yt0,zt0));
}
//______________________________________________________________________________
int StiKalmanTrackNode::locate()
{
  double yOff, zOff,ang;
  //fast way out for projections going out of fiducial volume
  const StiDetector *tDet = getDetector();
  if (!tDet) return 0;
  const StiPlacement *place = tDet->getPlacement();
  const StiShape     *sh    = tDet->getShape();

  if (fabs(mFP.z())>kMaxZ || mFP.rxy()> kMaxR) return -1;
  
  
  //YF edge is tolerance when we consider that detector is hit. //  edge = 0; //VP the meaning of edge is not clear
  Int_t shapeCode  = sh->getShapeCode();
  switch (shapeCode) {
  case kDisk:
  case kCylindrical: // cylinder
    break;
  case kSector: 	// cylinder sector
    ang = atan2(mFP.y(),mFP.x());
    yOff    = nice(ang +_alpha - place->getLayerAngle());
    if (fabs(yOff)>sh->getOpeningAngle()/2) return -1;
    break;
  case kPlanar: 
    yOff = mFP.y() - place->getNormalYoffset();
    if (fabs(yOff)> sh->getHalfWidth()) return -1;
    break;
  default: assert(0 && "Wrong Shape code");
  }
  zOff = mFP.z() - place->getZcenter();
  if (fabs(zOff)>sh->getHalfDepth()) return -1;
  return 0;
 }
//______________________________________________________________________________
void StiKalmanTrackNode::initialize(StiHit *h)
{
static const double initAng = atan2(0.6,0.8);
  reset();
  setHit(h);
  _detector = h->detector();
  _alpha   = _detector->getPlacement()->getNormalRefAngle(); 
  mFP._sinCA = 0.6;
  mFP._cosCA = 0.8;
  mFP.phi() = initAng;
  mFP.x() = h->x();
  mFP.y() = h->y();
  mFP.z() = h->z();
  mFP.hz() = getHz();
  resetError();
  mPP() = mFP;
  setHitErrors();
  setChi2(0.1);
}
//______________________________________________________________________________
void StiKalmanTrackNode::initialize(StiDetector *d)
{
  reset();
  _detector = d;
  _alpha   = _detector->getPlacement()->getNormalRefAngle(); 
  setChi2(1e10);
}
//______________________________________________________________________________
StiKalmanTrackNode::StiKalmanTrackNode(const StiKalmanTrackNode &n)
{
   _ext=0; _inf=0 ; *this = n;
}
//______________________________________________________________________________
const StiKalmanTrackNode& StiKalmanTrackNode::operator=(const StiKalmanTrackNode &n)
{
  StiTrackNode::operator=(n);
  memcpy(_beg,n._beg,_end-_beg+1);
  if (n._ext) { extend();*_ext = *n._ext;}
  else        { if(_ext) _ext->reset();  }
  if (n._inf) { extinf();*_inf = *n._inf;}
  else        { if(_inf) {BFactory::Free(_inf); _inf=0;}}
  return *this;
}
//______________________________________________________________________________
void StiKalmanTrackNode::setHitErrors(const StiHit *hit)
{
  if (!hit) hit = _hit;
  assert(hit);
  StiTrackNodeHelper::getHitErrors(hit,&mFP,&mHrr);
}
//______________________________________________________________________________
StiHitErrs StiKalmanTrackNode::getGlobalHitErrs(const StiHit *hit) const
{
   StiHitErrs hr;
   StiTrackNodeHelper::getHitErrors(hit,&mFP,&hr);
   hr.rotate(_alpha);
   return hr;
}
//______________________________________________________________________________
int StiKalmanTrackNode::testError(double *emx, int begend)
{
// Test and correct error matrix. Output : number of fixes
// DO NOT IMPROVE weird if() here. This accounts NaN


  static int nCall=0; nCall++;
  static const double dia[6] = { 1000.,1000., 1000.,1000.,1000,1000.};
  static double emxBeg[kNErrs];
//return 0;
  if (!begend) { memcpy(emxBeg,emx,sizeof(emxBeg));}
  int ians=0,j1,j2,jj;
  for (j1=0; j1<5;j1++){
    jj = idx55[j1][j1];
    if (!(emx[jj]>0)) {;
      LOG_DEBUG << Form("<StiKalmanTrackNode::testError> Negative diag %g[%d][%d]",emx[jj],j1,j1)
      << endm;
 	continue;}
    if (emx[jj]<=10*dia[j1] )	continue;
    assert(finite(emx[jj]));
    LOG_DEBUG << Form("<StiKalmanTrackNode::testError> Huge diag %g[%d][%d]",emx[jj],j1,j1)
    << endm;
    				continue;
//    ians++; emx[jj]=dia[j1];
//    for (j2=0; j2<5;j2++){if (j1!=j2) emx[idx55[j1][j2]]=0;}
  }
  for (j1=0; j1< 5;j1++){
  for (j2=0; j2<j1;j2++){
    jj = idx55[j1][j2];
    assert(finite(emx[jj]));
    double cormax = emx[idx55[j1][j1]]*emx[idx55[j2][j2]];
    if (emx[jj]*emx[jj]<cormax) continue;
    cormax= sqrt(cormax);
//    ians++;emx[jj]= (emx[jj]<0) ? -cormax:cormax;
  }}
  return ians;
}
//______________________________________________________________________________
void StiKalmanTrackNode::numeDeriv(double val,int kind,int shape,int dir)
{
   double maxStep[kNPars]={0.1,0.1,0.1,0.01,0.001,0.01};
   if (fDerivTestOn<0) return;
   gCurrShape = shape;
   fDerivTestOn=-1;
   double save[20];
   StiKalmanTrackNode myNode;
   double *pars = &myNode.mFP.x();
   int state=0;
   saveStatics(save);
   if (fabs(mFP.curv())> 0.02) goto FAIL;
   int ipar;
   for (ipar=1;ipar<kNPars;ipar++)
   {
     for (int is=-1;is<=1;is+=2) {
       myNode = *this;
       backStatics(save);
       double step = 0.1*sqrt((mFE.A)[idx66[ipar][ipar]]);
       if (step>maxStep[ipar]) step = maxStep[ipar];
//       if (step>0.1*fabs(pars[ipar])) step = 0.1*pars[ipar];
//       if (fabs(step)<1.e-7) step = 1.e-7;
       pars[ipar] +=step*is;
// 		Update sinCA & cosCA       
       myNode.mFP._sinCA = sin(myNode.mFP.eta());
       if (fabs(myNode.mFP._sinCA) > 0.9) goto FAIL;
       myNode.mFP._cosCA = cos(myNode.mFP.eta());
       
       switch (kind) {
	 case 1: //propagate
	   state = myNode.propagate(val,shape,dir); break;
	 case 2: //rotate
	   state = myNode.rotate(val);break;
	 default: assert(0);  
       }  
       if(state  ) goto FAIL;

       for (int jpar=1;jpar<kNPars;jpar++) {
	 if (is<0) {
	   fDerivTest[jpar][ipar]= pars[jpar];
	 } else      {
	   double tmp = fDerivTest[jpar][ipar];
	   fDerivTest[jpar][ipar] = (pars[jpar]-tmp)/(2.*step);
	   if (ipar==jpar) fDerivTest[jpar][ipar]-=1.;
         }
       }
     }
   }
   fDerivTestOn=1;backStatics(save);return;
FAIL: 
   fDerivTestOn=0;backStatics(save);return;
}
//______________________________________________________________________________
int StiKalmanTrackNode::testDeriv(double *der)
{
   if (fDerivTestOn!=1) return 0;
   double *num = fDerivTest[0];
   int nerr = 0;
   for (int i=1;i<kNErrs;i++) {
     int ipar = i/kNPars;
     int jpar = i%kNPars;
     if (ipar==jpar)					continue;
     if (ipar==0)					continue;
     if (jpar==0)					continue;
     double dif = fabs(num[i]-der[i]);
     if (fabs(dif) <= 1.e-5) 				continue;
     if (fabs(dif) <= 0.2*0.5*fabs(num[i]+der[i]))	continue;
     nerr++;
     LOG_DEBUG << Form("***Wrong deriv [%d][%d] = %g %g %g",ipar,jpar,num[i],der[i],dif)
     << endm;
  }
  fDerivTestOn=0;
  return nerr;
}
//______________________________________________________________________________
void StiKalmanTrackNode::saveStatics(double *sav)
{  
  sav[ 0]=mgP.x1;
  sav[ 1]=mgP.x2; 
  sav[ 2]=mgP.y1; 
  sav[ 3]=mgP.y2; 
  sav[ 5]=mgP.dx; 
  sav[ 6]=mgP.cosCA1; 
  sav[ 7]=mgP.sinCA1; 
  sav[ 8]=mgP.cosCA2; 
  sav[ 9]=mgP.sinCA2; 
  sav[10]=mgP.sumSin; 
  sav[11]=mgP.sumCos; 
  sav[14]=mgP.dl; 
  sav[15]=mgP.dl0; 
  sav[16]=mgP.dy; 
}  
//______________________________________________________________________________
void StiKalmanTrackNode::backStatics(double *sav)
{  
  mgP.x1=             sav[ 0];
  mgP.x2= 		  sav[ 1]; 
  mgP.y1= 		  sav[ 2]; 
  mgP.y2= 		  sav[ 3]; 
  mgP.dx= 	  sav[ 5]; 
  mgP.cosCA1= 	  sav[ 6]; 
  mgP.sinCA1= 	  sav[ 7]; 
  mgP.cosCA2= 	  sav[ 8]; 
  mgP.sinCA2= 	  sav[ 9]; 
  mgP.sumSin= 	  sav[10]; 
  mgP.sumCos= 	  sav[11]; 
  mgP.dl=             sav[14];
  mgP.dl0=            sav[15];
  mgP.dy=             sav[16];
}
//________________________________________________________________________________
void   StiKalmanTrackNode::PrintpT(const Char_t *opt) const {
  // opt = "E" extapolation
  //       "M" Multiple scattering
  //       "V" at Vertex
  //       "B" at beam
  //       "R" at Radius
  //       "U" Updated
  //       mFP fit parameters
  //       mFE fit errors
  //       _ext->mPP 
  //       _ext->mPE
  //       _ext->mMtx
  Double_t dpTOverpT = 100*TMath::Sqrt(mFE._cPP/(mFP.ptin()*mFP.ptin()));
  if (dpTOverpT > 9999.9) dpTOverpT = 9999.9;
  comment += ::Form(" %s pT %8.3f+-%6.1f sy %6.4f",opt,getPt(),dpTOverpT,TMath::Sqrt(mFE._cYY));
}
//________________________________________________________________________________
void StiKalmanTrackNode::PrintStep() {
  LOG_INFO << comment << "\t" << commentdEdx << endm;
  ResetComment();
}
//________________________________________________________________________________
int   StiKalmanTrackNode::print(const char *opt) const
{
static const char *txt = "xyzeptchrXYZED";
//locals  xyz, e=Psi,p=1/pt, c=curvature, h=mag field, r=rxy
//global  XYZ, E=Psi D= direction +=outside

static const char *hhh = "uvwUVW";
static const char *HHH = "xyzXYZ";
  if (!opt || !opt[0]) opt = "2xh";
  StiHit *hit = getHit();
  TString ts;
  if (!isValid()) ts+="*";
  if (hit) {ts+=(getChi2()>1e3)? "h":"H";}
  printf("%p(%s)",(void*)this,ts.Data());
  if (strchr(opt,'2')) printf("\t%s=%g","ch2",getChi2());
  double val;
  for (int i=0;txt[i];i++) {
    if (!strchr(opt,txt[i])) continue;
    switch(i) {
      case 0:;case 1:;case 2:; case 3:;case 4:;case 5:;case 6:;case 7:;
      val = mFP[i]; break;
      case  8: val = mFP.rxy(); break;
      case  9: val = x_g(); 	break;
      case 10: val = y_g(); 	break;
      case 11: val = z_g();	break;
      case 12: val = getPsi();	break;
      case 13: val = mFP._cosCA*mFP[0]+mFP._sinCA*mFP[1];

    }
    printf("\t%c=%g",txt[i],val);
  }//end for i

  for (int i=0;hit && hhh[i];i++) {
    if (!strchr(opt,hhh[i])) continue;
    switch(i) {
      case 0:val = hit->x(); 	break;
      case 1:val = hit->y(); 	break;
      case 2:val = hit->z(); 	break;
      case 3:val = hit->x_g(); 	break;
      case 4:val = hit->y_g(); 	break;
      case 5:val = hit->z_g();	break;
    }
    printf("\th%c=%g",HHH[i],val);
  }
  if (isValid()) printf(" valid");
  if (getDetector()) {printf(" %s",getDetector()->getName().c_str());}
  printf("\n");
  return 1;
}    
//________________________________________________________________________________
StiNodeExt *StiKalmanTrackNode::nodeExtInstance()
{    
static StiFactory<StiNodeExt,StiNodeExt> *extFactory=0;
  if (!extFactory) {
    extFactory = StiFactory<StiNodeExt,StiNodeExt>::myInstance();
    extFactory->setMaxIncrementCount(400000);
    extFactory->setFastDelete();
  }
  return extFactory->getInstance();
}    
//________________________________________________________________________________
StiNodeInf *StiKalmanTrackNode::nodeInfInstance()
{    
static StiFactory<StiNodeInf,StiNodeInf> *infFactory=0;
  if (!infFactory) {
    infFactory = StiFactory<StiNodeInf,StiNodeInf>::myInstance();
    infFactory->setMaxIncrementCount(40000);
    infFactory->setFastDelete();
  }
  return infFactory->getInstance();
}    
//________________________________________________________________________________
void StiKalmanTrackNode::extend()
{
  if(_ext) return;
  _ext =  nodeExtInstance();
}
//________________________________________________________________________________
void StiKalmanTrackNode::extinf()
{
  if(_inf) return;
  _inf =  nodeInfInstance();
}
//________________________________________________________________________________
void StiKalmanTrackNode::saveInfo(int kase)
{
  if (kase){};
  extinf();
  _inf->mPP = mPP();
  _inf->mPE = mPE();
  _inf->mHrr = mHrr;
}

//________________________________________________________________________________
void StiKalmanTrackNode::reduce()
{
  if(!_ext) return;
  BFactory::Free(_ext); _ext=0;
}


//________________________________________________________________________________
StThreeVector<double> StiKalmanTrackNode::getPoint() const
{
  return StThreeVector<double>(mFP.x(),mFP.y(),mFP.z());
}

//________________________________________________________________________________
StThreeVector<double> StiKalmanTrackNode::getGlobalPoint() const
{
  double ca = cos(_alpha),sa=sin(_alpha);
  return StThreeVector<double>(ca*mFP.x()-sa*mFP.y(), sa*mFP.x()+ca*mFP.y(), mFP.z());
}

//________________________________________________________________________________
double StiKalmanTrackNode::x_g() const
{
  return cos(_alpha)*mFP.x()-sin(_alpha)*mFP.y();
}

//________________________________________________________________________________
double StiKalmanTrackNode::y_g() const
{
  return sin(_alpha)*mFP.x()+cos(_alpha)*mFP.y();
}

//________________________________________________________________________________
double StiKalmanTrackNode::z_g() const
{
  return mFP.z();
}

//________________________________________________________________________________
void StiKalmanTrackNode::setUntouched() 
{
  mUnTouch.set(mPP(),mPE());
}
//________________________________________________________________________________
double StiKalmanTrackNode::getTime() const
{
  static const double smax = 1e3; 
  double time = 0;
  if (! _laser) {
    double d = sqrt(mFP.x()*mFP.x()+mFP.y()*mFP.y());
    double sn = fabs(mFP._cosCA*mFP.y() - mFP._sinCA*mFP.x())/d;
    if (sn> 0.99) sn =  0.99;
    if (sn<0.2) {
      d *= (1.+sn*sn/6);
    } else {
      d *= asin(sn)/sn;
    }
    d *= sqrt(1.+mFP.tanl()*mFP.tanl());
    double beta = 1;   
    double pt = fabs(mFP.ptin());
    if (pt>0.1) {
      pt = 1./pt;
      double p2=(1.+mFP.tanl()*mFP.tanl())*pt*pt;
      double m=StiKalmanTrackFinderParameters::instance()->massHypothesis();
      double m2=m*m;
      double e2=p2+m2;
      double beta2=p2/e2;
      beta = TMath::Sqrt(beta2);
    }
    time = d/(TMath::Ccgs()*beta*1e-6); // mksec  
  } else {
    if (TMath::Abs(mFP.z()) > 20.0) {
static const double Radius = 197.;
static const int    nsurf  = 6;
static const double surf[6] = {-Radius*Radius, 0, 0, 0, 1, 1};
      double dir[3] = {mFP._cosCA,mFP._sinCA,mFP.tanl()};
      THelixTrack tc(mFP.P,dir,mFP.curv());
      double s = tc.Step(smax, surf, nsurf,0,0,1);
      if (TMath::Abs(s) < smax) 
	time = TMath::Abs(s)/(TMath::Ccgs()*1e-6); // mksec
    }
  }
  return time;
}

//________________________________________________________________________________
int StiKalmanTrackNode::inside(int mode) const 
{
const StiDetector *det = getDetector();
if (!det) return 1;
int ans = inside(mFP.P,mode);
return ans;
}
//________________________________________________________________________________
int StiKalmanTrackNode::inside(double x,double y,int mode) const 
{
const StiDetector *det = getDetector();
  if (!det) return 1;
  const StiPlacement *place = det->getPlacement();
  double z = place->getZcenter();
  double xyz[3] = {x,y,z};
return inside(xyz,mode);
}
//________________________________________________________________________________
int StiKalmanTrackNode::inside(const double *x,int mode) const 
{
const StiDetector *det = getDetector();
  if (!det) return 1;
  const StiPlacement *place = det->getPlacement();
  double placeAng = place->getNormalRefAngle();
  double dAng = placeAng-_alpha;
  double xyz[3];
  const double *X = x;
  if (fabs(dAng)>1e-6) {
    xyz[0] = x[0]*cos(dAng)+x[1]*sin(dAng);
    xyz[1] =-x[0]*sin(dAng)+x[1]*cos(dAng);
    xyz[2] = x[2];
    X = xyz;
  }
  return det->insideL(X,mode);
}
