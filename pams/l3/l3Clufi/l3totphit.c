/*:>--------------------------------------------------------------------
**: FILE:       l3totphit.c.template
**: HISTORY:
**:             00jan93-v000a-hpl- Created by stic Version
**:  Id: idl.y,v 1.19 1999/10/01 19:04:27 ward Exp  
**:<------------------------------------------------------------------*/
#include "l3totphit.h"
#include "sl3CoordinateTransform.h"

long type_of_call l3totphit_(
  TABLE_HEAD_ST       *hitarray_h,       HITARRAY_ST         *hitarray ,
  TABLE_HEAD_ST          *tpHit_h,      TCL_TPHIT_ST            *tpHit )
{
/*:>--------------------------------------------------------------------
**: ROUTINE:    l3totphit_
**: DESCRIPTION: Physics Analysis Module ANSI C template.
**:             This is an ANSI C Physics Analysis Module template
**:             automatically generated by stic from l3totphit.idl.
**:             Please edit comments and code.
**: AUTHOR:     hpl - H.P. Lovecraft, hplovecraft@cthulhu.void
**: ARGUMENTS:
**:       IN:
**:           hitarray    - PLEASE FILL IN DESCRIPTION HERE
**:          hitarray_h   - header Structure for hitarray
**:    INOUT:
**:      OUT:
**:              tpHit    - PLEASE FILL IN DESCRIPTION HERE
**:             tpHit_h   - header Structure for tpHit
**: RETURNS:    STAF Condition Value
**:>------------------------------------------------------------------*/

    int index;
    int sector,supersector;
    int rbindex,rboffset;
    int mzindex,mzoffset,mzlength;
    double pad,time;
    double xyz[3];
    long* bank;
    int hitcounter = 0;
    int cluspersec = 0;
    
    // find start value in tphit array
    for( index = 0 ; index < 50000 ; index ++ )
	{
	  if ( tpHit[index].x == 0 && tpHit[index].y == 0 && tpHit[index].z == 0 )
	      {
		  hitcounter = index;
		  break;
	      }
	}

    
    // get supersector
    bank = hitarray;
    supersector = (int) (bank[3]+1)/2;
    //printf("l3totphit :  now converting supersector :%d \n",(bank[3]+1)/2);
    //printf("\nsupsec : %d",(bank[3]+1)/2);
    // loop over receiverboards
    for(rbindex = 1 ;rbindex <=12;rbindex++)
	{
	    //printf("l3totphit :  now converting rb:%d   ",rbindex);
	    //printf("\nrb:%d   ",rbindex);
	    rboffset = bank[10+2*(rbindex-1)];
	    //printf("offset:%d   length:%d\n",rboffset,bank[11+2*(rbindex-1)]);
	    // loop over mezzanine
	    for(mzindex = 1 ; mzindex <= 3 ; mzindex++)
		{
		    //printf("\t mz:%d",mzindex);
		    mzoffset = bank[rboffset+10+2*(mzindex-1)];
		    mzlength = bank[rboffset+11+2*(mzindex-1)];
		    //printf("offset:%d   length:%d\n",mzoffset,mzlength);
		    // is mz empty ?
		    if ( mzlength == 11 ) 
			{
			    //    printf("\t\t 0 clusters on this mz \n");
			    //printf("\t 0 ");
			} 
		    else // mz is not empty !
			{ 
			    int nrows,row,clusternumb,rowindex,rowoffset;
			    // loop over rows
			    nrows = bank[rboffset+mzoffset+10];
			    rowoffset = 0;
			    for ( rowindex = 1 ; rowindex <= nrows ; rowindex++ )
				{ 
				    int clusindex;
				    // get row number
				    row = bank[rboffset+mzoffset+10+1+rowoffset];
				    // get cluster number  
				    clusternumb = bank[rboffset+mzoffset+10+2+rowoffset];
				    //printf("\t\t %d clusters on row : %d\n",clusternumb,row);
				    //printf("\t%d/%d",row,clusternumb);
				    // loop over clusters
				    for ( clusindex = 1 ; clusindex <= clusternumb ; clusindex++)
					{
					    double pad,time;
					    long padinfo;
					    long flaginfo;
					    short flag;
					    short charge;
					    double xyz[3];
					    struct dataword 
					    {
						ushort info1;
						ushort info2;
					    } *dword;

					    if (rbindex<=6) 
						{ 
						    sector=2*supersector-1; 
						}
					    else if ( rbindex>6 && rbindex <=12 ) 
						{ 
						    sector=2*supersector;
						};
					    // extract the pad & time center of gravity
					    padinfo = bank[rboffset+mzoffset+10+2+rowoffset+(2*clusindex-1)];
					    dword = &padinfo;
					    pad = (double)(dword->info1)/64;
                                            time = (double)(dword->info2)/64;
					    // etract flag & time
					    flaginfo = bank[rboffset+mzoffset+10+2+rowoffset+(2*clusindex-1)+1];
					    dword= &flaginfo;
					    flag = dword->info1;
					    charge = dword->info2;
					    // coordinate transformation
					    rawToGlobal(sector,row,pad,time,&xyz[0],&xyz[1],&xyz[2]);
					    //printf("sec:%d  row:%d  pad:%f time:%f  ",sector,row,(float)pad,(float)time);
					    //printf("x:%f  y:%f  z:%f\n",(float)xyz[0],(float)xyz[1],(float)xyz[2]);
					    // filling
					    tpHit[hitcounter].x =  (float) xyz[0];
					    tpHit[hitcounter].y =  (float) xyz[1];
					    tpHit[hitcounter].z =  (float) xyz[2];
					    // couting
					    hitcounter++;
					    cluspersec++;
					}


				    // set offset for next row
				    rowoffset += ((2 * clusternumb) + 2); 
				}
			}
		    //printf("\t\t # clusters on this mz : %d\n",clusternumb);
		}

	}
    printf("Clusters in supsector %d: %d  \n",supersector,cluspersec);


    /*for (index =0;index<34;index++)
	{
	    printf("%d\t",bank[index]); 
	};
    sector = 1;
    row = 1;
    pad =1;
    time =1;
   
    rawToGlobal(sector,row,pad,time,&xyz[0],&xyz[1],&xyz[2]);
    printf("\nx: %f  y:%f  z:%f\n",xyz[0],xyz[1],xyz[2]);
    */


   return STAFCV_OK;
}




