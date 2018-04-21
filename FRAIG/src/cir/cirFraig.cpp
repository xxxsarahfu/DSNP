/****************************************************************************
  FileName     [ cirFraig.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir FRAIG functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "sat.h"
#include "myHashMap.h"
#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::strash()" and "CirMgr::fraig()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
HashKey
getkey(CirGate* g)
{
	size_t l = g->fanin[0]->getID() * 2 + (g->invert_or_not[0] == 0);
	size_t r = g->fanin[1]->getID() * 2 + (g->invert_or_not[1] == 0);
	if(l < r) return HashKey(l, r);
	else 			return HashKey(r, l);
}

/*******************************************/
/*   Public member functions about fraig   */
/*******************************************/
// _floatList may be changed.
// _unusedList and _undefList won't be changed
void
CirMgr::strash()
{
	DFSlistbuild();
	//cout << "DFS build1. " << endl;
	HashMap<HashKey, CirGate*> hash(getHashSize(M));
	//cout << "DFSlist size = " << DFSlist.size() << endl;
	for(size_t i = 0 ; i < DFSlist.size(); i++){
		CirGate* t = DFSlist[i];
		//CirGate* m = DFSlist[i];
		//CirGate* t = glist[m->getID()];
		if(t->getTypeStr() == "AIG"){
			//cout << "t = AIG" << endl;
			//cout << t->getID() << " have fanin size " << t->fanin.size() << endl;
			//cout << t->getID() << " " << t->fanin[0]->getID() << " " << t->fanin[1]->getID() << endl;
			HashKey k = getkey(t);
			//cout << "getkey done" << endl;
			if(!hash.insert(k,t)){
				//cout << "insert" << endl;
				CirGate* pre;
				hash.query(k,pre);
				if(t == pre) break;
				cout << "Strashing: " << pre->getID() << " merging " << t->getID() << "..." << endl;
				replace(t,pre);
			}
		}
	}
}
/*
void
CirMgr::re_strash(CirGate* &g, HashMap<HashKey, CirGate*> &hash)
{
	//cout << "Gate " << g->gateid << " " << g->type << endl;
	if (g != 0 && g->type == "AIG") {
		HashKey k = getkey(g);
		//cout << "key = " << k() << endl;
		if (!hash.insert(k, g)) {
			//cout << "M" << endl;
			CirGate* king;
			hash.query(k, king);
			if (g == king) {
				//cout << "self check?" << endl;
				return;
			}
			cout << "Strashing: " << king->gateid << " merging " << g->gateid << "..." << endl;

			replace(g, king);

			for (size_t f = 0; f < king->fanout.size(); f++) {
				re_strash(king -> fanout[f], hash);
			}
			return;
		}

		for (size_t f = 0; f < g->fanout.size(); f++) {
			re_strash(g -> fanout[f], hash);
		}
		return;
	}
}
*/

void
CirMgr::fraig()
{
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/
void
CirMgr::replace(CirGate* &a, CirGate* b ) // b merge a
{
	for(size_t i = 0; i < a->fanin.size(); i++) { 
		for(size_t j = 0; j < a->fanin[i]->fanout.size(); j++) { 
			if(a == a->fanin[i]->fanout[j]){
				a->fanin[i]->fanout.erase(a->fanin[i]->fanout.begin() + j);
				//a->invert_or_not[i].erase()
				break;
			} 
		}
	}

	for (size_t i = 0; i < a->fanout.size(); i++) { 
		for (size_t j = 0; j < a->fanout[i]->fanin.size(); j++) { 
			if (a->fanout[i]->fanin[j] == a){
				a->fanout[i]->fanin[j] = b;
				//b->fanout.push_back(a->fanout[i]);
				//b->fanout[i] = a->fanout[i];
				//a->fanout[i]->invert_or_not.erase((a->fanout[j]->invert_or_not).begin() + j);
				//a->fanout[i]->invert_or_not[j] = (a->fanout[i]->invert_or_not[j] == 1);
			break;
			}
		}
	}
	//cout << a->getID() << " " << " fanin size = " << a->fanin.size() << ", fanout size = "<< a->fanout.size() << endl;
	//a->fanin.clear();
	//a->invert_or_not.clear();
	//a->fanout.clear();

	//cout << a->getID() << "After" << " fanin size = " << a->fanin.size() << ", fanout size = "<< a->fanout.size() << endl;
	//delete a;
	--A;
	//a = 0;
	glist[a->getID()] = 0;
}