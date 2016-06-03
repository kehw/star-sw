// $Id: StMuDstVtxT.cxx,v 2.6.2.1 2016/06/03 15:49:00 smirnovd Exp $
#include "StMuDstVtxT.h"
#include "StKFVertex.h"
ClassImp(StMuDstVtxT);
std::ostream&  operator<<(std::ostream& os,  const StMuDstVtxT& v) {
  os << Form("Q:%3i M:%3i/%3i/%3i W/E %3i/%3i tracks rank %5i xyz = %9.3f +/- %5.3f, %9.3f +/- %5.3f, %9.3f +/- %5.3f",
	     v.Q(),v.MultU(),v.Mult(),v.MultC(),v.MultW(),v.MultE(),v.Rank(),
	     v.Xyz().x(),v.SigmaXyz().x(),
	     v.Xyz().y(),v.SigmaXyz().x(),
	     v.Xyz().z(),v.SigmaXyz().x())
     << Form(" Mc/QA/t:%4i/%3i/%6.0f xyz: %8.3f%8.3f%8.3f m:%4i %6s",v.IdTruth(), v.QaTruth(),
	     v.TimeMc(), v.XyzMc().X(), v.XyzMc().Y(), v.XyzMc().Z(), 
	     v.NoDaughtersMc(),StKFVertex::GeNames[v.gePidMc()]);
  return os;
}
// $Log: StMuDstVtxT.cxx,v $
// Revision 2.6.2.1  2016/06/03 15:49:00  smirnovd
// Revert "Squashed commit of the following:"
//
// This reverts commit b0c5699a781ed8e5724e065390d3870af5de5b7c.
//
// Revision 2.4  2013/04/10 22:14:20  fisyak
// Roll back to version 04/04/2013
//
// Revision 2.2  2012/06/11 15:33:41  fisyak
// std namespace
//
// Revision 2.1  2012/05/07 14:56:14  fisyak
// Add StKFVertexMaker
//
// Revision 1.2  2012/02/07 19:38:26  fisyak
// Repackage
//
