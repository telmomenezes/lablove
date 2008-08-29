-- PROGRAMED POISON
-- Human programed agents that find and eat certain food items
-- Amount of energy gathered from food is realtive to symbol_distance(feed,food)
-- Food will be poisonous if symbol_distance(feed,food) > 0.5

-- Experiment Parameters
--------------------------------------------------------------------------------

numberOfPlants = 50
numberOfAgents = 25

agentSize = 10.0
plantSize = 10.0

worldWidth = 1000
worldHeight = 1000

viewRange = 150.0
viewAngle = 170.0

maxAge = 5000

initialEnergy = 1.0
metabolism = 0.0
goCost = 0.005
rotateCost = 0.005
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

sim = Sim2D()
sim:setWorldDimensions(worldWidth, worldHeight, 250)
sim:setSeedIndex(seedIndex)
sim:setTimeLimit(timeLimit)

-- Agents
--------------------------------------------------------------------------------

agent = SimObj2D()

agent:setSize(agentSize)
agent:setDrag(drag)
agent:setRotDrag(rotDrag)
agent:setInitialEnergy(initialEnergy)
agent:setMaxAge(maxAge)
agent:setViewRange(viewRange)
agent:setViewAngle(viewAngle)
agent:setGoCost(goCost)
agent:setRotateCost(rotateCost)
agent:setGoForceScale(goForceScale)
agent:setRotateForceScale(rotateForceScale)
agent:setFeedCenter(feedCenter)
agent:setShape(SimObj2D.SHAPE_TRIANGLE)
agent:setColoringSymbolName("color")

agentColor = SymbolRGB(0, 0, 255)
symTable = SymbolTable(agentColor)
agent:addSymbolTable(symTable)
colorTableCode = symTable:getID()
agent:setSymbolName("color", colorTableCode, agentColor:getID())

agentFeed = SymbolFloat(0.0)
agentFeedTable = SymbolTable(agentFeed)
agentFeedTable:setName("food")
agent:addSymbolTable(agentFeedTable)
feedTableCode = agentFeedTable:getID()
dummyTable = SymbolTable(agentFeed)
foodTableCode = dummyTable:getID()
agent:setSymbolName("feed", feedTableCode, agentFeed:getID())

-- Agent Brain

brain = Gridbrain()

brain:setMutateAddConnectionProb(0)
brain:setMutateRemoveConnectionProb(0)
brain:setMutateChangeParamProb(0)
brain:setMutateChangeComponentProb(0)
brain:setParamMutationStanDev(0)

grid = Grid()
grid:init(Grid.ALPHA, 5, 3)
brain:addGrid(grid, "objects");

grid2 = Grid()
grid2:init(Grid.BETA, 2, 3)
brain:addGrid(grid2, "beta")

brain:init()

brain:setComponent(0, 0, 0, PER(Sim2D.PERCEPTION_POSITION))
brain:setComponent(0, 1, 0, PER(Sim2D.PERCEPTION_SYMPRO, feedTableCode, agentFeed:getID(), foodTableCode))
brain:setComponent(0, 2, 0, PER(Sim2D.PERCEPTION_TARGET))
brain:setComponent(1, 1, 0, AMP(((-1 / ((0.1 / feedCenter) + 1))) + 1))
brain:setComponent(2, 1, 0, AND())
brain:setComponent(3, 1, 0, NOT())
brain:setComponent(3, 2, 0, MAX())
brain:setComponent(4, 0, 0, AND())
brain:setComponent(4, 1, 0, AND())
brain:setComponent(4, 2, 0, AND())

brain:setComponent(0, 0, 1, NOT())
brain:setComponent(1, 0, 1, ACT(Sim2D.ACTION_ROTATE))
brain:setComponent(1, 1, 1, ACT(Sim2D.ACTION_GO))
brain:setComponent(1, 2, 1, ACT(Sim2D.ACTION_EATB))

brain:addConnection(0, 0, 0, 4, 0, 0)
brain:addConnection(0, 0, 0, 3, 1, 0)
brain:addConnection(0, 1, 0, 1, 1, 0)
brain:addConnection(1, 1, 0, 2, 1, 0)
brain:addConnection(0, 1, 0, 3, 2, 0)
brain:addConnection(0, 2, 0, 4, 2, 0)
brain:addConnection(2, 1, 0, 4, 0, 0)
brain:addConnection(2, 1, 0, 0, 0, 1)
brain:addConnection(2, 1, 0, 3, 2, 0)
brain:addConnection(2, 1, 0, 4, 2, 0)
brain:addConnection(3, 1, 0, 4, 1, 0)
brain:addConnection(3, 2, 0, 4, 0, 0)
brain:addConnection(3, 2, 0, 4, 1, 0)
brain:addConnection(4, 0, 0, 1, 0, 1)
brain:addConnection(4, 1, 0, 1, 1, 1)
brain:addConnection(4, 2, 0, 1, 2, 1)
brain:addConnection(0, 0, 1, 1, 0, 1)

agent:setBrain(brain)

-- Plants
--------------------------------------------------------------------------------

plant = SimObj2D()
--plant:setBirthRadius(50.0)
plant:setSize(plantSize)
plant:setInitialEnergy(1.0)
plant:setMaxAge(maxAge)

plantFood = SymbolFloat(0.0)
plantFood:setAlwaysRandom()
plantFoodTable = SymbolTable(plantFood, foodTableCode)
plant:addSymbolTable(plantFoodTable)
plant:setSymbolName("food", foodTableCode, plantFood:getID())

plant:setShape(SimObj2D.SHAPE_SQUARE)
plant:setColoringScale("food", plantFood, feedCenter, 255, 0, 0, 0, 255, 0)

-- Population Dynamics
--------------------------------------------------------------------------------

popDyn = PopDynSEGA()
sim:setPopulationDynamics(popDyn)

agentSpecies = Species(agent, numberOfAgents, 1)
agentSpecies:addGoal(SimObj2D.FITNESS_ENERGY_SUM_ABOVE_INIT)
agentSpecies:setFitnessAging(0)
agentSpecies:setRecombineProb(0)

plantSpecies = Species(plant, numberOfPlants, 1)
plantSpecies:addGoal(SimObj2D.FITNESS_ENERGY)

agentSpeciesIndex = popDyn:addSpecies(agentSpecies)
popDyn:addSpecies(plantSpecies)

-- Logs and Statistics
--------------------------------------------------------------------------------

stats = StatCommon()
stats:setFile("log" .. logSuffix .. ".csv")
stats:addField("fitness")
stats:addField("energy")
stats:addField("gb_connections")
stats:addField("gb_active_connections")
stats:addField("gb_active_components")
stats:addField("gb_grid_width_objects")
stats:addField("gb_grid_height_objects")
stats:addField("gb_grid_width_beta")
stats:addField("gb_grid_height_beta")
agentSpecies:addDeathLog(stats)

if logBrains then
    logBrain = LogBestBrain()
    logBrain:setFileNameSuffix(".svg")
    if logOnlyLastBrain then
        logBrain:setLogOnlyLast(true)
        logBrain:setFileNamePrefix("brain" .. logSuffix)
    else
        logBrain:setFileNamePrefix("brain" .. logSuffix .. "t")
    end
    agentSpecies:addDeathLog(logBrain)
end

popDyn:setLogTimeInterval(logTimeInterval)

-- Start Simulation
--------------------------------------------------------------------------------

popDyn:setLogTimeInterval(logTimeInterval)

sim:initGraphics(screenWidth, screenHeight, fullScreen, noGraphics)
sim:run()
