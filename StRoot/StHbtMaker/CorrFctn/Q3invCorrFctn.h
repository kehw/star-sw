/***************************************************************************
 *
 * $Id: Q3invCorrFctn.h,v 1.4 2000/08/08 23:39:21 laue Exp $
 *
 * Author: Robert Willson, Ohio State, willson@bnl.gov 
 ***************************************************************************
 *
 * Description: part of STAR HBT Framework: StHbtMaker package
 *   A simple Q-invariant correlation function for three particle analyses.    
 *
 ***************************************************************************
 *
 * $Log: Q3invCorrFctn.h,v $
 * Revision 1.4  2000/08/08 23:39:21  laue
 * Updated for standalone version
 *
 * Revision 1.3  2000/05/11 21:17:30  willson
 * Modified CorrFctn class
 *
 * Revision 1.2  2000/04/12 01:53:28  willson
 * Initial Installation - Comments Added
 *
 * 
 ***************************************************************************/


#ifndef Q3invCorrFctn_hh
#define Q3invCorrFctn_hh

#include "StHbtMaker/Base/StHbtCorrFctn.hh"

class Q3invCorrFctn : public StHbtCorrFctn {
public:
  Q3invCorrFctn(char* title, const int& nbins, const float& QinvLo, const float& QinvHi);
  virtual ~Q3invCorrFctn();

  virtual StHbtString Report();
  virtual void AddRealTriplet(const StHbtTriplet*);
  virtual void AddMixedTriplet(const StHbtTriplet*);

  virtual void Finish();

  StHbt1DHisto* Numerator();
  StHbt1DHisto* Denominator();
  StHbt1DHisto* Ratio();

private:
  StHbt1DHisto* mNumerator;
  StHbt1DHisto* mDenominator;
  StHbt1DHisto* mRatio;

#ifdef __ROOT__
  ClassDef(Q3invCorrFctn, 1)
#endif

};

inline  StHbt1DHisto* Q3invCorrFctn::Numerator(){return mNumerator;}
inline  StHbt1DHisto* Q3invCorrFctn::Denominator(){return mDenominator;}
inline  StHbt1DHisto* Q3invCorrFctn::Ratio(){return mRatio;}


#endif

