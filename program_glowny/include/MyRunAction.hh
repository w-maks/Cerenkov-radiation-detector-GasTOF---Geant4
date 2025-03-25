#ifndef MYRUNACTION_HH
#define MYRUNACTION_HH

#include "G4UserRunAction.hh"
#include "MyPrimaryGeneratorAction.hh"
#include <map>
#include <tuple>


class G4Run;

class MyRunAction : public G4UserRunAction {

public:

  //Constructor
  MyRunAction(MyPrimaryGeneratorAction*);


  //Destructor
  virtual ~MyRunAction();

  //Method
   void BeginOfRunAction(const G4Run* );
   void EndOfRunAction(const G4Run* ); 
   static G4bool GetUseRandomPosition();
   static G4bool GetUseRandomAngle();
   static G4bool GetUseRandomEnergy();        
   G4Run* GenerateRun();

private:
  MyPrimaryGeneratorAction* fPrimaryGenerator;
  static G4bool useRandomPosition;
  static G4bool useRandomAngle;
  static G4bool useRandomEnergy;

};

#endif

