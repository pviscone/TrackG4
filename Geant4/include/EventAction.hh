#include "G4UserEventAction.hh"
#include "TTree.h"
#include "TFile.h"
#include "DataManager.hh"
#include <iostream>

class MyEventAction : public G4UserEventAction {
public:
    MyEventAction();
    virtual ~MyEventAction();
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
};