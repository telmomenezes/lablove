numberOfPlants = 100
numberOfAgents = 100
generationTime = 500

worldWidth = 1000
worldHeight = 1000

gridHeight = 6
gridAlpha = 5
gridBeta = 5

THR = true
MAX = true
MUL = true
NOT = true

viewRange = 150.0
viewAngle = 100.0

----------------------------------------------

lab = getLab()
lab:setSeedIndex(0)

sim = SimSimple()
sim:setWorldDimensions(worldWidth, worldHeight, viewRange * 2)
sim:setViewRange(viewRange)
sim:setViewAngle(viewAngle)

lab:setSimulation(sim)
animat = Agent(10, 25)

animat:setSize(10.0)
animat:setGraphic(GraphicTriangle())

perSet = GridbrainComponentSet()
perSet:addComponent(GridbrainComponent.PER, SimSimple.PERCEPTION_COLOR)
perSet:addComponent(GridbrainComponent.PER, SimSimple.PERCEPTION_POSITION)
perSet:addComponent(GridbrainComponent.PER, SimSimple.PERCEPTION_PROXIMITY)
perSet:addComponent(GridbrainComponent.PER, SimSimple.PERCEPTION_IN_CONTACT)

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
actSet:addComponent(GridbrainComponent.ACT, SimSimple.ACTION_GO)
actSet:addComponent(GridbrainComponent.ACT, SimSimple.ACTION_ROTATE)
actSet:addComponent(GridbrainComponent.ACT, SimSimple.ACTION_EAT)

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

plant = Plant()
plant:setSize(5.0)
plant:setInitialEnergy(1.0)
plant:setGraphic(GraphicSquare())

popDyn = PopDynGenerations()
sim:setPopulationDynamics(popDyn)

popDyn:addSpecies(plant, numberOfPlants, true)
animatSpeciesIndex = popDyn:addSpecies(animat, numberOfAgents, false)
popDyn:setGenerationTime(generationTime)

stats = StatMedAvgMinMax()
stats:setFile("default.csv")
stats:addField("energy")
stats:addField("connections")
popDyn:addSpeciesStatistics(animatSpeciesIndex, stats)

