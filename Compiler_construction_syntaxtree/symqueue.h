#ifndef _SYMSTACK_H_
#define _SYMSTACK_H_

#include <queue>
#include <string>
#include "types.h"

class SymQ {
  public:
    SymQ(void){}

    ~SymQ(void){clear();}

    string top() {
      string temp = queue.front();
      queue.pop();
      return temp;
    }

    void push(string newSym) {
      queue.push(newSym);
    }

    void clear() {
      while(!queue.empty()) {
        queue.pop();
      }
    }

    bool isEmpty() {
      return queue.empty();
    }

    void setType(SymbolType t) {
      type = t;
    }

    SymbolType getType(){ return type;}
  private:
    std::queue<std::string> queue; //queue of strings to add
    SymbolType type;               //tracks whether we are adding params or vars
};

#endif
