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

compCount = 1
bufferSize = 100
fitnessAging = 0.1

addConnectionProb = 0.01
removeConnectionProb = 0.01
changeParamProb = 0.0
paramMutationStanDev = 0.0
splitConnectionProb = 0.0
joinConnectionsProb = 0.0
changeComponentProb = 0.0
swapComponentProb = 0.0

recombineProb = 0.0
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
swapComponentProb = getNumberParameter("swapcompprob", swapComponentProb, "swp")
recombineProb = getNumberParameter("recombineprob", recombineProb, "rec")
bufferSize = getNumberParameter("buffersize", bufferSize, "buf")
compCount = getNumberParameter("compcount", compCount, "cc")
fitnessAging = getNumberParameter("fitnessaging", fitnessAging, "agi")
evolutionStopTime = getNumberParameter("evostop", evolutionStopTime, "est")
--viewRange = getNumberParameter("viewrange", viewRange, "vr")

logBaseName = "_poison_"

logSuffix = logBaseName
            .. parameterString
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
brain:setMutateSwapComponentProb(swapComponentProb)

brain:setGeneGrouping(geneGrouping)

alphaSet = GridbrainComponentSet()
for i, comp in pairs(alphaComponents) do
    alphaSet:addComponent(comp)
end
alphaSet:addComponent(IN, SimCont2D.PERCEPTION_POSITION)
alphaSet:addComponent(IN, SimCont2D.PERCEPTION_DISTANCE)
alphaSet:addComponent(IN, SimCont2D.PERCEPTION_TARGET)
alphaSet:addComponent(IN, SimCont2D.PERCEPTION_SYMBOL, SYM_TO_SYM, feedTableCode, agentFeed:getID(), foodTableCode)

grid = Grid()
grid:init(ALPHA, 0, 0)
grid:setComponentSet(alphaSet)

brain:addGrid(grid, "objects");

betaSet = GridbrainComponentSet()
for i, comp in pairs(betaComponents) do
    betaSet:addComponent(comp)
end
betaSet:addComponent(OUT, SimCont2D.ACTION_GO)
betaSet:addComponent(OUT, SimCont2D.ACTION_ROTATE)
betaSet:addComponent(OUT, SimCont2D.ACTION_EATB)
    
grid2 = Grid()
grid2:init(BETA, 0, 0)
grid2:setComponentSet(betaSet)

brain:addGrid(grid2, "beta")

agent:setBrain(brain)

-- Plants
--------------------------------------------------------------------------------

plant = GraphicalObject()
--plant:setBirthRadius(50.0)

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
graphic:setReferenceSymbol(plantFood)
graphic:setColor1(255, 0, 0)
graphic:setColor2(0, 255, 0)
graphic:setCenter(feedCenter)

plant:addGraphic(graphic)

-- Population Dynamics
--------------------------------------------------------------------------------

popDyn = PopDynSpeciesBuffers()
sim:setPopulationDynamics(popDyn)

popDyn:setCompCount(compCount)
popDyn:setFitnessAging(fitnessAging)
popDyn:setRecombineProb(recombineProb)
agentSpeciesIndex = popDyn:addSpecies(agent, numberOfAgents, bufferSize)
popDyn:addSpecies(plant, numberOfPlants, 1, 1)
popDyn:setEvolutionStopTime(evolutionStopTime)

-- Human Agent
--------------------------------------------------------------------------------

if humanAgent then
    human = Agent()
    
    symSize = SymbolFloat(agentSize)
    symTable = SymbolTable(symSize, sizeTableCode)
    human:addSymbolTable(symTable)
    human:setSymbolName("size", sizeTableCode, symSize:getID())

    symHFriction = SymbolFloat(friction)
    symHDrag = SymbolFloat(drag)
    symHRotFriction = SymbolFloat(rotFriction)
    symHRotDrag = SymbolFloat(rotDrag)
    symTable = SymbolTable(symFriction, physicsTableCode)
    symTable:addSymbol(symHDrag)
    symTable:addSymbol(symHRotFriction)
    symTable:addSymbol(symHRotDrag)
    human:addSymbolTable(symTable)
    human:setSymbolName("friction", physicsTableCode, symHFriction:getID())
    human:setSymbolName("drag", physicsTableCode, symHDrag:getID())
    human:setSymbolName("rot_friction", physicsTableCode, symHRotFriction:getID())
    human:setSymbolName("rot_drag", physicsTableCode, symHRotDrag:getID())

    symHInitialEnergy = SymbolFloat(1.0)
    symHMetabolism = SymbolFloat(metabolism)
    symTable = SymbolTable(symHInitialEnergy, energyTableCode)
    symTable:addSymbol(symHMetabolism)
    human:addSymbolTable(symTable)
    human:setSymbolName("initial_energy", energyTableCode, symHInitialEnergy:getID())
    human:setSymbolName("metabolism", energyTableCode, symHMetabolism:getID())

    symHLowAgeLimit = SymbolUL(maxAge)
    symHHighAgeLimit = SymbolUL(maxAge)
    symTable = SymbolTable(symHLowAgeLimit, ageTableCode)
    symTable:addSymbol(symHHighAgeLimit)
    human:addSymbolTable(symTable)
    human:setSymbolName("low_age_limit", ageTableCode, symHLowAgeLimit:getID())
    human:setSymbolName("high_age_limit", ageTableCode, symHHighAgeLimit:getID())

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
    dummyBrain:addPerception("Position", 0, SimCont2D.PERCEPTION_POSITION)
    dummyBrain:addPerception("Distance", 0, SimCont2D.PERCEPTION_DISTANCE)
    dummyBrain:addPerception("Feed", 0, SimCont2D.PERCEPTION_SYMBOL, feedTableCode, humanFeed:getID(), foodTableCode)
    dummyBrain:addPerception("Target", 0, SimCont2D.PERCEPTION_TARGET)

    human:setBrain(dummyBrain)

    human:addGraphic(GraphicTriangle())
    sim:addObject(human)
    sim:setPos(human, 300, 300)
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
