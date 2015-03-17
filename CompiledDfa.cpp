//-----------------------------------------------------------------------------
// Galen Deal
// CSS 502
//
// CompiledDfa
//
// Function definitions for CompiledDfa class.

#include "CompiledDfa.h"


CompiledDfa::CompiledDfa(FiniteStateMachine dfa) {
   for (list<Transition>::iterator transition = dfa.transitions.begin();
      transition != dfa.transitions.end(); transition++) {

         TransKey key(transition->source, transition->transitionChar);
         transMap[key] = transition->destination;
   }

   startNode = dfa.startNode;

   for (auto node = dfa.goalNodes.begin(); node != dfa.goalNodes.end(); node++) {
      goalNodes.insert(*node);
   }   
}


CompiledDfa::~CompiledDfa(void) {

}


// evaluate
//
// Accepts a string and applies it to the DFA, checking for validity. Returns
// true if the DFA evaluates the string as valid and false otherwise.

bool CompiledDfa::evaluate(string input) {
   currentNode = startNode;

   for (string::iterator currentChar = input.begin(); 
      currentChar != input.end(); currentChar++) {
         TransKey key(currentNode, *currentChar);

         if (transMap.count(key) == 0) {
            return false;
         } else {
            currentNode = transMap[key];
         }
   }

   return goalNodes.count(currentNode) == 1;
}
