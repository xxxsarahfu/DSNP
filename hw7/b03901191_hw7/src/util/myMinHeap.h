/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
  MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
  ~MinHeap() {}

  void clear() { _data.clear(); }

  // For the following member functions,
  // We don't respond for the case vector "_data" is empty!
  const Data& operator [] (size_t i) const { return _data[i]; }   
  Data& operator [] (size_t i) { return _data[i]; }

  size_t size() const { return _data.size(); }

  // TODO
  const Data& min() const { return _data[0]; }
  void insert(const Data& d) { 
    /*_data.push_back(d);
    size_t pos = _data.size() - 1;
    while(_data[pos] < _data[(pos - 1) /2]) {
      swap(_data[pos], _data[(pos - 1) /2]);
      pos = (pos - 1) / 2;
    }*/
    _data.push_back(d);
    size_t pos = _data.size();
    while(pos > 1){
      size_t p = pos / 2;
      if((_data[p - 1] < d) || _data[p - 1].getLoad() == d.getLoad()) break;
      _data[pos - 1] = _data[p - 1];
      pos = p;
    }
    _data[pos - 1] = d;
  }
  void delMin() { 
    /*
  if(_data.size() == 1) delete _data[0];
  else{
    for(int i = 0; i < _data.size() - 1; i++) {
    _data[i] = _data[i+1]; 
    delete _data[_data.size() - 1];
    }
  }*/
    delData(0);
}
  void delData(size_t i) {
    //if(_data.empty()) break;
    Data last = _data.back();
    _data.pop_back();

    int p = i, t = 2 * p + 1; //p:pos, t:left child
    while(t < _data.size()){
      if(t < _data.size() - 1) //has right child
        if(_data[t+1] < _data[t]) ++t; //to the smaller child
      if((last < _data[t]) || last.getLoad() == _data[t].getLoad()) break;
      //swap(_data[p],_data[t]);
      _data[p] = _data[t];
      p = t;
      t = 2 * p + 1;
    }    
    t = (p - 1) / 2; 
    while(t > 0){
      if(last < _data[t]){
        //swap(_data[p],_data[t]);
        _data[p] = _data[t];
        p = t;
        t = (p - 1) / 2;
      }
      else break;
    }
    _data[p] = last;
/*
    int pos = (int)i;
    _data[pos] = _data.back();
    _data.pop_back();
    if(_data[pos] < _data[(pos- 1) / 2]){
      while(_data[pos] < _data[(pos - 1) /2]){
        swap(_data[pos],_data[pos - 1]);
        pos = (pos - 1) / 2;
      }
    }
    else{
      while (pos*2+1 < size()){
        if(_data[2 * pos + 1] < _data[pos] && ((_data[2 * pos + 1] < _data[ 2 * pos + 2]) 
          || _data[2 * pos + 1].getLoad() == _data[2 * pos + 2].getLoad())){
          swap(_data[pos], _data[2 * pos + 1]);
          pos = 2*pos + 1;
        }
        else if((_data[2 * pos + 2] < _data[pos]) && _data[2 * pos + 2] < _data[2 * pos + 1]){
          swap(_data[pos], _data[2 * pos + 2]);
          pos = 2*pos + 2;
        }
        else break;
      }
    }
*/
}
private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
