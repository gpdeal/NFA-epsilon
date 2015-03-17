//-----------------------------------------------------------------------------
// Galen Deal
// CSS 502
//
// CompiledDfa
//
// Header for CompiledDfa class. A CompiledDfa takes a FiniteStateMachine object
// at construction and creates an internal representation of that machine which
// can then evaluate strings.
//-----------------------------------------------------------------------------


#pragma once

#include <unordered_map>
#include <list>
#include <unordered_set>
#include <iostream>
#include "TransKey.h"
#include "FiniteStateMachine.h"


class CompiledDfa {
public:
   
   // Constructs a CompiledDfa corresponding to the passed FiniteStateMachine
   CompiledDfa(FiniteStateMachine);
   ~CompiledDfa(void);

   // Returns true if the passed string satisfies the DFA, and false otherwise
   bool evaluate(string);


private:
   unordered_map<TransKey, int> transMap;
   unordered_set<int> goalNodes;
   int startNode;
   int currentNode;
};

