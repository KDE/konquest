/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright <inge@lysator.liu.se>
    Copyright <pinaraf@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef _GAMECORE_H_
#define _GAMECORE_H_

#include <krandomsequence.h>
#include "sector.h"

//**********************************************************
// Core Logic routines
//**********************************************************

class CoreLogic
{
public:
    CoreLogic();

    Coordinate  generatePlanetCoordinates (int rows, int cols);
    double      generateKillPercentage();
    int         generatePlanetProduction();

    double      roll();

private:
    KRandomSequence  random;
};

#endif // _GAMECORE_H_


