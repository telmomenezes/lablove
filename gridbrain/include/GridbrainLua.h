/*
 * Gridbrain
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
#include "gb.h"

namespace gb
{

class LuaGridbrain : public Gridbrain
{
public:
    LuaGridbrain(lua_State* luaState){}
    virtual ~LuaGridbrain(){}

    static const char mClassName[];
    static Orbit<LuaGridbrain>::MethodType mMethods[];
    static Orbit<LuaGridbrain>::NumberGlobalType mNumberGlobals[];

    int init(lua_State* luaState);
    int setComponent(lua_State* luaState);
    int addGrid(lua_State* luaState);
    int addConnection(lua_State* luaState);
    int addRandomConnections(lua_State* luaState);
    int setMutateAddConnectionProb(lua_State* luaState);
    int setMutateRemoveConnectionProb(lua_State* luaState);
    int setMutateChangeParamProb(lua_State* luaState);
    int setMutateSplitConnectionProb(lua_State* luaState);
    int setMutateJoinConnectionsProb(lua_State* luaState);
    int setMutateChangeComponentProb(lua_State* luaState);
    int setMutateChangeInactiveComponentProb(lua_State* luaState);
    int setMutateSwapComponentProb(lua_State* luaState);
    int setParamMutationStanDev(lua_State* luaState);
    int setMaxInputDepth(lua_State* luaState);
};

const char LuaGridbrain::mClassName[] = "Gridbrain";

Orbit<LuaGridbrain>::MethodType LuaGridbrain::mMethods[] = {
    {"init", &LuaGridbrain::init},
    {"setComponent", &LuaGridbrain::setComponent},
    {"addGrid", &LuaGridbrain::addGrid},
    {"addConnection", &LuaGridbrain::addConnection},
    {"addRandomConnections", &LuaGridbrain::addRandomConnections},
    {"setMutateAddConnectionProb", &LuaGridbrain::setMutateAddConnectionProb},
    {"setMutateRemoveConnectionProb", &LuaGridbrain::setMutateRemoveConnectionProb},
    {"setMutateChangeParamProb", &LuaGridbrain::setMutateChangeParamProb},
    {"setParamMutationStanDev", &LuaGridbrain::setParamMutationStanDev},
    {"setMutateSplitConnectionProb", &LuaGridbrain::setMutateSplitConnectionProb},
    {"setMutateJoinConnectionsProb", &LuaGridbrain::setMutateJoinConnectionsProb},
    {"setMutateChangeComponentProb", &LuaGridbrain::setMutateChangeComponentProb},
    {"setMutateChangeInactiveComponentProb", &LuaGridbrain::setMutateChangeInactiveComponentProb},
    {"setMutateSwapComponentProb", &LuaGridbrain::setMutateSwapComponentProb},
    {"setMaxInputDepth", &LuaGridbrain::setMaxInputDepth},
    {0,0}
};

Orbit<LuaGridbrain>::NumberGlobalType LuaGridbrain::mNumberGlobals[] = {{0,0}};

int LuaGridbrain::init(lua_State* luaState)
{
    Gridbrain::init();
    return 0;
}

int LuaGridbrain::setComponent(lua_State* luaState)
{
    unsigned int x = luaL_checkint(luaState, 1);
    unsigned int y = luaL_checkint(luaState, 2);
    unsigned int g = luaL_checkint(luaState, 3);
    Component* comp = (Component*)(Orbit<LuaGridbrain>::pointer(luaState, 4));
    Gridbrain::setComponent(x, y, g, *comp);
    return 0;
}

int LuaGridbrain::addGrid(lua_State* luaState)
{
    Grid* grid = (Grid*)(Orbit<LuaGridbrain>::pointer(luaState, 1));
    string name = luaL_checkstring(luaState, 2);
    Gridbrain::addGrid(grid, name);
    return 0;
}

int LuaGridbrain::addConnection(lua_State* luaState)
{
    unsigned int xOrig = luaL_checkint(luaState, 1);
    unsigned int yOrig = luaL_checkint(luaState, 2);
    unsigned int gOrig = luaL_checkint(luaState, 3);
    unsigned int xTarg = luaL_checkint(luaState, 4);
    unsigned int yTarg = luaL_checkint(luaState, 5);
    unsigned int gTarg = luaL_checkint(luaState, 6);

    Gridbrain::addConnection(xOrig, yOrig, gOrig, xTarg, yTarg, gTarg);
    return 0;
}

int LuaGridbrain::addRandomConnections(lua_State* luaState)
{
    unsigned int count = luaL_checkint(luaState, 1);
    Gridbrain::addRandomConnections(count);
    return 0;
}

int LuaGridbrain::setMutateAddConnectionProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    Gridbrain::setMutateAddConnectionProb(prob);
    return 0;
}

int LuaGridbrain::setMutateRemoveConnectionProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    Gridbrain::setMutateRemoveConnectionProb(prob);
    return 0;
}

int LuaGridbrain::setMutateChangeParamProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    Gridbrain::setMutateChangeParamProb(prob);
    return 0;
}

int LuaGridbrain::setMutateSplitConnectionProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    Gridbrain::setMutateSplitConnectionProb(prob);
    return 0;
}

int LuaGridbrain::setMutateJoinConnectionsProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    Gridbrain::setMutateJoinConnectionsProb(prob);
    return 0;
}

int LuaGridbrain::setMutateChangeComponentProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    Gridbrain::setMutateChangeComponentProb(prob);
    return 0;
}

int LuaGridbrain::setMutateChangeInactiveComponentProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    Gridbrain::setMutateChangeInactiveComponentProb(prob);
    return 0;
}

int LuaGridbrain::setMutateSwapComponentProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    Gridbrain::setMutateSwapComponentProb(prob);
    return 0;
}

int LuaGridbrain::setParamMutationStanDev(lua_State* luaState)
{
    float sd = luaL_checknumber(luaState, 1);
    Gridbrain::setParamMutationStanDev(sd);
    return 0;
}

int LuaGridbrain::setMaxInputDepth(lua_State* luaState)
{
    unsigned int depth = luaL_checkint(luaState, 1);
    Gridbrain::setMaxInputDepth(depth);
    return 0;
}

class LuaGrid : public Grid
{
public:
    LuaGrid(lua_State* luaState){}
    virtual ~LuaGrid(){}

    static const char mClassName[];
    static Orbit<LuaGrid>::MethodType mMethods[];
    static Orbit<LuaGrid>::NumberGlobalType mNumberGlobals[];

    int setComponentSet(lua_State* luaState);
    int init(lua_State* luaState);
};

const char LuaGrid::mClassName[] = "Grid";

Orbit<LuaGrid>::MethodType LuaGrid::mMethods[] = {
    {"setComponentSet", &LuaGrid::setComponentSet},
    {"init", &LuaGrid::init},
    {0,0}
};

Orbit<LuaGrid>::NumberGlobalType LuaGrid::mNumberGlobals[] = {
    {"ALPHA", ALPHA},
    {"BETA", BETA},
    {0,0}
};

int LuaGrid::setComponentSet(lua_State* luaState)
{
    ComponentSet* set = (ComponentSet*)Orbit<LuaGrid>::pointer(luaState, 1);
    Grid::setComponentSet(set);
    return 0;
}

int LuaGrid::init(lua_State* luaState)
{
    Type type = (Type)luaL_checkint(luaState, 1);
    unsigned int width = luaL_checkint(luaState, 2);
    unsigned int height = luaL_checkint(luaState, 3);
    Grid::init(type, width, height);
    return 0;
}

class LuaComponentSet : public ComponentSet
{
public:
    LuaComponentSet(lua_State* luaState){}
    virtual ~LuaComponentSet(){}

    static const char mClassName[];
    static Orbit<LuaComponentSet>::MethodType mMethods[];
    static Orbit<LuaComponentSet>::NumberGlobalType mNumberGlobals[];

    int addComponent(lua_State* luaState);
};

const char LuaComponentSet::mClassName[] = "ComponentSet";

Orbit<LuaComponentSet>::MethodType LuaComponentSet::mMethods[] = {
    {"addComponent", &LuaComponentSet::addComponent},
    {0,0}
};

Orbit<LuaComponentSet>::NumberGlobalType LuaComponentSet::mNumberGlobals[] = {{0,0}};

int LuaComponentSet::addComponent(lua_State* luaState)
{
    Component* comp = (Component*)(Orbit<LuaComponentSet>::pointer(luaState, 1));
    ComponentSet::addComponent(comp);
    return 0;
}

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
    LuaCompAMP(lua_State* luaState=NULL)
    {
        float param = luaL_optnumber(luaState, 1, 0.0f);
        mParam = param;
    }
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
    LuaCompCLK(lua_State* luaState=NULL)
    {
        float param = luaL_optnumber(luaState, 1, 0.0f);
        mParam = param;
    }
    virtual ~LuaCompCLK(){}

    static const char mClassName[];
    static Orbit<LuaCompCLK>::MethodType mMethods[];
    static Orbit<LuaCompCLK>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompCLK::mClassName[] = "CLK";
Orbit<LuaCompCLK>::MethodType LuaCompCLK::mMethods[] = {{0,0}};
Orbit<LuaCompCLK>::NumberGlobalType LuaCompCLK::mNumberGlobals[] = {{0,0}};

class LuaCompCLKG : public CompCLKG
{
public:
    LuaCompCLKG(lua_State* luaState=NULL)
    {
        float param = luaL_optnumber(luaState, 1, 0.0f);
        mParam = param;
    }
    virtual ~LuaCompCLKG(){}

    static const char mClassName[];
    static Orbit<LuaCompCLKG>::MethodType mMethods[];
    static Orbit<LuaCompCLKG>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompCLKG::mClassName[] = "CLKG";
Orbit<LuaCompCLKG>::MethodType LuaCompCLKG::mMethods[] = {{0,0}};
Orbit<LuaCompCLKG>::NumberGlobalType LuaCompCLKG::mNumberGlobals[] = {{0,0}};

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

class LuaCompMEM : public CompMEM
{
public:
    LuaCompMEM(lua_State* luaState=NULL){}
    virtual ~LuaCompMEM(){}

    static const char mClassName[];
    static Orbit<LuaCompMEM>::MethodType mMethods[];
    static Orbit<LuaCompMEM>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompMEM::mClassName[] = "MEM";
Orbit<LuaCompMEM>::MethodType LuaCompMEM::mMethods[] = {{0,0}};
Orbit<LuaCompMEM>::NumberGlobalType LuaCompMEM::mNumberGlobals[] = {{0,0}};

class LuaCompTMEM : public CompTMEM
{
public:
    LuaCompTMEM(lua_State* luaState=NULL){}
    virtual ~LuaCompTMEM(){}

    static const char mClassName[];
    static Orbit<LuaCompTMEM>::MethodType mMethods[];
    static Orbit<LuaCompTMEM>::NumberGlobalType mNumberGlobals[];
};
const char LuaCompTMEM::mClassName[] = "TMEM";
Orbit<LuaCompTMEM>::MethodType LuaCompTMEM::mMethods[] = {{0,0}};
Orbit<LuaCompTMEM>::NumberGlobalType LuaCompTMEM::mNumberGlobals[] = {{0,0}};

void gridbrainLuaRegister(lua_State* luaState)
{
    Orbit<LuaGridbrain>::orbitRegister(luaState);
    Orbit<LuaGrid>::orbitRegister(luaState);
    Orbit<LuaComponentSet>::orbitRegister(luaState);
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
    Orbit<LuaCompCLKG>::orbitRegister(luaState);
    Orbit<LuaCompDMUL>::orbitRegister(luaState);
    Orbit<LuaCompSEL>::orbitRegister(luaState);
    Orbit<LuaCompMEM>::orbitRegister(luaState);
    Orbit<LuaCompTMEM>::orbitRegister(luaState);
}

}

#endif

