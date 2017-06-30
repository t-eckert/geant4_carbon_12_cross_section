// The zero of the z-axis is the point where the LS hemisphere meets the PMT face. 
// The positive z-axis points towards the NaI.
// For now, the LS hemisphere is just a cylinder of UGAB
// The NaI surface is 2.86 cm above PMT face (z=0).
//
// Define kB here.
// Define LS efficiency in Run (detEff).
//
// TO DO: Move kB to run action and make user interface
// TO DO: Add glass vial around LS Cocktail
//
#include "LTAC1DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4SDManager.hh"
#include "LTAC1SD.hh"
#include "G4ios.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4Colour.hh"
#include "G4Element.hh"


LTAC1DetectorConstruction::LTAC1DetectorConstruction()
 :  Room_log(0), Room_phys(0), NaI1_log(), NaI2_log(), NaI1_phys(), NaI2_phys()
{ 

	 NaI_radius = 7.62*cm/2.0;				// 3 inch x 3 inch NaI
	 NaI_half_height = 7.62*cm /2.0;
	 NaIWallThickness = 0.508*mm;				// Al wall of NaI detctor

	 //SetSourceInnerRadius(0.0*cm/2);		// Source inner radius
	 //SetSourceRadius(7.62/2*cm);			// Source outer radius
	 //SetSourceHalfHeight(0.759*cm/2);		// Half of source height three-disk = 0.756*cm/2; two-disk = 0.507cm/2
	 SetSourceXoffset(3.81*cm);				// Source center position
	 SetSourceYoffset(0.0*cm);
	 SetSourceZoffset(0.0*cm);			//Offset for three-disk = 0.4485*cm; two-disk = 0.2789*cm
	 SetMaterialName("Graphite");

	 //Aluminized Mylar
	 MylarOuterRadius = 2.38*cm/2;
	 MylarHalfHeight = 0.254*mm/2;
	 MylarOffset = MylarHalfHeight;
	 MylarxOffset = GetSourceXoffset();

	 //Kapton Film
	 KaptonOuterRadius = 2.38*cm/2;
	 KaptonHalfHeight = .071*mm/2;
	 KaptonOffset = -KaptonHalfHeight;
	 KaptonxOffset = GetSourceXoffset();

	 //Graphite Disk 1 (Closer to NaI 1)
	 C1OuterRadius = 7.62/2*cm;
	 C1HalfHeight = 0.254*cm/2;			
	 C1Zoffset = C1HalfHeight +2*MylarHalfHeight;	 

	 //Graphite Disk 2 (Closer to NaI 2)
	 C2OuterRadius = 7.62/2*cm;
	 C2HalfHeight = 0.254*cm/2;				
	 C2Zoffset = -C2HalfHeight -2*KaptonHalfHeight;// -MylarHalfHeight;		

	 //Graphite Disk with Hole in Center
	 CwHInnerRadius = 2.38*cm/2;			
	 CwHOuterRadius = 7.62/2*cm;
	 CwHThickness = 1.289/2*mm;
	 CwHoffset = 0.0*mm;

	 NaIReflectorThickness = 2.7*mm;				// Thickness of barium oxide or other reflector

	 DetectorGap = 2*C1HalfHeight +2*C2HalfHeight +2*MylarHalfHeight +2*KaptonHalfHeight;	 
	 SetDetectorGap(DetectorGap);		// gap between detectors. Can be overwritten in batch.mac;

	 NaIShift = 0.00915*cm;
	;}


LTAC1DetectorConstruction::~LTAC1DetectorConstruction()
{ delete Room_log, Room_phys,NaI1_log, NaI2_log, NaI1_phys, NaI2_phys;
;
}

G4VPhysicalVolume* LTAC1DetectorConstruction::Construct()
{

  G4double a;				// atomic mass
  G4double z;				// atomic number
  G4double density;			// density of a substance
  G4double fractionmass;	// fraction by mass of a mixture
  G4int ncomponents;		// number of components of a mixture or compound

  G4NistManager* manager = G4NistManager::Instance(); // NIST substances

  G4Element* elC = manager->FindOrBuildElement("C");
  G4Element* elN = manager->FindOrBuildElement("N");
  G4Element* elNa = manager->FindOrBuildElement("Na");
  G4Element* elI = manager->FindOrBuildElement("I");
  G4Element* elTl = manager->FindOrBuildElement("Tl");
  G4Element* elAl = manager->FindOrBuildElement("Al");
  G4Element* elH = manager->FindOrBuildElement("H");
  G4Element *elO = manager->FindOrBuildElement("O");
  G4Element* elBa = manager->FindOrBuildElement("Ba");
 
  G4Material* Al = new G4Material("Aluminum", z= 13., a= 26.98*g/mole, density= 2.7*g/cm3);

  G4Material *N2 = new G4Material("N2", density=1.2*mg/cm3,ncomponents=1);
  N2->AddElement(elN,2);

  G4Material *Graphite = new G4Material("Graphite", density=1.8*g/cm3,ncomponents=1);
  Graphite->AddElement(elC,1);

  G4Material *Mylar = new G4Material("Mylar", density = 0.999*g/cm3, ncomponents = 4);
  Mylar->AddElement(elH, 8);
  Mylar->AddElement(elC, 10);
  Mylar->AddElement(elO, 4);
  Mylar->AddElement(elAl, 3);

  G4Material *Kapton = new G4Material("Kapton", density = 1.42*g/cm3, ncomponents = 4);
  Kapton->AddElement(elH, 5);
  Kapton->AddElement(elC, 11);
  Kapton->AddElement(elN, 1);
  Kapton->AddElement(elO, 3);

   	G4Material* BaO = new G4Material("BaO",density=3.0*g/cm3,ncomponents=2);
	BaO->AddElement(elBa,1);
	BaO->AddElement(elO,1);


  //G4Material *Scintillator = new G4Material("Scintillator", density=1.032*g/cm3, ncomponents=2);
  //Scintillator->AddElement(elC, 27);
  //Scintillator->AddElement(elH, 30);  



 // DEFINE NaI(Tl) ------------------//
	G4Material* NaI = new G4Material("NaI", density = 3.67*g/cm3, ncomponents = 2);
	NaI->AddElement(elNa,1);
	NaI->AddElement(elI,1);

	G4Material* TlMetal = new G4Material("TlMetal", density = 12.*g/cm3, ncomponents = 1);
	TlMetal->AddElement(elTl,1.);

	G4Material* NaI_Tl = new G4Material("NaI_Tl", density = 3.67*g/cm3, ncomponents = 2);
	NaI_Tl->AddMaterial(NaI,0.999);
	NaI_Tl->AddMaterial(TlMetal,0.001);


 //----DEFINE Volumes, starting with room box, in which detectors sit----//
// Geometry (Box, Tubs...), which gets associated with a,
// Logical volume, in which material is specified, and which gets associated with a,
// Physical Volume, which gets placed in a parent volume (hall is its own volume).
//
  x_pos = .10*m;
  y_pos = .10*m;
  z_pos = .25*m;
  G4Box* Room_box = new G4Box( "Room_box", x_pos, y_pos, z_pos); 
  Room_log = new G4LogicalVolume(Room_box, N2, "Room_log", 0, 0, 0); // Fill with N2 gas
  Room_phys = new G4PVPlacement(0, G4ThreeVector(), Room_log, "Room", 0, false, 0);

 /*//---------------BEGIN--- Graphite Disk ----------------------//

  inner_radius = GetSourceInnerRadius();
  outer_radius = source_radius;
  half_height = source_half_height;
  G4Tubs* Source = new G4Tubs("Disk1", inner_radius, outer_radius, half_height, 0.0, 360.*deg);
  Source_log = new G4LogicalVolume(Source, Graphite, "Source_log", 0, 0, 0);
  x_pos = source_x_offset;
  y_pos = source_y_offset;
  z_pos = source_z_offset;
  Source_phys = new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, z_pos), Source_log, "Source", Room_log, false, 0);
   G4VisAttributes Source_Att(G4Color::Yellow());
   Source_Att.SetForceWireframe(false);
   Source_Att.SetForceSolid(true);
   Source_Att.SetForceAuxEdgeVisible(true);
   Source_Att.SetVisibility(true);
   Source_log -> SetVisAttributes (Source_Att);

 //---------------END--- Graphite Disk ----------------------//*/

 //===============BEGIN=First Graphite Disk========================//

   G4Tubs *Disk1 = new G4Tubs("Disk1", 0.0, C1OuterRadius, C1HalfHeight, 0.0, 360.*deg);
   Disk1_log = new G4LogicalVolume(Disk1, Graphite, "Disk1_log", 0, 0, 0);
   Disk1_phys = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, C1Zoffset), Disk1_log, "Disk1", Room_log, false, 0);
   G4VisAttributes Disk1_Att(G4Color::Yellow());
   Disk1_Att.SetForceWireframe(false);
   Disk1_Att.SetForceSolid(true);
   Disk1_Att.SetVisibility(true);
   Disk1_log -> SetVisAttributes (Disk1_Att);

 //===============END===First Graphite Disk========================//

 //===============BEGIN=Second Graphite Disk========================//

   G4Tubs *Disk2 = new G4Tubs("Disk2", 0.0, C2OuterRadius, C2HalfHeight, 0.0, 360.*deg);
   Disk2_log = new G4LogicalVolume(Disk2, Graphite, "Disk2_log", 0, 0, 0);
   Disk2_phys = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, C2Zoffset), Disk2_log, "Disk2", Room_log, false, 0);
   G4VisAttributes Disk2_Att(G4Color::Yellow());
   Disk2_Att.SetForceWireframe(false);
   Disk2_Att.SetForceSolid(true);
   Disk2_Att.SetVisibility(true);
   Disk2_log -> SetVisAttributes (Disk2_Att);

 //===============END===Second Graphite Disk========================//

/*//===============BEGIN=Graphite Disk With Hole========================//
   
   G4Tubs *DiskwH = new G4Tubs("DiskwH", 0.0, CwHOuterRadius, CwHThickness, 0.0, 360.*deg);
   DiskwH_log = new G4LogicalVolume(DiskwH, Graphite, "DiskwH_log", 0, 0, 0);
   DiskwH_phys = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, CwHoffset), DiskwH_log, "DiskwH", Room_log, false, 0);
   G4VisAttributes DiskwH_Att(G4Color::Green());
   DiskwH_Att.SetForceWireframe(false);
   DiskwH_Att.SetForceSolid(true);
   DiskwH_Att.SetVisibility(true);
   DiskwH_log -> SetVisAttributes (DiskwH_Att);
   
 //===============END===Graphite Disk with Hole=========================//*/

 //===============BEGIN=Mylar========================//
   
   G4Tubs *MylarDisk = new G4Tubs("MylarDisk", 0.0, MylarOuterRadius, MylarHalfHeight, 0.0, 360.*deg);
   Mylar_log = new G4LogicalVolume(MylarDisk, Mylar, "Mylar_log", 0, 0, 0);
   Mylar_phys = new G4PVPlacement(0, G4ThreeVector(MylarxOffset, 0.0, MylarOffset), Mylar_log, "MylarDisk", Room_log, false, 0);
   G4VisAttributes Mylar_Att(G4Color::Blue());
   Mylar_Att.SetForceWireframe(false);
   Mylar_Att.SetForceSolid(true);
   Mylar_Att.SetVisibility(true);
   Mylar_log -> SetVisAttributes (Mylar_Att);
   
 //===============END===Mylar========================//

 //===============BEGIN=Kapton========================//
   
   G4Tubs *KaptonDisk = new G4Tubs("KaptonDisk", 0.0, KaptonOuterRadius, KaptonHalfHeight, 0.0, 360.*deg);
   Kapton_log = new G4LogicalVolume(KaptonDisk, Kapton, "Kapton_log", 0, 0, 0);
   Kapton_phys = new G4PVPlacement(0, G4ThreeVector(KaptonxOffset, 0.0, KaptonOffset), Kapton_log, "KaptonDisk", Room_log, false, 0);
   G4VisAttributes Kapton_Att(G4Color::Gray());
   Kapton_Att.SetForceWireframe(false);
   Kapton_Att.SetForceSolid(true);
   Kapton_Att.SetVisibility(true);
   Kapton_log -> SetVisAttributes (Kapton_Att);
   
 //===============END===Kapton========================//


 //---------------BEGIN--- Geneseo NaI ----------------------//
 // Include Al housing in design, but not in sensitive detector //
 // BEGIN NaI
  inner_radius = 0.0;
  outer_radius = NaI_radius;	
  half_height = NaI_half_height;
  G4Tubs* NaI1 = new G4Tubs("NaI1", inner_radius, outer_radius, half_height, 0.0, 360.*deg);
  G4Tubs* NaI2 = new G4Tubs("NaI2",inner_radius, outer_radius, half_height,0.0 ,360.*deg);
  NaI1_log = new G4LogicalVolume(NaI1, NaI_Tl, "NaI1_log", 0, 0, 0);
  NaI2_log = new G4LogicalVolume(NaI2,NaI_Tl,"NaI2_log",0,0,0);

  x_pos = 0.0*m;
  y_pos = 0.*m;
  z_pos = NaI_half_height + gap/2.0 +NaIWallThickness +NaIReflectorThickness +NaIShift; // For NaI cylinders and Al side wall cylinders

  NaI1_phys = new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, z_pos), NaI1_log, "NaI1", Room_log, false, 0);
  
  z_pos = 0-z_pos +2*NaIShift;
  NaI2_phys = new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, z_pos), NaI2_log, "NaI2", Room_log, false, 0);
  // END NaI

  //------ BEGIN REFLECTOR  ------
  inner_radius = 0.0;
  outer_radius = NaI_radius+NaIReflectorThickness;	
  half_height = NaI_half_height+NaIReflectorThickness;
  G4Tubs* RefSolid = new G4Tubs("RefSolid",inner_radius, outer_radius,half_height,0.0 ,360.*deg);
  outer_radius = NaI_radius;
  half_height = NaI_half_height;
  G4Tubs* RefOut = new G4Tubs("RefOut",inner_radius, outer_radius,half_height,0.0 ,360.*deg);

  Translation = new G4ThreeVector(0,0,0);
  Rotation = new G4RotationMatrix(0.,0.,0.);
  G4VSolid * reflectorShell = new G4SubtractionSolid("reflectorShell", RefSolid, RefOut, Rotation, *Translation);
 
  NaI1Reflector_log = new G4LogicalVolume(reflectorShell,BaO,"reflectorShell_log",0,0,0);
  NaI2Reflector_log = new G4LogicalVolume(reflectorShell,BaO,"reflectorShell_log",0,0,0);

  x_pos = 0.0*m;
  y_pos = 0.*m;
  z_pos = NaI_half_height+gap/2.0+NaIReflectorThickness+NaIWallThickness +NaIShift; // For NaI cylinders and Al side wall cylinders

  NaI1Reflector_phys = new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, z_pos),
                                     NaI1Reflector_log,"NaI1Reflector",Room_log,false,0);
  
  z_pos = 0-z_pos +2*NaIShift;
  NaI2Reflector_phys = new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, z_pos),
                                     NaI2Reflector_log,"NaI2Reflector",Room_log,false,0);


  G4VisAttributes   NaIReflector_Att(G4Color::Green());
  NaIReflector_Att.SetVisibility(true);
  NaIReflector_Att.SetForceWireframe(true);
  NaIReflector_Att.SetForceAuxEdgeVisible(true);
  NaIReflector_Att.SetForceSolid(false);
  NaI1Reflector_log->SetVisAttributes (NaIReflector_Att);
  NaI2Reflector_log->SetVisAttributes (NaIReflector_Att);

// ------ END REFLECTOR ------
 
  // BEGIN AL HOUSING
  G4double housing_gap = 2.5*mm; //This is the gap between the 3x3 inch crystal and the aluminum housing (not on faces). 
  inner_radius = 0.0;
  outer_radius = NaI_radius +NaIWallThickness +housing_gap;	
  half_height = NaI_half_height +NaIWallThickness;
  G4Tubs* alSolid = new G4Tubs("alSolid", inner_radius, outer_radius, half_height, 0.0, 360.*deg);
  outer_radius = NaI_radius;
  half_height = NaI_half_height;
  G4Tubs* cutOut = new G4Tubs("cutOut", inner_radius, outer_radius, half_height, 0.0, 360.*deg);

  Translation = new G4ThreeVector(0,0,0);
  Rotation = new G4RotationMatrix(0.,0.,0.);
  G4VSolid * aluminumShell = new G4SubtractionSolid("aluminumShell", alSolid, cutOut, Rotation, *Translation);
  // ALUMINUM SIDE WALLS
 
  NaI1Wall_log = new G4LogicalVolume(aluminumShell,Al,"alumimumShell_log",0,0,0);
  NaI2Wall_log = new G4LogicalVolume(aluminumShell,Al,"alumimumShell_log",0,0,0);

  x_pos = 0.0*m;
  y_pos = 0.*m;
  z_pos = NaI_half_height + gap/2.0 +NaIWallThickness +NaIShift; // For NaI cylinders and Al side wall cylinders 

  NaI1Wall_phys = new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, z_pos),
                                     NaI1Wall_log, "NaI1Wall", Room_log, false, 0);
  
 z_pos = 0-z_pos +2*NaIShift;
 NaI2Wall_phys = new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, z_pos),
                                     NaI2Wall_log,"Na2Wall",Room_log,false,0);




  G4VisAttributes NaI1_Att(G4Color::Blue());
  NaI1_Att.SetForceWireframe(true);
  NaI1_Att.SetForceSolid(true);
  NaI1_Att.SetForceAuxEdgeVisible(true);
  NaI1_Att.SetVisibility(true);
  NaI1_log -> SetVisAttributes (NaI1_Att);
  NaI2_log -> SetVisAttributes (NaI1_Att);

  G4VisAttributes NaIWall_Att(G4Color::Red());
  NaIWall_Att.SetVisibility(true);
  NaIWall_Att.SetForceWireframe(true);
  NaIWall_Att.SetForceAuxEdgeVisible(true);
  NaIWall_Att.SetForceSolid(true);
  NaI1Wall_log->SetVisAttributes (NaIWall_Att);
  NaI2Wall_log->SetVisAttributes (NaIWall_Att);

  inner_radius = NaI_radius;
  outer_radius = NaI_radius +NaIWallThickness;
  half_height = NaI_half_height +NaIWallThickness;


//---------------END--- GENESEO NaI ----------------------//


////==..==..==..==..==..==..==..==..==..==..==..==..==//
////   DEFINE SENSITIVE DETECTORS for HITS            //
////==..==..==..==..==..==..==..==..==..==..==..==..==//

	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	
	G4String mySDName = "LTAC1/NaI1SD";
	G4String myCollection = "NaI1Collection";
	LTAC1SD* myLTAC1SD = new LTAC1SD( mySDName, myCollection );
	SDman->AddNewDetector( myLTAC1SD );
	NaI1_log->SetSensitiveDetector( myLTAC1SD );	// Associate logical volume with sensitive detector.

	mySDName = "LTAC1/NaI2SD";
	myCollection = "NaI2Collection";
	myLTAC1SD = new LTAC1SD( mySDName, myCollection );
	SDman->AddNewDetector( myLTAC1SD );
	NaI2_log->SetSensitiveDetector( myLTAC1SD );			// Associate logical volume with sensitive detector. */


  return Room_phys; // Send the room and its contents to the Kernel.
}

