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

#if !defined(__INCLUDE_SIM_OBJ_H)
#define __INCLUDE_SIM_OBJ_H

#include "TableSet.h"
#include "Gridbrain.h"
#include "Message.h"
#include "types.h"
#include "Fitness.h"

#include "Orbit.h"

#include <map>
#include <string>
#include <list>
#include <vector>

using std::map;
using std::string;
using std::list;
using std::vector;

using namespace gb;

class SimObj
{
public:
    enum Type {TYPE_OBJECT, TYPE_AGENT};
    enum DeathType {DEATH_HARD, DEATH_EXPIRED};

    static llULINT CURRENT_ID;

    SimObj(lua_State* luaState=NULL);
    SimObj(SimObj* obj);
    virtual ~SimObj();
    virtual SimObj* clone();

    virtual void init(){}

    llULINT getID(){return mID;}

    virtual void process(){}
    virtual void perceive(){}
    virtual void act(){}

    virtual void onAdd();

    virtual void draw(){}

    unsigned int getSpeciesID(){return mSpeciesID;}
    void setSpeciesID(unsigned int id){mSpeciesID = id;}
    int getBodyID(){return mBodyID;}
    void setBodyID(int id){mBodyID = id;}

    llULINT getCreationTime(){return mCreationTime;}
    void setCreationTime(llULINT time){mCreationTime = time;}

    virtual SimObj* recombine(SimObj* otherParent);
    virtual void mutate(float factor=1.0f);

    void setBrain(Gridbrain* brain);
    Gridbrain* getBrain(){return mBrain;}

    virtual void compute();

    void addSymbolTable(SymbolTable* table);
    SymbolTable* getSymbolTable(int id);
    SymbolTable* getSymbolTableByName(string name);

    void setSymbolName(string name, int table, llULINT id);
    Symbol* getSymbolByName(string name);
    string getSymbolName(int table, llULINT id);
    string getTableName(int table);

    void setFloatDataFromSymbol(string symbolName, float& var);
    void setULDataFromSymbol(string symbolName, llULINT& var);

    virtual bool getFieldValue(string fieldName, float& value);

    void setBirthRadius(float rad){mBirthRadius = rad;}
    float getBirthRadius(){return mBirthRadius;}
    void setKeepBodyOnHardDeath(bool val){mKeepBodyOnHardDeath = val;}
    bool getKeepBodyOnHardDeath(){return mKeepBodyOnHardDeath;}
    void setKeepBodyOnExpirationDeath(bool val){mKeepBodyOnExpirationDeath = val;}
    bool getKeepBodyOnExpirationDeath(){return mKeepBodyOnExpirationDeath;}
    
    void addFitness(int type);
    Fitness* getFitness(int type){return &mFitMap[type];}
    Fitness* getFitnessByIndex(unsigned int index);
    void clearFitnesses();
    virtual void updateFitnesses(){}
    
    virtual void addMessage(Message* msg){mMessageList.push_back(msg);}
    list<Message*>* getMessageList(){return &mMessageList;}
    void emptyMessageList();

    virtual void printDebug();

    void repairBrain();
    bool symbolUsed(int tableID, llULINT symbolID);
    void markUsedSymbols(TableSet* tab);

    void setDeathType(DeathType type){mDeathType = type;}
    DeathType getDeathType(){return mDeathType;}

    bool mDeleted;

    Type mType;

    llULINT mCreationTime;
    bool mInitialized;

    int mBodyID;

    TableSet* mTableSet;

    static const char mClassName[];
    static Orbit<SimObj>::MethodType mMethods[];
    static Orbit<SimObj>::NumberGlobalType mNumberGlobals[];

    int addSymbolTable(lua_State* luaState);
    int setSize(lua_State* luaState);
    int setSymbolName(lua_State* luaState);
    int setBirthRadius(lua_State* luaState);
    int setBrain(lua_State* luaState);
    int setKeepBodyOnHardDeath(lua_State* luaState);
    int setKeepBodyOnExpirationDeath(lua_State* luaState);

protected:
    virtual void recombine(SimObj* parent1, SimObj* parent2);
    void generateGridSets();

    llULINT mID;

    map<int, Fitness> mFitMap;

    unsigned int mSpeciesID;

    float mBirthRadius;
    bool mKeepBodyOnHardDeath;
    bool mKeepBodyOnExpirationDeath;

    Gridbrain* mBrain;
    list<Message*> mMessageList;

    DeathType mDeathType;
};
#endif

