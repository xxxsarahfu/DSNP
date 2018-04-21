#ifndef P2_TABLE_H
#define P2_TABLE_H

#include <vector>
#include <string>

using namespace std;

class Row
{
   //friend class Table;
public:
   const int operator[] (size_t i) const { //row裡第i個element，data = 一行row裡的資料
   		return _data[i];
   }
   int& operator[] (size_t i) { return _data[i]; } // TODO
   Row(int _ncols){_data = new int[_ncols];}


private:
   int  *_data;
};

class Table
{
public:
   const Row& operator[] (size_t i) const//改~~ 第i條row // size_table = rows shu!!
   {return _rows[i];}
   Row& operator[] (size_t i){return _rows[i];}
   bool read(const string&);
   void print();
   int sum(int);
   double avg(int);
   int max(int);
   int min(int);
   int count(int);
   void add();
   int _nrows;
   int _ncols;


private:
   vector<Row>  _rows;//vector 裡存好多條row

};

#endif // P2_TABLE_H
