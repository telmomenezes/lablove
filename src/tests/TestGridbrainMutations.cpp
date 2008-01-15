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
    mGridbrain.removeRandomConnection();
    count--;
    CHECK_EQUAL(mGridbrain.getConnectionsCount(), count);
}

TEST_FIXTURE(GridbrainMutFixture, GridbrainMutateAddConnection)
{
    mGridbrain.setMutateAddConnectionProb(0.01f);
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    mGridbrain.addRandomConnections(100);
    printf("connections before: %d\n", mGridbrain.getConnectionsCount());
    mGridbrain.mutate();
    CHECK(mGridbrain.isValid());
    printf("connections after: %d\n", mGridbrain.getConnectionsCount());
}

