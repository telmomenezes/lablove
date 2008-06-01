/*
 * LabLOVE
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

#ifndef __INCLUDE_GRIDBRAIN_STATE_H
#define __INCLUDE_GRIDBRAIN_STATE_H

#include "types.h"
#include <string>
using std::string;

class GridbrainState
{
public:
    GridbrainState(unsigned int size);
    virtual ~GridbrainState();

    bool operator==(const GridbrainState &state) const;

protected:
    unsigned char* mBits;
    unsigned long mCount;
    unsigned int mSize;
};
#endif

