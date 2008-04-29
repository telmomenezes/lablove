-- PREDPREY
-- Two species of agents (preys and predators) coevolve
-- Preys can gather energy by eating food item
-- Predators can gather energy by eating the preys
--------------------------------------------------------------------------------

dofile("basic_defines.lua")

-- Experiment Parameters
--------------------------------------------------------------------------------

numberOfPlants = 200
numberOfPreds = 50
numberOfPreys = 50

preySize = 10.0
predSize = 5.0
plantSize = 10.0

worldWidth = 3000
worldHeight = 3000

alphaComponents = {TAND, TNAND, NOT, THR, STHR, INV, MMAX}
betaComponents = {TAND, TNAND, NOT, THR, STHR, INV, RAND, CLK, MEMA, MEMC, MEMT, MEMW}
betaComponents = {TAND, TNAND, NOT, THR, STHR, INV, RAND, CLK}

viewRange = 200.0
viewAngle = 170.0

maxAge = 5000

initialEnergy = 1.0
metabolism = 0.0
goCost = 0.001
rotateCost = 0.0
goForceScale = 0.7
rotateForceScale = 0.006

friction = 0.0
drag = 0.05
rotFriction = 0.0
rotDrag = 0.05

compCount = 1
bufferSize = 100
fitnessAging = 0.1

addConnectionProb = 0.005
removeConnectionProb = 0.01
changeWeightProb = 0.01
weightMutationStanDev = 1.0
splitConnectionProb = 0.005
changeInactiveComponentProb = 0.01
swapComponentProb = 0.1

recombineProb = 1.0

evoPred = true
evoPrey = true

timeLimit = 0
logTimeInterval = 100
logBrains = true
logOnlyLastBrain = true

humanAgent = false

birthRadius = 500.0

soundRange = 1000
speakInterval = 250

-- Command line, log file names, etc
--------------------------------------------------------------------------------

dofile("basic_command_line.lua")

birthRadius = getNumberParameter("birthrad", birthRadius, "brad")
soundRange = getNumberParameter("sndrange", soundRange, "srg")
speakInterval = getNumberParameter("spkint", speakInterval, "spki")
evoPred = getBoolParameter("evopred", evoPred, "evopred")
evoPrey = getBoolParameter("evoprey", evoPrey, "evoprey")

logBaseName = "_predprey_"

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
sim:setFeedCenter(0.9)
sim:setSoundRange(soundRange)
sim:setSpeakInterval(speakInterval)

-- Population Dynamics
--------------------------------------------------------------------------------

popDyn = PopDynSpeciesBuffers()
sim:setPopulationDynamics(popDyn)

popDyn:setCompCount(compCount)
popDyn:setFitnessAging(fitnessAging)
popDyn:setRecombineProb(recombineProb)

popDyn:setLogTimeInterval(logTimeInterval)

-- Generic Agent Function
--------------------------------------------------------------------------------
sizeTableCode = 0
physicsTableCode = 1 
energyTableCode = 2
ageTableCode = 3
colorTableCode = 4
feedTableCode = 5

function addAgentSpecies(name, pop, red, green, blue, feed, food, evo, size)
    agent = Agent()
    agent:setBirthRadius(birthRadius)

    if evo then
        agent:setFitnessMeasure(SimCont2D.FITNESS_ENERGY_SUM_ABOVE_INIT)
    else
        agent:setFitnessMeasure(SimCont2D.FITNESS_RANDOM)
    end

    agent:addGraphic(GraphicTriangle())

    symSize = SymbolFloat(size)
    symTable = SymbolTable(symSize, sizeTableCode)
    agent:addSymbolTable(symTable)
    agent:setSymbolName("size", sizeTableCode, symSize:getID())

    symFriction = SymbolFloat(friction)
    symDrag = SymbolFloat(drag)
    symRotFriction = SymbolFloat(rotFriction)
    symRotDrag = SymbolFloat(rotDrag)
    symTable = SymbolTable(symFriction, physicsTableCode)
    symTable:addSymbol(symDrag)
    symTable:addSymbol(symRotFriction)
    symTable:addSymbol(symRotDrag)
    agent:addSymbolTable(symTable)
    agent:setSymbolName("friction", physicsTableCode, symFriction:getID())
    agent:setSymbolName("drag", physicsTableCode, symDrag:getID())
    agent:setSymbolName("rot_friction", physicsTableCode, symRotFriction:getID())
    agent:setSymbolName("rot_drag", physicsTableCode, symRotDrag:getID())

    symInitialEnergy = SymbolFloat(initialEnergy)
    symMetabolism = SymbolFloat(metabolism)
    symTable = SymbolTable(symInitialEnergy, energyTableCode)
    symTable:addSymbol(symMetabolism)
    agent:addSymbolTable(symTable)
    agent:setSymbolName("initial_energy", energyTableCode, symInitialEnergy:getID())
    agent:setSymbolName("metabolism", energyTableCode, symMetabolism:getID())

    symLowAgeLimit = SymbolUL(maxAge)
    symHighAgeLimit = SymbolUL(maxAge)
    symTable = SymbolTable(symLowAgeLimit, ageTableCode)
    symTable:addSymbol(symHighAgeLimit)
    agent:addSymbolTable(symTable)
    agent:setSymbolName("low_age_limit", ageTableCode, symLowAgeLimit:getID())
    agent:setSymbolName("high_age_limit", ageTableCode, symHighAgeLimit:getID())
    
    agentColor = SymbolRGB(red, green, blue)
    symTable = SymbolTable(agentColor, colorTableCode)
    symTable:setDynamic(true)
    symTable:setName("color")
    agent:addSymbolTable(symTable)
    agent:setSymbolName("color", colorTableCode, agentColor:getID())

    symFeed = SymbolBitString(feed)
    symFood = SymbolBitString(food)
    agentFeedTable = SymbolTable(symFeed, feedTableCode)
    agentFeedTable:addSymbol(symFood)
    agent:addSymbolTable(agentFeedTable)
    agent:setSymbolName("feed", feedTableCode, symFeed:getID())
    agent:setSymbolName("food", feedTableCode, symFood:getID())

    -- Agent Brain

    brain = Gridbrain()

    brain:setMutateAddConnectionProb(addConnectionProb)
    brain:setMutateRemoveConnectionProb(removeConnectionProb)
    brain:setMutateChangeConnectionWeightProb(changeWeightProb)
    brain:setWeightMutationStanDev(weightMutationStanDev)
    brain:setMutateSplitConnectionProb(splitConnectionProb)
    brain:setMutateChangeInactiveComponentProb(changeInactiveComponentProb)
    brain:setMutateSwapComponentProb(swapComponentProb)

    brain:setGrowMethod(Gridbrain.GM_PRESSURE)
    brain:setCloneConnectionsMode(Gridbrain.CC_ALL_PLUS)
    brain:setMutationScope(Gridbrain.MS_ALL)

    alphaSet = GridbrainComponentSet()
    for i, comp in pairs(alphaComponents) do
        alphaSet:addComponent(comp)
    end
    alphaSet:addComponent(PER, SimCont2D.PERCEPTION_POSITION)
    alphaSet:addComponent(PER, SimCont2D.PERCEPTION_DISTANCE)
    alphaSet:addComponent(PER, SimCont2D.PERCEPTION_TARGET)
    alphaSet:addComponent(PER, SimCont2D.PERCEPTION_SYMBOL, TAB_TO_SYM, colorTableCode, agentColor:getID(), colorTableCode)

    grid = Grid()
    grid:init(ALPHA, 0, 0)
    grid:setComponentSet(alphaSet)

    brain:addGrid(grid, "objects");

    soundSet = GridbrainComponentSet()
    for i, comp in pairs(alphaComponents) do
        soundSet:addComponent(comp)
    end
    soundSet:addComponent(PER, SimCont2D.PERCEPTION_POSITION)
    soundSet:addComponent(PER, SimCont2D.PERCEPTION_DISTANCE)
    soundSet:addComponent(PER, SimCont2D.PERCEPTION_SYMBOL, TAB_TO_SYM, colorTableCode, agentColor:getID(), colorTableCode)

    soundGrid = Grid()
    soundGrid:init(ALPHA, 0, 0)
    soundGrid:setComponentSet(soundSet)

    brain:addGrid(soundGrid, "sounds");

    selfSet = GridbrainComponentSet()
    for i, comp in pairs(alphaComponents) do
        selfSet:addComponent(comp)
    end
    selfSet:addComponent(PER, SimCont2D.PERCEPTION_ENERGY)
    selfSet:addComponent(PER, SimCont2D.PERCEPTION_CAN_SPEAK)

    selfGrid = Grid()
    selfGrid:init(ALPHA, 0, 0)
    selfGrid:setComponentSet(selfSet)

    brain:addGrid(selfGrid, "self");

    betaSet = GridbrainComponentSet()
    for i, comp in pairs(betaComponents) do
        betaSet:addComponent(comp)
    end
    betaSet:addComponent(ACT, SimCont2D.ACTION_GO)
    betaSet:addComponent(ACT, SimCont2D.ACTION_ROTATE)
    betaSet:addComponent(ACT, SimCont2D.ACTION_EAT)
    betaSet:addComponent(ACT, SimCont2D.ACTION_SPEAK, TAB_TO_SYM, colorTableCode, agentColor:getID())
    
    grid2 = Grid()
    grid2:init(BETA, 0, 0)
    grid2:setComponentSet(betaSet)

    brain:addGrid(grid2, "beta")

    agent:setBrain(brain)

    agentSpeciesIndex = popDyn:addSpecies(agent, pop, bufferSize, true)

    -- Logs and Statistics

    logLabel = logSuffix .. "_" .. name

    stats = StatCommon()
    stats:setFile("log" .. logLabel .. ".csv")
    stats:addField("fitness")
    stats:addField("energy")
    stats:addField("gb_connections")
    stats:addField("gb_active_connections")
    stats:addField("gb_grid_width_objects")
    stats:addField("gb_grid_height_objects")
    stats:addField("gb_grid_width_beta")
    stats:addField("gb_grid_height_beta")
    stats:addField("symtable_size_color")
    stats:addField("gb_active_perceptions_table_" .. colorTableCode)
    popDyn:addDeathLog(agentSpeciesIndex, stats)

    if logBrains then
        logBrain = LogBestBrain()
        logBrain:setFileNameSuffix(".svg")
        if logOnlyLastBrain then
            logBrain:setLogOnlyLast(true)
            logBrain:setFileNamePrefix("brain" .. logLabel)
        else
            logBrain:setFileNamePrefix("brain" .. logLabel .. "t")
        end
        popDyn:addDeathLog(agentSpeciesIndex, logBrain)
    end
end

-- Generic Plant Function
--------------------------------------------------------------------------------

function addPlantSpecies(pop, red, green, blue, feed, food)
    plant = GraphicalObject()
    plant:setBirthRadius(birthRadius)

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

    plantColor = SymbolRGB(red, green, blue)
    symTable = SymbolTable(plantColor, colorTableCode)
    plant:addSymbolTable(symTable)
    plant:setSymbolName("color", colorTableCode, plantColor:getID())

    symFeed = SymbolBitString(feed)
    symFood = SymbolBitString(food)
    plantFeedTable = SymbolTable(symFeed, feedTableCode)
    plantFeedTable:addSymbol(symFood)
    plant:addSymbolTable(plantFeedTable)
    plant:setSymbolName("feed", feedTableCode, symFeed:getID())
    plant:setSymbolName("food", feedTableCode, symFood:getID())

    plant:addGraphic(GraphicSquare())

    popDyn:addSpecies(plant, pop, 1)
end

-- Create Species
--------------------------------------------------------------------------------

addAgentSpecies("pred", numberOfPreds, 255, 0, 0, "11", "01", evoPred, predSize)
addAgentSpecies("prey", numberOfPreys, 0, 0, 255, "00", "11", evoPrey, preySize)
addPlantSpecies(numberOfPlants, 0, 255, 0, "00", "00")

-- Human Agent
--------------------------------------------------------------------------------

if humanAgent then
    human = Agent()
    dummyBrain = DummyBrain(1)
    dummyBrain:setChannelName(0, "objects")
    dummyBrain:addPerception("Position", 0, SimCont2D.PERCEPTION_POSITION)
    dummyBrain:addPerception("Distance", 0, SimCont2D.PERCEPTION_DISTANCE)
    dummyBrain:addPerception("Target", 0, SimCont2D.PERCEPTION_TARGET)

    human:setBrain(dummyBrain)

    symSize = SymbolFloat(preySize)
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

    feed = "11"
    food = "01"
    symFeed = SymbolBitString(feed)
    symFood = SymbolBitString(food)
    humanFeedTable = SymbolTable(symFeed, feedTableCode)
    humanFeedTable:addSymbol(symFood)
    human:addSymbolTable(humanFeedTable)
    human:setSymbolName("feed", feedTableCode, symFeed:getID())
    human:setSymbolName("food", feedTableCode, symFood:getID())

    human:addGraphic(GraphicTriangle())
    sim:addObject(human)
    sim:setPos(human, 300, 300)
    sim:setHuman(human)
end

-- Start Simulation
--------------------------------------------------------------------------------

sim:initGraphics(screenWidth, screenHeight, fullScreen, noGraphics)
sim:run()
