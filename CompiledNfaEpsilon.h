//-----------------------------------------------------------------------------
// Galen Deal
// CSS 502
//
// CompiledNfaEpsilon
//
// Header for CompiledNfaEpsilon class. A CompiledNfaEpsilon takes a
// FiniteStateMachine object at construction and creates an internal
// representation of that machine which can then evaluate strings or be converted
// into a CompiledDfa.

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
   
   // Constructs a CompiledNfaEpsilon corresponding to the passed
   // FiniteStateMachine
   CompiledNfaEpsilon(const FiniteStateMachine&);
   ~CompiledNfaEpsilon(void);

   // Returns true if the passed string satisfies the NFA, and false otherwise
   bool evaluate(string);

   // Accepts a FiniteStateMachine representing an NFA and converts it to a
   // FiniteStateMachine representing an equivalent DFA which is then returned
   static FiniteStateMachine nfaToDfa(const FiniteStateMachine&);

   // Returns a FiniteStateMachine object representing a DFA which is equivalent
   // to the NFA represented by the CompiledNfaEpsilon object
   FiniteStateMachine convertToDfa();

private:
   unordered_map<TransKey, list<int>*> transMap;
   unordered_set<int> currentNodes;
   unordered_set<int> goalNodes;
   int startNode;

   void addTransitionToMap(const TransKey&, const Transition&);

   // Causes the object to occupy the full epsilon enclosure of the passed node
   void traverseEpsilons(int);

   list<int>* getEpsilonTransitions(int);

   void processDestinationNodes(list<int>*, queue<int>&, unordered_set<int>&);

   // Populates the CurrentNodes set with the ints in the passed list
   void repopulateCurrentNodes(const list<int>&);
   // Causes the object to traverse any transitions on the passed character that
   // exist in any of the currently occupied nodes
   void applyCharacter(char);

   void followTransitions(const TransKey&, list<int>&);
   // Sets the set of current nodes to the ints in the passed set
   void setCurrentState(const set<int>&);

   // Returnts true if any of the currently occupied nodes is an end state, and
   // false otherwise
   bool isInEndState() const;
   // Returns a set of ints corresponding to the currently occupied nodes
   set<int> getCurrentNodes() const;
   // Returns a set of chars representing all characters used in transitions by
   // the NFA
   set<char> getAlphabet() const;

};

