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
#include "cirDef.h"
#include "cirGate.h"
using namespace std;



extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
public:
   CirMgr() : M(0), I(0), L(0), O(0), A(0) { glist.resize(0, 0); DFSlist.resize(0,0);}
   ~CirMgr() {
    DFSlist.clear();
    for(size_t i = 0; i < glist.size(); i++)
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

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist();// const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&) ;//const;

   //myself
   bool readHeader(string&);
   void connectCircuit(CirGate*, CirGate*, int);
   void buildCircuit(GateList&);

   void DFSTraversal(CirGate*);
   void DFSlistbuild();
   void DFSReset();
   
private://myself

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

  GateList glist;
  GateList DFSlist;
  //GateList 

};

#endif // CIR_MGR_H
