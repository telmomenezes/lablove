numberOfPlants = 100
numberOfAgents = 100

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

sim = SimCont2D()
sim:setWorldDimensions(worldWidth, worldHeight, viewRange * 2)
sim:setViewRange(viewRange)
sim:setViewAngle(viewAngle)
sim:setGoCost(goCost)
sim:setRotateCost(rotateCost)

animat = Agent()
animat:setSize(10.0)
animat:setAgeRange(lowAgeLimit, highAgeLimit)
animat:setInitialEnergy(1.0)
animat:setMetabolism(metabolism)

animat:setGraphic(GraphicTriangle())

animatColor = SymbolRGB(255, 50, 255)
animat:setColor(animatColor)

symTable = SymbolTable(animatColor)
colorSymIndex = symTable:addSymbol(animatColor)
colorTableCode = 0
animat:addSymbolTable(symTable, colorTableCode)

brain = Gridbrain()

perSet = GridbrainComponentSet()
--perSet:addComponent(GridbrainComponent.PER, -1, 0, colorTableCode, colorSymIndex, colorTableCode, colorSymIndex)
perSet:addComponent(GridbrainComponent.PER, SimCont2D.PERCEPTION_POSITION)
perSet:addComponent(GridbrainComponent.PER, SimCont2D.PERCEPTION_PROXIMITY)
perSet:addComponent(GridbrainComponent.PER, SimCont2D.PERCEPTION_IN_CONTACT)

alphaSet = GridbrainComponentSet()
if THR then
    alphaSet:addComponent(GridbrainComponent.THR)
end
if MAX then
    alphaSet:addComponent(GridbrainComponent.MAX)
end
if MUL then
    alphaSet:addComponent(GridbrainComponent.MUL)
end
if NOT then
    alphaSet:addComponent(GridbrainComponent.NOT)
end

grid = Grid()
grid:init(Grid.ALPHA, gridAlpha, gridHeight)
grid:addComponentSet(perSet, 0, 0)
grid:addComponentSet(alphaSet, 1, gridAlpha - 1)

brain:addGrid(grid);

actSet = GridbrainComponentSet()
actSet:addComponent(GridbrainComponent.ACT, SimCont2D.ACTION_GO)
actSet:addComponent(GridbrainComponent.ACT, SimCont2D.ACTION_ROTATE)
actSet:addComponent(GridbrainComponent.ACT, SimCont2D.ACTION_EAT)

betaSet = GridbrainComponentSet()
if THR then
    betaSet:addComponent(GridbrainComponent.THR)
end
if MAX then
    betaSet:addComponent(GridbrainComponent.MUL)
end
if NOT then
    betaSet:addComponent(GridbrainComponent.NOT)
end
    
grid2 = Grid()
grid2:init(Grid.BETA, gridBeta, gridHeight)
grid2:addComponentSet(betaSet, 0, gridBeta - 2)
grid2:addComponentSet(actSet, gridBeta - 1, gridBeta - 1)

brain:addGrid(grid2)

animat:setBrain(brain)

i = 0
while i < initialConnections do
    brain:addRandomConnection()
    i = i + 1
end

plant = Plant()
plant:setSize(5.0)
plant:setInitialEnergy(1.0)

plantColor = SymbolRGB(10, 250, 50)
plant:setColor(plantColor)

plant:setGraphic(GraphicSquare())

popDyn = PopDynFixedSpecies()
sim:setPopulationDynamics(popDyn)

animatSpeciesIndex = popDyn:addSpecies(animat, numberOfAgents)
popDyn:addSpecies(plant, numberOfPlants)

human = Agent()

--human:setPos(worldWidth / 2, worldHeight / 2)
--human:setRot(0.0)
human:setSize(10.0)
human:setInitialEnergy(1.0)
human:setMetabolism(metabolism)

humanColor = SymbolRGB(0, 0, 200)
human:setColor(humanColor)

human:setGraphic(GraphicTriangle())

--popDyn:setHuman(human)

stats = StatMedAvgMinMax()
--stats:setFile("energy.csv")
stats:addField("energy")
stats:addField("connections")
--popDyn:addSpeciesStatistics(animatSpeciesIndex, stats)

statTime = StatTime()
statTime:setFile("time.csv")
--popDyn:addStatistics(statTime)

sim:run()
