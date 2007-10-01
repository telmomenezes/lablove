/*
 * LOVE Lab
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

#include "UnitTest++.h"
#include "SimCont2D.h"

#include <stdexcept>

struct SimCont2DFixture
{
    SimCont2DFixture()
    {
        mSim.setWorldDimensions(1000, 500, 50);
    }

    ~SimCont2DFixture(){}

    SimCont2D mSim;
};

TEST_FIXTURE(SimCont2DFixture, SimCont2DSetDimensions)
{
    CHECK_EQUAL(mSim.getWorldWidth(), 1000);
    CHECK_EQUAL(mSim.getWorldLength(), 500);
    CHECK_EQUAL(mSim.getCellSide(), 50);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DEmptyCollisionDetection)
{
    mSim.startCollisionDetection(100, 100, 100);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
    CHECK_EQUAL(distance, 0);
    CHECK_EQUAL(angle, 0);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DCollisionDetection1)
{
    Agent agent;
    mSim.startCollisionDetection(100, 100, 100);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
    CHECK_EQUAL(distance, 0);
    CHECK_EQUAL(angle, 0);
}

