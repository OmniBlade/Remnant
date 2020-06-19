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
#ifndef PROTOCOL_H
#define PROTOCOL_H

class CommBufferClass;

class ProtocolClass
{
public:
    virtual void Data_Received() = 0;
    virtual void Connected_To_Server(int) = 0;
    virtual void Connection_Requested() = 0;
    virtual void Closed() = 0;
    virtual void Name_Resolved() = 0;

public:
    CommBufferClass *Queue;
};

#endif
