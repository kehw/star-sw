//StiHit.cxx
//M.L. Miller (Yale Software)
//04/01

#include <iostream.h>
#include "StiHit.h"

StiHit::StiHit()
{
    msthit=0;
}

StiHit::~StiHit()
{
}

StiHit::StiHit(const StiHit& rhs)
{
    if (*this==rhs) return;
    copyToThis(rhs);
    return;
}

ostream& operator<<(ostream& os, const StiHit& hit)
{
    
    //return os <<hit.refangle()<<"\t"<<hit.position()<<"\t"
    //      <<hit.x()<<"\t"<<hit.y()<<"\t"<<hit.z()
    //      <<hit.sxx()<<"\t"<<hit.syy()<<"\t"<<hit.szz()<<"\t"
    //      <<hit.sxy()<<"\t"<<hit.sxz()<<"\t"<<hit.syz();

    return os <<hit.refangle()<<"\t"<<hit.position()<<"\t"
	      <<hit.x()<<"\t"<<hit.y()<<"\t"<<hit.z();
}
