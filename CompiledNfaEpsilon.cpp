//-----------------------------------------------------------------------------
// Galen Deal
// CSS 502
//
// CompiledNfaEpsilon
//
// Function definitions for CompiledNfaEpsilon class.


#include "CompiledNfaEpsilon.h"

using namespace std;


// Constructor
//
// Accepts a FiniteStateMachine representing an NFA and builds an internal
// representation of that NFA.


/* initial pseudocode implementation:

CompiledNfaEpsilon(FiniteStateMachine nfa) {
    for (Transition trans in nfa.transitions) {
        key = new TransKey(trans.source, trans.transitionChar);
        
        if (transMap[key] == nullptr) {
            transMap[key] = new list<int>;
        }
        
        transMap.insert(trans.destination);
    }

    currentNodes.add(nfa.startNode);
    goalNodes = nfa.goalNodes;
    traverseEpsilons(nfa.startNode);
}
*/

CompiledNfaEpsilon::CompiledNfaEpsilon(const FiniteStateMachine& nfa) {
   
   // for each item in nfa's list of transitions
   for (auto it = nfa.transitions.cbegin(); it != nfa.transitions.cend(); it++) {

         // create key from source and transition character
         TransKey key(it->source, it->transitionChar);
            
         // if no transitions from that node on that character exist yet, create a
         // new list to hold any such transitions
         list<int>* destList;
         if (transMap.count(key) == 0) {
            destList = new list<int>;
            transMap[key] = destList;
         } else {
            destList = transMap[key];
         }

         // add the transition to the list in the map
         destList->push_front(it->destination);
   }
   
   startNode = nfa.startNode;
   for (auto it = nfa.goalNodes.begin(); it != nfa.goalNodes.end(); it++) {
      goalNodes.insert(*it);
   }
}

// Destructor
//
// Deletes all the pointer in the transMap map, which I believe are the only
// pieces of memory which would not otherwise be taken care of

CompiledNfaEpsilon::~CompiledNfaEpsilon(void) {
   for (auto it = transMap.begin(); it != transMap.end(); it++) {
      delete it->second;
   }
}


// traverseEpsilons
//
// Explores and occupies the full epsilon enclosure of the node represented
// by the passed int. If a given node is accessible to the passed node by 
// following only epsilon transitions, then, on completion, this method will 
// cause the CompiledNfaEpsilon to occupy that node.

/* initial pseudocode implementation:

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
   queue<int> nodeQueue;
   unordered_set<int> insertedToQueue;
   
   nodeQueue.push(node);
   insertedToQueue.insert(node);
   
   while (!nodeQueue.empty()) {
      int current = nodeQueue.front();
      nodeQueue.pop();

      // get the list of epsilon transitions ond the current node
      TransKey key(current, 0);
      list<int>* destinations;
      if (transMap.count(key) != 0) { destinations = transMap[key]; }
      else { destinations = nullptr; }

      if (destinations != nullptr) {
         for (auto it = destinations->begin(); it != destinations->end(); it++) {
            if (insertedToQueue.count(*it) == 0) {
               nodeQueue.push(*it);
               insertedToQueue.insert(*it);
            }
            currentNodes.insert(*it);
         }
      }
   }
}


// evaluate
//
// Accepts a string which is then applied to the NFA in order to check for
// validity. Returns true if the NFA evaluates the string as valid and false
// otherwise.

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
   currentNodes.insert(startNode);
   traverseEpsilons(startNode);

   // for each character in the input string
   for (string::iterator it = input.begin(); it != input.end(); it++) {
      applyCharacter(*it);
   }

   bool success = isInEndState();
   currentNodes.clear();   
   return success;
}


// applyCharacter
//
// Applies a single character to the current state of the NFA, following all
// transitions on the passed character from any currently occupied node, and
// then setting the set of currently occupied nodes to be only those reached by
// such transitions.

void CompiledNfaEpsilon::applyCharacter(char currentChar) {
   list<int> newCurrentNodes;
   
   auto it = currentNodes.begin();
   int setSize = currentNodes.size();
   // for each node that is currently occupied
   for (int i = 0; i < setSize; i++) {
      int node = *it;
      it++;

      currentNodes.erase(node);
      TransKey key(node, currentChar);
      
      // if there is a transition from the current node on the current character
      if (transMap.count(key) != 0) {
         list<int>* destList = transMap[key];
         // add each of the destination nodes in the list to the new list of
         // currently occupied nodes
         for (list<int>::iterator destIt = destList->begin(); 
            destIt != destList->end(); destIt++) {
               newCurrentNodes.push_front(*destIt);
         }
      }
   }
   repopulateCurrentNodes(newCurrentNodes);
}


// isInEndStates
//
// Checks to see if the NFA is currently occupying any nodes which are in the
// set of goal nodes. Returns true if it is and false otherwise.

bool CompiledNfaEpsilon::isInEndState() const {
   for (auto it = goalNodes.begin(); it != goalNodes.end(); it++) {
      if (currentNodes.count(*it) == 1) {
         return true;
      }
   }
   return false;
}


// repopulateCurrentNodes
//
// Accepts a list of ints and adds all of them to the set of currently occupied
// nodes.

void CompiledNfaEpsilon::repopulateCurrentNodes(const list<int>& nodeList) {
   for (auto nodeIt = nodeList.cbegin(); nodeIt != nodeList.cend(); nodeIt++) {

         if (currentNodes.count(*nodeIt) == 0) {
            currentNodes.insert(*nodeIt);
            traverseEpsilons(*nodeIt);
         }
   }
}

// nfaToDfa
//
// Static method which accepts a FiniteStateMachine object representing an NFA
// and returns a FiniteStateMachine object representing an equivalent DFA.

FiniteStateMachine CompiledNfaEpsilon::nfaToDfa(const FiniteStateMachine& nfa) {
   CompiledNfaEpsilon cnfa(nfa);
   return cnfa.convertToDfa();
}



// convertToDfa
//
// Creates a FiniteStateMachine object which represents a DFA which is
// equivalent to the internally stored NFA and returns it.

/* initial pseudocode implementation:

FiniteStateMachine convertToDfa() {
    FiniteStatMachine dfa;
    map<set<int>, int> dfaNodes;
    queue<set<int>> unprocessedNodes;
    list<char> alphabet = getAlphabet();
    int nodeCount;
    
    traverseEpsilons(startNode);
    
    set<int> node = getCurrentNodes();
    
    dfaNodes.insert(node, 1);
    unprocessedNodes.insert(node);
    dfa.nodes.insert(1);
    dfa.startNode = 1;
    if (isInEndState()) {
        dfa.goalNodes.insert(1);
    }
    nodeCount = 1;
    
    // for each possible set of states
    while (unprocessedNodes.size() > 0) {
        node = unprocessedNodes.dequeue();
        
        // for each character in the alphabet
        for (char character in alphabet) {
            setCurrentState(node.nfaNodeSet);
            applyCharacter(character);
            set<int> newNode = getCurrentNodes;
            
            // if a new set of states is encountered
            if (!dfaNodes.contains(newNode)) {
                nodeCount++;
                dfaNodes.insert(newNode, nodeCount);
                dfa.nodes.insert(nodeCount);
                if (isInEndState()) {
                    dfa.goalNodes.insert(nodeCount);
                }
                unprocessedNodes.enqueue(newNode);
            }

            // make new transition from starting node to destination node
            Transition newTrans(dfaNodes.retrieve(node), character, dfaNodes.retrieve(newNode));
            dfa.transitions.intert(newTrans);   
        }   
    }
}
*/

FiniteStateMachine CompiledNfaEpsilon::convertToDfa() {
   FiniteStateMachine dfa;
   unordered_map<NodeSet, int> dfaNodes;
   queue<NodeSet> unprocessedNodes;
   set<char> alphabet = getAlphabet();
   NodeSet nfaState;
   int nodeCount = 1;

   currentNodes.clear();
   currentNodes.insert(startNode);
   traverseEpsilons(startNode);

   NodeSet dfaStartNode(getCurrentNodes());

   // create DFA's starting node
   dfaNodes.insert(make_pair(dfaStartNode, 1));
   unprocessedNodes.push(dfaStartNode);
   dfa.nodes.insert(1);
   dfa.startNode = 1;
   if (isInEndState()) { dfa.goalNodes.insert(1); }
   nodeCount = 1;

   // for each possible set of states in the NFA
   while (unprocessedNodes.size() > 0) {
      nfaState = unprocessedNodes.front();
      unprocessedNodes.pop();

      // for each character in the alphabet
      for (auto it = alphabet.begin(); it != alphabet.end(); it++) {
         setCurrentState(nfaState.nodes);
         applyCharacter(*it);
         NodeSet newNode(getCurrentNodes());

         if (!newNode.nodes.empty()) {
            // if a new set of states is encountered, add it as a new DFA node
            if (dfaNodes.count(newNode) == 0) {
               nodeCount++;
               dfaNodes.insert(make_pair(newNode, nodeCount));
               dfa.nodes.insert(nodeCount);
               if (isInEndState()) { dfa.goalNodes.insert(nodeCount); }
               unprocessedNodes.push(newNode);
            }

            // create a new transition in the DFA
            Transition newTrans(dfaNodes[nfaState], *it, dfaNodes[newNode]);
            dfa.transitions.push_front(newTrans);
         }
      }
   }

   return dfa;
}


// setCurrentState
//
// Clears the set of currently occupied nodes and then sets its contents to be
// equal to the passed set of ints.

void CompiledNfaEpsilon::setCurrentState(const set<int>& nodes) {
   currentNodes.clear();

   for (auto it = nodes.cbegin(); it != nodes.end(); it++) {
      currentNodes.insert(*it);
   }
}

// getCurrentNodes
//
// Returns an ordered set of ints representing the currently occupied nodes.

set<int> CompiledNfaEpsilon::getCurrentNodes() const {
   set<int> nodes;

   for (auto it = currentNodes.cbegin(); it != currentNodes.cend(); it++) {
      nodes.insert(*it);
   }

   return nodes;
}


// getAlphabet
//
// Returns an ordered set of chars representing all of the characters which can
// trigger a valid transition in the internally stored NFA.

set<char> CompiledNfaEpsilon::getAlphabet() const {
   set<char> alphabet;
   
   for (auto it = transMap.cbegin(); it != transMap.cend(); it++) {
      alphabet.insert(it->first.transitionChar);
   }

   return alphabet;
}
