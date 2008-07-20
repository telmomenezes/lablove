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

#ifndef __INCLUDE_GRIDBRAIN_LUA_H
#define __INCLUDE_GRIDBRAIN_LUA_H

#include "Orbit.h"
#include "comps.h"

class LuaCompNUL : public CompNUL
{
public:
    LuaCompNUL(lua_State* luaState=NULL){}
    virtual ~LuaCompNUL(){}

    static const char mClassName[];
    static Orbit<LuaCompNUL>::MethodType mMethods[];
    static Orbit<LuaCompNUL>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompNUL::mClassName[] = "NUL";
Orbit<LuaCompNUL>::MethodType LuaCompNUL::mMethods[] = {{0,0}};
Orbit<LuaCompNUL>::NumberGlobalType LuaCompNUL::mNumberGlobals[] = {{0,0}};


class LuaCompIN : public CompIN
{
public:
    LuaCompIN(lua_State* luaState=NULL){}
    virtual ~LuaCompIN(){}

    static const char mClassName[];
    static Orbit<LuaCompIN>::MethodType mMethods[];
    static Orbit<LuaCompIN>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompIN::mClassName[] = "IN";
Orbit<LuaCompIN>::MethodType LuaCompIN::mMethods[] = {{0,0}};
Orbit<LuaCompIN>::NumberGlobalType LuaCompIN::mNumberGlobals[] = {{0,0}};


class LuaCompOUT : public CompOUT
{
public:
    LuaCompOUT(lua_State* luaState=NULL){}
    virtual ~LuaCompOUT(){}

    static const char mClassName[];
    static Orbit<LuaCompOUT>::MethodType mMethods[];
    static Orbit<LuaCompOUT>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompOUT::mClassName[] = "OUT";
Orbit<LuaCompOUT>::MethodType LuaCompOUT::mMethods[] = {{0,0}};
Orbit<LuaCompOUT>::NumberGlobalType LuaCompOUT::mNumberGlobals[] = {{0,0}};


class LuaCompSUM : public CompSUM
{
public:
    LuaCompSUM(lua_State* luaState=NULL){}
    virtual ~LuaCompSUM(){}

    static const char mClassName[];
    static Orbit<LuaCompSUM>::MethodType mMethods[];
    static Orbit<LuaCompSUM>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompSUM::mClassName[] = "SUM";
Orbit<LuaCompSUM>::MethodType LuaCompSUM::mMethods[] = {{0,0}};
Orbit<LuaCompSUM>::NumberGlobalType LuaCompSUM::mNumberGlobals[] = {{0,0}};

class LuaCompMAX : public CompMAX
{
public:
    LuaCompMAX(lua_State* luaState=NULL){}
    virtual ~LuaCompMAX(){}

    static const char mClassName[];
    static Orbit<LuaCompMAX>::MethodType mMethods[];
    static Orbit<LuaCompMAX>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompMAX::mClassName[] = "MAX";
Orbit<LuaCompMAX>::MethodType LuaCompMAX::mMethods[] = {{0,0}};
Orbit<LuaCompMAX>::NumberGlobalType LuaCompMAX::mNumberGlobals[] = {{0,0}};

class LuaCompMIN : public CompMIN
{
public:
    LuaCompMIN(lua_State* luaState=NULL){}
    virtual ~LuaCompMIN(){}

    static const char mClassName[];
    static Orbit<LuaCompMIN>::MethodType mMethods[];
    static Orbit<LuaCompMIN>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompMIN::mClassName[] = "MIN";
Orbit<LuaCompMIN>::MethodType LuaCompMIN::mMethods[] = {{0,0}};
Orbit<LuaCompMIN>::NumberGlobalType LuaCompMIN::mNumberGlobals[] = {{0,0}};

class LuaCompAVG : public CompAVG
{
public:
    LuaCompAVG(lua_State* luaState=NULL){}
    virtual ~LuaCompAVG(){}

    static const char mClassName[];
    static Orbit<LuaCompAVG>::MethodType mMethods[];
    static Orbit<LuaCompAVG>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompAVG::mClassName[] = "AVG";
Orbit<LuaCompAVG>::MethodType LuaCompAVG::mMethods[] = {{0,0}};
Orbit<LuaCompAVG>::NumberGlobalType LuaCompAVG::mNumberGlobals[] = {{0,0}};

class LuaCompMUL : public CompMUL
{
public:
    LuaCompMUL(lua_State* luaState=NULL){}
    virtual ~LuaCompMUL(){}

    static const char mClassName[];
    static Orbit<LuaCompMUL>::MethodType mMethods[];
    static Orbit<LuaCompMUL>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompMUL::mClassName[] = "MUL";
Orbit<LuaCompMUL>::MethodType LuaCompMUL::mMethods[] = {{0,0}};
Orbit<LuaCompMUL>::NumberGlobalType LuaCompMUL::mNumberGlobals[] = {{0,0}};

class LuaCompNOT : public CompNOT
{
public:
    LuaCompNOT(lua_State* luaState=NULL){}
    virtual ~LuaCompNOT(){}

    static const char mClassName[];
    static Orbit<LuaCompNOT>::MethodType mMethods[];
    static Orbit<LuaCompNOT>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompNOT::mClassName[] = "NOT";
Orbit<LuaCompNOT>::MethodType LuaCompNOT::mMethods[] = {{0,0}};
Orbit<LuaCompNOT>::NumberGlobalType LuaCompNOT::mNumberGlobals[] = {{0,0}};

class LuaCompAND : public CompAND
{
public:
    LuaCompAND(lua_State* luaState=NULL){}
    virtual ~LuaCompAND(){}

    static const char mClassName[];
    static Orbit<LuaCompAND>::MethodType mMethods[];
    static Orbit<LuaCompAND>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompAND::mClassName[] = "AND";
Orbit<LuaCompAND>::MethodType LuaCompAND::mMethods[] = {{0,0}};
Orbit<LuaCompAND>::NumberGlobalType LuaCompAND::mNumberGlobals[] = {{0,0}};

class LuaCompINV : public CompINV
{
public:
    LuaCompINV(lua_State* luaState=NULL){}
    virtual ~LuaCompINV(){}

    static const char mClassName[];
    static Orbit<LuaCompINV>::MethodType mMethods[];
    static Orbit<LuaCompINV>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompINV::mClassName[] = "INV";
Orbit<LuaCompINV>::MethodType LuaCompINV::mMethods[] = {{0,0}};
Orbit<LuaCompINV>::NumberGlobalType LuaCompINV::mNumberGlobals[] = {{0,0}};

class LuaCompNEG : public CompNEG
{
public:
    LuaCompNEG(lua_State* luaState=NULL){}
    virtual ~LuaCompNEG(){}

    static const char mClassName[];
    static Orbit<LuaCompNEG>::MethodType mMethods[];
    static Orbit<LuaCompNEG>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompNEG::mClassName[] = "NEG";
Orbit<LuaCompNEG>::MethodType LuaCompNEG::mMethods[] = {{0,0}};
Orbit<LuaCompNEG>::NumberGlobalType LuaCompNEG::mNumberGlobals[] = {{0,0}};

class LuaCompAMP : public CompAMP
{
public:
    LuaCompAMP(lua_State* luaState=NULL){}
    virtual ~LuaCompAMP(){}

    static const char mClassName[];
    static Orbit<LuaCompAMP>::MethodType mMethods[];
    static Orbit<LuaCompAMP>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompAMP::mClassName[] = "AMP";
Orbit<LuaCompAMP>::MethodType LuaCompAMP::mMethods[] = {{0,0}};
Orbit<LuaCompAMP>::NumberGlobalType LuaCompAMP::mNumberGlobals[] = {{0,0}};

class LuaCompMOD : public CompMOD
{
public:
    LuaCompMOD(lua_State* luaState=NULL){}
    virtual ~LuaCompMOD(){}

    static const char mClassName[];
    static Orbit<LuaCompMOD>::MethodType mMethods[];
    static Orbit<LuaCompMOD>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompMOD::mClassName[] = "MOD";
Orbit<LuaCompMOD>::MethodType LuaCompMOD::mMethods[] = {{0,0}};
Orbit<LuaCompMOD>::NumberGlobalType LuaCompMOD::mNumberGlobals[] = {{0,0}};

class LuaCompRAND : public CompRAND
{
public:
    LuaCompRAND(lua_State* luaState=NULL){}
    virtual ~LuaCompRAND(){}

    static const char mClassName[];
    static Orbit<LuaCompRAND>::MethodType mMethods[];
    static Orbit<LuaCompRAND>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompRAND::mClassName[] = "RAND";
Orbit<LuaCompRAND>::MethodType LuaCompRAND::mMethods[] = {{0,0}};
Orbit<LuaCompRAND>::NumberGlobalType LuaCompRAND::mNumberGlobals[] = {{0,0}};

class LuaCompEQ : public CompEQ
{
public:
    LuaCompEQ(lua_State* luaState=NULL){}
    virtual ~LuaCompEQ(){}

    static const char mClassName[];
    static Orbit<LuaCompEQ>::MethodType mMethods[];
    static Orbit<LuaCompEQ>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompEQ::mClassName[] = "EQ";
Orbit<LuaCompEQ>::MethodType LuaCompEQ::mMethods[] = {{0,0}};
Orbit<LuaCompEQ>::NumberGlobalType LuaCompEQ::mNumberGlobals[] = {{0,0}};

class LuaCompGTZ : public CompGTZ
{
public:
    LuaCompGTZ(lua_State* luaState=NULL){}
    virtual ~LuaCompGTZ(){}

    static const char mClassName[];
    static Orbit<LuaCompGTZ>::MethodType mMethods[];
    static Orbit<LuaCompGTZ>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompGTZ::mClassName[] = "GTZ";
Orbit<LuaCompGTZ>::MethodType LuaCompGTZ::mMethods[] = {{0,0}};
Orbit<LuaCompGTZ>::NumberGlobalType LuaCompGTZ::mNumberGlobals[] = {{0,0}};

class LuaCompZERO : public CompZERO
{
public:
    LuaCompZERO(lua_State* luaState=NULL){}
    virtual ~LuaCompZERO(){}

    static const char mClassName[];
    static Orbit<LuaCompZERO>::MethodType mMethods[];
    static Orbit<LuaCompZERO>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompZERO::mClassName[] = "ZERO";
Orbit<LuaCompZERO>::MethodType LuaCompZERO::mMethods[] = {{0,0}};
Orbit<LuaCompZERO>::NumberGlobalType LuaCompZERO::mNumberGlobals[] = {{0,0}};

class LuaCompCLK : public CompCLK
{
public:
    LuaCompCLK(lua_State* luaState=NULL){}
    virtual ~LuaCompCLK(){}

    static const char mClassName[];
    static Orbit<LuaCompCLK>::MethodType mMethods[];
    static Orbit<LuaCompCLK>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompCLK::mClassName[] = "CLK";
Orbit<LuaCompCLK>::MethodType LuaCompCLK::mMethods[] = {{0,0}};
Orbit<LuaCompCLK>::NumberGlobalType LuaCompCLK::mNumberGlobals[] = {{0,0}};

class LuaCompDMUL : public CompDMUL
{
public:
    LuaCompDMUL(lua_State* luaState=NULL){}
    virtual ~LuaCompDMUL(){}

    static const char mClassName[];
    static Orbit<LuaCompDMUL>::MethodType mMethods[];
    static Orbit<LuaCompDMUL>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompDMUL::mClassName[] = "DMUL";
Orbit<LuaCompDMUL>::MethodType LuaCompDMUL::mMethods[] = {{0,0}};
Orbit<LuaCompDMUL>::NumberGlobalType LuaCompDMUL::mNumberGlobals[] = {{0,0}};

class LuaCompSEL : public CompSEL
{
public:
    LuaCompSEL(lua_State* luaState=NULL){}
    virtual ~LuaCompSEL(){}

    static const char mClassName[];
    static Orbit<LuaCompSEL>::MethodType mMethods[];
    static Orbit<LuaCompSEL>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompSEL::mClassName[] = "SEL";
Orbit<LuaCompSEL>::MethodType LuaCompSEL::mMethods[] = {{0,0}};
Orbit<LuaCompSEL>::NumberGlobalType LuaCompSEL::mNumberGlobals[] = {{0,0}};



void gridbrainLuaRegister(lua_State* luaState)
{
    Orbit<LuaCompNUL>::orbitRegister(luaState);
    Orbit<LuaCompIN>::orbitRegister(luaState);
    Orbit<LuaCompOUT>::orbitRegister(luaState);
    Orbit<LuaCompSUM>::orbitRegister(luaState);
    Orbit<LuaCompMAX>::orbitRegister(luaState);
    Orbit<LuaCompMIN>::orbitRegister(luaState);
    Orbit<LuaCompAVG>::orbitRegister(luaState);
    Orbit<LuaCompMUL>::orbitRegister(luaState);
    Orbit<LuaCompNOT>::orbitRegister(luaState);
    Orbit<LuaCompAND>::orbitRegister(luaState);
    Orbit<LuaCompINV>::orbitRegister(luaState);
    Orbit<LuaCompNEG>::orbitRegister(luaState);
    Orbit<LuaCompAMP>::orbitRegister(luaState);
    Orbit<LuaCompMOD>::orbitRegister(luaState);
    Orbit<LuaCompRAND>::orbitRegister(luaState);
    Orbit<LuaCompEQ>::orbitRegister(luaState);
    Orbit<LuaCompGTZ>::orbitRegister(luaState);
    Orbit<LuaCompZERO>::orbitRegister(luaState);
    Orbit<LuaCompCLK>::orbitRegister(luaState);
    Orbit<LuaCompDMUL>::orbitRegister(luaState);
    Orbit<LuaCompSEL>::orbitRegister(luaState);
}
#endif

