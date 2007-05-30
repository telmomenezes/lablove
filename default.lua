number_of_plants = 100
number_of_animats = 100

world_width = 1000
world_height = 1000

grid_height = 10
grid_alpha = 10
grid_beta = 10

THR = true
MAX = true
MUL = true
NOT = true

view_range = 150.0
view_angle = 100.0

high_age_limit = 550
low_age_limit = 450

metabolism = 0.0
go_cost = 0.0
rotate_cost = 0.0

initial_connections = 10

----------------------------------------------

love_lab = LoveLab()
love_lab:create()
love_lab:set_seed_index(1)

sim = SimSimple2D()
sim:set_world_dimensions(world_width, world_height, view_range * 2)

love_lab:set_simulation(sim)
animat = AnimatSimple2D(initial_connections, 25)

animat:set_size(10.0)
animat:set_view_range(view_range)
animat:set_view_angle(view_angle)
animat:set_age_range(low_age_limit, high_age_limit)
animat:set_initial_energy(1.0)
animat:set_metabolism(metabolism)
animat:set_go_cost(go_cost)
animat:set_rotate_cost(rotate_cost)

animat_color = MoleculeRGB(1.0, 0.0, 0.0)
animat:set_color(animat_color)

per_set = GridbrainComponentSet()
per_set:add_component(GBCOMP_PER, 1)
per_set:add_component(GBCOMP_PER, 2)
per_set:add_component(GBCOMP_PER, 3)
per_set:add_component(GBCOMP_PER, 4)

alpha_set = GridbrainComponentSet()
if THR then
	alpha_set:add_component(GBCOMP_THR, 0)
end
if MAX then
	alpha_set:add_component(GBCOMP_MAX, 0)
end
if MUL then
	alpha_set:add_component(GBCOMP_MUL, 0)
end
if NOT then
	alpha_set:add_component(GBCOMP_NOT, 0)
end

grid = Grid()
grid:set_width(grid_alpha)
grid:set_height(grid_height)
grid:add_component_set(per_set, 0)
grid:add_component_set(alpha_set, -1)

animat:set_alpha_objects_grid(grid);

act_set = GridbrainComponentSet()
act_set:add_component(GBCOMP_ACT, 1)
act_set:add_component(GBCOMP_ACT, 2)
act_set:add_component(GBCOMP_ACT, 3)

beta_set = GridbrainComponentSet()
if THR then
	beta_set:add_component(GBCOMP_THR, 0)
end
if MAX then
	beta_set:add_component(GBCOMP_MUL, 0)
end
if NOT then
	beta_set:add_component(GBCOMP_NOT, 0)
end
	
grid2 = Grid()
grid2:set_width(grid_beta)
grid2:set_height(grid_height)
grid2:add_component_set(beta_set, grid_beta - 2)
grid2:add_component_set(act_set, -1)

animat:set_beta_grid(grid2)

plant = PlantSimple2D()
plant:set_size(5.0)
plant:set_initial_energy(1.0)

plant_color = MoleculeRGB(0.0, 1.0, 0.0)
plant:set_color(plant_color)

pop_dyn = PopDynFixedSpecies()
sim:set_population_dynamics(pop_dyn)

pop_dyn:add_static_species(plant, number_of_plants)
pop_dyn:add_evolving_species(animat, number_of_animats)

