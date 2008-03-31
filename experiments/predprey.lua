-- PREDPREY
-- Two species of agents (preys and predators) coevolve
-- Preys can gather energy by eating food item
-- Predators can gather energy by eating the preys
--------------------------------------------------------------------------------

dofile("basic_defines.lua")

-- Experiment Parameters
--------------------------------------------------------------------------------

numberOfPlants = 100
numberOfPreds = 50
numberOfPreys = 50

agentSize = 10.0
plantSize = 10.0

worldWidth = 3000
worldHeight = 3000

alphaWidth = 0
betaWidth = 0
alphaHeight = 0
betaHeight = 0

alphaComponents = {TAND, TNAND, MMAX}
betaComponents = {TAND, TNAND}

viewRange = 150.0
viewAngle = 170.0

maxAge = 5000

initialEnergy = 1.0
metabolism = 0.0
goCost = 0.00--5
rotateCost = 0.0
goForceScale = 0.3
rotateForceScale = 0.006

friction = 0.0
drag = 0.05
rotFriction = 0.0
rotDrag = 0.05

compCount = 1
bufferSize = 100
fitnessAging = 0.1

addConnectionProb = 0.01
removeConnectionProb = 0.02
changeWeightProb = 0.01
weightMutationStanDev = 1.0
splitConnectionProb = 0.01
changeComponentProb = 0.01
swapComponentProb = 0.1

recombineProb = 0.0

evoPred = true
evoPrey = true

timeLimit = 0
logTimeInterval = 100
logBrains = true
logOnlyLastBrain = true

humanAgent = true

birthRadius = 0.0

-- Command line, log file names, etc
--------------------------------------------------------------------------------

dofile("basic_command_line.lua")

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

function addAgentSpecies(name, pop, red, green, blue, feed, food, evo)
    agent = Agent()
    agent:setBirthRadius(birthRadius)

    if evo then
        agent:setFitnessMeasure(SimCont2D.FITNESS_ENERGY_SUM_ABOVE_INIT)
    else
        agent:setFitnessMeasure(SimCont2D.FITNESS_RANDOM)
    end

    agent:addGraphic(GraphicTriangle())

    symSize = SymbolFloat(agentSize)
    symTable = SymbolTable(symSize, sizeTableCode)
    agent:addSymbolTable(symTable)
    agent:setSymbolName("size", sizeTableCode, 0)

    symFriction = SymbolFloat(friction)
    symDrag = SymbolFloat(drag)
    symRotFriction = SymbolFloat(rotFriction)
    symRotDrag = SymbolFloat(rotDrag)
    symTable = SymbolTable(symFriction, physicsTableCode)
    symTable:addSymbol(symDrag)
    symTable:addSymbol(symRotFriction)
    symTable:addSymbol(symRotDrag)
    agent:addSymbolTable(symTable)
    agent:setSymbolName("friction", physicsTableCode, 0)
    agent:setSymbolName("drag", physicsTableCode, 1)
    agent:setSymbolName("rot_friction", physicsTableCode, 2)
    agent:setSymbolName("rot_drag", physicsTableCode, 3)

    symInitialEnergy = SymbolFloat(initialEnergy)
    symMetabolism = SymbolFloat(metabolism)
    symTable = SymbolTable(symInitialEnergy, energyTableCode)
    symTable:addSymbol(symMetabolism)
    agent:addSymbolTable(symTable)
    agent:setSymbolName("initial_energy", energyTableCode, 0)
    agent:setSymbolName("metabolism", energyTableCode, 1)

    symLowAgeLimit = SymbolUL(maxAge)
    symHighAgeLimit = SymbolUL(maxAge)
    symTable = SymbolTable(symLowAgeLimit, ageTableCode)
    symTable:addSymbol(symHighAgeLimit)
    agent:addSymbolTable(symTable)
    agent:setSymbolName("low_age_limit", ageTableCode, 0)
    agent:setSymbolName("high_age_limit", ageTableCode, 1)

    agentColor = SymbolRGB(red, green, blue)
    symTable = SymbolTable(agentColor, colorTableCode)
    symTable:setDynamic(true)
    symTable:setName("color")
    agent:addSymbolTable(symTable)
    agent:setSymbolName("color", colorTableCode, 0)

    agentFeedTable = SymbolTable(SymbolBitString(feed), feedTableCode)
    agentFeedTable:addSymbol(SymbolBitString(food))
    agent:addSymbolTable(agentFeedTable)
    agent:setSymbolName("feed", feedTableCode, 0)
    agent:setSymbolName("food", feedTableCode, 1)

    -- Agent Brain

    brain = Gridbrain()

    brain:setMutateAddConnectionProb(addConnectionProb)
    brain:setMutateRemoveConnectionProb(removeConnectionProb)
    brain:setMutateChangeConnectionWeightProb(changeWeightProb)
    brain:setWeightMutationStanDev(weightMutationStanDev)
    brain:setMutateSplitConnectionProb(splitConnectionProb)
    brain:setMutateChangeComponentProb(changeComponentProb)
    brain:setMutateSwapComponentProb(swapComponentProb)

    alphaSet = GridbrainComponentSet()
    for i, comp in pairs(alphaComponents) do
        alphaSet:addComponent(comp)
    end
    alphaSet:addComponent(PER, SimCont2D.PERCEPTION_POSITION)
    alphaSet:addComponent(PER, SimCont2D.PERCEPTION_DISTANCE)
    alphaSet:addComponent(PER, SimCont2D.PERCEPTION_TARGET)
    alphaSet:addComponent(PER, SimCont2D.PERCEPTION_SYMBOL, TAB_TO_SYM, colorTableCode, 0, colorTableCode, 0)

    grid = Grid()
    grid:init(ALPHA, alphaWidth, alphaHeight)
    grid:setComponentSet(alphaSet)

    brain:addGrid(grid, "objects");

    betaSet = GridbrainComponentSet()
    for i, comp in pairs(betaComponents) do
        betaSet:addComponent(comp)
    end
    betaSet:addComponent(ACT, SimCont2D.ACTION_GO)
    betaSet:addComponent(ACT, SimCont2D.ACTION_ROTATE)
    betaSet:addComponent(ACT, SimCont2D.ACTION_EAT)
    
    grid2 = Grid()
    grid2:init(BETA, betaWidth, betaHeight)
    grid2:setComponentSet(betaSet)

    brain:addGrid(grid2, "beta")

    agent:setBrain(brain)

    agentSpeciesIndex = popDyn:addSpecies(agent, pop, bufferSize)

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
    plant:setSymbolName("size", sizeTableCode, 0)

    symPlantInitialEnergy = SymbolFloat(1.0)
    symTable = SymbolTable(symPlantInitialEnergy, energyTableCode)
    plant:addSymbolTable(symTable)
    plant:setSymbolName("initial_energy", energyTableCode, 0)

    symLowAgeLimit = SymbolUL(maxAge)
    symHighAgeLimit = SymbolUL(maxAge)
    symTable = SymbolTable(symLowAgeLimit, ageTableCode)
    symTable:addSymbol(symHighAgeLimit)
    plant:addSymbolTable(symTable)
    plant:setSymbolName("low_age_limit", ageTableCode, 0)
    plant:setSymbolName("high_age_limit", ageTableCode, 1)

    plantColor = SymbolRGB(red, green, blue)
    symTable = SymbolTable(plantColor, colorTableCode)
    plant:addSymbolTable(symTable)
    plant:setSymbolName("color", colorTableCode, 0)

    plantFeedTable = SymbolTable(SymbolBitString(feed), feedTableCode)
    plantFeedTable:addSymbol(SymbolBitString(food))
    plant:addSymbolTable(plantFeedTable)
    plant:setSymbolName("feed", feedTableCode, 0)
    plant:setSymbolName("food", feedTableCode, 1)

    plant:addGraphic(GraphicSquare())

    popDyn:addSpecies(plant, pop, 1)
end

-- Create Species
--------------------------------------------------------------------------------

addAgentSpecies("pred", numberOfPreds, 255, 0, 0, "11", "01", evoPred)
addAgentSpecies("prey", numberOfPreys, 0, 0, 255, "00", "11", evoPrey)
addPlantSpecies(numberOfPlants, 0, 255, 0, "00", "00")

-- Human Agent
--------------------------------------------------------------------------------

if humanAgent then
    human = Agent()
    dummyBrain = DummyBrain(1)
    dummyBrain:setChannelName(0, "objects")
    dummyBrain:addPerception("Position", 0, SimCont2D.PERCEPTION_POSITION)
    dummyBrain:addPerception("Distance", 0, SimCont2D.PERCEPTION_DISTANCE)
    dummyBrain:addPerception("Feed", 0, SimCont2D.PERCEPTION_SYMBOL, feedTableCode, 0, feedTableCode, 1)
    dummyBrain:addPerception("Food", 0, SimCont2D.PERCEPTION_SYMBOL, feedTableCode, 1, feedTableCode, 0)
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

    symHInitialEnergy = SymbolFloat(1.0)
    symHMetabolism = SymbolFloat(metabolism)
    symTable = SymbolTable(symHInitialEnergy, energyTableCode)
    symTable:addSymbol(symHMetabolism)
    human:addSymbolTable(symTable)
    human:setSymbolName("initial_energy", energyTableCode, 0)
    human:setSymbolName("metabolism", energyTableCode, 1)

    symHLowAgeLimit = SymbolUL(maxAge)
    symHHighAgeLimit = SymbolUL(maxAge)
    symTable = SymbolTable(symHLowAgeLimit, ageTableCode)
    symTable:addSymbol(symHHighAgeLimit)
    human:addSymbolTable(symTable)
    human:setSymbolName("low_age_limit", ageTableCode, 0)
    human:setSymbolName("high_age_limit", ageTableCode, 1)

    humanColor = SymbolRGB(82, 228, 241)
    symTable = SymbolTable(humanColor, colorTableCode)
    human:addSymbolTable(symTable)
    human:setSymbolName("color", colorTableCode, 0)

    feed = "11"
    food = "01"
    humanFeedTable = SymbolTable(SymbolBitString(feed), feedTableCode)
    humanFeedTable:addSymbol(SymbolBitString(food))
    human:addSymbolTable(humanFeedTable)
    human:setSymbolName("feed", feedTableCode, 0)
    human:setSymbolName("food", feedTableCode, 1)

    human:addGraphic(GraphicTriangle())
    sim:addObject(human)
    sim:setPos(human, 300, 300)
    sim:setHuman(human)
end

-- Start Simulation
--------------------------------------------------------------------------------

sim:initGraphics(screenWidth, screenHeight, fullScreen, noGraphics)
sim:run()
