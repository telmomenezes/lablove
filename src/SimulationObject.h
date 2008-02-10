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

#if !defined(__INCLUDE_SIMULATION_OBJECT_H)
#define __INCLUDE_SIMULATION_OBJECT_H

#include "SymbolTable.h"
#include "Symbol.h"
#include "SymbolPointer.h"

#include "Orbit.h"

#include "art.h"

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
            TYPE_GRAPHICAL_OBJECT,
            TYPE_AGENT};

    static unsigned long CURRENT_ID;

    SimulationObject(lua_State* luaState=NULL);
    SimulationObject(SimulationObject* obj);
    virtual ~SimulationObject();
    virtual SimulationObject* clone(bool randomize=false);

    virtual void onAdd(){}

    void initFloatData(unsigned int size);
    void initBoolData(unsigned int size);
    void initIntData(unsigned int size);
    void initULData(unsigned int size);

    unsigned long getID(){return mID;}

    virtual void draw(){}

    unsigned int getSpeciesID(){return mSpeciesID;}
    unsigned int getSubSpeciesID(){return mSubSpeciesID;}
    void setSpeciesID(unsigned int id){mSpeciesID = id;}
    void setSubSpeciesID(unsigned int id){mSubSpeciesID = id;}

    unsigned long getCreationTime(){return mCreationTime;}
    void setCreationTime(unsigned long time){mCreationTime = time;}

    virtual SimulationObject* recombine(SimulationObject* otherParent);
    virtual void mutate();

    void addSymbolTable(SymbolTable* table);
    SymbolTable* getSymbolTable(int id);

    void setSymbolName(string name, int table, unsigned int pos);
    Symbol* getSymbolByName(string name);
    string getSymbolName(int table, unsigned int pos);

    void setFloatDataFromSymbol(string symbolName, unsigned int dataIndex);
    void setULDataFromSymbol(string symbolName, unsigned int dataIndex);

    virtual bool getFieldValue(string fieldName, float& value);

    void setNamedFloatDataIndex(string name, int index);
    void setNamedBoolDataIndex(string name, int index);
    void setNamedIntDataIndex(string name, int index);
    void getNamedULDataIndex(string name, int index);
    int getNamedFloatDataIndex(string name);
    int getNamedBoolDataIndex(string name);
    int getNamedIntDataIndex(string name);
    int getNamedULDataIndex(string name);

    void setBirthRadius(float rad){mBirthRadius = rad;}
    float getBirthRadius(){return mBirthRadius;}

    bool mDeleted;

    Type mType;

    unsigned long mCreationTime;
    bool mInitialized;
    bool mDataInitialized;

    float mFitness;

    float* mFloatData;
    bool* mBoolData;
    int* mIntData;
    unsigned long* mULData;
    unsigned int mFloatDataSize;
    unsigned int mBoolDataSize;
    unsigned int mIntDataSize;
    unsigned int mULDataSize;

    static const char mClassName[];
    static Orbit<SimulationObject>::MethodType mMethods[];
    static Orbit<SimulationObject>::NumberGlobalType mNumberGlobals[];

    int addSymbolTable(lua_State* luaState);
    int setSize(lua_State* luaState);
    int setSymbolName(lua_State* luaState);
    int setBirthRadius(lua_State* luaState);

protected:
    unsigned long mID;

    map<int, SymbolTable*> mSymbolTables;
    map<string, SymbolPointer> mNamedSymbols;
    map<string, int> mNamedFloatDataIndexes;
    map<string, int> mNamedBoolDataIndexes;
    map<string, int> mNamedIntDataIndexes;
    map<string, int> mNamedULDataIndexes;

    unsigned int mSpeciesID;
    unsigned int mSubSpeciesID;

    float mBirthRadius;
};
#endif

