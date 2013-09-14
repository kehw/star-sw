// $Id: StvStl.cxx,v 1.3 2012/12/07 17:49:57 perev Exp $
//
//
// Class StvStl some stl containers for Stv objects
// ------------------
#include "StvStl.h"
#include "StvHit.h"
//_____________________________________________________________________________
StvHits &StvHits::operator+=(const StvHits &add)
{ insert(end(),add.begin(),add.end()); return *this;}
//_____________________________________________________________________________
StvHits &StvHits::operator+=(const std::vector<void*> &add)
{
  const StvHits &myAdd = (const StvHits &)add;
  (*this)+=myAdd;
  return *this;
}
//_____________________________________________________________________________
void StvHits::unused()
{
  for (int i=0;i<(int)size();i++) {(*this)[i]->setTimesUsed(0);}
}
//_____________________________________________________________________________
StvPoints &StvPoints::operator+=(const float  add[3])
{ push_back(add[0]); push_back(add[1]); push_back(add[2]);return *this;}
//_____________________________________________________________________________
StvPoints &StvPoints::operator+=(const double  add[3])
{ push_back(add[0]); push_back(add[1]); push_back(add[2]);return *this;}