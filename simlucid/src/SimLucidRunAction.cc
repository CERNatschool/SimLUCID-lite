/*! \file SimLucidRunAction.cc
 * \brief Implementation of the SimLucidRunAction class.
 */

#include "SimLucidRunAction.hh"

//
// Constructor.
//
SimLucidRunAction::SimLucidRunAction(SimLucidDetectorConstruction * det) {
  m_detectorPtr = det;
}//end of SimLucidRunAction constructor.

SimLucidRunAction::~SimLucidRunAction() {}

//
// GenerateRun method.
//
G4Run * SimLucidRunAction::GenerateRun() {

  m_SimLucidRun = new SimLucidRun(m_detectorPtr, true);

  return m_SimLucidRun;

}//end of the SimLucidRun::GenerateRun() method.

//
// BeginOfRunAction method.
//
void SimLucidRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout
    << "|-------------------------------------------------" << G4endl
    << "|* Run " << aRun->GetRunID() << " start."           << G4endl
    << "|-------------------------------------------------" << G4endl;
}

//
// EndOfRunAction method.
//
void SimLucidRunAction::EndOfRunAction(const G4Run* aRun)
{   
  // At the end of the run.
  G4cout
    << "|-------------------------------------------------" << G4endl
    << "|* Event Id = " << aRun->GetNumberOfEvent() <<  " " << G4endl
    << "|-------------------------------------------------" << G4endl;
}
