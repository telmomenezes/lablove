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

#include "Brain.h"

Brain::Brain()
{
}

Brain::~Brain()
{
	for (vector<list<InterfaceItem*>*>::iterator iterInterface = mInputInterfacesVector.begin();
		iterInterface != mInputInterfacesVector.end();
		iterInterface++)
	{
		list<InterfaceItem*>* interface = (*iterInterface);

		for (list<InterfaceItem*>::iterator iterItem = interface->begin();
			iterItem != interface->end();
			iterItem++)
		{
			delete (*iterItem);
		}
	}

	for (list<InterfaceItem*>::iterator iterItem = mOutputInterface.begin();
			iterItem != mOutputInterface.end();
			iterItem++)
	{
		delete (*iterItem);
	}
}

list<InterfaceItem*>* Brain::getInputInterface(unsigned int channel)
{
	return mInputInterfacesVector[channel];
}

list<InterfaceItem*>* Brain::getOutputInterface()
{
	return &mOutputInterface;
}

