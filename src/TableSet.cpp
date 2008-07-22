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

#include "TableSet.h"

TableSet::TableSet()
{
}

TableSet::TableSet(TableSet* ts)
{
    map<string, SymbolPointer>::iterator iterSymPointers;
    for (iterSymPointers = ts->mNamedSymbols.begin();
        iterSymPointers != ts->mNamedSymbols.end();
        iterSymPointers++)
    {
        mNamedSymbols[(*iterSymPointers).first] = (*iterSymPointers).second;
    }

    map<int, SymbolTable*>::iterator iterTables;
    for (iterTables = ts->mSymbolTables.begin();
        iterTables != ts->mSymbolTables.end();
        iterTables++)
    {
        mSymbolTables[(*iterTables).first] = new SymbolTable((*iterTables).second);
    }
}

TableSet::~TableSet()
{
    map<int, SymbolTable*>::iterator iterTables;

    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        delete (*iterTables).second;
    }

    mSymbolTables.clear();

    mNamedSymbols.clear();
}

void TableSet::cleanAndGrow()
{
    map<int, SymbolTable*>::iterator iterTables;
    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        SymbolTable* table = (*iterTables).second;

        if (table->isDynamic())
        {
            map<gbULINT, Symbol*>::iterator iterSymbol = table->getSymbolMap()->begin();
            while (iterSymbol != table->getSymbolMap()->end())
            {
                gbULINT symbolID = (*iterSymbol).first;

                Symbol* sym = (*iterSymbol).second;
                iterSymbol++;

                if ((!sym->mFixed) && (!sym->mUsed) && (!sym->mProtected))
                {
                    table->getSymbolMap()->erase(symbolID);
                    delete sym;
                }
            }

            table->grow();
        }
    }
}

void TableSet::addSymbolTable(SymbolTable* table)
{
    mSymbolTables[table->getID()] = table;
}

SymbolTable* TableSet::getSymbolTable(int id)
{
    if (mSymbolTables.count(id) == 0)
    {
        return NULL;
    }
    return mSymbolTables[id];
}

SymbolTable* TableSet::getSymbolTableByName(string name)
{
    map<int, SymbolTable*>::iterator iterTables;

    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        if ((*iterTables).second->getName() == name)
        {
            return (*iterTables).second;
        }
    }

    return NULL;
}

void TableSet::setSymbolName(string name, int table, gbULINT id)
{
    SymbolPointer sp;
    sp.mTable = table;
    sp.mID = id;
    mNamedSymbols[name] = sp;
}

Symbol* TableSet::getSymbolByName(string name)
{
    if (mNamedSymbols.count(name) == 0)
    {
        return NULL;
    }
    
    SymbolPointer sp = mNamedSymbols[name];
    SymbolTable* symTab = getSymbolTable(sp.mTable);

    if (symTab == NULL)
    {
        return NULL;
    }

    return symTab->getSymbol(sp.mID);
}

string TableSet::getSymbolName(int table, gbULINT id)
{
    for (map<string, SymbolPointer>::iterator iterName = mNamedSymbols.begin();
            iterName != mNamedSymbols.end();
            iterName++)
    {
        if (((*iterName).second.mTable == table)
            && ((*iterName).second.mID == id))
        {
            return (*iterName).first;
        }
    }

    return "?";
}

string TableSet::getTableName(int table)
{
    SymbolTable* t = getSymbolTable(table);

    if (t == NULL)
    {
        return "?";
    }

    if (t->getName() != "")
    {
        return t->getName();
    }

    return "?";
}

void TableSet::recombine(TableSet* parent1, TableSet* parent2)
{
    map<int, SymbolTable*>::iterator iterTables;

    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        delete (*iterTables).second;
    }

    mSymbolTables.clear();

    for (iterTables = parent1->mSymbolTables.begin();
        iterTables != parent1->mSymbolTables.end();
        iterTables++)
    {
        int tableID = (*iterTables).first;
        SymbolTable* table1 = (*iterTables).second;
        SymbolTable* table2 = parent2->getSymbolTable(tableID);

        mSymbolTables[tableID] = table1->recombine(table2);
    }
}

void TableSet::resetProtections()
{
    map<int, SymbolTable*>::iterator iterTables;
    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        SymbolTable* table = (*iterTables).second;

        if (table->isDynamic())
        {
            table->resetProtections();
        }
    }
}

void TableSet::printDebug()
{
    map<int, SymbolTable*>::iterator iterTables;
    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        ((*iterTables).second)->printDebug();
    }
}

bool TableSet::symbolExists(int table, gbULINT sym)
{
    SymbolTable* symTab = getSymbolTable(table);

    if (symTab == NULL)
    {
        return false;
    }

    if (symTab->getSymbol(sym) == NULL)
    {
        return false;
    }

    return true;
}

