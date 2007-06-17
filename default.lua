numberOfPlants = 100
numberOfAnimats = 100

worldWidth = 1000
worldHeight = 1000

gridHeight = 10
gridAlpha = 10
gridBeta = 10

THR = true
MAX = true
MUL = true
NOT = true

viewRange = 150.0
viewAngle = 100.0

highAgeLimit = 550
lowAgeLimit = 450

metabolism = 0.0
goCost = 0.0
rotateCost = 0.0

initialConnections = 10

----------------------------------------------

lab = getLab()
lab:setSeedIndex(0)

sim = SimSimple2D()
sim:setWorldDimensions(worldWidth, worldHeight, viewRange * 2)

lab:setSimulation(sim)
animat = AnimatSimple2D(initialConnections, 25)

animat:setSize(10.0)
animat:setViewRange(viewRange)
animat:setViewAngle(viewAngle)
animat:setAgeRange(lowAgeLimit, highAgeLimit)
animat:setInitialEnergy(1.0)
animat:setMetabolism(metabolism)
animat:setGoCost(goCost)
animat:setRotateCost(rotateCost)

animatColor = MoleculeRGB(1.0, 0.3, 0.1)
animat:setColor(animatColor)

perSet = GridbrainComponentSet()
perSet:addComponent(GridbrainComponent.PER, SimSimple2D.PERCEPTION_COLOR)
perSet:addComponent(GridbrainComponent.PER, SimSimple2D.PERCEPTION_POSITION)
perSet:addComponent(GridbrainComponent.PER, SimSimple2D.PERCEPTION_PROXIMITY)
perSet:addComponent(GridbrainComponent.PER, SimSimple2D.PERCEPTION_IN_CONTACT)

alphaSet = GridbrainComponentSet()
if THR then
	alphaSet:addComponent(GridbrainComponent.THR, 0)
end
if MAX then
	alphaSet:addComponent(GridbrainComponent.MAX, 0)
end
if MUL then
	alphaSet:addComponent(GridbrainComponent.MUL, 0)
end
if NOT then
	alphaSet:addComponent(GridbrainComponent.NOT, 0)
end

grid = Grid()
grid:setWidth(gridAlpha)
grid:setHeight(gridHeight)
grid:addComponentSet(perSet, 0)
grid:addComponentSet(alphaSet, -1)

animat:setAlphaObjectsGrid(grid);

actSet = GridbrainComponentSet()
actSet:addComponent(GridbrainComponent.ACT, SimSimple2D.ACTION_GO)
actSet:addComponent(GridbrainComponent.ACT, SimSimple2D.ACTION_ROTATE)
actSet:addComponent(GridbrainComponent.ACT, SimSimple2D.ACTION_EAT)

betaSet = GridbrainComponentSet()
if THR then
	betaSet:addComponent(GridbrainComponent.THR, 0)
end
if MAX then
	betaSet:addComponent(GridbrainComponent.MUL, 0)
end
if NOT then
	betaSet:addComponent(GridbrainComponent.NOT, 0)
end
	
grid2 = Grid()
grid2:setWidth(gridBeta)
grid2:setHeight(gridHeight)
grid2:addComponentSet(betaSet, gridBeta - 2)
grid2:addComponentSet(actSet, -1)

animat:setBetaGrid(grid2)

plant = PlantSimple2D()
plant:setSize(5.0)
plant:setInitialEnergy(1.0)

plantColor = MoleculeRGB(0.1, 0.9, 0.3)
plant:setColor(plantColor)

popDyn = PopDynFixedSpecies()
sim:setPopulationDynamics(popDyn)

popDyn:addStaticSpecies(plant, numberOfPlants)
popDyn:addEvolvingSpecies(animat, numberOfAnimats)

