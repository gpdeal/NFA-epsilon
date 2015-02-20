#pragma once

#include <set>
#include <list>
#include "Transition.h"

using namespace std;

struct FiniteStateMachine {
   set<int> nodes;
   int startNode;
   set<int> goalNodes;
   list<Transition> transitions;
};
