#ifndef MYRUN_HH
#define MYRUN_HH
#include "G4THitsMap.hh"
#include "G4Run.hh"
#include "MyPrimaryGeneratorAction.hh"
#include <vector>

struct PhotonData {
    G4double e;       
    G4double x, y, z; 
    G4double time;    
};

struct EventData {      
    G4double protonY;
    G4double protonZ;
    G4double protonTheta;
    G4double protonPhi;
    G4double protonEnergy;
    G4double nPE;
};

class G4Event;
class MyRun : public G4Run {

public:
  //Method
  void RecordEvent(const G4Event*);
  // Constructor 
  MyRun();

  //Destructor
  virtual ~MyRun();
  
  std::vector<PhotonData> fPhotonData;
  std::vector<EventData> fEventData;
  std::vector<G4int> fPhotonsCountPerEvent;
  std::vector<G4double> fPE_Times;
  
  G4int numberOfEvent=0;
  G4int fMapId;
  G4int fCollectionId;
private:
  G4int fSumOfAllPhotoelectrons;
  G4int nPE_thisEvent ;

};

#endif
