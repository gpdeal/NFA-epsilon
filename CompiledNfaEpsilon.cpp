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

CompiledNfaEpsilon::CompiledNfaEpsilon(const FiniteStateMachine& nfa) {
   
   for (auto transition = nfa.transitions.cbegin(); 
      transition != nfa.transitions.cend(); transition++) {

         TransKey key(transition->source, transition->transitionChar);            
         addTransitionToMap(key, *transition);
   }
   
   startNode = nfa.startNode;

   for (auto node = nfa.goalNodes.begin(); node != nfa.goalNodes.end(); node++) {
      goalNodes.insert(*node);
   }
}

void CompiledNfaEpsilon::addTransitionToMap(const TransKey& key, const Transition& trans) {
   list<int>* destList;
   if (transMap.count(key) == 0) {
      destList = new list<int>;
      transMap[key] = destList;
   } else {
      destList = transMap[key];
   }

   destList->push_front(trans.destination);
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


void CompiledNfaEpsilon::traverseEpsilons(int node) {
   queue<int> nodeQueue;
   unordered_set<int> processed;
   
   nodeQueue.push(node);
   processed.insert(node);
   
   while (!nodeQueue.empty()) {
      int current = nodeQueue.front();
      nodeQueue.pop();

      list<int>* destinations = getEpsilonTransitions(current);

      processDestinationNodes(destinations, nodeQueue, processed);
   }
}

list<int>* CompiledNfaEpsilon::getEpsilonTransitions(int node) {
   TransKey key(node, 0);
   list<int>* destinations;
   if (transMap.count(key) != 0) { destinations = transMap[key]; }
   else { destinations = nullptr; }

   return destinations;
}

void CompiledNfaEpsilon::processDestinationNodes(
   list<int>* destinations, queue<int>& nodeQueue, unordered_set<int>& processed) {

   if (destinations != nullptr) {
      for (auto node = destinations->begin(); node != destinations->end(); node++) {
         if (processed.count(*node) == 0) {
            nodeQueue.push(*node);
            processed.insert(*node);
         }
            currentNodes.insert(*node);
      }
   }

}



// evaluate
//
// Accepts a string which is then applied to the NFA in order to check for
// validity. Returns true if the NFA evaluates the string as valid and false
// otherwise.

bool CompiledNfaEpsilon::evaluate(string input) {
   currentNodes.insert(startNode);
   traverseEpsilons(startNode);

   for (string::iterator currentChar = input.begin(); currentChar != input.end(); currentChar++) {
      applyCharacter(*currentChar);
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
   
   auto nodeIterator = currentNodes.begin();
   int setSize = currentNodes.size();
   
   for (int i = 0; i < setSize; i++) {
      int node = *nodeIterator;
      nodeIterator++;

      currentNodes.erase(node);
      TransKey key(node, currentChar);
      followTransitions(key, newCurrentNodes);
      
   }
   repopulateCurrentNodes(newCurrentNodes);
}

void CompiledNfaEpsilon::followTransitions(const TransKey& key, list<int>& newCurrentNodes) {
   if (transMap.count(key) != 0) {
      list<int>* destList = transMap[key];

      for (list<int>::iterator nodeIterator = destList->begin(); 
         nodeIterator != destList->end(); nodeIterator++) {
            newCurrentNodes.push_front(*nodeIterator);
      }
   }
}


// isInEndStates
//
// Checks to see if the NFA is currently occupying any nodes which are in the
// set of goal nodes. Returns true if it is and false otherwise.

bool CompiledNfaEpsilon::isInEndState() const {
   for (auto nodeIterator = goalNodes.begin(); nodeIterator != goalNodes.end(); nodeIterator++) {
      if (currentNodes.count(*nodeIterator) == 1) {
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
   for (auto nodeIterator = nodeList.cbegin(); nodeIterator != nodeList.cend(); nodeIterator++) {

         if (currentNodes.count(*nodeIterator) == 0) {
            currentNodes.insert(*nodeIterator);
            traverseEpsilons(*nodeIterator);
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

   for (auto nodeIterator = nodes.cbegin(); nodeIterator != nodes.end(); nodeIterator++) {
      currentNodes.insert(*nodeIterator);
   }
}

// getCurrentNodes
//
// Returns an ordered set of ints representing the currently occupied nodes.

set<int> CompiledNfaEpsilon::getCurrentNodes() const {
   set<int> nodes;

   for (auto nodeIterator = currentNodes.cbegin(); nodeIterator != currentNodes.cend(); nodeIterator++) {
      nodes.insert(*nodeIterator);
   }

   return nodes;
}


// getAlphabet
//
// Returns an ordered set of chars representing all of the characters which can
// trigger a valid transition in the internally stored NFA.

set<char> CompiledNfaEpsilon::getAlphabet() const {
   set<char> alphabet;
   
   for (auto charIterator = transMap.cbegin(); charIterator != transMap.cend(); charIterator++) {
      alphabet.insert(charIterator->first.transitionChar);
   }

   return alphabet;
}
