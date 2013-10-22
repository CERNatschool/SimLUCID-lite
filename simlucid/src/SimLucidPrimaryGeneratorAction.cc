/*! \file SimLucidPrimaryGeneratorAction.cc
 * \brief Implementation of the SimLucidPrimaryGeneratorAction class.
 */

#include "SimLucidPrimaryGeneratorAction.hh"

#include "Randomize.hh"

// GEANT4 include statements.
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GeneralParticleSource.hh"
#include "G4HEPEvtInterface.hh"

//
// Constructor.
//
SimLucidPrimaryGeneratorAction::SimLucidPrimaryGeneratorAction(SourceType st) {

  m_sType = st;
  m_particleSource = 0x0;

  if (m_sType == _GeneralParticleSource) {
    // Use the G4GeneralParticleSource.
    m_particleSource = new G4GeneralParticleSource();
  }

  // Temporarily store the position of incoming particles.
  m_primaryParticlePos.clear();

}

//
// Destructor.
//
SimLucidPrimaryGeneratorAction::~SimLucidPrimaryGeneratorAction() {
  if(m_particleSource) delete m_particleSource;
}

//
// GeneratePrimaries method.
//
void SimLucidPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

  if  (m_particleSource) { // If the particle source exists.

    m_particleSource->GeneratePrimaryVertex(anEvent);

    // Clear the primaries if at the first event.
    if (anEvent->GetEventID() == 0) {  m_primaryParticlePos.clear(); } 

    G4ThreeVector pos = m_particleSource->GetParticlePosition();
    m_primaryParticlePos.push_back(pos);

  }//end of 

}// End of the GeneratePrimaries method.
