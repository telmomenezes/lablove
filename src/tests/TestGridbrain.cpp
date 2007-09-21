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

TEST(CreateGridbrain)
{
    Gridbrain gridbrain;
    CHECK_EQUAL(gridbrain.getConnectionsCount(), 0);
}

TEST(CheckAddOneGrid)
{
    Gridbrain gridbrain;
    Grid* grid = new Grid();
    grid->init(Grid::BETA, 2, 2);
    gridbrain.addGrid(grid);
    Grid* gridOut = gridbrain.getGrid(0);
    CHECK(gridOut != NULL);
    gridOut = gridbrain.getGrid(1);
    CHECK(gridOut == NULL);
}

