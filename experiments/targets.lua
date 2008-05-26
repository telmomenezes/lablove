-- TARGETS
-- Agents evolve to shoot at targets
--------------------------------------------------------------------------------

dofile("basic_defines.lua")

-- Experiment Parameters
--------------------------------------------------------------------------------

numberOfTargets = 50
numberOfAgents = 5

agentSize = 10.0

targetMinEnergy = 0.9
targetMaxEnergy = 0.9
targetSizeFactor = 10.0

worldWidth = 1000
worldHeight = 1000

alphaComponents = {AND, NOT, SUM, MUL, INV, NEG, MOD, AMP, RAND, GTZ, ZERO, MAX, AVG}
betaComponents = {AND, NOT, SUM, MUL, INV, NEG, MOD, AMP, RAND, GTZ, ZERO, CLK, MEMW, MEMC, MEMD}

viewRange = 300.0
viewAngle = 170.0

maxAge = 5000

initialEnergy = 1.0
goCost = 0.005
rotateCost = 0.005
goForceScale = 0.3
rotateForceScale = 0.006

drag = 0.05
rotDrag = 0.05

fireInterval = 500
laserLength = 25
laserRange = 300.0
laserStrengthFactor = 1.0
laserCostFactor = 0.1
laserHitDuration = 500

soundRange = 500
speakInterval = 250

compCount = 1
bufferSize = 100
fitnessAging = 0.1

addConnectionProb = 0.01
removeConnectionProb = 0.01
changeParamProb = 0.01
paramMutationStanDev = 1.0
splitConnectionProb = 0.01
joinConnectionsProb = 0.01
changeInComponentProb = 0.2

recombineProb = 1.0

timeLimit = 0
logTimeInterval = 100
logBrains = true
logOnlyLastBrain = true

humanAgent = false

evolutionStopTime = 0

comm = false

queen = true

targetBirthRadius = 500.0
agentBirthRadius = 100.0

-- Command line, log file names, etc
--------------------------------------------------------------------------------

dofile("basic_command_line.lua")

evolutionStopTime = getNumberParameter("evostop", evolutionStopTime, "est")

logBaseName = "_targets_"

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
agent:setFitnessMeasure(Sim2D.FITNESS_SCORE_SUM)
agent:setBirthRadius(agentBirthRadius)
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
agent:setShape(SimObj2D.SHAPE_TRIANGLE)
agent:setColoringSymbolName("color")
agent:setSoundRange(soundRange)
agent:setSpeakInterval(speakInterval)
agent:setFireInterval(fireInterval)
agent:setLaserLength(laserLength)
agent:setLaserRange(laserRange)
agent:setLaserStrengthFactor(laserStrengthFactor)
agent:setLaserCostFactor(laserCostFactor)
agent:setLaserHitDuration(laserHitDuration)

agentColor = SymbolRGB(0, 0, 255)
symTable = SymbolTable(agentColor)
agent:addSymbolTable(symTable)
colorTableCode = symTable:getID()
agent:setSymbolName("color", colorTableCode, agentColor:getID())

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
alphaSet:addComponent(IN, Sim2D.PERCEPTION_SIZE)
alphaSet:addComponent(IN, Sim2D.PERCEPTION_SYMBOL, TAB_TO_SYM, colorTableCode, agentColor:getID(), colorTableCode)
grid = Grid()
grid:init(ALPHA, 0, 0)
grid:setComponentSet(alphaSet)
brain:addGrid(grid, "objects");

-- Self Grid
selfSet = GridbrainComponentSet()
for i, comp in pairs(alphaComponents) do
    selfSet:addComponent(comp)
end
selfSet:addComponent(IN, Sim2D.PERCEPTION_ENERGY)
--selfSet:addComponent(IN, Sim2D.PERCEPTION_CAN_FIRE)
if comm then
    selfSet:addComponent(IN, Sim2D.PERCEPTION_CAN_SPEAK)
end
selfGrid = Grid()
selfGrid:init(ALPHA, 0, 0)
selfGrid:setComponentSet(selfSet)
brain:addGrid(selfGrid, "self");

-- Sounds grid
if comm then
    soundSet = GridbrainComponentSet()
    for i, comp in pairs(alphaComponents) do
        soundSet:addComponent(comp)
    end
    soundSet:addComponent(IN, Sim2D.PERCEPTION_POSITION)
    soundSet:addComponent(IN, Sim2D.PERCEPTION_DISTANCE)
    soundSet:addComponent(IN, Sim2D.PERCEPTION_SYMBOL, TAB_TO_SYM, colorTableCode, agentColor:getID(), colorTableCode)
    soundGrid = Grid()
    soundGrid:init(ALPHA, 0, 0)
    soundGrid:setComponentSet(soundSet)
    brain:addGrid(soundGrid, "sounds");
end

-- Beta Grid
betaSet = GridbrainComponentSet()
for i, comp in pairs(betaComponents) do
    betaSet:addComponent(comp)
end
betaSet:addComponent(OUT, Sim2D.ACTION_GO)
betaSet:addComponent(OUT, Sim2D.ACTION_ROTATE)
betaSet:addComponent(OUT, Sim2D.ACTION_FIREB)
if comm then
    betaSet:addComponent(OUT, Sim2D.ACTION_SPEAK, TAB_TO_SYM, colorTableCode, agentColor:getID())
end
    
grid2 = Grid()
grid2:init(BETA, 0, 0)
grid2:setComponentSet(betaSet)

brain:addGrid(grid2, "beta")

agent:setBrain(brain)

-- Targets
--------------------------------------------------------------------------------

target = Target2D()
target:setBirthRadius(targetBirthRadius)
target:setMaxAge(maxAge)
target:setEnergyLimits(targetMinEnergy, targetMaxEnergy)
target:setEnergySizeFactor(targetSizeFactor)
target:setShape(SimObj2D.SHAPE_CIRCLE)
target:setColoringSymbolName("color")
target:setLaserHitDuration(laserHitDuration)

targetColor = SymbolRGB(255, 255, 255)
symTable = SymbolTable(targetColor, colorTableCode)
target:addSymbolTable(symTable)
target:setSymbolName("color", colorTableCode, targetColor:getID())

-- Population Dynamics
--------------------------------------------------------------------------------

popDyn = PopDynSpeciesBuffers()
sim:setPopulationDynamics(popDyn)

popDyn:setCompCount(compCount)
popDyn:setFitnessAging(fitnessAging)
popDyn:setRecombineProb(recombineProb)
agentSpeciesIndex = popDyn:addSpecies(agent, numberOfAgents, bufferSize, queen)
popDyn:addSpecies(target, numberOfTargets, 1)
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

    dummyBrain = DummyBrain(1)
    dummyBrain:setChannelName(0, "objects")
    dummyBrain:addPerception("Position", 0, Sim2D.PERCEPTION_POSITION)
    dummyBrain:addPerception("Orientation", 0, Sim2D.PERCEPTION_ORIENTATION)
    dummyBrain:addPerception("Distance", 0, Sim2D.PERCEPTION_DISTANCE)
    dummyBrain:addPerception("Color", 0, Sim2D.PERCEPTION_SYMBOL, colorTableCode, humanColor:getID(), colorTableCode)
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
