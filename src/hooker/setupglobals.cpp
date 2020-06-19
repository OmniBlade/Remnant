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
