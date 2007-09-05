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
#include "Graphic.h"
#include "SymbolRGB.h"

#include "Orbit.h"

#include <map>
#include <string>

class SimulationObject
{
public:
	enum Type {TYPE_OBJECT,
			TYPE_PLANT,
			TYPE_AGENT};

	static unsigned long CURRENT_ID;

	SimulationObject();
	SimulationObject(SimulationObject* obj);
	virtual ~SimulationObject();

	virtual SimulationObject* clone(bool full=true) = 0;
	virtual void initRandom(){}

	unsigned long getID(){return mID;}

	virtual void onCycle();
	void draw();
	virtual bool isFood(){return false;}
	void setSelected(bool selected){mSelected = selected;}
	bool isSelected(){return mSelected;}

	virtual void setPos(float x, float y);
	virtual void setSize(float size);
	virtual void setRot(float rot);
	float getX(){return mX;}
	float getY(){return mY;}
	float getSize(){return mSize;}
	float getRot(){return mRot;}
	int getCellX(){return mCellX;}
	int getCellY(){return mCellY;}
	int getCellPos(){return mCellPos;}

        unsigned int getSpeciesID(){return mSpeciesID;}
	void setSpeciesID(unsigned int id){mSpeciesID = id;}
	float getEnergy(){return mEnergy;}
	float getInitialEnergy(){return mInitialEnergy;}
	void deltaEnergy(double delta){mEnergy += delta;}
	void setEnergy(float energy){mEnergy = energy;}
	void setInitialEnergy(float energy){mInitialEnergy = energy;}

	virtual SimulationObject* crossover(SimulationObject* otherParent){return this;}
	virtual void mutate(){}

	virtual void placeRandom();

	void setAgeRange(unsigned long lowAgeLimit, unsigned long highAgeLimit);
	void setMetabolism(float metabolism){mMetabolism = metabolism;}

	void setColor(SymbolRGB* color);
	SymbolRGB* getColor(){return &mColor;}

	void addSymbolTable(SymbolTable* table, unsigned int code);
	SymbolTable* getSymbolTable(unsigned int pos);

	void setGraphic(Graphic* graph);

	int setInitialEnergy(lua_State* luaState);
	int addSymbolTable(lua_State* luaState);
	int setPos(lua_State* luaState);
        int setSize(lua_State* luaState);
        int setRot(lua_State* luaState);
	int setAgeRange(lua_State* luaState);
	int setMetabolism(lua_State* luaState);
	int setColor(lua_State* luaState);
	int setGraphic(lua_State* luaState);

	virtual float getFieldValue(std::string fieldName);

	bool mHuman;
	bool mDeleted;

	float mX;
	float mY;
	float mSize;
	float mSizeSquared;
	float mRot;

	SimulationObject* mNextCellList;
	SimulationObject* mPrevCellList;

	int mCellX;
	int mCellY;
	int mCellPos;

	Type mType;

	float mEnergy;

protected:
	unsigned long mID;
	bool mSelected;
        unsigned int mSpeciesID;
	float mInitialEnergy;
	unsigned long mCreationTime;
	std::map<unsigned int, SymbolTable*> mSymbolTables;

	unsigned long mLowAgeLimit;
	unsigned long mHighAgeLimit;
	unsigned long mMaxAge;
	float mMetabolism;
	SymbolRGB mColor;

	Graphic* mGraphic;
};
#endif

