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

#if !defined(__INCLUDE_TABLE_SET_H)
#define __INCLUDE_TABLE_SET_H

#include "SymbolTable.h"
#include "types.h"
#include "Symbol.h"
#include "SymbolPointer.h"

#include <map>
#include <string>

using std::map;
using std::string;

class TableSet
{
public:
    TableSet();
    TableSet(TableSet* ts);
    virtual ~TableSet();

    void cleanAndGrow();
    void addSymbolTable(SymbolTable* table);
    SymbolTable* getSymbolTable(int id);
    SymbolTable* getSymbolTableByName(string name);
    void setSymbolName(string name, int table, llULINT id);
    Symbol* getSymbolByName(string name);
    string getSymbolName(int table, llULINT id);
    string getTableName(int table);
    void recombine(TableSet* parent1, TableSet* parent2);
    void resetProtections();
    void printDebug();
    bool symbolExists(int table, llULINT sym);

    map<int, SymbolTable*> mSymbolTables;
    map<string, SymbolPointer> mNamedSymbols;
};
#endif

