-- PROGRAMED POISON
-- Human programed agents that find and eat certain food items
-- Amount of energy gathered from food is realtive to symbol_distance(feed,food)
-- Food will be poisonous if symbol_distance(feed,food) > 0.5

dofile("experiments/aux/basic_defines.lua")

-- Experiment Parameters
--------------------------------------------------------------------------------

numberOfPlants = 200
numberOfAgents = 50

agentSize = 10.0
plantSize = 10.0

worldWidth = 3000
worldHeight = 3000

viewRange = 150.0
viewAngle = 170.0

lowAgeLimit = 4500
highAgeLimit = 5500

metabolism = 0.0
goCost = 0.0
rotateCost = 0.0
goForceScale = 0.3
rotateForceScale = 0.006

friction = 0.0
drag = 0.05
rotFriction = 0.0
rotDrag = 0.05

timeLimit = 0

logTimeInterval = 10

humanAgent = true

-- Command line, log file names, etc
--------------------------------------------------------------------------------

dofile("experiments/aux/basic_command_line.lua")

logSuffix = "_programed_poison"
            .. "s"
            .. seedIndex


-- Simulation
--------------------------------------------------------------------------------

sim = SimCont2D()
sim:setWorldDimensions(worldWidth, worldHeight, 250)
sim:setViewRange(viewRange)
sim:setViewAngle(viewAngle)
sim:setGoCost(goCost)
sim:setRotateCost(rotateCost)
sim:setGoForceScale(goForceScale)
sim:setRotateForceScale(rotateForceScale)
sim:setSeedIndex(seedIndex)
sim:setTimeLimit(timeLimit)

-- Agents
--------------------------------------------------------------------------------

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

symInitialEnergy = SymbolFloat(0.000001)
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

agentColor = SymbolRGB(0, 0, 255)
symTable = SymbolTable(agentColor)
agent:addSymbolTable(symTable)
colorTableCode = symTable:getID()
agent:setSymbolName("color", colorTableCode, 0)

agentFeed = SymbolFloat(0.0)
agentFeedTable = SymbolTable(agentFeed)
agent:addSymbolTable(agentFeedTable)
feedTableCode = agentFeedTable:getID()
dummyTable = SymbolTable(agentFeed)
foodTableCode = dummyTable:getID()
agent:setSymbolName("feed", feedTableCode, 0)

-- Agent Brain

brain = Gridbrain()

brain:setMutateAddConnectionProb(0)
brain:setMutateRemoveConnectionProb(0)
brain:setMutateChangeConnectionWeightProb(0)
brain:setMutateChangeComponentProb(0)
brain:setWeightMutationStanDev(0)

grid = Grid()
grid:init(Grid.ALPHA, 4, 3)
brain:addGrid(grid, "objects");

grid2 = Grid()
grid2:init(Grid.BETA, 2, 3)
brain:addGrid(grid2, "beta")

brain:initEmpty()

brain:setComponent(0, 0, 0, PER, SimCont2D.PERCEPTION_POSITION)
brain:setComponent(0, 1, 0, PER, SimCont2D.PERCEPTION_SYMBOL, SYM_TO_SYM, feedTableCode, 0, foodTableCode, 0)
brain:setComponent(0, 2, 0, PER, SimCont2D.PERCEPTION_TARGET)
brain:setComponent(1, 1, 0, TAND)
brain:setComponent(2, 1, 0, TNAND)
brain:setComponent(2, 2, 0, MMAX)
brain:setComponent(3, 0, 0, TAND)
brain:setComponent(3, 1, 0, TAND)
brain:setComponent(3, 2, 0, TAND)

brain:setComponent(0, 0, 1, NOT)
brain:setComponent(1, 0, 1, ACT, SimCont2D.ACTION_ROTATE)
brain:setComponent(1, 1, 1, ACT, SimCont2D.ACTION_GO)
brain:setComponent(1, 2, 1, ACT, SimCont2D.ACTION_EATB)

agent:setBrain(brain)

brain:addConnection(0, 0, 0, 3, 0, 0, 0.5)
brain:addConnection(0, 0, 0, 2, 1, 0, 0.5)
brain:addConnection(0, 1, 0, 1, 1, 0, 0.16)
brain:addConnection(0, 1, 0, 2, 2, 0, 0.5)
brain:addConnection(0, 2, 0, 3, 2, 0, 0.5)
brain:addConnection(1, 1, 0, 3, 0, 0, 0.5)
brain:addConnection(1, 1, 0, 0, 0, 1, 0.5)
brain:addConnection(1, 1, 0, 2, 2, 0, 0.5)
brain:addConnection(1, 1, 0, 3, 2, 0, 0.5)
brain:addConnection(2, 1, 0, 3, 1, 0, 0.5)
brain:addConnection(2, 2, 0, 3, 0, 0, 0.5)
brain:addConnection(2, 2, 0, 3, 1, 0, 0.5)
brain:addConnection(3, 0, 0, 1, 0, 1, -0.5)
brain:addConnection(3, 1, 0, 1, 1, 1, 0.5)
brain:addConnection(3, 2, 0, 1, 2, 1, 0.5)
brain:addConnection(0, 0, 1, 1, 0, 1, 0.5)

-- Plants
--------------------------------------------------------------------------------

plant = GraphicalObject()

symSize = SymbolFloat(plantSize)
symTable = SymbolTable(symSize, sizeTableCode)
plant:addSymbolTable(symTable)
plant:setSymbolName("size", sizeTableCode, 0)

symPlantInitialEnergy = SymbolFloat(1.0)
symTable = SymbolTable(symPlantInitialEnergy, energyTableCode)
plant:addSymbolTable(symTable)
plant:setSymbolName("initial_energy", energyTableCode, 0)

symLowAgeLimit = SymbolUL(lowAgeLimit)
symHighAgeLimit = SymbolUL(highAgeLimit)
symTable = SymbolTable(symLowAgeLimit, ageTableCode)
symTable:addSymbol(symHighAgeLimit)
plant:addSymbolTable(symTable)
plant:setSymbolName("low_age_limit", ageTableCode, 0)
plant:setSymbolName("high_age_limit", ageTableCode, 1)

plantFood = SymbolFloat(1.0)
plantFood:setAlwaysRandom()
plantFoodTable = SymbolTable(plantFood, foodTableCode)
plantFoodTable:addSymbol(plantFood)
plant:addSymbolTable(plantFoodTable)
plant:setSymbolName("food", foodTableCode, 0)

graphic = GraphicGradient()
graphic:setSymbolName("food")
graphic:setReferenceSymbol(plantFood)
graphic:setColor1(0, 255, 0)
graphic:setColor2(255, 0, 0)

plant:addGraphic(graphic)

-- Population Dynamics
--------------------------------------------------------------------------------

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
dummyBrain:addPerception("Color", 0, SimCont2D.PERCEPTION_SYMBOL, colorTableCode, 0, colorTableCode, 0)
dummyBrain:addPerception("Food", 0, SimCont2D.PERCEPTION_SYMBOL, feedTableCode, 0, feedTableCode, 1)
dummyBrain:addPerception("Predator", 0, SimCont2D.PERCEPTION_SYMBOL, feedTableCode, 1, feedTableCode, 0)

-- Human Agent
--------------------------------------------------------------------------------

if humanAgent then
    human = Agent()
    dummyBrain = DummyBrain(1)
    dummyBrain:setChannelName(0, "objects")
    dummyBrain:addPerception("Position", 0, SimCont2D.PERCEPTION_POSITION)
    dummyBrain:addPerception("Distance", 0, SimCont2D.PERCEPTION_DISTANCE)
    dummyBrain:addPerception("Feed", 0, SimCont2D.PERCEPTION_SYMBOL, feedTableCode, 0, foodTableCode, 0)
    dummyBrain:addPerception("Target", 0, SimCont2D.PERCEPTION_TARGET)

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

    symHInitialEnergy = SymbolFloat(0.000001)
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

    humanFeed = SymbolFloat(0.0)
    humanFeedTable = SymbolTable(humanFeed, feedTableCode)
    human:addSymbolTable(humanFeedTable)
    human:setSymbolName("feed", feedTableCode, 0)

    human:addGraphic(GraphicTriangle())
    sim:addObject(human)
    sim:setPos(human, 300, 300)
    sim:setHuman(human)
end

-- Logs and Statistics
--------------------------------------------------------------------------------

stats = StatCommon()
stats:setFile("logs/energy" .. logSuffix .. ".csv")
stats:addField("energy")
popDyn:addDeathLog(agentSpeciesIndex, stats)

if logBrains then
    logBrain = LogBestBrain()
    logBrain:setFileNamePrefix("logs/brains/brain" .. logSuffix .. "t")
    logBrain:setFileNameSuffix(".svg")
    popDyn:addDeathLog(agentSpeciesIndex, logBrain)
end

-- Start Simulation
--------------------------------------------------------------------------------

popDyn:setLogTimeInterval(logTimeInterval)

sim:initGraphics(screenWidth, screenHeight, fullScreen, noGraphics)
sim:run()
