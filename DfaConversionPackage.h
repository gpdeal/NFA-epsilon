#pragma once

#include <list>
#include <unordered_map>
#include <queue>
#include <set>
#include "NodeSet.h"
#include "FiniteStateMachine.h"


using namespace std;

struct DfaConversionPackage {
   FiniteStateMachine dfa;
   unordered_map<NodeSet, int> dfaNodes;
   queue<NodeSet> unprocessedNodes;
   set<char> alphabet;
   NodeSet nfaState;
   int nodeCount;

   DfaConversionPackage(set<char> input) {
      alphabet = input;
      nodeCount = 0;
   }


};
