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

struct GridbrainMutFixture
{
    GridbrainMutFixture()
    {
        mAlphaGrid = new Grid();
        mBetaGrid = new Grid();
        mAlphaGrid->init(Grid::ALPHA, 10, 10);
        mBetaGrid->init(Grid::BETA, 10, 10);
        mGridbrain.addGrid(mAlphaGrid, "alpha");
        mGridbrain.addGrid(mBetaGrid, "beta");
        mGridbrain.init();
    }

    ~GridbrainMutFixture(){}

    Gridbrain mGridbrain;
    Grid* mAlphaGrid;
    Grid* mBetaGrid;
};

TEST_FIXTURE(GridbrainMutFixture, GridbrainNoMutations)
{
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainRemoveRandomConn)
{
    mGridbrain.addRandomConnections(100);
    unsigned int count = mGridbrain.getConnectionsCount();
    mGridbrain.removeRandomConnection(count);
    count--;
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), count);
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainMutateAddConnection)
{
    mGridbrain.setMutateAddConnectionProb(0.1f);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    unsigned int conn1 = mGridbrain.getConnectionsCount();
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    unsigned int conn2 = mGridbrain.getConnectionsCount();
    CHECK(conn1 <= conn2);
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainMutateAddDoubleConnection)
{
    mGridbrain.setMutateAddDoubleConnectionProb(0.1f);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    unsigned int conn1 = mGridbrain.getConnectionsCount();
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    unsigned int conn2 = mGridbrain.getConnectionsCount();
    CHECK(conn1 <= conn2);
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainMutateRemoveConnection)
{
    mGridbrain.setMutateRemoveConnectionProb(0.1f);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    unsigned int conn1 = mGridbrain.getConnectionsCount();
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    unsigned int conn2 = mGridbrain.getConnectionsCount();
    CHECK(conn1 >= conn2);
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainMutateRemoveDoubleConnection1)
{
    mGridbrain.setMutateRemoveDoubleConnectionProb(0.1f);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    unsigned int conn1 = mGridbrain.getConnectionsCount();
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    unsigned int conn2 = mGridbrain.getConnectionsCount();
    CHECK(conn1 >= conn2);
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainMutateRemoveDoubleConnection2)
{
    mGridbrain.setMutateAddDoubleConnectionProb(0.1f);
    mGridbrain.setMutateRemoveDoubleConnectionProb(0.1f);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainMutateChangeConnection)
{
    mGridbrain.setMutateChangeConnectionWeightProb(0.1f);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    unsigned int conn1 = mGridbrain.getConnectionsCount();
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    unsigned int conn2 = mGridbrain.getConnectionsCount();
    CHECK(conn1 == conn2);
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainMutateMoveConnectionOrigin)
{
    mGridbrain.setMutateMoveConnectionOriginProb(0.1f);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    unsigned int conn1 = mGridbrain.getConnectionsCount();
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    unsigned int conn2 = mGridbrain.getConnectionsCount();
    CHECK(conn1 == conn2);
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainMutateForkConnection)
{
    mGridbrain.setMutateForkConnectionProb(0.1f);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    unsigned int conn1 = mGridbrain.getConnectionsCount();
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    unsigned int conn2 = mGridbrain.getConnectionsCount();
    CHECK(conn1 <= conn2);
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainMutateForkDblConnection)
{
    mGridbrain.setMutateForkDoubleConnectionProb(0.1f);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    unsigned int conn1 = mGridbrain.getConnectionsCount();
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    unsigned int conn2 = mGridbrain.getConnectionsCount();
    CHECK(conn1 <= conn2);
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainMutateSplitConnection)
{
    mGridbrain.setMutateSplitConnectionProb(0.1f);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    unsigned int conn1 = mGridbrain.getConnectionsCount();
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    unsigned int conn2 = mGridbrain.getConnectionsCount();
    CHECK(conn1 <= conn2);
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainMutateJoinConnections)
{
    mGridbrain.setMutateJoinConnectionsProb(0.1f);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    unsigned int conn1 = mGridbrain.getConnectionsCount();
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    unsigned int conn2 = mGridbrain.getConnectionsCount();
    CHECK(conn1 >= conn2);
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainMutateSplitAndJoin)
{
    mGridbrain.setMutateSplitConnectionProb(0.1f);
    mGridbrain.setMutateJoinConnectionsProb(0.1f);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainMutateChangeComponent)
{
    mGridbrain.setMutateChangeComponentProb(0.1f);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    unsigned int conn1 = mGridbrain.getConnectionsCount();
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    unsigned int conn2 = mGridbrain.getConnectionsCount();
    CHECK(conn1 == conn2);
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainMutateSwapComponent)
{
    mGridbrain.setMutateSwapComponentProb(0.1f);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    unsigned int conn1 = mGridbrain.getConnectionsCount();
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    unsigned int conn2 = mGridbrain.getConnectionsCount();
    CHECK(conn1 == conn2);
}

