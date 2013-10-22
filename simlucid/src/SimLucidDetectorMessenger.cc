/*! \file SimLucidDetectorMessenger.cc
 * \brief Implementation of the SimLucidDetectorMessenger class.
 */

#include "SimLucidDetectorMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//
// Constructor
//
SimLucidDetectorMessenger::SimLucidDetectorMessenger(
  SimLucidDetectorConstruction * SimLucidDet)
  : m_SimLucidDetector(SimLucidDet) { 

  m_SimLucidDir = new G4UIdirectory("/SimLucid/");
  m_SimLucidDir->SetGuidance("UI commands MedipixG4");

  m_detDir = new G4UIdirectory("/SimLucid/det/");
  m_detDir->SetGuidance("Detector control");

  m_configDir = new G4UIdirectory("/SimLucid/config/");
  m_configDir->SetGuidance("SimLucid configuration");


  //////////////////////////
  // Config

  // Set material for world volume
  m_worldMaterial = new G4UIcmdWithAString("/SimLucid/extras/setWorldMaterial", this);
  m_worldMaterial->SetGuidance("Set material for world volume. (string: \"Air\", \"Vacuum\")");
  m_worldMaterial->SetParameterName("Material", false); // non omittable, no default
  m_worldMaterial->SetDefaultValue("Vacuum");
  m_worldMaterial->SetCandidates("Air Vacuum");
  m_worldMaterial->AvailableForStates(G4State_PreInit, G4State_Idle);

}//end of SimLucidDetectorMessenger constructor.

//
// Destructor.
//
SimLucidDetectorMessenger::~SimLucidDetectorMessenger() {

  delete m_worldMaterial;
  delete m_detDir;
  delete m_SimLucidDir;

}// end of SimLucidDetectorMessenger destructor.

//
// SetNewValue method.
//
void SimLucidDetectorMessenger::SetNewValue(
  G4UIcommand* command,
  G4String newValue) { 

  if( command == m_worldMaterial ) {
    m_SimLucidDetector->SetWorldMaterial(newValue);
  }

}//end of SetNewValue method.
