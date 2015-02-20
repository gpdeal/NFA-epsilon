#include "CompiledNfaEpsilon.h"







int main() {


   // construct a*b*c* nfa-e machine
   FiniteStateMachine nfa1;

   int nodeInts[] = {1, 2, 3};
   set<int> nodes1(nodeInts, nodeInts + 3);
   
   set<int> goals1;
   goals1.insert(1);
   
   list<Transition> transList1;
   Transition a(1, 0, 1), b(1, 0, 2), c(2, 'b', 2), d(2, 0, 3), e(3, 'c', 3);
   transList1.push_front(a);
   transList1.push_front(b);
   //transList1.push_front(c);
   //transList1.push_front(d);
   //transList1.push_front(e);

   nfa1.nodes = nodes1;
   nfa1.goalNodes = goals1;
   nfa1.transitions = transList1;
   nfa1.startNode = 1;

   CompiledNfaEpsilon aStarBStarCStar(nfa1);
   aStarBStarCStar.printStates();

}
