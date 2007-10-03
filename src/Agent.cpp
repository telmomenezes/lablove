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

#include "Agent.h"

#include <math.h>

Agent::Agent(lua_State* luaState) : SimulationObject()
{
    mBrain = NULL;
    mType = TYPE_AGENT;
}

Agent::Agent(Agent* agent) : SimulationObject(agent)
{
    mBrain = agent->mBrain->clone();
}

Agent::~Agent()
{   
    if (mBrain != NULL)
    {
        delete mBrain;
        mBrain = NULL;
    }
}

/*void Agent::initTest()
{
    mGridbrain->setComponent(0, 0, 0, GridbrainComponent::PER, (float)SimSimple::PERCEPTION_POSITION);
    mGridbrain->setComponent(0, 1, 0, GridbrainComponent::PER, (float)SimSimple::PERCEPTION_PROXIMITY);
    //mGridbrain->setComponent(0, 2, 0, GridbrainComponent::PER, (float)SimSimple::PERCEPTION_COLOR);
    mGridbrain->setComponent(1, 0, 0, GridbrainComponent::THR, 0);
    mGridbrain->setComponent(1, 1, 0, GridbrainComponent::MAX, 0);
    mGridbrain->setComponent(2, 0, 0, GridbrainComponent::MUL, 0);
    mGridbrain->setComponent(2, 1, 0, GridbrainComponent::NOT, 0);
    mGridbrain->setComponent(3, 1, 0, GridbrainComponent::MUL, 0);

    mGridbrain->setComponent(0, 0, 1, GridbrainComponent::NOT, 0);
    mGridbrain->setComponent(0, 2, 1, GridbrainComponent::NOT, 0);
    mGridbrain->setComponent(1, 0, 1, GridbrainComponent::ACT, (float)SimSimple::ACTION_ROTATE);
    mGridbrain->setComponent(1, 1, 1, GridbrainComponent::ACT, (float)SimSimple::ACTION_GO);
    mGridbrain->setComponent(1, 2, 1, GridbrainComponent::ACT, (float)SimSimple::ACTION_EAT);

    mGridbrain->addConnection(0, 0, 0, 1, 0, 0, 1.0f);
    mGridbrain->addConnection(0, 1, 0, 1, 1, 0, 0.1f);
    mGridbrain->addConnection(0, 2, 0, 1, 1, 0, 0.5f);
    mGridbrain->addConnection(0, 2, 0, 2, 0, 0, 1.0f);
    mGridbrain->addConnection(0, 2, 0, 3, 1, 0, 1.0f);
    mGridbrain->addConnection(0, 2, 0, 0, 0, 1, 1.0f);
    mGridbrain->addConnection(1, 0, 0, 2, 0, 0, 1.0f);
    mGridbrain->addConnection(1, 0, 0, 2, 1, 0, 1.0f);
    mGridbrain->addConnection(1, 1, 0, 2, 0, 0, 1.0f);
    mGridbrain->addConnection(1, 1, 0, 3, 1, 0, 1.0f);
    mGridbrain->addConnection(2, 0, 0, 1, 0, 1, -1.0f);
    mGridbrain->addConnection(2, 1, 0, 3, 1, 0, 1.0f);
    mGridbrain->addConnection(3, 1, 0, 1, 1, 1, 1.0f);

    mGridbrain->addConnection(0, 0, 1, 1, 0, 1, 1.0f);
    mGridbrain->addConnection(0, 2, 1, 1, 2, 1, 1.0f);
}*/

SimulationObject* Agent::clone()
{
    return new Agent(this);
}

Brain* Agent::setBrain(Brain* brain)
{
    mBrain = brain;
    mBrain->init();
}

void Agent::compute()
{
    if (mBrain == NULL)
    {
        return;
    }

    mBrain->cycle();
}

/*Agent* Agent::crossover(Agent* otherParent)
{
    // TODO: check for incompatibilities, like gridbrain dimensions?

    Agent* child = new Agent();
    child->mMaxInputDepth = mMaxInputDepth;
    child->mGridbrain = mGridbrain->crossover(((Agent*)otherParent)->mGridbrain);

    return child;
}
*/

void Agent::mutate()
{
    if (mBrain != NULL)
    {
        mBrain->mutate();
    }
}

float Agent::getFieldValue(std::string fieldName)
{
    /*if (fieldName == "connections")
    {
        if (mGridbrain)
        {
            return ((float)(mGridbrain->getConnectionsCount()));
        }
        else
        {
            return 0.0f;
        }
    }
    else
    {*/
        return SimulationObject::getFieldValue(fieldName);
    //}
}

const char Agent::mClassName[] = "Agent";

Orbit<Agent>::MethodType Agent::mMethods[] = {
    {"setInitialEnergy", &SimulationObject::setInitialEnergy},
    {"addSymbolTable", &SimulationObject::addSymbolTable},
    {"setSize", &SimulationObject::setSize},
    {"setColor", &SimulationObject::setColor},
    {"setGraphic", &SimulationObject::setGraphic},
    {"setAgeRange", &SimulationObject::setAgeRange},
    {"setMetabolism", &SimulationObject::setMetabolism},
    {"setBrain", &Agent::setBrain},
    {0,0}
};

Orbit<Agent>::NumberGlobalType Agent::mNumberGlobals[] = {{0,0}};

int Agent::setBrain(lua_State* luaState)
{
    Brain* brain = (Brain*)Orbit<Agent>::pointer(luaState, 1);
    setBrain(brain);
    return 0;
}

/*void Agent::draw_brain()
{
    unsigned int height = _gridbrain->get_height();
    unsigned int alpha = _gridbrain->get_alpha();
    unsigned int beta = _gridbrain->get_beta();
    unsigned int gamma = _gridbrain->get_gamma();
    unsigned int width = alpha + beta + gamma;
    unsigned int screen_width = Love::get_instance().get_screen_width();
    unsigned int screen_height = Love::get_instance().get_screen_height();
    unsigned int component_width = screen_width / ((width * 2) + 1);
    unsigned int component_height = screen_height / ((height * 2) + 1);

    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            Layer* root = Love::getInstance().getRootLayer();
            int start_x = (x * component_width * 2) + component_width;
            int start_y = (y * component_height * 2) + component_height;
            int end_x = start_x + component_width;
            int end_y = start_y + component_height;

            GridbrainComponent* comp = _gridbrain->get_component(x, y);

            char* label = "?";

            unsigned int perception;
            unsigned int action;
            unsigned int color = COLOR_GBCOMPONENT_DEFAULT;

            switch (comp->_type)
            {
                case NUL:
                    label = "NULL";
                    break;
                case PER:
                    color = COLOR_GBCOMPONENT_PER;
                    perception = (unsigned int)comp->_parameter;
                    switch (perception)
                    {
                        case PERCEPTION_NULL:
                            label = "No Perception";
                            break;
                        case PERCEPTION_VISIBLE:
                            label = "Visible";
                            break;
                        case PERCEPTION_POSITION:
                            label = "Position";
                            break;
                        case PERCEPTION_PROXIMITY:
                            label = "Proximity";
                            break;
                        case PERCEPTION_IN_CONTACT:
                            label = "Contact";
                            break;
                        case PERCEPTION_IS_FOOD:
                            label = "Food";
                            break;
                        default:
                            break;
                    };
                    break;
                case STA:
                    color = COLOR_GBCOMPONENT_STA;
                    label = "STA";
                    break;
                case ACT:
                    color = COLOR_GBCOMPONENT_ACT;
                    action = (unsigned int)comp->_parameter;
                    switch (action)
                    {
                        case ACTION_NULL:
                            label = "No Action";
                            break;
                        case ACTION_GO:
                            label = "Go";
                            break;
                        case ACTION_ROTATE:
                            label = "Rotate";
                            break;
                        case ACTION_EAT:
                            label = "Eat";
                            break;
                        default:
                            break;
                    }
                    break;
                case THR:
                    color = COLOR_GBCOMPONENT_THR;
                    label = "THR";
                    break;
                case AGG:
                    color = COLOR_GBCOMPONENT_AGG;
                    label = "AGG";
                    break;
                case MAX:
                    color = COLOR_GBCOMPONENT_MAX;
                    label = "MAX";
                    break;
                case MUL:
                    color = COLOR_GBCOMPONENT_MUL;
                    label = "MUL";
                    break;
                case NOT:
                    color = COLOR_GBCOMPONENT_NOT;
                    label = "NOT";
                    break;
                default:
                    break;
            }

            boxColor(screen,
                start_x,
                start_y,
                end_x,
                end_y,
                color);

            int label_length = strlen(label);
            int label_x = start_x + ((component_width - (label_length * 8)) / 2);
            int label_y = start_y + ((component_height - 8) / 2);
            stringColor(screen, label_x, label_y, label, 0x000000FF);

            char string_buffer[255];
            sprintf(string_buffer, "%f", comp->_input);
            stringColor(screen, start_x, start_y, string_buffer, 0x00FF00FF);
            sprintf(string_buffer, "%f", comp->_output);
            stringColor(screen, start_x, end_y, string_buffer, 0xFF0000FF);

            start_x += component_width;
            GridbrainConnection* conn = comp->_first_connection;
            for (unsigned int i = 0; i < comp->_connections_count; i++)
            {
                start_y += 3;
                int target_x = conn->_x_targ;
                int target_y = conn->_target - (conn->_x_targ * _gridbrain->_height);
                end_x = (target_x * component_width * 2) + component_width;
                end_y = (target_y * component_height * 2) + component_height;
                end_y += component_height / 2;
                lineColor(screen, start_x, start_y, end_x, end_y, color);

                conn = (GridbrainConnection*)conn->_next_connection;
            }
        }
    }
}*/

