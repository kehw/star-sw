//*-- Author :    Valery Fine   14/05/99  (E-mail: fine@bnl.gov)
// $Id: St_GeantHitsPoints.cxx,v 1.1 2000/03/09 16:22:15 fine Exp $

// ***********************************************************************
// * Observer to draw use ant St_Table object as an element of "event" geometry
// * Copyright(c) 1997~1999  [BNL] Brookhaven National Laboratory, STAR, All rights reserved
// * Author                  Valerie Fine  (fine@bnl.gov)
// * Copyright(c) 1997~1999  Valerie Fine  (fine@bnl.gov)
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// *
// * Permission to use, copy, modify and distribute this software and its
// * documentation for any purpose is hereby granted without fee,
// * provided that the above copyright notice appear in all copies and
// * that both that copyright notice and this permission notice appear
// * in supporting documentation.  Brookhaven National Laboratory makes no
// * representations about the suitability of this software for any
// * purpose.  It is provided "as is" without express or implied warranty.
// ************************************************************************

#include "St_GeantHitsPoints.h"

///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//   Defines the St_Table as an element of "event" geometry                                                                                 //
//                                                                                   //
//  #include "St_GeantHitsPoints.h"                                                      //
//  #include "St_<your_table_name_here>.h"                                           //
//                                                                                   //
//  class St_<your_table_name_here>_Points : public St_GeantHitsPoints                   //
//  {                                                                                //
//    protected:                                                                     //
//       <your_table_name_here>_st   *m_Rows; // pointer to the STAF table           //
//    public:                                                                        //
//       St_<your_table_name_here>_Points(St_TableSorter *sorter,const void *key,Option_t *opt):
//                St_GeantHitsPoints(sorter,key,opt){}                                   //
//       virtual  ~St_<your_table_name_here>_Points(){} // default destructor        //
//       virtual Float_t GetX(Int_t indx) { return ((<your_table_name_here>_st *)m_Rows)[Indx(idx)]-> <x>;}               //
//       virtual Float_t GetY(Int_t indx) { return ((<your_table_name_here>_st *)m_Rows)[Indx(idx)]-> <y>;}               //
//       virtual Float_t GetZ(Int_t indx) { return ((<your_table_name_here>_st *)m_Rows)[Indx(idx)]-> <z>;}               //
//  };                                                                               //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

ClassImp(St_GeantHitsPoints)

//____________________________________________________________________________
St_GeantHitsPoints::St_GeantHitsPoints()
{
  m_TableSorter =  0;
  m_Key         =  0;
  m_FirstRow    = -1;
  m_Size        =  0;
}
//____________________________________________________________________________
St_GeantHitsPoints::St_GeantHitsPoints(St_TableSorter *sorter,const void *key,Option_t *opt)
{
  m_TableSorter =  0;
  m_Key         =  0;
  m_FirstRow    = -1;
  m_Size        =  0;
  if (sorter) {
     m_TableSorter = sorter;
     m_Key         = key;
     m_Size        = sorter->CountKey(m_Key,0,kTRUE,&m_FirstRow);
     SetTablePointer(GetTable());
  }
  SetOption(opt);
}

//____________________________________________________________________________
St_GeantHitsPoints::St_GeantHitsPoints(St_TableSorter *sorter, Int_t keyIndex,Option_t *opt)
{
  m_TableSorter =  0;
  m_Key         =  0;
  m_FirstRow    = -1;
  m_Size        =  0;
  if (sorter) {
     m_TableSorter = sorter;
     m_Key         = sorter->GetKeyAddress(keyIndex);
     m_Size        = sorter->CountKey(m_Key,keyIndex,kFALSE,&m_FirstRow);
     SetTablePointer(GetTable());
  }
  SetOption(opt); 
}

//______________________________________________________________________________
Int_t St_GeantHitsPoints::DistancetoPrimitive(Int_t px, Int_t py)
{
//*-*-*-*-*-*-*Compute distance from point px,py to a 3-D points *-*-*-*-*-*-*
//*-*          =====================================================
//*-*
//*-*  Compute the closest distance of approach from point px,py to each segment
//*-*  of the polyline.
//*-*  Returns when the distance found is below DistanceMaximum.
//*-*  The distance is computed in pixels units.
//*-*
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* 
   return -1;
}

//______________________________________________________________________________
// $Log: St_GeantHitsPoints.cxx,v $
// Revision 1.1  2000/03/09 16:22:15  fine
// new class to draw GEANT hits (under development)
//
// Revision 1.4  1999/12/17 23:28:41  fine
// clean up for the sake of docs + new class St_Table3DPackedPoints introduced
//
// Revision 1.3  1999/11/16 16:29:57  fine
// TObject::GetObjectInfo() implemented
//
// Revision 1.2  1999/11/04 18:03:10  fine
// new ctor for tablepoints introduced to make EventDiplay happy
//
// Revision 1.1  1999/05/18 20:21:25  fine
// New class to 3D table viewer
//______________________________________________________________________________


