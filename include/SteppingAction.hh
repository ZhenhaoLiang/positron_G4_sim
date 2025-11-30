/// \file SteppingAction.hh
/// \brief Definition of the SteppingAction class

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class EventAction;

class G4LogicalVolume;

/// Stepping action class
/// 

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(EventAction* eventAction);
    virtual ~SteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);
    void SetGammaCheck(G4bool flag) { fGammaCheck = flag; }
    void SetNGplan(G4bool flag) { fNGplan = flag; }
    void SetOutterSheildRecord(G4bool flag) { fOutterSheildRecord = flag; }
    
    G4bool GetGammaCheck() const { return fGammaCheck; }
    G4bool GetNGplan() const { return fNGplan; }
    G4bool GetOutterSheild() const { return fOutterSheildRecord; }
  private:
    EventAction*  fEventAction;
    G4LogicalVolume* fScoringVolume;
    G4bool fGammaCheck;
    G4bool fNGplan;
    G4bool fOutterSheildRecord;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
