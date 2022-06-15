#include "Action.hh"

MyActionInitialization::MyActionInitialization()
{
}

MyActionInitialization::~MyActionInitialization()
{
}

void MyActionInitialization::Build() const{
    //Define the user action to define the structure of the output file
    MyRunAction* runAction = new MyRunAction();
    SetUserAction(runAction);

    MyEventAction* eventAction = new MyEventAction();
    SetUserAction(eventAction);

    //Define the action that manage the beam
    MyPrimaryGenerator* primaryGenerator = new MyPrimaryGenerator();
    SetUserAction(primaryGenerator);

}

void MyActionInitialization::BuildForMaster() const{
    //Define the user action to define the structure of the output file on the master thread
    MyRunAction* runAction = new MyRunAction();
    SetUserAction(runAction);
}