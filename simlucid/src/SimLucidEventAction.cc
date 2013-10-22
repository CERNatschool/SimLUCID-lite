/*! \file SimLucidEventAction.cc
 * \brief Implementation of SimLucidEventAction class.
 */

#include "SimLucidEventAction.hh"

SimLucidEventAction::SimLucidEventAction(SimLucidRunAction* run) {
  m_run_action = run;
}

SimLucidEventAction::~SimLucidEventAction()                       {}

void SimLucidEventAction::BeginOfEventAction(const G4Event * evt) {}

void SimLucidEventAction::EndOfEventAction(const G4Event * evt)   {}
