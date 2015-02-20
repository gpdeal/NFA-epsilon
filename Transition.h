#pragma once

struct Transition {
   int source;
   char transitionChar;
   int destination;

   Transition() {}
   Transition(int s, char t, int d) {
      source = s;
      transitionChar = t;
      destination = d;
   }

};
