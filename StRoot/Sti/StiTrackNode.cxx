#include <stdio.h>
#include <stdlib.h>
#include "StiTrackNode.h"
#include "TCL.h"
int StiTrackNode::mgFlag=0;
  static const int idx66[6][6] =
  {{ 0, 1, 3, 6,10,15},{ 1, 2, 4, 7,11,16},{ 3, 4, 5, 8,12,17}
  ,{ 6, 7, 8, 9,13,18},{10,11,12,13,14,19},{15,16,17,18,19,20}};
static const double MAX1ERR[]={3,3,3,0.1,3.,0.1};
static const double MAX2ERR[]={MAX1ERR[0]*MAX1ERR[0]
                              ,MAX1ERR[1]*MAX1ERR[1]
                              ,MAX1ERR[2]*MAX1ERR[2]
                              ,MAX1ERR[3]*MAX1ERR[3]
                              ,MAX1ERR[4]*MAX1ERR[4]
                              ,MAX1ERR[5]*MAX1ERR[5]};

static const double MIN1ERR[]={1e-4,1e-4,1e-4,1e-6,1e-6,1e-6};
static const double MIN2ERR[]={MIN1ERR[0]*MIN1ERR[0]
                              ,MIN1ERR[1]*MIN1ERR[1]
                              ,MIN1ERR[2]*MIN1ERR[2]
                              ,MIN1ERR[3]*MIN1ERR[3]
                              ,MIN1ERR[4]*MIN1ERR[4]
                              ,MIN1ERR[5]*MIN1ERR[5]};
static const double recvCORRMAX  = 0.99;
static const double chekCORRMAX  = 0.9999;
static double MAXPARS[]={250,250,250,1.5,100,100};

//______________________________________________________________________________
void StiTrackNode::errPropag6( double G[21],const double F[6][6],int nF )
{
  enum {NP=6,NE=21};

  double g[NE];      memcpy(g,    G,sizeof( g));
  double fg[NP][NP]; memset(fg[0],0,sizeof(fg));
//   double myF[6][6];  memcpy(myF[0],F[0],sizeof(fg));
//   for (int i=0;i<NP;i++) {myF[i][i]+=1.;}
//   double myG[NE];
//   TCL::trasat(myF[0],G,myG,NP,NP);

  for (int i=0;i<nF;i++) {
  for (int j=0;j<nF;j++) {
    if (!F[i][j]) 	continue;
    for (int k=0;k<NP;k++) {
      int jk = idx66[j][k];
      if (!g[jk])	continue;
      fg[i][k] += F[i][j]*g[jk]; 
  }}}

  for (int i=0;i<NP;i++) {        
  for (int k=i;k<NP;k++) {        
    int ik = idx66[i][k];
    double s = 0; 
    for (int j=0;j<NP;j++) {
      if (!F[k][j])	continue;
      s += fg[i][j]*F[k][j];
    }
    G[ik] += (s + fg[i][k] + fg[k][i]);
  }}
//  for (int i=0;i<NE;i++) {assert(fabs(G[i]-myG[i]) <= 1e-6*fabs(G[i]+myG[i]));}


}       

//______________________________________________________________________________
void StiHitContino::reset()
{
   memset(this,0,sizeof(StiHitContino));
   mChi2[0]=1e61;
}
//______________________________________________________________________________
void StiHitContino::add(StiHit *hit,double chi2,double detr)
{
   int i=0;
   for (;i<kMaxSize;i++) {
     if (!mHits[i]) 		break;
     if (chi2 > mChi2[i]) 	continue;
     for (int jr = kMaxSize-2;jr>=i;jr--) 
       {mHits[jr+1]=mHits[jr]; mChi2[jr+1]=mChi2[jr];mDetr[jr+1]=mDetr[jr];}
     				break;
   }
   if (i>=kMaxSize) 		return;
   mHits[i]=hit; mChi2[i]=chi2;mDetr[i]=detr;
}
//______________________________________________________________________________
void StiHitContino::print(const char* tit) const
{
  if (!tit || !*tit) tit ="print()";
  int n=getNHits();	
  printf(" ****	StiHitContino::%s nHits=%d\n",tit,n);
  for (int i=0;i<n;i++) {
    printf("%3d - hit=%p chi2 = %g\n",i,(void*)mHits[i],mChi2[i]);}
  printf("\n");
}
//______________________________________________________________________________
int StiHitContino::getNHits() const
{ int n=0; for(int i=0;i<kMaxSize;i++) {if (mHits[i]) n++;}; return n;}	
//______________________________________________________________________________

#include <complex>
typedef std::complex<double > Complex;
const Complex Im(0,1);
//______________________________________________________________________________
int StiTrackNode::cylCross(double r, const double dx[4],double Rho,double out[4])
{
//  dx[0] == cosCA; dx[1] == sinCA;dx[2] == _x; dx[3]==_y

  Complex d(dx[0],dx[1]),n(-dx[1],dx[0]),x(dx[2],dx[3]);
  Complex res[2];
  Complex xd = x/d;
  double R2 = x.real()*x.real()+x.imag()*x.imag();

  if (R2*Rho*Rho< 1e-4) {// Low curvature approx
    double a = (1.+xd.imag()*Rho);
    double b = xd.real();
    double c = (R2-r*r);
    double dis = b*b - a*c;
    if (dis<0.) 		return 1;
    dis = sqrt(dis);
    double L[2];
    if (b<0) {a=-a;b=-b;c=-c;}
    L[0] = -c/(dis+b);
    L[1] = -(b+(dis))/a;
    res[0] = x+L[0]*(1.+Im*(0.5*L[0]*Rho))*d;
    res[1] = x+L[1]*(1.+Im*(0.5*L[1]*Rho))*d;

  } else {	//General case

    Complex Q = Rho*x+n;
    double a1Q = ((x.real()*n.real()+x.imag()*n.imag())*2 + R2*Rho);
    double aQ = a1Q*Rho+1;
    double q = std::log(Q).imag();
 //       cos(Al-q) = ((Rho*r)**2 +Q**2-1)/(2*Q*r*Rho)
    double mycos = (r*r*Rho+a1Q)/(2*aQ*r);
    if (fabs(mycos)>1) 		return 1;
    double ang = acos(mycos);
    res[0] = r*std::exp(Im*(q+ang));
    res[1] = r*std::exp(Im*(q-ang));
  }
  if (res[0].real() < res[1].real()) {//swap
    xd = res[0]; res[0]=res[1]; res[1]=xd;}
  memcpy(out,res,sizeof(res));
  return 0;
}

//______________________________________________________________________________
 
double StiTrackNode::sinX(double x)
{
  double x2 = x*x;
  if (x2>0.5) return (sin(x)-x)/x2/x;
  double nom = -1./6;
  double sum = nom;
  for (int it=4;1;it+=2) {
    nom = -nom*x2/(it*(it+1));
    sum +=nom;
    if (fabs(nom) <= 1e-10*fabs(sum)) break;
  }
  return sum;
} 
//______________________________________________________________________________
void StiTrackNode::mult6(double Rot[kNPars][kNPars],const double Pro[kNPars][kNPars]) 
{
  double T[kNPars][kNPars];

  if (!Rot[0][0]) {memcpy(Rot[0],Pro[0],sizeof(T)); return;}

  memcpy(T[0],Pro[0],sizeof(T));

  for (int i=0;i<kNPars;i++) {
  for (int j=0;j<kNPars;j++) {
    if(!Rot[i][j]) continue;
    for (int k=0;k<kNPars;k++) {
      if (!Pro[k][i]) continue;
      T[k][j] += Pro[k][i]*Rot[i][j];
  }}} 
  for (int i=0;i<kNPars;i++) {
  for (int k=0;k<kNPars;k++) {
    Rot[i][k] += T[i][k];
}}
}     
//______________________________________________________________________________
double StiTrackNode::getRefPosition() const
{
  if(!_detector) {
    assert(_hit);
    return _hit->x();
  }
  StiPlacement * place = _detector->getPlacement();
  assert(place);
  return place->getLayerRadius();
}
//______________________________________________________________________________
  double StiTrackNode::getLayerAngle()  const
{
  assert(_detector);
  StiPlacement * place = _detector->getPlacement();
  assert(place);
  return place->getLayerAngle();
}

//______________________________________________________________________________
double StiNodeErrs::operator()(int i,int j) const
{
  return A[idx66[i][j]];
}
//______________________________________________________________________________
StiNodeErrs &StiNodeErrs::merge(double wt,StiNodeErrs &other)
{
   double wt0 = 1.-wt;
   for (int i=0;i<kNErrs;i++) {A[i] = wt0*A[i] + wt*other.A[i];}
   return *this;
}
//______________________________________________________________________________
void StiNodeErrs::get00(      double *a) const
{
   memcpy(a,A,6*sizeof(double));
}
//______________________________________________________________________________
void StiNodeErrs::set00(const double *a) 
{
   memcpy(A,a,6*sizeof(double));
}
//______________________________________________________________________________
void StiNodeErrs::get10(double *a) const
{
// 0: 00
// 1: 10 11 
// 3: 20 21 22
// 6: 30 31 32 33
//10: 40 41 42 43 44
//15: 50 51 52 53 54 55
  memcpy(a+0,A+ 6,3*sizeof(double));
  memcpy(a+3,A+10,3*sizeof(double));
  memcpy(a+6,A+15,3*sizeof(double));
}
//______________________________________________________________________________
void StiNodeErrs::set10(const double *a) 
{
  memcpy(A+ 6,a+0,3*sizeof(double));
  memcpy(A+10,a+3,3*sizeof(double));
  memcpy(A+15,a+6,3*sizeof(double));
}
//______________________________________________________________________________
void StiNodeErrs::get11(      double *a) const
{
  memcpy(a+0,A+ 9,1*sizeof(double));
  memcpy(a+1,A+13,2*sizeof(double));
  memcpy(a+3,A+18,3*sizeof(double));
}
//______________________________________________________________________________
void StiNodeErrs::set11(const double *a) 
{
  memcpy(A+ 9,a+0,1*sizeof(double));
  memcpy(A+13,a+1,2*sizeof(double));
  memcpy(A+18,a+3,3*sizeof(double));
}
//______________________________________________________________________________
void StiNodeErrs::zeroX() 
{ 
  for (int i=0;i<kNPars;i++) {A[idx66[i][0]]=0;}
}
//______________________________________________________________________________
void StiNodeErrs::recov() 
{
static int PRINT_IT=0;

  int i0=0; if (!_cXX) i0 = 1;
  for (int i=i0;i<kNPars;i++) {
    double maxDia = MAX2ERR[i];
    double minDia = MIN2ERR[i];
    int ld = idx66[i][i];
    if (A[ld]<minDia) {
if(PRINT_IT){
      printf("StiNodeErrs::cut. Negative diagonal %g(%d)\n",A[ld],i);
      print();
}
       A[ld] = minDia;
    }
    if (A[ld]>maxDia) {
if(PRINT_IT) {
      printf("StiNodeErrs::cut. Too big  diagonal %g(%d)\n",A[ld],i);
      print();
}
       for (int j=0;j<kNPars;j++){ A[idx66[i][j]]=0;}
       A[ld]=maxDia;
    }
  }
  for (int i=i0;i<kNPars;i++) {
    double &aii = A[idx66[i][i]];
    for (int j=i+1;j<kNPars;j++) {
      double &ajj = A[idx66[j][j]];
      double &aij = A[idx66[i][j]];
      if (aij*aij <= aii*ajj*recvCORRMAX) continue;
      if (aij*aij > aii*ajj){
        printf("StiNodeErrs::recov : Correlation too big %g[%d][%d]>%g\n"
              ,aij,i,j,sqrt(aii*ajj));}	  
      double ab = sqrt(aii*ajj);
      double t2 = (fabs(aij)/ab-recvCORRMAX)/(1+recvCORRMAX);
      aii += aii*t2; ajj += ajj*t2;
      if (aij<0) t2 = -t2; aij -= ab*t2;
      
    }//end j
  }//end i

}
//______________________________________________________________________________
void StiNodeErrs::print() const
{
   const double *d = A;
   for (int n=1;n<=6;n++) {
     printf("%d - ",n);
     for (int i=0;i<n;i++){printf("%g\t",*(d++));}; printf("\n");
   }  
}     

//______________________________________________________________________________
int StiNodeErrs::check(const char *pri) const
{
  int i,j,kase=0;
  double aii,ajj,aij;
  int i0=0; if (!_cXX) i0 = 1;
  for (i=i0;i<kNPars;i++) {
    aii = A[idx66[i][i]];
    if (aii<MIN2ERR[i]) {kase = 1; break;}
  }
  if (kase) goto RETN;
  for (i=i0;i<kNPars;i++) {
    aii = A[idx66[i][i]];
    for (j=i+1;j<kNPars ;j++) {
      ajj = A[idx66[j][j]];
      if (ajj<=0) continue;
      aij = A[idx66[i][j]];
      if ((aij*aij)> chekCORRMAX*aii*ajj) {kase = 2; break;}
    }
    if (kase) break;
  }  
RETN:
//  if (!kase && sign()<0 ) kase = 3;  //It is rather slow. For debug only
  if ((kase == 3) && (!pri || !pri[0])) pri = "QWERTY";

  if (!kase) 	return kase;
  if (!pri ) 	return kase;
  switch(kase) {
  
    case 1: printf("StiNodeErrs::check(%s) FAILED: Negative diagonal %g[%d][%d]\n",pri,aii,i,i);  
          break;
    case 2: printf("StiNodeErrs::check(%s) FAILED: Correlation too big %g[%d][%d]>%g\n"
                ,pri,aij,i,j,sqrt(aii*ajj)); 	  
          break;
    case 3: printf("StiNodeErrs::check(%s) FAILED: Non Positive matrix\n",pri);  
  }    
  return kase;
}  
//____________________________________________________________
double StiNodeErrs::sign() const
{
   enum {n=kNPars};
   double ans=3e33;
   const double *a = A;
   double *xx = (double *)A;
   double save = *xx; if (!save) *xx = 1;
   double B[kNErrs];
         double *b = B;
   // trchlu.F -- translated by f2c (version 19970219).
   //
   //see original documentation of CERNLIB package F112 

   /* Local variables */
   int ipiv, kpiv, i__, j;
   double r__, dc;
   int id, kd;
   double sum;


   /* CERN PROGLIB# F112    TRCHLU          .VERSION KERNFOR  4.16  870601 */
   /* ORIG. 18/12/74 W.HART */


   /* Parameter adjuTments */
   --b;    --a;

   /* Function Body */
   ipiv = 0;

   i__ = 0;

   do {
      ++i__;
      ipiv += i__;
      kpiv = ipiv;
      r__ = a[ipiv];

      for (j = i__; j <= n; ++j) {
         sum = 0.;
         if (i__ == 1)       goto L40;
         if (r__ == 0.)      goto L42;
         id = ipiv - i__ + 1;
         kd = kpiv - i__ + 1;

         do {
            sum += b[kd] * b[id];
            ++kd;   ++id;
         } while (id < ipiv);

L40:
         sum = a[kpiv] - sum;
L42:
         if (j != i__) b[kpiv] = sum * r__;
         else {
            if (sum<ans) ans = sum;
            if (sum<0.) goto RETN;
            dc = sqrt(sum);
            b[kpiv] = dc;
            if (r__ > 0.)  r__ = (double)1. / dc;
         }
         kpiv += j;
      }

   } while  (i__ < n);

RETN: *xx=save; 
   return ans;
} /* trchlu_ */


//______________________________________________________________________________
int StiNodePars::check(const char *pri) const
{

//?? temp test
  double tmp = _curv - _ptin* _hz;
  assert(fabs(tmp)< 1e-3*fabs(_curv));
//??
  int ierr=0;
  for (int i=0;i<kNPars;i++) {if (fabs(P[i]) > MAXPARS[i]) {ierr = i+1 ; break;}} 
  if(ierr) goto FAILED;
  for (int i=-2;i<0;i++)     {if (fabs(P[i]) > 1.)         {ierr = i+12; break;}} 
FAILED: 
  if (!ierr) return ierr;
  if (!pri ) return ierr;
  printf("StiNodePars::check(%s) == FAILED(%d)\n",pri,ierr);
  print();
  return ierr;
} 
//______________________________________________________________________________
StiNodePars &StiNodePars::merge(double wt,StiNodePars &other)
{
   double wt0 = 1.-wt;
   for (int i=0;i<kNPars+1;i++) {P[i] = wt0*P[i] + wt*other.P[i];}
   ready();
   return *this;
}
//______________________________________________________________________________
void StiNodePars::print() const
{
static const char* tit[]={"cosCA","sinCA","X","Y","Z","Eta","Ptin","TanL","Curv",0};
  for (int i=-2;i<kNPars+1;i++) {printf("%s = %g, ",tit[i+2],P[i]);}
  printf("\n");
}   
//______________________________________________________________________________
void StiHitErrs::rotate(double angle)
{
  double t[2][2];
  t[0][0] = cos(angle); t[0][1] = sin(angle);
  t[1][0] = -t[0][1];   t[1][1] = t[0][0];
  double r[3];
  TCL::trasat(t[0],&hXX,r,2,2);
  TCL::ucopy(r,&hXX,3);
}




