#include "p2Table.h"

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
//#include <stdlib.h>

using namespace std;

#define NOO 850121

// Implement member functions of class Row and Table here
bool Table::read(const string& csvFile)//csvfilename 
{
	ifstream file;
	file.open(csvFile,ios::in);//or "test_hw1"??
	if (file.is_open()){
	//ver1
		/*char datatmp[5000];
		file.getline(datatmp,5000)；//
		string datastring(datatmp);//ifstream data 

		_data = new int[s];//s = nums of cols    ？？？？？？
		s = n_cols(datastring);
		*/
	//ver2
		string line;

		while(getline(file,line, '\r')){
			if(line.empty()) break;
			stringstream ss(line);
			_ncols = std::count(line.begin(), line.end(), ',') + 1;//從line頭到尾 數有幾個‘,’ algorithm
			Row r(_ncols);//開好一個array 長度為_ncols
			//r._data = new int[_ncols]; 
			//_rows.push_back(r);

			string a;
			int i = 0;
			for(int j = 0; j < _ncols; j++){
				getline(ss, a, ',');
				if(a.empty()){ r[i++] = NOO; }//先做事後再加加？
				else {r[i++] = atoi(a.c_str()); }
				//r._data[i++] = ( a.empty() ? NOO : atoi(a.c_str()) );
				a.clear();//
			}
			_rows.push_back(r);
			line.clear();
		}
	
		_nrows = _rows.size();

	return true;
	}
	else return false;
}
// TODO



   //PRINT
void Table::print()
{
  for(size_t i = 0; i < _nrows; i++){//R = row zongshu  or table.size()?? i++
     for(size_t j = 0; j < _ncols; j++)//upper bound???  or int (table[i].size()) ?? j++
        if(_rows[i][j] == NOO) cout << right << setw(4) <<" ";
        else cout << right << setw(4) << _rows[i][j];
    cout << endl;}
  
}

//SUM columns
int Table::sum (int C)
{      
  int sum = 0;
  for(int i = 0; i < _nrows; i++)//?? R = row_count
     if(_rows[i][C] != NOO)
        sum += _rows[i][C];
  return sum;
}


//AVE
double Table::avg(int C)
{   
  int num = 0;
  for(int i = 0; i < _nrows; i++)
     if(_rows[i][C] != NOO)
        num++;
  return (double) sum(C) / num;
}


//MAX   
int Table::max(int C )
{//input ??
  int Max = -1000;
  for(int i = 0; i < _nrows; i++)//M row nums
     if(_rows[i][C]> Max && _rows[i][C] != NOO)
        //swap(Max,_rows[i][C]);
        Max = _rows[i][C];
  return Max;
}


//MIN 
int Table::min(int C)
{
  int Min = _rows[0][C];
  for(int i = 0; i < _nrows; i++)
     if(_rows[i][C] < Min)
        //swap(Min,_rows[i][C]);
        Min = _rows[i][C];
  return Min;
}

 
//COUNT
int Table::count(int C)
{
  int Count = 0;
  for(int i = 0; i < _nrows; i++){
  	bool keke = 0;//diffrenet
  	for(int j = 0; j < i; j++)
  		if(_rows[i][C]==_rows[j][C] || _rows[i][C]== NOO)
  			keke = 1;
  	if(keke == 0) Count++;
  }
  return Count;
}

//ADD
void Table::add()
{
	Row r(_ncols);
	string ain;
	for( int i = 0 ; i < _ncols ; i++ ){
		cin >> ain;
		if(ain == "-") r[i] = NOO;
		else r[i] = atoi(ain.c_str());
	}
   _nrows++;
   _rows.push_back(r);
  
}

