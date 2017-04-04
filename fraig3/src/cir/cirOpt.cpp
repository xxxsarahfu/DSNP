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
#include <algorithm>

using namespace std;

// TODO: Please keep "CirMgr::sweep()" and "CirMgr::optimize()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
void
CirMgr::cutline(CirGate* &c)
{
	for(size_t i = 0; i < c->fanout.size(); i++){
		for(size_t j = 0; j < c->fanout[i]->fanin.size(); j++){
			if(c->fanout[i]->fanin[j] == c){
				c->fanout[i]->fanin.erase((c->fanout[i]->fanin).begin() + j);
				c->fanout[i]->invert_or_not.erase((c->fanout[i]->invert_or_not).begin() + j);
			}
		}
	}
	for(size_t i = 0; i < c->fanin.size(); i++){
		for(size_t j = 0; j < c->fanin[i]->fanout.size(); j++){
			if(c == c->fanin[i]->fanout[j]){
				c->fanin[i]->fanout.erase((c->fanin[i]->fanout).begin() + j);
			}
		}
	}
	
	c->fanin.clear();
	c->fanout.clear();
	//c->fanin.push_back(p);
	c->invert_or_not.clear();
	//k->invert_or_not.push_back(inv);
	delete c;
	c = 0;
	//--A;
}

/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/
// Remove unused gates
// DFS list should NOT be changed
// UNDEF, float and unused list may be changed

void
CirMgr::sweep()
{
	
	DFSlistbuild();
	unsigned unused = 0;
	for(size_t i = 0; i < glist.size(); i++)
		if(glist[i] != 0 && glist[i]->getTypeStr() != "PO"// && glist[i]->getTypeStr() != "UNDEF" 
			&& glist[i]->fanout.empty())
			unused++;
	if(unused == 0) return; 


		for(unsigned i = 1; i < glist.size(); ++i){
			if(glist[i]!= 0){
				GateList::iterator it;
				it = find(DFSlist.begin(), DFSlist.end(), glist[i]);
				if(it == DFSlist.end()) {
					//cout << i << " WHY NOT IN DFS!!!" << endl;
					if(glist[i]->getTypeStr() == "AIG" || glist[i]->getTypeStr() == "UNDEF") {
						cout << "Sweeping: " << glist[i]->getTypeStr()
							  << "(" << i << ") removed..." << endl;
						cutline(glist[i]);
						//cout << "cut success." << endl;
						//kill(glist[i]);
					}
				}
			}
		}

}

// Recursively simplifying from POs;
// _dfsList needs to be reconstructed afterwards
// UNDEF gates may be delete if its fanout becomes empty...
void
CirMgr::optimize()
{
	DFSlistbuild();

	for(size_t i = 0; i < DFSlist.size(); i++){
		if(DFSlist[i]->fanin.size() == 2){ //must be AND gate
			myOptimize(glist[DFSlist[i]->getID()]);
		}
	}	
}
/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/

void
CirMgr::myOptimize(CirGate* &d){
	
	CirGate* pre = 0; 
	bool inv = true;// no invert

	if((d->fanin[0] == glist[0] && d->invert_or_not[0] == true) ||
		 (d->fanin[1] == glist[0] && d->invert_or_not[1] == true)){
			pre = glist[0]; inv = true;
		}
	//case 2: fanin = 1
	else if(d->fanin[0] == glist[0] && d->invert_or_not[0] == false){
		pre = d->fanin[1]; inv = d->invert_or_not[1];
	}
	else if(d->fanin[1] == glist[0] && d->invert_or_not[1] == false){
		pre = d->fanin[0]; inv = d->invert_or_not[0];
	}
	//case 3/4: two same fanin (++,-- / +- )
	else if(d->fanin[0] == d->fanin[1]){
		if(d->invert_or_not[0] == d->invert_or_not[1]){ 
			pre = d->fanin[0]; inv = d->invert_or_not[0];
		}
		else{
			pre = glist[0]; inv = true;
		}
	}

	if(pre){ 
		cout << "Simplifying: " << pre->getID() << " merging " << (inv ? "" : "!") << d->getID() << "..." << endl;
		kill(d, pre, inv);
	}

}


void
CirMgr::kill(CirGate* &k, CirGate* p, bool inv){
	//assert(opt_finish = false);
	for(size_t i = 0; i < k->fanin.size(); i++){
		for(size_t j = 0; j < k->fanin[i]->fanout.size(); j++){
			if(k == k->fanin[i]->fanout[j]){
				k->fanin[i]->fanout.erase(k->fanin[i]->fanout.begin() + j);
				break;
			}
		}
	}

	for(size_t i = 0; i < k->fanout.size(); i++){
		p->fanout.push_back(k->fanout[i]);
	}

	for(size_t i = 0; i < k->fanout.size(); i++) {
		for(size_t j = 0; j < k->fanout[i]->fanin.size(); j++) {
			if(k == k->fanout[i]->fanin[j]) {
				k->fanout[i]->fanin[j] = p;
				k->fanout[i]->invert_or_not[j] = (inv == k->fanout[i]->invert_or_not[j]);
				break;
			}
		}
	}

	//k->fanout.clear();
	k->fanin.clear();
	k->fanin.push_back(p);
	k->invert_or_not.clear();
	k->invert_or_not.push_back(inv);
	delete k;
	k = 0;
	--A;
}
