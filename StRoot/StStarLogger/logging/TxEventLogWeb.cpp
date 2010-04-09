/*****************************************************************
 * @file TxEventLogFile.cpp
 * @author Valeri Fine
 *
 * @(#)cpp/api:$Id: TxEventLogWeb.cpp,v 1.4 2010/04/09 16:28:19 fine Exp $
 *
 * Please see TxEventLogFile.h for more documentation.
 *****************************************************************/

#include "TxEventLogWeb.h"

#include <string>
#include <cassert>

using namespace TxLogging;
namespace {
//const char *WebServiceURL="http://connery.star.bnl.gov/ucm/?m=";
const char *WebServiceURL="http://f3.star.bnl.gov/UCMCollector/UCMCollector --post-data=\"ucm=";
}

void TxEventLogWeb::writeDown(const std::string& message)
{
  std::string httpstring="wget -b  -q -o /dev/null ";
  httpstring+= "-O /dev/null ";
  httpstring+= WebServiceURL;

  std::string qmessage = message;
  std::string searchString( "'" ); 
  std::string replaceString( "%27" );

  std::string::size_type pos = 0;
  while ( (pos = qmessage.find(searchString, pos)) != std::string::npos ) {
        qmessage.replace( pos, searchString.size(), replaceString );
        pos++;
  }
  httpstring+=qmessage;
  httpstring+="\'>/dev/null";
  system( httpstring.c_str());
}
	// --- 
TXEVENT_DEFAULT_IMPLEMENTAION(TxEventLogWeb)	
TXEVENT_DEFAULT_IMPLEMENTAION_2(TxEventLogWeb)	
