/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"
#include "sat.h"

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
class CirGate
{
public:
  CirGate(unsigned gid = 0, unsigned lineNo = 0)
    : gateid(gid), lineNum(lineNo), name(""){
    invert_or_not.resize(2, true);
    float_or_not = false;
    def_or_not = false;
    in[0] = in[1] = 0;
    op = 0;
  }
  virtual ~CirGate() {fanin.clear(); fanout.clear();}

  // Basic access methods
  string getTypeStr() const   { return type; }
  unsigned getLineNo() const  { return lineNum; }
  unsigned getID() const      { return gateid; }
  string getSymbol() const    { return name; }
  void setSymbol(string sym)  { name = sym; }

  virtual bool isAig() const { return false; }

  // Printing functions
  virtual void printGate() const {}

  void reportGate() const;
  void reportFanin(int level); //const;
  void reportFanout(int level);// const;
  void myFanin(CirGate*, unsigned, unsigned, bool);
  void myFanout(CirGate*, unsigned, unsigned, bool);

  //DFTraversal
  bool isGlobalRef() { return (_ref == _globalRef); }
  void setToGlobalRef() { _ref = _globalRef; }
  static void setGlobalRef() { _globalRef++; }

  unsigned gateid;
  unsigned lineNum;
  unsigned in[2], op;
  vector<CirGate*> fanin;
  vector<CirGate*> fanout;
  vector<bool> invert_or_not;

  bool float_or_not;
  bool def_or_not;
  string type, name;
  static unsigned _globalRef;
  unsigned _ref;


private:

protected:
};

class PIGate: public CirGate
{
public:
  PIGate(unsigned gid = 0, unsigned lineNo = 0)
    : CirGate(gid, lineNo) { type = "PI"; gateid = gid; lineNum = lineNo; }
  ~PIGate() {};

};

class POGate: public CirGate
{
public:
  POGate(unsigned gid = 0, unsigned lineNo = 0)
    : CirGate(gid, lineNo) { type = "PO"; gateid = gid; lineNum = lineNo;}
  ~POGate() {};

};

class ANDGate: public CirGate
{
public:
  ANDGate(unsigned gid = 0, unsigned lineNo = 0)//, unsigned i1 = 0, unsigned i2 = 0)
    : CirGate(gid, lineNo) { type = "AIG"; gateid = gid; lineNum = lineNo;} //in1 = pi1; in2 = pi2;}
  ~ANDGate() {};

};

class UNDEFGate: public CirGate
{
public:
  UNDEFGate(unsigned gid = 0, unsigned lineNo = 0)//id only cuz noline???
    : CirGate(gid, lineNo) {type = "UNDEF"; gateid = gid; lineNum = lineNo;}
  ~UNDEFGate() {};

};

class NOGate: public CirGate
{
public:
  NOGate(unsigned gid = 0, unsigned lineNo = 0)//id only cuz noline???
    : CirGate(gid, lineNo) {type = "NO"; gateid = gid; lineNum = 0;}
  ~NOGate() {};

};


class CONSTGate: public CirGate
{
public:
  CONSTGate(unsigned gid = 0, unsigned lineNo = 0)
    : CirGate(gid, lineNo) { type = "CONST"; gateid = 0; lineNum = 0;}

  ~CONSTGate() {};

};

#endif // CIR_GATE_H