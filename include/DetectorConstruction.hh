/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4NistManager.hh"
#include <G4UnionSolid.hh>
#include "G4Polycone.hh"
#include <G4Box.hh>
#include <G4UIcmdWithABool.hh>

#include "DetectorMessenger.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume* Construct();
    void ChooseModel(G4String);
    void setXehalflength(G4float);
    void setXeradius(G4float);
    void setPb1Thickness(G4float);
    void setBPEThickness(G4float);
    void setPb2Thickness(G4float);

    void setScintorDistance(G4float);
    void setConstructFloor(G4bool);

    G4float GetXeradius () {return Xeradius;};
    G4float GetXehalflength () {return Xehalflength;};
    G4float Xeradius,Xehalflength,Pb1thickness,Pb2thickness,BPEthickness,disctancefromXe;
    G4bool ConstructFloor;
    G4String RunModel;
  private:
    void DefineMaterial();
    G4VSolid* CutHolesInSolid(G4VSolid* target_solid, G4double cutter_thickness,G4double hole_radius,G4ThreeVector circle_center,G4int num_holes);
    G4VSolid* SetAnEmptyBox(G4double x1,G4double y1,G4double z1,G4double thickness,G4String name);
    G4VSolid* CreateHexagonalPrism(G4String name, G4double sideLength, G4double height);
    G4VPhysicalVolume* ConstructPTFEContainer(G4LogicalVolume* motherLV);
    G4VPhysicalVolume* ConstructHangingring(G4LogicalVolume* motherLV);
    G4VPhysicalVolume* ConstructPTFEOutContainer(G4LogicalVolume* motherLV);
    G4VPhysicalVolume* ConstructFloors(G4LogicalVolume* motherLV);
    G4Material *Air,*Water,*LXe,*Pb,*SS304LSteel,*EJ276,*ePTFE,*BPE,*Cu,*GlassFiber,*epoxy,*silica,*air,*NaI,*CsI;
    void ConstructNGPlan(G4LogicalVolume* motherLV);
    void ConstructShapingRing(G4LogicalVolume* motherLV);
    void ConstructHangingRot(G4LogicalVolume* motherLV);
    void ConstructSheild(G4LogicalVolume* motherLV);
  protected:
    G4LogicalVolume*  fScoringVolume;
    DetectorMessenger* fDetectorMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

