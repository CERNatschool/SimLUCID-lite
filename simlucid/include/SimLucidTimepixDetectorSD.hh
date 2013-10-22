/*! \file SimLucidTimepixDetectorSD.hh
 * \brief Header file for the SimLucidTimepixDetectorSD class.
 */

#ifndef SimLucidTimepixDetectorSD_h
#define SimLucidTimepixDetectorSD_h 1

// GEANT4 include statements.
#include "G4VSensitiveDetector.hh"

// ROOT include statements.
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

// Standard include statements.
#include <set>

using namespace std;

//class SimLucidGeoDsc;

/*! \brief SimLucid Sensitive Detector class for the Timepix detectors.

@author T. Whyntie
@date   Autumn 2013

Based on work on the Allpix simulation package by J. Idarraga et al.

 */
class SimLucidTimepixDetectorSD : public G4VSensitiveDetector
{
 	
 public:
	
  SimLucidTimepixDetectorSD(G4String); //!< Constructor.
  ~SimLucidTimepixDetectorSD();        //!< Destructor.

  void Initialize(G4HCofThisEvent *);  //!< Called at the start of an event.

  /*! \brief Helper method for processing the hits into hit pixels.

@param[in]     hg   The 2D histogram representing the TPX detector.
@param[in,out] map  Map of pixel XY ID to energy deposited in the pixel.
@param[in]     x    The "x" value of the hit (relative to the TPX).
@param[in]     y    The "y" value of the hit (relative to the TPX).
@param[in]     edep The energy deposited in the silicon by the hit.

   */
  void ProcessOcc(TH2D * hg, std::map<G4int, G4double> & map,
                  G4double x, G4double y, G4double edep);

  /*! \brief Processes the hits found in the Timepix detector.

@param[in] step      The GEANT4 particle step for the hit.
@param[in] history   The GEANT4 "touchable history" for the hit.

   */
  G4bool ProcessHits(G4Step * step, G4TouchableHistory * history);

  void EndOfEvent(G4HCofThisEvent *); //!< Called at the end of an event.

 private:

  // Histograms
  TFile * m_file; //!< The ROOT file for the histograms.
  //
  TH1D * hg_E_all_;
  TH1D * hg_E_0_;
  TH1D * hg_E_1_;
  TH1D * hg_E_2_;
  TH1D * hg_E_3_;
  TH1D * hg_E_4_;
  //
  G4double plot_E_min_;
  G4double plot_E_max_;
  G4int    plot_E_bins_;
  //
  G4double plot_E_thresh_;    //!< Threshold on hit kinetic energy.
  //
 
  // Occupancy plots
  std::map<G4int, G4double> tpx0_E_map_; 
  std::map<G4int, G4double> tpx1_E_map_; 
  std::map<G4int, G4double> tpx2_E_map_; 
  std::map<G4int, G4double> tpx3_E_map_; 
  std::map<G4int, G4double> tpx4_E_map_; 

  TH1D * hg_tpx0_occ_;
  TH1D * hg_tpx1_occ_;
  TH1D * hg_tpx2_occ_;
  TH1D * hg_tpx3_occ_;
  TH1D * hg_tpx4_occ_;

  TH2D * hg_tpx0_map_;
  TH2D * hg_tpx1_map_;
  TH2D * hg_tpx2_map_;
  TH2D * hg_tpx3_map_;
  TH2D * hg_tpx4_map_;

};//end of SimLucidTimepixDetectorSD class definition.

#endif //~SimLucidTimepixDetectorSD_h
