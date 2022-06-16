#include "G4UserEventAction.hh"
#include "TTree.h"
#include "TFile.h"
#include "DataManager.hh"
#include "DataManagerMT.hh"
#include "G4Threading.hh"
#include <iostream>

/**
 * @brief This class manage the user action at each event
 *
 */
class MyEventAction : public G4UserEventAction
{
public:
    MyEventAction();
    virtual ~MyEventAction();

    /**
     * @brief Function that is called at the beginning of the event.
     *
     * @param event Pointer to the event that is currently running.
     */

    virtual void BeginOfEventAction(const G4Event *);

    /**
     * @brief Function that is called at the end of each event.
     *
     * @param event Pointer to the event that is currently running.
     */
    virtual void EndOfEventAction(const G4Event *);
};