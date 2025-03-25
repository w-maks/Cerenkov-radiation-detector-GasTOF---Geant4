#include "MyPhotonSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"

MyPhotonSD::MyPhotonSD(const G4String& name)
  :G4VSensitiveDetector(name)
{
  collectionName.insert("PhotonCollection");
  fPhotonCollectionID = -1;
}

MyPhotonSD::~MyPhotonSD() {}

void MyPhotonSD::Initialize(G4HCofThisEvent* hitsCollectionOfThisEvent)
{
 // Create a new collection
 fPhotonCollection = new G4THitsCollection<MyPhotonHit>(SensitiveDetectorName, collectionName[0]);
 if( fPhotonCollectionID < 0) 
    fPhotonCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(fPhotonCollection);
 // Add collection to the events
    hitsCollectionOfThisEvent->AddHitsCollection(fPhotonCollectionID, fPhotonCollection); 
}

G4bool MyPhotonSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
 // Create a new hit
 MyPhotonHit* aHit = new MyPhotonHit();

 // Get energy and position
 G4double energy = aStep->GetTrack()->GetTotalEnergy();
 G4ThreeVector position = aStep->GetTrack()->GetPosition();
 G4double time = aStep->GetTrack()->GetGlobalTime();


 // Set energy and position
 aHit->SetEnergy(energy);
 aHit->SetPosition(position);
 aHit->SetTime(time);


 // Insert hit into the collection
 fPhotonCollection->insert(aHit); 
 return true;
}

void MyPhotonSD::EndOfEvent(G4HCofThisEvent*) {}
