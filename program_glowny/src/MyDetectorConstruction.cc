#include "MyDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4RunManager.hh"
#include "math.h"
#include "G4SDManager.hh"
#include "G4OpticalSurface.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4UserLimits.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4PSFlatSurfaceCurrent.hh"
#include "G4ExtrudedSolid.hh" 
#include "MyPhotonSD.hh"


MyDetectorConstruction::MyDetectorConstruction()
  : fpWorldLogical(0),
    fpWorldPhysical(0),
    C4F10(nullptr),
    air(nullptr),
    vacuum(nullptr),
    mirrorMaterial(nullptr),
    photocathodeMaterial(nullptr)
    //pmtWallsMaterial(nullptr),
    //galactic(nullptr)
{}

MyDetectorConstruction::~MyDetectorConstruction() {}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
    DefineMaterials();  
    SetupGeometry();   
    return fpWorldPhysical;  
}

void MyDetectorConstruction::DefineMaterials()
{
  G4String symbol;
  G4double a,z,density;
  G4int ncomponents, natoms;
  G4double fractionmass;
  /////////////////tworzenie C4F10///////////////////////////

  //pierwiastki
  G4Element* C = new G4Element("Carbon", "C", 6., 12.01 * g/mole);
  G4Element* F = new G4Element("Fluorine", "F", 9., 19.00 * g/mole);
  //parametry
  G4double densityg = 0.00973 * g/cm3;
  G4double pressure = 1.2 * atmosphere;
  G4double temperature = 293.15 * kelvin;
  //tworzenie
  C4F10 = new G4Material("C4F10", densityg, 2, kStateGas, temperature, pressure);
  C4F10->AddElement(C, 4);
  C4F10->AddElement(F, 10);
  //nadanie parametrow optycznych
  G4MaterialPropertiesTable* mptC4F10 = new G4MaterialPropertiesTable();
  G4int num = 16;
  G4double energy[num] = { 2.88*eV, 3.25*eV, 3.62*eV, 3.98*eV, 4.35*eV, 4.72*eV, 5.09*eV, 5.46*eV, 
  5.83*eV, 6.20*eV, 6.52*eV, 6.88*eV, 7.29*eV, 7.50*eV, 7.60*eV, 7.70*eV };
  G4double n[num];
  for (int i = 0; i < num; i++) { n[i] = 1.018; }
  mptC4F10->AddProperty("RINDEX", energy, n, num, true, true);
  C4F10->SetMaterialPropertiesTable(mptC4F10);

  /////////////////NistManager//////////////////////////

  G4NistManager* nist = G4NistManager::Instance();
  //powietrze
  air= nist->FindOrBuildMaterial("G4_AIR");
  vacuum = new G4Material("Vacuum", density=1.e-5*g/cm3, ncomponents=1, kStateGas, STP_Temperature, 2.e-2*bar);
  vacuum->AddMaterial(air, fractionmass=1.);
  //material lustra
  mirrorMaterial = nist->FindOrBuildMaterial("G4_Al");
  //nadanie wlasciwosci optycznych materialowi lustra
  G4MaterialPropertiesTable* mptMirror = new G4MaterialPropertiesTable();
  G4double REFLECTIVITY[num] = {0.999970, 0.999950, 0.999940, 0.999910, 0.999870, 0.999860, 0.999850, 0.999840, 
  0.999840, 0.999830, 0.999820, 0.999810, 0.999800, 0.999795, 0.999790, 0.999785};
  mptMirror->AddProperty("REFLECTIVITY", energy, REFLECTIVITY, num);
  mirrorMaterial->SetMaterialPropertiesTable(mptMirror);
  
  //material fotokatody
  G4Element* K = nist->FindOrBuildElement("K"); // Potas
  G4Element* Cs = nist->FindOrBuildElement("Cs"); // Cez
  G4Element* Sb = nist->FindOrBuildElement("Sb"); // Antymon

  photocathodeMaterial = new G4Material("BialkaliCathode", 3.0*g/cm3, 3);
  photocathodeMaterial->AddElement(K, 2); // 2 części Potasu
  photocathodeMaterial->AddElement(Cs, 1); // 1 część Cezu
  photocathodeMaterial->AddElement(Sb, 1); // 1 część Antymonu
  G4MaterialPropertiesTable* mptCathode = new G4MaterialPropertiesTable();
  G4double energy2[30] = { 0.01*eV, 0.05*eV, 0.10*eV, 0.50*eV, 1.00*eV, 1.50*eV, 1.75*eV, 2.00*eV, 2.50*eV, 2.88*eV, 
    3.25*eV, 3.62*eV, 3.98*eV, 4.35*eV, 4.72*eV, 5.09*eV, 5.46*eV, 5.83*eV, 6.20*eV, 6.52*eV, 
    6.88*eV, 7.29*eV, 7.50*eV, 7.60*eV, 7.70*eV, 7.80*eV, 8.00*eV, 8.50*eV, 9.00*eV, 10.00*eV };
  G4double EFFICIENCY[30];
  for(int i=0; i<30; i++){
      EFFICIENCY[i]=1;
  }
  mptCathode->AddProperty("EFFICIENCY", energy2, EFFICIENCY, 30);
  photocathodeMaterial->SetMaterialPropertiesTable(mptCathode);
  
  /*
  //material pmt
  galactic = nist->FindOrBuildMaterial("G4_Galactic");

  //szklo borokrzemowe
    G4Material* SiO2 = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE"); 
    G4Material* B2O3 = nist->FindOrBuildMaterial("G4_BORON_OXIDE");    
    G4Material* Na2O = nist->FindOrBuildMaterial("G4_SODIUM_MONOXIDE");
    G4Material* K2O = nist->FindOrBuildMaterial("G4_POTASSIUM_OXIDE");
    G4Material* Al2O3 = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");

    G4Material* borosilicateGlass = new G4Material("BorosilicateGlass", 2.23*g/cm3, 5); 
    borosilicateGlass->AddMaterial(SiO2, 0.80);  
    borosilicateGlass->AddMaterial(B2O3, 0.13);  
    borosilicateGlass->AddMaterial(Na2O, 0.04);  
    borosilicateGlass->AddMaterial(K2O, 0.02);  
    borosilicateGlass->AddMaterial(Al2O3, 0.01); 

  pmtWallsMaterial = borosilicateGlass;
  */
}

void MyDetectorConstruction::SetupSensitivePMT(G4LogicalVolume* scoringVolume)
{  
  G4MultiFunctionalDetector* detector = new G4MultiFunctionalDetector("MyDetector");

  G4SDManager* manager = G4SDManager::GetSDMpointer();  

  manager->AddNewDetector(detector);

  G4PSFlatSurfaceCurrent* scorer = new G4PSFlatSurfaceCurrent("MyScorer",fCurrent_In);

  MyPhotonSD* photonSD =  new MyPhotonSD("Photon");
  manager->AddNewDetector(photonSD);
  scoringVolume->SetSensitiveDetector(photonSD);

  detector->RegisterPrimitive(scorer);
}


void MyDetectorConstruction::SetupGeometry()
{
  ////////////////swiat//////////////////////////

  G4Box* worldSolid = new G4Box("World", 50*cm, 50*cm, 50*cm);
  fpWorldLogical = new G4LogicalVolume(worldSolid, air, "World");
  fpWorldPhysical = new G4PVPlacement(0, G4ThreeVector(), fpWorldLogical, "World", nullptr, false, 0);

  ////////////////tworzenie objetosci wypelnionej gazem C4F10//////////////////////////

  //definiowanie trapeza w przekroju XY
  std::vector<G4TwoVector> frontPolygon;
  frontPolygon.push_back(G4TwoVector(-15.5*cm,-3*cm));
  frontPolygon.push_back(G4TwoVector(15.5*cm,-3*cm));
  frontPolygon.push_back(G4TwoVector(9.5*cm, 3*cm));
  frontPolygon.push_back(G4TwoVector(-15.5*cm,3*cm));
  //rozciaganie trapeza wzdluz osi z rownomiernie o 3cm, uzyskujac pelna bryle
  G4ExtrudedSolid* frontChamberSolid = new G4ExtrudedSolid("FrontChamber", frontPolygon, 3*cm, G4TwoVector(0,0),1.0, G4TwoVector(0,0),1.0);
  //wypelnienie C4F10
  G4LogicalVolume* frontChamberLogical = new G4LogicalVolume(frontChamberSolid, C4F10, "FrontChamber_Logical");
  //umiejscowienie w swiecie
  new G4PVPlacement(0, G4ThreeVector(), frontChamberLogical, "FrontChamber_Physical", fpWorldLogical, false, 0);

  ////////////////tworzenie powierzchni lustrzanych wokol gazu//////////////////////////

  //powierzchnia lustrzana
  G4OpticalSurface* opticalMirror = new G4OpticalSurface("MirrorSurface");
  opticalMirror->SetType(dielectric_metal); //powierzchnia na ktorej zachodzi odbicie, nie zalamanie
  opticalMirror->SetFinish(polished); //powierzchnia idealnie gladka
  opticalMirror->SetModel(unified); //umozliwia rozne rodzaje odbic, w polaczeniu z polished powoduje czyste odbicie lustrzane
  opticalMirror->SetMaterialPropertiesTable(mirrorMaterial->GetMaterialPropertiesTable()); //

  G4double wallsWidth= 0.0005*cm;

  //sciany boczne
  G4ExtrudedSolid* sideWallSolid = new G4ExtrudedSolid("sideWallChamber", frontPolygon, wallsWidth, G4TwoVector(0,0),1.0, G4TwoVector(0,0),1.0);
  G4LogicalVolume* sideWallLogical = new G4LogicalVolume(sideWallSolid, mirrorMaterial, "SidetWall_Logical");
  
  new G4LogicalSkinSurface("SideWallSurface", sideWallLogical, opticalMirror);
  new G4PVPlacement(0, G4ThreeVector(0,0,-3*cm-wallsWidth), sideWallLogical, "LeftWall_Physical", fpWorldLogical, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0, 0, 3*cm+wallsWidth), sideWallLogical, "RighWall_Physical", fpWorldLogical, false, 1);
  
  /*//przednia sciana
  G4Box* frontWallSolid = new G4Box("frontWallChamber", wallsWidth, 3*cm+2*wallsWidth, 3*cm+2*wallsWidth);
  G4LogicalVolume* frontWallLogical = new G4LogicalVolume(frontWallSolid, mirrorMaterial, "FrontWall_Logical");
  
  new G4LogicalSkinSurface("FrontWallSurface", frontWallLogical, opticalMirror);
  new G4PVPlacement(0, G4ThreeVector(-15.5*cm-wallsWidth,0,0), frontWallLogical, "frontWall_Physical", fpWorldLogical, false, 0);
  */
  //gorna sciana
  std::vector<G4TwoVector> topWallPolygon;
  topWallPolygon.push_back(G4TwoVector(-15.5*cm,3*cm+2*wallsWidth));
  topWallPolygon.push_back(G4TwoVector(15.5*cm-(6*cm-2*wallsWidth),3*cm+2*wallsWidth));
  topWallPolygon.push_back(G4TwoVector(15.5*cm-(6*cm), 3*cm));
  topWallPolygon.push_back(G4TwoVector(-15.5* cm, 3*cm));

  G4ExtrudedSolid* topWallSolid = new G4ExtrudedSolid("TopWall", topWallPolygon, 3 * cm+2*wallsWidth, G4TwoVector(0, 0), 1.0, G4TwoVector(0, 0), 1.0);
  G4LogicalVolume* topWallLogical = new G4LogicalVolume(topWallSolid, mirrorMaterial, "TopWall_Logical");

  new G4LogicalSkinSurface("TopWallSurface", topWallLogical, opticalMirror);
  new G4PVPlacement(0, G4ThreeVector(), topWallLogical, "TopWall_Physical", fpWorldLogical, false, 0);

  //dolna sciana
  G4Box* bottomWallSolid = new G4Box("BottomWall", (15.5-2.55)*cm, wallsWidth, 3*cm+2*wallsWidth);
  G4LogicalVolume* bottomWallLogical = new G4LogicalVolume(bottomWallSolid, mirrorMaterial, "BottomWall_Logical");

  new G4LogicalSkinSurface("BottomWallSurface", bottomWallLogical, opticalMirror);
  new G4PVPlacement(0, G4ThreeVector(-2.55*cm,-3*cm-wallsWidth,0), bottomWallLogical, "BottomWall_Physical", fpWorldLogical, false, 0);

  /////////////////tworzenie lustra//////////////////////////

  G4Box* mirrorSolid = new G4Box("Mirror", wallsWidth*sqrt(2), (6*cm-0*wallsWidth)*sqrt(2)/2, 3 * cm+2*wallsWidth);
  G4LogicalVolume* mirrorLogical = new G4LogicalVolume(mirrorSolid, mirrorMaterial, "Mirror_Logical");
  new G4LogicalSkinSurface("MirrorSurface", mirrorLogical, opticalMirror);

  G4RotationMatrix* rotMirror = new G4RotationMatrix();
  rotMirror->rotateZ(-45. * deg);
  new G4PVPlacement(rotMirror, G4ThreeVector((15.5-3)*cm+wallsWidth, wallsWidth, 0), mirrorLogical, "Mirror_Physical", fpWorldLogical, false, 0);
  
  /*/////////////////tworzenie pmt//////////////////////////
  G4Box* pmtSolid = new G4Box("PMT", 2.55*cm, 2.55*cm, 2.55*cm);
  G4LogicalVolume* pmtLogical = new G4LogicalVolume(pmtSolid, galactic, "PMT_Logical");
  new G4PVPlacement(0, G4ThreeVector((15.5-2.55)*cm+wallsWidth, (-3-2.55)*cm, 0), pmtLogical, "PMT_Physical", fpWorldLogical, false, 0);

  G4Box* pmtSideWallSolid = new G4Box("PMTSideWall", 2.55*cm, 2.55*cm-wallsWidth, wallsWidth);
  G4LogicalVolume* pmtSideWallLogical = new G4LogicalVolume(pmtSideWallSolid, pmtWallsMaterial, "PMTSideWall_Logical");
  new G4PVPlacement(0, G4ThreeVector((15.5-2.55)*cm, (-3-2.55)*cm-wallsWidth, 2.55*cm+wallsWidth), pmtSideWallLogical, "FirstPMTWall_Physical", fpWorldLogical, false, 0);
  G4RotationMatrix* rotMirrorPMT = new G4RotationMatrix();
  rotMirrorPMT->rotateY(90. * deg);
  new G4PVPlacement(rotMirrorPMT, G4ThreeVector(15.5*cm+wallsWidth, (-3-2.55)*cm-wallsWidth, 0), pmtSideWallLogical, "SecondPMTWall_Physical", fpWorldLogical, false, 1);
  new G4PVPlacement(0, G4ThreeVector((15.5-2.55)*cm, (-3-2.55)*cm-wallsWidth, -2.55*cm-wallsWidth), pmtSideWallLogical, "ThirdPMTWall_Physical", fpWorldLogical, false, 2);
  new G4PVPlacement(rotMirrorPMT, G4ThreeVector(-2.55*cm-wallsWidth, -wallsWidth, 0), pmtSideWallLogical, "FourthPMTWall_Physical", pmtLogical, false, 3);
  G4Box* pmtBottomWallSolid = new G4Box("PMTBottomWall", 2.55*cm, 2.55*cm, wallsWidth);
  G4LogicalVolume* pmtBottomWallLogical = new G4LogicalVolume(pmtBottomWallSolid, pmtWallsMaterial, "PMTBottomWall_Logical");
  G4RotationMatrix* rotMirrorPMT2 = new G4RotationMatrix();
  rotMirrorPMT2->rotateX(90. * deg);
  new G4PVPlacement(rotMirrorPMT2, G4ThreeVector(0, -2.55*cm-wallsWidth, 0), pmtBottomWallLogical, "FithPMTWall_Physical", pmtLogical, false, 4);
  */

  /////////////////tworzenie fotokatody//////////////////////////

  G4Box* photocathodeSolid = new G4Box("PhotoCathode", 2.55*cm, wallsWidth, 2.55*cm);
  G4LogicalVolume* photocathodeLogical = new G4LogicalVolume(photocathodeSolid, photocathodeMaterial, "Photocathode_Logical");
  new G4PVPlacement(0,G4ThreeVector((15.5-2.55)*cm,-3*cm-wallsWidth, 0), photocathodeLogical, "Photocathode_Physical",fpWorldLogical, false, 0);

  G4Box*  sideOfCathodeSolid = new G4Box("SideOfCathode", 2.55*cm, wallsWidth, (3-2.55)/2 * cm+wallsWidth);
  G4LogicalVolume* sideOfCathodeLogical =new G4LogicalVolume(sideOfCathodeSolid, mirrorMaterial, "SideOfCathode_Logical");
  new G4LogicalSkinSurface("SideOfCathode", sideOfCathodeLogical, opticalMirror);
  new G4PVPlacement(0, G4ThreeVector((15.5-2.55)*cm,-3*cm-wallsWidth, (-3+(3-2.55)/2)*cm-wallsWidth), sideOfCathodeLogical, "LeftWall_Physical", fpWorldLogical, false, 0);
  new G4PVPlacement(0, G4ThreeVector((15.5-2.55)*cm,-3*cm-wallsWidth, (3-(3-2.55)/2)*cm+wallsWidth), sideOfCathodeLogical, "RighWall_Physical", fpWorldLogical, false, 1);
  
  //wlasciwosci optyczne fotokatody, symulujace dzialanie realnej
  G4OpticalSurface* photocathodeSurface = new G4OpticalSurface("PhotocathodeSurface");
  photocathodeSurface->SetType(dielectric_metal);
  photocathodeSurface->SetFinish(polished);
  photocathodeSurface->SetModel(unified);
  photocathodeSurface->SetMaterialPropertiesTable(photocathodeMaterial->GetMaterialPropertiesTable());
  new G4LogicalSkinSurface("PhotocathodeSkinSurface",photocathodeLogical,photocathodeSurface);
  //sensitivedetector
  SetupSensitivePMT(photocathodeLogical);


  /////////////////wizualizacja//////////////////////////
  fpWorldLogical->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4VisAttributes* frontChamberVis = new G4VisAttributes(G4Colour(0.745, 0.678, 0.78,0.5));
  frontChamberLogical->SetVisAttributes(frontChamberVis);

  G4VisAttributes* mirrorVis = new G4VisAttributes(G4Colour(0.9,0.9,0.9,1.0));
  mirrorLogical->SetVisAttributes(mirrorVis);

  //G4VisAttributes* pmtVis = new G4VisAttributes(G4Colour(0.361, 0.251, 0.333,0.9));
  //pmtLogical->SetVisAttributes(pmtVis);

  G4VisAttributes* cathVis = new G4VisAttributes(G4Colour(0.361, 0.251, 0.333,0.9));
  photocathodeLogical->SetVisAttributes(cathVis);
}



