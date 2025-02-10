#include "PMDetectorConstruction.hh"
#include "globals.hh"
#include "PMSensitiveDetector.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

PMDetectorConstruction::PMDetectorConstruction()
{
    DefineMaterials();
}

PMDetectorConstruction::~PMDetectorConstruction()
{
}

void PMDetectorConstruction::DefineMaterials()
{
    G4NistManager* nist = G4NistManager::Instance();
    
    // Create silica (SiO2) rock material
    rockMaterial = new G4Material("SilicaRock", 2.65*g/cm3, 2);
    G4Element* Si = nist->FindOrBuildElement("Si");
    G4Element* O = nist->FindOrBuildElement("O");
    rockMaterial->AddElement(Si, 1);
    rockMaterial->AddElement(O, 2);
}

G4VPhysicalVolume *PMDetectorConstruction::Construct()
{
    G4bool checkOverlaps = true;

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    G4Material *leadMat = nist->FindOrBuildMaterial("G4_Pb");
    G4Material *waterMat = nist->FindOrBuildMaterial("G4_WATER");
    G4Material *detMat = nist->FindOrBuildMaterial("G4_Li");

    // World volume - make it big enough for cavern
    G4double xWorld = 6.0 * m;  // Increased to accommodate cavern
    G4double yWorld = 6.0 * m;
    G4double zWorld = 6.0 * m;

    G4Box *solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "LogicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), 
        logicWorld, "physWorld", 0, false, 0, checkOverlaps);

    // Cavern construction
    G4double cavernWidth = 5.*m;
    G4double cavernHeight = 5.*m;
    G4double rockThickness = 3.*m;
    
    // Outer rock box
    G4double outerWidth = cavernWidth + 2*rockThickness;
    G4double outerHeight = cavernHeight + 2*rockThickness;
    G4Box* outerRockSolid = new G4Box("OuterRock", 
        outerWidth/2, outerHeight/2, outerWidth/2);
    G4LogicalVolume* outerRockLogical = new G4LogicalVolume(outerRockSolid, 
        rockMaterial, "OuterRock");
    
    // Inner air cavity
    G4Box* innerCavernSolid = new G4Box("InnerCavern", 
        cavernWidth/2, cavernHeight/2, cavernWidth/2);
    G4LogicalVolume* innerCavernLogical = new G4LogicalVolume(innerCavernSolid, 
        worldMat, "InnerCavern");
    
    // Place inner cavity inside rock
    new G4PVPlacement(0, G4ThreeVector(0,0,0), 
        innerCavernLogical, "InnerCavern", 
        outerRockLogical, false, 0, checkOverlaps);
    
    // Place rock structure in world
    new G4PVPlacement(0, G4ThreeVector(0,0,0), 
        outerRockLogical, "OuterRock", 
        logicWorld, false, 0, checkOverlaps);

    // Your detector setup - placed inside the cavern
    G4double detectorSize = 50.0 * cm;
    
    G4Box *solidDetector = new G4Box("solidDetector", 
        0.5 * detectorSize, 0.5 * detectorSize, 0.5 * detectorSize);
    logicDetector = new G4LogicalVolume(solidDetector, detMat, "logicDetector");
    G4VPhysicalVolume *physDetector = new G4PVPlacement(0, 
        G4ThreeVector(0., 0., 0.), logicDetector, "physDetector", 
        innerCavernLogical, false, 0, checkOverlaps);

    // Water box
    G4double waterThickness = 5. *cm;
    G4double waterSize = 10. *cm;
    //G4Box *waterBox = new G4Box("waterBox", waterSize, waterSize, waterThickness);
    //G4LogicalVolume *logicWater = new G4LogicalVolume(waterBox, waterMat, "logicWater");
    //G4VPhysicalVolume *physWater = new G4PVPlacement(0, 
        //G4ThreeVector(0., 0., 4.9 *cm), logicWater, "physWater", 
        //logicDetector, false, 0, checkOverlaps);

    // Visualization attributes
    G4VisAttributes* rockVisAtt = new G4VisAttributes(G4Colour(0.7, 0.4, 0.1, 0.2));
    outerRockLogical->SetVisAttributes(rockVisAtt);
    
    G4VisAttributes* cavernVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.3));
    cavernVisAtt->SetForceSolid(true);
    innerCavernLogical->SetVisAttributes(cavernVisAtt);

    G4VisAttributes *detVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0,0.2));
    detVisAtt->SetForceSolid(true);
    logicDetector->SetVisAttributes(detVisAtt);

    //G4VisAttributes *waterVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.8));
    //waterVisAtt->SetForceSolid(true);
    //logicWater->SetVisAttributes(waterVisAtt);

    return physWorld;
}

void PMDetectorConstruction::ConstructSDandField()
{
    PMSensitiveDetector *sensDet = new PMSensitiveDetector("SensitiveDetector");
    logicDetector->SetSensitiveDetector(sensDet);
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
}