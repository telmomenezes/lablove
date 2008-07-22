/*
 * Gridbrain
 * Copyright (C) 2007 Telmo Menezes.
 * telmo@telmomenezes.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the version 2 of the GNU General Public License 
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __INCLUDE_GRID_COORD_H
#define __INCLUDE_GRID_COORD_H

#include "types.h"
#include <string>
using std::string;

namespace gb
{

class Coord
{
public:
    Coord();
    Coord(llULINT depth, llULINT branch);
    Coord(const Coord& coord);
    virtual ~Coord();

    bool operator==(const Coord &gc) const;

    Coord leftOf();
    Coord rightOf();

    int position(Coord gc);

    llULINT getDepth(){return mDepth;}
    llULINT getBranch(){return mBranch;}

    string toString();

    bool isValid();


protected:
    llULINT mDepth;
    llULINT mBranch;
};

}

#endif

