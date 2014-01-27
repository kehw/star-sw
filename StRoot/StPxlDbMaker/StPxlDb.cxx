/*!
 * \class StPxlDb
 * \author Qiu Hao, Jan 2014
 * \Initial Revision.
 */
/***************************************************************************
 *
 * $Id: StPxlDb.cxx,v 1.1 2014/01/27 02:38:33 qiuh Exp $
 *
 * Author: Qiu Hao, Jan 2014
 ***************************************************************************
 *
 * Description:
 * DB information on pxl geometry and sensor/row/column status
 * More information at
 * https://www.star.bnl.gov/protected/heavy/qiuh/HFT/software/PXL_software.pdf
 *
 ***************************************************************************
 *
 * $Log: StPxlDb.cxx,v $
 * Revision 1.1  2014/01/27 02:38:33  qiuh
 * *** empty log message ***
 *
 *
 **************************************************************************/

/*
  relation within STAR frame
  GlobalXyz = TpcOnGlobal * IdsOnTpc * PxlOnIds * HalfOnPxl * SectorOnHalf * LadderOnSector * SensorOnLadder * SensorLocalXyz

  numbering :
  Id  = (sector-1)*40 + (ladder-1)*10 + sensor
  1<= sector <= 10
  1<= ladder <= 4
  1<= sensor <= 10
*/

#include "StPxlDb.h"
#include "StPxlUtil/StPxlConstants.h"
#include "StPxlUtil/StThinPlateSpline.h"
#include "StMessMgr.h"
#include "tables/St_Survey_Table.h"
#include "StTpcDb/StTpcDb.h"
#include "tables/St_pxlSensorStatus_Table.h"
#include "tables/St_pxlRowColumnStatus_Table.h"
#include "tables/St_pxlSensorTps_Table.h"
#include "tables/St_pxlControl_Table.h"

ClassImp(StPxlDb)
//_____________________________________________________________________________
StPxlDb::StPxlDb() :
   StObject()
{
   mGeoHMatrixTpcOnGlobal = 0;
   mSensorStatusTable = 0;
   mRowColumnStatusTable = 0;
   mPxlControl = 0;
   memset(mThinPlateSpline, 0, sizeof(mThinPlateSpline));
}
//_____________________________________________________________________________
void StPxlDb::setGeoHMatrices(Survey_st **tables)
{
   if (gStTpcDb)
      mGeoHMatrixTpcOnGlobal = (TGeoHMatrix *)&gStTpcDb->Tpc2GlobalMatrix();
   else {
      if (mGeoHMatrixTpcOnGlobal) delete mGeoHMatrixTpcOnGlobal;
      mGeoHMatrixTpcOnGlobal = new TGeoHMatrix("tpcOnGlobal");
      LOG_WARN << "No gStTpcDb, use null transformation for tpc on global" << endm;
   }

   Survey_st *IdsOnTpc          = tables[0];
   Survey_st *PstOnIds          = tables[1];
   Survey_st *PxlOnPst          = tables[2];
   Survey_st *HalfOnPxl         = tables[3];
   Survey_st *SectorsOnHalf     = tables[4];
   Survey_st *LaddersOnSectors  = tables[5];
   Survey_st *SensorsOnLadders  = tables[6];

   mGeoHMatrixIdsOnTpc.SetName("idsOnTpc");
   mGeoHMatrixIdsOnTpc.SetRotation(&IdsOnTpc->r00);
   mGeoHMatrixIdsOnTpc.SetTranslation(&IdsOnTpc->t0);

   mGeoHMatrixPstOnIds.SetName("pstOnIds");
   mGeoHMatrixPstOnIds.SetRotation(&PstOnIds->r00);
   mGeoHMatrixPstOnIds.SetTranslation(&PstOnIds->t0);

   mGeoHMatrixPxlOnPst.SetName("pxlOnPst");
   mGeoHMatrixPxlOnPst.SetRotation(&PxlOnPst->r00);
   mGeoHMatrixPxlOnPst.SetTranslation(&PxlOnPst->t0);

   mGeoHMatrixTpcOnGlobal->Print();
   mGeoHMatrixIdsOnTpc.Print();
   mGeoHMatrixPstOnIds.Print();
   mGeoHMatrixPxlOnPst.Print();

   LOG_INFO << "geoHMatrix pxl half on pxl: " << endm;
   for (Int_t t = 0; t < 2; t++, HalfOnPxl++) {
      mGeoHMatrixHalfOnPxl[HalfOnPxl->Id - 1].SetName(Form("halfOnPxl%03i", HalfOnPxl->Id));
      mGeoHMatrixHalfOnPxl[HalfOnPxl->Id - 1].SetRotation(&HalfOnPxl->r00);
      mGeoHMatrixHalfOnPxl[HalfOnPxl->Id - 1].SetTranslation(&HalfOnPxl->t0);
      mGeoHMatrixHalfOnPxl[HalfOnPxl->Id - 1].Print();
   }

   LOG_INFO << "geoHMatrix pxl sector on half: " << endm;
   for (int i = 0; i < kNumberOfPxlSectors; i++) {
      mGeoHMatrixSectorOnHalf[SectorsOnHalf->Id - 1].SetName(Form("sectorOnHalf%03i", SectorsOnHalf->Id));
      mGeoHMatrixSectorOnHalf[SectorsOnHalf->Id - 1].SetRotation(&SectorsOnHalf->r00);
      mGeoHMatrixSectorOnHalf[SectorsOnHalf->Id - 1].SetTranslation(&SectorsOnHalf->t0);
      mGeoHMatrixSectorOnHalf[SectorsOnHalf->Id - 1].Print();
      SectorsOnHalf++;
   }

   LOG_INFO << "geoHMatrix pxl ladder on sector: " << endm;
   for (int i = 0; i < kNumberOfPxlSectors; i++)
      for (int j = 0; j < kNumberOfPxlLaddersPerSector; j++) {
         int iSector = (LaddersOnSectors->Id - 1) / kNumberOfPxlLaddersPerSector;
         int iLadder = (LaddersOnSectors->Id - 1) % kNumberOfPxlLaddersPerSector;
         mGeoHMatrixLadderOnSector[iSector][iLadder].SetName(Form("ladderOnSector%03i%03i", iSector + 1, iLadder + 1));
         mGeoHMatrixLadderOnSector[iSector][iLadder].SetRotation(&LaddersOnSectors->r00);
         mGeoHMatrixLadderOnSector[iSector][iLadder].SetTranslation(&LaddersOnSectors->t0);
         mGeoHMatrixLadderOnSector[iSector][iLadder].Print();
         LaddersOnSectors++;
      }

   LOG_INFO << "geoHMatrix pxl sensor on ladder: " << endm;
   for (int i = 0; i < kNumberOfPxlSectors; i++)
      for (int j = 0; j < kNumberOfPxlLaddersPerSector; j++)
         for (int k = 0; k < kNumberOfPxlSensorsPerLadder; k++) {
            int iSector = (SensorsOnLadders->Id - 1) / kNumberOfPxlLaddersPerSector / kNumberOfPxlSensorsPerLadder;
            int iLadder = ((SensorsOnLadders->Id - 1) / kNumberOfPxlSensorsPerLadder) % kNumberOfPxlLaddersPerSector;
            int iSensor = (SensorsOnLadders->Id - 1) % kNumberOfPxlSensorsPerLadder;
            mGeoHMatrixSensorOnLadder[i][j][k].SetName(Form("sensorOnLadder%03i%03i%03i", iSector + 1, iLadder + 1, iSensor + 1));
            mGeoHMatrixSensorOnLadder[iSector][iLadder][iSensor].SetRotation(&SensorsOnLadders->r00);
            mGeoHMatrixSensorOnLadder[iSector][iLadder][iSensor].SetTranslation(&SensorsOnLadders->t0);
            mGeoHMatrixSensorOnLadder[iSector][iLadder][iSensor].Print();
            SensorsOnLadders++;
         }

   LOG_INFO << "geoHMatrix pxl sensor on global: " << endm;
   for (int i = 0; i < kNumberOfPxlSectors; i++)
      for (int j = 0; j < kNumberOfPxlLaddersPerSector; j++)
         for (int k = 0; k < kNumberOfPxlSensorsPerLadder; k++) {
            mGeoHMatrixSensorOnGlobal[i][j][k].SetName(Form("sensorOnGlobal%03i%03i%03i", i + 1, j + 1, k + 1));
            mGeoHMatrixSensorOnGlobal[i][j][k] = (*mGeoHMatrixTpcOnGlobal) * mGeoHMatrixIdsOnTpc * mGeoHMatrixPstOnIds
                                                 * mGeoHMatrixPxlOnPst * mGeoHMatrixHalfOnPxl[i / 5] * mGeoHMatrixSectorOnHalf[i]
                                                 * mGeoHMatrixLadderOnSector[i][j] * mGeoHMatrixSensorOnLadder[i][j][k];
            mGeoHMatrixSensorOnGlobal[i][j][k].Print();
         }

}
//_____________________________________________________________________________
Int_t StPxlDb::sensorStatus(Int_t sector, Int_t ladder, Int_t sensor) const
{
   if (!mSensorStatusTable) return 255;
   return mSensorStatusTable->status[(sector - 1) * kNumberOfPxlLaddersPerSector * kNumberOfPxlSensorsPerLadder + (ladder - 1) * kNumberOfPxlSensorsPerLadder + (sensor - 1)];
}
//_____________________________________________________________________________
Int_t StPxlDb::rowStatus(Int_t sector, Int_t ladder, Int_t sensor, Int_t row) const
{
   if (!mRowColumnStatusTable) return 255;
   return mRowColumnStatusTable->rows[kNumberOfPxlRowsOnSensor * ((sector - 1) * (kNumberOfPxlSensorsPerLadder * kNumberOfPxlLaddersPerSector) + (ladder - 1) * kNumberOfPxlSensorsPerLadder + (sensor - 1)) + row];
}
//_____________________________________________________________________________
Int_t StPxlDb::columnStatus(Int_t sector, Int_t ladder, Int_t sensor, Int_t column) const
{
   if (!mRowColumnStatusTable) return 255;
   return mRowColumnStatusTable->cols[kNumberOfPxlColumnsOnSensor * ((sector - 1) * (kNumberOfPxlSensorsPerLadder * kNumberOfPxlLaddersPerSector) + (ladder - 1) * kNumberOfPxlSensorsPerLadder + (sensor - 1)) + column];
}
//_____________________________________________________________________________
void StPxlDb::setThinPlateSpline(pxlSensorTps_st *pxlSensorTps)
{
   for (Int_t i = 0; i < kNumberOfPxlSectors * kNumberOfPxlLaddersPerSector * kNumberOfPxlSensorsPerLadder; i++) {
      int id = pxlSensorTps[i].Id;
      int iSector = (id - 1) / kNumberOfPxlSensorsPerLadder / kNumberOfPxlLaddersPerSector;
      int iLadder = (id - 1) / kNumberOfPxlSensorsPerLadder % kNumberOfPxlLaddersPerSector;
      int iSensor = (id - 1) % kNumberOfPxlSensorsPerLadder;
      int nMeasurements = pxlSensorTps[i].nMeasurements;
      LOG_INFO << "thin plate spline: " << iSector << " " << iLadder << " " << iSensor << "    " << nMeasurements << " measurements." << endl;
      if (mThinPlateSpline[iSector][iLadder][iSensor]) {delete mThinPlateSpline[iSector][iLadder][iSensor];}
      mThinPlateSpline[iSector][iLadder][iSensor] = new StThinPlateSpline(nMeasurements, pxlSensorTps[i].X, pxlSensorTps[i].Y, pxlSensorTps[i].W, pxlSensorTps[i].A);
   }
}
