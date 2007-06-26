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

#ifndef __INCLUDE_GRID_H
#define __INCLUDE_GRID_H

#include "ScriptableObject.h"
#include "GridbrainComponentSet.h"
#include "Orbit.h"

#include <vector>
#include <list>
#include <map>

class Grid : public ScriptableObject
{
public:
	static const int ALPHA;
	static const int BETA;

	Grid(int type, unsigned int width, unsigned int height);
	Grid(Grid* grid);
	virtual ~Grid();

	void init();
	void setType(int type){mType = type;}
	void setNumber(unsigned int number){mNumber = number;}
	void setWidth(unsigned int width){mWidth = width;}
	void setHeight(unsigned int height){mHeight = height;}
	void addComponentSet(GridbrainComponentSet* componentSet, int endColumn=-1);
	void setOffset(unsigned int offset){mOffset = offset;}
	int getType(){return mType;}
	unsigned int getNumber(){return mNumber;}
	unsigned int getWidth(){return mWidth;}
	unsigned int getHeight(){return mHeight;}
	unsigned int getSize(){return mSize;}
	unsigned int getOffset(){return mOffset;}
	GridbrainComponent* getRandomComponent(unsigned int pos);
	void setInput(unsigned int number, unsigned int depth, float value);
	unsigned int addPerception(GridbrainComponent* per);
	unsigned int addAction(GridbrainComponent* act);
	void initInputMatrix(unsigned int maxInputDepth);
	float* getInputMatrix(){return mInputMatrix;}
	void initOutputVector();
	float* getOutputVector(){return mOutputVector;}
	unsigned int getPerceptionType(unsigned int number);
	void setInputDepth(unsigned int inputDepth){mInputDepth = inputDepth;}
	unsigned int getInputDepth(){return mInputDepth;}
	unsigned int getMaxInputDepth(){return mMaxInputDepth;}
	unsigned int getPerceptionsCount(){return mPerceptionsCount;}
	unsigned int getActionsCount(){return mActionsCount;}
	float getOutput(unsigned int number);
	unsigned int getActionType(unsigned int number);
	unsigned long getRowCode(unsigned int pos){return mRowsVec[pos];}
	unsigned long getColumnCode(unsigned int pos){return mColumnsVec[pos];}
	unsigned int getXByCode(unsigned long code){return mColumnsMap[code];}
	unsigned int getYByCode(unsigned long code){return mRowsMap[code];}
	unsigned int getXByOffset(unsigned int offset);
	void removeInputOutput();
	unsigned int getColConnCount(unsigned int col){return mColumnsConnectionsCountVec[col];}
	void setColConnCount(unsigned int col, unsigned int count){mColumnsConnectionsCountVec[col] = count;}

	static const char mClassName[];
        static Orbit<Grid>::MethodType mMethods[];
	static Orbit<Grid>::NumberGlobalType mNumberGlobals[];

        Grid(lua_State* luaState);
        int addComponentSet(lua_State* luaState);
	int setWidth(lua_State* luaState);
	int setHeight(lua_State* luaState);

	static unsigned int CURRENT_COLUMN_ID;
	static unsigned int CURRENT_ROW_ID;

protected:
	int mType;
	unsigned int mNumber;
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mSize;
	unsigned int mOffset;
	std::list<GridbrainComponentSet*> mComponentSet;
	std::list<int> mComponentSetEndColumn;

	std::vector<unsigned long> mRowsVec;
	std::vector<unsigned long> mColumnsVec;
	std::map<unsigned long, unsigned int> mRowsMap;
	std::map<unsigned long, unsigned int> mColumnsMap;

	float* mInputMatrix;
	float* mOutputVector;
	unsigned int mMaxInputDepth;
	unsigned int mInputDepth;
	std::vector<GridbrainComponent*> mPerceptionsVec;
	std::vector<GridbrainComponent*> mActionsVec;
	unsigned int mPerceptionsCount;
	unsigned int mActionsCount;

	std::vector<unsigned int> mColumnsConnectionsCountVec;
};
#endif

