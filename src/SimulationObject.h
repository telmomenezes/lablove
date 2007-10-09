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

#if !defined(__INCLUDE_SIMULATION_OBJECT_H)
#define __INCLUDE_SIMULATION_OBJECT_H

#include "SymbolTable.h"
#include "Symbol.h"
#include "Graphic.h"
#include "SymbolPointer.h"

#include "Orbit.h"

#include "pyc.h"

#include <map>
#include <string>
#include <list>

using std::map;
using std::string;
using std::list;

class SimulationObject
{
public:
    enum Type {TYPE_OBJECT,
            TYPE_AGENT};

    static unsigned long CURRENT_ID;

    SimulationObject(lua_State* luaState=NULL);
    SimulationObject(SimulationObject* obj);
    virtual ~SimulationObject();
    virtual SimulationObject* clone();

    void initFloatData(unsigned int size);
    void initBoolData(unsigned int size);
    void initIntData(unsigned int size);
    void initUnsignedLongData(unsigned int size);

    virtual void initRandom(){}

    unsigned long getID(){return mID;}

    void draw(pyc::Layer* layer);

    virtual void setSize(float size);
    float getSize(){return mSize;}

    unsigned int getSpeciesID(){return mSpeciesID;}
    void setSpeciesID(unsigned int id){mSpeciesID = id;}
    float getEnergy(){return mEnergy;}
    float getInitialEnergy(){return mInitialEnergy;}
    void deltaEnergy(double delta){mEnergy += delta;}
    void setEnergy(float energy){mEnergy = energy;}
    void setInitialEnergy(float energy){mInitialEnergy = energy;}

    unsigned long getCreationTime(){return mCreationTime;}
    void setCreationTime(unsigned long time){mCreationTime = time;}

    virtual SimulationObject* crossover(SimulationObject* otherParent){return this;}
    virtual void mutate(){}

    void setAgeRange(unsigned long lowAgeLimit, unsigned long highAgeLimit);
    void setMetabolism(float metabolism){mMetabolism = metabolism;}

    void addSymbolTable(SymbolTable* table);
    SymbolTable* getSymbolTable(int id);

    void setSymbolName(string name, int table, unsigned int pos);
    Symbol* getSymbolByName(string name);

    void addGraphic(Graphic* graph);

    virtual float getFieldValue(string fieldName);

    bool mDeleted;

    float mX;
    float mY;
    float mZ;

    float mRotX;
    float mRotY;
    float mRotZ;

    float mSize;
    float mSizeSquared;

    Type mType;

    float mEnergy;
    unsigned long mMaxAge;
    float mMetabolism;
    unsigned long mCreationTime;

    unsigned long mCollisionDetectionIteration;

    bool mInitialized;

    float* mFloatData;
    bool* mBoolData;
    int* mIntData;
    unsigned long* mUnsignedLongData;
    unsigned int mFloatDataSize;
    unsigned int mBoolDataSize;
    unsigned int mIntDataSize;
    unsigned int mUnsignedLongDataSize;

    static const char mClassName[];
    static Orbit<SimulationObject>::MethodType mMethods[];
    static Orbit<SimulationObject>::NumberGlobalType mNumberGlobals[];

    int setInitialEnergy(lua_State* luaState);
    int addSymbolTable(lua_State* luaState);
    int setSize(lua_State* luaState);
    int setAgeRange(lua_State* luaState);
    int setMetabolism(lua_State* luaState);
    int addGraphic(lua_State* luaState);
    int setSymbolName(lua_State* luaState);

protected:
    unsigned long mID;

    map<int, SymbolTable*> mSymbolTables;
    map<string, SymbolPointer> mNamedSymbols;

    unsigned int mSpeciesID;
    float mInitialEnergy;
    unsigned long mLowAgeLimit;
    unsigned long mHighAgeLimit;
    
    list<Graphic*> mGraphics;
};
#endif

