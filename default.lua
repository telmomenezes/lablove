numberOfPlants = 100
numberOfAgents = 100

worldWidth = 3000
worldHeight = 3000

gridHeight = 5
gridAlpha = 3
gridBeta = 3

THR = true
MAX = true
MUL = true
NOT = true

viewRange = 150.0
viewAngle = 100.0

highAgeLimit = 5500
lowAgeLimit = 4500

metabolism = 0.0
goCost = 0.0
rotateCost = 0.0

initialConnections = 10

tournmentSize = 10

----------------------------------------------

sim = SimCont2D()
sim:setWorldDimensions(worldWidth, worldHeight, 150)
sim:setViewRange(viewRange)
sim:setViewAngle(viewAngle)
sim:setGoCost(goCost)
sim:setRotateCost(rotateCost)

agent = Agent()
agent:setSize(10.0)
agent:setAgeRange(lowAgeLimit, highAgeLimit)
agent:setInitialEnergy(1.0)
agent:setMetabolism(metabolism)

agent:addGraphic(GraphicTriangle())

agentColor = SymbolRGB(255, 50, 255)
symTable = SymbolTable(agentColor)
agent:addSymbolTable(symTable)
colorTableCode = symTable:getID()
agent:setSymbolName("color", colorTableCode, 0)

agentFeed = SymbolFixedString("abc", "aaa")
agentFood = SymbolFixedString("abc", "bbb")
agentFeedTable = SymbolTable(agentFeed)
agentFeedTable:addSymbol(agentFood)
agent:addSymbolTable(agentFeedTable)
feedTableCode = agentFeedTable:getID()
agent:setSymbolName("feed", feedTableCode, 0)
agent:setSymbolName("food", feedTableCode, 1)

brain = Gridbrain()

perSet = GridbrainComponentSet()
perSet:addComponent(GridbrainComponent.PER, SimCont2D.PERCEPTION_POSITION)
perSet:addComponent(GridbrainComponent.PER, SimCont2D.PERCEPTION_DISTANCE)
perSet:addComponent(GridbrainComponent.PER, SimCont2D.PERCEPTION_IN_CONTACT)
perSet:addComponent(GridbrainComponent.PER, SimCont2D.PERCEPTION_OBJECT_FEATURE, 0, colorTableCode, 0, 0)

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

agent:setBrain(brain)

i = 0
while i < initialConnections do
    brain:addRandomConnection()
    i = i + 1
end

plant = SimulationObject()
plant:setSize(10.0)
plant:setInitialEnergy(1.0)

plantColor = SymbolRGB(10, 250, 50)
symTable = SymbolTable(plantColor, colorTableCode)
plant:addSymbolTable(symTable)
plant:setSymbolName("color", colorTableCode, 0)

plantFood = SymbolFixedString("abc", "aaa")
plantFeedTable = SymbolTable(plantFood, feedTableCode)
plant:addSymbolTable(plantFeedTable)
plant:setSymbolName("food", feedTableCode, 0)

plant:addGraphic(GraphicSquare())

popDyn = PopDynFixedSpecies()
sim:setPopulationDynamics(popDyn)

popDyn:setTournmentSize(10)
agentSpeciesIndex = popDyn:addSpecies(agent, numberOfAgents)
popDyn:addSpecies(plant, numberOfPlants)

human = Agent()
dummyBrain = DummyBrain(1)
dummyBrain:addPerception("Position", 0, SimCont2D.PERCEPTION_POSITION)
dummyBrain:addPerception("Distance", 0, SimCont2D.PERCEPTION_DISTANCE)
dummyBrain:addPerception("Contact", 0, SimCont2D.PERCEPTION_IN_CONTACT)
dummyBrain:addPerception("Color", 0, SimCont2D.PERCEPTION_OBJECT_FEATURE, colorTableCode, 0, 0)
human:setSize(10.0)
human:setInitialEnergy(1.0)
human:setMetabolism(metabolism)
human:setBrain(dummyBrain)

humanColor = SymbolRGB(82, 228, 241)
symTable = SymbolTable(humanColor, colorTableCode)
human:addSymbolTable(symTable)
human:setSymbolName("color", colorTableCode, 0)

humanFeed = SymbolFixedString("abc", "aaa")
humanFood = SymbolFixedString("abc", "bbb")
humanFeedTable = SymbolTable(humanFeed, feedTableCode)
humanFeedTable:addSymbol(humanFood)
human:addSymbolTable(humanFeedTable)
human:setSymbolName("feed", feedTableCode, 0)
human:setSymbolName("food", feedTableCode, 1)

human:addGraphic(GraphicTriangle())
sim:addObject(human)
sim:setPos(human, 300, 300)
sim:setHuman(human)

stats = StatMedAvgMinMax()
stats:setFile("energy.csv")
stats:addField("energy")
--stats:addField("connections")
popDyn:addSpeciesStatistics(agentSpeciesIndex, stats)

statTime = StatTime()
statTime:setFile("time.csv")
popDyn:addStatistics(statTime)

sim:initGraphics(800, 600, false)
sim:run()
