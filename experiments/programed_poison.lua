-- PROGRAMED POISON
-- Human programed agents that find and eat certain food items
-- Amount of energy gathered from food is realtive to symbol_distance(feed,food)
-- Food will be poisonous if symbol_distance(feed,food) > 0.5

dofile("basic_defines.lua")

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

maxAge = 5000

initialEnergy = 1.0
metabolism = 0.0
goCost = 0.005
rotateCost = 0.0
goForceScale = 0.3
rotateForceScale = 0.006

friction = 0.0
drag = 0.05
rotFriction = 0.0
rotDrag = 0.05

feedCenter = 0.25

timeLimit = 0

logTimeInterval = 100
logBrains = true

-- Command line, log file names, etc
--------------------------------------------------------------------------------

dofile("basic_command_line.lua")

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
sim:setFeedCenter(feedCenter)

-- Agents
--------------------------------------------------------------------------------

agent = Agent()

agent:setFitnessMeasure(SimCont2D.FITNESS_ENERGY_SUM_ABOVE_INIT)
agent:addGraphic(GraphicTriangle())

symSize = SymbolFloat(agentSize)
symTable = SymbolTable(symSize)
agent:addSymbolTable(symTable)
sizeTableCode = symTable:getID()
agent:setSymbolName("size", sizeTableCode, symSize:getID())

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
agent:setSymbolName("friction", physicsTableCode, symFriction:getID())
agent:setSymbolName("drag", physicsTableCode, symDrag:getID())
agent:setSymbolName("rot_friction", physicsTableCode, symRotFriction:getID())
agent:setSymbolName("rot_drag", physicsTableCode, symRotDrag:getID())

symInitialEnergy = SymbolFloat(initialEnergy)
symMetabolism = SymbolFloat(metabolism)
symTable = SymbolTable(symInitialEnergy)
symTable:addSymbol(symMetabolism)
agent:addSymbolTable(symTable)
energyTableCode = symTable:getID()
agent:setSymbolName("initial_energy", energyTableCode, symInitialEnergy:getID())
agent:setSymbolName("metabolism", energyTableCode, symMetabolism:getID())

symLowAgeLimit = SymbolUL(maxAge)
symHighAgeLimit = SymbolUL(maxAge)
symTable = SymbolTable(symLowAgeLimit)
symTable:addSymbol(symHighAgeLimit)
agent:addSymbolTable(symTable)
ageTableCode = symTable:getID()
agent:setSymbolName("low_age_limit", ageTableCode, symLowAgeLimit:getID())
agent:setSymbolName("high_age_limit", ageTableCode, symHighAgeLimit:getID())

agentColor = SymbolRGB(0, 0, 255)
symTable = SymbolTable(agentColor)
agent:addSymbolTable(symTable)
colorTableCode = symTable:getID()
agent:setSymbolName("color", colorTableCode, agentColor:getID())

agentFeed = SymbolFloat(0.0)
agentFeedTable = SymbolTable(agentFeed)
agent:addSymbolTable(agentFeedTable)
feedTableCode = agentFeedTable:getID()
dummyTable = SymbolTable(agentFeed)
foodTableCode = dummyTable:getID()
agent:setSymbolName("feed", feedTableCode, agentFeed:getID())

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

brain:init()

brain:setComponent(0, 0, 0, PER, SimCont2D.PERCEPTION_POSITION)
brain:setComponent(0, 1, 0, PER, SimCont2D.PERCEPTION_SYMBOL, SYM_TO_SYM, feedTableCode, agentFeed:getID(), foodTableCode)
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

brain:addConnectionReal(0, 0, 0, 3, 0, 0, 1.0)
brain:addConnectionReal(0, 0, 0, 2, 1, 0, 1.0)
brain:addConnectionReal(0, 1, 0, 1, 1, 0, 0.1 / feedCenter)
brain:addConnectionReal(0, 1, 0, 2, 2, 0, 1.0)
brain:addConnectionReal(0, 2, 0, 3, 2, 0, 1.0)
brain:addConnectionReal(1, 1, 0, 3, 0, 0, 1.0)
brain:addConnectionReal(1, 1, 0, 0, 0, 1, 1.0)
brain:addConnectionReal(1, 1, 0, 2, 2, 0, 1.0)
brain:addConnectionReal(1, 1, 0, 3, 2, 0, 1.0)
brain:addConnectionReal(2, 1, 0, 3, 1, 0, 1.0)
brain:addConnectionReal(2, 2, 0, 3, 0, 0, 1.0)
brain:addConnectionReal(2, 2, 0, 3, 1, 0, 1.0)
brain:addConnectionReal(3, 0, 0, 1, 0, 1, 1.0)
brain:addConnectionReal(3, 1, 0, 1, 1, 1, 1.0)
brain:addConnectionReal(3, 2, 0, 1, 2, 1, 1.0)
brain:addConnectionReal(0, 0, 1, 1, 0, 1, 1.0)

agent:setBrain(brain)

-- Plants
--------------------------------------------------------------------------------

plant = GraphicalObject()

symSize = SymbolFloat(plantSize)
symTable = SymbolTable(symSize, sizeTableCode)
plant:addSymbolTable(symTable)
plant:setSymbolName("size", sizeTableCode, symSize:getID())

symPlantInitialEnergy = SymbolFloat(1.0)
symTable = SymbolTable(symPlantInitialEnergy, energyTableCode)
plant:addSymbolTable(symTable)
plant:setSymbolName("initial_energy", energyTableCode, symPlantInitialEnergy:getID())

symLowAgeLimit = SymbolUL(maxAge)
symHighAgeLimit = SymbolUL(maxAge)
symTable = SymbolTable(symLowAgeLimit, ageTableCode)
symTable:addSymbol(symHighAgeLimit)
plant:addSymbolTable(symTable)
plant:setSymbolName("low_age_limit", ageTableCode, symLowAgeLimit:getID())
plant:setSymbolName("high_age_limit", ageTableCode, symHighAgeLimit:getID())

plantFood = SymbolFloat(0.0)
plantFood:setAlwaysRandom()
plantFoodTable = SymbolTable(plantFood, foodTableCode)
plant:addSymbolTable(plantFoodTable)
plant:setSymbolName("food", foodTableCode, plantFood:getID())

graphic = GraphicGradient()
graphic:setSymbolName("food")
graphic:setReferenceSymbol(agentFeed)
graphic:setColor1(255, 0, 0)
graphic:setColor2(0, 255, 0)
graphic:setCenter(feedCenter)

plant:addGraphic(graphic)

-- Population Dynamics
--------------------------------------------------------------------------------

popDyn = PopDynSpeciesBuffers()
sim:setPopulationDynamics(popDyn)

agentSpeciesIndex = popDyn:addSpecies(agent, numberOfAgents, 1, false)
popDyn:addSpecies(plant, numberOfPlants, 1)

-- Logs and Statistics
--------------------------------------------------------------------------------

stats = StatCommon()
stats:setFile("energy" .. logSuffix .. ".csv")
stats:addField("fitness")
stats:addField("energy")
popDyn:addDeathLog(agentSpeciesIndex, stats)

if logBrains then
    logBrain = LogBestBrain()
    logBrain:setFileNamePrefix("brain" .. logSuffix .. "t")
    logBrain:setFileNameSuffix(".svg")
    popDyn:addDeathLog(agentSpeciesIndex, logBrain)
end

-- Start Simulation
--------------------------------------------------------------------------------

popDyn:setLogTimeInterval(logTimeInterval)

sim:initGraphics(screenWidth, screenHeight, fullScreen, noGraphics)
sim:run()
