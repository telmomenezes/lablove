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
#include "GridbrainComponent.h"
#include "GridbrainConnection.h"
#include "GridbrainComponentSet.h"
#include "GridbrainMemCell.h"
#include "RandDistManager.h"
#include "types.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

class Gridbrain : public Brain
{
public:
    enum GrowMethod {GM_FREE, GM_PRESSURE};
    enum CloneConnectionsMode {CC_ALL, CC_ALL_PLUS, CC_ACTIVE};
    enum MutationScope {MS_ALL, MS_ACTIVE};
    enum ExpansionType {ET_NONE,
                        ET_COLUMN_RANDOM,
                        ET_COLUMN_FIRST,
                        ET_COLUMN_LAST,
                        ET_COLUMN_BEFORE,
                        ET_COLUMN_AFTER,
                        ET_ROW};
    enum CompEquivalenceType {CET_ORIGIN, CET_TARGET, CET_NEW};

    Gridbrain(lua_State* luaState=NULL);
    virtual ~Gridbrain();

    virtual Brain* clone();
    Gridbrain* clone(bool grow, ExpansionType expansion, unsigned int targetGrid, unsigned int pos=0);

    void addGrid(Grid* grid, string name);
    virtual void init();

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
                float weight,
                GridbrainGeneTag tag=GridbrainGeneTag());
    void addConnectionReal(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg,
                float realWeight,
                GridbrainGeneTag tag=GridbrainGeneTag());
    void removeConnection(GridbrainConnection* conn);
    void removeConnection(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg);
    void removeRandomConnection(unsigned int initialPop);
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

    virtual void mutate();
    virtual Brain* recombine(Brain* brain);

    virtual void setSelectedSymbols(SimulationObject* obj);

    void setComponent(unsigned int x,
                unsigned int y,
                unsigned int gridNumber,
                GridbrainComponent::Type type,
                int subType=-1,
                InterfaceItem::TableLinkType linkType=InterfaceItem::NO_LINK,
                int origSymTable=-1,
                llULINT origSymID=0,
                int targetSymTable=-1);

    void setGrowMethod(GrowMethod val){mGrowMethod = val;}
    void setCloneConnectionsMode(CloneConnectionsMode val){mCloneConnectionsMode = val;}
    void setMutationScope(MutationScope val){mMutationScope = val;}

    void setMutateAddConnectionProb(float prob){mMutateAddConnectionProb = prob;}
    void setMutateRemoveConnectionProb(float prob){mMutateRemoveConnectionProb = prob;}
    void setMutateChangeConnectionWeightProb(float prob){mMutateChangeConnectionWeightProb = prob;}
    void setWeightMutationStanDev(float sd){mWeightMutationStanDev = sd;}
    void setMutateSplitConnectionProb(float prob){mMutateSplitConnectionProb = prob;}
    void setMutateJoinConnectionsProb(float prob){mMutateJoinConnectionsProb = prob;}
    void setMutateChangeComponentProb(float prob){mMutateChangeComponentProb = prob;}
    void setMutateChangeInactiveComponentProb(float prob){mMutateChangeInactiveComponentProb = prob;}
    void setMutateSwapComponentProb(float prob){mMutateSwapComponentProb = prob;}

    void setGeneGrouping(bool val){mGeneGrouping = val;}

    virtual bool getFieldValue(string fieldName, float& value);

    virtual string write(SimulationObject* obj, PopulationManager* pop);
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

    static const char mClassName[];
    static Orbit<Gridbrain>::MethodType mMethods[];
    static Orbit<Gridbrain>::NumberGlobalType mNumberGlobals[];

    int init(lua_State* luaState);
    int setComponent(lua_State* luaState);
    int addGrid(lua_State* luaState);
    int addConnection(lua_State* luaState);
    int addConnectionReal(lua_State* luaState);
    int addRandomConnections(lua_State* luaState);
    int setGrowMethod(lua_State* luaState);
    int setCloneConnectionsMode(lua_State* luaState);
    int setMutationScope(lua_State* luaState);
    int setMutateAddConnectionProb(lua_State* luaState);
    int setMutateRemoveConnectionProb(lua_State* luaState);
    int setMutateChangeConnectionWeightProb(lua_State* luaState);
    int setMutateSplitConnectionProb(lua_State* luaState);
    int setMutateJoinConnectionsProb(lua_State* luaState);
    int setMutateChangeComponentProb(lua_State* luaState);
    int setMutateChangeInactiveComponentProb(lua_State* luaState);
    int setMutateSwapComponentProb(lua_State* luaState);
    int setWeightMutationStanDev(lua_State* luaState);
    int setGeneGrouping(lua_State* luaState);

    static long MUTATIONS_ADD_CONN;
    static long MUTATIONS_REM_CONN;
    static long MUTATIONS_CHG_WGT;
    static long MUTATIONS_SPLIT_CONN;
    static long MUTATIONS_JOIN_CONN;
    static long MUTATIONS_CHG_COMP;
    static long MUTATIONS_CHG_IN_COMP;
    static long MUTATIONS_SWP_COMP;

    static void debugMutationsCount();

protected:
    Gridbrain* baseClone();
    
    void generateMemory(Gridbrain* originGB=NULL);

    void update();

    void initGridsIO();
    void calcConnectionCounts();
    void calcActive();
    void calcDensityMetrics();
    void calcExpansion();
    void linkMemory();

    void applyWeight(GridbrainConnection* conn);

    bool swapComponents(GridbrainComponent* comp1, GridbrainComponent* comp2);

    bool isSplitable(GridbrainConnection* conn);
    GridbrainConnection* selectSplitableConnection(unsigned int initialPop);
    GridbrainConnection* selectJoinableConnection(unsigned int initialPop);

    GridbrainConnection* getFirstMutableConnection(unsigned int initialPop);
    unsigned int getMutableConnectionsCount(unsigned int initialPop);

    void mutateAddConnection(unsigned int popSize);
    void mutateRemoveConnection(unsigned int popSize);
    void mutateChangeConnectionWeight();
    void mutateSplitConnection(unsigned int popSize);
    void mutateJoinConnections(unsigned int popSize);
    void mutateChangeComponent();
    void mutateChangeInactiveComponent();
    void mutateSwapComponent(float prob=0.0f);

    void initGridWritePositions();
    void getComponentWritePos(unsigned int& posX,
                                unsigned int& posY,
                                unsigned int x,
                                unsigned int y,
                                unsigned int grid);
    
    void initRandomConnectionSequence(float selectionProb);
    GridbrainConnection* nextRandomConnection();
    void initRandomComponentSequence(float selectionProb);
    int nextRandomComponent();

    unsigned int generateEventCount(float eventProb, unsigned int popSize);

    unsigned int getRelativeOffset(GridbrainComponent* compOrig,
                                            unsigned int targX,
                                            unsigned int targY,
                                            unsigned int targG);
    GridbrainComponent* getCompByRelativeOffset(GridbrainComponent* compOrig, unsigned int offset);

    void clearRecombineInfo();
    GridbrainComponent* findEquivalentComponent(GridbrainComponent* comp, CompEquivalenceType eqType);
    Gridbrain* importConnection(Gridbrain* gb,
                                GridbrainConnection* conn,
                                bool &canAddComponent,
                                bool &success,
                                unsigned int &failsOrder,
                                unsigned int &failsComp);
    bool correctOrder(int& x1, int& y1, int& x2, int& y2, int g);

    bool checkGene(llULINT geneID);
    bool selectGene(llULINT geneID, bool select);

    int compEquivalence(GridbrainComponent* comp1, GridbrainComponent* comp2, CompEquivalenceType eqType);
    GridbrainGeneTag findGeneTag(GridbrainConnection* conn);
    virtual void popAdjust(vector<SimulationObject*>* popVec);

    static llULINT CURRENT_MEM_ID;

    static mt_distribution* mDistConnections;
    static mt_distribution* mDistMutationsProb;
    static mt_distribution* mDistWeights;
    static mt_distribution* mDistComponents;
    static mt_distribution* mDistRecombine;
    static mt_distribution* mDistGridbrain;

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

    float mMutateAddConnectionProb;
    float mMutateRemoveConnectionProb;
    float mMutateChangeConnectionWeightProb;
    float mWeightMutationStanDev;
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

    GrowMethod mGrowMethod;
    CloneConnectionsMode mCloneConnectionsMode;
    MutationScope mMutationScope;

    bool mGeneGrouping;

    map<llULINT, GridbrainMemCell> mMemory;
};

#endif

