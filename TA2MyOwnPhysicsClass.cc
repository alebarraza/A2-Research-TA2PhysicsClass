//--Author	Alejandro Barraza-Valdez   Feb 2015   Compton analysis

#include "TA2MyOwnPhysicsClass.h"

enum { EComptonPromptWindows = 1000, EComptonRandomWindows, EPi0PromptWindows, EPi0RandomWindows, EPi0InvariantMassCuts,
		                     EProduceTreeFile, ETreeFileName};
static const Map_t kInputs[] = {
	{"Compton-Prompt-Windows:",	EComptonPromptWindows},
	{"Compton-Random-Windows:",	EComptonRandomWindows},
	{"Pi0-Prompt-Windows:",		EPi0PromptWindows},
	{"Pi0-Random-Windows:",		EPi0RandomWindows},
	{"Pi0-Invariant-Mass-Cuts:",	EPi0InvariantMassCuts},
	{"Produce-Tree-File:",		EProduceTreeFile},
	{"Tree-File-Name:",		ETreeFileName},
	{NULL,          -1}
};


//-----------------------------------------------------------------------------
TA2MyOwnPhysicsClass::TA2MyOwnPhysicsClass( const char* name, TA2Analysis* analysis )
	:TA2Physics( name, analysis ) 
{
// Initialise Detectors
	fTAGG			= NULL; // Tagger
	fCB			= NULL; // CB (PID, MWPC, NaI)
	fTAPS			= NULL; // TAPS

	fNaI			= NULL; // CB
	fMWPC			= NULL; // MWPC
	fPID			= NULL; // PID

	fBaF2			= NULL;	// BaF2
	fVeto			= NULL; // TAPS Vetos

// Pi0Compton Variables

	// Particle Counters
	fNPhotTemp		= 0;
	fNPhoton		= 0;
	fNProton		= 0;
	fNPi0			= 0;
	fNUnknown		= 0;	

	// Particle arrays
	fTaggedPhoton		= NULL;
	fPhotTemp		= NULL;
	fPhoton 		= NULL;	
	fProton			= NULL;
	fPi0			= NULL;
	fUnknown		= NULL;

	fPhotonEnergy		= NULL;
	fPhotonTheta		= NULL;
	fPhotonPhi		= NULL;
	fPhotonTime		= NULL;

	fProtonEnergy		= NULL;
	fProtonTheta		= NULL;
	fProtonPhi		= NULL;
	fProtonTime		= NULL;

	fPi0Energy		= NULL;
	fPi0Theta		= NULL;
	fPi0Phi			= NULL;
	fPi0Time		= NULL;

	fIsPionPhoton		= NULL;
	fNPionPhoton		= 0;

	fNTagg			= 0;
	fTaggerChannel		= NULL;
	fTaggerTime		= NULL;

	fNPrompt 		= 0;
	fNRandom		= 0;
	fNTaggNPhot		= 0;

	fNPromptPi0		= 0;
	fNRandomPi0		= 0;
	fNTaggNPi0		= 0;

	// photon and pi0 physics reconstruction variables
	
	fPromptRandomRatio	= 0.0;
	fPromptRandomRatioPi0	= 0.0;

	f2PhotonInvariantMass	= NULL;
	fTaggerPhotonTime	= NULL;
	fTaggerPi0Time		= NULL;

	fTaggerChannelPrompt	= NULL;
	fTaggerChannelRandom	= NULL;
	fMissingMassPrompt	= NULL;
	fMissingMassRandom	= NULL;
	fPhotonThetaPrompt	= NULL;
	fPhotonThetaRandom	= NULL;
	fPhotonPhiPrompt	= NULL;
	fPhotonPhiRandom	= NULL;

	fTaggerChannelPromptPi0	= NULL;
	fTaggerChannelRandomPi0	= NULL;
	fMissingMassPromptPi0	= NULL;
	fMissingMassRandomPi0	= NULL;
	fPi0ThetaPrompt		= NULL;
	fPi0ThetaRandom		= NULL;
	fPi0PhiPrompt		= NULL;
	fPi0PhiRandom		= NULL;

	// Trigger Variables
	fCBESum			= 0.0;
	fNaINCluster		= 0;
	fBaF2NCluster		= 0;

	AddCmdList(kInputs);
}


//-----------------------------------------------------------------------------
TA2MyOwnPhysicsClass::~TA2MyOwnPhysicsClass()
{

// Delete Tree Files
//
//	delete fTree;
//	delete fFile;

}
	
//-----------------------------------------------------------------------------
void TA2MyOwnPhysicsClass::SetConfig(Char_t* line, Int_t key)
{
	// Any special command-line input for Crystal Ball apparatus

	switch (key){
		case EComptonPromptWindows:
			//  Compton Prompt Windows
			if( sscanf( line, "%d %d\n", &fPhotTimePL, &fPhotTimePR ) != 2 ){
				PrintError( line, "<Error: Compton Prompt Windows not set correctly>");
				return;
			}
		break;
		case EComptonRandomWindows:
			//  Compton Random Windows
			if( sscanf( line, "%d %d %d %d\n", &fPhotTimeRL1, &fPhotTimeRR1, &fPhotTimeRL2, &fPhotTimeRR2 ) != 4 ){
				PrintError( line, "<Error: Compton Random Windows not set correctly>");
				return;
			}
		break;
		case EPi0PromptWindows:
			//  Pi0 Prompt Windows
			if( sscanf( line, "%d %d\n", &fPi0TimePL, &fPi0TimePR ) != 2 ){

				PrintError( line, "<Error: Pi0 Prompt Windows not set correctly>");
				return;
			}
		break;
		case EPi0RandomWindows:
			//  Pi0 Random Windows
			if( sscanf( line, "%d %d %d %d\n", &fPi0TimeRL1, &fPi0TimeRR1, &fPi0TimeRL2, &fPi0TimeRR2 ) != 4 ){
				PrintError( line, "<Error: Pi0 Random Windows not set correctly>");
				return;
			}
		break;
                case EPi0InvariantMassCuts:
                        //  Pi0 Invariant Mass Cuts
                        if( sscanf( line, "%d %d\n", &fPi0InvMassCut1, &fPi0InvMassCut2 ) != 2 ){
                      		PrintError( line, "<Error: Pi0 Invariant Mass Cuts not set correctly>");
                       		return;
                        }
                break;
		case EProduceTreeFile:
			//  Pi0 Random Windows
			if( sscanf( line, "%d\n", &fProduceTreeFile) != 1 ){
				PrintError( line, "<Error: Tree files not turned on/off correctly>");
				return;
			}
			if(fProduceTreeFile == 1) printf("\n\nPhysics tree file enabled\n");
                        else printf("\n\nPhysics tree file disabled\n");
		break;
		case ETreeFileName:
			//  Tree File Name
			if( sscanf( line, "%s\n", fTreeFileName) != 1){
				PrintError( line, "<Error: Tree file name not set correctly>");
				return;
			}
			else printf("Physics class tree file will be saved to: %s\n\n", fTreeFileName);
		break;
		default:
		// default main apparatus SetConfig()
		TA2Physics::SetConfig( line, key );
		break;
	}
}

//---------------------------------------------------------------------------
void TA2MyOwnPhysicsClass::PostInit()
{

// Introduce Detectors

	// Tagger
	fTAGG = (TA2Tagger*)((TA2Analysis*)fParent)->GetChild("TAGG");
	if ( !fTAGG) PrintError("","<No Tagger class found>",EErrFatal);
	else {  printf("Tagger included in analysis\n");
		fTAGGParticles = fTAGG->GetParticles(); }

	// Ladder
	fLADD = (TA2Ladder*)((TA2Analysis*)fParent)->GetGrandChild( "FPD");
	if ( !fLADD) PrintError( "", "<No Ladder class found>", EErrFatal);

	// Central Apparatus
	fCB = (TA2CentralApparatus*)((TA2Analysis*)fParent)->GetChild("CB");	
	if (!fCB) PrintError( "", "<No Central Apparatus/CB class found>", EErrFatal);
	else {  printf("CB system included in analysis\n");
		fCBParticles  = fCB->GetParticles(); }

        // NaI
        fNaI = (TA2CalArray*)((TA2Analysis*)fParent)->GetGrandChild("NaI");
        if ( !fNaI) PrintError( "", "<No NaI class found>", EErrFatal);

	// TAPS
	fTAPS = (TA2Taps*)((TA2Analysis*)fParent)->GetChild("TAPS");
	if ( !fTAPS) printf("TAPS *NOT* included in analysis\n");
	else {  printf("TAPS included in analysis\n");
		fTAPSParticles = fTAPS->GetParticles();

	        fBaF2 = (TA2TAPS_BaF2*)((TA2Analysis*)fParent)->GetGrandChild("BaF2PWO");
	        if (!fBaF2) PrintError( "", "<No BaF2 class found>", EErrFatal);
	}

	printf("\n");

// Calculate ratio of prompt to random windows

	if (gAR->GetProcessType() == EMCProcess) {
		fPromptRandomRatio	= 0.0;
		fPromptRandomRatioPi0	= 0.0;		
	}
	else {
		fPromptRandomRatio	= double(fPi0TimePR - fPi0TimePL)/double(fPi0TimeRR1 - fPi0TimeRL1 + fPi0TimeRR2 - fPi0TimeRL2);
		fPromptRandomRatioPi0	= double(fPi0TimePR - fPi0TimePL)/double(fPi0TimeRR1 - fPi0TimeRL1 + fPi0TimeRR2 - fPi0TimeRL2);
 	}

// Get max # of Particles from detectors, used for defining array sizes

	fCBMaxNParticle 	= fCB->GetMaxParticle();	
	if (fTAPS) 						
	fTAPSMaxNParticle	= fTAPS->GetMaxParticle(); 
	else fTAPSMaxNParticle 	= 0;

	fMaxNParticle		= fCBMaxNParticle + fTAPSMaxNParticle;  

// Create arrays to hold Particles

	fPhotTemp		= new TA2Particle*[fMaxNParticle];
	fPhoton 		= new TA2Particle*[fMaxNParticle];	
	fProton			= new TA2Particle*[fMaxNParticle];

	UInt_t squareMax		= fMaxNParticle*fMaxNParticle*2;
	fPi0	 		= new TA2Particle*[squareMax];
	TA2Particle* part	= new TA2Particle[squareMax];
	for ( i = 0; i < squareMax; i++) fPi0[i] = part + i;

	fUnknown		= new TA2Particle*[fMaxNParticle];
	fTaggedPhoton		= new TA2Particle*[352];

	fPhotonEnergy		= new Double_t[fMaxNParticle];
	fPhotonTheta		= new Double_t[fMaxNParticle];
	fPhotonPhi		= new Double_t[fMaxNParticle];
	fPhotonTime		= new Double_t[fMaxNParticle];

	fProtonEnergy		= new Double_t[fMaxNParticle];
	fProtonTheta		= new Double_t[fMaxNParticle];
	fProtonPhi		= new Double_t[fMaxNParticle];
	fProtonTime		= new Double_t[fMaxNParticle];

	fPi0Energy		= new Double_t[fMaxNParticle];
	fPi0Theta		= new Double_t[fMaxNParticle];
	fPi0Phi			= new Double_t[fMaxNParticle];
	fPi0Time		= new Double_t[fMaxNParticle];

	fIsPionPhoton		= new Bool_t[fMaxNParticle];
	f2PhotonInvariantMass	= new Double_t[squareMax];

	fTaggerTime		= new Double_t[352];
	fTaggerPhotonTime	= new Double_t[352*fMaxNParticle];
	fTaggerPi0Time		= new Double_t[352*fMaxNParticle];
	fTaggerChannel		= new Int_t[352*5];

	fTaggerChannelPrompt 	= new Int_t[352*fMaxNParticle*fMaxNParticle];
	fTaggerChannelRandom 	= new Int_t[352*fMaxNParticle*fMaxNParticle];
	fMissingMassPrompt	= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fMissingMassRandom	= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPhotonThetaPrompt	= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPhotonThetaRandom	= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPhotonPhiPrompt	= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPhotonPhiRandom	= new Double_t[352*fMaxNParticle*fMaxNParticle];

	fTaggerChannelPromptPi0 = new Int_t[352*fMaxNParticle*fMaxNParticle];
	fTaggerChannelRandomPi0 = new Int_t[352*fMaxNParticle*fMaxNParticle];
	fMissingMassPromptPi0	= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fMissingMassRandomPi0	= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPi0ThetaPrompt		= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPi0ThetaRandom		= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPi0PhiPrompt		= new Double_t[352*fMaxNParticle*fMaxNParticle];
	fPi0PhiRandom		= new Double_t[352*fMaxNParticle*fMaxNParticle];

// Create Tree Files, Define Branches (if option is turned on "fProduceTreeFile ==1")

	if(fProduceTreeFile == 1){

	fFile = new TFile(fTreeFileName, "RECREATE", "Physics", 3);
	fTree = new TTree("Pi0ComptonTree", "Compton and Pi0 Kinematics");
	fTree->Branch("NPhotTemp",	&fNPhotTemp, 	"NPhotTemp/I");
	fTree->Branch("NPhoton",	&fNPhoton, 	"NPhoton/I");
	fTree->Branch("NProton",	&fNProton, 	"NProton/I");
	fTree->Branch("NPi0",		&fNPi0, 	"NPi0/I");
	fTree->Branch("NUnknown",	&fNUnknown, 	"NUnknown/I");
	fTree->Branch("NPionPhoton",	&fNPionPhoton, 	"NPionPhoton/I");
	fTree->Branch("CBNParticle",	&fCBNParticle, 	"CBNParticle/I");

	fTree->Branch("PhotonEnergy",	fPhotonEnergy,	"PhotonEnergy[NPhoton]/D");
	fTree->Branch("PhotonTheta",	fPhotonTheta, 	"PhotonTheta[NPhoton]/D");
	fTree->Branch("PhotonPhi",	fPhotonPhi, 	"PhotonPhi[NPhoton]/D");
	fTree->Branch("PhotonTime",	fPhotonTime, 	"PhotonTime[NPhoton]/D");

	fTree->Branch("ProtonEnergy",	fProtonEnergy,	"ProtonEnergy[NProton]/D");
	fTree->Branch("ProtonTheta",	fProtonTheta, 	"ProtonTheta[NProton]/D");
	fTree->Branch("ProtonPhi",	fProtonPhi, 	"ProtonPhi[NProton]/D");
	fTree->Branch("ProtonTime",	fProtonTime, 	"ProtonTime[NProton]/D");

	fTree->Branch("Pi0Energy",	fPi0Energy,	"Pi0Energy[NPi0]/D");
	fTree->Branch("Pi0Theta",	fPi0Theta, 	"Pi0Theta[NPi0]/D");
	fTree->Branch("Pi0Phi",		fPi0Phi, 	"Pi0Phi[NPi0]/D");
	fTree->Branch("Pi0Time",	fPi0Time, 	"Pi0Time[NPi0]/D");

	fTree->Branch("NTagg",		&fNTagg,	"NTagg/I");
	fTree->Branch("TaggerChannel",	fTaggerChannel,	"TaggerChannel[NTagg]/I");

	fTree->Branch("NPrompt",	&fNPrompt, 	"NPrompt/I");
	fTree->Branch("NRandom",	&fNRandom, 	"NRandom/I");
	fTree->Branch("NTaggNPhot",	&fNTaggNPhot,	"NTaggNPhot/I");

	fTree->Branch("NPromptPi0",	&fNPromptPi0, 	"NPromptPi0/I");
	fTree->Branch("NRandomPi0",	&fNRandomPi0, 	"NRandomPi0/I");
	fTree->Branch("NTaggNPi0",	&fNTaggNPi0,	"NTaggNPi0/I");

	fTree->Branch("TaggerTime",  		fTaggerTime,   		"TaggerTime[NTagg]/D");
	fTree->Branch("TaggerPhotonTime",  	fTaggerPhotonTime,   	"TaggerPhotonTime[NTaggNPhot]/D");
	fTree->Branch("TaggerPi0Time",	   	fTaggerPi0Time,      	"TaggerPi0Time[NTaggNPi0]/D");

	fTree->Branch("N2PhotonInvariantMass", 	&fN2PhotonInvariantMass,"N2PhotonInvariantMass/I");
	fTree->Branch("2PhotonInvariantMass",  	f2PhotonInvariantMass,  "2PhotonInvariantMass[N2PhotonInvariantMass]/D");

	fTree->Branch("PromptRandomRatio",	&fPromptRandomRatio,	"PromptRandomRatio/D");
	fTree->Branch("TaggerChannelPrompt",	fTaggerChannelPrompt,	"TaggerChannelPrompt[NPrompt]/I");
	fTree->Branch("TaggerChannelRandom",	fTaggerChannelRandom,	"TaggerChannelRandom[NRandom]/I");
	fTree->Branch("MissingMassPrompt",	fMissingMassPrompt,	"MissingMassPrompt[NPrompt]/D");
	fTree->Branch("MissingMassRandom",	fMissingMassRandom,	"MissingMassRandom[NRandom]/D");
	fTree->Branch("PhotonThetaPrompt",	fPhotonThetaPrompt, 	"PhotonThetaPrompt[NPrompt]/D");
	fTree->Branch("PhotonThetaRandom",	fPhotonThetaRandom, 	"PhotonThetaRandom[NRandom]/D");
	fTree->Branch("PhotonPhiPrompt",	fPhotonPhiPrompt, 	"PhotonPhiPrompt[NPrompt]/D");
	fTree->Branch("PhotonPhiRandom",	fPhotonPhiRandom, 	"PhotonPhiRandom[NRandom]/D");

	fTree->Branch("PromptRandomRatioPi0",	&fPromptRandomRatioPi0,	"PromptRandomRatioPi0/D");
	fTree->Branch("TaggerChannelPromptPi0",	fTaggerChannelPromptPi0,"TaggerChannelPromptPi0[NPromptPi0]/I");
	fTree->Branch("TaggerChannelRandomPi0",	fTaggerChannelRandomPi0,"TaggerChannelRandomPi0[NRandomPi0]/I");
	fTree->Branch("MissingMassPromptPi0",	fMissingMassPromptPi0,	"MissingMassPromptPi0[NPromptPi0]/D");
	fTree->Branch("MissingMassRandomPi0",	fMissingMassRandomPi0,	"MissingMassRandomPi0[NRandomPi0]/D");
	fTree->Branch("Pi0ThetaPrompt",		fPi0ThetaPrompt, 	"Pi0ThetaPrompt[NPromptPi0]/D");
	fTree->Branch("Pi0ThetaRandom",		fPi0ThetaRandom, 	"Pi0ThetaRandom[NRandomPi0]/D");
	fTree->Branch("Pi0PhiPrompt",		fPi0PhiPrompt, 		"Pi0PhiPrompt[NPromptPi0]/D");
	fTree->Branch("Pi0PhiRandom",		fPi0PhiRandom, 		"Pi0PhiRandom[NRandomPi0]/D");

        fTree->Branch("CBESum",  		&fCBESum,		"CBESum/D");
        fTree->Branch("NaINCluster",            &fNaINCluster,          "NaINCluster/I");
        fTree->Branch("BaF2NCluster",           &fBaF2NCluster,         "BaF2NCluster/I");

	gROOT->cd();
	}
	// Default physics initialisation
	TA2Physics::PostInit();
}

//-----------------------------------------------------------------------------
void TA2MyOwnPhysicsClass::LoadVariable( )
{

// Input name - variable pointer associations for any subsequent cut/histogram setup

	TA2Physics::LoadVariable();

	TA2DataManager::LoadVariable("NPhoton", 		&fNPhoton,			EISingleX);
	TA2DataManager::LoadVariable("PhotonTheta", 		fPhotonTheta,			EDMultiX);
	TA2DataManager::LoadVariable("PhotonPhi", 		fPhotonPhi,			EDMultiX);
	TA2DataManager::LoadVariable("PhotonEnergy", 		fPhotonEnergy,			EDMultiX);
	TA2DataManager::LoadVariable("PhotonTime", 		fPhotonTime,			EDMultiX);

	TA2DataManager::LoadVariable("NProton", 		&fNProton,			EISingleX);
	TA2DataManager::LoadVariable("ProtonTheta", 		fProtonTheta,			EDMultiX);
	TA2DataManager::LoadVariable("ProtonPhi", 		fProtonPhi,			EDMultiX);
	TA2DataManager::LoadVariable("ProtonEnergy", 		fProtonEnergy,			EDMultiX);
	TA2DataManager::LoadVariable("ProtonTime", 		fProtonTime,			EDMultiX);

	TA2DataManager::LoadVariable("NPi0", 			&fNPi0,				EISingleX);
	TA2DataManager::LoadVariable("Pi0Theta", 		fPi0Theta,			EDMultiX);
	TA2DataManager::LoadVariable("Pi0Phi", 			fPi0Phi,			EDMultiX);
	TA2DataManager::LoadVariable("Pi0Energy", 		fPi0Energy,			EDMultiX);
	TA2DataManager::LoadVariable("Pi0Time", 		fPi0Time,			EDMultiX);

	TA2DataManager::LoadVariable("2PhotonInvariantMass", 	f2PhotonInvariantMass,		EDMultiX);

	TA2DataManager::LoadVariable("TaggerChannel",		fTaggerChannel,			EIMultiX);

	TA2DataManager::LoadVariable("TaggerTime",		fTaggerTime,			EDMultiX);
	TA2DataManager::LoadVariable("TaggerPhotonTime",	fTaggerPhotonTime,		EDMultiX);
	TA2DataManager::LoadVariable("TaggerPi0Time",		fTaggerPi0Time,			EDMultiX);

	TA2DataManager::LoadVariable("PromptRandomRatio",	&fPromptRandomRatio,		EDSingleX);
	TA2DataManager::LoadVariable("PromptRandomRatioPi0",	&fPromptRandomRatioPi0,		EDSingleX);	
	
	TA2DataManager::LoadVariable("TaggerChannelPrompt",	fTaggerChannelPrompt,		EIMultiX);
	TA2DataManager::LoadVariable("TaggerChannelRandom",	fTaggerChannelRandom,		EIMultiX);
	TA2DataManager::LoadVariable("MissingMassPrompt",	fMissingMassPrompt,		EDMultiX);
	TA2DataManager::LoadVariable("MissingMassRandom",	fMissingMassRandom,		EDMultiX);
	TA2DataManager::LoadVariable("PhotonThetaPrompt",	fPhotonThetaPrompt, 		EDMultiX);
	TA2DataManager::LoadVariable("PhotonThetaRandom",	fPhotonThetaRandom, 		EDMultiX);
	TA2DataManager::LoadVariable("PhotonPhiPrompt",		fPhotonPhiPrompt, 		EDMultiX);
	TA2DataManager::LoadVariable("PhotonPhiRandom",		fPhotonPhiRandom, 		EDMultiX);

	TA2DataManager::LoadVariable("TaggerChannelPromptPi0",	fTaggerChannelPromptPi0,	EIMultiX);
	TA2DataManager::LoadVariable("TaggerChannelRandomPi0",	fTaggerChannelRandomPi0,	EIMultiX);
	TA2DataManager::LoadVariable("MissingMassPromptPi0",	fMissingMassPromptPi0,		EDMultiX);
	TA2DataManager::LoadVariable("MissingMassRandomPi0",	fMissingMassRandomPi0,		EDMultiX);
	TA2DataManager::LoadVariable("Pi0ThetaPrompt",		fPi0ThetaPrompt, 		EDMultiX);
	TA2DataManager::LoadVariable("Pi0ThetaRandom",		fPi0ThetaRandom, 		EDMultiX);
	TA2DataManager::LoadVariable("Pi0PhiPrompt",		fPi0PhiPrompt, 			EDMultiX);
	TA2DataManager::LoadVariable("Pi0PhiRandom",		fPi0PhiRandom, 			EDMultiX);

	TA2DataManager::LoadVariable("CBESum",			&fCBESum,			EDSingleX);
	TA2DataManager::LoadVariable("NaINCluster",		&fNaINCluster,			EISingleX);
	TA2DataManager::LoadVariable("BaF2NCluster",		&fBaF2NCluster,			EISingleX);

	return;
}

//-----------------------------------------------------------------------------
void TA2MyOwnPhysicsClass::Reconstruct() 
{
// Get # of Particles from detectors

	fTAGGNParticle 		= fTAGG->GetNparticle();
	fCBNParticle 		= fCB->GetNParticle();
	if (fTAPS) 					
	fTAPSNParticle		= fTAPS->GetNparticle(); 
	else fTAPSNParticle 	= 0;
// Sort according to Particle type
	
	fNPhotTemp		= 0;
	fNPhoton		= 0;
	fNProton		= 0;
	fNPi0			= 0;
	fNUnknown		= 0;	
	
	// CentAPP
	for (i = 0; i < fCBParticle; i++) {
		
		switch( (fCBParticles+i)->GetParticleID() ) { // Obtaining PDG code
		
		case: kGamma 									//ID'd as photon
		fPhotTemp[fNPhotTemp] = fCBParticles+i;		//Adding to photon Array
		fNPhotTemp++;
		break;
		
		/*case: kProton									//ID'd as proton
		fProton[fNProton] = fCBParticles+i;			//Adding to Proton Array
		fNProton++;
		break;*/
		
		default: 
		fUnknown[fNUnknown] = fCBParticles+i;		//Included in "Unknown" list
		fNUnknown++;
		}
	}
	//Particles from TAPS
	if(fTAPS) {
		for (i = 0; i < fTAPSNParticles; i++) {
			
				switch( (fTAPSPatricles+i)->GetParticleID() ) { //Getting the PDG code of particles in TAPS
				
				case kGamma:								//ID'd as photon
				fPhotTemp[fNPhotTemp] = fTAPSParticles+i; //Adding to photon array
				fNPhotTemp++;
				break;
				
				/*case kProton:								//ID'd as proton
				fProton[fNProton] = fTAPSParticles+i;	//Adding to proton array
				fNProton++;
				break;*/
				
				default:
				fUnknown[fNUnknown] = fTAPSParticles+i;		//Include in "Unknown" list
				fNUnknown++;
				}
			}
		}
		
//Reconstruct missing mass of proton from detected photons

	TLorentzVector p4;
	Double_t time;
	fN2PhotonInvariantMass = 0;
	
	for (i = 0; i < fNPhotTemp; i++) {
	
		TA2Particle photon1 = *fPhotTemp[i]; //This is the detected particle 
		
		p4 = photon1.GetP4(); //This is getting the four momentum of the photon (in this case) which has been detected
		
		f2PhotonInvariantMass[fN2PhotonInvariantMass] = p4.M(); //Obtaining the invariant mass of detected particle (photon)
		
		fN2PhotonInvariantMass++; //Increasing counter
		
	}
	
	//Fill Photon properties (Since we only simulated compton scattering there is no need to remove pion photons)
	for ( i = 0; i < fNPhtoTemp; i++) {
	TA2Particle photon = *fPhotTemp[i];
	fPhoton[fNPhoton] = fPhotTemp[i];
	fPhotonEnergy[fNPhoton] = photon.GetT();
	fPhotonTheta[fNPhoton] = photon.GetThetaDg();
	fPhotonPhi[fNPhoton] = photon.GetPhiDg();
	fPhotonTime[fNPhoton] = photon.GetTime();
	fNPhoton++;
	}
	
	//Caclulating the Missing Mass for all photons
	
	fNTagg  = fTAGGNParticcle;
	for (i = 0; i < fNTagg; i++) {
		fTaggerChannel[i]  = (fLADD->GetHits())[i];
		fTaggedPhoton[i]   = fTAGGParticles+i;
		TA2Particle taggerphton = *fTaggedPhoton[i];
		fTaggerTime[i]          =  taggerphoton.GetTime();
	}
	
	fNTaggNPhot = 0;
	fNPrompt    = 0;
	fNRandom    = 0;
	
	for ( i = 0; i < fNPhoton; i++) [
	
		for (j = 0; j < fNTagg; j++) {
			
			TA2Particle taggerphoton   = *fTaggedPhoton[j];
			fTaggerPhotonTime[fNTaggNPhot] = fTaggerTime[j] - fPhotonTime[i];
			TA2Particle photon       = *fPhoton[i];
			
			TLorentzVecotr p4incident, p4missing; //p4incidnet is the beam energy
			p4incident = fP4target[0] + taggerphoton.GetP4();
			p4missing  = p4incidnet   - photon.GetP4();
			
			if ( (fTaggerPhotonTime[fNTaggNPhot] >= fPhotTimePL && fTaggerPhotonTime[fNTaggNPhot] <= fPhotTimePR) ||
			  	(gAR->GetProcessType() == EMCProcess) ) {

				fTaggerChannelPrompt[fNPrompt]  = fTaggerChannel[j];
				fMissingMassPrompt[fNPrompt]	= p4missing.M();
				fPhotonThetaPrompt[fNPrompt]	= fPhotonTheta[i];
				fPhotonPhiPrompt[fNPrompt]	= fPhotonPhi[i];
				fNPrompt++;
			}

			if ( (fTaggerPhotonTime[fNTaggNPhot] >= fPhotTimeRL1 && fTaggerPhotonTime[fNTaggNPhot] <= fPhotTimeRR1) ||
			     (fTaggerPhotonTime[fNTaggNPhot] >= fPhotTimeRL2 && fTaggerPhotonTime[fNTaggNPhot] <= fPhotTimeRR2) ) {

				fTaggerChannelRandom[fNRandom]  = fTaggerChannel[j];
				fMissingMassRandom[fNRandom]	= p4missing.M();
				fPhotonThetaRandom[fNRandom]	= fPhotonTheta[i];
				fPhotonPhiRandom[fNRandom]	= fPhotonPhi[i];

				fNRandom++;
			}
		fNTaggNPhot++;
		}
	}
	
// Calculating Missing Mass for all proton 

	fNTaggNPi0	= 0;
	fNPromptPi0	= 0;
	fNRandomPi0	= 0;
	Doubtle_t ProtonRestMass = 938;
	
	for (i = 0; i < fNPi0; i++) {
	
		for (j = 0; j < fNTagg; j++) {
		
			TA2Particle taggerphoton    = *fTaggedPhoton[j];
			TLorentzVecotr p4incident, p4missing;
			
			
	
	
	
	
	
	
		
		
		
		
		
		

		
				
		
		
		


