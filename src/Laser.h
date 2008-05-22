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

#if !defined(__INCLUDE_LASER_H)
#define __INCLUDE_LASER_H

#include "types.h"

class Laser
{
public:
    enum LaserType {LASER_ONE_HIT, LASER_COMULATIVE};

    Laser();
    virtual ~Laser();
    Laser(const Laser& laser);

    float mX1;
    float mY1;
    float mX2;
    float mY2;
    float mAng;
    float mLength;
    float mSpeed;
    float mM;
    float mB;
    int mDirX;
    int mDirY;
    llULINT mOwnerID;
    LaserType mType;
    float mEnergy;
    float mRange;
    float mDistanceTraveled;
    llULINT mFireTime;
};
#endif

