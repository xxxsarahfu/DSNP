/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

#include "cirDef.h"
#include "cirGate.h"

extern CirMgr *cirMgr;

class CirMgr
{
public:
  CirMgr() : M(0), I(0), L(0), O(0), A(0)
            { glist.resize(0, 0); DFSlist.resize(0, 0);}
  ~CirMgr() {
    DFSlist.clear();
    for (size_t i = 0; i < glist.size(); i++)
      delete glist[i];
    glist.clear();
    PI.clear();
    LA.clear();
    PO.clear();
    li.clear();
    ID.clear();
    trueAIG.clear();
  }


  // Access functions
  // return '0' if "gid" corresponds to an undefined gate.
  CirGate* getGate(unsigned gid) const;

  // Member functions about circuit construction
  bool readCircuit(const string&);

  // Member functions about circuit optimization
  void sweep();
  void cutline(CirGate* &);
  void optimize();
  void myOptimize(CirGate* &);
  void kill(CirGate* &a , CirGate* b, bool inv = true);
  void updateFloat();
  void updateUdf();

  // Member functions about simulation
  void randomSim();
  void fileSim(ifstream&);
  void setSimLog(ofstream *logFile) { _simLog = logFile; }

  // Member functions about fraig
  void strash();
  void re_strash(CirGate* &, HashMap<HashKey, CirGate*> &);
  void replace(CirGate* &, CirGate*);
  void printFEC() const;
  void fraig();

  // Member functions about circuit reporting
  void printSummary() const;
  void printNetlist() ;
  void printPIs() const;
  void printPOs() const;
  void printFloatGates() const;
  void printFECPairs() const;
  void writeAag(ostream&) ;
  void writeGate(ostream&, CirGate*) const;

  // myself function
  bool readHeader(string&);
  void connectCircuit(CirGate*, CirGate*, int);
  void buildCircuit(GateList&);

  void DFSTraversal(CirGate*);
  void DFSlistbuild();
  void DFSReset();



private:
  ofstream           *_simLog;

  //MY CODE
  unsigned int gid;
  //number of...
  unsigned M;
  unsigned I;
  unsigned L;
  unsigned O;
  unsigned A;

  //literals
  vector<unsigned> PI;
  vector<unsigned> LA;
  vector<unsigned> PO;
  vector<unsigned> li;
  vector<unsigned> ID;
  vector<unsigned> trueAIG;
  vector<unsigned> truePI;
  vector<unsigned> truePO;
  vector<unsigned> finalPO;
 
  GateList glist;
  GateList DFSlist;
  
  vector<IdList> FEClist;
};

#endif // CIR_MGR_H
