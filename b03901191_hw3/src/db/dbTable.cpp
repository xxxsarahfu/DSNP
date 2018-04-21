/****************************************************************************
  FileName     [ dbTable.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Table member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <string>
#include <cctype>
#include <cassert>
#include <set>
#include <algorithm>
#include <sstream> //
#include <iostream> //
#include <fstream> //
#include "dbTable.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/

#define NOO INT_MAX



ostream& operator << (ostream& os, const DBRow& r)
{
   // TODO: to print out a row.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'

  for(int i = 0; i < r.size(); i++){
    if(r[i] == NOO) os << ".";
    else os << r[i];
  }
   return os;
}

ostream& operator << (ostream& os, const DBTable& t)
{
   // TODO: to print out a table
   // - Data are seperated by setw(6) and aligned right.
   // - Null cells should be left blank (printed as ' ').

  for(int i = 0; i < t.nRows(); i++){
    for(int j = 0; j < t.nCols(); j++){
      if(t[i][j] == NOO) os << setw(6) << right << ".";//_table or t?
      else os << setw(6) << right << t[i][j];//_table or t???
    }
    os << endl;
  }  

   return os;
}

ifstream& operator >> (ifstream& ifs, DBTable& t)//ifs hw1 = file 
{
   // TODO: to read in data from csv file and store them in a table
   // - You can assume all the data of the table are in a single line.
  //ifs.open(csvFile, ios::in);

  string line;
  while (getline(ifs, line, '\r')){
    if (line.empty()) break; 
    stringstream ss(line);
    int _ncols = std::count(line.begin(),line.end(), ',') + 1;//?? int _ncols = t.nCols(); 

    //vector<int> r;
    //for(int j = 0; j < _ncols; j++)
      //r.push_back(line[j]);
    DBRow r;


    string a;
    for(int j = 0; j < _ncols; j++){
      getline(ss,a,',');
      if(a.empty()){ r.addData(NOO); /*r[i++] = NOO;*/ }//先做事後再加加？
      else {r.addData(atoi(a.c_str())); }
      a.clear();//
    }
    //_table.push_back(r);
    t.addRow(r);

    line.clear();
  }
  int _nrows = t.nRows();
  return ifs;
}

/*****************************************/
/*   Member Functions for class DBRow    */
/*****************************************/
void
DBRow::removeCell(size_t c)
{
   // TODO
   _data.erase(_data.begin() + c);
}

/*****************************************/
/*   Member Functions for struct DBSort  */
/*****************************************/
bool
DBSort::operator() (const DBRow& r1, const DBRow& r2) const
{
   // TODO: called as a functional object that compares the data in r1 and r2
   //       based on the order defined in _sortOrder
  for(int i = 0; i < _sortOrder.size(); i++)//- 1?
    if(r1[_sortOrder[i]] != r2[_sortOrder[i]])
      return r1[_sortOrder[i]] < r2[_sortOrder[i]]; 
    else return false;
}

/*****************************************/
/*   Member Functions for class DBTable  */
/*****************************************/
void
DBTable::reset()
{
   // TODO
  _table.clear();
}

void
DBTable::addCol(const vector<int>& d)
{
   // TODO: add a column to the right of the table. Data are in 'd'.
  
  //if(d.size() > nRows())
  assert(d.size() != 0);//?

  for(int i = 0; i < nRows(); i++)
    _table[i].addData(d[i]);
  /*if(d.size() < _nrows){//也是可以寫在dbcmd
    for(int i = d.size(); i < _nrows; i++)
      _data.push_back(NOO); 
  }*/
}

void
DBTable::delRow(int c)
{
   // TODO: delete row #c. Note #0 is the first row.
    _table.erase(_table.begin() + c);
}

void
DBTable::delCol(int c)
{
   // delete col #c. Note #0 is the first row.
   for (int i = 0; i < nRows(); ++i)
      _table[i].removeCell(c);
}

// For the following getXXX() functions...  (except for getCount())
// - Ignore null cells
// - If all the cells in column #c are null, return NAN
// - Return "float" because NAN is a float.
float
DBTable::getMax(size_t c) const
{
   // TODO: get the max data in column #c
  int Max = INT_MIN;
  int counter = 0;
  
  for(int i = 0; i < nRows(); ++i){    
    if(_table[i][c] == NOO)
      counter++;
    else if(_table[i][c] > Max && _table[i][c] != NOO) 
      Max = _table[i][c];  
    }

  if(counter == nRows())
    return NAN;

  return (float)Max;
}

float
DBTable::getMin(size_t c) const
{
   // TODO: get the min data in column #c
  int Min = INT_MAX;
  int counter = 0;
   
  for(int i = 0; i < nCols(); ++i){
    if(_table[i][c] == NOO) 
      counter++;
    else if(_table[i][c] < Min && _table[i][c] != NOO)
      Min = _table[i][c];
  }
  if(counter == nRows())
    return NAN;

  return (float)Min;
}

float 
DBTable::getSum(size_t c) const
{
   // TODO: compute the sum of data in column #c
  int sum = 0;
    
  for(int i = 0; i < nRows(); ++i)
    if(_table[i][c] != NOO)
      sum += _table[i][c];
  return (float)sum;
}

int
DBTable::getCount(size_t c) const
{
   // TODO: compute the number of distinct data in column #c
   // - Ignore null cells
  int Count = 0;
  for(int i = 0; i < nRows(); i++){
    bool keke = 0;//diffrenet
    for(int j = 0; j < i; j++)
      if(_table[i][c]==_table[j][c] || _table[i][c]== NOO)
        keke = 1;
    if(keke == 0) Count++;
  }
  return Count;
}

float
DBTable::getAve(size_t c) const
{
   // TODO: compute the average of data in column #c
  int num = 0;
  for(int i = 0; i < nRows(); i++)
     if(_table[i][c] != NOO)
        num++;
  return (float) (getSum(c) / num);
}

void
DBTable::sort(const struct DBSort& s)
{
   // TODO: sort the data according to the order of columns in 's'
    std::sort(_table.begin(), _table.end(), s);
}

void
DBTable::printCol(size_t c) const
{
   // TODO: to print out a column.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
  for (int i = 0; i < nRows(); i++)
    if(_table[i][c] == NOO) cout << setw(6) << right << ".";
    else cout << setw(6) << right << _table[i][c];
}

void
DBTable::printSummary() const
{
   size_t nr = nRows(), nc = nCols(), nv = 0;
   for (size_t i = 0; i < nr; ++i)
      for (size_t j = 0; j < nc; ++j)
         if (_table[i][j] != INT_MAX) ++nv;
   cout << "(#rows, #cols, #data) = (" << nr << ", " << nc << ", "
        << nv << ")" << endl;
}

