#pragma once

struct TransKey {
   int source;
   char transitionChar;

   TransKey(int node, char c) {
      source = node;
      transitionChar = c;
   }


   bool operator==(const TransKey& other) const {
      return (source == other.source 
         && transitionChar == other.transitionChar);
   }

};


/* The following code was adapted from an answer to a question on stackoverflow
provided by user jogojapan. Original code is here:
http://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
*/

namespace std {

   template <>
   struct hash<TransKey> {
      size_t operator()(const TransKey& k) const {
         using std::hash;

         return ((hash<int>()(k.source)
            ^ (hash<char>()(k.transitionChar) << 1)) >> 1);
      }
   };

}


//struct KeyHasher {
//   std::size_t operator()(TransKey* k) const {
//      using std::size_t;
//      using std::hash;
//
//      return ((hash<int>()(k->source)
//         ^ (hash<char>()(k->transitionChar) << 1)) >> 1);
//   }
//};
