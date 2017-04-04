/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

// TODO: Keep "CirGate::reportGate()", "CirGate::reportFanin()" and
//       "CirGate::reportFanout()" for cir cmds. Feel free to define
//       your own variables and functions.

extern CirMgr *cirMgr;

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
	ostringstream ss;
	ostringstream ss2;
	ostringstream ss3;
	if (type == "") cout << "Error: Gate(" << gateid << ") " << "not found!!" << endl << "Error: Illegal option!! (" << gateid << ")" << endl;
	else {
		cout << "==================================================" << endl;
		ss << "= " << type << "(" << gateid << ")";
		if (name != "")
			ss << "\"" << name << "\"";
		ss << ", line " << lineNum;
		cout <<  setw(49) << left << ss.str() << right << "=" << endl;
		ss2 << "= FECs: ";
		cout <<  setw(49) << left << ss2.str() << right << "=" << endl;
		ss3 << "= Value: ";
		cout <<  setw(49) << left << ss3.str() << right << "=" << endl;
		cout << "==================================================" << endl;
	}
}

unsigned CirGate::_globalRef = 0;

void
CirGate::reportFanin(int level) //const
{
	assert (level >= 0);
	CirGate::setGlobalRef();
	myFanin(this, level, 0, 1);
}

void
CirGate::myFanin(CirGate* travGate, unsigned level, unsigned levelCount, bool inv_or_not)
{
	assert (level >= 0);
	assert( levelCount <= level);
	for (size_t i = 0; i < levelCount; i++) { cout << "  ";  }
	if (inv_or_not == false) cout << "!";
	cout << travGate -> getTypeStr() << " " << travGate -> getID();

	if ((travGate->fanin.size() == 0) || levelCount == level) { cout << endl; return;}

	if (travGate->isGlobalRef() == true) cout << " (*)" << endl;
	else {
		cout << endl;
		for (size_t i = 0; i < travGate->fanin.size(); i++) {
			myFanin(travGate->fanin[i], level, levelCount + 1, travGate->invert_or_not[i]); //不能打++耶好酷哈哈
		}
	}
	travGate->setToGlobalRef();//標記一波
	return;

}

void
CirGate::reportFanout(int level) //const
{
	assert (level >= 0);
	CirGate::setGlobalRef();
	myFanout(this, level, 0, 1);
}

void
CirGate::myFanout(CirGate* travGate, unsigned level, unsigned levelCount, bool inv_or_not)
{
	assert (level >= 0);
	assert( levelCount <= level);
	cout << "fanout = " << travGate -> fanout.size() << " " << travGate->fanout[0]->gateid << endl;
	for (size_t i = 0; i < levelCount; i++) { cout << "  ";}
	if (inv_or_not == false) cout << "!";
	cout << travGate -> getTypeStr() << " " << travGate -> getID();
	if ((travGate->fanout.size() == 0) || levelCount == level) {cout << endl; return;}
	if (travGate->isGlobalRef() == true) cout << " (*)" << endl;
	else {
		cout << endl;
		for (size_t i = 0; i < travGate->fanout.size(); i++) {
			for (size_t j = 0; j < travGate->fanout[i]->fanin.size(); j++) {
				if (travGate -> fanout[i] -> fanin[j] -> getID() == travGate -> getID())
					myFanout(travGate->fanout[i], level, levelCount + 1, travGate->fanout[i]->invert_or_not[j]);
			}
		}
	}
	travGate->setToGlobalRef();
	return;
}
