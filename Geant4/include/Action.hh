#pragma once
#include "G4VUserActionInitialization.hh"
#include "Generator.hh"
//#include "run.hh"

class MyActionInitialization : public G4VUserActionInitialization{
    public:
        MyActionInitialization();
        ~MyActionInitialization();

        //Runna la particle gun e si occupa dello stepping (build per ogni thread, build for master solo per il master thread)
        virtual void Build() const;
        //virtual void BuildForMaster() const;
};