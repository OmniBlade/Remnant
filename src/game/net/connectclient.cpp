/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Connection function for accepting incoming client requests.
 *
 * @copyright Remnant is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "connectclient.h"
#include "listener.h"
#include "reliable.h"
#include "vector.h"

extern DynamicVectorClass<ProtocolClass *> &ProtocolsVector;
extern DynamicVectorClass<ReliableCommClass *> &CommsVector;

BOOL Connect_To_Client(ListenerClass *listener)
{
    const int MAX_PACKET = 422;
    ProtocolClass *protocol = new ReliableProtocolClass();
    ReliableCommClass *comms = new ReliableCommClass(protocol, MAX_PACKET);

    ProtocolsVector.Add_Head(protocol);
    CommsVector.Add_Head(comms);

    // If we successfully connect here, we are done.
    if (CommsVector[0]->Connect(listener)) {
        return true;
    }

    CommsVector[0]->Disconnect();
    delete CommsVector[0]; 
    CommsVector.Delete(0);

    delete ProtocolsVector[0];
    ProtocolsVector.Delete(0);

    return false;
}
