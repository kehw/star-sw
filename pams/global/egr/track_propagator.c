/*:>--------------------------------------------------------------------
**: FILE:       track_propagator.c.template
**: HISTORY:
**:             00jan93-v000a-hpl- Created by stic Version
**:  Id: idl.y,v 1.14 1998/08/28 21:22:28 fisyak Exp  
**:<------------------------------------------------------------------*/
#include "track_propagator.h"

long track_propagator_(
  TABLE_HEAD_ST         *gtrack_h,      DST_TRACK_ST           *gtrack ,
  TABLE_HEAD_ST         *target_h,  EGR_PROPAGATE_ST           *target )
{
/*:>--------------------------------------------------------------------
**: ROUTINE:    track_propagator_
**: DESCRIPTION: Physics Analysis Module ANSI C template.
**:             This is an ANSI C Physics Analysis Module template
**:             automatically generated by stic from track_propagator.idl.
**:             Please edit comments and code.
**: AUTHOR:     hpl - H.P. Lovecraft, hplovecraft@cthulhu.void
**: ARGUMENTS:
**:       IN:
**:             target    - PLEASE FILL IN DESCRIPTION HERE
**:            target_h   - header Structure for target
**:    INOUT:
**:             gtrack    - PLEASE FILL IN DESCRIPTION HERE
**:            gtrack_h   - header Structure for gtrack
**:      OUT:
**: RETURNS:    STAF Condition Value
**:>------------------------------------------------------------------*/
  
  long i;
  float bfld=5., GRADDEG=57.2958;
  float psi, pt, tanl, x0, y0, z0, xp[2], xout[4];
  long q;


    for(i=0; i<gtrack_h->nok; i++) 
      {
	psi  = gtrack[i].psi;
	q    = (long) gtrack[i].icharge;
	tanl = gtrack[i].tanl;
	if(gtrack[i].invpt != 0 )
	  {
	    pt    = 1/gtrack[i].invpt ;
	  }
	else
	  {
	    /*	    message('Error, invpt); */
	    pt = 0.01;
	  }
	x0   = gtrack[i].x0 ;
	y0  = gtrack[i].y0 ;
	z0   = gtrack[i].z0;
	xp[0]= target->x[0]; 
	xp[1]= target->x[1];
	
	project_track_(&psi, &q, &pt, &tanl, &x0, &y0, &bfld, &z0, xp, xout); 
	
	gtrack[i].x0 = xout[0];
	gtrack[i].y0 = xout[1];
	gtrack[i].z0 = xout[2];
	gtrack[i].psi= xout[3];
      }


   return STAFCV_OK;
}
