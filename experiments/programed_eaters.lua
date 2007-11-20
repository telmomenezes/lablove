numberOfPlants = 100
numberOfAgents = 100

agentSize = 10.0
plantSize = 10.0

worldWidth = 3000
worldHeight = 3000

gridHeight = 10
gridAlpha = 10
gridBeta = 10

THR = true
MAX = true
MUL = true
NOT = true

viewRange = 150.0
viewAngle = 170.0

highAgeLimit = 5500
lowAgeLimit = 4500

metabolism = 0.0
goCost = 0.0
rotateCost = 0.0
goForceScale = 0.3
rotateForceScale = 0.006

friction = 0.000--3
drag = 0.05
rotFriction = 0.00000--3
rotDrag = 0.05

initialConnections = 10

tournamentSize = 10

timeLimit = 0

----------------------------------------------

dofile("experiments/aux/basic_command_line.lua")

sim = SimCont2D()
sim:setWorldDimensions(worldWidth, worldHeight, 100)
sim:setViewRange(viewRange)
sim:setViewAngle(viewAngle)
sim:setGoCost(goCost)
sim:setRotateCost(rotateCost)
sim:setGoForceScale(goForceScale)
sim:setRotateForceScale(rotateForceScale)
sim:setSeedIndex(seedIndex)
sim:setTimeLimit(timeLimit)

agent = Agent()

agent:addGraphic(GraphicTriangle())

symSize = SymbolFloat(agentSize)
symTable = SymbolTable(symSize)
agent:addSymbolTable(symTable)
sizeTableCode = symTable:getID()
agent:setSymbolName("size", sizeTableCode, 0)

symFriction = SymbolFloat(friction)
symDrag = SymbolFloat(drag)
symRotFriction = SymbolFloat(rotFriction)
symRotDrag = SymbolFloat(rotDrag)
symTable = SymbolTable(symFriction)
symTable:addSymbol(symDrag)
symTable:addSymbol(symRotFriction)
symTable:addSymbol(symRotDrag)
agent:addSymbolTable(symTable)
physicsTableCode = symTable:getID()
agent:setSymbolName("friction", physicsTableCode, 0)
agent:setSymbolName("drag", physicsTableCode, 1)
agent:setSymbolName("rot_friction", physicsTableCode, 2)
agent:setSymbolName("rot_drag", physicsTableCode, 3)

symInitialEnergy = SymbolFloat(1.0)
symMetabolism = SymbolFloat(metabolism)
symTable = SymbolTable(symInitialEnergy)
symTable:addSymbol(symMetabolism)
agent:addSymbolTable(symTable)
energyTableCode = symTable:getID()
agent:setSymbolName("initial_energy", energyTableCode, 0)
agent:setSymbolName("metabolism", energyTableCode, 1)

symLowAgeLimit = SymbolUL(lowAgeLimit)
symHighAgeLimit = SymbolUL(highAgeLimit)
symTable = SymbolTable(symLowAgeLimit)
symTable:addSymbol(symHighAgeLimit)
agent:addSymbolTable(symTable)
ageTableCode = symTable:getID()
agent:setSymbolName("low_age_limit", ageTableCode, 0)
agent:setSymbolName("high_age_limit", ageTableCode, 1)

agentColor = SymbolRGB(255, 0, 0)
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

brain:setMutateAddConnectionProb(0)
brain:setMutateRemoveConnectionProb(0)
brain:setMutateChangeConnectionWeightProb(0)
brain:setMutateChangeComponentProb(0)
brain:setWeightMutationStanDev(0)

grid = Grid()
grid:init(Grid.ALPHA, 3, 3)
brain:addGrid(grid, "objects");

grid2 = Grid()
grid2:init(Grid.BETA, 2, 3)
brain:addGrid(grid2, "beta")

brain:initEmpty()

brain:setComponent(0, 0, 0, GridbrainComponent.PER, SimCont2D.PERCEPTION_POSITION)
brain:setComponent(0, 1, 0, GridbrainComponent.PER, SimCont2D.PERCEPTION_OBJECT_FEATURE, feedTableCode, 0, 1)
brain:setComponent(0, 2, 0, GridbrainComponent.PER, SimCont2D.PERCEPTION_DISTANCE)
brain:setComponent(1, 1, 0, GridbrainComponent.TNAND)
brain:setComponent(1, 2, 0, GridbrainComponent.MMAX)
brain:setComponent(2, 0, 0, GridbrainComponent.TAND)
brain:setComponent(2, 1, 0, GridbrainComponent.TAND)

brain:setComponent(0, 0, 1, GridbrainComponent.NOT)
brain:setComponent(0, 2, 1, GridbrainComponent.NOT)
brain:setComponent(1, 0, 1, GridbrainComponent.ACT, SimCont2D.ACTION_ROTATE)
brain:setComponent(1, 1, 1, GridbrainComponent.ACT, SimCont2D.ACTION_GO)
brain:setComponent(1, 2, 1, GridbrainComponent.ACT, SimCont2D.ACTION_EAT)

agent:setBrain(brain)

brain:addConnection(0, 0, 0, 2, 0, 0, 0.5)
brain:addConnection(0, 0, 0, 1, 1, 0, 0.5)
brain:addConnection(0, 1, 0, 2, 0, 0, 0.5)
brain:addConnection(0, 1, 0, 0, 0, 1, 0.5)
brain:addConnection(0, 1, 0, 1, 2, 0, 0.5)
brain:addConnection(0, 2, 0, 1, 2, 0, -0.5)
brain:addConnection(1, 1, 0, 2, 1, 0, 0.5)
brain:addConnection(1, 2, 0, 2, 0, 0, 0.5)
brain:addConnection(1, 2, 0, 2, 1, 0, 0.5)
brain:addConnection(2, 0, 0, 1, 0, 1, -0.5)
brain:addConnection(2, 1, 0, 1, 1, 1, 0.5)
brain:addConnection(0, 0, 1, 1, 0, 1, 0.5)
brain:addConnection(0, 2, 1, 1, 2, 1, 0.5)

plant = GraphicalObject()

symSize = SymbolFloat(plantSize)
symTable = SymbolTable(symSize, sizeTableCode)
plant:addSymbolTable(symTable)
plant:setSymbolName("size", sizeTableCode, 0)

symPlantInitialEnergy = SymbolFloat(1.0)
symTable = SymbolTable(symPlantInitialEnergy, energyTableCode)
plant:addSymbolTable(symTable)
plant:setSymbolName("initial_energy", energyTableCode, 0)

plantColor = SymbolRGB(0, 255, 0)
symTable = SymbolTable(plantColor, colorTableCode)
plant:addSymbolTable(symTable)
plant:setSymbolName("color", colorTableCode, 0)

plantFeed = SymbolFixedString("abcd", "ddd")
plantFood = SymbolFixedString("abc", "aaa")
plantFeedTable = SymbolTable(plantFeed, feedTableCode)
plantFeedTable:addSymbol(plantFood)
plant:addSymbolTable(plantFeedTable)
plant:setSymbolName("feed", feedTableCode, 0)
plant:setSymbolName("food", feedTableCode, 1)

plant:addGraphic(GraphicSquare())

popDyn = PopDynFixedSpecies()
sim:setPopulationDynamics(popDyn)

popDyn:setTournamentSize(10)
agentSpeciesIndex = popDyn:addSpecies(agent, numberOfAgents, false)
popDyn:addSpecies(plant, numberOfPlants)

human = Agent()
dummyBrain = DummyBrain(1)
dummyBrain:setChannelName(0, "objects")
dummyBrain:addPerception("Position", 0, SimCont2D.PERCEPTION_POSITION)
dummyBrain:addPerception("Distance", 0, SimCont2D.PERCEPTION_DISTANCE)
dummyBrain:addPerception("Contact", 0, SimCont2D.PERCEPTION_IN_CONTACT)
dummyBrain:addPerception("Color", 0, SimCont2D.PERCEPTION_OBJECT_FEATURE, colorTableCode, 0, 0)
dummyBrain:addPerception("Food", 0, SimCont2D.PERCEPTION_OBJECT_FEATURE, feedTableCode, 0, 1)
dummyBrain:addPerception("Predator", 0, SimCont2D.PERCEPTION_OBJECT_FEATURE, feedTableCode, 1, 0)

human:setBrain(dummyBrain)

symSize = SymbolFloat(agentSize)
symTable = SymbolTable(symSize, sizeTableCode)
human:addSymbolTable(symTable)
human:setSymbolName("size", sizeTableCode, 0)

symHFriction = SymbolFloat(friction)
symHDrag = SymbolFloat(drag)
symHRotFriction = SymbolFloat(rotFriction)
symHRotDrag = SymbolFloat(rotDrag)
symTable = SymbolTable(symFriction, physicsTableCode)
symTable:addSymbol(symHDrag)
symTable:addSymbol(symHRotFriction)
symTable:addSymbol(symHRotDrag)
human:addSymbolTable(symTable)
human:setSymbolName("friction", physicsTableCode, 0)
human:setSymbolName("drag", physicsTableCode, 1)
human:setSymbolName("rot_friction", physicsTableCode, 2)
human:setSymbolName("rot_drag", physicsTableCode, 3)

symHInitialEnergy = SymbolFloat(1.0)
symHMetabolism = SymbolFloat(metabolism)
symTable = SymbolTable(symHInitialEnergy, energyTableCode)
symTable:addSymbol(symHMetabolism)
human:addSymbolTable(symTable)
human:setSymbolName("initial_energy", energyTableCode, 0)
human:setSymbolName("metabolism", energyTableCode, 1)

symHLowAgeLimit = SymbolUL(lowAgeLimit)
symHHighAgeLimit = SymbolUL(highAgeLimit)
symTable = SymbolTable(symHLowAgeLimit, ageTableCode)
symTable:addSymbol(symHHighAgeLimit)
human:addSymbolTable(symTable)
human:setSymbolName("low_age_limit", ageTableCode, 0)
human:setSymbolName("high_age_limit", ageTableCode, 1)

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

stats = StatCommon()
stats:setFile("energy.csv")
stats:addField("energy")
--stats:addField("connections")
popDyn:addDeathLog(agentSpeciesIndex, stats)

statTime = StatTime()
statTime:setFile("time.csv")
popDyn:addDeathLog(agentSpeciesIndex, statTime)

sim:initGraphics(screenWidth, screenHeight, fullScreen, noGraphics)
sim:run()
