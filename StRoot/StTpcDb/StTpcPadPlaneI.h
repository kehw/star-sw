/***************************************************************************
 *
 * $Id: StTpcPadPlaneI.h,v 1.4 2000/01/12 15:14:41 hardtke Exp $
 *
 * Author:  David Hardtke
 ***************************************************************************
 *
 * Description: Abstract Interface for Tpc Pad Plane Geometry  
 *
 ***************************************************************************
 *
 * $Log: StTpcPadPlaneI.h,v $
 * Revision 1.4  2000/01/12 15:14:41  hardtke
 * Update StTpcWirePlanes to use new variable names in tpcWirePlanes.idl/ Add Z position functions to StTpcPadPlane
 *
 * Revision 1.3  1999/12/16 22:00:54  hardtke
 * add CVS tags
 *
 **************************************************************************/
#ifndef __STTPCPADPLANEI__
#define __STTPCPADPLANEI__
#include <TObject.h>

class StTpcPadPlaneI : public TObject {

  //Abstract base class defining accessors
public:

  virtual int   numberOfRows()           const = 0;
  virtual int   numberOfInnerRows()      const = 0;
  virtual int   numberOfInnerRows48()    const = 0;
  virtual int   numberOfInnerRows52()    const = 0;
  virtual int   numberOfOuterRows()      const = 0;
  virtual float innerSectorPadWidth()    const = 0;
  virtual float innerSectorPadLength()   const = 0;
  virtual float innerSectorPadPitch()    const = 0;
  virtual float innerSectorRowPitch1()   const = 0;
  virtual float innerSectorRowPitch2()   const = 0;
  virtual float firstPadRow()            const = 0;
  virtual float firstOuterSectorPadRow() const = 0;
  virtual float lastOuterSectorPadRow()  const = 0;
  virtual float firstRowWidth()          const = 0; 
  virtual float lastRowWidth()           const = 0;
  virtual float outerSectorPadWidth()    const = 0;
  virtual float outerSectorPadLength()   const = 0;
  virtual float outerSectorPadPitch()    const = 0;
  virtual float outerSectorRowPitch()    const = 0;
  virtual float outerSectorLength()      const = 0;
  virtual float ioSectorSeparation()     const = 0;
  virtual float innerSectorEdge()        const = 0;
  virtual float outerSectorEdge()        const = 0;
  virtual float innerSectorPadPlaneZ()   const = 0;
  virtual float outerSectorPadPlaneZ()   const = 0;
 
  virtual int   numberOfPadsAtRow(int row)    const = 0;
  virtual float radialDistanceAtRow(int row) const = 0;  
  virtual float PadWidthAtRow(int row)       const = 0;
  virtual float PadLengthAtRow(int row)      const = 0;
  virtual float PadPitchAtRow(int row)       const = 0;
  virtual float RowPitchAtRow(int row)       const = 0;

  virtual int indexForRowPad(int row, int pad)  const = 0;


ClassDef(StTpcPadPlaneI,0)

};
#endif















