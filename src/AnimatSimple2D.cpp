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

#include "AnimatSimple2D.h"
#include "LoveLab.h"
#include "SimSimple2D.h"
#include "math.h"
#include "functions.h"
#include "defines.h"
#include "perceptions.h"
#include "actions.h"

#ifdef __LOVE_GRAPHICS
#include "GL/gl.h"
#endif

AnimatSimple2D::AnimatSimple2D()
{
	_max_input_depth = 0;
	_gridbrain = NULL;
	_current_input_depth = 0;
	_initial_connections = 0;

	_view_range = 0.0f;
	_view_angle = 0.0f;
	_half_view_angle = 0.0f;
	_low_limit_view_angle = 0.0f;
	_high_limit_view_angle = 0.0f;
	_nearest_food = NULL;
	_distance_to_nearest_food = 0.0f;

	_action_go = false;
	_action_rotate = false;
	_action_eat = false;

	_go_cost = 0.0f;
	_rotate_cost = 0.0f;

	_color_molecule_table = NULL;
}

AnimatSimple2D::AnimatSimple2D(lua_State* L)
{
	unsigned int initial_connections = luaL_checkint(L, 1);
	unsigned int max_input_depth = luaL_checkint(L, 2);

	_view_range = 0.0f;
	_view_angle = 0.0f;
	_half_view_angle = 0.0f;
	_low_limit_view_angle = 0.0f;
	_high_limit_view_angle = 0.0f;
	_nearest_food = NULL;
	_distance_to_nearest_food = 0.0f;

	_action_go = false;
	_action_rotate = false;
	_action_eat = false;

	_max_input_depth = max_input_depth;
	_initial_connections = initial_connections;

	_gridbrain = new Gridbrain();
	_gridbrain->set_max_input_depth(_max_input_depth);
	_current_input_depth = 0;

	_go_cost = 0.0f;
	_rotate_cost = 0.0f;

	_color_molecule_table = NULL;
}

AnimatSimple2D::AnimatSimple2D(AnimatSimple2D* anim, bool full) : ObjectSimple2D(anim)
{
	_max_input_depth = anim->_max_input_depth;
	_initial_connections = anim->_initial_connections;
	_gridbrain = anim->_gridbrain->clone(full);
	if (full)
	{
		_color_molecule_table = anim->_color_molecule_table->clone();
	}
	else
	{
		_color_molecule_table = NULL;
	}
	_current_input_depth = 0;

	_view_range = anim->_view_range;
	_view_angle = anim->_view_angle;
	_half_view_angle = anim->_half_view_angle;
	_low_limit_view_angle = anim->_low_limit_view_angle;
	_high_limit_view_angle = anim->_high_limit_view_angle;
	_nearest_food = NULL;
	_distance_to_nearest_food = 0;

	_action_go = false;
	_action_rotate = false;
	_action_eat = false;

	_go_cost = anim->_go_cost;
	_rotate_cost = anim->_rotate_cost;
}

void AnimatSimple2D::set_alpha_objects_grid(Grid* grid)
{
	grid->set_type(Grid::ALPHA);
	_gridbrain->add_grid(grid);
}

void AnimatSimple2D::set_beta_grid(Grid* grid)
{
	grid->set_type(Grid::BETA);
	_gridbrain->add_grid(grid);
}

void AnimatSimple2D::init()
{
	_color_molecule_table = new MoleculeTable();

	for (unsigned int i = 0; i < 5; i++)
	{
		MoleculeRGB* mol = new MoleculeRGB();
		mol->init_random();
		_color_molecule_table->add_molecule(mol);
	}

	_gridbrain->init();

	for (unsigned int i = 0; i < _initial_connections; i++)
	{
		_gridbrain->add_random_connection();
	}
	//init_test();
}

void AnimatSimple2D::init_test()
{
	_gridbrain->set_component(0, 0, 0, GridbrainComponent::PER, (float)PERCEPTION_POSITION);
	_gridbrain->set_component(0, 1, 0, GridbrainComponent::PER, (float)PERCEPTION_PROXIMITY);
	_gridbrain->set_component(0, 2, 0, GridbrainComponent::PER, (float)PERCEPTION_COLOR);
	_gridbrain->set_component(1, 0, 0, GridbrainComponent::THR, 0);
	_gridbrain->set_component(1, 1, 0, GridbrainComponent::MAX, 0);
	_gridbrain->set_component(2, 0, 0, GridbrainComponent::MUL, 0);
	_gridbrain->set_component(2, 1, 0, GridbrainComponent::NOT, 0);
	_gridbrain->set_component(3, 1, 0, GridbrainComponent::MUL, 0);

	_gridbrain->set_component(0, 0, 1, GridbrainComponent::NOT, 0);
	_gridbrain->set_component(0, 2, 1, GridbrainComponent::NOT, 0);
	_gridbrain->set_component(1, 0, 1, GridbrainComponent::ACT, (float)ACTION_ROTATE);
	_gridbrain->set_component(1, 1, 1, GridbrainComponent::ACT, (float)ACTION_GO);
	_gridbrain->set_component(1, 2, 1, GridbrainComponent::ACT, (float)ACTION_EAT);

	_gridbrain->add_connection(0, 0, 0, 1, 0, 0, 1.0f);
	_gridbrain->add_connection(0, 1, 0, 1, 1, 0, 0.1f);
	_gridbrain->add_connection(0, 2, 0, 1, 1, 0, 0.5f);
	_gridbrain->add_connection(0, 2, 0, 2, 0, 0, 1.0f);
	_gridbrain->add_connection(0, 2, 0, 3, 1, 0, 1.0f);
	_gridbrain->add_connection(0, 2, 0, 0, 0, 1, 1.0f);
	_gridbrain->add_connection(1, 0, 0, 2, 0, 0, 1.0f);
	_gridbrain->add_connection(1, 0, 0, 2, 1, 0, 1.0f);
	_gridbrain->add_connection(1, 1, 0, 2, 0, 0, 1.0f);
	_gridbrain->add_connection(1, 1, 0, 3, 1, 0, 1.0f);
	_gridbrain->add_connection(2, 0, 0, 1, 0, 1, -1.0f);
	_gridbrain->add_connection(2, 1, 0, 3, 1, 0, 1.0f);
	_gridbrain->add_connection(3, 1, 0, 1, 1, 1, 1.0f);

	_gridbrain->add_connection(0, 0, 1, 1, 0, 1, 1.0f);
	_gridbrain->add_connection(0, 2, 1, 1, 2, 1, 1.0f);

	_gridbrain->init_grids_input_output();
}

AnimatSimple2D::~AnimatSimple2D()
{	
	if (_gridbrain != NULL)
	{
		delete _gridbrain;
		_gridbrain = NULL;
	}

	if (_color_molecule_table != NULL)
	{
		delete _color_molecule_table;
		_color_molecule_table = NULL;
	}
}

SimulationObject* AnimatSimple2D::clone(bool full)
{
	return new AnimatSimple2D(this, full);
}

void AnimatSimple2D::set_rot(float rot)
{
	ObjectSimple2D::set_rot(rot);
	_low_limit_view_angle = normalize_angle(_rot - _half_view_angle);
	_high_limit_view_angle = normalize_angle(_rot + _half_view_angle);
}

void AnimatSimple2D::on_cycle()
{
	ObjectSimple2D::on_cycle();
	perception_step();
	computation_step();
	action_step();
	end_cycle();
}

void AnimatSimple2D::computation_step()
{
	Grid* grid = _gridbrain->get_grid(0);
	grid->set_input_depth(_current_input_depth);
	_current_input_depth = 0;

	// TODO: update states

	// Eval gridbrain
	_gridbrain->eval();

	// Apply actions
	_action_go = false;
	_action_rotate = false;
	_action_eat = false;

	_action_go_param = 0.0f;
	_action_rotate_param = 0.0f;
	_action_eat_param = 0.0f;

	grid = _gridbrain->get_grid(1);
	unsigned int actions_count = grid->get_actions_count();

	for (unsigned int number = 0; number < actions_count; number++)
	{
		float output = grid->get_output(number);
		unsigned int action_type = grid->get_action_type(number);

		if (output != 0.0f)
		{
			switch (action_type)
			{
				case ACTION_GO:
					_action_go = true;
					_action_go_param += output;
					break;
				case ACTION_ROTATE:
					_action_rotate = true;
					_action_rotate_param += output;
					break;
				case ACTION_EAT:
					_action_eat = true;
					_action_eat_param += output;
					break;
			}
		}
	}
}

void AnimatSimple2D::end_cycle()
{
	_nearest_food = NULL;

	_action_go = false;
	_action_rotate = false;
	_action_eat = false;
}

void AnimatSimple2D::perception_step()
{
	// Determine cells to analyse
	unsigned int cell_side = (unsigned int)(((SimSimple2D*)(LoveLab::get_instance().get_simulation()))->get_cell_side());
	int x = (int)_x;
	int y = (int)_y;
	int cell_pos_x = x % cell_side;
	int cell_pos_y = y % cell_side;
	int range = (int)_view_range;
	int left_limit = cell_pos_x - range; 
	int right_limit = cell_pos_x + range;
	int top_limit = cell_pos_y - range;
	int bottom_limit = cell_pos_y + range;

	unsigned int cell_x = ((unsigned int)_x) / cell_side;
	unsigned int cell_y = ((unsigned int)_y) / cell_side;

	bool left = false;
	bool right = false;
	bool top = false;
	bool bottom = false;

	if ((cell_x > 0) && (left_limit < 0))
	{
		left = true;
	}
	if ((cell_x < (((SimSimple2D*)(LoveLab::get_instance().get_simulation()))->get_world_cell_width() - 1))
		&& (right_limit >= cell_side))
	{
		right = true;
	}
	if ((cell_y > 0) && (top_limit < 0))
	{
		top = true;
	}
	if ((cell_y < (((SimSimple2D*)(LoveLab::get_instance().get_simulation()))->get_world_cell_length() - 1))
		&& (bottom_limit >= cell_side))
	{
		bottom = true;
	}

	// Check all objects in each cell
	scan_cell(cell_x, cell_y);
	if (left)
	{
		scan_cell(cell_x - 1, cell_y);
	}
	if (left && top)
	{
		scan_cell(cell_x - 1, cell_y - 1);
	}
	if (left && bottom)
	{
		scan_cell(cell_x - 1, cell_y + 1);
	}
	if (right)
	{
		scan_cell(cell_x + 1, cell_y);
	}
	if (right && top)
	{
		scan_cell(cell_x + 1, cell_y - 1);
	}
	if (right && bottom)
	{
		scan_cell(cell_x + 1, cell_y + 1);
	}
	if (top)
	{
		scan_cell(cell_x, cell_y - 1);
	}
	if (bottom)
	{
		scan_cell(cell_x, cell_y + 1);
	}
}

void AnimatSimple2D::scan_cell(int cell_x, int cell_y)
{
	ObjectSimple2D* obj = (ObjectSimple2D*)
		((SimSimple2D*)(LoveLab::get_instance().get_simulation()))->get_cell_grid()[
		(cell_y * ((SimSimple2D*)(LoveLab::get_instance().get_simulation()))->get_world_cell_width()) + cell_x];

	while (obj)
	{
		if (obj != this)
		{
			bool visible = false;
			bool contact = false;

			float dx = _x - obj->_x;
			float dy = _y - obj->_y;
			float distance = sqrtf((dx * dx) + (dy * dy));
			float angle = 0.0f;

			if (distance <= _view_range)
			{
				angle = atan2f(-dy, -dx);

				if (_high_limit_view_angle > _low_limit_view_angle)
				{
					if ((angle <= _high_limit_view_angle) && (angle >= _low_limit_view_angle))
					{
						visible = true;
					}
				}
				else
				{
					if ((angle <= _high_limit_view_angle) || (angle >= _low_limit_view_angle))
					{
						visible = true;
					}
				}

				if (distance <= _size + obj->get_size())
				{
					contact = true;
				}
			}

			if (contact && obj->is_food())
			{
				if ((_nearest_food == NULL) || (distance < _distance_to_nearest_food))
				{
					_nearest_food = obj;
					_distance_to_nearest_food = distance;
				}
			}

			if (visible)
			{
				on_scan_object(obj,
						visible,
						contact,
						normalize_angle(_rot - angle),
						distance);
			}
		}

		obj = ((ObjectSimple2D*)obj)->_next_cell_list;
	}
}

void AnimatSimple2D::set_view_angle(float angle)
{
	_view_angle = (angle * M_PI) / 180.0f;
	_half_view_angle = _view_angle / 2.0f;
}

void AnimatSimple2D::go_front(float distance)
{
	// TODO: check if move is possible

	_energy -= _go_cost * distance;
	float new_x = _x + (cosf(_rot) * distance);
	float new_y = _y + (sinf(_rot) * distance);
	set_pos(new_x, new_y);
}

void AnimatSimple2D::rotate(float angle)
{
	_energy -= _rotate_cost * angle;
	set_rot(get_rot() - angle);
}

void AnimatSimple2D::set_view_range(float range)
{
	_view_range = range;
}

void AnimatSimple2D::eat()
{
	if (_nearest_food)
	{
		delta_energy(_nearest_food->get_energy());
		LoveLab::get_instance().get_simulation()->kill_organism(_nearest_food);
	}
}

/*void AnimatSimple2D::action_step()
{
	if ((_action_go) && (_action_go_param > fabsf(_action_rotate_param)) && (_action_go_param > _action_eat_param))
	{
		go_front(1.0f);
	}
	else if ((_action_rotate) && (fabsf(_action_rotate_param) > _action_eat_param))
	{
		if (_action_rotate_param > 0.0f)
		{
			rotate(0.01f);
		}
		else if (_action_rotate_param < 0.0f)
		{
			rotate(-0.01f);
		}
	}
	else if (_action_eat)
	{
		eat();
	}
}*/

void AnimatSimple2D::action_step()
{
	if (_action_go)
	{
		go_front(1.0f);
	}
	if (_action_rotate)
	{
		if (_action_rotate_param > 0.0f)
		{
			rotate(-0.01f);
		}
		else if (_action_rotate_param < 0.0f)
		{
			rotate(0.01f);
		}
	}
	if (_action_eat)
	{
		eat();
	}
}

void AnimatSimple2D::on_scan_object(SimulationObject* obj, bool visible, bool contact, float angle, float distance)
{
	float normalized_value;

	if (_current_input_depth >= _max_input_depth)
	{
		return;
	}

	Grid* grid = _gridbrain->get_grid(0);

	unsigned int perceptions_count = grid->get_perceptions_count();
	for (unsigned int perception_number = 0; perception_number < perceptions_count; perception_number++)
	{
		unsigned int perception_type = grid->get_perception_type(perception_number);
		unsigned int perception_molecule = grid->get_perception_molecule(perception_number);

		switch (perception_type)
		{
			case PERCEPTION_COLOR:
				grid->set_input(perception_number,
						_current_input_depth,
						((ObjectSimple2D*)obj)->get_color()->bind(_color_molecule_table->get_molecule(perception_molecule)));
				break;

			case PERCEPTION_IN_CONTACT:
				if (contact)
				{
					grid->set_input(perception_number, _current_input_depth, 1.0f);
				}
				else
				{
					grid->set_input(perception_number, _current_input_depth, 0.0f);
				}
				break;

			case PERCEPTION_POSITION:
				normalized_value = 0.0f;
				if (visible)
				{
					normalized_value = angle / _half_view_angle;
				}
				grid->set_input(perception_number, _current_input_depth, normalized_value);
				break;

			case PERCEPTION_PROXIMITY:
				normalized_value = 0.0f;
				if (visible)
				{
					normalized_value = 1.0f - (distance / _view_range);
				}
				grid->set_input(perception_number, _current_input_depth, normalized_value);
				break;
		}
	}

	_current_input_depth++;
}

/*AnimatSimple2D* AnimatSimple2D::crossover(AnimatSimple2D* other_parent)
{
	// TODO: check for incompatibilities, like gridbrain dimensions?

	Animat* child = new Animat();
	child->_max_input_depth = _max_input_depth;
	child->_gridbrain = _gridbrain->crossover(((Animat*)other_parent)->_gridbrain);

	return child;
}
*/

void AnimatSimple2D::mutate()
{
	unsigned type = rand() % 4;

	switch (type)
	{
	case 0:
		_gridbrain->mutate_add_connection();
		break;
	case 1:
		_gridbrain->mutate_remove_connection();
		break;
	case 2:
		_gridbrain->mutate_change_connection_weight();
		break;
	case 3:
		_gridbrain->mutate_change_component();
		break;
	deafult:
		// ASSERT ERROR?
		break;
	}
}

#ifdef __LOVE_GRAPHICS
void AnimatSimple2D::draw()
{
	SimSimple2D* sim = (SimSimple2D*)(LoveLab::get_instance().get_simulation());

	float x = _x - sim->get_view_x();
	float y = _y - sim->get_view_y();

	if (sim->get_show_view_range())
	{
		float x = _x - sim->get_view_x();
		float y = _y - sim->get_view_y();

		float start_angle = _rot - (_view_angle / 2.0f);
		float end_angle = _rot + (_view_angle / 2.0f);

		glColor4ub(DEF_S2D_ANIMAT_VIEW_R,
			DEF_S2D_ANIMAT_VIEW_G,
			DEF_S2D_ANIMAT_VIEW_B,
			DEF_S2D_ANIMAT_VIEW_A);
		glBegin(GL_POLYGON);
		glVertex2f(x, y);

		float angle = start_angle;
		while (angle < end_angle)
		{
			glVertex2f(x + (cosf(angle) * _view_range), y + (sinf(angle) * _view_range));
			angle += 0.2f;
		}
		glVertex2f(x + (cosf(end_angle) * _view_range), y + (sinf(end_angle) * _view_range));

		glEnd();
	}

	float a1 = _rot;
	float a2 = _rot + (M_PI * 0.8f);
	float a3 = _rot + (M_PI * 1.2f);
	float p1x = x + (cosf(a1) * _size);
	float p1y = y + (sinf(a1) * _size);
	float p2x = x + (cosf(a2) * _size);
	float p2y = y + (sinf(a2) * _size);
	float p3x = x + (cosf(a3) * _size);
	float p3y = y + (sinf(a3) * _size);

	glColor3f(_color._red,
			_color._green,
			_color._blue);
	glBegin(GL_POLYGON);
	glVertex2f(p1x, p1y);
	glVertex2f(p2x, p2y);
	glVertex2f(p3x, p3y);
	glEnd();
}
#endif

const char AnimatSimple2D::className[] = "AnimatSimple2D";

Lunar<AnimatSimple2D>::RegType AnimatSimple2D::methods[] = {
	{"set_size", &ObjectSimple2D::set_size},
	{"set_color", &ObjectSimple2D::set_color},
	{"set_initial_energy", &SimulationObject::set_initial_energy},
        {"set_view_range", &AnimatSimple2D::set_view_range},
	{"set_view_angle", &AnimatSimple2D::set_view_angle},
	{"set_alpha_objects_grid", &AnimatSimple2D::set_alpha_objects_grid},
	{"set_beta_grid", &AnimatSimple2D::set_beta_grid},
	{"set_age_range", &ObjectSimple2D::set_age_range},
	{"set_metabolism", &ObjectSimple2D::set_metabolism},
	{"set_go_cost", &AnimatSimple2D::set_go_cost},
	{"set_rotate_cost", &AnimatSimple2D::set_rotate_cost},
        {0,0}
};

int AnimatSimple2D::set_view_range(lua_State* L)
{
        int view_range = luaL_checkint(L, 1);
        set_view_range(view_range);
        return 0;
}

int AnimatSimple2D::set_view_angle(lua_State* L)
{
        int view_angle = luaL_checkint(L, 1);
        set_view_angle(view_angle);
        return 0;
}

int AnimatSimple2D::set_alpha_objects_grid(lua_State* L)
{
	Grid* grid = (Grid*)Lunar<AnimatSimple2D>::pointer(L, 1);
	set_alpha_objects_grid(grid);
	return 0;
}

int AnimatSimple2D::set_beta_grid(lua_State* L)
{
	Grid* grid = (Grid*)Lunar<AnimatSimple2D>::pointer(L, 1);
	set_beta_grid(grid);
	return 0;
}

int AnimatSimple2D::set_go_cost(lua_State* L)
{
        float cost = luaL_checknumber(L, 1);
	set_go_cost(cost);
	return 0;
}

int AnimatSimple2D::set_rotate_cost(lua_State* L)
{
	float cost = luaL_checknumber(L, 1);
	set_rotate_cost(cost);
	return 0;
}

/*void AnimatSimple2D::draw_brain()
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
			SDL_Surface* screen = Love::get_instance().get_screen();
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

