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

#define GB_THRESHOLD 0.1f

#include <stdio.h>

#include "Brain.h"
#include "Grid.h"
#include "GridbrainComponent.h"
#include "GridbrainConnection.h"
#include "GridbrainComponentSet.h"
#include "RandDistManager.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

class Gridbrain : public Brain
{
public:
    Gridbrain(lua_State* luaState=NULL);
    virtual ~Gridbrain();

    virtual Brain* clone(bool randomize=false);

    void addGrid(Grid* grid, string name);
    virtual void init();
    virtual void onAdd();
    void initEmpty();

    virtual float* getInputBuffer(unsigned int channel);
    virtual float* getOutputBuffer();

    GridbrainComponent* getComponent(unsigned int x,
                unsigned int y,
                unsigned int gridNumber);
    void addConnection(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg,
                float weight);
    void removeConnection(GridbrainConnection* conn);
    void removeConnection(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg);
    GridbrainConnection* getConnection(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg);
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
    void addRandomConnections(unsigned int count);
    void setMaxInputDepth(unsigned int maxInputDepth){mMaxInputDepth = maxInputDepth;}

    void cycle();
    
    Grid* getGrid(unsigned int number);

    void initGridsInputOutput();

    unsigned int getConnectionsCount(){return mConnectionsCount;}

    virtual void draw(pyc::Layer2D* layer){}

    virtual void mutate();

    void setComponent(unsigned int x,
                unsigned int y,
                unsigned int gridNumber,
                GridbrainComponent::Type type,
                int subType=-1,
                int symTable=-1,
                int origSymIndex=-1,
                int targetSymIndex=-1);

    void setMutateAddConnectionProb(float prob){mMutateAddConnectionProb = prob;}
    void setMutateRemoveConnectionProb(float prob){mMutateRemoveConnectionProb = prob;}
    void setMutateChangeConnectionWeightProb(float prob){mMutateChangeConnectionWeightProb = prob;}
    void setMutateSplitConnectionProb(float prob){mMutateSplitConnectionProb = prob;}
    void setMutateChangeComponentProb(float prob){mMutateChangeComponentProb = prob;}
    void setWeightMutationStanDev(float sd){mWeightMutationStanDev = sd;}

    static const char mClassName[];
    static Orbit<Gridbrain>::MethodType mMethods[];
    static Orbit<Gridbrain>::NumberGlobalType mNumberGlobals[];

    int initEmpty(lua_State* luaState);
    int setComponent(lua_State* luaState);
    int addGrid(lua_State* luaState);
    int addConnection(lua_State* luaState);
    int addRandomConnections(lua_State* luaState);
    int setMutateAddConnectionProb(lua_State* luaState);
    int setMutateRemoveConnectionProb(lua_State* luaState);
    int setMutateChangeConnectionWeightProb(lua_State* luaState);
    int setMutateSplitConnectionProb(lua_State* luaState);
    int setMutateChangeComponentProb(lua_State* luaState);
    int setWeightMutationStanDev(lua_State* luaState);

    virtual string write();

protected:
    void initGridInputOutput(Grid* grid, int gPos=-1);
    void calcConnectionCounts();

    void applyWeight(GridbrainConnection* conn);

    void mutateAddConnection();
    void mutateRemoveConnection();
    void mutateChangeConnectionWeight();
    void mutateSplitConnection();
    void mutateChangeComponent();

    void initGridWritePositions();
    void getComponentWritePos(unsigned int& posX,
                                unsigned int& posY,
                                unsigned int x,
                                unsigned int y,
                                unsigned int grid);

    static mt_distribution* mDistConnections;
    static mt_distribution* mDistMutationsProb;
    static mt_distribution* mDistWeights;
    static mt_distribution* mDistComponents;

    vector<Grid*> mGridsVec;

    GridbrainComponent* mComponents;
    unsigned int mMaxInputDepth;
    unsigned int mNumberOfComponents;
    GridbrainConnection* mConnections;
    unsigned int mConnectionsCount;
    unsigned int mGridsCount;
    unsigned int mFirstBetaIndex;
    unsigned int mTotalPossibleConnections;
    unsigned int mBetaComponentsCount;

    bool bRecurrentAllowed;

    float mMutateAddConnectionProb;
    float mMutateRemoveConnectionProb;
    float mMutateChangeConnectionWeightProb;
    float mMutateSplitConnectionProb;
    float mMutateChangeComponentProb;
    float mWeightMutationStanDev;
};

#endif

