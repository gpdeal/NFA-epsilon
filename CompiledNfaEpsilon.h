#pragma once

#include <unordered_map>
#include <list>
#include <unordered_set>
#include <iostream>
#include "TransKey.h"
#include "FiniteStateMachine.h"

using namespace std;

class CompiledNfaEpsilon {


public:

   CompiledNfaEpsilon(FiniteStateMachine&);
   ~CompiledNfaEpsilon(void);

   void printStates() const;

   bool evaluate(string);

private:
   unordered_map<TransKey, list<int>*> transMap;
   unordered_set<int> currentNodes;
   unordered_set<int> goalNodes;
   int startNode;

   void traverseEpsilons(int);
   bool isInEndState() const;
   void repopulateCurrentNodes(const list<int>&);
   void followTransitions(list<int>&, char);

};

