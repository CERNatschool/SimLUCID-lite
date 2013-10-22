/*! \file SimLucidPhysicsList.hh
 * \brief The header file for the SimLucidPhysicsList class.
 */

#ifndef SimLucidPhysicsList_h
#define SimLucidPhysicsList_h 1

// GEANT4 physics include statements.
#include "G4VModularPhysicsList.hh"
//
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsLHEP.hh"
#include "G4HadronQElasticPhysics.hh"
#include "G4ChargeExchangePhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4NeutronCrossSectionXS.hh"
#include "G4QStoppingPhysics.hh"
#include "G4LHEPStoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4IonPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmProcessOptions.hh"
//
#include "HadronPhysicsFTFP_BERT.hh"
#include "HadronPhysicsFTF_BIC.hh"
#include "HadronPhysicsLHEP.hh"
#include "HadronPhysicsLHEP_EMV.hh"
#include "G4HadronInelasticQBBC.hh"
#include "HadronPhysicsQGSC_BERT.hh"
#include "HadronPhysicsQGSP.hh"
#include "HadronPhysicsQGSP_BERT.hh"
#include "HadronPhysicsQGSP_BERT_HP.hh"
#include "HadronPhysicsQGSP_BIC.hh"
#include "HadronPhysicsQGSP_BIC_HP.hh"
#include "HadronPhysicsQGSP_FTFP_BERT.hh"
#include "HadronPhysicsQGS_BIC.hh"
//
#include "G4IonPhysics.hh"
//
#include "G4LossTableManager.hh"
#include "G4StepLimiter.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"

// Forward class declarations.
class G4VPhysicsConstructor;

/*! \brief The GEANT4 physics list class for SimLUCID.

@author T. Whyntie
@date Autumn 2013
 
Based on work on the Allpix simulation package by J. Idarraga et al.
See Section 5 of the
[GEANT4 User's Guide: For Application Developers]
(http://geant4.web.cern.ch/geant4/geant4/support/userdocuments.shtml)
for more information about tracking and physics processes.
 */
class SimLucidPhysicsList: public G4VModularPhysicsList
{

 public:
  SimLucidPhysicsList();
  virtual ~SimLucidPhysicsList();
	
  void ConstructParticle();
  void SetVerbose(G4int verbose); 
  void SetCuts();
  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForPositron(G4double);
  void SetCutForProton(G4double);
   	    
  void AddSimLucidPhysicsList(const G4String& name);
  void ConstructProcess();
  void AddStepMax();
  void List();
 	
 private:	
  void SetBuilderList0(G4bool flagHP      = false);
  void SetBuilderList1(G4bool flagHP      = false);
  void SetBuilderList2(G4bool addStopping = false);
  void SetBuilderList3();
  void SetBuilderList4();
	
  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;
  G4double cutForProton;
	
  G4VPhysicsConstructor*  emSimLucidPhysicsList;
  G4VPhysicsConstructor*  particleList;
  std::vector<G4VPhysicsConstructor*>  hadronPhys;

  G4bool dump;

};//end of SimLucidPhysicsList class definition.

#endif
