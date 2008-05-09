// -*- mode: c++;-*-
// $Id: StJetMakerBackwordCompatibility.h,v 1.4 2008/05/09 02:14:52 tai Exp $
// Copyright (C) 2008 Tai Sakuma <sakuma@bnl.gov>
#ifndef STJETMAKERBACKWORDCOMPATIBILITY_H
#define STJETMAKERBACKWORDCOMPATIBILITY_H

#include "StppJetAnalyzer.h"

#include <string>
#include <map>

namespace StSpinJet {

class StJetTreeWriter;

class StJetMakerBackwordCompatibility {

public:

  StJetMakerBackwordCompatibility() { }
  virtual ~StJetMakerBackwordCompatibility() { }

  typedef std::map<std::string, StppJetAnalyzer*> jetBranchesMap;

  void addAnalyzer(StppJetAnalyzer* analyzer, StJetTreeWriter* treeWriter, const char* name);

  jetBranchesMap& getJets() { return _jetBranches; }

private:

  jetBranchesMap  _jetBranches;

};

}

#endif // STJETMAKERBACKWORDCOMPATIBILITY_H


