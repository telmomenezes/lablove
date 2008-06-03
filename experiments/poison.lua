-- POISON
-- Agents evolve to find and eat certain food items
-- Amount of energy gathered from food is realtive to symbol_distance(feed,food)
-- Food will be poisonous if symbol_binding(feed,food) < feedCenter
--------------------------------------------------------------------------------

dofile("basic_defines.lua")

-- Experiment Parameters
--------------------------------------------------------------------------------

numberOfPlants = 50
numberOfAgents = 25

agentSize = 10.0
plantSize = 10.0

worldWidth = 1000
worldHeight = 1000

alphaComponents = {AND, NOT, SUM, MUL, INV, NEG, MOD, AMP, RAND, GTZ, ZERO, MAX, AVG}
betaComponents = {AND, NOT, SUM, MUL, INV, NEG, MOD, AMP, RAND, GTZ, ZERO, CLK, MEMW, MEMC, MEMD}

viewRange = 150.0
viewAngle = 170.0

maxAge = 5000

initialEnergy = 1.0
goCost = 0.005
rotateCost = 0.005
goForceScale = 0.3
rotateForceScale = 0.006

drag = 0.05
rotDrag = 0.05

feedCenter = 0.25

compCount = 1
bufferSize = 100
fitnessAging = 0.1

addConnectionProb = 0.01
removeConnectionProb = 0.01
changeParamProb = 0.0
paramMutationStanDev = 0.0
splitConnectionProb = 0.01
joinConnectionsProb = 0.01
changeComponentProb = 0.0
changeInComponentProb = 0.2
swapComponentProb = 0.0

recombineProb = 1.0
recTree = false
recTerminal = false
geneGrouping = false

timeLimit = 0
logTimeInterval = 100
logBrains = true
logOnlyLastBrain = true

humanAgent = false

evolutionStopTime = 0

-- Command line, log file names, etc
--------------------------------------------------------------------------------

dofile("basic_command_line.lua")

addConnectionProb = getNumberParameter("addconnprob", addConnectionProb, "con+")
removeConnectionProb = getNumberParameter("removeconnprob", removeConnectionProb, "con-")
changeParamProb = getNumberParameter("changeparamprob", changeParamProb, "par")
paramMutationStanDev = getNumberParameter("parammutstandev", paramMutationStanDev, "psd")
splitConnectionProb = getNumberParameter("splitconnprob", splitConnectionProb, "spl")
joinConnectionsProb = getNumberParameter("joinconnprob", joinConnectionsProb, "joi")
changeComponentProb = getNumberParameter("changecompprob", changeComponentProb, "chg")
changeInComponentProb = getNumberParameter("changeincompprob", changeInComponentProb, "chin")
swapComponentProb = getNumberParameter("swapcompprob", swapComponentProb, "swp")
recombineProb = getNumberParameter("recombineprob", recombineProb, "rec")
bufferSize = getNumberParameter("buffersize", bufferSize, "buf")
compCount = getNumberParameter("compcount", compCount, "cc")
fitnessAging = getNumberParameter("fitnessaging", fitnessAging, "agi")
evolutionStopTime = getNumberParameter("evostop", evolutionStopTime, "est")
recTree = getBoolParameter("rectree", recTree, "rectree")
recTerminal = getBoolParameter("recterm", recTerminal, "recterm")
--viewRange = getNumberParameter("viewrange", viewRange, "vr")

recType = Gridbrain.RT_UNIFORM

if recTree then
    if recTerminal then
        recType = Gridbrain.RT_TREE_TERMINAL
    else
        recType = Gridbrain.RT_TREE
    end
end

logBaseName = "_poison_"

logSuffix = logBaseName
            .. parameterString
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

agent:setFitnessMeasure(Sim2D.FITNESS_ENERGY_SUM_ABOVE_INIT)

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

brain:setMutateAddConnectionProb(addConnectionProb)
brain:setMutateRemoveConnectionProb(removeConnectionProb)
brain:setMutateChangeParamProb(changeParamProb)
brain:setParamMutationStanDev(paramMutationStanDev)
brain:setMutateSplitConnectionProb(splitConnectionProb)
brain:setMutateJoinConnectionsProb(joinConnectionsProb)
brain:setMutateChangeComponentProb(changeComponentProb)
brain:setMutateChangeInactiveComponentProb(changeInComponentProb)
brain:setMutateSwapComponentProb(swapComponentProb)
brain:setRecombinationType(recType)
brain:setGeneGrouping(geneGrouping)

alphaSet = GridbrainComponentSet()
for i, comp in pairs(alphaComponents) do
    alphaSet:addComponent(comp)
end
alphaSet:addComponent(IN, Sim2D.PERCEPTION_POSITION)
alphaSet:addComponent(IN, Sim2D.PERCEPTION_DISTANCE)
alphaSet:addComponent(IN, Sim2D.PERCEPTION_TARGET)
alphaSet:addComponent(IN, Sim2D.PERCEPTION_SYMBOL, SYM_TO_SYM, feedTableCode, agentFeed:getID(), foodTableCode)

grid = Grid()
grid:init(ALPHA, 0, 0)
grid:setComponentSet(alphaSet)

brain:addGrid(grid, "objects");

betaSet = GridbrainComponentSet()
for i, comp in pairs(betaComponents) do
    betaSet:addComponent(comp)
end
betaSet:addComponent(OUT, Sim2D.ACTION_GO)
betaSet:addComponent(OUT, Sim2D.ACTION_ROTATE)
betaSet:addComponent(OUT, Sim2D.ACTION_EATB)
    
grid2 = Grid()
grid2:init(BETA, 0, 0)
grid2:setComponentSet(betaSet)

brain:addGrid(grid2, "beta")

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

popDyn = PopDynSpeciesBuffers()
sim:setPopulationDynamics(popDyn)

popDyn:setCompCount(compCount)
popDyn:setFitnessAging(fitnessAging)
popDyn:setRecombineProb(recombineProb)
agentSpeciesIndex = popDyn:addSpecies(agent, numberOfAgents, bufferSize)
popDyn:addSpecies(plant, numberOfPlants, 1)
popDyn:setEvolutionStopTime(evolutionStopTime)

-- Human Agent
--------------------------------------------------------------------------------

if humanAgent then
    human = SimObj2D()
    human:setSize(agentSize)
    human:setDrag(drag)
    human:setRotDrag(rotDrag)
    human:setInitialEnergy(1.0)
    human:setMaxAge(maxAge)
    human:setViewRange(viewRange)
    human:setViewAngle(viewAngle)
    human:setGoCost(goCost)
    human:setRotateCost(rotateCost)
    human:setGoForceScale(goForceScale)
    human:setRotateForceScale(rotateForceScale)
    human:setFeedCenter(feedCenter)
    human:setShape(SimObj2D.SHAPE_TRIANGLE)
    human:setColoringSymbolName("color")


    
    humanColor = SymbolRGB(82, 228, 241)
    symTable = SymbolTable(humanColor, colorTableCode)
    human:addSymbolTable(symTable)
    human:setSymbolName("color", colorTableCode, humanColor:getID())

    humanFeed = SymbolFloat(0.0)
    humanFeedTable = SymbolTable(humanFeed, feedTableCode)
    human:addSymbolTable(humanFeedTable)
    human:setSymbolName("feed", feedTableCode, humanFeed:getID())

    dummyBrain = DummyBrain(1)
    dummyBrain:setChannelName(0, "objects")
    dummyBrain:addPerception("Position", 0, Sim2D.PERCEPTION_POSITION)
    dummyBrain:addPerception("Orientation", 0, Sim2D.PERCEPTION_ORIENTATION)
    dummyBrain:addPerception("Distance", 0, Sim2D.PERCEPTION_DISTANCE)
    dummyBrain:addPerception("Feed", 0, Sim2D.PERCEPTION_SYMBOL, feedTableCode, humanFeed:getID(), foodTableCode)
    dummyBrain:addPerception("Target", 0, Sim2D.PERCEPTION_TARGET)

    human:setBrain(dummyBrain)

    sim:addObject(human)
    human:setPos(300, 300)
    sim:setHuman(human)
end

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
popDyn:addDeathLog(agentSpeciesIndex, stats)

if logBrains then
    logBrain = LogBestBrain()
    logBrain:setFileNameSuffix(".svg")
    if logOnlyLastBrain then
        logBrain:setLogOnlyLast(true)
        logBrain:setFileNamePrefix("brain" .. logSuffix)
    else
        logBrain:setFileNamePrefix("brain" .. logSuffix .. "t")
    end
    popDyn:addDeathLog(agentSpeciesIndex, logBrain)
end

popDyn:setLogTimeInterval(logTimeInterval)

-- Start Simulation
--------------------------------------------------------------------------------

sim:initGraphics(screenWidth, screenHeight, fullScreen, noGraphics)
sim:run()
