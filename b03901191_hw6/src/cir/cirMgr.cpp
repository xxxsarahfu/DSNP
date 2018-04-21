/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "util.h"

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
//static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine constant (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/

CirGate* 
CirMgr::getGate(unsigned gid) const {
  if(gid > M + O) return 0;
  if(glist[gid] != 0) return glist[gid];
  return 0;
}

bool
CirMgr::readCircuit(const string& fileName)
{
  //cout << "Reading !!" <<endl;
      ifstream cirFile(fileName.c_str());
      if(cirFile.is_open()){
        string Firstline;
        getline(cirFile,Firstline,'\n');

        if(readHeader(Firstline)){
          glist.resize(M+O+1,0);
          glist[0] = new CONSTGate(0,0);

          string Line, comment;
          unsigned pi,po,ad,pi1,pi2,id;
          char type;
          char* name = new char[Line.size()+1];
          
          //I:input
          for(size_t i = 0; i < I; i++){
            getline(cirFile,Line,'\n');
            if(sscanf(Line.c_str(),"%d",&pi) == 1){
              lineNo = 2 + i;
              gid = pi/2;
              PI.push_back(pi); 
              li.push_back(lineNo);
              ID.push_back(gid);
              glist[gid] = new PIGate(gid,lineNo);
              glist[gid] -> op = pi;
            }
            else return parseError(ILLEGAL_NUM);  
          }

          //O:output
          for(size_t i = 0; i < O; i++){
            getline(cirFile,Line,'\n');
            if(sscanf(Line.c_str(),"%d",&po) == 1){
              lineNo = 2 + I + i;
              gid = M + i + 1;
              PO.push_back(po);
              li.push_back(lineNo);
              ID.push_back(gid);
              glist[1+M+i] = new POGate(gid,lineNo);
              glist[gid] -> in[0] = po; 
            }
            else return parseError(ILLEGAL_NUM); 
          }       
          
          //A:and gate 
          for(size_t i = 0; i < A; i++){
            getline(cirFile,Line,'\n');
            if(sscanf(Line.c_str(),"%d %d %d", &ad, &pi1, &pi2) == 3){
              lineNo = 2 + I + O + i;
              gid = ad/2;
              li.push_back(lineNo);
              glist[gid] = new ANDGate(gid,lineNo);
              glist[gid] -> in[0] = pi1;
              glist[gid] -> in[1] = pi2;
              glist[gid] -> op = ad;
            }

            else return parseError(ILLEGAL_NUM);
          }               
          
          //Symbols & Comments 
          while(getline(cirFile,Line,'\n')){
            /*if(sscanf(Line.c_str(),"%c", &type) == 1){
              if(sscanf(Line.c_str(),"%[^\n]")) return parseError(ILLEGAL_NUM);
              else {
                if(sscanf(Line.c_str(),"%d %s", &id,name) == 2){
                  if(type == 'i')
                    glist[1+id]->setSymbol(name);
                  else if(type == 'o')
                    glist[M+1+id]->setSymbol(name);
                  else return parseError(ILLEGAL_NUM);
                }
              }
            }*/
            if(sscanf(Line.c_str(),"%c%d %s", &type,&id,name) == 3){   //name 要不要加&
                if(type == 'i')
                  glist[1+id]->setSymbol(name);
                else if(type == 'o') 
                  glist[M+1+id]->setSymbol(name);
            } 

            else if(Line == "c") break;
            else { parseError(ILLEGAL_NUM); return false;}
          }
          //cout <<"glist size is : "<< glist.size() <<endl;
          buildCircuit(glist);
          //cout << "Build Done!" <<endl;
        }
        else return parseError(ILLEGAL_NUM);
      }
      else{
        cout << "Cannot open design \"" << fileName << "\"!!" << endl;  
        return false;
      }
    //cout << "read Done !" <<endl;
   return true;
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
  cout << endl;
  cout << "Circuit Statistics" << endl;
  cout << "==================" << endl;
  cout << "  PI" << setw(12) << right << I << "  " << endl;
  cout << "  PO" << setw(12) << right << O << "  " << endl;
  cout << "  AIG" << setw(11) << right << A << "  " << endl;
  cout << "------------------" << endl;
  cout << "  Total" << setw(9) << right << I + O + A << endl;
}


void
CirMgr::printNetlist() //const
{
  DFSlistbuild();
  for(size_t i = 0; i < DFSlist.size(); i++){
    
    cout << "[" << i << "] " << setw(3) << left << DFSlist[i]->getTypeStr() << " " << DFSlist[i]->getID();
    for(size_t j = 0; j < DFSlist[i]->fanin.size(); j++){
      cout << " ";
      if(DFSlist[i]->fanin[j]->getTypeStr() == "UNDEF") cout << "*";
      if(DFSlist[i]->invert_or_not[j] == false) cout << "!";
      cout << DFSlist[i]->fanin[j]->getID();// << " ";
    }
  if(DFSlist[i]->getSymbol()!="") cout << " (" << DFSlist[i]->getSymbol() << ")";
  cout << endl;
  }
}

void
CirMgr::DFSlistbuild()
{
  DFSReset();
  for(size_t i = 0; i < O; i++)
    DFSTraversal(glist[M+i+1]);
}

void
CirMgr::DFSTraversal(CirGate* travGate)
{
  assert(travGate->getTypeStr() != "UNDEF");
  assert(travGate->getTypeStr() != ""); 
  assert(travGate->isGlobalRef() == false);

  for(size_t i = 0; i < travGate->fanin.size(); i++){
    if((travGate->fanin[i]->getTypeStr() != "UNDEF") &&
       (travGate->fanin[i]->isGlobalRef() == false))
      DFSTraversal(travGate->fanin[i]);
  }
  DFSlist.push_back(travGate);
  travGate->setToGlobalRef();
  if(travGate->getTypeStr() == "AIG") {trueAIG.push_back(travGate->getID());}
}

void
CirMgr::DFSReset()
{
  DFSlist.clear();
  trueAIG.clear();
  CirGate::setGlobalRef();
}

void
CirMgr::printPIs() const //ok
{
  cout << "PIs of the circuit:";
  for(size_t i = 0; i < PI.size(); i++) 
    cout << " " << PI[i]/2;
  cout << endl;
}

void
CirMgr::printPOs() const //ok
{
  cout << "POs of the circuit:";
  for(size_t i = 0; i < PO.size(); i++) 
    cout << " " << M+1+i;
  cout << endl;
}

void
CirMgr::printFloatGates() const
{
  bool have_fl = false;
  bool have_und = false;
  for(size_t i = 0; i < M+O+1; i++){
    if(glist[i] != 0){
      if(glist[i]->getTypeStr() == "AIG"){
        if(glist[i]->fanin.size()!= 2)
          {glist[i]->float_or_not = true; have_fl = true;}
        if(glist[i]->fanout.size() == 0) {glist[i]-> def_or_not = true; have_und = true;}
      }
      else if(glist[i]->getTypeStr() == "PO"){
        if(glist[i]->fanin.size() == 0) { glist[i]-> float_or_not = true; have_fl = true;}
      } 
      else if(glist[i]->getTypeStr() == "PI"){
        if(glist[i]->fanout.size() == 0) { glist[i]-> def_or_not = true; have_und = true;}
      }
    }
  }

  if(have_fl == true){
    cout << "Gates with floating fanin(s):" ; 
    for(size_t i = 0; i < glist.size(); i++)
      if(glist[i] != 0)
        if(glist[i]->float_or_not == true)
          cout << " " << glist[i]->getID();
    cout << endl; 
  }

  if(have_und == true){
    cout << "Gates defined but not used  :";
    for(size_t i = 0; i < glist.size(); i++)
      if(glist[i] != 0)
        if(glist[i]->def_or_not == true)
          cout << " " << glist[i]->getID();
    cout << endl; 
  }    
}

void
CirMgr::writeAag(ostream& outfile) //const
{
  DFSReset();
  for(size_t i = 0; i < O; i++)
    DFSTraversal(glist[M+i+1]);
  outfile << "aag " << M << " " << I << " " << L << " " << O << " " << trueAIG.size() << endl;
  for(size_t i = 0; i < I; i++) outfile << PI[i] << endl;
  for(size_t i = 0; i < O; i++) outfile << PO[i] << endl;
  for(size_t i = 0; i < trueAIG.size(); i++) {
    unsigned t = trueAIG[i];
    outfile << glist[t] -> op << " " << glist[t]->in[0] << " " << glist[t]->in[1] << endl;
  }
  for(size_t i = 0; i < I; i++)
    if(glist[PI[i]/2]->getSymbol() != "") outfile << "i" << i << " " << glist[PI[i]/2]->getSymbol() << endl;
  for(size_t i = 0; i < O; i++)
    if(glist[M+i+1]->getSymbol() != "") outfile << "o" << i << " " << glist[M+1+i]->getSymbol() << endl;
  
  outfile << "c" << endl << "AAG output by xxxsarahfu" << endl;
}




void
CirMgr::connectCircuit(CirGate* LHS, CirGate* RHS, int pos){
  //cout << "DOing connection!" <<endl;
  if(((RHS->in[pos])/2 * 2) != RHS->in[pos]) {RHS -> invert_or_not[pos] = false;} //LHS-> invert_or_noo[pos] = false;}
  LHS -> fanout.push_back(RHS);
  if(!(RHS -> getTypeStr() == "PO" && RHS -> in[0]/2 != LHS -> getID())) RHS->fanin.push_back(LHS);
}

void 
CirMgr::buildCircuit(GateList& glist){
  for(size_t i = 1; i < M+O+1; i++){
    if(glist[i] != 0){
      for(size_t j = 0; j < 2; j++){
        if(glist[i] -> getTypeStr() != "PI" && !(j==1 && glist[i] -> getTypeStr() == "PO") && glist[i] -> in[j]/2 == 0)
          connectCircuit(glist[0], glist[i], j);
        else if(getGate((glist[i]->in[j])/2) != 0 && glist[i]->in[j] != 0)
          connectCircuit(glist[(glist[i]->in[j])/2], glist[i], j);
        else if(getGate((glist[i]->in[j])/2) == 0 && glist[i]->in[j] != 0 && glist[i] -> getTypeStr() != "PI" && glist[i] -> getTypeStr() != "UNDEF"){//undef id(in[0] in[1]) doesnt in gatelist
          glist[(glist[i]->in[j])/2] = new UNDEFGate((glist[i]->in[j])/2 , 0);
          glist[glist[i]->in[j] /2]-> op = glist[i]->in[j];
          glist[glist[i]->in[j] /2]-> in[0] = 0;
          glist[glist[i]->in[j] /2]-> in[1] = 0;
        }
      }
    }
  } 
}

bool 
CirMgr::readHeader(string& Firstline){
    if(sscanf(Firstline.c_str(), "aag %d %d %d %d %d", &M, &I, &L, &O, &A) != 5)
      return false;
    else {
      if(M < I + A)return false;
      else return true;
    }
}






