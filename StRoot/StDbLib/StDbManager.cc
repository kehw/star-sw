/***************************************************************************
 *
 * $Id: StDbManager.cc,v 1.10 1999/12/03 22:24:01 porter Exp $
 *
 * Author: R. Jeff Porter
 ***************************************************************************
 *
 * Description:  Manages access to Servers and passes Query-by-Table to db
 *
 ***************************************************************************
 *
 * $Log: StDbManager.cc,v $
 * Revision 1.10  1999/12/03 22:24:01  porter
 * expanded functionality used by online, fixed bug in
 * mysqlAccessor::getElementID(char*), & update StDbDataSet to
 * conform to changes in Xml reader & writer
 *
 * Revision 1.9  1999/10/19 14:30:39  porter
 * modifications relevant to use with StDbBroker and future merging with
 * "params" database structure + some docs + suppressing diagnostics messages
 *
 * Revision 1.8  1999/09/30 02:06:07  porter
 * add StDbTime to better handle timestamps, modify SQL content (mysqlAccessor)
 * allow multiple rows (StDbTable), & Added the comment sections at top of
 * each header and src file
 *
 **************************************************************************/
#include "StDbManager.hh"
#include "StDbLists.hh"
#include "StDbConfigNode.hh"
#include "StDbServer.hh"
#include "StDbTable.h"
#include "StDbTime.h"
#include "TableIter.hh"
#include <iostream.h>
#include <strstream.h>
#include <strings.h>

StDbManager* StDbManager::mInstance=0;

////////////////////////////////////////////////////////////////

StDbManager::~StDbManager(){
 

  //  if(m_configNode)m_configNode->deleteTree();
  deleteServers();
  deleteDomains();
  deleteTypes();

}

////////////////////////////////////////////////////////////////

void
StDbManager::deleteServers() {  

  ServerList::iterator itr;
  StDbServer* server;

  do {
      for(itr = mservers.begin(); itr != mservers.end(); ++itr){
         server = *itr;
         mservers.erase(itr);
         delete server;
         break;
        }
     } while( mservers.begin() != mservers.end() );
}

////////////////////////////////////////////////////////////////

void
StDbManager::deleteDomains(){

  dbDomains::iterator itr;
  dbDomain* domain;

  do {
    for(itr = mDomains.begin(); itr!=mDomains.end(); ++itr){
        domain=*itr;
        mDomains.erase(itr);
        delete domain;
        break;
    }
  } while (mDomains.begin() != mDomains.end());

}

////////////////////////////////////////////////////////////////

void
StDbManager::deleteTypes(){

  dbTypes::iterator itr;
  dbType* type;

  do {
    for(itr = mTypes.begin(); itr!=mTypes.end(); ++itr){
        type=*itr;
        mTypes.erase(itr);
        delete type;
        break;
    }
  } while (mTypes.begin() != mTypes.end());

}

////////////////////////////////////////////////////////////////

char*
StDbManager::getDbTypeName(StDbType type){

char* name;
  for(dbTypes::iterator itr=mTypes.begin();
      itr != mTypes.end(); ++itr){
    if((*itr)->type == type){
      name = (*itr)->name;
      break;
    }
  }

return mstringDup(name);
}     

////////////////////////////////////////////////////////////////

char*
StDbManager::getDbDomainName(StDbDomain domain){

char* name;
  for(dbDomains::iterator itr=mDomains.begin();
      itr != mDomains.end(); ++itr){
    if((*itr)->domain == domain){
      name = (*itr)->name;
      break;
    }
  }

char* retName=mstringDup(name);
if(strcmp(retName,"unknown")==0)retName=0;

return retName;
}     


////////////////////////////////////////////////////////////////

StDbType
StDbManager::getDbType(const char* typeName){

StDbType retType;
  for(dbTypes::iterator itr=mTypes.begin();
      itr != mTypes.end(); ++itr){
    if(strcmp((*itr)->name,typeName)==0){
      retType = (*itr)->type;
      break;
    }
  }

return retType;
}     


////////////////////////////////////////////////////////////////

StDbDomain
StDbManager::getDbDomain(const char* domainName){

StDbDomain retType;
  for(dbDomains::iterator itr=mDomains.begin();
      itr != mDomains.end(); ++itr){
    if(strcmp((*itr)->name,domainName) ==0){
      retType = (*itr)->domain;
      break;
    }
  }


return retType;
}     

////////////////////////////////////////////////////////////////

void
StDbManager::initServers(const char* refFile){

  if(!refFile){
  //
  // No longer called. servers are created as needed 
  //
  /*
  StDbServer* mserver = 0;
  cout << "StDbManager:: Creating Server List " << endl;
  mservers.push_back(new StDbServer(StarDb,Star,"StarDb","Star"));
  mservers.push_back(new StDbServer(Conditions,Star,"Conditions","Star"));
  mservers.push_back(new StDbServer(Calibrations,Star,"Calibrations","Star"));
  mservers.push_back(new StDbServer(Geometry,Star,"Geometry","Star"));
  mservers.push_back(new StDbServer(Conditions,Tpc,"Conditions","tpc"));
  mservers.push_back(new StDbServer(Calibrations,Tpc,"Calibrations","tpc"));
  mservers.push_back(new StDbServer(Geometry,Tpc,"Geometry","tpc"));
  */
  }

}

////////////////////////////////////////////////////////////////

void
StDbManager::lookUpServers(){

  //cout << "looking up Servers " << endl;

char* xmlfile1 = getFileName("HOME");
char* xmlfile2 = getFileName("STDB_SERVERS");
char* xmlfile3 = getFileName("STAR","/StDb/servers");

if(xmlfile1){
 ifstream is1(xmlfile1);

 //cout << "Trying HomeDir " << xmlfile1 << endl;

 if(is1)findServersXml(is1);
 delete [] xmlfile1;
}

if(xmlfile2){
 ifstream is2(xmlfile2);
 if(is2)findServersXml(is2);
 delete [] xmlfile2;
}

if(xmlfile3){
 ifstream is3(xmlfile3);
 if(is3)findServersXml(is3);
 delete [] xmlfile3;
}

mhasServerList = true;

}

////////////////////////////////////////////////////////////////

char*
StDbManager::getFileName(const char* envName, const char* subDirName){

char* xmlFile = 0;
char* dirname=getenv(envName);

 if(dirname){

  xmlFile = new char[1024];
  ostrstream os(xmlFile,1024);

  char* serverFile="/dbServers.xml";

  if(subDirName){
     os<<dirname<<subDirName<<serverFile<<ends;
   } else {
     os<<dirname<<serverFile<<ends;
   }

 }


return xmlFile;
}


////////////////////////////////////////////////////////////////

void
StDbManager::findServersXml(ifstream& is){


  //cout << " finding Server from ifstream " << endl;
  //  parseXmlString mparser;
  char* stardatabase;

  while(!is.eof()){

    //cout << " not eof yet " << endl;
  // builds 1 string from file contained by "<StDbServer> ... </StDbServer>"
  

  stardatabase = findServerString(is); 
  if(!stardatabase) continue;

  int portNum = 0;
  //cout << stardatabase << endl;

  char* servName = mparser.getString(stardatabase,"<server>","</server>");
  //cout << "server Name = " << servName << endl;
  char* hostName = mparser.getString(stardatabase,"<host>","</host>");
  char* uSocket = mparser.getString(stardatabase,"<socket>","</socket>");
  char* portNumber = mparser.getString(stardatabase,"<port>","</port>");

  if(portNumber){
    portNum   = atoi(portNumber);
    delete [] portNumber;
  }

  char* dbNames = mparser.getString(stardatabase,"<databases>","</databases>");

   StDbServer* server = new StDbServer();
   server->setServerName(servName); delete [] servName;
   server->setHostName(hostName); delete [] hostName;
   server->setUnixSocket(uSocket); delete [] uSocket;
   server->setPortNumber(portNum); 

  if( !dbNames && !mhasDefaultServer ){

    //cout << " setting the default Server" << endl;
    server->setIsDefaultServer();
    mservers.push_back(server);
    mhasDefaultServer = true;

  } else {

    StDbServer* aserver;
    char* p1 = &dbNames[0];
    char* aname;

    while(p1){
      aname = getNextName(p1); // p1 is reset to after next "," 
      //cout << " dbName = " << aname;
      //if(p1) cout << " & the remaining are = " << p1<< endl;
      aserver = new StDbServer(*server);
      aserver->setDbName(aname);
      mservers.push_back(aserver);
      if(aname)delete [] aname;
    }
    
   if(dbNames)delete [] dbNames;
  }
  if(stardatabase)delete [] stardatabase;
  }

}

////////////////////////////////////////////////////////////////

char*
StDbManager::findServerString(ifstream& is){

char* line = new char[10240];
char tmpline[256];
bool done = false;
bool started = false;
char* id;

ostrstream os(line,10240);

 while(!done){

   if(is.eof()){
     done = true; 
     delete [] line;
     line = 0; // and no new complete string was found
   } else {
     is.getline(tmpline,255);

     if((id=strstr(tmpline,"//")))continue;

     if(!started){

        id= strstr(tmpline,"<StDbServer>");
        if(id){
          os<<tmpline;
          started = true;
        }

     } else {

        os<<tmpline;

        id=strstr(tmpline,"</StDbServer>");
        if(id){
          os<<ends;
          done=true;
        }

     } // started check          
   } // eof check 
 } // while loop

return line;
}

////////////////////////////////////////////////////////////////

char*
StDbManager::getNextName(char*& names){

char* nextName = 0;
if(!names)return nextName;

char* id = strstr(names,",");

if(!id) {
  // nextName = mstringDup(names);
 nextName = mparser.removeBlankEnds(names);
 //delete [] names;
 names = 0;
} else {
  int iloc = id-names;
  char* saveName = new char[iloc+1];
  strncpy(saveName,names,iloc);
  saveName[iloc]='\0';
  nextName=mparser.removeBlankEnds(saveName);
  delete [] saveName;
  names = id; names++;
}

return nextName;
}


////////////////////////////////////////////////////////////////

StDbServer*
StDbManager::findServer(StDbType type, StDbDomain domain){

 if(!mhasServerList)lookUpServers();
 StDbServer* server = 0;

 // first check if it exists in list

 for(ServerList::iterator itr = mservers.begin();
     itr != mservers.end(); ++itr){
   if((*itr)->getDbDomain()==domain && (*itr)->getDbType()==type){
     server = *itr;
     break;
   }
 }

 // if not build from default server

 if(!server){ 

    char * typeName = getDbTypeName(type);
    char * domainName = getDbDomainName(domain);
    //cout << " looking for Default server " << endl;
    StDbServer* defaultServer = findDefaultServer();

    if(defaultServer){
      // cout << " creating real server from default " << endl;
     server = new StDbServer(*defaultServer);
     // cout << "  Setting real DB attributes " << endl;
     server->setDataBase(type,domain,typeName,domainName);
     // cout << "  adding to list " << endl;
     mservers.push_back(server);
    }

    delete [] typeName; delete [] domainName;

 }

 // connect to database if needed

 //cout << server->getServerName() << " & " << server->isconnected() << endl;
 if(server && !server->isconnected()){
   server->init();
   //  cout << " Connecting Server " << endl;
 }
 // report failure

 if(!server) {
   cerr << "No Such Server :: " << endl;
   cerr << "Type Name= " << getDbTypeName(type) << endl;
   cerr << "Domain Name = " << getDbDomainName(domain) << endl;
}

return server;
}


////////////////////////////////////////////////////////////////

StDbServer*
StDbManager::findDefaultServer(){

 StDbServer* server = 0;

 // first check if it exists in list

 for(ServerList::iterator itr = mservers.begin();
     itr != mservers.end(); ++itr){
   if((*itr)->isDefault()){
     server = *itr;
     break;
   }
 }

 /*
 if(server){
    cout << " found default " << endl;
 } else {
    cout << " default not found " << endl;
 }
 */
return server;

}


////////////////////////////////////////////////////////////////

StDbConfigNode*
StDbManager::initConfig(const char* configName){

StDbConfigNode* configNode = new StDbConfigNode(StarDb,Star,"StarDb",configName);
configNode->buildTree();

return configNode;
}

////////////////////////////////////////////////////////////////
StDbConfigNode*
StDbManager::initConfig(StDbType type, StDbDomain domain, const char* configName){

StDbConfigNode * configNode = 0;

char* name;
 if(domain == Star){
  name = getDbTypeName(type);
 } else {
  name = getDbDomainName(domain);
 }

 configNode = new StDbConfigNode(type,domain,name,configName);
 configNode->buildTree();
 return configNode;

}

////////////////////////////////////////////////////////////////
void
StDbManager::setRequestTime(unsigned int time){

mcheckTime.munixTime = time;
StDbServer* server = findServer(StarDb,Star);
if(server)mcheckTime.mdateTime = server->getDateTime(time);

}

////////////////////////////////////////////////////////////////

void
StDbManager::setRequestTime(const char* time){

mcheckTime.setDateTime(time);
StDbServer* server = findServer(StarDb,Star);
if(server)mcheckTime.munixTime = server->getUnixTime(time);

}


////////////////////////////////////////////////////////////////
void
StDbManager::setStoreTime(unsigned int time){

mstoreTime.munixTime = time;
StDbServer* server = findServer(StarDb,Star);
if(server)mstoreTime.mdateTime = server->getDateTime(time);

}

////////////////////////////////////////////////////////////////

void
StDbManager::setStoreTime(const char* time){

mstoreTime.setDateTime(time);
StDbServer* server = findServer(StarDb,Star);
if(server)mstoreTime.munixTime = server->getUnixTime(time);

}

////////////////////////////////////////////////////////////////

unsigned int
StDbManager::getUnixCheckTime(){ return mcheckTime.munixTime; }

////////////////////////////////////////////////////////////////

char* 
StDbManager::getDateCheckTime(){ return mcheckTime.mdateTime; }

////////////////////////////////////////////////////////////////

unsigned int
StDbManager::getUnixStoreTime(){ return mstoreTime.munixTime; }

////////////////////////////////////////////////////////////////

char* 
StDbManager::getDateStoreTime(){ return mstoreTime.mdateTime; }

////////////////////////////////////////////////////////////////

bool
StDbManager::IsValid(StDbTableI* table){

 unsigned int time = mcheckTime.munixTime;

 bool retVal = false;
 if(!table) return retVal;
 if(time >= table->getBeginTime() && time < table->getEndTime()) retVal = true;

return retVal;

}

////////////////////////////////////////////////////////////////

void
StDbManager::fetchDbTable(StDbTableI* table){

  if(!table){
    cout << "Cannot Update StDbTable=0" << endl;
  } else {

  if(!mcheckTime.munixTime && !mcheckTime.mdateTime)return;

  StDbServer* server = findServer(table->getDbType(),table->getDbDomain());
  server->QueryDb((StDbTable*)table,mcheckTime.munixTime);  // table is filled 

  }
}

////////////////////////////////////////////////////////////////

void
StDbManager::fetchAllTables(StDbConfigNode* node){

  if(!mcheckTime.munixTime && !mcheckTime.mdateTime){
    cerr<< "No storage time set"; return;
  }

  if(node->hasData()){
    TableIter* itr = node->getTableIter();
    StDbTableI* table = 0;
    while(!itr->done()){
      table = itr->next();
      fetchDbTable(table);
    }
  delete itr;
  }

  if(node->hasChildren())fetchAllTables(node->getFirstChildNode());
  StDbConfigNode* nextNode = 0;
  if((nextNode=node->getNextNode()))fetchAllTables(nextNode);

}
    

////////////////////////////////////////////////////////////////

void
StDbManager::storeDbTable(StDbTableI* table){

  if(!table){
    cout << "Cannot Update StDbTable=0" << endl;
  } else {

    //  table->setRequestTime(time);
  if(!mstoreTime.munixTime && !mstoreTime.mdateTime){
    cerr<< "No storage time set"; return;
  }
  StDbServer* server = findServer(table->getDbType(),table->getDbDomain());
  server->WriteDb((StDbTable*)table,mstoreTime.munixTime);  // table is filled 

  }
}

////////////////////////////////////////////////////////////////

void
StDbManager::storeAllTables(StDbConfigNode* node){

  if(!mstoreTime.munixTime && !mstoreTime.mdateTime){
    cerr<< "No storage time set"; return;
  }

  if(node->hasData()){
    TableIter* itr = node->getTableIter();
    StDbTableI* table = 0;
    while(!itr->done()){
      table = itr->next();
      storeDbTable(table);
    }
  delete itr;
  }

  if(node->hasChildren())storeAllTables(node->getFirstChildNode());
  StDbConfigNode* nextNode = 0;
  if((nextNode=node->getNextNode()))storeAllTables(nextNode);

}
    

////////////////////////////////////////////////////////////////

void
StDbManager::initTypes(){

mTypes.push_back(new dbType(StarDb,"StarDb")); 
mTypes.push_back(new dbType(RunLog,"RunLog")); 
mTypes.push_back(new dbType(DbServer,"StDbServer")); 
mTypes.push_back(new dbType(Conditions,"Conditions")); 
mTypes.push_back(new dbType(Calibrations,"Calibrations")); 
mTypes.push_back(new dbType(Geometry,"Geometry")); 
mTypes.push_back(new dbType(RunCatalog,"RunCatalog")); 
mTypes.push_back(new dbType(Configurations,"Configurations")); 
mTypes.push_back(new dbType(RunParams,"RunParams")); 
mTypes.push_back(new dbType(TestScheme,"TestScheme")); 

}

////////////////////////////////////////////////////////////////

void
StDbManager::initDomains(){

mDomains.push_back(new dbDomain(Star,"Star")); 
mDomains.push_back(new dbDomain(Tpc,"tpc")); 
mDomains.push_back(new dbDomain(Ftpc,"ftpc")); 
mDomains.push_back(new dbDomain(Emc,"emc")); 
mDomains.push_back(new dbDomain(Svt,"svt")); 
mDomains.push_back(new dbDomain(Ctb,"ctb")); 
mDomains.push_back(new dbDomain(Trg,"trg")); 
mDomains.push_back(new dbDomain(Daq,"daq")); 
mDomains.push_back(new dbDomain(Scaler,"scaler")); 
mDomains.push_back(new dbDomain(Global,"global")); 
mDomains.push_back(new dbDomain(L3,"l3")); 

}

////////////////////////////////////////////////////////////////
char*
StDbManager::mstringDup(const char* str){

char* retString=0;
if(!str)return retString;
retString = new char[strlen(str)+1];
strcpy(retString,str);

return retString;
}


////////////////////////////////////////////////////////////////

bool
StDbManager::getDataBaseInfo(const char* dbName, char*& type, char*& domain){

bool retVal = false;

char* id = strstr(dbName,"/");
if(!id) {
type = new char[strlen(dbName)+1];
strcpy(type,dbName);
domain = new char[strlen("Star")+1];
strcpy(domain,"Star");
return true;
}

*id='\0';
type = new char[strlen(dbName)+1];
strcpy(type,dbName);
id++;
domain = new char[strlen(id)+1];
strcpy(domain,id);
retVal = true;

return retVal;
}



