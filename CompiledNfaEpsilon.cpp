#pragma once

#include <unordered_map>
#include <list>
#include <unordered_set>
#include <iostream>
#include <queue>
#include <list>
#include "TransKey.h"
#include "FiniteStateMachine.h"
#include "NodeSet.h"

using namespace std;

class CompiledNfaEpsilon {


public:

   CompiledNfaEpsilon(FiniteStateMachine&);
   ~CompiledNfaEpsilon(void);

   void printStates() const;

   bool evaluate(string);

   FiniteStateMachine convertToDfa();

private:
   unordered_map<TransKey, list<int>*> transMap;
   unordered_set<int> currentNodes;
   unordered_set<int> goalNodes;
   int startNode;

   void traverseEpsilons(int);
   void repopulateCurrentNodes(const list<int>&);
   void applyCharacter(char);
   void setCurrentState(const set<int>&);

   bool isInEndState() const;
   set<int> getCurrentNodes() const;
   set<char> getAlphabet() const;

};

