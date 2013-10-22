/*! \file SimLucidRunAction.hh
 * \brief The header file for the SimLucidRunAction class.
 */

#ifndef SimLucidRunAction_h
#define SimLucidRunAction_h 1

// GEANT4 includes
#include "G4UserRunAction.hh"
#include "G4Run.hh"

// SimLUCID includes.
#include "SimLucidRun.hh"
#include "SimLucidDetectorConstruction.hh"

/*! \brief The G4UserRunAction class for SimLUCID.
  
@author T. Whyntie
@date   Autumn 2013
  
Based on work on the Allpix simulation package by J. Idarraga et al.
See Section 3.1.4.3 of the
[GEANT4 User's Guide: For Application Developers]
(http://geant4.web.cern.ch/geant4/geant4/support/userdocuments.shtml)
for more information.

 */
class SimLucidRunAction : public G4UserRunAction {

 public:

  /*! \brief Constructor for the SimLucidRunAction class.
   
\param[in,out] m_detectorPtr Pointer to the detector object.
   
This constructor is called only once in the whole program.
   
   */
  SimLucidRunAction(SimLucidDetectorConstruction * m_detectorPtr);

  ~SimLucidRunAction(); //!< Destructor.
	
 public:
  void BeginOfRunAction(const G4Run* aRun); //!< Called at the run start.
  void EndOfRunAction(  const G4Run* aRun); //!< Called at the run end.

  G4Run * GenerateRun(); //!< Generates the run.
	
 private:
  SimLucidDetectorConstruction * m_detectorPtr; //!< Pointer to the detector.

  SimLucidRun * m_SimLucidRun; //!< Pointer to the SimLucidRun object.

};//end of SimLucidRunAction class definition.

#endif /*SimLucidRunAction_h*/
