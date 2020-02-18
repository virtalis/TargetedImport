// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

HMODULE hExchangeDLL = LoadLibrary(L"vrtree-exchange.dll");

typedef uint32_t(*VRXImportFunc)(const char*, HNode, HNode, const char*);
VRXImportFunc VRXImport = (VRXImportFunc)GetProcAddress(hExchangeDLL, "__vrexport_VRXImport");

typedef uint32_t(*VRXImportAndMergeFunc)(const char*, HNode, HNode, HNode, const char*);
VRXImportAndMergeFunc VRXImportAndMerge = (VRXImportAndMergeFunc)GetProcAddress(hExchangeDLL, "__vrexport_VRXImportAndMerge");

// Implement all of the standard API functions for api versioning, 
// and other hooks such as logging and progress displays
VRPLUGIN_API_IMPL;

// before using the s_logFunc hook, it should be checked for null
#define LOG(LEVEL,LOGSTRING) if (s_logFunc) s_logFunc(LEVEL, LOGSTRING);

// local success = TargetedImport.Import(filepath, scenesRoot, librariesRoot, importerName)
static HFFIVar Import(int argc, HFFIVar* argv, void*) {
    // Check types, hacky cos they're not written down...
    int stringTypeID = VRFFIGetType(VRFFIMakeString(""));
    int nodeTypeID = VRFFIGetType(VRFFIMakeNode(VRGetRootNode()));

    bool filepathValid = VRFFIGetType(argv[0]) == stringTypeID;
    bool scenesRootValid = VRFFIGetType(argv[1]) == nodeTypeID;
    bool librariesRootValid = VRFFIGetType(argv[2]) == nodeTypeID;

    // Report errors
    if (!filepathValid) LOG(1, "filepath argument to Import must be a string");
    if (!scenesRootValid) LOG(1, "scenesRoot argument to Import must be a node");
    if (!librariesRootValid) LOG(1, "librariesRoot argument to Import must be a node");

    if (!filepathValid || !scenesRootValid || !librariesRootValid) {
        return nullptr;
    }

    const char* filepath = VRFFIGetString(argv[0]);
    HNode scenesRoot = VRFFIGetNode(argv[1]);
    HNode librariesRoot = VRFFIGetNode(argv[2]);

    // Check optional importer name, default to null
    const char* importerName = nullptr;
    if (argc >= 4) {
        // Importer name provided, check it
        if (VRFFIGetType(argv[3]) == stringTypeID) {
            importerName = VRFFIGetString(argv[3]);
        }
        else {
            LOG(1, "If provided, importerName argument to Import must be a string");
            return nullptr;
        }
    }

    // Do the import
    int importSuccess = VRXImport(
        filepath,
        scenesRoot,
        librariesRoot,
        importerName
    );

    char logBuffer[50];
    (void)sprintf_s(logBuffer, "VRXImport returned: %d\n", importSuccess);
    LOG(0, logBuffer);

    return VRFFIMakeBool(importSuccess == 0);
}

// local success = TargetedImport.ImportAndMerge(filepath, scenesRoot, librariesRoot, mergeSettings, importerName)
static HFFIVar ImportAndMerge(int argc, HFFIVar* argv, void*) {
    // Check types, hacky cos they're not written down...
    int stringTypeID = VRFFIGetType(VRFFIMakeString(""));
    int nodeTypeID = VRFFIGetType(VRFFIMakeNode(VRGetRootNode()));

    bool filepathValid = VRFFIGetType(argv[0]) == stringTypeID;
    bool scenesRootValid = VRFFIGetType(argv[1]) == nodeTypeID;
    bool librariesRootValid = VRFFIGetType(argv[2]) == nodeTypeID;
    bool mergeSettingsValid = VRFFIGetType(argv[3]) == nodeTypeID;

    // Report errors
    if (!filepathValid) LOG(1, "filepath argument to ImportAndMerge must be a string");
    if (!scenesRootValid) LOG(1, "scenesRoot argument to ImportAndMerge must be a node");
    if (!librariesRootValid) LOG(1, "librariesRoot argument to ImportAndMerge must be a node");
    if (!mergeSettingsValid) LOG(1, "mergeSettings argument to ImportAndMerge must be a node");

    if (!filepathValid || !scenesRootValid || !librariesRootValid || !mergeSettingsValid) {
        return nullptr;
    }

    const char* filepath = VRFFIGetString(argv[0]);
    HNode scenesRoot = VRFFIGetNode(argv[1]);
    HNode librariesRoot = VRFFIGetNode(argv[2]);
    HNode mergeSettings = VRFFIGetNode(argv[3]);

    // Check optional importer name, default to null
    const char* importerName = nullptr;
    if (argc >= 5) {
        // Importer name provided, check it
        if (VRFFIGetType(argv[4]) == stringTypeID) {
            importerName = VRFFIGetString(argv[4]);
        }
        else {
            LOG(1, "If provided, importerName argument to ImportAndMerge must be a string");
            return nullptr;
        }
    }

    // Do the import
    int importSuccess = VRXImportAndMerge(
        filepath,
        scenesRoot,
        librariesRoot,
        mergeSettings,
        importerName
    );

    char logBuffer[50];
    (void)sprintf_s(logBuffer, "VRXImport returned: %d\n", importSuccess);
    LOG(0, logBuffer);

    return VRFFIMakeBool(importSuccess == 0);
}
// Enable API stuff
PLUGIN_ENTRY_POINT const char* VRTREE_APIENTRY VRPSignature()
{
    return "<VRTREE_API><company>Virtalis</company><feature>TargetedImport</feature><feature>TargetedImport.dll</feature><feature>SCC_PLUGIN</feature></VRTREE_API>4c3a4233335dab37e8fcec475d4e17a5d14b076f51d1e91650f1684726427eb8f54ab3cc2ab23becedf30164aa9e3b0713a041d24a5c9721b1064cf2b375ab7834ff450c2902eb735a06e65d104e52365995776a276f94cb9f8f641c8062b93647dd3fbfb089496a897c13f907102981fef653928f71310fa6a83cb91dcd9ebd";
}

PLUGIN_ENTRY_POINT const char* VRTREE_APIENTRY VRPName()
{
    return "TargetedImport";
}

PLUGIN_ENTRY_POINT const char* VRTREE_APIENTRY VRPVersion()
{
    return "1.0.0";
}

// Implement VRPInit to respond to application startup
PLUGIN_ENTRY_POINT int VRTREE_APIENTRY VRPInit()
{
    // load all of the VRTree C API functions. Note that a valid API license is required (see licensing section below this example).
    VRPLUGIN_LOADVRTREE;

    // before using any of the hooks such as s_logFunc, it should be checked for null
    if (s_logFunc)
        s_logFunc(LOG_INFO, "Targeted Import Plugin Initialised.");

    // Register utility functions
    VRFFIRegister("Import", &Import, 3, nullptr);
    VRFFIRegister("ImportAndMerge", &ImportAndMerge, 4, nullptr);

    return 0;
}

// Implement VRPCleanup to respond to unloading of the plugin
PLUGIN_ENTRY_POINT int VRTREE_APIENTRY VRPCleanup()
{
    return 0;
}