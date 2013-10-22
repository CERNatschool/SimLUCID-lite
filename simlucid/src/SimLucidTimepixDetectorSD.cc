/*! \file SimLucidTimepixDetectorSD.cc
 * \brief Implementation of the SimLucidTimepixDetectorSD class.
 */

#include "SimLucidTimepixDetectorSD.hh"

// GEANT4 include statements.
#include "G4UnitsTable.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

// ROOT include statements.
#include "TMath.h"

SimLucidTimepixDetectorSD::SimLucidTimepixDetectorSD(G4String name)
  :  G4VSensitiveDetector(name)
{

  // Initialise the histograms
  //---------------------------
  //
  // Create the output file.
  m_file = new TFile("timepix.root","RECREATE");

  // These values are hardcoded for now.
  plot_E_bins_ = 40000;
  //
  plot_E_min_  =   0.; //MeV
  plot_E_max_  = 400.; //MeV
  //
  plot_E_thresh_ = 0.004; // 4 keV, ~ the Timepix energy threshold.
	
  // Energy histograms
  hg_E_all_ = new TH1D("E_all","",plot_E_bins_,plot_E_min_,plot_E_max_);
  hg_E_all_->Sumw2();
  hg_E_0_   = new TH1D("E_0",  "",plot_E_bins_,plot_E_min_,plot_E_max_);
  hg_E_0_->Sumw2();
  hg_E_1_   = new TH1D("E_1",  "",plot_E_bins_,plot_E_min_,plot_E_max_);
  hg_E_1_->Sumw2();
  hg_E_2_   = new TH1D("E_2",  "",plot_E_bins_,plot_E_min_,plot_E_max_);
  hg_E_2_->Sumw2();
  hg_E_3_   = new TH1D("E_3",  "",plot_E_bins_,plot_E_min_,plot_E_max_);
  hg_E_3_->Sumw2();
  hg_E_4_   = new TH1D("E_4",  "",plot_E_bins_,plot_E_min_,plot_E_max_);
  hg_E_4_->Sumw2();

  // TPX maps
  G4int    tpx_hpix  = 128; // Half the number of pixels across.
  //
  G4double tpx_pixw  = 0.055; // [mm] The pixel pitch (55 um)
  G4double tpx_hw    =  G4double(tpx_hpix) * tpx_pixw; // 1/2 sensor width.
  G4double tpx_y_cen = 18.30; // [mm] Height of TPX1-4
  G4double tpx_y_min = tpx_y_cen - tpx_hw; // [mm] TPX1-4 y minimum.
  G4double tpx_y_max = tpx_y_cen + tpx_hw; // [mm] TPX1-4 y maximum.

  G4double tpx_x_min = -tpx_hw; // [mm] TPX1-4 x minimum.
  G4double tpx_x_max =  tpx_hw; // [mm] TPX1-4 x maximum.
  G4double tpx_z_min = -tpx_hw; // [mm] TPX1-4 z maximum.
  G4double tpx_z_max =  tpx_hw; // [mm] TPX1-4 z maximum.

  // Initialise the TPX map histograms - 2D plots representing the
  // sensor surfaces.
  hg_tpx0_map_ = new TH2D("tpx0_map","", 2*tpx_hpix, -tpx_hw,  tpx_hw,
                                         2*tpx_hpix, -tpx_hw,  tpx_hw);
  hg_tpx0_map_->Sumw2();
  hg_tpx1_map_ = new TH2D("tpx1_map","", 2*tpx_hpix, tpx_x_min,  tpx_x_max,
                                         2*tpx_hpix, tpx_y_min,  tpx_y_max);
  hg_tpx1_map_->Sumw2();
  hg_tpx2_map_ = new TH2D("tpx2_map","", 2*tpx_hpix, tpx_z_min,  tpx_z_max,
                                         2*tpx_hpix, tpx_y_min,  tpx_y_max);
  hg_tpx2_map_->Sumw2();
  hg_tpx3_map_ = new TH2D("tpx3_map","", 2*tpx_hpix, tpx_x_min,  tpx_x_max,
                                         2*tpx_hpix, tpx_y_min,  tpx_y_max);
  hg_tpx3_map_->Sumw2();
  hg_tpx4_map_ = new TH2D("tpx4_map","", 2*tpx_hpix, tpx_z_min,  tpx_z_max,
                                         2*tpx_hpix, tpx_y_min,  tpx_y_max);
  hg_tpx4_map_->Sumw2();

  // Initialise the occupancy plots - i.e. the histograms recording how
  // many hit pixels each TPX detector recorded in each event.
  // (Technically, to get occupancy, you need to divide this number by
  // the total number of channels per detector, i.e. 65,536.)
  hg_tpx0_occ_ = new TH1D("tpx0_occ", "", 100, 0., 100.0);
  hg_tpx0_occ_->Sumw2();
  hg_tpx1_occ_ = new TH1D("tpx1_occ", "", 100, 0., 100.0);
  hg_tpx1_occ_->Sumw2();
  hg_tpx2_occ_ = new TH1D("tpx2_occ", "", 100, 0., 100.0);
  hg_tpx2_occ_->Sumw2();
  hg_tpx3_occ_ = new TH1D("tpx3_occ", "", 100, 0., 100.0);
  hg_tpx3_occ_->Sumw2();
  hg_tpx4_occ_ = new TH1D("tpx4_occ", "", 100, 0., 100.0);
  hg_tpx4_occ_->Sumw2();

}//end of SimLucidTimepixDetectorSD constructor.


SimLucidTimepixDetectorSD::~SimLucidTimepixDetectorSD() {
  
  // Write out the histograms to the ROOT file.	
  m_file->Write();

}//end of destructor.


//
// Initialise the detector
//
void SimLucidTimepixDetectorSD::Initialize(G4HCofThisEvent* HCE)
{

  bool dbg = false; // set to true if there's any debugging to do...

  if (dbg) G4cout
    << "DEBUG:" << G4endl
    << "DEBUG: SimLucidTimepixDetectorSD::Initialize() called." << G4endl
    << "DEBUG:" << G4endl;

}//end of SimLucidTimepixDetectorSD::Initialize method.

//
// Helper method for processing the "occupancy".
//
void SimLucidTimepixDetectorSD::ProcessOcc(
  TH2D * hg,
  std::map<G4int, G4double> & map,
  G4double x, G4double y, G4double edep) { 
 
  // Find the "X" and "Y" positions (in pixel terms) of the supplied hit
  // using the ROOT histogram's "FindBin" function.
  G4int X = hg->GetXaxis()->FindBin(x);
  G4int Y = hg->GetYaxis()->FindBin(y); // Careful! Y and Z...

  // Calculate the unique pixel "XY" ID, as Pixelman does.
  G4int XY = (256 * Y) + X;

  // Some debug statements - uncomment to see what's going on.
  //G4cout << "Hit at (" << X << ", " << Y << ") = " << XY 
  //       << " with energy " << T << " MeV "
  //       << "has deposited " << edep << " MeV." << G4endl;

  // Now the "clever" bit. We want to make sure we only count each hit pixel
  // once (there may be multiple hits in one pixel, which we're
  // indifferent to. So we use C++ std::map of pixel ID.
  std::map<G4int, G4double>::iterator pit = map.find(XY);

  if (pit==map.end()) {
    // If the "find" method returns the end, we have not hit this pixel
    // yet. So we create a new entry in the map (with the energy deposited
    // in the pixel).
    map[XY] = edep;
  } else {
    // If it doesn't the hit is already there in the map. So we add the
    // energy deposit to that entry (there might be fun things to do
    // with the energy deposited later on...).
    pit->second += edep;
  }
}//end of ProcessOcc method.

//
// Process the hits detected in the sensitive detector.
//
G4bool SimLucidTimepixDetectorSD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {

  bool dbg = false; // for debugging
	
  if (dbg) {
    G4cout<<"DEBUG:------------------------------------------------------" << G4endl;
    G4cout<<"DEBUG: SimLucidTimepixDetectorSD::ProcessHits method called " << G4endl;
    G4cout<<"DEBUG:------------------------------------------------------" << G4endl;
  }

  // Work with the Hit
  G4double edep = aStep->GetTotalEnergyDeposit();

  // We're only interested in hits that deposit energy in the silicon, so
  // leave the method is there isn't any.
  if(edep==0.0) {
    if (dbg) G4cout << "DEBUG: No energy deposited in this event." << G4endl;
    return false;
  }

  // Get the pre-step and post-step point from the step information.
  G4StepPoint * preStepPoint  = aStep->GetPreStepPoint();
  G4StepPoint * postStepPoint = aStep->GetPostStepPoint();

  // Process
  const G4VProcess * aProcessPointer = postStepPoint->GetProcessDefinedStep();
  // Track
  G4Track * aTrack = aStep->GetTrack();                        // The track.
  G4ParticleDefinition * aParticle = aTrack->GetDefinition();  // The particle.

  // Touchables.
  const G4TouchableHandle touchablepre  = preStepPoint->GetTouchableHandle();
  const G4TouchableHandle touchablepost = postStepPoint->GetTouchableHandle();

  if (dbg) {
    G4cout
    << "DEBUG: | @ Energy deposit                  : " << G4BestUnit(edep, "Energy") << G4endl
    << "DEBUG: | @ Pre-Step Point                  : " << G4BestUnit(preStepPoint->GetPosition(),  "Length") << G4endl
    << "DEBUG: | @ Touchable at the PreStepPoint   : " << touchablepre  << G4endl
    << "DEBUG: | @ Post-Step Point                 : " << G4BestUnit(postStepPoint->GetPosition(), "Length") << G4endl
    << "DEBUG: | @ Touchable at the PostStepPoint  : " << touchablepost << G4endl
    << "DEBUG: | @ Process                         : " << aProcessPointer->GetProcessName() << G4endl
    << "DEBUG: | @ Track total energy              : " << G4BestUnit(aTrack->GetTotalEnergy(),   "Energy") << G4endl
    << "DEBUG: | @ Track kin. energy               : " << G4BestUnit(aTrack->GetKineticEnergy(), "Energy") << G4endl
    << "DEBUG: | @ Particle name                   : " << aParticle->GetParticleName() << G4endl
    << "DEBUG: |---------------------------------------------------------------" << G4endl;
  }

  // Get the kinetic energy of the particle (n.b. not the energy deposited).
  G4double T = aTrack->GetKineticEnergy(); // [MeV]
  //
  G4double hit_x = postStepPoint->GetPosition().getX();
  G4double hit_y = postStepPoint->GetPosition().getY();
  G4double hit_z = postStepPoint->GetPosition().getZ();

  // Threshold on kinetic energy of the incident particle
  if (T < plot_E_thresh_) return false;

  hg_E_all_->Fill(T);

  // Fill histograms and populate the hit maps.
  // TPX 0
  if (touchablepre->GetCopyNumber() == 0) {
    hg_E_0_->Fill(T);
    hg_tpx0_map_->Fill(hit_x, hit_z);
    ProcessOcc(hg_tpx0_map_, tpx0_E_map_, hit_x, hit_z, edep);
  }
  // TPX 1
  else if (touchablepre->GetCopyNumber() == 1) {
    hg_E_1_->Fill(T);
    hg_tpx1_map_->Fill(hit_x, hit_y);
    ProcessOcc(hg_tpx1_map_, tpx1_E_map_, hit_x, hit_y, edep);
  }
  // TPX 2
  else if (touchablepre->GetCopyNumber() == 2) {
    hg_E_2_->Fill(T);
    hg_tpx2_map_->Fill(hit_z, hit_y);
    ProcessOcc(hg_tpx2_map_, tpx2_E_map_, hit_z, hit_y, edep);
  }
  // TPX 3
  else if (touchablepre->GetCopyNumber() == 3) {
    hg_E_3_->Fill(T);
    hg_tpx3_map_->Fill(hit_x, hit_y);
    ProcessOcc(hg_tpx3_map_, tpx3_E_map_, hit_x, hit_y, edep);
  }
  // TPX 4
  else if (touchablepre->GetCopyNumber() == 4) {
    hg_E_4_->Fill(T);
    hg_tpx4_map_->Fill(hit_z, hit_y);
    ProcessOcc(hg_tpx4_map_, tpx4_E_map_, hit_z, hit_y, edep);
  }

  return true;

}// End of the ProcessHits method.

//
// Method called at the end of an event.
//
void SimLucidTimepixDetectorSD::EndOfEvent(G4HCofThisEvent *) {

  bool dbg = false;

  if (dbg) G4cout
    << "DEBUG:" << G4endl
    << "DEBUG: SimLucidTimepixDetectorSD::EndOfEvent called " << G4endl
    << "DEBUG:----------------------------------------------" << G4endl;

  // Process the occupancy maps
  if (dbg) {
    G4cout << "DEBUG: size of TPX0 energy map = " << tpx0_E_map_.size() << G4endl;
    std::map<G4int, G4double>::iterator pit = tpx0_E_map_.begin();
    for (; pit != tpx0_E_map_.end(); ++pit) {
      G4cout
        << "DEBUG: Pixel " << pit->first
        << " has E = " << pit->second << " MeV" << G4endl;
    }//end of loop over the hit pixels.
  }

  // Fill the "Hit Pixels Per Source Particle" histograms with the
  // number of entries in the hit maps, i.e. the number of pixels that
  // were hit in the event (rather than the number of actual hits).
  hg_tpx0_occ_->Fill(tpx0_E_map_.size());
  hg_tpx1_occ_->Fill(tpx1_E_map_.size());
  hg_tpx2_occ_->Fill(tpx2_E_map_.size());
  hg_tpx3_occ_->Fill(tpx3_E_map_.size());
  hg_tpx4_occ_->Fill(tpx4_E_map_.size());

  // Clear the maps ready for the next event.
  tpx0_E_map_.clear();
  tpx1_E_map_.clear();
  tpx2_E_map_.clear();
  tpx3_E_map_.clear();
  tpx4_E_map_.clear();

}//end of EndOfEvent method.
