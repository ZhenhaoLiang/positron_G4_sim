/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include <G4Tubs.hh>
#include <G4Torus.hh>
#include <G4RotationMatrix.hh>
#include <G4ThreeVector.hh>
#include <G4UnionSolid.hh>
#include <G4Material.hh>
#include "G4TessellatedSolid.hh"
#include "G4Polycone.hh"
#include <G4SubtractionSolid.hh>
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4ExtrudedSolid.hh"
#include <G4VisAttributes.hh>

#define pi 3.14159265359

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{
    fDetectorMessenger = new DetectorMessenger(this);
    RunModel = "NaI";
    DefineMaterial();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
    delete fDetectorMessenger;
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::DefineMaterial()
{
  G4NistManager* nist = G4NistManager::Instance();
  G4Element *D = new G4Element("Deuterium", "D", 1);
  G4Element *B = nist->FindOrBuildElement("B");
  G4Element *H = new G4Element("Hydrogen", "H", 1., 1.0079 * g / mole);
  G4Element *C = new G4Element("Carbon", "C", 6., 12.011 * g / mole);
  G4Element *N = new G4Element("Nitrogen", "N", 7., 14.007 * g / mole);
  G4Element *O = new G4Element("Oxygen", "O", 8., 15.999 * g / mole);
  G4Element *F = new G4Element("Fluorine", "F", 9., 18.998 * g / mole);
  G4Element *Si = new G4Element("Silicon", "Si", 14., 28.086 * g / mole);
  G4Element *Cr = new G4Element("Chromium", "Cr", 24., 51.996 * g / mole);
  G4Element *Mn = new G4Element("Manganese", "Mn", 25., 54.938 * g / mole);
  G4Element *Fe = new G4Element("Iron", "Fe", 26., 55.85 * g / mole);
  G4Element *Ni = new G4Element("Nickel", "Ni", 28., 58.693 * g / mole);
  G4Element* Ar = new G4Element("Argon", "Ar", 18., 39.948*g/mole);
  G4Element* elNa = new G4Element("Sodium", "Na", 11., 22.989770*g/mole);
  G4Element* elI = new G4Element("Iodine", "I", 53., 126.90447*g/mole);

  G4Isotope *D_Iso = new G4Isotope("D_Iso", 1, 2, 2.014102 * g / mole);
  D->AddIsotope(D_Iso, 1);

  G4Element *Xe = new G4Element("Xenon", "Xe", 54., 131.293 * g / mole);
  LXe = new G4Material("LXe", 2.862 * g / cm3, 1, kStateLiquid,
                                   177.05 * kelvin, 1.5 * atmosphere);
  
  // DR 20180518 - Density according to:
  // -
  // https://xe1t-wiki.lngs.infn.it/doku.php?id=xenon:xenon1t:deg:tpc:targetmass
  // -
  // https://xe1t-wiki.lngs.infn.it/doku.php?id=xenon:xenon1t:analysis:sciencerun1:sc_summary
  LXe->AddElement(Xe, 1);
  G4Element* elCu = new G4Element("Copper", "Cu", 29., 63.546*g/mole);

  Cu = new G4Material("Copper_manual", 8.96*g/cm3, 1);
  Cu->AddElement(elCu, 1.0); 

  Air = nist->FindOrBuildMaterial("G4_AIR");
  Water = nist->FindOrBuildMaterial("G4_WATER");
  Pb = nist->FindOrBuildMaterial("G4_Pb");
  //
  
  epoxy = new G4Material("Epoxy_Resin", 1.14*g/cm3, 3); // 密度约为 1.14 g/cm³
  epoxy->AddElement(C, 15); // 假设化学式为 C15H22O2 (示例)
  epoxy->AddElement(H, 22);
  epoxy->AddElement(O,  2);
  // --- 3. 定义玻璃纤维 (Glass Fiber) ---
  // 玻璃纤维主要成分是二氧化硅 (SiO2)
  silica = new G4Material("Silica", 2.200*g/cm3, 2);
  silica->AddElement(Si, 1);
  silica->AddElement(O, 2);
  
  // --- 4. 创建玻璃纤维板复合材料 (G4_Composite) ---
  // 使用 G4Material 来创建复合材料，按质量分数混合
  GlassFiber = new G4Material("GlassFiberBoard", 1.86*g/cm3, 2); // 典型密度约为 1.86 g/cm³
  GlassFiber->AddMaterial(silica, 0.6); // 玻璃纤维占 60%
  GlassFiber->AddMaterial(epoxy,  0.4); // 环氧树脂占 40%
  // --- 定义玻璃纤维复合材料 ---

  //==== Stainless Steel ====

  SS304LSteel = new G4Material("SS304LSteel", 8.00 * g / cm3, 5, kStateSolid);
  SS304LSteel->AddElement(Fe, 0.65);
  SS304LSteel->AddElement(Cr, 0.20);
  SS304LSteel->AddElement(Ni, 0.12);
  SS304LSteel->AddElement(Mn, 0.02);
  SS304LSteel->AddElement(Si, 0.01);

  //==== ePTFE ==== Expanded PTFE (as Teflon, but lower density, used in the nVeto reflector)
  ePTFE = new G4Material("ePTFE", 0.7 * g / cm3, 2, kStateSolid);
  ePTFE->AddElement(C, 0.240183);
  ePTFE->AddElement(F, 0.759817);
  //==== EJ-276 for Scintillator target in BeamPipe ====

  //G4double EJ276_density = 1.099 * g / cm3;
  G4double H_density = 4.647e+22*(1. / cm3);
  G4double C_density = 4.944e+22*(1. / cm3);

  G4double H_M = nist->FindOrBuildElement("H")->GetAtomicMassAmu()*(g / mole);
  G4double C_M = nist->FindOrBuildElement("C")->GetAtomicMassAmu()*(g / mole);

  G4double H_MassDensity = (H_density/CLHEP::Avogadro)*H_M;
  G4double C_MassDensity = (C_density/CLHEP::Avogadro)*C_M;

  G4double HC_density = H_MassDensity + C_MassDensity;
  G4double H_Frac = H_MassDensity/HC_density;
  G4double C_Frac 	= C_MassDensity/HC_density;

  EJ276 = new G4Material("EJ276", HC_density, 2, kStateSolid);
  EJ276->AddElement(nist->FindOrBuildElement("H"), H_Frac);
  EJ276->AddElement(nist->FindOrBuildElement("C"), C_Frac);

  G4cout << "EJ276 : density " <<  EJ276->GetDensity()/(g / cm3) << " , "
         << "NbOfAtomsPerVolume " << EJ276->GetTotNbOfAtomsPerVolume()/(1. / cm3) << G4endl;


  //========================Vaccum Air 1e-7 bar=============================
  // 步骤2：计算1e-7 bar空气的密度（理想气体状态方程）
  // ρ = (P * μM) / (R * T)
  // 其中：P=1e-7 bar=1e-2 Pa；T=293 K（室温）；R=气体常数；μM=空气平均摩尔质量
  G4double pressure = 1e-7 * bar;          // 目标压强
  G4double temperature = 293.15 * kelvin;  // 20℃（标准环境温度）
  G4double molarMass_N2 = 28.0134 * g/mole; // N₂分子摩尔质量
  G4double molarMass_O2 = 31.9988 * g/mole; // O₂分子摩尔质量
  G4double molarMass_Ar = 39.948 * g/mole;  // Ar原子摩尔质量
  // 空气平均摩尔质量（按体积分数加权）：78% N₂ + 21% O₂ + 1% Ar
  G4double molarMass_air = 0.78*molarMass_N2 + 0.21*molarMass_O2 + 0.01*molarMass_Ar;
  // 计算密度（ρ = P*μM/(R*T)，R=8.314 J/(mol·K) 已包含在Geant4物理常数中）
  G4double gas_constant = 8.31446261815324 * joule / (mole * kelvin);
  G4double density_air = (pressure * molarMass_air) / (gas_constant * temperature);

  // 步骤3：创建空气材料（按元素组成定义，匹配真实空气成分）
  air = new G4Material("Air_1e-7bar", density_air, 3);
  air->AddElement(N, 0.78);  // 氮占78%（体积/摩尔分数）
  air->AddElement(O, 0.21);  // 氧占21%
  air->AddElement(Ar, 0.01); // 氩占1%

  G4cout << "✅ 1e-7 bar air" 
          << "density = " << air->GetDensity()/(g/cm3) << " g/cm³，"
          << "mass = " << molarMass_air/(g/mole) << " g/mole" << G4endl;

  // ======================== NaI ========================

  NaI = new G4Material("NaI", 3.67*g/cm3, 2);
  NaI->AddElement(elNa, 1);
  NaI->AddElement(elI, 1);
  G4cout << "✅ NaI finished:density " << NaI->GetDensity()/(g/cm3) << " g/cm³" << G4endl;

  // ======================== CsI ========================

  G4Element* elCs = new G4Element("Cesium", "Cs", 55., 132.90545*g/mole);
  CsI = new G4Material("CsI", 4.51*g/cm3, 2);
  CsI->AddElement(elCs, 1);
  CsI->AddElement(elI, 1);
  G4cout << "✅CsI 材finished:density " << CsI->GetDensity()/(g/cm3) << " g/cm³" << G4endl;

  //===========================BPE===========================================
  

  BPE = new G4Material("B_poly", 0.93 * g / cm3, 4,
    kStateSolid);  // B-doped, from
  // http://lss.fnal.gov/archive/2000/fn/FN-697.pdf
  BPE->AddElement(nist->FindOrBuildElement("H"), 0.116);
  BPE->AddElement(nist->FindOrBuildElement("C"), 0.612);
  BPE->AddElement(nist->FindOrBuildElement("B"), 0.05);
  BPE->AddElement(nist->FindOrBuildElement("O"), 0.222);

  G4cout << "BPE : density " <<  BPE->GetDensity()/(g / cm3) << " , "
         << "NbOfAtomsPerVolume " << BPE->GetTotNbOfAtomsPerVolume()/(1. / cm3) << G4endl;
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  // Option to switch on/off checking of volumes overlaps
  //
  //G4double DistanccFromXeToFloor = 1.625*m + Pbthickness*cm + (102.05+231.32)*mm;//1.6*m + 293.2*mm - 25.0*cm + Xehalflength*cm;
  G4bool checkOverlaps = true;
  
  
  // World
  G4double world_sizeZ = 1.0*m;
  G4double world_sizeXY = 0.5*m;
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       world_sizeXY, world_sizeXY, world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        air,           //its material
                        "World");            //its name      

  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking   
  /*
  auto physPTFEContiner = ConstructPTFEContainer(logicWorld);    
  auto physPTFEOuter = ConstructPTFEOutContainer(logicWorld);    
  auto physHangring = ConstructHangingring(logicWorld);         
  ConstructHangingRot(logicWorld);   
  ConstructShapingRing(logicWorld);
  ConstructSheild(logicWorld);*/
  // LXe cyclinder
  // TPC Teflon：外高18.54cm，外半径8.3cm（对应直径16.6cm），侧壁厚度0.55cm，顶部厚0.25cm，底部厚0.25cm
  G4double Xeradius = (8.3 - 0.55)*cm; // cm
  G4double Xehalflength = (18.54*0.5 - 0.25)*cm; // cm
  G4double TPCXeradius = 8.3*cm; // cm
  G4double TPCHalflength = (18.54*0.5)*cm;
  G4Tubs* solidTPC =    
    new G4Tubs("TPC",                    //its name
        0, TPCXeradius, TPCHalflength,0, 360*deg); //its size
  G4LogicalVolume* logicTPC =                         
    new G4LogicalVolume(solidTPC,            //its solid
                        ePTFE,             //its material
                        "logicTPC");         //its name
new G4PVPlacement(0,           // rotation 
                    G4ThreeVector(0,0,Xehalflength),         //at (0,0,0)
                    logicTPC,                //its logical volume
                    "TPC",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps); 

  G4Tubs* solidXecylinder =    
    new G4Tubs("Xecylinder",                    //its name
        0, Xeradius, Xehalflength,0, 360*deg); //its size

  G4LogicalVolume* logicXecylinder =                         
    new G4LogicalVolume(solidXecylinder,            //its solid
                        LXe,             //its material
                        "logicXecylinder");         //its name

  G4Colour my_purple_colour(0.5, 0.0, 0.5);
  G4VisAttributes* LXevisAttr = new G4VisAttributes(my_purple_colour); 
  LXevisAttr->SetForceSolid(true);  // 关键：强制以实体表面显示
  logicXecylinder->SetVisAttributes(LXevisAttr);

  G4cout<<"Construct Xe cylinder with radius is "
  <<Xeradius<<" cm, half hight is "<<Xehalflength<<" cm.";
   
  new G4PVPlacement(0,           // rotation 
                    G4ThreeVector(0,0,0),         //at (0,0,0)
                    logicXecylinder,                //its logical volume
                    "Xecylinder",              //its name
                    logicTPC,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking  
  
  //不锈钢罐体侧壁高45cm，外半径12.7cm，厚度0.3cm
  //顶盖厚3cm，半径15.2cm
  //底盖厚3cm，半径15.2cm
  G4double SSContainerOutterradius = 12.7*cm;
  G4double SSContainerInnerradius = 12.4*cm;
  G4double SSContainerHalflength = 22.5*cm;
  G4Tubs* solidSSContainer =    
    new G4Tubs("SSContainer",                    //its name
        SSContainerInnerradius, SSContainerOutterradius, SSContainerHalflength,0, 360*deg); //its size 
  G4Tubs* solidSSContainerFlange =    
    new G4Tubs("SSContainerTop",                    //its name
        0, 15.2*cm, 0.5*3.0*cm,0, 360*deg); //its size
  G4UnionSolid* solidSSContainerWithFlange = new G4UnionSolid("SSContainerWithTop",
                           solidSSContainer,
                           solidSSContainerFlange, 0, G4ThreeVector(0,0,SSContainerHalflength+1.5*cm));
  solidSSContainerWithFlange = new G4UnionSolid("SSContainerWithTop",
                           solidSSContainerWithFlange,
                           solidSSContainerFlange, 0, G4ThreeVector(0,0,-SSContainerHalflength-1.5*cm));  
  G4LogicalVolume* logicSSContainer =                         
    new G4LogicalVolume(solidSSContainerWithFlange,            //its solid
                        SS304LSteel,             //its material
                        "logicSSContainer");         //its name 
  new G4PVPlacement(0,           // rotation 
                    G4ThreeVector(0,0,Xehalflength),         //at (0,0,0)
                    logicSSContainer,                //its logical volume
                    "SSContainer",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  // ==================== Scintor ==================
  G4double Cubesize = 3.0*cm;
  G4String NaICube_namePrefix = "NaICube_";

  G4Box* solidNaICube = new G4Box(
      "SolidNaICube",                  // 固体名称
      Cubesize, Cubesize,Cubesize  // 半尺寸（x,y,z）
  );
  G4Material* ScintorMaterial = CsI;
  if (RunModel == "NaI")
    ScintorMaterial = NaI;

  G4LogicalVolume* logicNaICube = new G4LogicalVolume(
      solidNaICube,    // 关联的固体
      ScintorMaterial,     // 关联的材料（NaI）
      "LogicScintor"   // 逻辑体名称
  );

  // 核心思路：以容器中心为原点，在 X-Y 平面（环绕容器径向）和 Z 轴（容器轴向）分布，
  // 确保立方体不与容器重叠，且间距合理（可调整 gap 参数）
  G4double container_centerZ = Xehalflength; // 容器中心Z坐标（与你的容器物理体一致）
  G4double container_maxRadius = 15.2*cm;    // 容器最大半径（法兰外半径15.2cm）
  G4double cube_gap = 1.0*cm;                // 立方体与容器/立方体之间的间隙（可调整）
  G4double cube_outerRadius = container_maxRadius + cube_gap + Cubesize; // 立方体中心到Z轴的距离

  // 定义阵列维度：X-Y平面环绕数（周向）、Z轴层数（轴向）
  G4int num_phi = 8;         // 周向均匀分布8个立方体（可调整，如6/12个）
  G4int num_z_layers = 5;    // Z轴方向3层（可调整，如2/4层）
  G4double z_layer_gap = 2.0*cm; // Z轴层间间隙（可调整）

  // 计算Z轴各层中心坐标（以容器中心为基准，上下对称分布）
  std::vector<G4double> z_positions;
  G4double z_start = container_centerZ - (num_z_layers-1)*(Cubesize*2 + z_layer_gap)/2;
  for (G4int z_idx = 0; z_idx < num_z_layers; z_idx++) {
      z_positions.push_back(z_start + z_idx*(Cubesize*2 + z_layer_gap));
  }

  // ======================================
  // 4. 循环创建NaI立方体物理体（阵列分布）
  // ======================================
  G4int cube_copyNum = 0; // 复制编号（用于区分不同立方体）
  for (G4int z_idx = 0; z_idx < num_z_layers; z_idx++) { // 遍历Z轴各层
      for (G4int phi_idx = 0; phi_idx < num_phi; phi_idx++) { // 遍历周向各位置
          // 计算当前立方体的中心坐标（极坐标转直角坐标）
          G4double phi = phi_idx * (360.0*deg / num_phi); // 周向角度（均匀分布）
          G4double x_pos = cube_outerRadius * cos(phi);
          G4double y_pos = cube_outerRadius * sin(phi);
          G4double z_pos = z_positions[z_idx];
          G4ThreeVector cube_center(x_pos, y_pos, z_pos);

          // 创建物理体（放置在世界逻辑体中，与容器同级）
          new G4PVPlacement(
              0,                      // 旋转（无旋转，保持立方体轴与世界轴一致）
              cube_center,            // 立方体中心坐标
              logicNaICube,           // 关联的逻辑体
              "Scintor", // 物理体名称（唯一）
              logicWorld,             // 母逻辑体（与容器相同，均为世界逻辑体）
              false,                  // 无布尔运算
              cube_copyNum,           // 复制编号（唯一）
              checkOverlaps           // 重叠检查（与容器一致）
          );
          cube_copyNum++;
      }
  }

  G4cout << "\n📋 Creat " << cube_copyNum << " Scintor Cube(" 
        << num_z_layers << "  X " << num_phi << " )" << G4endl;

  G4VisAttributes* ScintorAttr = new G4VisAttributes(G4Colour::Green()); 
  ScintorAttr->SetForceSolid(true);  // 关键：强制以实体表面显示
  logicNaICube->SetVisAttributes(ScintorAttr);

  return physWorld;
}

void DetectorConstruction::ChooseModel(G4String value)
{
  RunModel = value; 
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
