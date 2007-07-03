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

#ifndef __INCLUDE_GRIDBRAIN_H
#define __INCLUDE_GRIDBRAIN_H

#include <stdio.h>

#include "Grid.h"
#include "GridbrainComponent.h"
#include "GridbrainConnection.h"
#include "GridbrainComponentSet.h"

#include <vector>

class Gridbrain
{
public:
	Gridbrain();
	virtual ~Gridbrain();

	Gridbrain* clone(bool full=true);

	void addGrid(Grid* grid);
	void init();

	void setComponent(unsigned int x,
				unsigned int y,
				unsigned int gridNumber,
				GridbrainComponent::Type type,
				float parameter);
	void addConnection(unsigned int xOrig,
				unsigned int yOrig,
				unsigned int gOrig,
				unsigned int xTarg,
				unsigned int yTarg,
				unsigned int gTarg,
				float weight);
	bool connectionExists(unsigned int xOrig,
				unsigned int yOrig,
				unsigned int gOrig,
				unsigned int xTarg,
				unsigned int yTarg,
				unsigned int gTarg);
	void selectRandomConnection(unsigned int &x1,
					unsigned int &y1,
					unsigned int &g1,
					unsigned int &x2,
					unsigned int &y2,
					unsigned int &g2);
	void addRandomConnection();
	void setMaxInputDepth(unsigned int maxInputDepth){mMaxInputDepth = maxInputDepth;}

	void eval();
	
	Grid* getGrid(unsigned int number);

	void initGridsInputOutput();

	unsigned int getConnectionsCount(){return mConnectionsCount;}

	void mutateAddConnection();
	void mutateRemoveConnection();
	void mutateChangeConnectionWeight();
	void mutateChangeComponent();

protected:
	void initGridInputOutput(Grid* grid);
	void calcConnectionCounts();

	std::vector<Grid*> mGridsVec;

        GridbrainComponent* mComponents;
	unsigned int mMaxInputDepth;
	unsigned int mNumberOfComponents;
	GridbrainConnection* mConnections;
	unsigned int mConnectionsCount;
	unsigned int mGridsCount;
	unsigned int mFirstBetaIndex;
	unsigned int mTotalPossibleConnections;
	unsigned int mBetaComponentsCount;
};

#endif

