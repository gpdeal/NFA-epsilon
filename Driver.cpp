#include "CompiledNfaEpsilon.h"
#include "CompiledDfa.h"







int main() {

   
   // construct a*b*c* nfa-e machine
   FiniteStateMachine nfa1;

   int nodeInts[] = {1, 2, 3};
   set<int> nodes1(nodeInts, nodeInts + 3);
   
   set<int> goals1;
   goals1.insert(3);
   
   list<Transition> transList1;
   Transition na(1, 'a', 1), nb(1, 0, 2), nc(2, 'b', 2), nd(2, 0, 3), ne(3, 'c', 3);
   transList1.push_front(na);
   transList1.push_front(nb);
   transList1.push_front(nc);
   transList1.push_front(nd);
   transList1.push_front(ne);

   nfa1.nodes = nodes1;
   nfa1.goalNodes = goals1;
   nfa1.transitions = transList1;
   nfa1.startNode = 1;

   CompiledNfaEpsilon nfaAStarBStarCStar(nfa1);
 
   
   // construct a*b*c* dfa machine
   FiniteStateMachine dfa1;  

   list<Transition> dfa1TransList;
   Transition da(1, 'a', 1), db(1, 'b', 2), dc(1, 'c', 3), dd(2, 'b', 2), de(2, 'c', 3), df(3, 'c', 3);
   dfa1TransList.push_front(da);
   dfa1TransList.push_front(db);
   dfa1TransList.push_front(dc);
   dfa1TransList.push_front(dd);
   dfa1TransList.push_front(de);
   dfa1TransList.push_front(df);


   int dfa1Nodes[] = {1, 2, 3};
   set<int> dfa1NodeSet(dfa1Nodes, dfa1Nodes + 3);
   set<int>dfa1Goals(dfa1Nodes, dfa1Nodes + 3);

   dfa1.nodes = dfa1NodeSet;
   dfa1.goalNodes = dfa1Goals;
   dfa1.startNode = 1;
   dfa1.transitions = dfa1TransList;
   

   CompiledDfa dfaAStarBStarCStar(dfa1);

   cout << "NFA-e evaluation of a*b*c*:" << endl << endl;

   bool nfaabc = nfaAStarBStarCStar.evaluate("abc");
   cout << "abc evaluates to: " << (nfaabc ? "true" : "false") << endl;

   bool nfaempty = nfaAStarBStarCStar.evaluate("");
   cout << "empty string evaluates to: " << (nfaempty ? "true" : "false") << endl;

   bool nfaaaab = nfaAStarBStarCStar.evaluate("aaab");
   cout << "aaab evaluates to: " << (nfaaaab ? "true" : "false") << endl;
   
   bool nfadstring = nfaAStarBStarCStar.evaluate("d");
   cout << "d evaluates to: " << (nfadstring ? "true" : "false") << endl;

   bool nfabastring = nfaAStarBStarCStar.evaluate("ba");
   cout << "ba evaluates to: " << (nfabastring ? "true" : "false") << endl;

   cout << endl;


   cout << "DFA evaluation of a*b*c*:" << endl << endl;

   bool dfaabc = dfaAStarBStarCStar.evaluate("abc");
   cout << "abc evaluates to: " << (dfaabc ? "true" : "false") << endl;

   bool dfaempty = dfaAStarBStarCStar.evaluate("");
   cout << "empty string evaluates to: " << (dfaempty ? "true" : "false") << endl;

   bool dfaaaab = dfaAStarBStarCStar.evaluate("aaab");
   cout << "aaab evaluates to: " << (dfaaaab ? "true" : "false") << endl;
   
   bool dfadstring = dfaAStarBStarCStar.evaluate("d");
   cout << "d evaluates to: " << (dfadstring ? "true" : "false") << endl;

   bool dfabastring = dfaAStarBStarCStar.evaluate("ba");
   cout << "ba evaluates to: " << (dfabastring ? "true" : "false") << endl;

   cout << endl << endl;

   
   cout << "converting NFA to DFA..." << endl << endl;


   FiniteStateMachine convertedDfa = nfaAStarBStarCStar.convertToDfa();


   CompiledDfa converted(convertedDfa);

   cout << "converted DFA evaluation of a*b*c*:" << endl << endl;

   dfaabc = converted.evaluate("abc");
   cout << "abc evaluates to: " << (dfaabc ? "true" : "false") << endl;

   dfaempty = converted.evaluate("");
   cout << "empty string evaluates to: " << (dfaempty ? "true" : "false") << endl;

   dfaaaab = converted.evaluate("aaab");
   cout << "aaab evaluates to: " << (dfaaaab ? "true" : "false") << endl;
   
   dfadstring = converted.evaluate("d");
   cout << "d evaluates to: " << (dfadstring ? "true" : "false") << endl;

   dfabastring = converted.evaluate("ba");
   cout << "ba evaluates to: " << (dfabastring ? "true" : "false") << endl;


   int x;
   x = 5;
}
