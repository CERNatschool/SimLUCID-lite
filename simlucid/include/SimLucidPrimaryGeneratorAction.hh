/*! \file SimLucidPrimaryGeneratorAction.hh
 * \brief The header file for the SimLucidPrimaryGeneratorAction class.
 */

#ifndef SimLucidPrimaryGeneratorAction_h
#define SimLucidPrimaryGeneratorAction_h 1

// GEANT4 include statements.
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4VPrimaryGenerator.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

// Standard include statements.
#include <vector>

using namespace std;

// Forward class declarations.
class G4Event;
class G4VPrimaryGenerator;

enum SourceType {
  _GeneralParticleSource
};

/*! \brief The SimLucidPrimaryGeneration class.

@author T. Whyntie
@date   Autumn 2013

Based on work on the Allpix simulation package by J. Idarraga et al.

*/
class SimLucidPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {

 public:

  SimLucidPrimaryGeneratorAction(SourceType); //!< Constructor.
  ~SimLucidPrimaryGeneratorAction();          //!< Destructor.
  
 public:
  void GeneratePrimaries(G4Event*);
  
 private:
  // Using the General Particle Source (GPS).
  G4GeneralParticleSource * m_particleSource; //!< General Particle Source.
  // Temporarily store the particle positions.
  vector<G4ThreeVector> m_primaryParticlePos; //!< Source particle positions.

  SourceType m_sType; //!< The source particle type.

}; // End of the SimLucidPrimaryGeneratorAction class.

#endif //~SimLucidPrimaryGeneratorAction_h
