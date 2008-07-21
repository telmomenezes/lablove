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

#ifndef _INCLUDE_GRIDBRAIN_COMPONENT_NUL_H
#define _INCLUDE_GRIDBRAIN_COMPONENT_NUL_H

#include "Component.h"

class CompNUL : public Component
{
public:
    CompNUL();
    virtual ~CompNUL();

    virtual Component* clone();

    virtual void input(float value, int pin);
    virtual float output(unsigned int id);
    virtual void reset(int pass);

    virtual string getName(){return "NUL";}
    virtual ConnType getConnectorType(){return CONN_INOUT;}

    virtual bool isUnique(){return false;}
    virtual bool isNUL(){return true;}
    virtual bool compare(Component* comp){return true;}
};

static CompNUL COMP_NUL;

#endif

