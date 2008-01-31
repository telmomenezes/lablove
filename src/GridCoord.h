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

#ifndef __INCLUDE_GRID_COORD_H
#define __INCLUDE_GRID_COORD_H

class GridCoord
{
public:
    GridCoord();
    GridCoord(unsigned int depth, unsigned int branch);
    GridCoord(const GridCoord& coord);
    virtual ~GridCoord();

    bool operator==(const GridCoord &gc) const;

    GridCoord leftOf();
    GridCoord rightOf();

    int position(GridCoord gc);

    unsigned int getDepth(){return mDepth;}
    unsigned int getBranch(){return mBranch;}

    char mXoverOrigin;

protected:
    unsigned int mDepth;
    unsigned int mBranch;
};
#endif

