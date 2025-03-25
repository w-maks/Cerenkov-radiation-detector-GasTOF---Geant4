
#include "MyPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"
#include <cmath>

G4double MyPrimaryGeneratorAction::theta = 0;
G4double MyPrimaryGeneratorAction::phi = 0;
G4double MyPrimaryGeneratorAction::energy = 200;

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  particleGun(nullptr),
  fUseRandomPosition(true),
  fUseRandomAngle(true),
  fUseRandomEnergy(true),
  fMessenger(nullptr)
{
  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="proton");
  
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  particleGun->SetParticleEnergy(200.*GeV);
  particleGun->SetParticlePosition(G4ThreeVector(-15.6*cm,0.,0.));

  fMessenger = new G4GenericMessenger(this, "/mygen/", "Sterowanie generatorem cząstek");

  // Komenda: /mygen/useRandomEnergy [true/false]
  fMessenger->DeclareProperty("useRandomEnergy", fUseRandomEnergy).SetGuidance("Wlacza/wylacza losowa energie.").SetStates(G4State_PreInit, G4State_Idle);

  // Komenda: /mygen/useRandomPosition [true/false]
  fMessenger->DeclareProperty("useRandomPosition", fUseRandomPosition).SetGuidance("Wlacza/wylacza losowanie pozycji poczatkowej protonu.").SetStates(G4State_PreInit, G4State_Idle);

  // Komenda: /mygen/useRandomAngle [true/false]
  fMessenger->DeclareProperty("useRandomAngle", fUseRandomAngle).SetGuidance("Wlacza/wylacza losowanie kierunku (pelny zakres katowy).").SetStates(G4State_PreInit, G4State_Idle);
}

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction()
{
  delete particleGun;
  delete fMessenger;
}

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if (fUseRandomPosition)
  {
    G4double halfSize = (3.0-0.0005)*cm;  
    G4double yRand = (2.* G4UniformRand() - 1.) * halfSize; 
    G4double zRand = (2.* G4UniformRand() - 1.) * halfSize;
    particleGun->SetParticlePosition(G4ThreeVector(-15.6*cm, yRand, zRand));
  }

  if (fUseRandomAngle)
  {
    //wyznaczanie kata brylowego
    G4double detectorDistance = 0.5;
    G4double halfAngleTangent = (3.0 - 0.0005) / detectorDistance;
    G4double maxTheta = std::atan(halfAngleTangent);
    theta = maxTheta * G4UniformRand();
    
    phi = 2.*CLHEP::pi * G4UniformRand();

    G4double sinTheta = std::sin(theta);
    G4double dirX = std::cos(theta);  // skierowany w stronę detektora
    G4double dirY = sinTheta * std::cos(phi);
    G4double dirZ = sinTheta * std::sin(phi);

    //normalizacja
    G4ThreeVector newDir(dirX, dirY, dirZ);
    newDir = newDir.unit();
    particleGun->SetParticleMomentumDirection(newDir);
  }
  if (fUseRandomEnergy)
  {
    G4double minEnergy = 1.0 * GeV;
    G4double maxEnergy = 211.0 * GeV;
    energy = minEnergy + G4UniformRand() * (maxEnergy - minEnergy);
    particleGun->SetParticleEnergy(energy);
  }
  particleGun->GeneratePrimaryVertex(anEvent);
}
  G4bool MyPrimaryGeneratorAction::GetRandomPosition(){
    return fUseRandomPosition;
  }
 G4bool MyPrimaryGeneratorAction::GetRandomAngle(){
    return fUseRandomAngle;
  }
  G4bool MyPrimaryGeneratorAction::GetRandomEnergy(){
    return fUseRandomEnergy;
  }
  G4double MyPrimaryGeneratorAction::GetTheta(){
    return theta;
  }
 G4double MyPrimaryGeneratorAction::GetPhi(){
    return phi;
  }
  G4double MyPrimaryGeneratorAction::GetEnergy(){
    return energy;
  }