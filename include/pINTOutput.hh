//
// This class will have a singleton behavior,. i.e., only one instance of the
// class can exist.
//
// This class enables the Geant4 user to open a file in the main program 
// which can then be accessed in other class member functions. The user
// must instantiate this class in main() and then instantiate it in the 
// constructors of all classes in which it is needed. 
//

// Modified for writing simulation output to a file (a singleton class)
// Oct 15, 2010, Hexc, Kanishka

#ifndef pINTOutput_h
#define pINTOutput_h 1

#include <iostream>
#include <fstream>
#include <string>

//#include "globals.hh"

using namespace std;

class pINTOutput {
  char* ofileName;
  pINTOutput(){}
public:    
  ofstream fout; 
  
  // By declaring instance() to be static we guarantee that wherever the
  // constructor is called the same "instance()" will be created. If it
  // doesn't exist it will be created; if it already exists nothing is
  // changed...
  
  static pINTOutput* instance() {
    static pINTOutput* fileOutput = new pINTOutput();
    return fileOutput;
  }
  
  void Fopen(char* myFilename) {
    ofileName = new char[strlen(myFilename)];
    strcpy(ofileName, myFilename);
    fout.open(ofileName);
  }
  
  void Fclose() {
    fout.close();
  }
  
};

#endif //


