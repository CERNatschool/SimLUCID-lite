/*! \file SimLucidPseudoDetectorSD.cc
 * \brief Implementation of the SimLucidPseudoDetectorSD class.
 */

#include "SimLucidPseudoDetectorSD.hh"

// GEANT4 includes.
#include "G4UnitsTable.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

// ROOT includes.
#include "TMath.h"

//
// Constructor for the pseudo-detector.
//
SimLucidPseudoDetectorSD::SimLucidPseudoDetectorSD(G4String name)
  :  G4VSensitiveDetector(name)
{

  // Create the output file.
  m_file = new TFile("pseudodet.root","RECREATE");
	
  // Histograms
  // Hardcoded for now.
  plot_E_bins_ = 40000;
  //
  plot_E_min_  =   0.; //MeV
  plot_E_max_  = 400.; //MeV
  //
  plot_E_thresh_ = 0.004; // 4 keV, ~ the Timepix energy threshold.
  //	
  hg_kinenergy_ = new TH1D("kinenergy","",plot_E_bins_,plot_E_min_,plot_E_max_);
  hg_kinenergy_->Sumw2();

  // Positional histograms.
  // ard-coded for now... TODO get from the input file.
  det_r_ = 50.0; // [mm] 
  
  plot_max_  = G4double(G4int(det_r_) + 5);
  plot_bins_ = G4int(2 * plot_max_);
  //
  hg_x_   = new TH1D("x",  "", plot_bins_, -plot_max_,  plot_max_);
  hg_x_->Sumw2();
  hg_y_   = new TH1D("y",  "", plot_bins_, -plot_max_,  plot_max_);
  hg_y_->Sumw2();
  hg_z_   = new TH1D("z",  "", plot_bins_, -plot_max_,  plot_max_);
  hg_z_->Sumw2();
  hg_x_y_ = new TH2D("x_y","", plot_bins_, -plot_max_,  plot_max_,
                               plot_bins_, -plot_max_,  plot_max_);
  hg_x_y_->Sumw2();
  hg_y_z_ = new TH2D("y_z","", plot_bins_, -plot_max_,  plot_max_,
                               plot_bins_, -plot_max_,  plot_max_);
  hg_y_z_->Sumw2();
  hg_z_x_ = new TH2D("z_x","", plot_bins_, -plot_max_,  plot_max_,
                               plot_bins_, -plot_max_,  plot_max_);
  hg_z_x_->Sumw2();

}//end of SimLucidPseudoDetectorSD constructor.


SimLucidPseudoDetectorSD::~SimLucidPseudoDetectorSD() {
	
  m_file->Write();

}//end of destructor.


//
//
//
void SimLucidPseudoDetectorSD::Initialize(G4HCofThisEvent* HCE)
{
  bool dbg = false;
  if (dbg) G4cout
    << "DEBUG:" << G4endl
    << "DEBUG: SimLucidPseudoDetectorSD::Initialize() called." << G4endl
    << "DEBUG:" << G4endl;

}//end of SimLucidPseudoDetectorSD::Initialize method.

//
// Process the hits.
//
G4bool SimLucidPseudoDetectorSD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {

  bool dbg = false;
	
  if (dbg) {
    G4cout<<"DEBUG:"<<G4endl;
    G4cout<<"DEBUG: SimLucidPseudoDetectorSD::ProcessHits method called " << G4endl;
    G4cout<<"DEBUG:--------------------------------------------" << G4endl;
  }

  // Work with the Hit
  G4double edep = aStep->GetTotalEnergyDeposit();

  if(edep==0.0) {
    if (dbg) G4cout << "DEBUG: No energy deposited in this event." << G4endl;
    return false;
  }

  G4StepPoint * preStepPoint  = aStep->GetPreStepPoint();
  G4StepPoint * postStepPoint = aStep->GetPostStepPoint();

  // Process
  const G4VProcess * aProcessPointer = postStepPoint->GetProcessDefinedStep();
  // Track
  G4Track * aTrack = aStep->GetTrack();                        //!< The track.
  G4ParticleDefinition * aParticle = aTrack->GetDefinition();  //!< The particle.

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

  // Get the kinetic energy of the track.
  G4double T = aTrack->GetKineticEnergy();

  // Threshold on kinetic energy of the incident particle
  if (T < plot_E_thresh_) return false;

  // Plot kinetic energy of the particle.
  hg_kinenergy_->Fill(T);

  // Plot the hit positional information.
  G4double hit_x = postStepPoint->GetPosition().getX();
  G4double hit_y = postStepPoint->GetPosition().getY();
  G4double hit_z = postStepPoint->GetPosition().getZ();
  //
  hg_x_->Fill(hit_x);
  hg_y_->Fill(hit_y);
  hg_z_->Fill(hit_z);
  //
  hg_x_y_->Fill(hit_x, hit_y);
  hg_y_z_->Fill(hit_y, hit_z);
  hg_z_x_->Fill(hit_z, hit_x);

  return true;
}

//
//
//
void SimLucidPseudoDetectorSD::EndOfEvent(G4HCofThisEvent *) {

  bool dbg = false;

  if (dbg) G4cout
    << "DEBUG:" << G4endl
    << "DEBUG: SimLucidPseudoDetectorSD::EndOfEvent called " << G4endl
    << "DEBUG:------------------------------------" << G4endl;

}//end of EndOfEvent method.
