#include "hooker.h"

// alloc.cpp
#include "alloc.h"
memerrorhandler_t &g_MemoryError = Make_Global<memerrorhandler_t>(0x005EEA58); // Memory error handler function pointer.
memexithandler_t &g_MemoryErrorExit = Make_Global<memexithandler_t>(0x005EEA5C);

// listener.cpp
class ProtocolClass;
class ListenerClass;
HINSTANCE &hWSockInstance = Make_Global<HINSTANCE>(0x005EEC10);
ProtocolClass *&ListenerProtocol = Make_Global<ProtocolClass *>(0x00577B14);
ListenerClass *&Listener = Make_Global<ListenerClass *>(0x00577B18);
long &PlanetWestwoodPortNumber = Make_Global<long>(0x005346EC);
char &GameToPlay = Make_Global<char>(0x00576AF1);

// cmdline.cpp
class SpecialClass;
int &Scenario = Make_Global<int>(0x005729F0);
char &ScenPlayer = Make_Global<char>(0x005729F4);
char &ScenDir = Make_Global<char>(0x005729F5);
char &Whom = Make_Global<char>(0x005729EC);
SpecialClass &Special = Make_Global<SpecialClass>(0x005729E8);
BOOL &Debug_Map = Make_Global<BOOL>(0x0057105C);
BOOL &Debug_Unshroud = Make_Global<BOOL>(0x00571064);
BOOL &Debug_Quiet = Make_Global<BOOL>(0x00571040);
BOOL &OfflineMode = Make_Global<BOOL>(0x00591BD0);
int &WChatMode = Make_Global<int>(0x00591884);
