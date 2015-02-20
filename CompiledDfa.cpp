#include "CompiledDfa.h"



/* initial pseudocode implementation:

CompiledDfa(FiniteStateMachine dfa) {
    for (Transition trans in dfa.transitions) {
        key = new TransKey(trans.source, trans.transitionChar);
                
        transMap.insert(trans.destination);
    }

    currentNodes.add(dfa.startNode);
    goalNodes = nfa.goalNodes;
}
*/
CompiledDfa::CompiledDfa(FiniteStateMachine dfa) {
   // build internal list of transitions
   for (list<Transition>::iterator it = dfa.transitions.begin();
      it != dfa.transitions.end(); it++) {

         TransKey* key = new TransKey(it->source, it->transitionChar);
         transMap[key] = it->destination;
   }

   currentNode = dfa.startNode;

   // build internal set of goal nodes
   for (auto it = dfa.goalNodes.begin(); it != dfa.goalNodes.end(); it++) {
      goalNodes.insert(*it);
   }   
}


CompiledDfa::~CompiledDfa(void) {

}



/* initial pseudocode implementation

bool evaluate(string input) {
    for (char character in input) {
        TransKey key(currentNode, character);
        
        if (!transMap.contains(key)) {
            return false;
        } else {
            currentNode = transMap[key];
        }
    }
    
    return goalNodes.contains(currentNode);
}
*/


bool CompiledDfa::evaluate(string input) {
   for (string::iterator it = input.begin(); it != input.end(); it++) {
      TransKey key(currentNode, *it);

      if (transMap.count(&key) == 0) {
         return false;
      } else {
         currentNode = transMap[&key];
      }
   }

   return goalNodes.count(currentNode) == 1;
}


