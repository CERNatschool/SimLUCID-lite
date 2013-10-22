/*! \file SimLucidRun.hh
 * \brief Header file for the SimLucidRun class.
 */

#ifndef SimLucidRun_h
#define SimLucidRun_h 1

// GEANT4 includes.
#include "G4Run.hh"

#include <TString.h>

class SimLucidDetectorConstruction;

/*! \brief Runs the SimLucid simulation.
 */
class SimLucidRun : public G4Run {

 public:

  SimLucidRun(SimLucidDetectorConstruction *, bool dbg = false);
    //!< Constructor.
  virtual ~SimLucidRun(); //!< Destructor.

 private:

  SimLucidDetectorConstruction * m_detectorPtr; //!< Pointer to the detector.

};

#endif
