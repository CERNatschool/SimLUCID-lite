/*! \file SimLucid.cc
 *\brief Main file for the simlucid GEANT4 simulation.
 */

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//                                  SimLucid
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// @author: T. Whyntie (t.whyntie@qmul.ac.uk)
// @date:   Autumn 2013
//
// Inspired by J. Idaragga's "Allpix" code:
//
//  * https://twiki.cern.ch/twiki/bin/view/Main/AllPix
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include <time.h>
#include <string>

using namespace std;

// GEANT4 include statements.
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"

// SimLUCID include statements.
#include "SimLucidPhysicsList.hh"
#include "SimLucidDetectorConstruction.hh"
#include "SimLucidSteppingVerbose.hh"
#include "SimLucidRunAction.hh"
#include "SimLucidPrimaryGeneratorAction.hh"
#include "SimLucidEventAction.hh"
#include "SimLucidRun.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

// User Interface includes
#if defined(G4UI_USE_TCSH)
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#elif defined(G4UI_USE_XM)
#include "G4UIXm.hh"
#elif defined(G4UI_USE_WIN32)
#include "G4UIWin32.hh"
#elif defined(G4UI_USE_QT)
#include "G4UIQt.hh"
#include "G4Qt.hh"
#else
#include "G4UIterminal.hh"
#endif

// Input parameters - indexes for argv[]
typedef enum { 
  _MACRO = 1,
  _RUN_BATCH // run batch mode in this case
} inputPars;

void checkflags(int,char**); //!< Helper function for checking flags.

/*! \brief Main function.

@param[in] argc The argument numbers.
@param[in] argv The srguments supplied to the code.

*/
int main(int argc, char** argv)
{
  G4cout
    << "|=====================================================" << G4endl
    << "| SimLUCID: GEANT4 simulation of the LUCID experiment " << G4endl
    << "|=====================================================" << G4endl
    << "|                                                     " << G4endl;

  // Flags
  checkflags(argc, argv);
  G4String fileName = argv[_MACRO];

  // Seed the random number generator manually
  time_t rawtime;
  time(&rawtime);
  G4long myseed = G4long(rawtime);
  G4cout << G4endl << "* The random seed (localtime): " << myseed << G4endl;

  // User Verbose output class
  G4VSteppingVerbose* verbosity = new SimLucidSteppingVerbose;
  G4VSteppingVerbose::SetInstance(verbosity);

  // The Run Manager
  G4RunManager* runManager = new G4RunManager;

  // UserInitialization classes - mandatory
  //----------------------------------------
  //
  // Detector
  SimLucidDetectorConstruction * detector = new SimLucidDetectorConstruction();
  runManager->SetUserInitialization(detector);
  //
  // Physics List
  G4VUserPhysicsList * physics = new SimLucidPhysicsList;
  runManager->SetUserInitialization(physics);
  //
  // Run Action
  SimLucidRunAction * run_action = new SimLucidRunAction(detector);
  //
  runManager->SetUserAction(run_action);

  // Particle gun
  SourceType st = _GeneralParticleSource;
  G4VUserPrimaryGeneratorAction * gen_action = new SimLucidPrimaryGeneratorAction(st);
  runManager->SetUserAction(gen_action);

  // Event Action
  SimLucidEventAction * event_action = new SimLucidEventAction(run_action);
  runManager->SetUserAction(event_action);

  // Initialize G4 kernel
  // This is done from the macro, and so commented out here.
  //runManager->Initialize();

  CLHEP::HepRandom::setTheSeed(myseed);

#ifdef G4VIS_USE
  // Initialize visualization
  //
  G4VisManager* visManager = 0;
  if (argc-1 != _RUN_BATCH) {  // Only if we're not in batch mode
    visManager = new G4VisExecutive;
    visManager->Initialize();
  }
#endif

  // Get the pointer to the User Interface manager
  //
  G4UImanager* UI = G4UImanager::GetUIpointer();

  G4String command = "/control/execute ";

  if (argc-1 == _RUN_BATCH) {  // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[_MACRO];
    UI->ApplyCommand(command+fileName);
  } else {
    G4UIsession * session = 0;

    // Create a new interactive session.
#if defined(G4UI_USE_TCSH)
    session = new G4UIterminal(new G4UItcsh);      
#elif defined(G4UI_USE_XM)
    session = new G4UIXm(argc,argv);
#elif defined(G4UI_USE_WIN32)
    session = new G4UIWin32();
#elif defined(G4UI_USE_QT)
    session = new G4UIQt(argc, argv);
#else
    session = new G4UIterminal();
#endif

#ifdef G4VIS_USE
    UI->ApplyCommand(command + fileName);
#endif
    session->SessionStart();
    delete session;
  }//end of batch mode check.

#ifdef G4VIS_USE
  delete visManager;
#endif

  G4cout
    << "|-----------------------------------------------------" << G4endl
    << "| Done!                                               " << G4endl
    << "|=====================================================" << G4endl
    << "| SimLUCID: GEANT4 simulation of the LUCID experiment " << G4endl
    << "|=====================================================" << G4endl;

  delete runManager;
  delete verbosity;

  return 0;

}//end of main function.

//
// Function for checking the run flags.
//
void checkflags(int argc, char** argv){
  if(argc < 2){
    G4cout << "Use: " << G4endl;
    G4cout << "     " << argv[0] << " [macro filename]" << G4endl;
    exit(1);
  }//end of argc check
}//end of checkflags function.
