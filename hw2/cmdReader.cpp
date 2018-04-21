/****************************************************************************
  FileName     [ cmdReader.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command line reader member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <cstring>
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();
char mygetc(istream&);
ParseChar getChar(istream&);


//----------------------------------------------------------------------
//    Member Function for class Parser
//----------------------------------------------------------------------
void
CmdParser::readCmd()
{
   if (_dofile.is_open()) {
      readCmdInt(_dofile);
      _dofile.close();
   }
   else
      readCmdInt(cin);
}

void
CmdParser::readCmdInt(istream& istr)
{
   resetBufAndPrintPrompt();

   while (1) {
      ParseChar pch = getChar(istr);
      if (pch == INPUT_END_KEY) break;
      switch (pch) {
         case LINE_BEGIN_KEY :
         case HOME_KEY       : moveBufPtr(_readBuf); break;
         case LINE_END_KEY   :
         case END_KEY        : moveBufPtr(_readBufEnd); break;
         case BACK_SPACE_KEY : /* TODO */ if(_readBufPtr!= _readBuf) {
                                            moveBufPtr(_readBufPtr - 1);deleteChar();}else{mybeep();break;}  /*deleteLine()*/;break;
         case DELETE_KEY     : deleteChar(); break;
         case NEWLINE_KEY    : addHistory();
                               cout << char(NEWLINE_KEY);
                               resetBufAndPrintPrompt(); break;
         case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1); break;
         case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1); break;
         case ARROW_RIGHT_KEY: /* TODO */ moveBufPtr(_readBufPtr + 1); break;
         case ARROW_LEFT_KEY : /* TODO */ moveBufPtr(_readBufPtr - 1); break;
         case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
         case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
         case TAB_KEY        : /* TODO */ for(int i = (_readBufPtr - _readBuf); i < TAB_POSITION; i++) insertChar(' ', 1); 
         break;
         case INSERT_KEY     : //cout << "\n the sz of his " << _history.size();
                               //cout << "\n the hisidx is " << _historyIdx;
                               //resetBufAndPrintPrompt(); break; 

         // not yet supported; fall through to UNDEFINE
         case UNDEFINED_KEY:   mybeep(); break;
         default:  // printable character
            insertChar(char(pch)); break;
      }
      #ifdef TA_KB_SETTING
      taTestOnly();
      #endif
   }
}


// This function moves _readBufPtr to the "ptr" pointer
// It is used by left/right arrowkeys, home/end, etc.
//
// Suggested steps:
// 1. Make sure ptr is within [_readBuf, _readBufEnd].
//    If not, make a beep sound and return false. (DON'T MOVE)
// 2. Move the cursor to the left or right, depending on ptr
// 3. Update _readBufPtr accordingly. The content of the _readBuf[] will
//    not be changed
//
// [Note] This function can also be called by other member functions below
//        to move the _readBufPtr to proper position.
bool
CmdParser::moveBufPtr(char* const ptr)
{
   // TODO...
  if (ptr <= _readBufEnd && ptr >= _readBuf){
  
    while(_readBufPtr < ptr){ cout << *_readBufPtr; ++_readBufPtr; }//直接cout下一個！加＊是_readbufPtr的value
    while(_readBufPtr > ptr){ cout << '\b'; --_readBufPtr; }
    //_readBufPtr = ptr;//?
    return true;
  }
  else{ mybeep(); return false; }
}


// [Notes]
// 1. Delete the char at _readBufPtr
// 2. mybeep() and return false if at _readBufEnd
// 3. Move the remaining string left for one character
// 4. The cursor should stay at the same position
// 5. Remember to update _readBufEnd accordingly.
// 6. Don't leave the tailing character.
// 7. Call "moveBufPtr(...)" if needed.
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteChar()---
//
// cmd> This is he command
//              ^
//
bool
CmdParser::deleteChar()
{
   // TODO...
  if(_readBufPtr != _readBufEnd){
    //cout << '\b'; cout << " ";

    for(int i = (_readBufPtr - _readBuf) ; i < int(_readBufEnd - _readBuf) ; i++){
      _readBuf[i] = _readBuf[i+1];
    }
    
    char *precur;
    precur = _readBufPtr;
    moveBufPtr(_readBufEnd);//cursor move to end
    cout << " ";//看我一個把結尾洗掉 <- 87
    _readBufEnd--; //  update?
    *_readBufEnd = 0;
    moveBufPtr(precur);// cursor stay?

    return true;
  }
  else { mybeep(); return false; }
}

// 1. Insert character 'ch' for "repeat" times at _readBufPtr
// 2. Move the remaining string right for "repeat" characters
// 3. The cursor should move right for "repeats" positions afterwards
// 4. Default value for "repeat" is 1. You should assert that (repeat >= 1).
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling insertChar('k', 3) ---
//
// cmd> This is kkkthe command
//                 ^
//



void
CmdParser::insertChar(char ch, int repeat)

{
   // TODO...
 
  moveBufPtr(_readBufPtr);
  assert(repeat >= 1);
  //_readBufEnd += repeat;
  repeat = 1;//暫時滴
  _readBufEnd++;

  for(int i = _readBufEnd - _readBuf; i > int(_readBufPtr - _readBuf); i--){
    _readBuf[i] = _readBuf[i - 1];
  }

  char *precur = _readBufPtr;
  _readBuf[int(_readBufPtr - _readBuf)] = ch;
  moveBufPtr(precur);
  moveBufPtr(_readBufEnd);
  moveBufPtr(precur + 1);


}




// 1. Delete the line that is currently shown on the screen
// 2. Reset _readBufPtr and _readBufEnd to _readBuf
// 3. Make sure *_readBufEnd = 0
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteLine() ---
//
// cmd>
//      ^
//
void
CmdParser::deleteLine()
{
   // TODO...
  //moveBufPtr(_readBufPtr);

  for (int i = 0; i < int(_readBufPtr - _readBuf); ++i)
    cout << '\b';
  for (int i = 0; i < int(_readBufEnd - _readBuf); ++i)
    {cout << ' '; _readBuf[i] = 0;}
  for(int i = 0; i < int(_readBufEnd - _readBuf); ++i)
    cout << '\b';

  _readBufPtr = _readBufEnd = _readBuf;  
  *_readBufEnd = 0;
}


// This functions moves _historyIdx to index and display _history[index]
// on the screen.
//
// Need to consider:
// If moving up... (i.e. index < _historyIdx)
// 1. If already at top (i.e. _historyIdx == 0), beep and do nothing.
// 2. If at bottom, temporarily record _readBuf to history.
//    (Do not remove spaces, and set _tempCmdStored to "true")
// 3. If index < 0, let index = 0.
//
// If moving down... (i.e. index > _historyIdx)
// 1. If already at bottom, beep and do nothing
// 2. If index >= _history.size(), let index = _history.size() - 1.
//
// Assign _historyIdx to index at the end.
//
// [Note] index should not = _historyIdx
//

void
CmdParser::moveToHistory(int index)
{
  //cerr << "\nhis "  << _historyIdx << endl;
  //cerr << "idx " << index << endl;
  //cerr << "sz " << (int)_history.size() << endl;
  //cerr << "ptr " << (int)(_readBufPtr-_readBuf) << endl;
  //cerr << "end " << (int)(_readBufEnd-_readBuf) << endl;
   // TODO... 



  assert(index != _historyIdx);

  //move up
  if(index < _historyIdx){
    if(!_historyIdx){ mybeep(); return; }
    if(index < 0) index = 0;
    if(_historyIdx == _history.size()){_history.push_back(_readBuf);_tempCmdStored = true;}
    _historyIdx = index;
    retrieveHistory();
  }
  if(index > _historyIdx){
    if(_historyIdx == _history.size()){ mybeep(); return;}
    if(index >= _history.size()) index = _history.size() - 1;

    _historyIdx = index;
    retrieveHistory();

    if(_historyIdx == _history.size() - 1 && _tempCmdStored == true){
      _history.pop_back();
      _tempCmdStored = false;
    }
  }




//////////////////////////////////
//ver2//
/*
  if( index < _historyIdx && !_historyIdx ) {
    mybeep();
    return;
  }
  if( index < 0 ) index = 0;
  if(_historyIdx == _history.size() &&  _tempCmdStored == false){
    _history.push_back(_readBuf);
    _tempCmdStored = true;
  }

  if( index >= (int)_history.size() ) index = _history.size() - 1;
  _historyIdx = index;
  retrieveHistory();
  if( index == (int)_history.size()-1 ) {
      _history.pop_back();
      _tempCmdStored = false;
      //mybeep();
  }
  if( _historyIdx == _history.size() && _tempCmdStored == false){mybeep();}
*/
  moveBufPtr(_readBufEnd);
}

  
  //


// This function adds the string in _readBuf to the _history.
// The size of _history may or may not change. Depending on whether 
// there is a temp history string.
//
// 1. Remove ' ' at the beginning and end of _readBuf ok
// 2. If not a null string, add string to _history. 
//    Be sure you are adding to the right entry of _history.
// 3. If it is a null string, don't add anything to _history.
// 4. Make sure to clean up "temp recorded string" (added earlier by up/pgUp,
//    and reset _tempCmdStored to false
// 5. Reset _historyIdx to _history.size() // for future insertion ok
//
void
CmdParser::addHistory()
{
  // TODO...
  //if(_readBuf == NULL){return;}
  //if(*_readBuf != 0){
  int begstring = -1; int endstring = 0;
  string newaddstring;
  //if((_readBuf != _readBufPtr)|| (_readBufEnd != _readBufPtr)){
  
  for (int i = 0; i < int(_readBufEnd - _readBuf); i++){
    if(_readBuf[i] != ' '){
      begstring = i;  //紀錄開頭
      break;
    }
  }

  if(begstring == -1) {
    if(_tempCmdStored == true) _history.pop_back();
  } 
  else {
  for (int i = _readBufEnd - _readBuf - 1; i >= begstring; i--){
    if(_readBuf[i] != ' '){
      endstring = i;  //記錄結尾
      break;
    }
  }

 //cerr << begstring << " " << endstring << " ";
 for(int i = begstring; i <= endstring; i++)
    newaddstring += _readBuf[i];
  //newaddstring += '\0';
  //cerr << "\"" << newaddstring << "\" ";
  

  //clear temp
  if(_history.size() > 0 && _tempCmdStored == true){
    _history.pop_back(); _tempCmdStored = false; 

  }
  
 
  //cerr << "new added: " << newaddstring << endl;
  _history.push_back(newaddstring);
  //_history.size()++;
  //}
  }
  _historyIdx = _history.size();
  _readBufPtr = _readBufEnd = _readBuf;
  *_readBufEnd = 0;
}



// 1. Replace current line with _history[_historyIdx] on the screen
// 2. Set _readBufPtr and _readBufEnd to end of line
//
// [Note] Do not change _history.size().
//
void
CmdParser::retrieveHistory()
{
  deleteLine();
  strcpy(_readBuf, _history[_historyIdx].c_str());
  cout << _readBuf;

  _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}
