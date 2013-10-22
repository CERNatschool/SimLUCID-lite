/*! \file SimLucidPhysicsList.cc
 * \brief Implementation of the SimLucidPhysicsList class.
 */

#include "SimLucidPhysicsList.hh"

//
// Constructor.
//
SimLucidPhysicsList::SimLucidPhysicsList() : G4VModularPhysicsList() {

  G4LossTableManager::Instance();
  defaultCutValue = 0.010*mm;
  cutForGamma     = defaultCutValue;
  cutForElectron  = defaultCutValue;
  cutForPositron  = defaultCutValue;
  cutForProton    = defaultCutValue;
  verboseLevel    = 1;
	
  // Particles
  particleList = new G4DecayPhysics("decays");
	
  // EM physics
  emSimLucidPhysicsList = new G4EmStandardPhysics();

}//end of SimLucidPhysicsList constructor.

//
// Destructor.
//
SimLucidPhysicsList::~SimLucidPhysicsList()
{
  delete particleList;
  delete emSimLucidPhysicsList;
  for (size_t i=0; i<hadronPhys.size(); i++) {
    delete hadronPhys[i];
  }

}//end of SimLucidPhysicsList desctructor.

//
//
//
void SimLucidPhysicsList::ConstructParticle()
{
  particleList->ConstructParticle();
}

//
//
//
void SimLucidPhysicsList::ConstructProcess()
{
  AddTransportation();
  emSimLucidPhysicsList->ConstructProcess();
  particleList->ConstructProcess();
 
 for(size_t i=0; i<hadronPhys.size(); i++) {
    hadronPhys[i]->ConstructProcess();
  }
  AddStepMax();
}

void SimLucidPhysicsList::SetVerbose(G4int verbose)
{
  emSimLucidPhysicsList->SetVerboseLevel(verbose);
  for(size_t i=0; i<hadronPhys.size(); i++) {
    hadronPhys[i]->SetVerboseLevel(verbose);
  }
}

//
//
//
void SimLucidPhysicsList::AddStepMax()
{

  // Step limitation seen as a process
  G4StepLimiter* stepLimiter = new G4StepLimiter();

  theParticleIterator->reset();

  while ((*theParticleIterator)()) {
    G4ParticleDefinition* particle = theParticleIterator->value();
    //G4cout << particle->GetPDGCharge() << G4endl; // TW commented out...
    G4ProcessManager * pmanager = particle->GetProcessManager();

    if (particle->GetPDGCharge() != 0.0) {
      pmanager->AddDiscreteProcess(stepLimiter);
    }
  }//end of while loop over the particle iterator.
}

//
//
//
void SimLucidPhysicsList::AddSimLucidPhysicsList(const G4String& name)
{
  if (verboseLevel > 0) {
    G4cout << "SimLucidPhysicsList::AddSimLucidPhysicsList: <" << name << ">" << G4endl;
  }
	
  if (name == "emstandard_opt2") {		
    delete emSimLucidPhysicsList;
    emSimLucidPhysicsList = new G4EmStandardPhysics_option2();
  } else if (name == "emstandard_opt3") {
    delete emSimLucidPhysicsList;
    emSimLucidPhysicsList = new G4EmStandardPhysics_option3();
  } else if (name == "emstandard_opt1") {
    delete emSimLucidPhysicsList;
    emSimLucidPhysicsList = new G4EmStandardPhysics_option1();
  } else if (name == "emstandard_opt0") {
    delete emSimLucidPhysicsList;
    emSimLucidPhysicsList = new G4EmStandardPhysics();
  } else if (name == "FTFP_BERT_EMV") {
    AddSimLucidPhysicsList("emstandard_opt1");
    AddSimLucidPhysicsList("FTFP_BERT");
  } else if (name == "FTFP_BERT_EMY") {
    AddSimLucidPhysicsList("emstandard_opt3");
    AddSimLucidPhysicsList("FTFP_BERT");
  } else if (name == "FTFP_BERT_EMX") {
    AddSimLucidPhysicsList("emstandard_opt2");
    AddSimLucidPhysicsList("FTFP_BERT");
  } else if (name == "FTFP_BERT") {
    SetBuilderList1();
    hadronPhys.push_back( new HadronPhysicsFTFP_BERT());
  } else if (name == "FTF_BIC") {
    SetBuilderList0();
    hadronPhys.push_back( new HadronPhysicsFTF_BIC());
    hadronPhys.push_back( new G4NeutronCrossSectionXS(verboseLevel));
  } else if (name == "LHEP") {
    SetBuilderList2();
    hadronPhys.push_back( new HadronPhysicsLHEP());
  } else if (name == "LHEP_EMV") {
    AddSimLucidPhysicsList("emstandard_opt1");
    SetBuilderList2(true);
    hadronPhys.push_back( new HadronPhysicsLHEP_EMV());
  } else if (name == "QBBC") {
    AddSimLucidPhysicsList("emstandard_opt2");
    SetBuilderList3();
    hadronPhys.push_back( new G4HadronInelasticQBBC());
  } else if (name == "QGSC_BERT") {
    SetBuilderList4();
    hadronPhys.push_back( new HadronPhysicsQGSC_BERT());
  } else if (name == "QGSP") {
    SetBuilderList1();
    hadronPhys.push_back( new HadronPhysicsQGSP());
  } else if (name == "QGSP_BERT") {
    SetBuilderList1();
    hadronPhys.push_back( new HadronPhysicsQGSP_BERT());
  } else if (name == "QGSP_FTFP_BERT") {
    SetBuilderList1();
    hadronPhys.push_back( new HadronPhysicsQGSP_FTFP_BERT());
  } else if (name == "QGSP_BERT_EMV") {
    AddSimLucidPhysicsList("emstandard_opt1");
    AddSimLucidPhysicsList("QGSP_BERT");
  } else if (name == "QGSP_BERT_EMX") {
    AddSimLucidPhysicsList("emstandard_opt2");
    AddSimLucidPhysicsList("QGSP_BERT");
  } else if (name == "QGSP_BERT_HP") {
    SetBuilderList1(true);
    hadronPhys.push_back( new HadronPhysicsQGSP_BERT_HP());
  } else if (name == "QGSP_BIC") {
    SetBuilderList0();
    hadronPhys.push_back( new HadronPhysicsQGSP_BIC());
  } else if (name == "QGSP_BIC_EMY") {
    AddSimLucidPhysicsList("emstandard_opt3");
    SetBuilderList0();
    hadronPhys.push_back( new HadronPhysicsQGSP_BIC());
  } else if (name == "QGS_BIC") {
    SetBuilderList0();
    hadronPhys.push_back( new HadronPhysicsQGS_BIC());
    hadronPhys.push_back( new G4NeutronCrossSectionXS(verboseLevel));
  } else if (name == "QGSP_BIC_HP") {
    SetBuilderList0(true);
    hadronPhys.push_back( new HadronPhysicsQGSP_BIC_HP());
  } else if (name == "LIVERMORE_FTFP_BERT") {
    SetBuilderList1();
    hadronPhys.push_back( new HadronPhysicsFTFP_BERT());
    emSimLucidPhysicsList = new G4EmLivermorePhysics();
    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(250*eV, 1000*GeV); 
    G4cout << "Implementing physics list LIVERMORE_FTFP_BERT" << G4endl;
  } else {
    G4cout << "Physics list not found" << G4endl;
  }//end of physics list check.

}//end of AddSimLucidPhysicsList method.

//
//
//
void SimLucidPhysicsList::SetBuilderList0(G4bool flagHP)
{
  hadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  if(flagHP) {
    hadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
  } else {
    hadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
  }
  hadronPhys.push_back( new G4QStoppingPhysics(verboseLevel));
  hadronPhys.push_back( new G4IonBinaryCascadePhysics(verboseLevel));
  hadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}

//
//
//
void SimLucidPhysicsList::SetBuilderList1(G4bool flagHP)
{
  hadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  if(flagHP) {
    hadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
  } else {
    hadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
  }
  hadronPhys.push_back( new G4QStoppingPhysics(verboseLevel));
  hadronPhys.push_back( new G4IonPhysics(verboseLevel));
  hadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}

//
//
//
void SimLucidPhysicsList::SetBuilderList2(G4bool addStopping)
{
  hadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  hadronPhys.push_back( new G4HadronElasticPhysicsLHEP(verboseLevel));
  if (addStopping) {
    hadronPhys.push_back( new G4QStoppingPhysics(verboseLevel));
  }
  hadronPhys.push_back( new G4IonPhysics(verboseLevel));
}

//
//
//
void SimLucidPhysicsList::SetBuilderList3()
{
  hadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  RegisterPhysics( new G4HadronElasticPhysicsXS(verboseLevel) );
  hadronPhys.push_back( new G4QStoppingPhysics(verboseLevel));
  hadronPhys.push_back( new G4IonBinaryCascadePhysics(verboseLevel));
  hadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}

//
//
//
void SimLucidPhysicsList::SetBuilderList4()
{
  hadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  hadronPhys.push_back( new G4HadronQElasticPhysics(verboseLevel));
  hadronPhys.push_back( new G4QStoppingPhysics(verboseLevel));
  hadronPhys.push_back( new G4IonPhysics(verboseLevel));
  hadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}

//
//
//
void SimLucidPhysicsList::SetCuts()
{

  if (verboseLevel >0) {
    G4cout << "SimLucidPhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");
  SetCutValue(cutForProton, "proton");

  if (verboseLevel>0) DumpCutValuesTable();

}

//
//
//
void SimLucidPhysicsList::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}

//
//
//
void SimLucidPhysicsList::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}

//
//
//
void SimLucidPhysicsList::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}

//
//
//
void SimLucidPhysicsList::SetCutForProton(G4double cut)
{
  cutForProton = cut;
  SetParticleCuts(cutForProton, G4Proton::Proton());
}

//
//
//
void SimLucidPhysicsList::List()
{
  G4cout << "### PhysicsLists available: FTFP_BERT FTFP_BERT_EMV FTFP_BERT_EMX FTFP_BERT_EMY FTF_BIC"                   << G4endl;
  G4cout << "                            LHEP LHEP_EMV QBBC QGS_BIC QGSP"                                               << G4endl;
  G4cout << "                            QGSC_BERT QGSP_BERT QGSP_BERT_EMV QGSP_BIC_EMY"                                << G4endl;
  G4cout << "                            QGSP_BERT_EMX QGSP_BERT_HP QGSP_BIC QGSP_BIC_HP LIVERMORE_LIVERMORE_FTFP_BERT" << G4endl; 
}
