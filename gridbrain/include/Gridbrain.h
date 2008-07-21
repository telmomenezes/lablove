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

#include "Grid.h"
#include "Component.h"
#include "Connection.h"
#include "RandDistManager.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

class Gridbrain
{
public:
    enum ExpansionType {ET_NONE, ET_COLUMN, ET_ROW};
    enum CompEquivalenceType {CET_ORIGIN, CET_TARGET, CET_NEW};
    enum RecombinationType {RT_UNIFORM, RT_PATHS};

    Gridbrain(lua_State* luaState=NULL);
    virtual ~Gridbrain();

    Gridbrain* clone(bool grow=true, ExpansionType expansion=ET_NONE, unsigned int targetGrid=0, Coord* gc=NULL);

    void addGrid(Grid* grid, string name);
    void init();

    float* getInputBuffer(unsigned int channel, unsigned int id=0);
    float* getOutputBuffer();

    Component* getComponent(unsigned int x,
                unsigned int y,
                unsigned int gridNumber);
    Component* getComponent(unsigned int pos){return mComponents[pos];}
    void addConnection(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg,
                GeneTag tag=GeneTag());
    void removeConnection(Connection* conn);
    void removeConnection(unsigned int xOrig,
                unsigned int yOrig,
                unsigned int gOrig,
                unsigned int xTarg,
                unsigned int yTarg,
                unsigned int gTarg);
    Connection* getRandomConnection();
    Connection* getConnection(unsigned int xOrig,
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

    void mutate(float factor=1.0f);
    Gridbrain* recombine(Gridbrain* brain);

    Component* setComponent(unsigned int x, unsigned int y, unsigned int g, Component& comp);

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

    bool getFieldValue(string fieldName, float& value);

    string write();
    void printDebug();
    void printConnection(Connection* conn);

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

    void setAllActive(bool active){mAllActive = active;}

    void update();

    float getDistance(Gridbrain* brain);

    void generateGenes(vector<Gridbrain*>* brainVec);

    int getChannelByName(string name);
    string getChannelName(int chan);
    list<Component*>* getInputInterface(unsigned int channel);
    list<Component*>* getOutputInterface();

    unsigned int getNumberOfComponents(){return mNumberOfComponents;}
    unsigned int getGridsCount(){return mGridsCount;}

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
    Connection* nextRandomConnection();
    Connection* skipNextRandomConnection();
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
                                Connection* conn);

    bool checkGene(llULINT geneID);
    bool isGeneSelected(llULINT geneID);
    bool selectGene(llULINT geneID, bool select);

    int compEquivalence(Component* comp1, Component* comp2, CompEquivalenceType eqType);
    GeneTag findGeneTag(Connection* conn);

    void clearInterfaces();

    static mt_distribution* mDistConnections;
    static mt_distribution* mDistMutationsProb;
    static mt_distribution* mDistComponents;
    static mt_distribution* mDistRecombine;
    static mt_distribution* mDistGridbrain;

    vector<Grid*> mGridsVec;

    vector<Component*> mComponents;
    unsigned int mMaxInputDepth;
    unsigned int mNumberOfComponents;
    Connection* mConnections;
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
    Connection* mConnSeqCurrent;
    float mCompSeqProb;
    unsigned int mCompSeqPos;

    bool mAllActive;
    unsigned int mActiveConnections;
    unsigned int mActiveComponents;
    unsigned int mActivePerceptions;
    unsigned int mActiveActions;

    bool mGeneGrouping;

    RecombinationType mRecombinationType;

    vector<list<Component*>*> mInputInterfacesVector;
    list<Component*> mOutputInterface;
    map<string, int> mChannels;
};

#endif

