#ifndef MYPRIMARYGENERATORACTION_HH
#define MYPRIMARYGENERATORACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "G4GenericMessenger.hh"

class G4ParticleGun;
class G4Event;

class MyPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {

public:

  MyPrimaryGeneratorAction();    

  virtual ~MyPrimaryGeneratorAction();
  
  void GeneratePrimaries(G4Event*);
  G4bool GetRandomPosition();
  G4bool GetRandomAngle();
  G4bool GetRandomEnergy();
  static G4double GetTheta();
  static G4double GetPhi();
  static G4double GetEnergy();

private:

  G4ParticleGun* particleGun;	
  G4bool fUseRandomPosition;
  G4bool fUseRandomAngle;
  G4bool fUseRandomEnergy; 
  G4GenericMessenger* fMessenger;
  static G4double theta;
  static G4double phi;
  static G4double energy;
};

#endif
