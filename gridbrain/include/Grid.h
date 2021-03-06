/*
 * Gridbrain
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

#include "ComponentSet.h"
#include "Coord.h"

#include <vector>
#include <list>
#include <map>

using std::vector;
using std::list;
using std::map;

namespace gb
{

class Grid
{
public:
    enum Type {ALPHA, BETA};

    Grid();
    Grid(const Grid& grid);
    virtual ~Grid();

    void init(Type type, unsigned int width, unsigned int height);

    int getType(){return mType;}
    unsigned int getWidth(){return mWidth;}
    unsigned int getHeight(){return mHeight;}
    unsigned int getSize(){return mSize;}

    void setComponentSet(ComponentSet* componentSet);
    ComponentSet* getComponentSet(){return mComponentSet;}

    void setOffset(unsigned int offset){mOffset = offset;}
    
    unsigned int getOffset(){return mOffset;}
    Component* getRandomComponent();
    void setInput(unsigned int number, unsigned int depth, float value);
    unsigned int addPerception(Component* per);
    unsigned int addAction(Component* act);
    void initInputMatrix(unsigned int maxInputDepth);
    float* getInputMatrix(){return mInputMatrix;}
    unsigned int getInputID(unsigned int depth);
    void initOutputVector();
    float* getOutputVector(){return mOutputVector;}
    Component* getPerception(unsigned int number);
    Component* getAction(unsigned int number);
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
    float* getInputBuffer(unsigned int id);

    unsigned int getWriteX(){return mWriteX;}
    unsigned int getWriteY(){return mWriteY;}
    void setWritePos(unsigned int x, unsigned int y){mWriteX = x; mWriteY = y;}

    void addColumn(Coord* gc=NULL);
    void addRow(Coord* gc=NULL);
    void deleteColumn(Coord col);
    void deleteRow(Coord row);
    Coord getColCoordAfter(Coord col);

    Coord getColumnCoord(unsigned int col){return mColumnCoords[col];}
    Coord getRowCoord(unsigned int row){return mRowCoords[row];}
    int getColumnByCoord(Coord coord);
    int getRowByCoord(Coord coord);

    bool isValid();

    Component** mComponentSequence;
    unsigned int mComponentSequenceSize;

    unsigned int mMaxDepth;
    unsigned int mMaxActiveCol;

protected:
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
    unsigned int* mInputIDs;
    float* mOutputVector;

    ComponentSet* mComponentSet;

    vector<Component*> mPerceptionsVec;
    vector<Component*> mActionsVec;

    vector<unsigned int> mColumnTargetCountVec;

    unsigned int mWriteX;
    unsigned int mWriteY;

    vector<Coord> mColumnCoords;
    vector<Coord> mRowCoords;
};

}

#endif

