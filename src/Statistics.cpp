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

#include "Statistics.h"

Statistics::Statistics()
{
	mFile = stdout;
}

Statistics::~Statistics()
{
	fclose(mFile);
}


void Statistics::addField(std::string fieldName)
{
	mFields.push_back(fieldName);
}

void Statistics::setFile(std::string filePath)
{
	mFile = fopen(filePath.c_str(), "w");
}

int Statistics::addField(lua_State* luaState)
{
        std::string fieldName = luaL_checkstring(luaState, 1);
        addField(fieldName);

        return 0;
}

int Statistics::setFile(lua_State* luaState)
{
        std::string filePath = luaL_checkstring(luaState, 1);
	setFile(filePath);

	return 0;
}

