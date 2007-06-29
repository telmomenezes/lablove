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

#include "Gridbrain.h"
#include "functions.h"
#include "random.h"

#include <stdlib.h>
#include <string.h>

Gridbrain::Gridbrain()
{
	mMaxInputDepth = 50;
        mComponents = NULL;
	mNumberOfComponents = 0;
	mConnections = NULL;
	mConnectionsCount = 0;
	mFirstBetaIndex = 0;
	mTotalPossibleConnections = 0;
	mBetaComponentsCount = 0;
	mGridsCount = 0;
}

Gridbrain::~Gridbrain()
{
	while (mConnections != NULL)
	{
		GridbrainConnection* conn = mConnections;
		mConnections = (GridbrainConnection*)conn->mNextGlobalConnection;
		free(conn);
	}
	if (mComponents != NULL)
	{
        	free(mComponents);
		mComponents = NULL;
	}
	for (unsigned int i = 0; i < mGridsCount; i++)
	{
		if (!mGridsVec[i]->mCreatedByScript)
		{
			delete mGridsVec[i];
		}
	}
	mGridsCount = 0;
}

Gridbrain* Gridbrain::clone(bool full)
{
	Gridbrain* gb = new Gridbrain();

	gb->mMaxInputDepth = mMaxInputDepth;
	gb->mNumberOfComponents = mNumberOfComponents;
	gb->mConnections = NULL;
	gb->mConnectionsCount = 0;
	gb->mGridsCount = mGridsCount;
	gb->mFirstBetaIndex = mFirstBetaIndex;
	gb->mTotalPossibleConnections = mTotalPossibleConnections;
	gb->mBetaComponentsCount = mBetaComponentsCount;

	for (unsigned int i = 0; i < mGridsCount; i++)
	{
		Grid* grid = mGridsVec[i];
		gb->mGridsVec.push_back(new Grid(grid));
	}

	if (full)
	{
		gb->mComponents = (GridbrainComponent*)malloc(mNumberOfComponents * sizeof(GridbrainComponent));

		unsigned int index = 0;

		for (unsigned int index = 0; index < mNumberOfComponents; index++)
		{
			gb->mComponents[index].copy(&mComponents[index]);
		}

		for (unsigned int index = 0; index < mNumberOfComponents; index++)
		{
			GridbrainConnection* conn = mComponents[index].mFirstConnection;
			while (conn != NULL)
			{
				Grid* gridOrig = mGridsVec[conn->mGridOrig];
				Grid* gridTarg = mGridsVec[conn->mGridTarg];
				gb->addConnection(gridOrig->getXByCode(conn->mColumnOrig),
					gridOrig->getYByCode(conn->mRowOrig),
					conn->mGridOrig,
					gridTarg->getXByCode(conn->mColumnTarg),
					gridTarg->getYByCode(conn->mRowTarg),
					conn->mGridTarg,
					conn->mWeight);
				conn = (GridbrainConnection*)conn->mNextConnection;
			}
		}

		gb->initGridsInputOutput();
	}

	return gb;
}

void Gridbrain::addGrid(Grid* grid)
{
	grid->setNumber(mGridsVec.size());
	grid->setOffset(mNumberOfComponents);
	unsigned int gridComponentCount = grid->getHeight() * grid->getWidth();
	mNumberOfComponents += gridComponentCount;

	if (grid->getType() == Grid::BETA)
	{
		mFirstBetaIndex = mNumberOfComponents;
		mBetaComponentsCount = gridComponentCount;
	}

	grid->init();
	mGridsVec.push_back(grid);
}

void Gridbrain::init()
{
	mGridsCount = mGridsVec.size();

	calcConnectionCounts();
	
        mComponents = (GridbrainComponent*)malloc(mNumberOfComponents * sizeof(GridbrainComponent));

	// Init grids with random components

	unsigned int pos = 0;

	for (unsigned int i = 0; i < mGridsCount; i++)
	{
		Grid* grid = mGridsVec[i];

		for (unsigned int x = 0;
			x < grid->getWidth();
			x++)
		{
			for (unsigned int y = 0;
				y < grid->getHeight();
				y++)
			{
				GridbrainComponent* comp = grid->getRandomComponent(pos);
				mComponents[pos].copy(comp);

				mComponents[pos].mOffset = pos;
				mComponents[pos].mColumn = grid->getColumnCode(x);
				mComponents[pos].mRow = grid->getRowCode(y);
				mComponents[pos].mGrid = i;

				pos++;
			}
		}
	}

	initGridsInputOutput();
}

void Gridbrain::initGridsInputOutput()
{
	for (unsigned int i = 0; i < mGridsCount; i++)
	{
		Grid* grid = mGridsVec[i];
		initGridInputOutput(grid);		
	}
}

void Gridbrain::initGridInputOutput(Grid* grid)
{
	grid->removeInputOutput();
	unsigned int pos = grid->getOffset();

	for (unsigned int j = 0;
		j < grid->getSize();
		j++)
	{
		if (grid->getType() == Grid::ALPHA)
		{
			if (mComponents[pos].mType == GridbrainComponent::PER)
			{
				mComponents[pos].mPerceptionPosition = grid->addPerception(&mComponents[pos]);
			}
		}
		else
		{
			if (mComponents[pos].mType == GridbrainComponent::ACT)
			{
				mComponents[pos].mActionPosition = grid->addAction(&mComponents[pos]);
			}
		}
		pos++;
	}

	if (grid->getType() == Grid::ALPHA)
	{
		grid->initInputMatrix(mMaxInputDepth);
	}
	else
	{
		grid->initOutputVector();
	}
}

void Gridbrain::setComponent(unsigned int x,
				unsigned int y,
				unsigned int gridNumber,
				GridbrainComponent::Type type,
				float parameter)
{
	Grid* grid = mGridsVec[gridNumber];

	unsigned int pos = (x * grid->getHeight()) + y + grid->getOffset();

	mComponents[pos].clear();
	mComponents[pos].mType = type;
	mComponents[pos].mParameter = parameter;
	mComponents[pos].mColumn = grid->getColumnCode(x);
	mComponents[pos].mRow = grid->getRowCode(y);
	mComponents[pos].mAggregator = mComponents[pos].isAggregator();
}

void Gridbrain::addConnection(unsigned int xOrig,
				unsigned int yOrig,
				unsigned int gOrig,
				unsigned int xTarg,
				unsigned int yTarg,
				unsigned int gTarg,
				float weight)
{
	// TODO: disallow invalid connections?
	if (connectionExists(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg))
	{
		return;
	}

	Grid* grid = mGridsVec[gOrig];
	unsigned int orig = (xOrig * grid->getHeight()) + yOrig + grid->getOffset();
	grid = mGridsVec[gTarg];
	unsigned int target = (xTarg * grid->getHeight()) + yTarg + grid->getOffset();

	GridbrainComponent* comp = &(mComponents[orig]);
	GridbrainComponent* targComp = &(mComponents[target]);
	GridbrainConnection* conn = (GridbrainConnection*)malloc(sizeof(GridbrainConnection));
	conn->mColumnOrig = comp->mColumn;
	conn->mRowOrig = comp->mRow;
	conn->mGridOrig = comp->mGrid;
	conn->mColumnTarg = targComp->mColumn;
	conn->mRowTarg = targComp->mRow;
	conn->mGridTarg = targComp->mGrid;
	conn->mTarget = target;
	conn->mWeight = weight;
	conn->mOrigComponent = comp;
	conn->mFeedForward = true;

	if (conn->mGridOrig == conn->mGridTarg)
	{
		conn->mInterGrid = false;

		if (xOrig >= xTarg)
		{
			conn->mFeedForward = false;
		}
	}
	else
	{
		conn->mInterGrid = true;
	}

	if (comp->mConnectionsCount == 0)
	{
		comp->mFirstConnection = conn;
		conn->mPrevConnection = NULL;
		conn->mNextConnection = NULL;
	}
	else
	{
		GridbrainConnection* nextConn = comp->mFirstConnection;
		conn->mPrevConnection = NULL;
		conn->mNextConnection = nextConn;
		comp->mFirstConnection = conn;
		nextConn->mPrevConnection = conn;
	}

	(comp->mConnectionsCount)++;

	GridbrainConnection* nextConn = mConnections;
	conn->mNextGlobalConnection = nextConn;
	conn->mPrevGlobalConnection = NULL;
	mConnections = conn;

	if (nextConn != NULL)
	{
		nextConn->mPrevGlobalConnection = conn;
	}

	mConnectionsCount++;
}

bool Gridbrain::connectionExists(unsigned int xOrig,
				unsigned int yOrig,
				unsigned int gOrig,
				unsigned int xTarg,
				unsigned int yTarg,
				unsigned int gTarg)
{
	Grid* grid = mGridsVec[gOrig];
	unsigned int orig = (xOrig * grid->getHeight()) + yOrig + grid->getOffset();
	grid = mGridsVec[gTarg];
	unsigned int target = (xTarg * grid->getHeight()) + yTarg + grid->getOffset();

	GridbrainComponent* comp = &(mComponents[orig]);
	GridbrainConnection* conn = comp->mFirstConnection;

	unsigned int i = 0;
	while ((conn) && (i < comp->mConnectionsCount))
	{
		if (conn->mTarget == target)
		{
			return true;
		}

		conn = (GridbrainConnection*)conn->mNextConnection;
		i++;
	}

	return false;
}

void Gridbrain::selectRandomConnection(unsigned int &x1,
					unsigned int &y1,
					unsigned int &g1,
					unsigned int &x2,
					unsigned int &y2,
					unsigned int &g2)
{
	unsigned int connPos = rand() % mTotalPossibleConnections;
	unsigned int curPos = 0;
	unsigned int colConnCount = 0;


	Grid* gridOrig;

	bool found = false;
	for (unsigned g = 0;
		(g < mGridsCount) && (!found);
		g++)
	{
		gridOrig = mGridsVec[g];
		unsigned int width = gridOrig->getWidth();
		
		for (unsigned int col = 0;
			(col < width) && (!found);
			col++)
		{
			colConnCount = gridOrig->getColConnCount(col);
			curPos += colConnCount;

			if (curPos >= connPos)
			{
				g1 = gridOrig->getNumber();
				x1 = col;
				y1 = randomUniformInt(0, gridOrig->getHeight() - 1);
				found = true;
			}
		}
	}

	

	if (gridOrig->getType() == Grid::ALPHA)
	{
		unsigned int targConnPos = rand() % colConnCount;

		curPos = 0;
		unsigned int width = gridOrig->getWidth();
		for (unsigned int col = (x1 + 1); col < width; col++)
		{
			curPos += gridOrig->getColConnCount(col);

			if (curPos >= targConnPos)
			{
				g2 = gridOrig->getNumber();
				x2 = col;
				y2 = randomUniformInt(0, gridOrig->getHeight() - 1);
				return;
			}
		}

		// Assumes one beta at the end
		g2 = mGridsCount - 1;
	}
	else
	{
		g2 = g1;
	}

	Grid* targGrid = mGridsVec[g2];

	x2 = randomUniformInt(0, targGrid->getWidth() - 1);
	y2 = randomUniformInt(0, targGrid->getHeight() - 1);
}

void Gridbrain::addRandomConnection()
{
	unsigned int x1;
	unsigned int x2;
	unsigned int y1;
	unsigned int y2;
	unsigned int g1;
	unsigned int g2;
	float weight;

	selectRandomConnection(x1, y1, g1, x2, y2, g2);
	
	weight = randomUniformProbability();
	
	if (randomUniformBool())
	{
		weight = -weight;
	}
	
	addConnection(x1, y1, g1, x2, y2, g2, weight);
}

void Gridbrain::eval()
{
	GridbrainComponent* comp;
	GridbrainConnection* conn;

	// Reset all components
	for (unsigned int i = 0; i < mNumberOfComponents; i++)
	{
		comp = &(mComponents[i]);

		comp->mInput = 0;
		comp->mState = 0;
		comp->mForwardFlag = false;
		comp->mRecurrentFlag = false;

		// Transfer recurrent inputs to direct inputs
		if (i >= mFirstBetaIndex)
		{
			comp->mInput = mComponents[i].mRecurrentInput;
			comp->mRecurrentInput = 0;
		}
	}

	// Evaluate grids
	for (unsigned int gridNumber = 0; gridNumber < mGridsCount; gridNumber++)
	{
		Grid* grid = mGridsVec[gridNumber];

		unsigned int inputDepth = 1;
		int passCount = 1;
		unsigned int perceptionsCount = 0;
		if (grid->getType() == Grid::ALPHA)
		{
			passCount = 2;
			perceptionsCount = grid->getPerceptionsCount();
			inputDepth = grid->getInputDepth();
		}

		unsigned int startIndex;
		unsigned int endIndex;

		startIndex = grid->getOffset();
		endIndex = startIndex + grid->getSize();

		float* inputMatrix = grid->getInputMatrix();
		float* outputVector = grid->getOutputVector();

		for (unsigned int pass = 0; pass < passCount; pass++)
		{
			bool firstAlpha = false;
			if (pass < (passCount - 1))
			{
				firstAlpha = true;
			}

			unsigned int inputDepthOffset = 0;

			for (unsigned int i = 0; i < inputDepth; i++)
			{
				if (grid->getType() == Grid::ALPHA)
				{
					// reset alpha components except aggregators
					for (unsigned int i = startIndex; i < endIndex; i++)
					{
						comp = &(mComponents[i]);

						if (!comp->mAggregator)
						{
							if (comp->mType == GridbrainComponent::PER)
							{
								// apply perceptions for this input depth
								comp->mState =
									inputMatrix[comp->mPerceptionPosition
											+ inputDepthOffset];
							}
							else
							{
								comp->mState = 0;
							}
							comp->mForwardFlag = false;
							comp->mRecurrentFlag = false;
						}
						comp->mInput = 0;
					}
				}

				for (unsigned int j = startIndex; j < endIndex; j++)
				{
					comp = &(mComponents[j]);
					conn = comp->mFirstConnection;

					// compute component output
					float output;
					if (comp->mType == GridbrainComponent::NUL)
					{
						//printf("NUL ");
						output = 0.0f;
					}
					else if (comp->mType == GridbrainComponent::PER)
					{
						//printf("PER ");
						output = comp->mState;
					}
					else if (comp->mType == GridbrainComponent::STA)
					{
						//printf("STA ");
						output = comp->mState;
					}
					else if (comp->mType == GridbrainComponent::ACT)
					{
						//printf("ACT ");
						// TODO: return 1 if the action was executed?
						outputVector[comp->mActionPosition] = comp->mInput;
						output = 0.0f;
					}
					else if (comp->mType == GridbrainComponent::THR)
					{
						//printf("THR ");
						if ((comp->mInput > 0.1)
							|| (comp->mInput < -0.1))
						{
							output = comp->mInput;
						}
						else
						{
							output = 0.0f;
						}
					}
					else if (comp->mType == GridbrainComponent::AGG)
					{
						//printf("AGG ");
						if ((comp->mInput > 0.1)
							|| (comp->mInput < -0.1))
						{
							output = comp->mInput;
						}
						else
						{
							output = 0.0f;
						}
					}
					else if (comp->mType == GridbrainComponent::MAX)
					{
						//printf("MAX ");
						if (comp->mInput >= comp->mState)
						{
							output = 1.0f;
							comp->mState = comp->mInput;
						}
						else
						{
							output = 0.0f;
						}
					}
					else if (comp->mType == GridbrainComponent::MUL)
					{
						//printf("MUL ");
						output = comp->mInput;
					}
					else if (comp->mType == GridbrainComponent::NOT)
					{
						//printf("NOT ");
						if (comp->mInput == 0.0f)
						{
							output = 1.0f;
						}
						else
						{
							output = 0.0f;
						}
					}
					else {
							// TODO: this is an error. how to inform?
							output = 0.0f;
					}

					if (output > 1.0f)
					{
						output = 1.0f;
					}
					else if (output < -1.0f)
					{
						output = -1.0f;
					}
					comp->mOutput = output;

					//printf("%f => %f\n", comp->mInput, output);

					// propagate outputs (inside grid if fist alpha)
					for (unsigned int k = 0; k < comp->mConnectionsCount; k++)
					{
						if ((!firstAlpha) || (!conn->mInterGrid))
						{
							GridbrainComponent* targetComp = &(mComponents[conn->mTarget]);
							float input = output * conn->mWeight;

							/*printf("(%d, %d, %d)[%f] -> (%d, %d, %d)[%f]\n",
								conn->mColumnOrig,
								conn->mRowOrig,
								conn->mGridOrig,
								output,
								conn->mColumnTarg,
								conn->mRowTarg,
								conn->mGridTarg,
								input);*/
							
							// Feed forward
							if (conn->mFeedForward)
							{
								if (targetComp->mType == GridbrainComponent::MUL)
								{
									if (!targetComp->mForwardFlag)
									{
										targetComp->mInput = input;
										targetComp->mForwardFlag = true;
									}
									else
									{
										targetComp->mInput *= input;
									}
								}
								else
								{
									targetComp->mInput += input;
								}
							}
							// Recurent
							else
							{
								if (targetComp->mType == GridbrainComponent::MUL)
								{
									if (!targetComp->mRecurrentFlag)
									{
										targetComp->mRecurrentInput = input;
										targetComp->mRecurrentFlag = true;
									}
									else
									{
										targetComp->mRecurrentInput *= input;
									}
								}
								else
								{
									targetComp->mRecurrentInput += input;
								}
							}
						}
						conn = (GridbrainConnection*)conn->mNextConnection;
					}
				}

				inputDepthOffset += perceptionsCount;
			}
		}
	}
}

void Gridbrain::mutateAddConnection()
{
	addRandomConnection();
}

void Gridbrain::mutateRemoveConnection()
{
	if (mConnectionsCount > 0) 
	{
		unsigned int connectionPos = randomUniformInt(0, mConnectionsCount - 1);

		GridbrainConnection* conn = mConnections;
		for (unsigned int i = 0; i < connectionPos; i++)
		{
			conn = (GridbrainConnection*)conn->mNextGlobalConnection;
		}

		GridbrainComponent* comp = (GridbrainComponent*)conn->mOrigComponent;
		comp->mConnectionsCount--;

		if (conn->mPrevConnection)
		{
			((GridbrainConnection*)conn->mPrevConnection)->mNextConnection = conn->mNextConnection;
		}
		else
		{
			comp->mFirstConnection = (GridbrainConnection*)conn->mNextConnection;
		}
		if (conn->mNextConnection)
		{
			((GridbrainConnection*)conn->mNextConnection)->mPrevConnection = conn->mPrevConnection;
		}

		mConnectionsCount--;
		if (conn->mPrevGlobalConnection)
		{
			((GridbrainConnection*)conn->mPrevGlobalConnection)->mNextGlobalConnection =
				conn->mNextGlobalConnection;
		}
		else
		{
			mConnections = (GridbrainConnection*)conn->mNextGlobalConnection;
		}
		if (conn->mNextGlobalConnection)
		{
			((GridbrainConnection*)conn->mNextGlobalConnection)->mPrevGlobalConnection =
				conn->mPrevGlobalConnection;
		}

		free(conn);
	}
}

void Gridbrain::mutateChangeConnectionWeight()
{
	if (mConnectionsCount > 0) 
	{
		unsigned int connectionPos = randomUniformInt(0, mConnectionsCount - 1);

		GridbrainConnection* conn = mConnections;
		for (unsigned int i = 0; i < connectionPos; i++)
		{
			conn = (GridbrainConnection*)conn->mNextGlobalConnection;
		}
		
		float newWeight = conn->mWeight;
		newWeight += randomUniformFloat(-1.0f, 1.0f);
		if (newWeight > 1.0f)
		{
			newWeight = 1.0f;
		}
		else if (newWeight < -1.0f)
		{
			newWeight = -1.0f;
		}

		conn->mWeight = newWeight;
	}
}

void Gridbrain::mutateChangeComponent()
{
	unsigned int pos = randomUniformInt(0, mNumberOfComponents - 1);

	Grid* grid = mGridsVec[mComponents[pos].mGrid];

	GridbrainComponent* comp = grid->getRandomComponent(pos);

	mComponents[pos].copy(comp, false);

	initGridInputOutput(grid);
}

Grid* Gridbrain::getGrid(unsigned int number)
{
	return mGridsVec[number];
}

void Gridbrain::calcConnectionCounts()
{
	mTotalPossibleConnections = 0;

	for (unsigned int g = 0; g < mGridsCount; g++)
	{
		Grid* grid = mGridsVec[g];
		unsigned int width = grid->getWidth();
		unsigned int height = grid->getHeight();

		for (unsigned int col = 0; col < width; col++)
		{
			unsigned int possibleConnections;

			possibleConnections = mBetaComponentsCount * height;

			if (grid->getType() == Grid::ALPHA)
			{
				possibleConnections += (width - col) * height * height;
			}
			
			grid->setColConnCount(col, possibleConnections);
			mTotalPossibleConnections += possibleConnections;
		}
	}
}

