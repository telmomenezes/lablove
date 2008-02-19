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
#include "Gridbrain.h"
#include "Grid.h"

#include <stdexcept>

TEST(CreateGridbrain)
{
    Gridbrain gridbrain;
    CHECK_EQUAL(gridbrain.getConnectionsCount(), 0);
    CHECK(gridbrain.isValid());
}

struct GridbrainFixture
{
    GridbrainFixture()
    {
        mGrid = new Grid();
        mGrid->init(Grid::BETA, 3, 3);
        mGridbrain.addGrid(mGrid, "test");
        mGridbrain.init();
    }

    ~GridbrainFixture(){}

    Gridbrain mGridbrain;
    Grid* mGrid;
};

struct GridbrainFixture2
{
    GridbrainFixture2()
    {
        mAlphaGrid = new Grid();
        mAlphaGrid->init(Grid::ALPHA, 3, 3);
        mGridbrain.addGrid(mAlphaGrid, "alpha");
        mBetaGrid = new Grid();
        mBetaGrid->init(Grid::BETA, 2, 2);
        mGridbrain.addGrid(mBetaGrid, "alpha");
        mGridbrain.init();
    }

    ~GridbrainFixture2(){}

    Gridbrain mGridbrain;
    Grid* mAlphaGrid;
    Grid* mBetaGrid;
};

TEST_FIXTURE(GridbrainFixture, GridbrainAddOneGrid)
{
    Grid* gridOut = mGridbrain.getGrid(0);
    CHECK(gridOut != NULL);
    gridOut = mGridbrain.getGrid(1);
    CHECK(gridOut == NULL);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainZeroInitialConnections)
{
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    CHECK(!mGridbrain.connectionExists(0, 0, 0, 1, 1, 0));
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddConnection)
{
    mGridbrain.addConnection(0, 0, 0, 1, 1, 0, 0.5);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 1);
    CHECK(mGridbrain.connectionExists(0, 0, 0, 1, 1, 0));
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddRandomConnections1)
{
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(1);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 1);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddRandomConnections2)
{
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(27);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 27);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddRandomConnections3)
{
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(28);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 27);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddRandomConnections4)
{
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(20);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 20);
    mGridbrain.addRandomConnections(20);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 27);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture2, GridbrainAddRandomConnections5)
{
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(20);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 20);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture2, GridbrainAddRandomConnections6)
{
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(100);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 67);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddRandomConnections7)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(1, 1, 0);
    comp1->mType = GridbrainComponent::ACT;
    mGridbrain.init();
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(27);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 24);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddRandomConnections8)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(1, 1, 0);
    comp1->mType = GridbrainComponent::PER;
    mGridbrain.init();
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(27);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 24);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainAddRandomConnections9)
{
    GridbrainComponent* comp1 = mGridbrain.getComponent(1, 1, 0);
    GridbrainComponent* comp2 = mGridbrain.getComponent(1, 2, 0);
    comp1->mType = GridbrainComponent::PER;
    comp2->mType = GridbrainComponent::ACT;
    mGridbrain.init();
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 0);
    mGridbrain.addRandomConnections(27);
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), 21);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainGetComponent)
{
    GridbrainComponent* comp = mGridbrain.getComponent(0, 0, 0);
    CHECK_EQUAL(comp->mType, GridbrainComponent::NUL);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainGetComponentFromInvalidGrid)
{
    CHECK_THROW(mGridbrain.getComponent(0, 0, 99999), std::runtime_error);
}

TEST_FIXTURE(GridbrainFixture, GridbrainGetComponentFromInvalidXPosition)
{
    CHECK_THROW(mGridbrain.getComponent(99999, 0, 0), std::runtime_error);
}

TEST_FIXTURE(GridbrainFixture, GridbrainGetComponentFromInvalidYPosition)
{
    CHECK_THROW(mGridbrain.getComponent(0, 99999, 0), std::runtime_error);
}

TEST_FIXTURE(GridbrainFixture, GridbrainCycleOnEmptyGrid)
{
    mGridbrain.cycle();
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainConnectionAge1)
{
    mGridbrain.addConnection(0, 0, 0, 1, 1, 0, 0.5);
    GridbrainConnection* conn = mGridbrain.getConnection(0, 0, 0, 1, 1, 0);
    CHECK_CLOSE(conn->mAge, 0.0f, 0.0001f);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainConnectionAge2)
{
    mGridbrain.addConnection(0, 0, 0, 1, 1, 0, 0.5);
    Gridbrain* gb = (Gridbrain*)mGridbrain.clone();
    GridbrainConnection* conn = gb->getConnection(0, 0, 0, 1, 1, 0);
    CHECK_CLOSE(conn->mAge, 1.0f, 0.0001f);
    CHECK(gb->isValid());
    delete gb;
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainConnectionAge3)
{
    mGridbrain.addConnection(0, 0, 0, 1, 1, 0, 0.5);
    Gridbrain* gb = (Gridbrain*)mGridbrain.clone();
    Gridbrain* gb2 = (Gridbrain*)gb->clone();
    GridbrainConnection* conn = gb2->getConnection(0, 0, 0, 1, 1, 0);
    CHECK_CLOSE(conn->mAge, 2.0f, 0.0001f);
    CHECK(gb->isValid());
    CHECK(gb2->isValid());
    delete gb;
    delete gb2;
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainConnectionAge4)
{
    mGridbrain.addConnection(0, 0, 0, 1, 1, 0, 0.5);
    Gridbrain* gb = (Gridbrain*)mGridbrain.clone();
    Gridbrain* gb2 = (Gridbrain*)gb->clone();
    gb2->removeConnection(0, 0, 0, 1, 1, 0);
    gb2->addConnection(0, 0, 0, 1, 1, 0, 0.5);
    GridbrainConnection* conn = gb2->getConnection(0, 0, 0, 1, 1, 0);
    CHECK_CLOSE(conn->mAge, 0.0f, 0.0001f);
    CHECK(gb->isValid());
    CHECK(gb2->isValid());
    delete gb;
    delete gb2;
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainFixture, GridbrainConnectionAge5)
{
    mGridbrain.addConnection(0, 0, 0, 1, 0, 0, 0.5);
    mGridbrain.addConnection(0, 0, 0, 2, 0, 0, 0.5);
    Gridbrain* gb = (Gridbrain*)mGridbrain.clone();
    gb->addConnection(0, 0, 0, 1, 1, 0, 0.5);
    Gridbrain* gb2 = (Gridbrain*)gb->clone();
    gb2->addConnection(0, 0, 0, 2, 1, 0, 0.5);
    gb2->removeConnection(0, 0, 0, 1, 0, 0);
    gb2->addConnection(0, 0, 0, 1, 0, 0, 0.5);
    GridbrainConnection* conn1 = gb2->getConnection(0, 0, 0, 1, 0, 0);
    GridbrainConnection* conn2 = gb2->getConnection(0, 0, 0, 2, 0, 0);
    GridbrainConnection* conn3 = gb2->getConnection(0, 0, 0, 1, 1, 0);
    GridbrainConnection* conn4 = gb2->getConnection(0, 0, 0, 2, 1, 0);
    CHECK_CLOSE(conn1->mAge, 0.0f, 0.0001f);
    CHECK_CLOSE(conn2->mAge, 2.0f, 0.0001f);
    CHECK_CLOSE(conn3->mAge, 1.0f, 0.0001f);
    CHECK_CLOSE(conn4->mAge, 0.0f, 0.0001f);
    CHECK(gb->isValid());
    CHECK(gb2->isValid());
    delete gb;
    delete gb2;
    CHECK(mGridbrain.isValid());
}

