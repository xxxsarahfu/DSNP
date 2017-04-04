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
// "operator ==" is to check whether there has already been
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
         : _buck(b), _numBuckets(n), _pos(bp), _idx(id) {}
      iterator(const iterator& i)
         : _buck(i._buck), _numBuckets(i._numBuckets), _pos(i._pos), _idx(i._idx) {}
      ~iterator() {}

      const Data& operator * () const { return _buck[_pos][_idx]; }
      Data& operator * () { return _buck[_pos][_idx]; }

      iterator& operator ++() {
         if (_pos == _numBuckets) return *this;
         else if (_idx < _buck[_pos].size() - 1) ++_idx;
         else {
            while (++_pos < _numBuckets) {
               if (_buck[_pos].size() != 0) break;
               ++_pos;
            }
            _idx = 0;
         }
         return *this;
      }

      iterator operator ++(int) {
         iterator temp = *this;
         ++(*this);
         return temp;
      }

      iterator& operator --() {
         if (_idx > 0) --_idx;
         else {
            while (--_pos >= 0) {
               if (_buck[_pos].size() != 0) break;
            }
            _idx = _buck[_pos].size() - 1;
         }
         return *this;
      }

      iterator operator --(int) {
         iterator temp = *this;
         --(*this);
         return temp;
      }

      iterator& operator = (const iterator& i) { return *this; }
      bool operator == (const iterator& i) { return (i._pos == _pos) && (i._idx == _idx);}
      bool operator != (const iterator& i) { return (i._pos != _pos) && (i._idx != _idx);}//?

   private:
      vector<Data>* _buck;
      size_t _numBuckets;
      size_t _pos, _idx;
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
      for (size_t i = 0; i < _numBuckets; i++) {
         if (_buckets[i].size()) return iterator(_buckets, _numBuckets, i, 0);
      }
      return end();
   }
// Pass the end
   iterator end() const { return iterator(_buckets, _numBuckets, _numBuckets, 0); }
// return true if no valid data
   bool empty() const { return begin() == end(); }
// number of valid data
   size_t size() const {
      size_t s = 0;
      for (iterator i = begin(); i != end(); ++i) ++s;
      return s;
   }

// check if d is in the hash...
// if yes, return true;
// else return false;
   bool check(const Data& d) const {
      size_t n = bucketNum(d);
      for (size_t i = 0; i < _buckets[n].size(); i++) 
         if (_buckets[n][i] == d) return true;
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
    for(size_t i = 0; i < _buckets[n].size(); ++i){
      if(_buckets[n][i] == d) {_buckets[n][i] = d; return true;}
    }
    return false;
  }

// return true if inserted successfully (i.e. d is not in the hash)
// return false is d is already in the hash ==> will not insert
   bool insert(const Data& d) {
      if (check(d)) return false;
      _buckets[bucketNum(d)].push_back(d);
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
      return (d() % _numBuckets);
   }
};

#endif // MY_HASH_SET_H
