/*! \file SimLucidEventAction.hh
 * \brief Header file for the SimLucidEventAction class.
 */

#ifndef SimLucidEventAction_h
#define SimLucidEventActino_h 1

// GEANT4 include statments.
#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4PrimaryVertex.hh"

// SimLUCID include statements.
#include "SimLucidRunAction.hh"

// Standard includes.
#include <vector>
#include <string>

using namespace std;

/*! \brief The event action class.

@author T. Whyntie
@date Autumn 2013

Based on work on the Allpix simulation package by J. Idarraga et al.

 */
class SimLucidEventAction : public G4UserEventAction {

 public:

  SimLucidEventAction(SimLucidRunAction *); //!< Constructor.
  virtual ~SimLucidEventAction();           //!< Destructor.

  void BeginOfEventAction(const G4Event*);
    //!< Method called at the beginning of every event.
  void   EndOfEventAction(const G4Event*);
    //!< Method called at the end of every event.

 private:

  SimLucidRunAction * m_run_action; //!< The RunAction object instance.

};//end of SimLucidEventAction class.

#endif
