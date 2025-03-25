#ifndef MYPHYSICSLIST_HH
#define MYPHYSICSLIST_HH

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class MyPhysicsList: public G4VUserPhysicsList {

public:

  //Constructor
  MyPhysicsList();

  //Destructor
  virtual ~MyPhysicsList();
  
protected:

  //Construct particles and physics processes
  void ConstructParticle();
  void ConstructProcess();
  void SetCuts();
  
private:
  
  //Helper methods
  void ConstructGeneral();
  void ConstructEM();
  void ConstructOptical();

};

#endif



