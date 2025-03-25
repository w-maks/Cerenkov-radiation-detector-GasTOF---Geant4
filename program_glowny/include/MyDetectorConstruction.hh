
#ifndef MYDETECTORCONSTRUCTION_HH
#define MYDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Material.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;

class MyDetectorConstruction : public G4VUserDetectorConstruction {

public:
  
  // Constructor
  MyDetectorConstruction();
  
  // Destructor
  virtual ~MyDetectorConstruction();
  
  // Method
  virtual G4VPhysicalVolume* Construct();
  
private:

  void DefineMaterials();
  void SetupGeometry();
  void SetupSensitivePMT(G4LogicalVolume* pmtLogical); 


  
  //World logical and physical volumes
  G4LogicalVolume*   fpWorldLogical;
  G4VPhysicalVolume* fpWorldPhysical;
  G4Material* C4F10;           
  G4Material* air;            
  G4Material* vacuum;         
  G4Material* mirrorMaterial; 
  G4Material* photocathodeMaterial;
  //G4Material* galactic;    
  //G4Material* pmtWallsMaterial;
  
};

#endif

