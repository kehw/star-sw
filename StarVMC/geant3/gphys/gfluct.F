*
* $Id: gfluct.F,v 1.1.1.2 2009/02/01 17:10:31 fisyak Exp $
*
* $Log: gfluct.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:31  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:48:36  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2002/07/24 15:56:25  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:41  hristov
* Separate distribution  of Geant3
*
* Revision 1.2  1999/06/03 16:12:17  fca
* Introduced M.Kowalski modifications for very short steps.
*
* Revision 1.1.1.1  1999/05/18 15:55:20  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:21:24  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*-- Author :
      SUBROUTINE G3FLUCT(DEMEAN,DE)
C.
C.    ******************************************************************
C.    *                                                                *
C.    *   Subroutine to decide which method is used to simulate        *
C.    *   the straggling around the mean energy loss.                  *
C.    *                                                                *
C.    *                                                                *
C.    *   DNMIN:  <---------->1<-------->30<--------->50<--------->    *
C.    *                                                                *
C.    *   LOSS=2  :                                                    *
C.    *   STRA=0  <----------G3LANDZ-------------------><--G3LANDO-->  *
C.    *   LOSS=1,3:                                                    *
C.    *   STRA=0  <---------------------G3LANDZ-------------------->   *
C.    *                                                                *
C.    *   STRA=1  <-----------PAI---------------------><--G3LANDZ-->   *
#if defined(CERNLIB_ASHO)
C.    *   STRA=2  <---PAI----><---ASHO---><----PAI----><--G3LANDZ-->   *
C.    *                                                                *
#endif
C.    *                                                                *
C.    *   DNMIN :  an estimation of the number of collisions           *
C.    *            with energy close to the ionization energy          *
C.    *            (see PHYS333)                                       *
C.    *                                                                *
C.    *   Input  : DEMEAN (mean energy loss)                           *
C.    *   Output : DE   (energy loss in the current step)              *
C.    *                                                                *
C.    *    ==>Called by : G3TELEC,G3TMUON,G3THADR                      *
C.    *                                                                *
C.    ******************************************************************
C.
#include "geant321/gcbank.inc"
#include "geant321/gcjloc.inc"
#include "geant321/gconsp.inc"
#include "geant321/gcmate.inc"
#include "geant321/gccuts.inc"
#include "geant321/gckine.inc"
#include "geant321/gcmulo.inc"
#include "geant321/gcphys.inc"
#include "geant321/gctrak.inc"
*
      PARAMETER (EULER=0.577215,GAM1=EULER-1)
      PARAMETER (P1=.60715,P2=.67794,P3=.52382E-1,P4=.94753,
     +           P5=.74442,P6=1.1934)
      PARAMETER (DGEV=0.153536 E-3, DNLIM=50)
#if defined(CERNLIB_ASHO)
      PARAMETER (ASHMIN=1,ASHMAX=30)
#endif
* These parameters are needed by M.Kowalski's fluctuation algorithm
      PARAMETER (FPOT=20.77E-9, EEND=10E-6, EEXPO=2.2, EEND1=1E-6)
      PARAMETER (XEXPO=-EEXPO+1, YEXPO=1/XEXPO)
* These parameters are needed by M.Kowalski's fluctuation algorithm
      DIMENSION RNDM(2)
      DE2(DPOT,RAN)=(DPOT**XEXPO*(1-RAN)+EEND**XEXPO*RAN)**YEXPO
      FLAND(X) = P1+P6*X+(P2+P3*X)*EXP(P4*X+P5)
*
      DE21(DPOT,RAN)=(DPOT**XEXPO*(1-RAN)+EEND1**XEXPO*RAN)**YEXPO
*
      IF(STEP.LE.0) THEN
         DE=DEMEAN
      ELSE
         DEDX = DEMEAN/STEP
         POTI=Q(JPROB+9)
         IF(ISTRA.EQ.0.AND.(ILOSS.EQ.1.OR.ILOSS.EQ.3)) THEN
            CALL G3LANDZ(Z,STEP,VECT(7),GETOT,DEDX,DE,POTI,Q(JPROB+10))
         ELSEIF (ILOSS.EQ.5) THEN
* This is Marek Kowalski's fluctuation algorithm, it works only when
* the step size has been limited to one ionisation on average
            CALL GRNDM(RNDM,1)
            DE=DE2(FPOT,RNDM(1))
*
         ELSEIF (ILOSS.EQ.6) THEN
* This is Marek Kowalski's fluctuation algorithm, it works only when
* the step size has been limited to one ionisation on average and for Kr only
            CALL GRNDM(RNDM,1)
            DE=DE21(FPOT,RNDM(1))
         ELSE
*
* *** mean ionization potential (GeV)
*        POTI=16E-9*Z**0.9
*
            GAMMA = GETOT/AMASS
            BETA = VECT(7)/GETOT
            BET2 = BETA**2
*
* ***    low energy transfer
            XI = DGEV*CHARGE**2*STEP*DENS*Z/(A*BET2)
*
* ***    regime
* ***    ISTRA = 1 --> PAI + URBAN
#if defined(CERNLIB_ASHO)
* ***    ISTRA = 2 --> PAI + URBAN + ASHO
#endif
            DNMIN = MIN(XI,DEMEAN)/POTI
*
            IF (ISTRA.EQ.0) THEN
               IF(DNMIN.GE.DNLIM) THEN
*
*  Energy straggling using Gaussian, Landau & Vavilov theories.
*
*  STEP   =  current step-length (cm)
*
*  DELAND =  DE/DX - <DE/DX>     (GeV)
*
*  Author      : G.N. Patrick
*
                  IF(STEP.LT.1.E-7)THEN
                     DELAND=0.
                  ELSE
*
*     Maximum energy transfer to atomic electron (GeV).
                     ETA = BETA*GAMMA
                     RATIO = EMASS/AMASS
*
*     Calculate Kappa significance ratio.
*                 EMAX=(2*EMASS*ETA**2)/(1+2*RATIO*GAMMA+RATIO**2)
*                 CAPPA = XI/EMAX
                     CAPPA = XI*(1+2*RATIO*GAMMA+RATIO**2)/(2*EMASS*
     +               ETA**2)
                     IF (CAPPA.GE.10.) THEN
*
*     +-----------------------------------+
*     I Sample from Gaussian distribution I
*     +-----------------------------------+
                        SIGMA = XI*SQRT((1.-0.5*BET2)/CAPPA)
                        CALL GRNDM(RNDM,2)
                        F1 = -2.*LOG(RNDM(1))
                        DELAND = SIGMA*SQRT(F1)*COS(TWOPI*RNDM(2))
                     ELSE
                        XMEAN = -BET2-LOG(CAPPA)+GAM1
                        IF (CAPPA.LT.0.01) THEN
                           XLAMX = FLAND(XMEAN)
*     +---------------------------------------------------------------+
*     I Sample lambda variable from Kolbig/Schorr Landau distribution I
*     +---------------------------------------------------------------+
*  10                   CALL GRNDM(RNDM,1)
*                       IF( RNDM(1) .GT. 0.980 ) GO TO 10
*                       XLAMB = G3LANDR(RNDM(1))
*     +---------------------------------------------------------------+
*     I Sample lambda variable from James/Hancock Landau distribution I
*     +---------------------------------------------------------------+
   10                      CALL G3LANDG(XLAMB)
                           IF(XLAMB.GT.XLAMX) GO TO 10
                        ELSE
*            +---------------------------------------------------------+
*            I Sample lambda variable (Landau not Vavilov) from        I
*            I Rotondi&Montagna&Kolbig Vavilov distribution            I
*            +---------------------------------------------------------+
                           CALL GRNDM(RNDM,1)
                           XLAMB = G3VAVIV(CAPPA,BET2,RNDM(1))
                        ENDIF
*
*     Calculate DE/DX - <DE/DX>
                        DELAND = XI*(XLAMB-XMEAN)
                     ENDIF
                  ENDIF
                  DE = DEMEAN + DELAND
               ELSE
                  CALL G3LANDZ(Z,STEP,VECT(7),GETOT,DEDX,DE,POTI,
     +            Q(JPROB+ 10))
               ENDIF
            ELSE IF (ISTRA.LE.2) THEN
               IF(DNMIN.GE.DNLIM) THEN
                  CALL G3LANDZ(Z,STEP,VECT(7),GETOT,DEDX,DE,POTI,
     +            Q(JPROB+ 10))
               ELSE
                  NMEC = NMEC+1
                  LMEC(NMEC)=109
#if defined(CERNLIB_ASHO)
                  IF (DNMIN.GE.ASHMIN.AND.DNMIN.LT.ASHMAX .AND.ISTRA.EQ
     +            .2) THEN
                     CALL G3ASHO(VECT(7),AMASS,STEP,DE)
                  ELSE
                     DE = G3STREN(GAMMA,DCUTE,STEP)
                  ENDIF
#endif
#if !defined(CERNLIB_ASHO)
                  DE = G3STREN(GAMMA,DCUTE,STEP)
#endif
*
* ***   Add brem losses to ionisation
                  IF(ITRTYP.EQ.2) THEN
                     JBASE = LQ(JMA-1)+2*NEK1+IEKBIN
                     DE = DE +(1.-GEKRAT)*Q(JBASE)+GEKRAT*Q(JBASE+1)
                  ELSEIF(ITRTYP.EQ.5) THEN
                     JBASE = LQ(JMA-2)+NEK1+IEKBIN
                     DE = DE +(1.-GEKRAT)*Q(JBASE)+GEKRAT*Q(JBASE+1)
                  ENDIF
               ENDIF
            ENDIF
         ENDIF
      ENDIF
*
      END
