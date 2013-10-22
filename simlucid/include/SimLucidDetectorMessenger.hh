/*! \file SimLucidDetectorMessenger.hh
 * \brief Header file for the SimLucidDetectorMessenger class.
 */

#ifndef SimLucidDetectorMessenger_h
#define SimLucidDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "SimLucidDetectorConstruction.hh"

// Forward declarations.
class SimLucidDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAString;
 
/*! \brief Messenger class for the SimLucid detectors.
 *
 * @author T. Whyntie
 * @date   Autumn 2013
 *
 * Based on work on the Allpix simulation package by J. Idarraga et al.
 *
 */
class SimLucidDetectorMessenger : public G4UImessenger {

 public:
  SimLucidDetectorMessenger(SimLucidDetectorConstruction *); //!< Constructor.
  ~SimLucidDetectorMessenger();                              //!< Destructor.
	
  void SetNewValue(G4UIcommand*, G4String); //!< Set a new value of a command.
	
 private:
  SimLucidDetectorConstruction * m_SimLucidDetector;
    //!< Pointer to the detector.
  
  G4UIdirectory * m_SimLucidDir; //!< Directory for SimLUCID commands.
  G4UIdirectory * m_detDir;      //!< Directory for detector commands.
  G4UIdirectory * m_configDir;   //!< Directory for configuration commands.

  G4UIcmdWithAString * m_worldMaterial; //!< The world material command.
	
};//end of SimLucidDetectorMessenger class definition.

#endif //~SimLucidDetectorMessenger_h
