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

#include "GridbrainComponent.h"

const int GridbrainComponent::NUL = 0;
const int GridbrainComponent::PER = 1;
const int GridbrainComponent::STA = 2;
const int GridbrainComponent::ACT = 3;
const int GridbrainComponent::THR = 4;
const int GridbrainComponent::AGG = 5;
const int GridbrainComponent::MAX = 6;
const int GridbrainComponent::MUL = 7;
const int GridbrainComponent::NOT = 8;

GridbrainComponent::GridbrainComponent()
{
}

GridbrainComponent::~GridbrainComponent()
{
}

void GridbrainComponent::init_lua_table(lua_State* L)
{
	lua_pushnumber(L, 0);
	lua_setglobal(L, "GBCOMP_NUL");
	lua_pushnumber(L, 1);
	lua_setglobal(L, "GBCOMP_PER");
	lua_pushnumber(L, 2);
	lua_setglobal(L, "GBCOMP_STA");
	lua_pushnumber(L, 3);
	lua_setglobal(L, "GBCOMP_ACT");
	lua_pushnumber(L, 4);
	lua_setglobal(L, "GBCOMP_THR");
	lua_pushnumber(L, 5);
	lua_setglobal(L, "GBCOMP_AGG");
	lua_pushnumber(L, 6);
	lua_setglobal(L, "GBCOMP_MAX");
	lua_pushnumber(L, 7);
	lua_setglobal(L, "GBCOMP_MUL");
	lua_pushnumber(L, 8);
	lua_setglobal(L, "GBCOMP_NOT");
}

