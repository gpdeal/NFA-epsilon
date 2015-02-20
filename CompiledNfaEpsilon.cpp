#include "CompiledNfaEpsilon.h"

using namespace std;


/* pseudocode implementation:

CompiledNfaEpsilon(FiniteStateMachine nfa) {
    for (trans in nfa.transitions) {
        key = new TransKey(trans.source, trans.transitionChar);
        
        if (transMap[key] == nullptr) {
            transMap[key] = new list<int>;
        }
        
        transMap[key].insert(trans.destination);
    }

    currentNodes.add(nfa.startNode);
    traverseEpsilons(nfa.startNode);
}
*/

CompiledNfaEpsilon::CompiledNfaEpsilon(FiniteStateMachine& nfa) {
   
   // for each item in nfa's list of transitions
   for (list<Transition>::iterator it = nfa.transitions.begin(); 
      it != nfa.transitions.end(); it++) {

      // create key from source and transition character
      TransKey* key = new TransKey(it->source, it->transitionChar);
            
      // if no transitions from that node on that character exist yet, create a
      // new list to hold any such transitions

      /*
      FIRST ATTEMPT using operator[key]

      list<int>* destList = transMap[key];
      if (destList == nullptr) {
         destList = new list<int>;
      }
      list<int>* destList2 = transMap[key];*/

      
      // SECOND ATTEMPT using count(key)
      
      list<int>* destList;
      if (transMap.count(key) == 0) {
         destList = new list<int>;
         transMap[key] = destList;
      } else {
         destList = transMap[key];
      }

      // add the transition to the list in the map
      destList->push_front(it->destination);
      int x;
      x = 5;
   }
   
   currentNodes.insert(nfa.startNode);
   traverseEpsilons(nfa.startNode);
}


CompiledNfaEpsilon::~CompiledNfaEpsilon(void) {
}



/* pseudocode implementation:

void traverseEpsilons(int node) {
    TransKey key(node, 0);
    list transList = transMap[key];
    
    if (transList != nullptr) {
        for (node in transList) {
            currentNodes.add(node);
            traverseEpsilons(node);
        }
    }
}
*/


void CompiledNfaEpsilon::traverseEpsilons(int node) {
   TransKey key(node, 0);
   list<int>* transList = transMap[&key];
    
   if (transList != nullptr) {
      for (list<int>::iterator it = transList->begin(); 
         it !=transList->end(); it++) {

         currentNodes.insert(node);
         traverseEpsilons(node);
      }
   }
}


void CompiledNfaEpsilon::printStates() const {
   cout << "FSA's current states: ";

   for (auto it = currentNodes.begin(); it != currentNodes.end(); it++) {
      cout << *it << ", ";
   }

   cout << endl;
}

/* initial pseudocode implementation:

bool evaluate(string str) {
   
    for (char character in str) {
        list<int> newCurrentSet;
        
        for (int node in currentNodes) {
            currentNodes.remove(node);
            TransKey key(node, character)
            list<int> transList = transMap[key];
            
            for (int destination in transList) {
                newCurrentSet.insert(destination);
            }
        }
        
        for (int node in newCurrentSet) {
            if (!currentNodes.contains(node)) {
                currentNodes.insert(node);
                traverseEpsilons(node);
            }
            
        }
    }
   
    for (int node in goalNodes) {
        if (currentNodes.contains(node)) {
            return true;
        }
    }
    return false.
}
*/

bool CompiledNfaEpsilon::evaluate(string input) {
   for (string::iterator charIt = input.begin(); charIt != input.end(); charIt++) {
      list<int> newCurrentSet;

      for (auto nodeIt = currentNodes.begin(); nodeIt != currentNodes.end(); nodeIt++) {
         int node = *nodeIt;
         currentNodes.erase(node);
         TransKey key(node, *charIt);
         list<int>* destList = transMap[&key];

         for (list<int>::iterator destIt = destList->begin(); 
            destIt != destList->end(); destIt++) {
               newCurrentSet.push_front(*destIt);
         }
      }

      repopulateCurrentNodes(newCurrentSet);
   }

   return isInEndState();
}



bool CompiledNfaEpsilon::isInEndState() const {
   for (auto it = goalNodes.begin(); it != goalNodes.end(); it++) {
      if (currentNodes.count(*it) == 1) {
         return true;
      }
   }
   return false;
}



void CompiledNfaEpsilon::repopulateCurrentNodes(const list<int>& nodeList) {
   for (list<int>::iterator nodeIt = nodeList.begin(); 
      nodeIt != nodeList.end(); nodeIt++) {

         if (currentNodes.count(*nodeIt) == 0) {
            currentNodes.insert(*nodeIt);
            traverseEpsilons(*nodeIt);
         }
   }
}
