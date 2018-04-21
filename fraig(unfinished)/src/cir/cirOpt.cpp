/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir optimization functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::sweep()" and "CirMgr::optimize()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/
// Remove unused gates
// DFS list should NOT be changed
// UNDEF, float and unused list may be changed
void
CirMgr::sweep()
{

	IdList sweeplist;

	unsigned unusedcount = 0;
	for(size_t i = 0; i < glist.size(); i++)
		if(glist[i]->fanout.size() == 0) unusedcount++;
	if(unusedcount == 0) return;

	for(size_t i = 0; i < glist.size(); i++){
		for(size_t j = 0; j < DFSlist.size(); j++){
			if(i != DFSlist[j]->getID()){//不在dfs
				if(glist[i] -> getTypeStr() == "AIG" || glist[i] -> getTypeStr() == "UNDEF") {
					cout << "Sweeping: " << glist[i] -> getTypeStr() << "(" << i << ") removed..." << endl;
					kill(glist[i]);//kill:清線
					sweeplist.push_back(i);
				}
			}
		}
		for(size_t i = 0; i < sweeplist.size(); i++){
      delete glist[sweeplist[i]];
      glist[sweeplist[i]] = NULLs
    }
    A = trueAIG.size();
	}

}

// Recursively simplifying from POs;
// _dfsList needs to be reconstructed afterwards
// UNDEF gates may be delete if its fanout becomes empty...
void
CirMgr::optimize()
{
}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/


/***************************************************/
/*   Private member functions about sweep by myself*/
/***************************************************/
void
CirMgr::kill(CirGate* killgate){

	for(int i = 0; i < killgate->fanin.size(); i++){
		for(int j = 0; j < killgate->fanin[i]->fanout.size(); j++)
			if(killgate == killgate->fanin[i]->fanout[j]){
				killgate->fanin[i]->fanout.erase(killgate->fanin[i]->fanout.begin() + j);
				break;
			}
	}
	for(int i = 0; i < killgate->fanout.size(); i++){
		for(int j = 0; j < killgate->fanout[i]->fanin.size(); j++)
			if(killgate == killgate->fanout[i]->fanin[j]){
				killgate->fanout[i]->fanin.erase(killgate->fanout[i]->fanin.begin() + j);
			}
	}
	//killgate->fanout.clear();//?
	//killgate->fanin.clear();//?

}
