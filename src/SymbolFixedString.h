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

#if !defined(__INCLUDE_SYMBOL_FIXED_STRING_H)
#define __INCLUDE_SYMBOL_FIXED_STRING_H

#include "Symbol.h"
#include "Orbit.h"
#include "RandDistManager.h"

#include <string>

using std::string;

class SymbolFixedString : public Symbol
{
public:
    SymbolFixedString(lua_State* luaState=NULL);
    SymbolFixedString(string alphabet, string str);
    SymbolFixedString(SymbolFixedString* sym);
    virtual ~SymbolFixedString();
    virtual Symbol* clone();

    virtual float getBinding(Symbol* sym);
    virtual void initRandom();
    virtual void mutate();

    string getString(){return mString;}
    string getAlphabet(){return mAlphabet;}
    unsigned int getLength(){return mLength;}
    unsigned int getAlphabetLength(){return mAlphabetLength;}

    void replaceChar(unsigned int replacePos, unsigned int charIndex);

    static const char mClassName[];
    static Orbit<SymbolFixedString>::MethodType mMethods[];
    static Orbit<SymbolFixedString>::NumberGlobalType mNumberGlobals[];

protected:
    static mt_distribution* mDistFixedString;

    string mString;
    string mAlphabet;
    unsigned int mLength;
    unsigned int mAlphabetLength;
};
#endif

