#ifndef PHYS_CONSTANTS
#define PHYS_CONSTANTS
#if 0
/* $Id: phys_constants.h,v 1.5 1999/07/26 14:43:54 fisyak Exp $ 
 * $Log: phys_constants.h,v $
 * Revision 1.5  1999/07/26 14:43:54  fisyak
 * gcc complains
 *
 * Revision 1.4  1998/11/30 16:06:10  fisyak
 * Protection versus usage in FORTRAN
 *
 * Revision 1.3  1998/03/27 14:18:45  fisyak
 * +/- => POS/MIN
 *
 * Revision 1.2  1998/03/05 00:16:07  fisyak
 * Missing C_D_CURVATURE
 *
 * Revision 1.1  1998/02/19 15:56:21  fisyak
 * Math. Phys. constants from H.Ward
 * */
#endif

#define C_C_LIGHT           2.99792458e10   /* speed light cm/s */
#define C_ELECT_CHARGE_MAG  1.60217733e-19  /* electron charge C */
#define C_AMU               0.93149432      /* unified amu (u) GeV/c^2 */

#define C_ALPHA              1.0/137.035989561 /* Fine structure const */
#define C_AVOGAD             6.0221367E23    /* Avogadro's number */
#define C_PLABRC             1.97327053E-14  /* hbar-c (GeV-cm) */
#define C_RCLSEL             2.8179409183694872E-13 /* e- radius (cm) */
#define C_DGEV               1.53538E-4      /* E-loss const (GeV/(g/cm^2)) */
#define C_ELCCGS             4.8032068E-10   /* Electron charge (CGS) */
#define C_K_BOLTZMAN         8.617385E-14    /* Boltzman k (GeV/deg. K) */
#define C_G_GRAV             9.80665E2       /* Grav., sea level (cm/s^2) */
#define C_MOLAR_VOL          2.241410E4      /* STP Molar volume (cm^3/mol) */
#define C_D_CURVATURE        0.000299792458  /* (GeV/c)(1/kG)(1/cm) */

#define  M_GAMMA            0.               /* mass     in GeV */
#define  L_GAMMA            1.E+15           /* lifetime in sec */
#define  M_POSITRON         0.0005109990615  /* mass     in GeV */
#define  L_POSITRON         1.E+15           /* lifetime in sec */
#define  M_ELECTRON         0.0005109990615  /* mass     in GeV */
#define  L_ELECTRON         1.E+15           /* lifetime in sec */
#define  M_NEUTRINO         0.               /* mass     in GeV */
#define  L_NEUTRINO         1.E+15           /* lifetime in sec */
#define  M_MUON_PLUS        0.105658387      /* mass     in GeV */
#define  L_MUON_PLUS        2.19703E-06      /* lifetime in sec */
#define  M_MUON_MINUS       0.105658387      /* mass     in GeV */
#define  L_MUON_MINUS       2.19703E-06      /* lifetime in sec */
#define  M_PION_0           0.1349764        /* mass     in GeV */
#define  L_PION_0           0.84E-16         /* lifetime in sec */
#define  M_PION_PLUS        0.1395700        /* mass     in GeV */
#define  L_PION_PLUS        2.603E-08        /* lifetime in sec */
#define  M_PION_MINUS       0.1395700        /* mass     in GeV */
#define  L_PION_MINUS       2.603E-08        /* lifetime in sec */
#define  M_KAON_0_LONG      0.497672         /* mass     in GeV */
#define  L_KAON_0_LONG      5.17E-08         /* lifetime in sec */
#define  M_KAON_PLUS        0.493677         /* mass     in GeV */
#define  L_KAON_PLUS        1.237E-08        /* lifetime in sec */
#define  M_KAON_MINUS       0.493677         /* mass     in GeV */
#define  L_KAON_MINUS       1.237E-08        /* lifetime in sec */
#define  M_NEUTRON          0.93956563       /* mass     in GeV */
#define  L_NEUTRON          8.870E+02        /* lifetime in sec */
#define  M_PROTON           0.9382723128     /* mass     in GeV */
#define  L_PROTON           1.000000E+15     /* lifetime in sec */
#define  M_ANTIPROTON       0.9382723128     /* mass     in GeV */
#define  L_ANTIPROTON       1.000000E+15     /* lifetime in sec */
#define  M_KAON_0_SHORT     0.497672         /* mass     in GeV */
#define  L_KAON_0_SHORT     8.926E-11        /* lifetime in sec */
#define  M_ETA              0.54745          /* mass     in GeV */
#define  L_ETA              5.485E-19        /* lifetime in sec */
#define  M_LAMBDA           1.115684         /* mass     in GeV */
#define  L_LAMBDA           2.632E-10        /* lifetime in sec */
#define  M_SIGMA_PLUS       1.18937          /* mass     in GeV */
#define  L_SIGMA_PLUS       0.799E-10        /* lifetime in sec */
#define  M_SIGMA_0          1.19255          /* mass     in GeV */
#define  L_SIGMA_0          7.4E-20          /* lifetime in sec */
#define  M_SIGMA_MINUS      1.197436         /* mass     in GeV */
#define  L_SIGMA_MINUS      1.479E-10        /* lifetime in sec */
#define  M_XI_0             1.3149           /* mass     in GeV */
#define  L_XI_0             2.9E-10          /* lifetime in sec */
#define  M_XI_MINUS         1.32132          /* mass     in GeV */
#define  L_XI_MINUS         1.639E-10        /* lifetime in sec */
#define  M_OMEGA_MINUS      1.67245          /* mass     in GeV */
#define  L_OMEGA_MINUS      0.822E-10        /* lifetime in sec */
#define  M_ANTINEUTRON      0.93956563       /* mass     in GeV */
#define  L_ANTINEUTRON      8.870E+02        /* lifetime in sec */
#define  M_ANTILAMBDA       1.115684         /* mass     in GeV */
#define  L_ANTILAMBDA       2.632E-10        /* lifetime in sec */
#define  M_ANTISIGMA_MINUS  1.18937          /* mass     in GeV */
#define  L_ANTISIGMA_MINUS  0.799E-10        /* lifetime in sec */
#define  M_ANTISIGMA_0      1.19255          /* mass     in GeV */
#define  L_ANTISIGMA_0      7.4E-20          /* lifetime in sec */
#define  M_ANTISIGMA_PLUS   1.197436         /* mass     in GeV */
#define  L_ANTISIGMA_PLUS   1.479E-10        /* lifetime in sec */
#define  M_ANTIXI_0         1.3149           /* mass     in GeV */
#define  L_ANTIXI_0         2.9E-10          /* lifetime in sec */
#define  M_ANTIXI_PLUS      1.32132          /* mass     in GeV */
#define  L_ANTIXI_PLUS      1.639E-10        /* lifetime in sec */
#define  M_ANTIOMEGA_PLUS   1.67245          /* mass     in GeV */
#define  L_ANTIOMEGA_PLUS   0.822E-10        /* lifetime in sec */
#define  M_TAU_PLUS         1.7771           /* mass     in GeV */
#define  L_TAU_PLUS         3.040000E-13     /* lifetime in sec */
#define  M_TAU_MINUS        1.7771           /* mass     in GeV */
#define  L_TAU_MINUS        3.040000E-13     /* lifetime in sec */
#define  M_D_PLUS           1.8694           /* mass     in GeV */
#define  L_D_PLUS           1.062000E-12     /* lifetime in sec */
#define  M_D_MINUS          1.8694           /* mass     in GeV */
#define  L_D_MINUS          1.062000E-12     /* lifetime in sec */
#define  M_D_0              1.8646           /* mass     in GeV */
#define  L_D_0              4.280000E-13     /* lifetime in sec */
#define  M_ANTI_D_0         1.8646           /* mass     in GeV */
#define  L_ANTI_D_0         4.280000E-13     /* lifetime in sec */
#define  M_DS_POS           1.9685           /* mass     in GeV */
#define  L_DS_POS           4.360000E-13     /* lifetime in sec */
#define  M_DS_MIN           1.9685           /* mass     in GeV */
#define  L_DS_MIN           4.360000E-13     /* lifetime in sec */
#define  M_LAMBDA_C_PLUS    2.2851           /* mass     in GeV */
#define  L_LAMBDA_C_PLUS    1.790000E-13     /* lifetime in sec */
#define  M_W_PLUS           80.220           /* mass     in GeV */
#define  L_W_PLUS           3.160000E-25     /* lifetime in sec */
#define  M_W_MINUS          80.220           /* mass     in GeV */
#define  L_W_MINUS          2.640000E-25     /* lifetime in sec */
#define  M_Z_0              91.187           /* mass     in GeV */
#define  L_Z_0              7.740000E-26     /* lifetime in sec */
#define  M_DEUTERON         1.875613         /* mass     in GeV */
#define  L_DEUTERON         1.000000E+15     /* lifetime in sec */
#define  M_TRITON           2.80925          /* mass     in GeV */
#define  L_TRITON           1.000000E+15     /* lifetime in sec */
#define  M_ALPHA            3.727417         /* mass     in GeV */
#define  L_ALPHA            1.000000E+15     /* lifetime in sec */
#define  M_GEANTINO         0.               /* mass     in GeV */
#define  L_GEANTINO         1.000000E+15     /* lifetime in sec */
#define  M_HE3              2.80923          /* mass     in GeV */
#define  L_HE3              1.000000E+15     /* lifetime in sec */
#define  M_CERENKOV         0.               /* mass     in GeV */
#define  L_CERENKOV         1.000000E+15     /* lifetime in sec */
#define  M_LI6              5.60305          /* mass     in GeV */
#define  L_LI6              1000.            /* lifetime in sec */
#define  M_LI7              6.53536          /* mass     in GeV */
#define  L_LI7              1000.            /* lifetime in sec */
#define  M_BE7              6.53622          /* mass     in GeV */
#define  L_BE7              1000.            /* lifetime in sec */
#define  M_BE9              8.39479          /* mass     in GeV */
#define  L_BE9              1000.            /* lifetime in sec */
#define  M_B10              9.32699          /* mass     in GeV */
#define  L_B10              1000.            /* lifetime in sec */
#define  M_B11              10.25510         /* mass     in GeV */
#define  L_B11              1000.            /* lifetime in sec */
#define  M_C12              11.17793         /* mass     in GeV */
#define  L_C12              1000.            /* lifetime in sec */
#define  M_N14              13.0437          /* mass     in GeV */
#define  L_N14              1000.            /* lifetime in sec */
#define  M_O16              14.89917         /* mass     in GeV */
#define  L_O16              1000.            /* lifetime in sec */
#define  M_F19              17.69690         /* mass     in GeV */
#define  L_F19              1000.            /* lifetime in sec */
#define  M_NE20             18.62284         /* mass     in GeV */
#define  L_NE20             1000.            /* lifetime in sec */
#define  M_NA23             21.41483         /* mass     in GeV */
#define  L_NA23             1000.            /* lifetime in sec */
#define  M_MG24             22.34193         /* mass     in GeV */
#define  L_MG24             1000.            /* lifetime in sec */
#define  M_AL27             25.13314         /* mass     in GeV */
#define  L_AL27             1000.            /* lifetime in sec */
#define  M_SI28             26.06034         /* mass     in GeV */
#define  L_SI28             1000.            /* lifetime in sec */
#define  M_P31              28.85188         /* mass     in GeV */
#define  L_P31              1000.            /* lifetime in sec */
#define  M_S32              29.78180         /* mass     in GeV */
#define  L_S32              1000.            /* lifetime in sec */
#define  M_CL35             32.57328         /* mass     in GeV */
#define  L_CL35             1000.            /* lifetime in sec */
#define  M_AR36             33.50356         /* mass     in GeV */
#define  L_AR36             1000.            /* lifetime in sec */
#define  M_K39              36.29447         /* mass     in GeV */
#define  L_K39              1000.            /* lifetime in sec */
#define  M_CA40             37.22492         /* mass     in GeV */
#define  L_CA40             1000.            /* lifetime in sec */
#define  M_SC45             41.87617         /* mass     in GeV */
#define  L_SC45             1000.            /* lifetime in sec */
#define  M_TI48             44.66324         /* mass     in GeV */
#define  L_TI48             1000.            /* lifetime in sec */
#define  M_V51              47.45401         /* mass     in GeV */
#define  L_V51              1000.            /* lifetime in sec */
#define  M_CR52             48.38228         /* mass     in GeV */
#define  L_CR52             1000.            /* lifetime in sec */
#define  M_MN55             51.17447         /* mass     in GeV */
#define  L_MN55             1000.            /* lifetime in sec */
#define  M_FE56             52.10307         /* mass     in GeV */
#define  L_FE56             1000.            /* lifetime in sec */
#define  M_CO59             54.89593         /* mass     in GeV */
#define  L_CO59             1000.            /* lifetime in sec */
#define  M_NI58             53.96644         /* mass     in GeV */
#define  L_NI58             1000.            /* lifetime in sec */
#define  M_CU63             58.61856         /* mass     in GeV */
#define  L_CU63             1000.            /* lifetime in sec */
#define  M_ZN64             59.54963         /* mass     in GeV */
#define  L_ZN64             1000.            /* lifetime in sec */
#define  M_GE74             68.85715         /* mass     in GeV */
#define  L_GE74             1000.            /* lifetime in sec */
#define  M_SE80             74.44178         /* mass     in GeV */
#define  L_SE80             1000.            /* lifetime in sec */
#define  M_KR84             78.16309         /* mass     in GeV */
#define  L_KR84             1000.            /* lifetime in sec */
#define  M_SR88             81.88358         /* mass     in GeV */
#define  L_SR88             1000.            /* lifetime in sec */
#define  M_ZR90             83.74571         /* mass     in GeV */
#define  L_ZR90             1000.            /* lifetime in sec */
#define  M_MO98             91.19832         /* mass     in GeV */
#define  L_MO98             1000.            /* lifetime in sec */
#define  M_PD106            98.64997         /* mass     in GeV */
#define  L_PD106            1000.            /* lifetime in sec */
#define  M_CD114            106.10997        /* mass     in GeV */
#define  L_CD114            1000.            /* lifetime in sec */
#define  M_SN120            111.68821        /* mass     in GeV */
#define  L_SN120            1000.            /* lifetime in sec */
#define  M_XE132            122.86796        /* mass     in GeV */
#define  L_XE132            1000.            /* lifetime in sec */
#define  M_BA138            128.45793        /* mass     in GeV */
#define  L_BA138            1000.            /* lifetime in sec */
#define  M_CE140            130.32111        /* mass     in GeV */
#define  L_CE140            1000.            /* lifetime in sec */
#define  M_SM152            141.51236        /* mass     in GeV */
#define  L_SM152            1000.            /* lifetime in sec */
#define  M_DY164            152.69909        /* mass     in GeV */
#define  L_DY164            1000.            /* lifetime in sec */
#define  M_YB174            162.02245        /* mass     in GeV */
#define  L_YB174            1000.            /* lifetime in sec */
#define  M_W184             171.34924        /* mass     in GeV */
#define  L_W184             1000.            /* lifetime in sec */
#define  M_PT194            180.67513        /* mass     in GeV */
#define  L_PT194            1000.            /* lifetime in sec */
#define  M_AU197            183.47324        /* mass     in GeV */
#define  L_AU197            1000.            /* lifetime in sec */
#define  M_HG202            188.13451        /* mass     in GeV */
#define  L_HG202            1000.            /* lifetime in sec */
#define  M_PB208            193.72907        /* mass     in GeV */
#define  L_PB208            1000.            /* lifetime in sec */
#define  M_U238             221.74295        /* mass     in GeV */
#define  L_U238             1000.            /* lifetime in sec */

#endif
