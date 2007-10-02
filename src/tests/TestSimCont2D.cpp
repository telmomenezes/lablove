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
#include "functions.h"

#include <stdexcept>

struct SimCont2DFixture
{
    SimCont2DFixture()
    {
        srand((long int)(mSim.getRealTime()));
        mWidth = 1000;
        mLength = 500;
        mCell = (rand() % 300) + 1;
        mSim.setWorldDimensions(mWidth, mLength, mCell);
    }

    ~SimCont2DFixture(){}

    SimCont2D mSim;
    unsigned int mWidth;
    unsigned int mLength;
    unsigned int mCell;
};

TEST_FIXTURE(SimCont2DFixture, SimCont2DSetDimensions)
{
    CHECK_EQUAL(mSim.getWorldWidth(), mWidth);
    CHECK_EQUAL(mSim.getWorldLength(), mLength);
    CHECK_EQUAL(mSim.getCellSide(), mCell);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DPlaceObject)
{
    SimulationObject* dummy = new SimulationObject();
    dummy->setSize(50.0f);
    mSim.addObject(dummy);
    mSim.setPos(dummy, 400.0f, 400.0f);
    list<SimulationObject*>** grid = mSim.getCellGrid();
    int gridX1 = 350 / mCell;
    int gridX2 = 450 / mCell;
    int gridY1 = 350 / mCell;
    int gridY2 = 450 / mCell;

    for (int x = 0; x < mSim.getWorldCellWidth(); x++)
    {
        for (int y = 0; y < mSim.getWorldCellLength(); y++)
        {
            list<SimulationObject*>* l = grid[(y * mSim.getWorldCellWidth()) + x];
            int count = 0;
            if ((x >= gridX1) && (x <= gridX2) && (y >= gridY1) && (y <= gridY2))
            {
                count = 1;
            }
            CHECK_EQUAL(l->size(), count);
        }
    }
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DRemoveObject)
{
    SimulationObject* dummy = new SimulationObject();
    dummy->setSize(10.0f);
    mSim.addObject(dummy);
    mSim.setPos(dummy, 100.0f, 100.0f);
    mSim.removeObject(dummy);
    list<SimulationObject*>** grid = mSim.getCellGrid();

    for (int x = 0; x < mSim.getWorldCellWidth(); x++)
    {
        for (int y = 0; y < mSim.getWorldCellLength(); y++)
        {
            list<SimulationObject*>* l = grid[(y * mSim.getWorldCellWidth()) + x];
            CHECK_EQUAL(l->size(), 0);
        }
    }
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
    mSim.startCollisionDetection(100, 100, 100);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
    CHECK_EQUAL(distance, 0);
    CHECK_EQUAL(angle, 0);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DCollisionDetection2)
{
    SimulationObject* dummy = new SimulationObject();
    mSim.addObject(dummy);
    mSim.setPos(dummy, 100.0f, 100.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_EQUAL(distance, 0);
    CHECK_EQUAL(angle, 0.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DCollisionDetection3)
{
    SimulationObject* dummy1 = new SimulationObject();
    SimulationObject* dummy2 = new SimulationObject();
    SimulationObject* dummy3 = new SimulationObject();
    SimulationObject* dummy4 = new SimulationObject();
    mSim.addObject(dummy1);
    mSim.addObject(dummy2);
    mSim.addObject(dummy3);
    mSim.addObject(dummy4);
    mSim.setPos(dummy1, 110.0f, 100.0f);
    mSim.setPos(dummy2, 100.0f, 110.0f);
    mSim.setPos(dummy3, 90.0f, 100.0f);
    mSim.setPos(dummy4, 100.0f, 90.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_EQUAL(distance, 9.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_EQUAL(distance, 9.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_EQUAL(distance, 9.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_EQUAL(distance, 9.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DCollisionDetection4)
{
    SimulationObject* dummy1 = new SimulationObject();
    mSim.addObject(dummy1);
    mSim.setPos(dummy1, 110.0f, 100.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, 0.0f, 0.001f);
    CHECK_EQUAL(distance, 9.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DCollisionDetection5)
{
    SimulationObject* dummy1 = new SimulationObject();
    mSim.addObject(dummy1);
    mSim.setPos(dummy1, 100.0f, 110.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, M_PI / 2.0f, 0.001f);
    CHECK_EQUAL(distance, 9.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DCollisionDetection6)
{
    SimulationObject* dummy1 = new SimulationObject();
    mSim.addObject(dummy1);
    mSim.setPos(dummy1, 90.0f, 100.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, M_PI, 0.001f);
    CHECK_EQUAL(distance, 9.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DCollisionDetection7)
{
    SimulationObject* dummy1 = new SimulationObject();
    mSim.addObject(dummy1);
    mSim.setPos(dummy1, 100.0f, 90.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, -M_PI / 2.0f, 0.001f);
    CHECK_EQUAL(distance, 9.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DCollisionDetection8)
{
    SimulationObject* dummy1 = new SimulationObject();
    mSim.addObject(dummy1);
    mSim.setPos(dummy1, 110.0f, 110.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, M_PI / 4.0f, 0.001f);
    CHECK_CLOSE(distance, sqrt((10 * 10 + 10 * 10)) - 1.0f, 0.001f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DCollisionDetection9)
{
    SimulationObject* dummy1 = new SimulationObject();
    mSim.addObject(dummy1);
    mSim.setPos(dummy1, 110.0f, 90.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, -M_PI / 4.0f, 0.001f);
    CHECK_CLOSE(distance, sqrt((10 * 10 + 10 * 10)) - 1.0f, 0.001f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DCollisionDetection10)
{
    SimulationObject* dummy1 = new SimulationObject();
    mSim.addObject(dummy1);
    mSim.setPos(dummy1, 90.0f, 110.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, (3 * M_PI) / 4.0f, 0.001f);
    CHECK_CLOSE(distance, sqrt((10 * 10 + 10 * 10)) - 1.0f, 0.001f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DCollisionDetection11)
{
    SimulationObject* dummy1 = new SimulationObject();
    mSim.addObject(dummy1);
    mSim.setPos(dummy1, 90.0f, 90.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, -(3 * M_PI) / 4.0f, 0.001f);
    CHECK_CLOSE(distance, sqrt((10 * 10 + 10 * 10)) - 1.0f, 0.001f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DCollisionDetection12)
{
    SimulationObject* dummy1 = new SimulationObject();
    mSim.addObject(dummy1);
    mSim.setPos(dummy1, 50.0f, 50.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, -(3 * M_PI) / 4.0f, 0.001f);
    CHECK_CLOSE(distance, sqrt((50 * 50 + 50 * 50)) - 1.0f, 0.001f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DCollisionDetection13)
{
    SimulationObject* dummy1 = new SimulationObject();
    dummy1->setSize(10.0f);
    mSim.addObject(dummy1);
    mSim.setPos(dummy1, 50.0f, 150.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, (3 * M_PI) / 4.0f, 0.001f);
    CHECK_CLOSE(distance, sqrt((50 * 50 + 50 * 50)) - 10.0f, 0.001f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DCollisionDetection14)
{
    SimulationObject* dummy1 = new SimulationObject();
    mSim.addObject(dummy1);
    mSim.setPos(dummy1, 300.0f, 300.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(SimCont2DFixture, SimCont2DCollisionDetection15)
{
    SimulationObject* dummy1 = new SimulationObject();
    SimulationObject* dummy2 = new SimulationObject();
    mSim.addObject(dummy1);
    mSim.addObject(dummy2);
    mSim.setPos(dummy1, 90.0f, 100.0f);
    mSim.setPos(dummy2, 300.0f, 300.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimulationObject* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, M_PI, 0.001f);
    CHECK_EQUAL(distance, 9.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

