/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Interface for protocol class implementations.
 *
 * @copyright Remnant is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#ifndef LISTENER_H
#define LISTENER_H

#include "always.h"
#include "protocol.h"
#include <sockets.h>

struct DestAddress
{
    char Host[30];
    unsigned short Port;
};

class ListenerProtocolClass : public ProtocolClass
{
public:
    ListenerProtocolClass();
    virtual ~ListenerProtocolClass();

    virtual void Data_Received() override {}
    virtual void Connected_To_Server(int status) override {}
    virtual void Connection_Requested() override;
    virtual void Closed() override {}
    virtual void Name_Resolved() override {}

private:
    unsigned ConnectionRequests;
};

class ListenerClass
{
    friend void Setup_Hooks();
public:
    ListenerClass();
    ListenerClass(ProtocolClass *protocol);
    virtual ~ListenerClass();

    virtual void Register(ProtocolClass *protocol);
    virtual int Start_Listening(void *address, int size, int unk);
    virtual void Stop_Listening();

private:
    void Create_Window();
    void Destroy_Window();
    int Open_Socket(unsigned short port);
    void Close_Socket();
    static long __stdcall Window_Proc(HWND hwnd, unsigned umsg, unsigned wparam, long lparam);

public:
    SOCKET Socket;
    ProtocolClass *Protocol;
    HWND Window;
    int IsListening : 1;
};

int Init_Listener();
void Deinit_Listener();

#endif
