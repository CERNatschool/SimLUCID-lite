/*! \file SimLucidPseudoDetectorSD.hh
 * \brief Header file for the SimLucidPseudoDetectorSD class.
 */

#ifndef SimLucidPseudoDetectorSD_h
#define SimLucidPseudoDetectorSD_h 1

// GEANT4 includes.
#include "G4VSensitiveDetector.hh"

// ROOT includes.
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

// Standard includes.
#include <set>

using namespace std;

// SimLUCID includes.
class SimLucidGeoDsc;

/*! \brief SimLucid Pseudo Detector class.

@author T. Whyntie
@date   Autumn 2013
 
Based on work on the Allpix simulation package by J. Idarraga et al.

 */
class SimLucidPseudoDetectorSD : public G4VSensitiveDetector
{
 	
 public:
	
  SimLucidPseudoDetectorSD(G4String); //!< Constructor.
  ~SimLucidPseudoDetectorSD();        //!< Destructor.

  void Initialize(G4HCofThisEvent *); //!< Called at the start of an event.
  G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    //!< Processes the detector interactions found in the event.
  void EndOfEvent(G4HCofThisEvent *); //!< Called at the end of an event.

 private:

  // Histograms
  TFile * m_file;          //!< The ROOT file.
  //
  TH1D * hg_kinenergy_;    //!< 1D histogram of the hit kinetic energy.
  //
  G4double plot_E_min_;    //!< Minimum energy of the energy histogram.
  G4double plot_E_max_;    //!< Maximum energy of the energy histogram.
  G4int    plot_E_bins_;   //!< The number of bins in the energy histogram.
  //
  G4double plot_E_thresh_; //!< Threshold on hit kinetic energy.

  // Positional histograms
  G4double det_r_;     //!< The pseudo-detector radius [mm].
  //
  G4double plot_max_;  //!< The plot maximum [mm].
  G4int    plot_bins_; //!< The number of bins for the positional histograms.
  //
  TH1D * hg_x_;        //!< Pseudo-detector hits - x position.
  TH1D * hg_y_;        //!< Pseudo-detector hits - y position.
  TH1D * hg_z_;        //!< Pseudo-detector hits - z position.
  TH2D * hg_x_y_;      //!< Pseudo-detector hits - x vs y position.
  TH2D * hg_y_z_;      //!< Pseudo-detector hits - y vs z position.
  TH2D * hg_z_x_;      //!< Pseudo-detector hits - z vs x position.

};//end of SimLucidPseudoDetectorSD class definition.

#endif //~SimLucidPseudoDetectorSD_h
