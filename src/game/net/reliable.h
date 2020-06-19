/**
 * @file
 *
 * @author OmniBlade
 * @author tomsons26
 *
 * @brief Class for handling a TCP/IP connection.
 *
 * @copyright Remnant is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef RELIABLE_H
#define RELIABLE_H

#include "commbuff.h"
#include "protocol.h"
#include "sockets.h"

class ListenerClass;

class ReliableProtocolClass : public ProtocolClass
{
public:
    ReliableProtocolClass();

    virtual ~ReliableProtocolClass() {}

    virtual void Data_Received() override {}
    virtual void Connected_To_Server(int status) override
    {
        if (status == 1) {
            Connected = 1;
        } else {
            Connected = -1;
        }
    }
    virtual void Connection_Requested() override {}
    virtual void Closed() override { Connected = 0; }
    virtual void Name_Resolved() override { NameResolved = 1; }

    int Connected;
    BOOL NameResolved;
};

struct TCPHost
{
    int Addr;
    char DotAddr[16];
    char Name[255];
    short Port;
};

class ReliableCommClass
{
public:
    ReliableCommClass(int maxpacketsize);
    ReliableCommClass(ProtocolClass *protocol, int maxpacketsize);

    virtual ~ReliableCommClass();
    virtual void Register(ProtocolClass *protocol);
    virtual int Connect(void *, int, int);
    virtual int Connect(ListenerClass *listener);
    virtual void Disconnect();
    virtual int Send();

private:
    static unsigned short Network_Short(unsigned short);
    static unsigned short Local_Short(unsigned short);
    static unsigned long Network_Long(unsigned long);
    static unsigned long Local_Long(unsigned long);
    void Create_Window();
    void Destroy_Window();
    int Open_Socket();
    void Close_Socket();
    static long __stdcall Window_Proc(HWND hwnd, unsigned int message, unsigned int wParam, long lParam);

private:
    TCPHost Host;
    int MaxPacketSize;
    SOCKET Socket;
    ProtocolClass *Protocol;
    HWND Window;
    HANDLE Async;
    char Hbuf[1024];
    CommBufferClass::SendQueueType *SendEntry;
    int SendLen;
    char *ReceiveBuf;
    int ReceiveLen;
    int IsConnected : 1;
};

#endif
