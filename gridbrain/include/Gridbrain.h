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

#ifndef __INCLUDE_GRIDBRAIN_H
#define __INCLUDE_GRIDBRAIN_H

#define GB_THRESHOLD 0.1f

//#define GB_VALIDATE

#include <stdio.h>

#include "Brain.h"
#include "Grid.h"
#include "Component.h"
#include "GridbrainConnection.h"
#include "ComponentSet.h"
#include "RandDistManager.h"
#include "types.h"
#include "SimObj.h"
#include "Simulation.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

class Gridbrain : public Brain
{
public:
    enum ExpansionType {ET_NONE, ET_COLUMN, ET_ROW};
    enum CompEquivalenceType {CET_ORIGIN, CET_TARGET, CET_NEW};
    enum RecombinationType {RT_UNIFORM, RT_PATHS};

    Gridbrain(lua_State* luaState=NULL);
    virtual ~Gridbrain();

    virtual Brain* clone();
    Gridbrain* clone(bool grow, ExpansionType expansion=ET_NONE, unsigned int targetGrid=0, GridCoord* gc=NULL);

    void addGrid(Grid* grid, string name);
    virtual void init();

    virtual float* getInputBuffer(unsigned int channel, unsigned int id=0);
    virtual float* getOutputBuffer();

    Component* getComponent(unsigned int x,
                unsigned int y,
                unsigned int gridNumber);
    void addConnection(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg,
                GridbrainGeneTag tag=GridbrainGeneTag());
    void removeConnection(GridbrainConnection* conn);
    void removeConnection(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg);
    GridbrainConnection* getRandomConnection();
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
    bool selectRandomConnection(unsigned int &x1,
                unsigned int &y1,
                unsigned int &g1,
                unsigned int &x2,
                unsigned int &y2,
                unsigned int &g2);
    bool selectRandomOrigin(unsigned int &x1,
                unsigned int &y1,
                unsigned int &g1,
                unsigned int x2,
                unsigned int y2,
                unsigned int g2);
    bool selectRandomTarget(unsigned int x1,
                unsigned int y1,
                unsigned int g1,
                unsigned int &x2,
                unsigned int &y2,
                unsigned int &g2);
    void addRandomConnections(unsigned int count);
    void setMaxInputDepth(unsigned int maxInputDepth){mMaxInputDepth = maxInputDepth;}

    void cycle();
    
    Grid* getGrid(unsigned int number);
    Grid* getGrid(string name);

    unsigned int getConnectionsCount(){return mConnectionsCount;}

    virtual void draw(){}

    virtual void mutate(float factor=1.0f);
    virtual Brain* recombine(Brain* brain);

    Component* setComponent(unsigned int x, unsigned int y, unsigned int g, Component& comp);
    void setComponent(unsigned int x,
                unsigned int y,
                unsigned int gridNumber,
                Component::Type type,
                float param=0.0f,
                int subType=-1,
                InterfaceItem::TableLinkType linkType=InterfaceItem::NO_LINK,
                int origSymTable=-1,
                llULINT origSymID=0,
                int targetSymTable=-1);

    Component* replaceComponent(unsigned int pos, Component* comp);

    void setMutateAddConnectionProb(float prob){mMutateAddConnectionProb = prob;}
    void setMutateRemoveConnectionProb(float prob){mMutateRemoveConnectionProb = prob;}
    void setMutateChangeParamProb(float prob){mMutateChangeParamProb = prob;}
    void setParamMutationStanDev(float sd){mParamMutationStanDev = sd;}
    void setMutateSplitConnectionProb(float prob){mMutateSplitConnectionProb = prob;}
    void setMutateJoinConnectionsProb(float prob){mMutateJoinConnectionsProb = prob;}
    void setMutateChangeComponentProb(float prob){mMutateChangeComponentProb = prob;}
    void setMutateChangeInactiveComponentProb(float prob){mMutateChangeInactiveComponentProb = prob;}
    void setMutateSwapComponentProb(float prob){mMutateSwapComponentProb = prob;}

    void setRecombinationType(RecombinationType type){mRecombinationType = type;}
    void setGeneGrouping(bool val){mGeneGrouping = val;}

    virtual bool getFieldValue(string fieldName, float& value);

    virtual string write(SimObj* obj, Simulation* sim);
    virtual void printDebug();
    void printConnection(GridbrainConnection* conn);

    bool isConnectionValid(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg);
    bool canCreateConnection(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg);
    void cleanInvalidConnections();
    bool isValid();

    virtual bool symbolUsed(int tableID, llULINT symbolID);

    void setAllActive(bool active){mAllActive = active;}

    void update();
    virtual void repair();

    virtual float getDistance(Brain* brain);

    virtual void markUsedSymbols(TableSet* tab);

    static const char mClassName[];
    static Orbit<Gridbrain>::MethodType mMethods[];
    static Orbit<Gridbrain>::NumberGlobalType mNumberGlobals[];

    int init(lua_State* luaState);
    int setComponent(lua_State* luaState);
    int addGrid(lua_State* luaState);
    int addConnection(lua_State* luaState);
    int addRandomConnections(lua_State* luaState);
    int setMutateAddConnectionProb(lua_State* luaState);
    int setMutateRemoveConnectionProb(lua_State* luaState);
    int setMutateChangeParamProb(lua_State* luaState);
    int setMutateSplitConnectionProb(lua_State* luaState);
    int setMutateJoinConnectionsProb(lua_State* luaState);
    int setMutateChangeComponentProb(lua_State* luaState);
    int setMutateChangeInactiveComponentProb(lua_State* luaState);
    int setMutateSwapComponentProb(lua_State* luaState);
    int setParamMutationStanDev(lua_State* luaState);
    int setRecombinationType(lua_State* luaState);
    int setGeneGrouping(lua_State* luaState);
    int setMaxInputDepth(lua_State* luaState);

    static long MUTATIONS_ADD_CONN;
    static long MUTATIONS_REM_CONN;
    static long MUTATIONS_SPLIT_CONN;
    static long MUTATIONS_JOIN_CONN;
    static long MUTATIONS_CHG_COMP;
    static long MUTATIONS_CHG_IN_COMP;
    static long MUTATIONS_CHG_PARAM;
    static long MUTATIONS_SWP_COMP;

    static void debugMutationsCount();

protected:
    void initGridsIO();
    void calcConnectionCounts();
    void calcActive();
    void calcActiveComponents();
    void calcDensityMetrics();
    void calcExpansion();

    void mutateAddConnection(unsigned int popSize, float prob);
    void mutateRemoveConnection(unsigned int popSize, float prob);
    void mutateSplitConnection(float prob);
    void mutateJoinConnections(float prob);
    void mutateChangeComponent(float prob);
    void mutateChangeInactiveComponent(float prob);
    void mutateChangeParam(float prob);

    void initGridWritePositions();
    void getComponentWritePos(unsigned int& posX,
                                unsigned int& posY,
                                unsigned int x,
                                unsigned int y,
                                unsigned int grid);
    
    void initRandomConnectionSequence(float selectionProb);
    GridbrainConnection* nextRandomConnection();
    GridbrainConnection* skipNextRandomConnection();
    void initRandomComponentSequence(float selectionProb);
    int nextRandomComponent();

    unsigned int generateEventCount(float eventProb, unsigned int popSize);

    unsigned int getRelativeOffset(Component* compOrig,
                                            unsigned int targX,
                                            unsigned int targY,
                                            unsigned int targG);
    Component* getCompByRelativeOffset(Component* compOrig, unsigned int offset);

    void clearRecombineInfo();
    void selectConnUniform(Gridbrain* gb1, Gridbrain* gb2);
    void selectPath(Component* comp);
    void selectConnPaths(Gridbrain* gb1, Gridbrain* gb2);
    bool exists(Component* comp, unsigned int ex, unsigned int ey);
    void recombineUnusedComponents(Gridbrain* gb1, Gridbrain* gb2);
    Component* findEquivalentComponent(Component* comp);
    Gridbrain* importComponent(Gridbrain* gbTarg,
                                Gridbrain* gbOrig,
                                Component* compOrig,
                                Component*& compTarg);
    Gridbrain* importConnection(Gridbrain* gbTarg,
                                Gridbrain* gbOrig,
                                GridbrainConnection* conn);

    bool checkGene(llULINT geneID);
    bool isGeneSelected(llULINT geneID);
    bool selectGene(llULINT geneID, bool select);

    int compEquivalence(Component* comp1, Component* comp2, CompEquivalenceType eqType);
    GridbrainGeneTag findGeneTag(GridbrainConnection* conn);
    virtual void popAdjust(vector<SimObj*>* popVec);

    static mt_distribution* mDistConnections;
    static mt_distribution* mDistMutationsProb;
    static mt_distribution* mDistComponents;
    static mt_distribution* mDistRecombine;
    static mt_distribution* mDistGridbrain;

    vector<Grid*> mGridsVec;

    vector<Component*> mComponents;
    unsigned int mMaxInputDepth;
    unsigned int mNumberOfComponents;
    GridbrainConnection* mConnections;
    unsigned int mConnectionsCount;
    unsigned int mGridsCount;
    unsigned int mFirstBetaIndex;
    unsigned int mTotalPossibleConnections;
    unsigned int mBetaComponentsCount;

    float mMutateAddConnectionProb;
    float mMutateRemoveConnectionProb;
    float mMutateChangeParamProb;
    float mParamMutationStanDev;
    float mMutateSplitConnectionProb;
    float mMutateJoinConnectionsProb;
    float mMutateChangeComponentProb;
    float mMutateChangeInactiveComponentProb;
    float mMutateSwapComponentProb;

    float mConnSeqProb;
    GridbrainConnection* mConnSeqCurrent;
    float mCompSeqProb;
    unsigned int mCompSeqPos;

    bool mAllActive;
    unsigned int mActiveConnections;
    unsigned int mActiveComponents;
    unsigned int mActivePerceptions;
    unsigned int mActiveActions;

    bool mGeneGrouping;

    RecombinationType mRecombinationType;

    llULINT mLastMemID;
};

#endif

