#ifndef StiSvtDetectorBuilder_H
#define StiSvtDetectorBuilder_H
#include "StSvtDbMaker/StSvtDbMaker.h"
#include "StSvtDbMaker/St_SvtDb_Reader.hh"
#include "StSvtClassLibrary/StSvtConfig.hh"
#include "StSvtClassLibrary/StSvtGeometry.hh"
#include "StSvtClassLibrary/StSvtHybridCollection.hh"
#include "StDbUtilities/StSvtLocalCoordinate.hh"
#include "StDbUtilities/StSvtCoordinateTransform.hh"
#include "StThreeVector.hh"
#include "Sti/StiDetectorBuilder.h"
class StiShape;
class StiPlanarShape;
class StiMaterial;
class StSvtCoordinateTransform;
class StiDefaultHitErrorCalculator;

class StiSvtDetectorBuilder : public StiDetectorBuilder
{
	
 public:
	StiSvtDetectorBuilder(bool active);
	virtual ~StiSvtDetectorBuilder(); 
	virtual void loadDb();
	virtual void buildMaterials();
	virtual void buildShapes();
	virtual void buildDetectors();

 protected:
	
	double phiForSvtBarrelLadder(unsigned int iBarrel, 
				     unsigned int iLadder) const;
	StiMaterial   * _gasMat;
	StiMaterial   * _siMat;
	StiMaterial   * _hybridMat;
	StiPlanarShape * _waferShape[6];
	StiPlanarShape * _hybridShape[6];
	StSvtConfig   * _config;
	StSvtGeometry * _geometry;
	StSvtCoordinateTransform * _transform;
	StiDefaultHitErrorCalculator * _calc;
};


inline double StiSvtDetectorBuilder::phiForSvtBarrelLadder(unsigned int layer, 
							   unsigned int ladder) const
{
  if(layer<0 || layer>=getNRows())
    cout << "phiForSvtLayerLadder(" << layer << ", " << ladder<< "): invalid layer" << endl;
  if(ladder<0 || ladder>=getNSectors(layer))
    cout << "phiForSvtLayerLadder(" << layer << ", " << ladder<< "): invalid ladder" << endl;
  return phiForSector(ladder, getNSectors(layer));
} // phiForSvtLayerLadder
#endif 
