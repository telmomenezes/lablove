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

#ifndef __INCLUDE_GRID_H
#define __INCLUDE_GRID_H

#include "ScriptableObject.h"
#include "GridbrainComponentSet.h"
#include "RandDistManager.h"
#include "Orbit.h"
#include "GridCoord.h"

#include <vector>
#include <list>
#include <map>

using std::vector;
using std::list;
using std::map;

class Grid : public ScriptableObject
{
public:
    enum Type {ALPHA, BETA};

    Grid(lua_State* luaState=NULL);
    Grid(const Grid& grid);
    virtual ~Grid();

    void init(Type type, unsigned int width, unsigned int height);

    int getType(){return mType;}
    unsigned int getWidth(){return mWidth;}
    unsigned int getHeight(){return mHeight;}
    unsigned int getSize(){return mSize;}

    void setComponentSet(GridbrainComponentSet* componentSet);

    void setOffset(unsigned int offset){mOffset = offset;}
    
    unsigned int getOffset(){return mOffset;}
    GridbrainComponent* getRandomComponent(SimObj* obj,
                                            GridbrainComponent* components,
                                            map<llULINT, GridbrainMemCell>* memory);
    void setInput(unsigned int number, unsigned int depth, float value);
    unsigned int addPerception(GridbrainComponent* per);
    unsigned int addAction(GridbrainComponent* act);
    void initInputMatrix(unsigned int maxInputDepth);
    float* getInputMatrix(){return mInputMatrix;}
    void initOutputVector();
    float* getOutputVector(){return mOutputVector;}
    GridbrainComponent* getPerception(unsigned int number);
    GridbrainComponent* getAction(unsigned int number);
    void setInputDepth(unsigned int inputDepth){mInputDepth = inputDepth;}
    unsigned int getInputDepth(){return mInputDepth;}
    unsigned int getMaxInputDepth(){return mMaxInputDepth;}
    unsigned int getPerceptionsCount(){return mPerceptionsCount;}
    unsigned int getActionsCount(){return mActionsCount;}
    float getOutput(unsigned int number);
    unsigned int getXByOffset(unsigned int offset);
    void removeInputOutput();
    unsigned int getColTargCount(unsigned int col){return mColumnTargetCountVec[col];}
    void setColTargCount(unsigned int col, unsigned int count){mColumnTargetCountVec[col] = count;}
    float* getInputBuffer();

    unsigned int getWriteX(){return mWriteX;}
    unsigned int getWriteY(){return mWriteY;}
    void setWritePos(unsigned int x, unsigned int y){mWriteX = x; mWriteY = y;}

    void addColumn(GridCoord* gc=NULL);
    void addRow(GridCoord* gc=NULL);
    void deleteColumn(GridCoord col);
    void deleteRow(GridCoord row);
    GridCoord getColCoordAfter(GridCoord col);

    GridCoord getColumnCoord(unsigned int col){return mColumnCoords[col];}
    GridCoord getRowCoord(unsigned int row){return mRowCoords[row];}
    int getColumnByCoord(GridCoord coord);
    int getRowByCoord(GridCoord coord);

    bool isValid();

    GridbrainComponent** mComponentSequence;
    unsigned int mComponentSequenceSize;

    unsigned int mMaxDepth;
    unsigned int mMaxActiveCol;

    static const char mClassName[];
    static Orbit<Grid>::MethodType mMethods[];
    static Orbit<Grid>::NumberGlobalType mNumberGlobals[];

    int setComponentSet(lua_State* luaState);
    int init(lua_State* luaState);

protected:
    static mt_distribution* mDistRowsAndColumns;
    static mt_distribution* mDistXover;

    Type mType;
    unsigned int mWidth;
    unsigned int mHeight;
    unsigned int mSize;
    unsigned int mOffset;
    unsigned int mMaxInputDepth;
    unsigned int mInputDepth;
    unsigned int mPerceptionsCount;
    unsigned int mActionsCount;

    float* mInputMatrix;
    float* mOutputVector;

    GridbrainComponentSet* mComponentSet;

    vector<GridbrainComponent*> mPerceptionsVec;
    vector<GridbrainComponent*> mActionsVec;

    vector<unsigned int> mColumnTargetCountVec;

    unsigned int mWriteX;
    unsigned int mWriteY;

    vector<GridCoord> mColumnCoords;
    vector<GridCoord> mRowCoords;
};
#endif

