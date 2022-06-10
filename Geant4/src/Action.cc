#include "Action.hh"

MyActionInitialization::MyActionInitialization()
{
}

MyActionInitialization::~MyActionInitialization()
{
}

void MyActionInitialization::Build() const
{
    MyPrimaryGenerator* primaryGenerator = new MyPrimaryGenerator();
    SetUserAction(primaryGenerator);

/*     MyRunAction* runAction = new MyRunAction();
    SetUserAction(runAction); */
}

/* void MyActionInitialization::BuildForMaster() const
{
    MyRunAction* runAction = new MyRunAction();
    SetUserAction(runAction);
} */