//-*- Mode: C++ -*-
// ************************************************************************
// This file is property of and copyright by the ALICE HLT Project        *
// ALICE Experiment at CERN, All rights reserved.                         *
// See cxx source for full Copyright notice                               *
//                                                                        *
//*************************************************************************


#ifndef ALIHLTTPCCAMERGEROUTPUT_H
#define ALIHLTTPCCAMERGEROUTPUT_H

#include "AliHLTTPCCADef.h"
#include "AliHLTTPCCAMergedTrack.h"
#include "AliHLTTPCCADataCompressor.h"

/**
 * @class AliHLTTPCCAMergerOutput
 *
 * AliHLTTPCCAMergerOutput class is used to store the output of AliHLTTPCCATracker{Component}
 * and transport the output to AliHLTTPCCAMerger{Component}
 *
 * The class contains all the necessary information about TPC tracks, reconstructed in one slice.
 * This includes the reconstructed track parameters and some compressed information
 * about the assigned clusters: clusterId, position and amplitude.
 *
 */
class AliHLTTPCCAMergerOutput
{
  public:

    AliHLTTPCCAMergerOutput()
        : fNTracks( 0 ), fNTrackClusters( 0 ), fTracks( 0 ), fClusterIDsrc( 0 ), fClusterPackedAmp( 0 ) {}

    AliHLTTPCCAMergerOutput( const AliHLTTPCCAMergerOutput & )
        : fNTracks( 0 ), fNTrackClusters( 0 ), fTracks( 0 ), fClusterIDsrc( 0 ), fClusterPackedAmp( 0 ) {}

    const AliHLTTPCCAMergerOutput& operator=( const AliHLTTPCCAMergerOutput &/*v*/ ) const {
      return *this;
    }

    ~AliHLTTPCCAMergerOutput() {}


    int NTracks()                    const { return fNTracks;              }
    int NTrackClusters()             const { return fNTrackClusters;       }

    const AliHLTTPCCAMergedTrack &Track( int i ) const { return fTracks[i]; }
    const DataCompressor::SliceRowCluster &ClusterIDsrc     ( int i )  const { return fClusterIDsrc[i]; }
    UChar_t  ClusterPackedAmp( int i )  const { return fClusterPackedAmp[i]; }

    static int EstimateSize( int nOfTracks, int nOfTrackClusters );
    void SetPointers();

    void SetNTracks       ( int v )  { fNTracks = v;        }
    void SetNTrackClusters( int v )  { fNTrackClusters = v; }

    void SetTrack( int i, const AliHLTTPCCAMergedTrack &v ) {  fTracks[i] = v; }
    void SetClusterIDsrc( int i, const DataCompressor::SliceRowCluster &v ) {  fClusterIDsrc[i] = v; }
    void SetClusterPackedAmp( int i, UChar_t v ) {  fClusterPackedAmp[i] = v; }

  private:

    int fNTracks;                 // number of reconstructed tracks
    int fNTrackClusters;          // total number of track clusters
    AliHLTTPCCAMergedTrack *fTracks; // pointer to reconstructed tracks
    DataCompressor::SliceRowCluster *fClusterIDsrc;         // pointer to cluster IDs ( packed IRow and ICluster)
    UChar_t  *fClusterPackedAmp;    // pointer to packed cluster amplitudes

};



inline int AliHLTTPCCAMergerOutput::EstimateSize( int nOfTracks, int nOfTrackClusters )
{
  // calculate the amount of memory [bytes] needed for the event

  const int kClusterDataSize = sizeof( unsigned int ) + sizeof( UChar_t );

  return sizeof( AliHLTTPCCAMergerOutput ) + sizeof( AliHLTTPCCAMergedTrack )*nOfTracks + kClusterDataSize*nOfTrackClusters;
}


inline void AliHLTTPCCAMergerOutput::SetPointers()
{
  // set all pointers

  fTracks            = ( AliHLTTPCCAMergedTrack* )( ( &fClusterPackedAmp ) + 1 );
  fClusterIDsrc      = ( DataCompressor::SliceRowCluster* )  ( fTracks            + fNTracks );
  fClusterPackedAmp  = ( UChar_t* ) ( fClusterIDsrc + fNTrackClusters );
}

#endif
