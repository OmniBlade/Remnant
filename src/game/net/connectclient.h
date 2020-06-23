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
#pragma once

#ifndef CONNECTCLIENT_H
#define CONNECTCLIENT_H

#include "always.h"

class ListenerClass;

BOOL Connect_To_Client(ListenerClass *listener);

#endif // CONNECTCLIENT_H