#include "EEmcSmdMap.h"
#include "EEmcSmd2SmdMapItem.h"

/////////////////////////////////////////////////////////////////////////////

ClassImp(EEmcSmdMap);

EEmcSmdMap *EEmcSmdMap::sInstance = 0;

// Returns the one and only instance of this class
EEmcSmdMap *EEmcSmdMap::instance() { 
  sInstance = (sInstance != 0) ? sInstance : new EEmcSmdMap();
  return sInstance;
}

// Constructor
EEmcSmdMap::EEmcSmdMap() {

  Init();

}

/////////////////////////////////////////////////////////////////////////////

void EEmcSmdMap::getRangeSmd2Smd( Int_t isector, Int_t iuv, Int_t istrip,
			Int_t &juv, Int_t &jMin, Int_t &jMax ) {
  
  // printf(" asking for isec=%d , plane=%c, istrip=%d\n",isector, 'U'+iuv,istrip);
  //printf("p=%p\n",eemcStrip2StripMapItem[iuv]);

  assert(isector>=0 && isector<kEEmcNumSectors);
  assert(iuv>=0 && iuv<kEEmcNumSmdUVs);
  assert(istrip>=0 && istrip<kEEmcNumStrips);
  jMin = mSmd2SmdMap[isector][iuv][istrip].iMin;
  jMax = mSmd2SmdMap[isector][iuv][istrip].iMax;
}

/////////////////////////////////////////////////////////////////////////////

void EEmcSmdMap::Init() {
  InitStrip2Strip();

  EEmcStripMapItem stripData[720] = {
  
    { "01TA01", 0, 17, 45, 67 }, 
    { "01TA02", 4, 27, 58, 82 }, 
    { "01TA03", 12, 38, 72, 98 }, 
    { "01TA04", 22, 50, 88, 115 }, 
    { "01TA05", 31, 62, 104, 133 }, 
    { "01TA06", 42, 75, 121, 152 }, 
    { "01TA07", 53, 88, 138, 171 }, 
    { "01TA08", 64, 102, 156, 192 }, 
    { "01TA09", 76, 117, 176, 214 }, 
    { "01TA10", 89, 132, 196, 237 }, 
    { "01TA11", 102, 148, 217, 260 }, 
    { "01TA12", 115, 165, 239, 285 }, 

    { "01TB01", 9, 30, 35, 57 }, 
    { "01TB02", 18, 42, 47, 71 }, 
    { "01TB03", 29, 55, 60, 86 }, 
    { "01TB04", 39, 68, 74, 102 }, 
    { "01TB05", 51, 82, 89, 119 }, 
    { "01TB06", 63, 96, 104, 136 }, 
    { "01TB07", 76, 111, 120, 155 }, 
    { "01TB08", 90, 128, 137, 174 }, 
    { "01TB09", 103, 145, 155, 195 }, 
    { "01TB10", 119, 162, 174, 216 }, 
    { "01TB11", 134, 180, 193, 238 }, 
    { "01TB12", 150, 200, 213, 261 }, 

    { "01TC01", 21, 43, 24, 46 }, 
    { "01TC02", 32, 56, 35, 59 }, 
    { "01TC03", 44, 70, 46, 73 }, 
    { "01TC04", 57, 85, 59, 87 }, 
    { "01TC05", 69, 100, 72, 103 }, 
    { "01TC06", 84, 116, 86, 119 }, 
    { "01TC07", 98, 133, 100, 136 }, 
    { "01TC08", 113, 151, 116, 153 }, 
    { "01TC09", 129, 170, 132, 172 }, 
    { "01TC10", 146, 189, 149, 192 }, 
    { "01TC11", 164, 209, 166, 212 }, 
    { "01TC12", 182, 231, 184, 233 }, 

    { "01TD01", 32, 54, 11, 33 }, 
    { "01TD02", 44, 68, 21, 45 }, 
    { "01TD03", 58, 83, 31, 57 }, 
    { "01TD04", 71, 100, 42, 70 }, 
    { "01TD05", 86, 116, 54, 84 }, 
    { "01TD06", 101, 134, 66, 99 }, 
    { "01TD07", 118, 152, 79, 114 }, 
    { "01TD08", 134, 172, 92, 130 }, 
    { "01TD09", 152, 192, 106, 147 }, 
    { "01TD10", 171, 213, 121, 165 }, 
    { "01TD11", 191, 235, 136, 183 }, 
    { "01TD12", 211, 259, 152, 202 }, 

    { "01TE01", 42, 64, 0, 20 }, 
    { "01TE02", 56, 79, 6, 30 }, 
    { "01TE03", 70, 95, 15, 41 }, 
    { "01TE04", 85, 112, 24, 52 }, 
    { "01TE05", 101, 130, 34, 65 }, 
    { "01TE06", 118, 149, 44, 77 }, 
    { "01TE07", 135, 169, 55, 91 }, 
    { "01TE08", 154, 190, 67, 105 }, 
    { "01TE09", 173, 211, 79, 119 }, 
    { "01TE10", 193, 234, 91, 135 }, 
    { "01TE11", 215, 258, 104, 151 }, 
    { "01TE12", 237, 283, 118, 168 }, 

    { "02TA01", 0, 19, 43, 65 }, 
    { "02TA02", 6, 30, 57, 80 }, 
    { "02TA03", 14, 41, 71, 96 }, 
    { "02TA04", 24, 52, 86, 113 }, 
    { "02TA05", 34, 64, 102, 131 }, 
    { "02TA06", 44, 77, 119, 150 }, 
    { "02TA07", 55, 90, 136, 170 }, 
    { "02TA08", 66, 104, 154, 190 }, 
    { "02TA09", 78, 119, 174, 212 }, 
    { "02TA10", 91, 134, 194, 235 }, 
    { "02TA11", 104, 150, 215, 258 }, 
    { "02TA12", 117, 167, 237, 283 }, 

    { "02TB01", 11, 33, 34, 55 }, 
    { "02TB02", 20, 45, 46, 69 }, 
    { "02TB03", 31, 57, 59, 84 }, 
    { "02TB04", 42, 70, 72, 100 }, 
    { "02TB05", 53, 84, 87, 117 }, 
    { "02TB06", 65, 99, 102, 135 }, 
    { "02TB07", 79, 114, 119, 153 }, 
    { "02TB08", 92, 130, 136, 173 }, 
    { "02TB09", 106, 147, 153, 193 }, 
    { "02TB10", 121, 164, 172, 214 }, 
    { "02TB11", 136, 183, 191, 236 }, 
    { "02TB12", 152, 202, 212, 259 }, 

    { "02TC01", 23, 45, 22, 44 }, 
    { "02TC02", 34, 59, 33, 57 }, 
    { "02TC03", 46, 72, 45, 71 }, 
    { "02TC04", 59, 87, 57, 86 }, 
    { "02TC05", 72, 102, 71, 101 }, 
    { "02TC06", 86, 119, 84, 117 }, 
    { "02TC07", 100, 135, 99, 134 }, 
    { "02TC08", 115, 153, 114, 152 }, 
    { "02TC09", 132, 172, 130, 170 }, 
    { "02TC10", 148, 191, 147, 190 }, 
    { "02TC11", 166, 212, 164, 210 }, 
    { "02TC12", 184, 233, 183, 231 }, 

    { "02TD01", 35, 57, 9, 32 }, 
    { "02TD02", 47, 71, 20, 43 }, 
    { "02TD03", 60, 86, 30, 56 }, 
    { "02TD04", 74, 102, 41, 69 }, 
    { "02TD05", 88, 118, 52, 83 }, 
    { "02TD06", 104, 136, 64, 97 }, 
    { "02TD07", 120, 154, 77, 113 }, 
    { "02TD08", 137, 174, 90, 129 }, 
    { "02TD09", 155, 194, 105, 145 }, 
    { "02TD10", 173, 215, 119, 163 }, 
    { "02TD11", 193, 237, 134, 181 }, 
    { "02TD12", 213, 260, 150, 200 }, 

    { "02TE01", 45, 66, 0, 18 }, 
    { "02TE02", 58, 81, 4, 28 }, 
    { "02TE03", 72, 98, 13, 39 }, 
    { "02TE04", 87, 115, 23, 51 }, 
    { "02TE05", 103, 133, 33, 63 }, 
    { "02TE06", 120, 151, 43, 75 }, 
    { "02TE07", 137, 171, 53, 89 }, 
    { "02TE08", 156, 191, 65, 103 }, 
    { "02TE09", 175, 213, 77, 118 }, 
    { "02TE10", 196, 236, 89, 133 }, 
    { "02TE11", 217, 260, 103, 149 }, 
    { "02TE12", 239, 285, 116, 166 }, 

    { "03TA01", 0, 18, 43, 64 }, 
    { "03TA02", 5, 29, 56, 80 }, 
    { "03TA03", 14, 40, 70, 96 }, 
    { "03TA04", 23, 51, 86, 113 }, 
    { "03TA05", 33, 63, 101, 131 }, 
    { "03TA06", 43, 76, 118, 150 }, 
    { "03TA07", 54, 90, 136, 170 }, 
    { "03TA08", 66, 104, 154, 190 }, 
    { "03TA09", 77, 119, 174, 212 }, 
    { "03TA10", 90, 134, 194, 235 }, 
    { "03TA11", 103, 150, 215, 258 }, 
    { "03TA12", 117, 167, 237, 283 }, 

    { "03TB01", 10, 32, 33, 55 }, 
    { "03TB02", 20, 44, 45, 69 }, 
    { "03TB03", 30, 57, 58, 84 }, 
    { "03TB04", 41, 70, 72, 100 }, 
    { "03TB05", 53, 84, 87, 117 }, 
    { "03TB06", 65, 98, 102, 135 }, 
    { "03TB07", 78, 113, 118, 153 }, 
    { "03TB08", 91, 129, 135, 172 }, 
    { "03TB09", 105, 146, 153, 192 }, 
    { "03TB10", 120, 164, 171, 214 }, 
    { "03TB11", 136, 182, 191, 236 }, 
    { "03TB12", 152, 202, 211, 259 }, 

    { "03TC01", 22, 45, 21, 44 }, 
    { "03TC02", 34, 58, 33, 56 }, 
    { "03TC03", 45, 72, 44, 70 }, 
    { "03TC04", 58, 86, 57, 85 }, 
    { "03TC05", 71, 102, 70, 100 }, 
    { "03TC06", 85, 118, 84, 117 }, 
    { "03TC07", 100, 135, 98, 134 }, 
    { "03TC08", 115, 153, 114, 152 }, 
    { "03TC09", 131, 171, 130, 170 }, 
    { "03TC10", 148, 191, 146, 190 }, 
    { "03TC11", 166, 211, 164, 210 }, 
    { "03TC12", 184, 233, 182, 231 }, 

    { "03TD01", 34, 56, 9, 31 }, 
    { "03TD02", 46, 70, 19, 42 }, 
    { "03TD03", 59, 85, 29, 55 }, 
    { "03TD04", 73, 101, 40, 69 }, 
    { "03TD05", 88, 118, 51, 82 }, 
    { "03TD06", 103, 136, 64, 97 }, 
    { "03TD07", 120, 154, 77, 112 }, 
    { "03TD08", 136, 173, 90, 128 }, 
    { "03TD09", 154, 194, 104, 145 }, 
    { "03TD10", 173, 215, 119, 163 }, 
    { "03TD11", 193, 237, 134, 181 }, 
    { "03TD12", 213, 261, 150, 200 }, 

    { "03TE01", 44, 66, 0, 17 }, 
    { "03TE02", 57, 81, 4, 27 }, 
    { "03TE03", 71, 97, 12, 39 }, 
    { "03TE04", 87, 114, 22, 50 }, 
    { "03TE05", 103, 132, 32, 62 }, 
    { "03TE06", 119, 151, 42, 75 }, 
    { "03TE07", 137, 171, 53, 89 }, 
    { "03TE08", 156, 192, 64, 103 }, 
    { "03TE09", 175, 213, 77, 117 }, 
    { "03TE10", 195, 236, 89, 133 }, 
    { "03TE11", 217, 260, 102, 149 }, 
    { "03TE12", 239, 282, 116, 166 }, 

    { "04TA01", 0, 17, 45, 67 }, 
    { "04TA02", 4, 27, 58, 82 }, 
    { "04TA03", 12, 38, 72, 98 }, 
    { "04TA04", 22, 50, 88, 115 }, 
    { "04TA05", 31, 62, 104, 133 }, 
    { "04TA06", 42, 75, 121, 152 }, 
    { "04TA07", 53, 88, 138, 171 }, 
    { "04TA08", 64, 102, 156, 192 }, 
    { "04TA09", 76, 117, 176, 214 }, 
    { "04TA10", 89, 132, 196, 237 }, 
    { "04TA11", 102, 148, 217, 260 }, 
    { "04TA12", 115, 165, 239, 282 },
 
    { "04TB01", 9, 30, 35, 57 }, 
    { "04TB02", 18, 42, 47, 71 }, 
    { "04TB03", 29, 55, 60, 86 }, 
    { "04TB04", 39, 68, 74, 102 }, 
    { "04TB05", 51, 82, 89, 119 }, 
    { "04TB06", 63, 96, 104, 136 }, 
    { "04TB07", 76, 111, 120, 155 }, 
    { "04TB08", 90, 128, 137, 174 }, 
    { "04TB09", 103, 145, 155, 195 }, 
    { "04TB10", 119, 162, 174, 216 }, 
    { "04TB11", 134, 180, 193, 238 }, 
    { "04TB12", 150, 200, 213, 261 }, 

    { "04TC01", 21, 43, 24, 46 }, 
    { "04TC02", 32, 56, 35, 59 }, 
    { "04TC03", 44, 70, 46, 73 }, 
    { "04TC04", 57, 85, 59, 87 }, 
    { "04TC05", 69, 100, 72, 103 }, 
    { "04TC06", 84, 116, 86, 119 }, 
    { "04TC07", 98, 133, 100, 136 }, 
    { "04TC08", 113, 151, 116, 153 }, 
    { "04TC09", 129, 170, 132, 172 }, 
    { "04TC10", 146, 189, 149, 192 }, 
    { "04TC11", 164, 209, 166, 212 }, 
    { "04TC12", 182, 231, 184, 233 }, 

    { "04TD01", 32, 54, 11, 33 }, 
    { "04TD02", 44, 68, 21, 45 }, 
    { "04TD03", 58, 83, 31, 57 }, 
    { "04TD04", 71, 100, 42, 70 }, 
    { "04TD05", 86, 116, 54, 84 }, 
    { "04TD06", 101, 134, 66, 99 }, 
    { "04TD07", 118, 152, 79, 114 }, 
    { "04TD08", 134, 172, 92, 130 }, 
    { "04TD09", 152, 192, 106, 147 }, 
    { "04TD10", 171, 213, 121, 165 }, 
    { "04TD11", 191, 235, 136, 183 }, 
    { "04TD12", 211, 259, 152, 202 }, 

    { "04TE01", 42, 64, 0, 20 }, 
    { "04TE02", 56, 79, 6, 30 }, 
    { "04TE03", 70, 95, 15, 41 }, 
    { "04TE04", 85, 112, 24, 52 }, 
    { "04TE05", 101, 130, 34, 65 }, 
    { "04TE06", 118, 149, 44, 77 }, 
    { "04TE07", 135, 169, 55, 91 }, 
    { "04TE08", 154, 190, 67, 105 }, 
    { "04TE09", 173, 211, 79, 119 }, 
    { "04TE10", 193, 234, 91, 135 }, 
    { "04TE11", 215, 258, 104, 151 }, 
    { "04TE12", 237, 283, 118, 168 },
 
    { "05TA01", 0, 19, 43, 65 }, 
    { "05TA02", 6, 30, 57, 80 }, 
    { "05TA03", 14, 41, 71, 96 }, 
    { "05TA04", 24, 52, 86, 113 }, 
    { "05TA05", 34, 64, 102, 131 }, 
    { "05TA06", 44, 77, 119, 150 }, 
    { "05TA07", 55, 90, 136, 170 }, 
    { "05TA08", 66, 104, 154, 190 }, 
    { "05TA09", 78, 119, 174, 212 }, 
    { "05TA10", 91, 134, 194, 235 }, 
    { "05TA11", 104, 150, 215, 258 }, 
    { "05TA12", 117, 167, 237, 283 }, 

    { "05TB01", 11, 33, 34, 55 }, 
    { "05TB02", 20, 45, 46, 69 }, 
    { "05TB03", 31, 57, 59, 84 }, 
    { "05TB04", 42, 70, 72, 100 }, 
    { "05TB05", 53, 84, 87, 117 }, 
    { "05TB06", 65, 99, 102, 135 }, 
    { "05TB07", 79, 114, 119, 153 }, 
    { "05TB08", 92, 130, 136, 173 }, 
    { "05TB09", 106, 147, 153, 193 }, 
    { "05TB10", 121, 164, 172, 214 }, 
    { "05TB11", 136, 183, 191, 236 }, 
    { "05TB12", 152, 202, 212, 259 }, 

    { "05TC01", 23, 45, 22, 44 }, 
    { "05TC02", 34, 59, 33, 57 }, 
    { "05TC03", 46, 72, 45, 71 }, 
    { "05TC04", 59, 87, 57, 86 }, 
    { "05TC05", 72, 102, 71, 101 }, 
    { "05TC06", 86, 119, 84, 117 }, 
    { "05TC07", 100, 135, 99, 134 }, 
    { "05TC08", 115, 153, 114, 152 }, 
    { "05TC09", 132, 172, 130, 170 }, 
    { "05TC10", 148, 191, 147, 190 }, 
    { "05TC11", 166, 212, 164, 210 }, 
    { "05TC12", 184, 233, 183, 231 }, 

    { "05TD01", 35, 57, 9, 32 }, 
    { "05TD02", 47, 71, 20, 43 }, 
    { "05TD03", 60, 86, 30, 56 }, 
    { "05TD04", 74, 102, 41, 69 }, 
    { "05TD05", 88, 118, 52, 83 }, 
    { "05TD06", 104, 136, 64, 97 }, 
    { "05TD07", 120, 154, 77, 113 }, 
    { "05TD08", 137, 174, 90, 129 }, 
    { "05TD09", 155, 194, 105, 145 }, 
    { "05TD10", 173, 215, 119, 163 }, 
    { "05TD11", 193, 237, 134, 181 }, 
    { "05TD12", 213, 260, 150, 200 }, 

    { "05TE01", 45, 66, 0, 18 }, 
    { "05TE02", 58, 81, 4, 28 }, 
    { "05TE03", 72, 98, 13, 39 }, 
    { "05TE04", 87, 115, 23, 51 }, 
    { "05TE05", 103, 133, 33, 63 }, 
    { "05TE06", 120, 151, 43, 75 }, 
    { "05TE07", 137, 171, 53, 89 }, 
    { "05TE08", 156, 191, 65, 103 }, 
    { "05TE09", 175, 213, 77, 118 }, 
    { "05TE10", 196, 236, 89, 133 }, 
    { "05TE11", 217, 260, 103, 149 }, 
    { "05TE12", 239, 285, 116, 166 }, 

    { "06TA01", 0, 18, 43, 64 }, 
    { "06TA02", 5, 29, 56, 80 }, 
    { "06TA03", 14, 40, 70, 96 }, 
    { "06TA04", 23, 51, 86, 113 }, 
    { "06TA05", 33, 63, 101, 131 }, 
    { "06TA06", 43, 76, 118, 150 }, 
    { "06TA07", 54, 90, 136, 170 }, 
    { "06TA08", 66, 104, 154, 190 }, 
    { "06TA09", 77, 119, 174, 212 }, 
    { "06TA10", 90, 134, 194, 235 }, 
    { "06TA11", 103, 150, 215, 258 }, 
    { "06TA12", 117, 167, 237, 283 }, 

    { "06TB01", 10, 32, 33, 55 }, 
    { "06TB02", 20, 44, 45, 69 }, 
    { "06TB03", 30, 57, 58, 84 }, 
    { "06TB04", 41, 70, 72, 100 }, 
    { "06TB05", 53, 84, 87, 117 }, 
    { "06TB06", 65, 98, 102, 135 }, 
    { "06TB07", 78, 113, 118, 153 }, 
    { "06TB08", 91, 129, 135, 172 }, 
    { "06TB09", 105, 146, 153, 192 }, 
    { "06TB10", 120, 164, 171, 214 }, 
    { "06TB11", 136, 182, 191, 236 }, 
    { "06TB12", 152, 202, 211, 259 }, 

    { "06TC01", 22, 45, 21, 44 }, 
    { "06TC02", 34, 58, 33, 56 }, 
    { "06TC03", 45, 72, 44, 70 }, 
    { "06TC04", 58, 86, 57, 85 }, 
    { "06TC05", 71, 102, 70, 100 }, 
    { "06TC06", 85, 118, 84, 117 }, 
    { "06TC07", 100, 135, 98, 134 }, 
    { "06TC08", 115, 153, 114, 152 }, 
    { "06TC09", 131, 171, 130, 170 }, 
    { "06TC10", 148, 191, 146, 190 }, 
    { "06TC11", 166, 211, 164, 210 }, 
    { "06TC12", 184, 233, 182, 231 }, 

    { "06TD01", 34, 56, 9, 31 }, 
    { "06TD02", 46, 70, 19, 42 }, 
    { "06TD03", 59, 85, 29, 55 }, 
    { "06TD04", 73, 101, 40, 69 }, 
    { "06TD05", 88, 118, 51, 82 }, 
    { "06TD06", 103, 136, 64, 97 }, 
    { "06TD07", 120, 154, 77, 112 }, 
    { "06TD08", 136, 173, 90, 128 }, 
    { "06TD09", 154, 194, 104, 145 }, 
    { "06TD10", 173, 215, 119, 163 }, 
    { "06TD11", 193, 237, 134, 181 }, 
    { "06TD12", 213, 261, 150, 200 }, 

    { "06TE01", 44, 66, 0, 17 }, 
    { "06TE02", 57, 81, 4, 27 }, 
    { "06TE03", 71, 97, 12, 39 }, 
    { "06TE04", 87, 114, 22, 50 }, 
    { "06TE05", 103, 132, 32, 62 }, 
    { "06TE06", 119, 151, 42, 75 }, 
    { "06TE07", 137, 171, 53, 89 }, 
    { "06TE08", 156, 192, 64, 103 }, 
    { "06TE09", 175, 213, 77, 117 }, 
    { "06TE10", 195, 236, 89, 133 }, 
    { "06TE11", 217, 260, 102, 149 }, 
    { "06TE12", 239, 285, 116, 166 }, 

    { "07TA01", 0, 17, 45, 67 }, 
    { "07TA02", 4, 27, 58, 82 }, 
    { "07TA03", 12, 38, 72, 98 }, 
    { "07TA04", 22, 50, 88, 115 }, 
    { "07TA05", 31, 62, 104, 133 }, 
    { "07TA06", 42, 75, 121, 152 }, 
    { "07TA07", 53, 88, 138, 171 }, 
    { "07TA08", 64, 102, 156, 192 }, 
    { "07TA09", 76, 117, 176, 214 }, 
    { "07TA10", 89, 132, 196, 237 }, 
    { "07TA11", 102, 148, 217, 260 }, 
    { "07TA12", 115, 165, 239, 285 }, 

    { "07TB01", 9, 30, 35, 57 }, 
    { "07TB02", 18, 42, 47, 71 }, 
    { "07TB03", 29, 55, 60, 86 }, 
    { "07TB04", 39, 68, 74, 102 }, 
    { "07TB05", 51, 82, 89, 119 }, 
    { "07TB06", 63, 96, 104, 136 }, 
    { "07TB07", 76, 111, 120, 155 }, 
    { "07TB08", 90, 128, 137, 174 }, 
    { "07TB09", 103, 145, 155, 195 }, 
    { "07TB10", 119, 162, 174, 216 }, 
    { "07TB11", 134, 180, 193, 238 }, 
    { "07TB12", 150, 200, 213, 261 }, 

    { "07TC01", 21, 43, 24, 46 }, 
    { "07TC02", 32, 56, 35, 59 }, 
    { "07TC03", 44, 70, 46, 73 }, 
    { "07TC04", 57, 85, 59, 87 }, 
    { "07TC05", 69, 100, 72, 103 }, 
    { "07TC06", 84, 116, 86, 119 }, 
    { "07TC07", 98, 133, 100, 136 }, 
    { "07TC08", 113, 151, 116, 153 }, 
    { "07TC09", 129, 170, 132, 172 }, 
    { "07TC10", 146, 189, 149, 192 }, 
    { "07TC11", 164, 209, 166, 212 }, 
    { "07TC12", 182, 231, 184, 233 }, 

    { "07TD01", 32, 54, 11, 33 }, 
    { "07TD02", 44, 68, 21, 45 }, 
    { "07TD03", 58, 83, 31, 57 }, 
    { "07TD04", 71, 100, 42, 70 }, 
    { "07TD05", 86, 116, 54, 84 }, 
    { "07TD06", 101, 134, 66, 99 }, 
    { "07TD07", 118, 152, 79, 114 }, 
    { "07TD08", 134, 172, 92, 130 }, 
    { "07TD09", 152, 192, 106, 147 }, 
    { "07TD10", 171, 213, 121, 165 }, 
    { "07TD11", 191, 235, 136, 183 }, 
    { "07TD12", 211, 259, 152, 202 }, 

    { "07TE01", 42, 64, 0, 20 }, 
    { "07TE02", 56, 79, 6, 30 }, 
    { "07TE03", 70, 95, 15, 41 }, 
    { "07TE04", 85, 112, 24, 52 }, 
    { "07TE05", 101, 130, 34, 65 }, 
    { "07TE06", 118, 149, 44, 77 }, 
    { "07TE07", 135, 169, 55, 91 }, 
    { "07TE08", 154, 190, 67, 105 }, 
    { "07TE09", 173, 211, 79, 119 }, 
    { "07TE10", 193, 234, 91, 135 }, 
    { "07TE11", 215, 258, 104, 151 }, 
    { "07TE12", 237, 283, 118, 168 }, 

    { "08TA01", 0, 19, 43, 65 }, 
    { "08TA02", 6, 30, 57, 80 }, 
    { "08TA03", 14, 41, 71, 96 }, 
    { "08TA04", 24, 52, 86, 113 }, 
    { "08TA05", 34, 64, 102, 131 }, 
    { "08TA06", 44, 77, 119, 150 }, 
    { "08TA07", 55, 90, 136, 170 }, 
    { "08TA08", 66, 104, 154, 190 }, 
    { "08TA09", 78, 119, 174, 212 }, 
    { "08TA10", 91, 134, 194, 235 }, 
    { "08TA11", 104, 150, 215, 258 }, 
    { "08TA12", 117, 167, 237, 283 },
    
    { "08TB01", 11, 33, 34, 55 }, 
    { "08TB02", 20, 45, 46, 69 }, 
    { "08TB03", 31, 57, 59, 84 }, 
    { "08TB04", 42, 70, 72, 100 }, 
    { "08TB05", 53, 84, 87, 117 }, 
    { "08TB06", 65, 99, 102, 135 }, 
    { "08TB07", 79, 114, 119, 153 }, 
    { "08TB08", 92, 130, 136, 173 }, 
    { "08TB09", 106, 147, 153, 193 }, 
    { "08TB10", 121, 164, 172, 214 }, 
    { "08TB11", 136, 183, 191, 236 }, 
    { "08TB12", 152, 202, 212, 259 }, 

    { "08TC01", 23, 45, 22, 44 }, 
    { "08TC02", 34, 59, 33, 57 }, 
    { "08TC03", 46, 72, 45, 71 }, 
    { "08TC04", 59, 87, 57, 86 }, 
    { "08TC05", 72, 102, 71, 101 }, 
    { "08TC06", 86, 119, 84, 117 }, 
    { "08TC07", 100, 135, 99, 134 }, 
    { "08TC08", 115, 153, 114, 152 }, 
    { "08TC09", 132, 172, 130, 170 }, 
    { "08TC10", 148, 191, 147, 190 }, 
    { "08TC11", 166, 212, 164, 210 }, 
    { "08TC12", 184, 233, 183, 231 }, 

    { "08TD01", 35, 57, 9, 32 }, 
    { "08TD02", 47, 71, 20, 43 }, 
    { "08TD03", 60, 86, 30, 56 }, 
    { "08TD04", 74, 102, 41, 69 }, 
    { "08TD05", 88, 118, 52, 83 }, 
    { "08TD06", 104, 136, 64, 97 }, 
    { "08TD07", 120, 154, 77, 113 }, 
    { "08TD08", 137, 174, 90, 129 }, 
    { "08TD09", 155, 194, 105, 145 }, 
    { "08TD10", 173, 215, 119, 163 }, 
    { "08TD11", 193, 237, 134, 181 }, 
    { "08TD12", 213, 260, 150, 200 }, 

    { "08TE01", 45, 66, 0, 18 }, 
    { "08TE02", 58, 81, 4, 28 }, 
    { "08TE03", 72, 98, 13, 39 }, 
    { "08TE04", 87, 115, 23, 51 }, 
    { "08TE05", 103, 133, 33, 63 }, 
    { "08TE06", 120, 151, 43, 75 }, 
    { "08TE07", 137, 171, 53, 89 }, 
    { "08TE08", 156, 191, 65, 103 }, 
    { "08TE09", 175, 213, 77, 118 }, 
    { "08TE10", 196, 236, 89, 133 }, 
    { "08TE11", 217, 260, 103, 149 }, 
    { "08TE12", 239, 285, 116, 166 }, 

    { "09TA01", 0, 18, 43, 64 }, 
    { "09TA02", 5, 29, 56, 80 }, 
    { "09TA03", 14, 40, 70, 96 }, 
    { "09TA04", 23, 51, 86, 113 }, 
    { "09TA05", 33, 63, 101, 131 }, 
    { "09TA06", 43, 76, 118, 150 }, 
    { "09TA07", 54, 90, 136, 170 }, 
    { "09TA08", 66, 104, 154, 190 }, 
    { "09TA09", 77, 119, 174, 212 }, 
    { "09TA10", 90, 134, 194, 235 }, 
    { "09TA11", 103, 150, 215, 258 }, 
    { "09TA12", 117, 167, 237, 283 }, 

    { "09TB01", 10, 32, 33, 55 }, 
    { "09TB02", 20, 44, 45, 69 }, 
    { "09TB03", 30, 57, 58, 84 }, 
    { "09TB04", 41, 70, 72, 100 }, 
    { "09TB05", 53, 84, 87, 117 }, 
    { "09TB06", 65, 98, 102, 135 }, 
    { "09TB07", 78, 113, 118, 153 }, 
    { "09TB08", 91, 129, 135, 172 }, 
    { "09TB09", 105, 146, 153, 192 }, 
    { "09TB10", 120, 164, 171, 214 }, 
    { "09TB11", 136, 182, 191, 236 }, 
    { "09TB12", 152, 202, 211, 259 }, 

    { "09TC01", 22, 45, 21, 44 }, 
    { "09TC02", 34, 58, 33, 56 }, 
    { "09TC03", 45, 72, 44, 70 }, 
    { "09TC04", 58, 86, 57, 85 }, 
    { "09TC05", 71, 102, 70, 100 }, 
    { "09TC06", 85, 118, 84, 117 }, 
    { "09TC07", 100, 135, 98, 134 }, 
    { "09TC08", 115, 153, 114, 152 }, 
    { "09TC09", 131, 171, 130, 170 }, 
    { "09TC10", 148, 191, 146, 190 }, 
    { "09TC11", 166, 211, 164, 210 }, 
    { "09TC12", 184, 233, 182, 231 }, 

    { "09TD01", 34, 56, 9, 31 }, 
    { "09TD02", 46, 70, 19, 42 }, 
    { "09TD03", 59, 85, 29, 55 }, 
    { "09TD04", 73, 101, 40, 69 }, 
    { "09TD05", 88, 118, 51, 82 }, 
    { "09TD06", 103, 136, 64, 97 }, 
    { "09TD07", 120, 154, 77, 112 }, 
    { "09TD08", 136, 173, 90, 128 }, 
    { "09TD09", 154, 194, 104, 145 }, 
    { "09TD10", 173, 215, 119, 163 }, 
    { "09TD11", 193, 237, 134, 181 }, 
    { "09TD12", 213, 261, 150, 200 }, 

    { "09TE01", 44, 66, 0, 17 }, 
    { "09TE02", 57, 81, 4, 27 }, 
    { "09TE03", 71, 97, 12, 39 }, 
    { "09TE04", 87, 114, 22, 50 }, 
    { "09TE05", 103, 132, 32, 62 }, 
    { "09TE06", 119, 151, 42, 75 }, 
    { "09TE07", 137, 171, 53, 89 }, 
    { "09TE08", 156, 192, 64, 103 }, 
    { "09TE09", 175, 213, 77, 117 }, 
    { "09TE10", 195, 236, 89, 133 }, 
    { "09TE11", 217, 260, 102, 149 }, 
    { "09TE12", 239, 282, 116, 166 }, 

    { "10TA01", 0, 17, 45, 67 }, 
    { "10TA02", 4, 27, 58, 82 }, 
    { "10TA03", 12, 38, 72, 98 }, 
    { "10TA04", 22, 50, 88, 115 }, 
    { "10TA05", 31, 62, 104, 133 }, 
    { "10TA06", 42, 75, 121, 152 }, 
    { "10TA07", 53, 88, 138, 171 }, 
    { "10TA08", 64, 102, 156, 192 }, 
    { "10TA09", 76, 117, 176, 214 }, 
    { "10TA10", 89, 132, 196, 237 }, 
    { "10TA11", 102, 148, 217, 260 }, 
    { "10TA12", 115, 165, 239, 282 }, 

    { "10TB01", 9, 30, 35, 57 }, 
    { "10TB02", 18, 42, 47, 71 }, 
    { "10TB03", 29, 55, 60, 86 }, 
    { "10TB04", 39, 68, 74, 102 }, 
    { "10TB05", 51, 82, 89, 119 }, 
    { "10TB06", 63, 96, 104, 136 }, 
    { "10TB07", 76, 111, 120, 155 }, 
    { "10TB08", 90, 128, 137, 174 }, 
    { "10TB09", 103, 145, 155, 195 }, 
    { "10TB10", 119, 162, 174, 216 }, 
    { "10TB11", 134, 180, 193, 238 }, 
    { "10TB12", 150, 200, 213, 261 }, 

    { "10TC01", 21, 43, 24, 46 }, 
    { "10TC02", 32, 56, 35, 59 }, 
    { "10TC03", 44, 70, 46, 73 }, 
    { "10TC04", 57, 85, 59, 87 }, 
    { "10TC05", 69, 100, 72, 103 }, 
    { "10TC06", 84, 116, 86, 119 }, 
    { "10TC07", 98, 133, 100, 136 }, 
    { "10TC08", 113, 151, 116, 153 }, 
    { "10TC09", 129, 170, 132, 172 }, 
    { "10TC10", 146, 189, 149, 192 }, 
    { "10TC11", 164, 209, 166, 212 }, 
    { "10TC12", 182, 231, 184, 233 }, 

    { "10TD01", 32, 54, 11, 33 }, 
    { "10TD02", 44, 68, 21, 45 }, 
    { "10TD03", 58, 83, 31, 57 }, 
    { "10TD04", 71, 100, 42, 70 }, 
    { "10TD05", 86, 116, 54, 84 }, 
    { "10TD06", 101, 134, 66, 99 }, 
    { "10TD07", 118, 152, 79, 114 }, 
    { "10TD08", 134, 172, 92, 130 }, 
    { "10TD09", 152, 192, 106, 147 }, 
    { "10TD10", 171, 213, 121, 165 }, 
    { "10TD11", 191, 235, 136, 183 }, 
    { "10TD12", 211, 259, 152, 202 }, 

    { "10TE01", 42, 64, 0, 20 }, 
    { "10TE02", 56, 79, 6, 30 }, 
    { "10TE03", 70, 95, 15, 41 }, 
    { "10TE04", 85, 112, 24, 52 }, 
    { "10TE05", 101, 130, 34, 65 }, 
    { "10TE06", 118, 149, 44, 77 }, 
    { "10TE07", 135, 169, 55, 91 }, 
    { "10TE08", 154, 190, 67, 105 }, 
    { "10TE09", 173, 211, 79, 119 }, 
    { "10TE10", 193, 234, 91, 135 }, 
    { "10TE11", 215, 258, 104, 151 }, 
    { "10TE12", 237, 283, 118, 168 }, 

    { "11TA01", 0, 19, 43, 65 }, 
    { "11TA02", 6, 30, 57, 80 }, 
    { "11TA03", 14, 41, 71, 96 }, 
    { "11TA04", 24, 52, 86, 113 }, 
    { "11TA05", 34, 64, 102, 131 }, 
    { "11TA06", 44, 77, 119, 150 }, 
    { "11TA07", 55, 90, 136, 170 }, 
    { "11TA08", 66, 104, 154, 190 }, 
    { "11TA09", 78, 119, 174, 212 }, 
    { "11TA10", 91, 134, 194, 235 }, 
    { "11TA11", 104, 150, 215, 258 }, 
    { "11TA12", 117, 167, 237, 283 }, 

    { "11TB01", 11, 33, 34, 55 }, 
    { "11TB02", 20, 45, 46, 69 }, 
    { "11TB03", 31, 57, 59, 84 }, 
    { "11TB04", 42, 70, 72, 100 }, 
    { "11TB05", 53, 84, 87, 117 }, 
    { "11TB06", 65, 99, 102, 135 }, 
    { "11TB07", 79, 114, 119, 153 }, 
    { "11TB08", 92, 130, 136, 173 }, 
    { "11TB09", 106, 147, 153, 193 }, 
    { "11TB10", 121, 164, 172, 214 }, 
    { "11TB11", 136, 183, 191, 236 }, 
    { "11TB12", 152, 202, 212, 259 }, 

    { "11TC01", 23, 45, 22, 44 }, 
    { "11TC02", 34, 59, 33, 57 }, 
    { "11TC03", 46, 72, 45, 71 }, 
    { "11TC04", 59, 87, 57, 86 }, 
    { "11TC05", 72, 102, 71, 101 }, 
    { "11TC06", 86, 119, 84, 117 }, 
    { "11TC07", 100, 135, 99, 134 }, 
    { "11TC08", 115, 153, 114, 152 }, 
    { "11TC09", 132, 172, 130, 170 }, 
    { "11TC10", 148, 191, 147, 190 }, 
    { "11TC11", 166, 212, 164, 210 }, 
    { "11TC12", 184, 233, 183, 231 }, 

    { "11TD01", 35, 57, 9, 32 }, 
    { "11TD02", 47, 71, 20, 43 }, 
    { "11TD03", 60, 86, 30, 56 }, 
    { "11TD04", 74, 102, 41, 69 }, 
    { "11TD05", 88, 118, 52, 83 }, 
    { "11TD06", 104, 136, 64, 97 }, 
    { "11TD07", 120, 154, 77, 113 }, 
    { "11TD08", 137, 174, 90, 129 }, 
    { "11TD09", 155, 194, 105, 145 }, 
    { "11TD10", 173, 215, 119, 163 }, 
    { "11TD11", 193, 237, 134, 181 }, 
    { "11TD12", 213, 260, 150, 200 }, 

    { "11TE01", 45, 66, 0, 18 }, 
    { "11TE02", 58, 81, 4, 28 }, 
    { "11TE03", 72, 98, 13, 39 }, 
    { "11TE04", 87, 115, 23, 51 }, 
    { "11TE05", 103, 133, 33, 63 }, 
    { "11TE06", 120, 151, 43, 75 }, 
    { "11TE07", 137, 171, 53, 89 }, 
    { "11TE08", 156, 191, 65, 103 }, 
    { "11TE09", 175, 213, 77, 118 }, 
    { "11TE10", 196, 236, 89, 133 }, 
    { "11TE11", 217, 260, 103, 149 }, 
    { "11TE12", 239, 285, 116, 166 },
 
    { "12TA01", 0, 18, 43, 64 }, 
    { "12TA02", 5, 29, 56, 80 }, 
    { "12TA03", 14, 40, 70, 96 }, 
    { "12TA04", 23, 51, 86, 113 }, 
    { "12TA05", 33, 63, 101, 131 }, 
    { "12TA06", 43, 76, 118, 150 }, 
    { "12TA07", 54, 90, 136, 170 }, 
    { "12TA08", 66, 104, 154, 190 }, 
    { "12TA09", 77, 119, 174, 212 }, 
    { "12TA10", 90, 134, 194, 235 }, 
    { "12TA11", 103, 150, 215, 258 }, 
    { "12TA12", 117, 167, 237, 283 }, 

    { "12TB01", 10, 32, 33, 55 }, 
    { "12TB02", 20, 44, 45, 69 }, 
    { "12TB03", 30, 57, 58, 84 }, 
    { "12TB04", 41, 70, 72, 100 }, 
    { "12TB05", 53, 84, 87, 117 }, 
    { "12TB06", 65, 98, 102, 135 }, 
    { "12TB07", 78, 113, 118, 153 }, 
    { "12TB08", 91, 129, 135, 172 }, 
    { "12TB09", 105, 146, 153, 192 }, 
    { "12TB10", 120, 164, 171, 214 }, 
    { "12TB11", 136, 182, 191, 236 }, 
    { "12TB12", 152, 202, 211, 259 }, 

    { "12TC01", 22, 45, 21, 44 }, 
    { "12TC02", 34, 58, 33, 56 }, 
    { "12TC03", 45, 72, 44, 70 }, 
    { "12TC04", 58, 86, 57, 85 }, 
    { "12TC05", 71, 102, 70, 100 }, 
    { "12TC06", 85, 118, 84, 117 }, 
    { "12TC07", 100, 135, 98, 134 }, 
    { "12TC08", 115, 153, 114, 152 }, 
    { "12TC09", 131, 171, 130, 170 }, 
    { "12TC10", 148, 191, 146, 190 }, 
    { "12TC11", 166, 211, 164, 210 }, 
    { "12TC12", 184, 233, 182, 231 }, 

    { "12TD01", 34, 56, 9, 31 }, 
    { "12TD02", 46, 70, 19, 42 }, 
    { "12TD03", 59, 85, 29, 55 }, 
    { "12TD04", 73, 101, 40, 69 }, 
    { "12TD05", 88, 118, 51, 82 }, 
    { "12TD06", 103, 136, 64, 97 }, 
    { "12TD07", 120, 154, 77, 112 }, 
    { "12TD08", 136, 173, 90, 128 }, 
    { "12TD09", 154, 194, 104, 145 }, 
    { "12TD10", 173, 215, 119, 163 }, 
    { "12TD11", 193, 237, 134, 181 }, 
    { "12TD12", 213, 261, 150, 200 }, 

    { "12TE01", 44, 66, 0, 17 }, 
    { "12TE02", 57, 81, 4, 27 }, 
    { "12TE03", 71, 97, 12, 39 }, 
    { "12TE04", 87, 114, 22, 50 }, 
    { "12TE05", 103, 132, 32, 62 }, 
    { "12TE06", 119, 151, 42, 75 }, 
    { "12TE07", 137, 171, 53, 89 }, 
    { "12TE08", 156, 192, 64, 103 }, 
    { "12TE09", 175, 213, 77, 117 }, 
    { "12TE10", 195, 236, 89, 133 }, 
    { "12TE11", 217, 260, 102, 149 }, 
    { "12TE12", 239, 285, 116, 166 }

  };

  //--
  //-- Initialize the strip-range to tower mapping
  //--
  Int_t itow = 0;
  for ( Int_t isec = 0; isec < 12; isec++ ) {
    for ( Int_t isub = 0; isub < 5; isub++ ) {
      for ( Int_t ieta = 0; ieta < 12; ieta++ ) {
	mSmdMap[isec][isub][ieta] = stripData[itow++];
      }
    }
  }


  //--
  //-- Initialize the tower to strip mapping
  //--
  itow = 0;
  for ( Int_t isec = 0; isec < 12; isec++ ) {
    for ( Int_t isub = 0; isub < 5; isub++ ) {
      for ( Int_t ieta = 0; ieta < 12; ieta++ ) {

	//-- Get the range in U and V strips for this tower
	Int_t uMin, uMax;
	Int_t vMin, vMax;

	getRangeU(isec,isub,ieta, uMin,uMax);
	getRangeV(isec,isub,ieta, vMin,vMax);

	//-- Loop over the range in u strips and add tower to
	//-- the flat list of towers, and increment the count
	for ( Int_t u = uMin; u <= uMax; u++ ) {
	  mTowerMap[isec][0][u].nTower++;
	  mTowerMap[isec][0][u].towers.push_back( TString( stripData[itow].tower ) );
	  mTowerMap[isec][0][u].sector.push_back(isec);
	  mTowerMap[isec][0][u].subsector.push_back(isub);
	  mTowerMap[isec][0][u].etabin.push_back(ieta);
	}
	//-- Repeat for v strips
	for ( Int_t v = vMin; v <= vMax; v++ ) {
	  mTowerMap[isec][1][v].nTower++;
	  mTowerMap[isec][1][v].towers.push_back( TString( stripData[itow].tower ) );
	  mTowerMap[isec][1][v].sector.push_back(isec);
	  mTowerMap[isec][1][v].subsector.push_back(isub);
	  mTowerMap[isec][1][v].etabin.push_back(ieta);
	}

	//-- Increment the tower index
	itow++;

      }
    }
  }

}
