/*! \file SimLucidDetectorConstruction.cc
 * \brief Implementation of the SimLucidDetectorConstruction class.
 */

// SimLUCID include statements.
#include "SimLucidDetectorConstruction.hh"
#include "SimLucidEventAction.hh"

//
// Constructor
//
SimLucidDetectorConstruction::SimLucidDetectorConstruction() {
	
  // Instantiate the detector messenger member.
  m_detectorMessenger = new SimLucidDetectorMessenger(this);

  // Materials.
  m_Air = 0x0;
  m_Vacuum = 0x0;
  
  m_fillingWorldMaterial = 0x0;
  m_userDefinedWorldMaterial = false;

} //end of constructor.

//
// Destructor.
//
SimLucidDetectorConstruction::~SimLucidDetectorConstruction() {} 

//
// Construct method.
//
G4VPhysicalVolume * SimLucidDetectorConstruction::Construct()
{

  //G4cout
  //  << "DEBUG:" << G4endl
  //  << "DEBUG: Start of the SimLucidDetectorConstruction Construct() method." << G4endl
  //  << "DEBUG:" << G4endl;

  // Clean old geometry, if any
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //===========
  // Materials
  //===========

  G4NistManager * nistman = G4NistManager::Instance();

  // User-defined materials
  G4Element * Si = nistman->FindOrBuildElement("Si");
  G4Element * O  = nistman->FindOrBuildElement("O");
  G4Element * C  = nistman->FindOrBuildElement("C");
  G4Element * H  = nistman->FindOrBuildElement("H");
  G4Element * Br = nistman->FindOrBuildElement("Br");

  // Vacuum
  G4double z,a,density,fractionmass;
  G4int ncomponents;

  m_Vacuum = new G4Material("Vacuum", z=1 , a=1.01*g/mole, density= 0.0001*g/cm3);

  // Air
  m_Air = nistman->FindOrBuildMaterial("G4_AIR");

  // Al
  G4Material * Aluminium = nistman->FindOrBuildMaterial("G4_Al");

  // Epoxy for the PCB, based on type FR4
  G4Material * EpoxyFR4 = new G4Material("EpoxyFR4", density=1.7*g/cm3, ncomponents=5);
  EpoxyFR4->AddElement(Si,   fractionmass=0.180774);
  EpoxyFR4->AddElement(O,    fractionmass=0.405633);
  EpoxyFR4->AddElement(C,    fractionmass=0.278042);
  EpoxyFR4->AddElement(H,    fractionmass=0.0684428);
  EpoxyFR4->AddElement(Br,   fractionmass=0.0671091);

  // Si
  G4Material * Silicon = nistman->FindOrBuildMaterial("G4_Si");

  // Air is the default.  Can be changed from the messenger using
  // /allpix/extras/setWorldMaterial
  // which calls SimLucidDetectorConstruction::SetWorldMaterial(G4String mat)
  if (!m_userDefinedWorldMaterial) {
    m_fillingWorldMaterial = m_Vacuum;
  }
  else {
    if(m_fillingWorldMaterialName == "Vacuum") {
      G4cout << "User action --> World volume material : Vacuum " << endl;
      m_fillingWorldMaterial = m_Vacuum;
    }
    else if (m_fillingWorldMaterialName == "Air") {
      G4cout << "User action --> World volume material : Air    " << endl;
      m_fillingWorldMaterial = m_Air;
    }
    G4cout << "DEBUG: m_fillingWorldMaterial = " << m_fillingWorldMaterialName << endl;
  }

  G4double worldRad = 50.0;

  ////////////////////////////////////////////////////////////////////////////
  // The experimental Hall - space, in our case!
  G4VisAttributes * invisibleVisAtt = new G4VisAttributes(G4Color(1.0, 0.65, 0.0, 0.1));
  //
  invisibleVisAtt->SetVisibility(false);
  invisibleVisAtt->SetForceSolid(false);

  // Experimental hall (sphere)
  G4Sphere * expHall_sphere = new G4Sphere(
    "World",
    0.0*mm,
    worldRad*mm,
    0, 360.*degree,
    0, 360.*degree);
  expHall_log = new G4LogicalVolume(
    expHall_sphere, //expHall_box,
    m_fillingWorldMaterial,
    "World",
    0,0,0);
  //
  expHall_log->SetVisAttributes(invisibleVisAtt);
  //
  expHall_phys = new G4PVPlacement(
    0,
    G4ThreeVector(0.,0.,0.),
    expHall_log,
    "World",
    0x0,
    false,
  0);

  ///////////////////////////////////////////////////////////
  // Hemi-spherical pseudo-detector
  ///////////////////////////////////////////////////////////
  G4VisAttributes * pseudoDetectorVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 0.0, 0.1));
  
  //invisibleVisAtt->SetForceSolid(false);
  pseudoDetectorVisAtt->SetVisibility(true);
  pseudoDetectorVisAtt->SetForceSolid(true);

  G4Sphere * pseudoDetector_sphere = new G4Sphere(
    "PseudoDetector",
    (worldRad-0.2)*mm,
    worldRad*mm,
    0, 360.*degree,
    0, 360.*degree);

  G4Box * pseudoDetector_box 
    = new G4Box("PseudoDetectorCutBox", worldRad*mm, (worldRad/2.0)*mm, worldRad*mm);

  G4RotationMatrix * dummyRot = new G4RotationMatrix;
  G4ThreeVector posPDboxcut(0.0*mm,-(worldRad/2.0)*mm,0.0*mm);

  G4SubtractionSolid * pseudoDetector_hemi = new G4SubtractionSolid(
    "PDhemi",
    pseudoDetector_sphere, pseudoDetector_box,
    dummyRot, posPDboxcut);

  m_pseudoDetector_log = new G4LogicalVolume(
    pseudoDetector_hemi,
    m_Vacuum,
    "PseudoDetector_log"
    );
  m_pseudoDetector_log->SetVisAttributes(pseudoDetectorVisAtt);


  m_pseudoDetector_phys = new G4PVPlacement(
    0,
    G4ThreeVector(0.,0.,0.),
    m_pseudoDetector_log,
    "PseudoDetector_phys",
    expHall_log, // mother log
    false,
    0, // copy number
    true); // check overlap


  SimLucidPseudoDetectorSD * aPseudoDetectorSD =
    new SimLucidPseudoDetectorSD("PseudoDetectorSD");
  //

  ///////////////////////////////////////////////////////////
  // The Aluminium foil - note - to be replaced by
  // Al base plate
  G4VisAttributes * AlBasePlateVisAtt = new G4VisAttributes(G4Color(0.6, 0.6, 0.6, 1.0));
  AlBasePlateVisAtt->SetLineWidth(2);
  AlBasePlateVisAtt->SetForceSolid(true);
  AlBasePlateVisAtt->SetVisibility(true);
  //
  G4String AlBasePlateName("AlBasePlate");

  G4Box * AlBasePlate_box = new G4Box(
    "AlBasePlate_box",
    worldRad*mm,
    1.2*mm,
    worldRad*mm//m_AlBasePlateDz // 0.0174*mm
    );
  G4Sphere * AlBasePlate_sphere = new G4Sphere(
    "AlBasePlate_sphere",
     0.0*mm,
    worldRad*mm,
    0, 360.*degree,
    0, 360.*degree);

  G4ThreeVector posAlBasePlate(0.0*mm,-1.2*mm,0.0*mm);

  G4IntersectionSolid * AlBasePlate_round = new G4IntersectionSolid(
    "AlBasePlate_box*AlBasePlate_sphere",
    AlBasePlate_box, AlBasePlate_sphere,
    dummyRot, -posAlBasePlate); // Make sure the plate is moved down

  G4Box * AlBasePlate_hole = new G4Box(
    "AlBasePlate_hole",
    17.5*mm,
    1.3*mm,
    17.5*mm
    );
  G4SubtractionSolid * AlBasePlate_plate = new G4SubtractionSolid(
    "AlBasePlate_round-AlBasePlate_hole",
    AlBasePlate_round, AlBasePlate_hole);
       
  m_AlBasePlate_log = new G4LogicalVolume(
    AlBasePlate_plate,
    Aluminium,
    AlBasePlateName+"_log"
    );
  m_AlBasePlate_log->SetVisAttributes(AlBasePlateVisAtt);

  m_AlBasePlate_phys = new G4PVPlacement(
    0,
    posAlBasePlate,
    m_AlBasePlate_log,
    AlBasePlateName+"_phys",
    expHall_log, // mother log
    false,
    0, // copy number
    true
    ); // check overlap

  ///////////////////////////////////////////////////////////
  // The Aluminium support posts
  G4VisAttributes * SupportPostVisAtt = 
    new G4VisAttributes(G4Color(0.6, 0.6, 0.6, 1.0));
  SupportPostVisAtt->SetLineWidth(2);
  SupportPostVisAtt->SetForceSolid(true);
  SupportPostVisAtt->SetVisibility(true);
  //
  G4String SupportPostName("SupportPost");

  G4Box * SupportPost_box = new G4Box(
    "SupportPost_box",
     3.50*mm,
    18.25*mm,
     3.50*mm
    );

  m_SupportPost_log = new G4LogicalVolume(
    SupportPost_box,
    Aluminium,
    SupportPostName+"_log"
    );

  m_SupportPost_log->SetVisAttributes(SupportPostVisAtt);

  G4ThreeVector posSP12( 14.0*mm,11.85*mm, 14.0*mm);
  G4ThreeVector posSP23( 14.0*mm,11.85*mm,-14.0*mm);
  G4ThreeVector posSP34(-14.0*mm,11.85*mm,-14.0*mm);
  G4ThreeVector posSP41(-14.0*mm,11.85*mm, 14.0*mm);

  m_SupportPost12_phys = new G4PVPlacement(
    0,
    posSP12,
    m_SupportPost_log,
    SupportPostName+"12_phys",
    expHall_log, // mother log
    false,
    1, // copy number
    true); // check overlap
  m_SupportPost23_phys = new G4PVPlacement(
    0,
    posSP23,
    m_SupportPost_log,
    SupportPostName+"23_phys",
    expHall_log, // mother log
    false,
    2, // copy number
    true); // check overlap
  m_SupportPost34_phys = new G4PVPlacement(
    0,
    posSP34,
    m_SupportPost_log,
    SupportPostName+"34_phys",
    expHall_log, // mother log
    false,
    3, // copy number
    true); // check overlap
  m_SupportPost41_phys = new G4PVPlacement(
    0,
    posSP41,
    m_SupportPost_log,
    SupportPostName+"41_phys",
    expHall_log, // mother log
    false,
    4, // copy number
    true); // check overlap
	
  ///////////////////////////////////////////////////////////
  // The PCB (base)
  //
  G4VisAttributes * pcbVisAtt = new G4VisAttributes(G4Color(0.2, 0.5, 0.1, 1.0));
  pcbVisAtt->SetLineWidth(2);
  pcbVisAtt->SetForceSolid(true);
  pcbVisAtt->SetVisibility(true);
  //
  G4String PcbBaseName("PcbBase");
  
  G4Box * PcbBase_box = new G4Box(
    "PcbBase_box",
    worldRad * mm,
    1.2*mm,
    worldRad * mm//m_PcbBaseDz // 0.0174*mm
  );
  G4Sphere * PcbBase_sphere = new G4Sphere(
    "PcbBase_sphere",
    0.0*mm,
    worldRad * mm,
    0, 360.*degree,
    0, 360.*degree);
 
  G4ThreeVector posPcbBase(0.0*mm,-7.6*mm,0.0*mm);

  G4IntersectionSolid * PcbBase_round = new G4IntersectionSolid(
    "PcbBase_box*PcbBase_sphere",
    PcbBase_box, PcbBase_sphere,
    dummyRot, -posPcbBase); // Make sure the plate is moved down
       
  m_PcbBase_log = new G4LogicalVolume(
    PcbBase_round,
    EpoxyFR4,
    PcbBaseName+"_log"
    );
  m_PcbBase_log->SetVisAttributes(pcbVisAtt);

  m_PcbBase_phys = new G4PVPlacement(
    0,
    posPcbBase,
    m_PcbBase_log,
    PcbBaseName+"_phys",
    expHall_log, // mother log
    false,
    0, // copy number
    true
    ); // check overlap

  // Repeated, rotated geometry
  G4RotationMatrix * rot0 = new G4RotationMatrix;
  rot0->rotateX(M_PI/2.*rad);
  //
  G4RotationMatrix * rot1 = new G4RotationMatrix;
  //rot1->rotateY(0.0*rad);  
  //
  G4RotationMatrix * rot2 = new G4RotationMatrix;
  rot2->rotateY(-M_PI/2.*rad);  
  //
  G4RotationMatrix * rot3 = new G4RotationMatrix;
  rot3->rotateY(M_PI   *rad);
  //
  G4RotationMatrix * rot4 = new G4RotationMatrix;
  rot4->rotateY( M_PI/2.*rad);  

  ///////////////////////////////////////////////////////////
  // The Timepix PCB mounts
  //
  G4String TpxPcbName("TpxPcb");
  
  G4Box * TpxPcb_box = new G4Box(
    TpxPcbName+"_box",
    15.50*mm,
    12.90*mm,
     0.80*mm
  );

  m_TpxPcb_log = new G4LogicalVolume(
    TpxPcb_box,
    EpoxyFR4,
    TpxPcbName+"_log"
    );
  m_TpxPcb_log->SetVisAttributes(pcbVisAtt);

  G4ThreeVector posTP1(  0.0*mm, 18.3*mm, 18.3*mm);
  G4ThreeVector posTP2( 18.3*mm, 18.3*mm,  0.0*mm);
  G4ThreeVector posTP3(  0.0*mm, 18.3*mm,-18.3*mm);
  G4ThreeVector posTP4(-18.3*mm, 18.3*mm,  0.0*mm);

  m_TpxPcb1_phys = new G4PVPlacement(
    rot1,
    posTP1,
    m_TpxPcb_log,
    TpxPcbName+"1_phys",
    expHall_log, // mother log
    false,
    1, // copy number
    true); // check overlap
  m_TpxPcb2_phys = new G4PVPlacement(
    rot2,
    posTP2,
    m_TpxPcb_log,
    TpxPcbName+"2_phys",
    expHall_log, // mother log
    false,
    2, // copy number
    true); // check overlap
  m_TpxPcb3_phys = new G4PVPlacement(
    rot3,
    posTP3,
    m_TpxPcb_log,
    TpxPcbName+"3_phys",
    expHall_log, // mother log
    false,
    3, // copy number
    true); // check overlap
  m_TpxPcb4_phys = new G4PVPlacement(
    rot4,
    posTP4,
    m_TpxPcb_log,
    TpxPcbName+"4_phys",
    expHall_log, // mother log
    false,
    4, // copy number
    true); // check overlap

  ////////////////////////////////////////////////////////////////////////////
  // The Timepix silicon chips and sensor
  G4VisAttributes * TpxVisAtt = new G4VisAttributes(G4Color(0.2,0.25,0.2,1.0)); // Si colour?
  TpxVisAtt->SetLineWidth(2);
  TpxVisAtt->SetForceSolid(true);
  //TpxVisAtt->SetVisibility(false);
  TpxVisAtt->SetVisibility(true); // TW debug

  G4VisAttributes * TpxSVisAtt = new G4VisAttributes(G4Color(0.2,0.45,0.3,0.9)); // Si colour?
  TpxSVisAtt->SetLineWidth(2);
  TpxSVisAtt->SetForceSolid(true);
  //TpxSVisAtt->SetVisibility(false);
  TpxSVisAtt->SetVisibility(true); // TW debug

  G4String TpxName("Tpx");
  G4String TpxSName("Sensor"); 

  G4double siliconthickness = 1.5;
  G4double sensorthickness  = 0.3; // [mm] (300 um). 

  G4Box * Tpx_box = new G4Box(
    TpxName+"_box",
     8.00*mm,
     8.00*mm,
     (siliconthickness/2.)*mm
  );
  G4Box * TpxS_box = new G4Box(
    TpxSName+"_box",
     7.04*mm,
     7.04*mm,
     (sensorthickness/2.)*mm
  );
  G4ThreeVector posSensor(0.0,0.0,(siliconthickness-sensorthickness)/2.0);
  G4SubtractionSolid * Tpx_chip = new G4SubtractionSolid(
    TpxName+"_chip",
    Tpx_box, TpxS_box,
    dummyRot, posSensor);

  m_Tpx_log = new G4LogicalVolume(
    Tpx_chip,
    Silicon,
    TpxName+"_log"
    );
  m_Tpx_log->SetVisAttributes(TpxVisAtt);

  G4double pushsensor = 20.6 - siliconthickness/2.;

  G4ThreeVector posT0(        0.0*mm, -5.65*mm,        0.0*mm);
  G4ThreeVector posT1(        0.0*mm, 18.3 *mm, pushsensor*mm);
  G4ThreeVector posT2( pushsensor*mm, 18.3 *mm,        0.0*mm);
  G4ThreeVector posT3(        0.0*mm, 18.3 *mm,-pushsensor*mm);
  G4ThreeVector posT4(-pushsensor*mm, 18.3 *mm,        0.0*mm);

  m_Tpx0_phys = new G4PVPlacement(
    rot0,
    posT0,
    m_Tpx_log,
    TpxName+"0_phys",
    expHall_log, // mother log
    false,
    0, // copy number
    true); // check overlap
  m_Tpx1_phys = new G4PVPlacement(
    rot1,
    posT1,
    m_Tpx_log,
    TpxName+"1_phys",
    expHall_log, // mother log
    false,
    1, // copy number
    true); // check overlap
  m_Tpx2_phys = new G4PVPlacement(
    rot2,
    posT2,
    m_Tpx_log,
    TpxName+"2_phys",
    expHall_log, // mother log
    false,
    2, // copy number
    true); // check overlap
  m_Tpx3_phys = new G4PVPlacement(
    rot3,
    posT3,
    m_Tpx_log,
    TpxName+"3_phys",
    expHall_log, // mother log
    false,
    3, // copy number
    true); // check overlap
  m_Tpx4_phys = new G4PVPlacement(
    rot4,
    posT4,
    m_Tpx_log,
    TpxName+"4_phys",
    expHall_log, // mother log
    false,
    4, // copy number
    true); // check overlap

  // The Sensors
  m_TpxS_log = new G4LogicalVolume(
    TpxS_box,
    Silicon,
    TpxSName+"_log"
    );
  m_TpxS_log->SetVisAttributes(TpxSVisAtt);

  pushsensor += (siliconthickness/2. - sensorthickness/2.);

  G4ThreeVector posTS0(  0.0*mm, (-5.65 + siliconthickness/2. -sensorthickness/2.)*mm,  0.0*mm);
  G4ThreeVector posTS1(  0.0*mm, 18.3  *mm, pushsensor*mm);
  G4ThreeVector posTS2( pushsensor*mm, 18.3  *mm,  0.0*mm);
  G4ThreeVector posTS3(  0.0*mm, 18.3  *mm,-pushsensor*mm);
  G4ThreeVector posTS4(-pushsensor*mm, 18.3  *mm,  0.0*mm);

  m_TpxS0_phys = new G4PVPlacement(
    rot0,
    posTS0,
    m_TpxS_log,
    TpxSName+"0_phys",
    expHall_log,
    false,
    0,
    true);
  m_TpxS1_phys = new G4PVPlacement(
    rot1,
    posTS1,
    m_TpxS_log,
    TpxSName+"1_phys",
    expHall_log,
    false,
    1,
    true);
  m_TpxS2_phys = new G4PVPlacement(
    rot2,
    posTS2,
    m_TpxS_log,
    TpxSName+"2_phys",
    expHall_log,
    false,
    2,
    true);
  m_TpxS3_phys = new G4PVPlacement(
    rot3,
    posTS3,
    m_TpxS_log,
    TpxSName+"3_phys",
    expHall_log, 
    false,
    3,
    true);
  m_TpxS4_phys = new G4PVPlacement(
    rot4,
    posTS4,
    m_TpxS_log,
    TpxSName+"4_phys",
    expHall_log, 
    false,
    4,
    true); 
  

  // The Timepix sensitive detectors
  SimLucidTimepixDetectorSD * theTimepixDetectorsSD =
    new SimLucidTimepixDetectorSD("TimepixDetectorSD");

  ////////////////////////////////////////////////////////////////////////////
  // The Timepix foil covering
  G4VisAttributes * FoilVisAtt = new G4VisAttributes(G4Color(0.6, 0.6, 0.6, 1.0));
  FoilVisAtt->SetLineWidth(2);
  FoilVisAtt->SetForceSolid(true);
  //FoilVisAtt->SetVisibility(false);
  FoilVisAtt->SetVisibility(true);

  G4String FoilName("Foil");
  
  G4Box * Foil_box = new G4Box(
    FoilName+"_box",
     8.000*mm,
     8.000*mm,
     0.005*mm
  );

  m_Foil_log = new G4LogicalVolume(
    Foil_box,
    Silicon,
    FoilName+"_log"
    );
  m_Foil_log->SetVisAttributes(FoilVisAtt);

  G4double pushfoil = 22.15 - (siliconthickness);
  G4double downfoil =  3.35 + siliconthickness;

  G4ThreeVector posF0(     0.00*mm, -downfoil*mm,     0.00*mm);
  G4ThreeVector posF1(     0.00*mm,     18.30*mm, pushfoil*mm);
  G4ThreeVector posF2( pushfoil*mm,     18.30*mm,     0.00*mm);
  G4ThreeVector posF3(     0.00*mm,     18.30*mm,-pushfoil*mm);
  G4ThreeVector posF4(-pushfoil*mm,     18.30*mm,     0.00*mm);

  m_Foil0_phys = new G4PVPlacement(
    rot0,
    posF0,
    m_Foil_log,
    FoilName+"0_phys",
    expHall_log, 
    false,
    0,
    true); 
  m_Foil1_phys = new G4PVPlacement(
    rot1,
    posF1,
    m_Foil_log,
    FoilName+"1_phys",
    expHall_log, 
    false,
    1, 
    true); 
  m_Foil2_phys = new G4PVPlacement(
    rot2,
    posF2,
    m_Foil_log,
    FoilName+"2_phys",
    expHall_log, 
    false,
    2, 
    true); 
  m_Foil3_phys = new G4PVPlacement(
    rot3,
    posF3,
    m_Foil_log,
    FoilName+"3_phys",
    expHall_log,
    false,
    3,
    true);
  m_Foil4_phys = new G4PVPlacement(
    rot4,
    posF4,
    m_Foil_log,
    FoilName+"4_phys",
    expHall_log, 
    false,
    4, 
    true); 

  ////////////////////////////////////////////////////////////////////////////
  // The Dome wall
  G4VisAttributes * DomeVisAtt = new G4VisAttributes(G4Color(0.6, 0.6, 0.6, 0.3));
  DomeVisAtt->SetLineWidth(2);
  DomeVisAtt->SetForceSolid(true);
  //DomeVisAtt->SetVisibility(false);
  DomeVisAtt->SetVisibility(true);

  G4String DomeWallName("DomeWall");
  
  G4Tubs * DomeWall_cyl = new G4Tubs(
    DomeWallName+"_box",
    30.00*mm,
    30.70*mm,
    16.50*mm,
    0.000,                               // Starting point for segment.
    2.0*pi                               // Angle (i.e. full tube).
    );

  m_DomeWall_log = new G4LogicalVolume(
    DomeWall_cyl,
    Aluminium,
    DomeWallName+"_log"
    );
  m_DomeWall_log->SetVisAttributes(DomeVisAtt);

  G4RotationMatrix * domeWallRot = new G4RotationMatrix;
  domeWallRot->rotateX(M_PI/2.*rad);
  G4ThreeVector domeWallPos = G4ThreeVector(0.0*mm,16.5*mm,0.0*mm);

  m_DomeWall_phys = new G4PVPlacement(
    domeWallRot,
    domeWallPos,
    m_DomeWall_log,
    DomeWallName+"_phys",
    expHall_log, 
    false,
    0,
    true);

  ////////////////////////////////////////////////////////////////////////////
  // The Dome top
  G4String DomeTopName("DomeTop");
  
  G4Tubs * DomeTop_cyl = new G4Tubs(
    DomeTopName+"_box",
     0.00*mm,
    30.70*mm,
     0.35*mm,
    0.000,                               // Starting point for segment.
    2.0*pi                               // Angle (i.e. full tube).
    );

  m_DomeTop_log = new G4LogicalVolume(
    DomeTop_cyl,
    Aluminium,
    DomeTopName+"_log"
    );
  m_DomeTop_log->SetVisAttributes(DomeVisAtt);

  G4RotationMatrix * domeTopRot = new G4RotationMatrix;
  domeTopRot->rotateX(M_PI/2.*rad);
  G4ThreeVector domeTopPos = G4ThreeVector(0.0*mm,33.35*mm,0.0*mm);

  m_DomeTop_phys = new G4PVPlacement(
    domeTopRot,
    domeTopPos,
    m_DomeTop_log,
    DomeTopName+"_phys",
    expHall_log, 
    false,
    0, 
    true); 

  //--------------------------------------------------------------------------
  // SD manager
  //--------------------------------------------------------------------------
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector(     aPseudoDetectorSD );
  SDman->AddNewDetector( theTimepixDetectorsSD );
  m_pseudoDetector_log->SetSensitiveDetector(     aPseudoDetectorSD );	
  m_TpxS_log->SetSensitiveDetector(           theTimepixDetectorsSD );	
  
  return expHall_phys;

}//end of Construct() method.


//
// Set the world material (via the command file).
//
void SimLucidDetectorConstruction::SetWorldMaterial(G4String mat) {

  m_fillingWorldMaterialName = mat;
  m_userDefinedWorldMaterial = true;

}
