/***************************************************************************
 *
 *  
 *
 * Author: Mike Lisa, Ohio State, lisa@mps.ohio-state.edu
 ***************************************************************************
 *
 * Description: part of STAR HBT Framework: StHbtMaker package
 *   a do-nothing v0 cut that simply says "true" to every v0
 *
 ***************************************************************************
 *
 *  
 *
 **************************************************************************/

#include "StHbtMaker/Cut/dummyV0Cut.h"

ClassImp(dummyV0Cut)

//________________________
dummyV0Cut::dummyV0Cut(){
  mNpassed = mNfailed = 0;
}
//________________________
bool dummyV0Cut::Pass(const StHbtV0* v0)
{
  mNpassed++;
  return (true);
}
//________________________
StHbtString dummyV0Cut::Report()
{
  return "dummyV0Cut\n";
}
//________________________
void dummyV0Cut::SetMass(const double& mass)
{
  mMass = mass;
}

