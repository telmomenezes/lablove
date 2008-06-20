-- SYNCH
-- Agents evolve to synchronize communication
--------------------------------------------------------------------------------

dofile("basic_defines.lua")

-- Experiment Parameters
--------------------------------------------------------------------------------

numberOfAgents = 10

agentSize = 10.0

worldWidth = 500
worldHeight = 500

alphaComponents = {AND, NOT, SUM, MUL, INV, NEG, MOD, AMP, RAND, EQ, GTZ, ZERO, MAX, MIN, AVG, MEMW, MEMC}
betaComponents = {AND, NOT, SUM, MUL, INV, NEG, MOD, AMP, RAND, EQ, GTZ, ZERO, CLK, MEMW, MEMC}

viewRange = 300.0
viewAngle = 350.0

maxAgeLow = 4500
maxAgeHigh = 5500

initialEnergy = 1.0
goCost = 0.001
rotateCost = 0.001
goForceScale = 0.3
rotateForceScale = 0.006

drag = 0.05
rotDrag = 0.05

soundRange = 500
speakInterval = 250

compCount = 1
bufferSize = 100
fitnessAging = 0.1

addConnectionProb = 0.01
removeConnectionProb = 0.01
changeParamProb = 0.01
paramMutationStanDev = 2.5
splitConnectionProb = 0.01
joinConnectionsProb = 0.01
changeInComponentProb = 0.2

recombineProb = 0.25

timeLimit = 0
logTimeInterval = 100
logBrains = true
logOnlyLastBrain = true

humanAgent = false

evolutionStopTime = 0

agentBirthRadius = 100.0

keepBodyOnExpire = true

kinFactor = 1.0

-- Command line, log file names, etc
--------------------------------------------------------------------------------

dofile("basic_command_line.lua")

kinFactor = getNumberParameter("kin", kinFactor, "kin")

logBaseName = "_synch_"

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
agent:setFitnessMeasure(Sim2D.FITNESS_SCORE)
agent:setBirthRadius(agentBirthRadius)
agent:setSize(agentSize)
agent:setDrag(drag)
agent:setRotDrag(rotDrag)
agent:setInitialEnergy(initialEnergy)
agent:setMaxAge(maxAgeLow, maxAgeHigh)
agent:setViewRange(viewRange)
agent:setViewAngle(viewAngle)
agent:setGoCost(goCost)
agent:setRotateCost(rotateCost)
agent:setGoForceScale(goForceScale)
agent:setRotateForceScale(rotateForceScale)
agent:setShape(SimObj2D.SHAPE_TRIANGLE)
agent:setColoringSymbolName("color")
agent:setSoundRange(soundRange)
agent:setSpeakInterval(speakInterval)
agent:setKeepBodyOnExpirationDeath(keepBodyOnExpire)
--agent:setKeepBodyOnHardDeath(true)

agentColor = SymbolRGB(0, 0, 255)
symTable = SymbolTable(agentColor)
agent:addSymbolTable(symTable)
symTable:setDynamic(true)
symTable:setName("color")
colorTableCode = symTable:getID()
agent:setSymbolName("color", colorTableCode, agentColor:getID())

-- Symbols acquisition
agent:addObjectSymbolAcquisition(colorTableCode, colorTableCode)
agent:addMessageSymbolAcquisition(colorTableCode)

-- Agent Brain

brain = Gridbrain()

brain:setMutateAddConnectionProb(addConnectionProb)
brain:setMutateRemoveConnectionProb(removeConnectionProb)
brain:setMutateChangeParamProb(changeParamProb)
brain:setParamMutationStanDev(paramMutationStanDev)
brain:setMutateSplitConnectionProb(splitConnectionProb)
brain:setMutateJoinConnectionsProb(joinConnectionsProb)
brain:setMutateChangeInactiveComponentProb(changeInComponentProb)

-- Objects Grid
alphaSet = GridbrainComponentSet()
for i, comp in pairs(alphaComponents) do
    alphaSet:addComponent(comp)
end
alphaSet:addComponent(IN, Sim2D.PERCEPTION_POSITION)
alphaSet:addComponent(IN, Sim2D.PERCEPTION_DISTANCE)
alphaSet:addComponent(IN, Sim2D.PERCEPTION_ID)
grid = Grid()
grid:init(ALPHA, 0, 0)
grid:setComponentSet(alphaSet)
brain:addGrid(grid, "objects");

-- Sounds grid
soundSet = GridbrainComponentSet()
for i, comp in pairs(alphaComponents) do
    soundSet:addComponent(comp)
end
soundSet:addComponent(IN, Sim2D.PERCEPTION_POSITION)
soundSet:addComponent(IN, Sim2D.PERCEPTION_DISTANCE)
soundSet:addComponent(IN, Sim2D.PERCEPTION_VALUE)
soundSet:addComponent(IN, Sim2D.PERCEPTION_SYMEQ, TAB_TO_SYM, colorTableCode, agentColor:getID(), colorTableCode)
soundGrid = Grid()
soundGrid:init(ALPHA, 0, 0)
soundGrid:setComponentSet(soundSet)
brain:addGrid(soundGrid, "sounds");

-- Beta Grid
betaSet = GridbrainComponentSet()
for i, comp in pairs(betaComponents) do
    betaSet:addComponent(comp)
end
betaSet:addComponent(OUT, Sim2D.ACTION_GO)
betaSet:addComponent(OUT, Sim2D.ACTION_ROTATE)
betaSet:addComponent(OUT, Sim2D.ACTION_SPEAK, TAB_TO_SYM, colorTableCode, agentColor:getID())
    
grid2 = Grid()
grid2:init(BETA, 0, 0)
grid2:setComponentSet(betaSet)

brain:addGrid(grid2, "beta")

agent:setBrain(brain)

-- Population Dynamics
--------------------------------------------------------------------------------

popDyn = PopDynSEGA()
sim:setPopulationDynamics(popDyn)

agentSpecies = Species(agent, numberOfAgents, bufferSize)

agentSpecies:setFitnessAging(fitnessAging)
agentSpecies:setRecombineProb(recombineProb)
agentSpecies:setKinFactor(kinFactor)

agentSpeciesIndex = popDyn:addSpecies(agentSpecies)
popDyn:setEvolutionStopTime(evolutionStopTime)


-- Human Agent
--------------------------------------------------------------------------------

if humanAgent then
    human = SimObj2D()
    human:setSize(agentSize)
    human:setDrag(drag)
    human:setRotDrag(rotDrag)
    human:setInitialEnergy(1.0)
    human:setMaxAge(maxAgeLow)
    human:setViewRange(viewRange)
    human:setViewAngle(viewAngle)
    human:setGoCost(goCost)
    human:setRotateCost(rotateCost)
    human:setGoForceScale(goForceScale)
    human:setRotateForceScale(rotateForceScale)
    human:setShape(SimObj2D.SHAPE_TRIANGLE)
    human:setColoringSymbolName("color")
    human:setFireInterval(fireInterval)
    human:setLaserLength(laserLength)
    human:setLaserRange(laserRange)
    human:setLaserStrengthFactor(laserStrengthFactor)
    human:setLaserCostFactor(laserCostFactor)
    human:setLaserHitDuration(laserHitDuration)

    humanColor = SymbolRGB(82, 228, 241)
    symTable = SymbolTable(humanColor, colorTableCode)
    human:addSymbolTable(symTable)
    human:setSymbolName("color", colorTableCode, humanColor:getID())

    dummyBrain = DummyBrain(3)
    dummyBrain:setChannelName(0, "objects")
    dummyBrain:setChannelName(1, "sounds")
    dummyBrain:setChannelName(2, "self")
    dummyBrain:addPerception("Position", 0, Sim2D.PERCEPTION_POSITION)
    dummyBrain:addPerception("Distance", 0, Sim2D.PERCEPTION_DISTANCE)
    dummyBrain:addPerception("Color", 0, Sim2D.PERCEPTION_SYMEQ, colorTableCode, humanColor:getID(), colorTableCode)
    dummyBrain:addPerception("ID", 0, Sim2D.PERCEPTION_ID)

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
stats:addField("gb_connections")
stats:addField("gb_active_connections")
stats:addField("gb_active_components")
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

sim:initGraphics(screenWidth, screenHeight, fullScreen, noGraphics)
sim:run()
