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

#include "UnitTest++.h"
#include "Sim2D.h"

#include <stdexcept>

struct Sim2DFixture
{
    Sim2DFixture()
    {
        srand((long int)(art_getTime()));
        mWidth = 1000;
        mLength = 500;
        mCell = (rand() % 300) + 1;
        mSim.setWorldDimensions(mWidth, mLength, mCell);
    }

    ~Sim2DFixture(){}

    Sim2D mSim;
    unsigned int mWidth;
    unsigned int mLength;
    unsigned int mCell;
};

TEST_FIXTURE(Sim2DFixture, Sim2DSetDimensions)
{
    CHECK_EQUAL(mSim.getWorldWidth(), mWidth);
    CHECK_EQUAL(mSim.getWorldLength(), mLength);
    CHECK_EQUAL(mSim.getCellSide(), mCell);
}

TEST_FIXTURE(Sim2DFixture, Sim2DPlaceObject)
{
    SimObj2D* dummy = new SimObj2D();
    mSim.addObject(dummy);
    dummy->setSize(50.0f);
    dummy->setPos(400.0f, 400.0f);
    list<SimObj2D*>** grid = mSim.getCellGrid();
    int gridX1 = 350 / mCell;
    int gridX2 = 450 / mCell;
    int gridY1 = 350 / mCell;
    int gridY2 = 450 / mCell;

    for (int x = 0; x < mSim.getWorldCellWidth(); x++)
    {
        for (int y = 0; y < mSim.getWorldCellLength(); y++)
        {
            list<SimObj2D*>* l = grid[(y * mSim.getWorldCellWidth()) + x];
            int count = 0;
            if ((x >= gridX1) && (x <= gridX2) && (y >= gridY1) && (y <= gridY2))
            {
                count = 1;
            }
            CHECK_EQUAL(l->size(), count);
        }
    }
}

TEST_FIXTURE(Sim2DFixture, Sim2DRemoveObject)
{
    SimObj2D* dummy = new SimObj2D();
    mSim.addObject(dummy);
    dummy->setSize(10.0f);
    dummy->setPos(100.0f, 100.0f);
    mSim.removeObject(dummy);
    list<SimObj2D*>** grid = mSim.getCellGrid();

    for (int x = 0; x < mSim.getWorldCellWidth(); x++)
    {
        for (int y = 0; y < mSim.getWorldCellLength(); y++)
        {
            list<SimObj2D*>* l = grid[(y * mSim.getWorldCellWidth()) + x];
            CHECK_EQUAL(l->size(), 0);
        }
    }
}

TEST_FIXTURE(Sim2DFixture, Sim2DEmptyCollisionDetection)
{
    mSim.startCollisionDetection(100, 100, 100);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
    CHECK_EQUAL(distance, 0);
    CHECK_EQUAL(angle, 0);
}

TEST_FIXTURE(Sim2DFixture, Sim2DCollisionDetection1)
{
    mSim.startCollisionDetection(100, 100, 100);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
    CHECK_EQUAL(distance, 0);
    CHECK_EQUAL(angle, 0);
}

TEST_FIXTURE(Sim2DFixture, Sim2DCollisionDetection2)
{
    SimObj2D* dummy = new SimObj2D();
    mSim.addObject(dummy);
    dummy->setPos(100.0f, 100.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_EQUAL(distance, 0);
    CHECK_EQUAL(angle, 0.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(Sim2DFixture, Sim2DCollisionDetection3)
{
    SimObj2D* dummy1 = new SimObj2D();
    SimObj2D* dummy2 = new SimObj2D();
    SimObj2D* dummy3 = new SimObj2D();
    SimObj2D* dummy4 = new SimObj2D();
    mSim.addObject(dummy1);
    mSim.addObject(dummy2);
    mSim.addObject(dummy3);
    mSim.addObject(dummy4);
    dummy1->setPos(110.0f, 100.0f);
    dummy2->setPos(100.0f, 110.0f);
    dummy3->setPos(90.0f, 100.0f);
    dummy4->setPos(100.0f, 90.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_EQUAL(distance, 10.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_EQUAL(distance, 10.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_EQUAL(distance, 10.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_EQUAL(distance, 10.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(Sim2DFixture, Sim2DCollisionDetection4)
{
    SimObj2D* dummy1 = new SimObj2D();
    mSim.addObject(dummy1);
    dummy1->setPos(110.0f, 100.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, 0.0f, 0.001f);
    CHECK_EQUAL(distance, 10.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(Sim2DFixture, Sim2DCollisionDetection5)
{
    SimObj2D* dummy1 = new SimObj2D();
    mSim.addObject(dummy1);
    dummy1->setPos(100.0f, 110.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, M_PI / 2.0f, 0.001f);
    CHECK_EQUAL(distance, 10.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(Sim2DFixture, Sim2DCollisionDetection6)
{
    SimObj2D* dummy1 = new SimObj2D();
    mSim.addObject(dummy1);
    dummy1->setPos(90.0f, 100.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, M_PI, 0.001f);
    CHECK_EQUAL(distance, 10.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(Sim2DFixture, Sim2DCollisionDetection7)
{
    SimObj2D* dummy1 = new SimObj2D();
    mSim.addObject(dummy1);
    dummy1->setPos(100.0f, 90.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, -M_PI / 2.0f, 0.001f);
    CHECK_EQUAL(distance, 10.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(Sim2DFixture, Sim2DCollisionDetection8)
{
    SimObj2D* dummy1 = new SimObj2D();
    mSim.addObject(dummy1);
    dummy1->setPos(110.0f, 110.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, M_PI / 4.0f, 0.001f);
    CHECK_CLOSE(distance, sqrt(10 * 10 + 10 * 10), 0.001f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(Sim2DFixture, Sim2DCollisionDetection9)
{
    SimObj2D* dummy1 = new SimObj2D();
    mSim.addObject(dummy1);
    dummy1->setPos(110.0f, 90.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, -M_PI / 4.0f, 0.001f);
    CHECK_CLOSE(distance, sqrt(10 * 10 + 10 * 10), 0.001f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(Sim2DFixture, Sim2DCollisionDetection10)
{
    SimObj2D* dummy1 = new SimObj2D();
    mSim.addObject(dummy1);
    dummy1->setPos(90.0f, 110.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, (3 * M_PI) / 4.0f, 0.001f);
    CHECK_CLOSE(distance, sqrt(10 * 10 + 10 * 10), 0.001f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(Sim2DFixture, Sim2DCollisionDetection11)
{
    SimObj2D* dummy1 = new SimObj2D();
    mSim.addObject(dummy1);
    dummy1->setPos(90.0f, 90.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, -(3 * M_PI) / 4.0f, 0.001f);
    CHECK_CLOSE(distance, sqrt(10 * 10 + 10 * 10), 0.001f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(Sim2DFixture, Sim2DCollisionDetection12)
{
    SimObj2D* dummy1 = new SimObj2D();
    mSim.addObject(dummy1);
    dummy1->setPos(50.0f, 50.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, -(3 * M_PI) / 4.0f, 0.001f);
    CHECK_CLOSE(distance, sqrt(50 * 50 + 50 * 50), 0.001f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(Sim2DFixture, Sim2DCollisionDetection13)
{
    SimObj2D* dummy1 = new SimObj2D();
    mSim.addObject(dummy1);
    dummy1->setSize(10.0f);
    dummy1->setPos(50.0f, 150.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, (3 * M_PI) / 4.0f, 0.001f);
    CHECK_CLOSE(distance, sqrt(50 * 50 + 50 * 50), 0.001f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(Sim2DFixture, Sim2DCollisionDetection14)
{
    SimObj2D* dummy1 = new SimObj2D();
    mSim.addObject(dummy1);
    dummy1->setPos(300.0f, 300.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

TEST_FIXTURE(Sim2DFixture, Sim2DCollisionDetection15)
{
    SimObj2D* dummy1 = new SimObj2D();
    SimObj2D* dummy2 = new SimObj2D();
    mSim.addObject(dummy1);
    mSim.addObject(dummy2);
    dummy1->setPos(90.0f, 100.0f);
    dummy2->setPos(300.0f, 300.0f);
    mSim.startCollisionDetection(100.0f, 100.0f, 100.0f);
    float distance;
    float angle;
    SimObj2D* obj = mSim.nextCollision(distance, angle);
    CHECK(obj != NULL);
    CHECK_CLOSE(angle, M_PI, 0.001f);
    CHECK_EQUAL(distance, 10.0f);
    obj = mSim.nextCollision(distance, angle);
    CHECK(obj == NULL);
}

