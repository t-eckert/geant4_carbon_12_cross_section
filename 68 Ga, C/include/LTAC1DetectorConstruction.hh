

#ifndef LTAC1DetectorConstruction_H
#define LTAC1DetectorConstruction_H 1

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Material.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;

class LTAC1DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    LTAC1DetectorConstruction();
    ~LTAC1DetectorConstruction();

    G4VPhysicalVolume* Construct();
	void SetSourceInnerRadius(G4double rad) { source_inner_radius=rad; };
	G4double GetSourceInnerRadius() { return source_inner_radius; };

	void SetSourceRadius(G4double rad) { source_radius=rad; };
	G4double GetSourceRadius() { return source_radius; };

	void SetSourceHalfHeight(G4double rad) { source_half_height=rad; };
	G4double GetSourceHalfHeight() { return source_half_height; };

	G4double GetSourceZoffset() { return source_z_offset; };
	void SetSourceZoffset(G4double offset) { source_z_offset = offset; };

	G4double GetSourceXoffset() { return source_x_offset; };
	void SetSourceXoffset(G4double offset) { source_x_offset = offset; };

	G4double GetSourceYoffset() { return source_y_offset; };
	void SetSourceYoffset(G4double offset) { source_y_offset = offset; };

	G4double GetDetectorGap() { return gap; };
	void SetDetectorGap(G4double theGap) { gap = theGap; };

	G4String GetMaterialName() { return DetMaterialName; };
	void SetMaterialName(const G4String MaterialName) { DetMaterialName = MaterialName; };

  private:

	 G4double source_inner_radius;		// use setters and getters
	 G4double source_radius;			// use setters and getters
	 G4double source_half_height;		// use setters and getters
	 G4double source_z_offset;			// use setters and getters
	 G4double source_x_offset;			// use setters and getters
	 G4double source_y_offset;			// use setters and getters
	 G4String DetMaterialName;			// use setters and getters

	 G4double NaIWallThickness;			// Geneseo
	 G4double NaI_radius;				// Geneseo
	 G4double NaI_half_height;			// Geneseo
	 G4double gap;						// gap between detectors

	 //Aluminized Mylar
	 G4double MylarOuterRadius;
	 G4double MylarHalfHeight;
	 G4double MylarOffset;
	 G4double MylarxOffset;

	 //Kapton Film
	 G4double KaptonOuterRadius;
	 G4double KaptonHalfHeight;
	 G4double KaptonOffset;
	 G4double KaptonxOffset;

	 //First Graphite Disk
	 G4double C1OuterRadius;				
	 G4double C1HalfHeight;
	 G4double C1Zoffset;

	 //Second Graphite Disk
	 G4double C2OuterRadius;				
	 G4double C2HalfHeight;
	 G4double C2Zoffset;

	 //Graphite with Hole
	 G4double CwHInnerRadius;			
	 G4double CwHOuterRadius;
	 G4double CwHThickness;
	 G4double CwHoffset;

	 G4double DetectorGap;
	 G4double NaIShift;
	 //G4double NaI2Shift;

	 G4double NaIReflectorThickness;

	// REUSABLE DIMENSIONS

	G4double inner_radius;	// generic dimension, used various times for making objects.
	G4double outer_radius;	// etc.
	G4double height;
	G4double half_height;

	G4double x_pos, y_pos, z_pos;	// positions of items
	G4ThreeVector  * Translation;
	G4RotationMatrix * Rotation;
	
	// LOGICAL VOLUMES
    G4LogicalVolume* Room_log;	// Matriarch logical volume

	G4LogicalVolume* NaI1Wall_log;
	G4LogicalVolume* NaI2Wall_log;
   	G4LogicalVolume* NaI1_log;				// NaI Logical volume, linked to NaI1 Sensitive Detector
	G4LogicalVolume* NaI2_log;				// NaI Logical volume, linked to NaI2 Sensitive Detector
	G4LogicalVolume* Source_log;			// Source logical volume
	G4LogicalVolume* Disk1_log;
	G4LogicalVolume* Disk2_log;
	G4LogicalVolume* DiskwH_log;
	G4LogicalVolume* Mylar_log;
	G4LogicalVolume* Kapton_log;
	G4LogicalVolume* NaI1Reflector_log;
	G4LogicalVolume* NaI2Reflector_log;

    // PHYSICAL VOLUMES
    G4VPhysicalVolume* Room_phys;	// physical volume
	G4VPhysicalVolume* NaI1Wall_phys;
	G4VPhysicalVolume* NaI2Wall_phys;
	G4VPhysicalVolume* NaI1_phys;			// NaI1 (solid cylinder)
	G4VPhysicalVolume* NaI2_phys;			// NaI2 (solid cylinder)
	G4VPhysicalVolume* Source_phys;			// Source physical volume
	G4VPhysicalVolume* Disk1_phys;
	G4VPhysicalVolume* Disk2_phys;
	G4VPhysicalVolume* DiskwH_phys;
	G4VPhysicalVolume* Mylar_phys;
	G4VPhysicalVolume* Kapton_phys;
	G4VPhysicalVolume* NaI1Reflector_phys;
	G4VPhysicalVolume* NaI2Reflector_phys;
};

// Keep at the end
#endif //

