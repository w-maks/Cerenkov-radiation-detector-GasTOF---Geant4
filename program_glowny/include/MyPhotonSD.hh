#ifndef MYPHOTONSD_HH
#define MYPHOTONSD_HH

#include "G4VSensitiveDetector.hh"
#include "MyPhotonHit.hh"
#include "G4THitsCollection.hh"

class G4HCofThisEvent;
class G4TouchableHistory;
class G4Step;

class MyPhotonSD : public G4VSensitiveDetector {

public:

  // Constructor
  MyPhotonSD(const G4String& name);

  // Destructor
  virtual ~MyPhotonSD();
  
  // Methods
  void Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent);  
  G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* history);
  void EndOfEvent(G4HCofThisEvent* hitsCollectionOfThisEvent);
  
private:
  
  // Data members
  G4THitsCollection<MyPhotonHit>* fPhotonCollection;
  G4int fPhotonCollectionID;

};




#endif

