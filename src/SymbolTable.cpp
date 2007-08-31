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

#include "SymbolTable.h"

SymbolTable::SymbolTable(Symbol* refSymbol)
{
	mReferenceSymbol = refSymbol;
}

SymbolTable::SymbolTable(lua_State* luaState)
{
	Symbol* symbol = (Symbol*)Orbit<SymbolTable>::pointer(luaState, 1);

	mReferenceSymbol = symbol;
}

SymbolTable::SymbolTable(SymbolTable* table)
{
	mReferenceSymbol = table->mReferenceSymbol->clone();

	std::vector<Symbol*>::iterator iterSymbol;

	for (iterSymbol = table->mSymbols.begin();
		iterSymbol != table->mSymbols.end();
		iterSymbol++)
	{
		mSymbols.push_back((*iterSymbol)->clone());
	}
}

SymbolTable::~SymbolTable()
{
	std::vector<Symbol*>::iterator iterSymbol;

	for (iterSymbol = mSymbols.begin();
		iterSymbol != mSymbols.end();
		iterSymbol++)
	{
		delete (*iterSymbol);
	}

	mSymbols.clear();
}

Symbol* SymbolTable::getSymbol(unsigned int index)
{
	return mSymbols[index];
}

unsigned int SymbolTable::addSymbol(Symbol* sym)
{
	unsigned int pos = mSymbols.size();
	mSymbols.push_back(sym);
	return pos;
}

void SymbolTable::mutate()
{
	unsigned int index = random() % mSymbols.size();
	mSymbols[index]->mutate();
}

int SymbolTable::addSymbol(lua_State* luaState)
{
	Symbol* sym = (Symbol*)Orbit<SymbolTable>::pointer(luaState, 1);
	unsigned int index = addSymbol(sym);
	lua_pushnumber(luaState, index);
	return 1;
}

const char SymbolTable::mClassName[] = "SymbolTable";

Orbit<SymbolTable>::MethodType SymbolTable::mMethods[] = {
	{"addSymbol", &SymbolTable::addSymbol},
        {0,0}
};

Orbit<SymbolTable>::NumberGlobalType SymbolTable::mNumberGlobals[] = {{0,0}};

