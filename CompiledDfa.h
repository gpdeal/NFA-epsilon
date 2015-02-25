#pragma once

#include <unordered_map>
#include <list>
#include <unordered_set>
#include <iostream>
#include "TransKey.h"
#include "FiniteStateMachine.h"


class CompiledDfa {
public:
   CompiledDfa(FiniteStateMachine);
   ~CompiledDfa(void);

   bool evaluate(string);


private:
   unordered_map<TransKey, int> transMap;
   unordered_set<int> goalNodes;
   int startNode;
   int currentNode;



};

