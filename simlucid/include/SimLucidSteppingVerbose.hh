/*! \file SimLucidSteppingVerbose.hh
 * \brief The header file for the SimLucidSteppingVerbose class.
 */

class SimLucidSteppingVerbose;

#ifndef SimLucidSteppingVerbose_h
#define SimLucidSteppingVerbose_h 1

// GEANT4 includes
#include "G4SteppingVerbose.hh"

/*! \brief Class handling user-defined verbose actions.

@author T. Whyntie
@date   Autumn 2013

Based on work on the Allpix simulation package by J. Idarraga et al.
This class provides methods for outputting information about the
step and tracking processes that take place in the simulations.

*/
class SimLucidSteppingVerbose : public G4SteppingVerbose
{
 public:   

   SimLucidSteppingVerbose(); //!< Constructor.
  ~SimLucidSteppingVerbose(); //!< Destructor.

   void StepInfo(); //!< Information printed at every step.

   void TrackingStarted(); //!< Information printed when the tracking starts.

};

#endif
