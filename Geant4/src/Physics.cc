#include "Physics.hh"


MyPhysicsList::MyPhysicsList(){
    //Definition of all standard electromagnetic processes
    RegisterPhysics(new G4EmStandardPhysics());

}

MyPhysicsList::~MyPhysicsList(){}

void MyPhysicsList::SetCuts()
{

    DumpCutValuesTable();
}