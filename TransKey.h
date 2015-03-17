//-----------------------------------------------------------------------------
// Galen Deal
// CSS 502
//
// TransKey
//
// Header for TransKey struct. This object serves as a key for a map of
// transitions which maps the source node and transition character to the
// destination node.


#pragma once

using namespace std;

struct TransKey {
   int source;
   char transitionChar;

   TransKey() {}

   // Constructor which sets the source node and transition char to the passed
   // int and char
   TransKey(int node, char c) {
      source = node;
      transitionChar = c;
   }

   // Overloaded == operator. Returns true if the member variables of this
   // object are equal to those of the TransKey to which it is compared
   bool operator==(const TransKey& other) const {
      return (source == other.source 
         && transitionChar == other.transitionChar);
   }

};




/*
Hash function for TransKey object

The following code was adapted from an answer to a question on stackoverflow
provided by user jogojapan. Original code is here:
http://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
*/

namespace std {

   template <>
   struct hash<TransKey> {
      size_t operator()(const TransKey& k) const {

         return ((hash<int>()(k.source)
            ^ (hash<char>()(k.transitionChar) << 1)) >> 1);
      }
   };

}
