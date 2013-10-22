/*! \file SimLucidDetectorConstruction.hh
 * \brief Header file for the SimLucidDetectorConstruction class.
 */

#ifndef SimLucidDetectorConstruction_h
#define SimLucidDetectorConstruction_h 1

//
// GEANT4 include statements.
//
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
//
#include "G4RunManager.hh"
//
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PVDivision.hh"
//
// Materials includes.
#include "G4NistManager.hh"
//
// Geometry includes.
#include "G4SystemOfUnits.hh"
#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4SolidStore.hh"
#include "G4GeometryManager.hh"
//
// Logical volume includes.
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4PVPlacement.hh"
//
// "Sensitive Detector" includes.
#include "G4SDManager.hh"
//
#include "G4UserEventAction.hh"
//
#include "SimLucidDetectorMessenger.hh"
#include "SimLucidPseudoDetectorSD.hh"
#include "SimLucidTimepixDetectorSD.hh"

// Standard includes
#include <vector>
#include <stdio.h>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

// Forward class declarations
class SimLucidDetectorMessenger;

/*! \brief The SimLUCID Detector Construction Class.
 
@author T. Whyntie
@date   Autumn 2013
 
Based on work on the Allpix simulation package by J. Idarraga et al.

 */
class SimLucidDetectorConstruction : public G4VUserDetectorConstruction {

 public:

  SimLucidDetectorConstruction();
  ~SimLucidDetectorConstruction();

  void SetOutputFilePrefix(G4String);

//  inline G4String GetOutputFilePrefix() { return m_outputFilePrefix; };

  G4VPhysicalVolume* Construct(); //!< Construct the world and detectors.

  void SetWorldMaterial(G4String mat);

 private:

  // The experimental hall - space, in our case!
  G4LogicalVolume   * expHall_log;
  G4VPhysicalVolume * expHall_phys;
  //
  // The "Pseudo Detector" - a false detector for checking source details.
  G4LogicalVolume   * m_pseudoDetector_log;
  G4VPhysicalVolume * m_pseudoDetector_phys;
  //
  // The aluminium base plate (top of the LUCID box).
  G4LogicalVolume   * m_AlBasePlate_log;
  G4VPhysicalVolume * m_AlBasePlate_phys;
  //
  // The PCB base.
  G4LogicalVolume   * m_PcbBase_log;
  G4VPhysicalVolume * m_PcbBase_phys;
  //
  // The Timepix (TPX) support posts.
  G4LogicalVolume   * m_SupportPost_log;
  G4VPhysicalVolume * m_SupportPost12_phys;
  G4VPhysicalVolume * m_SupportPost23_phys;
  G4VPhysicalVolume * m_SupportPost34_phys;
  G4VPhysicalVolume * m_SupportPost41_phys;
  //
  // The TPX PCBs.
  G4LogicalVolume   * m_TpxPcb_log;
  G4VPhysicalVolume * m_TpxPcb1_phys;
  G4VPhysicalVolume * m_TpxPcb2_phys;
  G4VPhysicalVolume * m_TpxPcb3_phys;
  G4VPhysicalVolume * m_TpxPcb4_phys;
  //
  // The Timepix microchips.
  G4LogicalVolume   * m_Tpx_log;   //!< The TPX logical volume.
  G4VPhysicalVolume * m_Tpx0_phys; //!< The physical placement of TPX0.
  G4VPhysicalVolume * m_Tpx1_phys; //!< The physical placement of TPX1.
  G4VPhysicalVolume * m_Tpx2_phys; //!< The physical placement of TPX2.
  G4VPhysicalVolume * m_Tpx3_phys; //!< The physical placement of TPX3.
  G4VPhysicalVolume * m_Tpx4_phys; //!< The physical placement of TPX4.
  //
  // The silicon sensors.
  G4LogicalVolume   * m_TpxS_log;
  G4VPhysicalVolume * m_TpxS0_phys;
  G4VPhysicalVolume * m_TpxS1_phys;
  G4VPhysicalVolume * m_TpxS2_phys;
  G4VPhysicalVolume * m_TpxS3_phys;
  G4VPhysicalVolume * m_TpxS4_phys;
  //
  // The aluminium foil covering the sensor surface(s).
  G4LogicalVolume   * m_Foil_log;
  G4VPhysicalVolume * m_Foil0_phys;
  G4VPhysicalVolume * m_Foil1_phys;
  G4VPhysicalVolume * m_Foil2_phys;
  G4VPhysicalVolume * m_Foil3_phys;
  G4VPhysicalVolume * m_Foil4_phys;
  //
  // The "dome" wall.
  G4LogicalVolume   * m_DomeWall_log;
  G4VPhysicalVolume * m_DomeWall_phys;
  //
  // The "dome" top.
  G4LogicalVolume   * m_DomeTop_log;
  G4VPhysicalVolume * m_DomeTop_phys;

  SimLucidDetectorMessenger * m_detectorMessenger;
    //!< Pointer to the detector messenger, for communicating with the
    //!< detector via the input card.
	
  // Materials
  G4Material * m_Air;    //!< Air (material).
  G4Material * m_Vacuum; //!< The vacuum (material).
  //
  G4Material * m_fillingWorldMaterial;     //!< World-filling material.
  G4String     m_fillingWorldMaterialName; //!< World-filling material name.
  //
  bool m_userDefinedWorldMaterial;

};//end of class SimLucidDetectorConstruction.

#endif //~SimLucidDetectorConstruction_h
