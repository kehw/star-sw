/*:>--------------------------------------------------------------------
**: FILE:       tfs_filt.c.template
**: HISTORY:
**:             00jan93-v000a-hpl- Created by stic Version
**:  Id: idl.y,v 1.7 1996/10/02 21:07:52 ward Exp  
**:<------------------------------------------------------------------*/
#include "tfs_filt.h"

long type_of_call tfs_filt_(
  TABLE_HEAD_ST          *tphit_h,      TCL_TPHIT_ST            *tphit )
{
/*:>--------------------------------------------------------------------
**: ROUTINE:    tfs_filt_
**: DESCRIPTION: Physics Analysis Module ANSI C template.
**:             This is an ANSI C Physics Analysis Module template
**:             automatically generated by stic from tfs_filt.idl.
**:             Please edit comments and code.
**: AUTHOR:     hpl - H.P. Lovecraft, hplovecraft@cthulhu.void
**: ARGUMENTS:
**:       IN:
**:    INOUT:
**:              tphit    - PLEASE FILL IN DESCRIPTION HERE
**:             tphit_    - header Structure for tphit
**:      OUT:
**: RETURNS:    STAF Condition Value
**:>------------------------------------------------------------------*/

long i, count=0;

for (i=0; i < tphit_h[0].nok; i++)
    {  
       if (tphit[i].flag<0||tphit[i].flag==8||tphit[i].flag==9||tphit[i].flag==11 )  continue;
       if (count !=i)  tphit[count] = tphit[i];
       count++;
    }

   tphit_h[0].nok = count;

   return STAFCV_OK;
}





