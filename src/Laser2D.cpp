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

#include "Laser2D.h"

Laser2D::Laser2D()
{
    mX1 = 0;
    mY1 = 0;
    mX2 = 0;
    mY2 = 0;
    mAng = 0;
    mLength = 0;
    mSpeed = 0;
    mM = 0;
    mB = 0;
    mDirX = 0;
    mDirY = 0;
    mOwnerID = 0;
    mType = LASER_ONE_HIT;
    mEnergy = 0;
    mRange = 0;
    mDistanceTraveled = 0;
    mFireTime = 0;
}

Laser2D::~Laser2D()
{   
}

Laser2D::Laser2D(const Laser2D& laser)
{
    mX1 = laser.mX1;
    mY1 = laser.mY1;
    mX2 = laser.mX2;
    mY2 = laser.mY2;
    mAng = laser.mAng;
    mLength = laser.mLength;
    mSpeed = laser.mSpeed;
    mM = laser.mM;
    mB = laser.mB;
    mDirX = laser.mDirX;
    mDirY = laser.mDirY;
    mOwnerID = laser.mOwnerID;
    mType = laser.mType;
    mEnergy = laser.mEnergy;
    mRange = laser.mRange;
    mDistanceTraveled = laser.mDistanceTraveled;
    mFireTime = laser.mFireTime;
}

