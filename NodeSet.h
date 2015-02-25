#pragma once

#include <set>

using namespace std;

struct NodeSet {
   set<int> nodes;

   NodeSet() {}

   NodeSet(const set<int>& otherSet) {
      for (auto it = otherSet.cbegin(); it != otherSet.cend(); it++) {
         nodes.insert(*it);
      }

   }

   NodeSet(const NodeSet& other) {
      for (auto it = other.nodes.cbegin(); it != other.nodes.cend(); it++) {
         nodes.insert(*it);
      }
   }

   bool operator==(const NodeSet& other) const {
      return nodes == other.nodes;
   }
};


namespace std {
   
   template <>
   struct hash<NodeSet> {
      
      size_t operator()(const NodeSet& k) const {
         int hashval = 0;

         for (auto it = k.nodes.cbegin(); it != k.nodes.cend(); it++) {
            int temp = *it;
            int numberOfDigits = 0;
            for (;;) {
               numberOfDigits++;
               temp = temp / 10;
               if (temp == 0) { break; }
            }

            hashval *= 10 * numberOfDigits;
            hashval += *it;
         }

         return hash<int>()(hashval);
      }

   };

}
