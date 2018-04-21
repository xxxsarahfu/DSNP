/****************************************************************************
  FileName     [ myHashSet.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashSet ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_HASH_SET_H
#define MY_HASH_SET_H

#include <vector>

using namespace std;

//---------------------
// Define HashSet class
//---------------------
// To use HashSet ADT,
// the class "Data" should at least overload the "()" and "==" operators.
//
// "operator ()" is to generate the hash key (size_t)
// that will be % by _numBuckets to get the bucket number.
// ==> See "bucketNum()"
//
// "operator ()" is to check whether there has already been
// an equivalent "Data" object in the HashSet.
// Note that HashSet does not allow equivalent nodes to be inserted
//
template <class Data>
class HashSet
{
public:
   HashSet(size_t b = 0) : _numBuckets(0), _buckets(0) { if (b != 0) init(b); }
   ~HashSet() { reset(); }

   // TODO: implement the HashSet<Data>::iterator
   // o An iterator should be able to go through all the valid Data
   //   in the Hash
   // o Functions to be implemented:
   //   - constructor(s), destructor
   //   - operator '*': return the HashNode
   //   - ++/--iterator, iterator++/--
   //   - operators '=', '==', !="
   //
   class iterator
   {
      friend class HashSet<Data>;

  public:
    iterator(vector<Data>* b = 0, size_t n = 0, size_t bp = 0, size_t id = 0)
      : _buck(b), _numBuckets(n), _buckPos(bp), _idx(id) {}
    iterator(const iterator& i) 
      : _buck(i._buck), _numBuckets(i._numBuckets), _buckPos(i._buckPos), _idx(i._idx) {} //把括號裡的東東初始化成它
    ~iterator() {}

    const Data& operator * () const { return _buck[_buckPos][_idx]; }
    Data& operator * () { return _buck[_buckPos][_idx]; }

    iterator& operator ++ (){ 
      //cout << "Doing ++" << endl;
      if(_buckPos == _numBuckets) return (*this);
      if(_idx < _buck[_buckPos].size() - 1){ _idx++; return *(this); }  
      else{
        _buckPos++; //_buck++      //_buck++的意義在哪啊
        while(_buckPos <= _numBuckets ){ 
          if(_buck[_buckPos].size() != 0 || _buckPos == _numBuckets) break;
            _buckPos++; //_buck++;
        }
        _idx = 0;
      }
      //cout << "after ++ : " << endl;
      //cout << "\t" << _buckPos << "\t" << _idx << endl;
      return *(this);
    }

    iterator operator ++ (int){ 
      iterator temp = (*this);
      ++(*this);
      return temp; 
    }
    iterator& operator -- (){ 
      if(_idx > 0){ _idx--; return *(this); }
      else{ 
        _buckPos--; //_buck--;
        while(_buckPos >= 0) { 
          if(_buck[_buckPos].size() != 0) break;
            _buckPos--; //_buck--;
        }
        _idx = _buck[_buckPos].size() - 1;
      }
      return *(this);
    }
    iterator operator -- (int){ 
      iterator temp = (*this);
      --(*this);
      return temp; 
    }
    iterator& operator = (const iterator& i){ return (*this); }
    bool operator == (const iterator& i){ return ((i._buckPos == _buckPos) && (i._idx == _idx)); }
    bool operator != (const iterator& i){ return (((i._buckPos != _buckPos) || (i._idx != _idx))); }
   
  private:
    vector<Data>* _buck;
    size_t _numBuckets;
    size_t _buckPos, _idx; //[_buckPos][_idx]
    
  };

    void init(size_t b) { _numBuckets = b; _buckets = new vector<Data>[b]; }
    void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
    }
    void clear() {
      for (size_t i = 0; i < _numBuckets; ++i) _buckets[i].clear();
    }
    size_t numBuckets() const { return _numBuckets; }

    vector<Data>& operator [] (size_t i) { return _buckets[i]; }
    const vector<Data>& operator [](size_t i) const { return _buckets[i]; }

    // TODO: implement these functions
    //
    // Point to the first valid data
    iterator begin() const {  
       for(size_t i = 0; i < _numBuckets; i++)
    //for(size_t j = 0; j < _buckets[_numBuckets].size(); ++j)
      if(_buckets[i].size() != 0) return iterator(_buckets, _numBuckets, i, 0);
    //iterator it(_buckets, _numBuckets, 0, 0);
      //if(_buckets[0].size()!= 0) return it;
      //else return it++;
    }
    // Pass the end
    iterator end() const { 

      //for(size_t i = _numBuckets; i > 0; i--)
        //if(_buckets[i].size() != 0) 
          return iterator(_buckets, _numBuckets, _numBuckets, 0);
    }
    
    // return true if no valid data
    bool empty() const { return (begin() == end());}

    // number of valid data
    size_t size() const {
      size_t s = 0; 
      for(size_t i = 0; i < _numBuckets; i++)
        s += _buckets[i].size();
      return s; 
    }

    // check if d is in the hash...
    // if yes, return true;
    // else return false;
    bool check(const Data& d) const {
      size_t n = bucketNum(d);
      for(size_t i = 0; i < _buckets[n].size(); i++)
        if(_buckets[n][i] == d) return true;
      return false;
    }

    // query if d is in the hash...
    // if yes, replace d with the data in the hash and return true;
    // else return false;
    bool query(Data& d) const { 
      size_t n = bucketNum(d);
      for(size_t i = 0; i < _buckets[n].size(); i++)
        if(_buckets[n][i] == d) { d = _buckets[n][i]; return true;}
      return false;
      
    }

    // update the entry in hash that is equal to d (i.e. == return true)
    // if found, update that entry with d and return true;
    // else insert d into hash as a new entry and return false;
    bool update(const Data& d) { 
      size_t n = bucketNum(d);
      if(!check(d)){ _buckets[n].push_back(d); return false; }
      for(size_t i = 0; i < _buckets[n].size(); ++i)
        if(_buckets[n][i] == d) {_buckets[n][i] = d; return true;}
    }

    // return true if inserted successfully (i.e. d is not in the hash)
    // return false is d is already in the hash ==> will not insert
    bool insert(const Data& d) { 
      if(check(d)) return false;
      _buckets[bucketNum(d)].push_back(d);
     // cout << "Bucket[" << bucketNum(d) << "] has Data : " << endl;
      //for (size_t i = 0; i < _buckets[bucketNum(d)].size(); ++i)
        //cout << "\t" << _buckets[bucketNum(d)][i] << endl;
      //cout << "after ++: " << *(begin()++) << endl; 
      //cout << "Begin is : " << *(begin()) << endl;
      //cout << "End   is : " << *end() << endl;
      return true; 
    }

    // return true if removed successfully (i.e. d is in the hash)
    // return fasle otherwise (i.e. nothing is removed)
    bool remove(const Data& d) { 
      if(!check(d)) return false; 
      for(size_t i = 0; i < _buckets[bucketNum(d)].size(); i++)
        if(_buckets[bucketNum(d)][i] == d) {
          _buckets[bucketNum(d)].erase(_buckets[bucketNum(d)].begin()+i); 
          break;
      }
      return true;
    }

    private:
    // Do not add any extra data member
    size_t            _numBuckets;
    vector<Data>*     _buckets;

    size_t bucketNum(const Data& d) const {
      return (d() % _numBuckets); }
};

#endif // MY_HASH_SET_H
