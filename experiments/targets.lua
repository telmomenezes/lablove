-- TARGETS
-- Agents evolve to shoot at targets
--------------------------------------------------------------------------------

dofile("basic_defines.lua")

-- Experiment Parameters
--------------------------------------------------------------------------------

numberOfTargets = 5
numberOfAgents = 10

agentSize = 10.0

targetMinEnergy = 2.0
targetMaxEnergy = 2.0
targetSizeFactor = 2.0
targetSpeed = 0.1

worldWidth = 500
worldHeight = 500

alphaComponents = {AND, NOT, SUM, MUL, INV, NEG, MOD, AMP, RAND, EQ, GTZ, ZERO, MAX, MIN, AVG, DMUL, SEL}
betaComponents = {AND, NOT, SUM, MUL, INV, NEG, MOD, AMP, RAND, EQ, GTZ, ZERO, CLK, DMUL}

viewRange = 300.0
viewAngle = 350.0

maxAgeLow = 4500
maxAgeHigh = 5500

initialEnergy = 1.0
goCost = 0.001
rotateCost = 0.001
goForceScale = 0.3
rotateForceScale = 0.1

drag = 0.05
rotDrag = 0.05

fireInterval = 2000
laserLength = 25
laserSpeed = 100.0
laserRange = 300.0
laserStrengthFactor = 1.0
laserCostFactor = 0.1
laserHitDuration = 5

soundRange = 500
speakInterval = 250

compCount = 1
bufferSize = 100
fitnessAging = 0.8

addConnectionProb = 0.01
removeConnectionProb = 0.01
changeParamProb = 0.01
paramMutationStanDev = 2.5
splitConnectionProb = 0.01
joinConnectionsProb = 0.01
changeInComponentProb = 0.2

recombineProb = 0.25

groupFactor = 0.5

timeLimit = 0
logTimeInterval = 100
logBrains = true
logOnlyLastBrain = true

humanAgent = false

evolutionStopTime = 0

self = false
comm = true

agentBirthRadius = 100.0

keepBodyOnExpire = false

-- Command line, log file names, etc
--------------------------------------------------------------------------------

dofile("basic_command_line.lua")

groupFactor = getNumberParameter("grp", groupFactor, "grp")

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
--agent:setBirthRadius(agentBirthRadius)
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
agent:setFireInterval(fireInterval)
agent:setLaserLength(laserLength)
agent:setLaserSpeed(laserSpeed)
agent:setLaserRange(laserRange)
agent:setLaserStrengthFactor(laserStrengthFactor)
agent:setLaserCostFactor(laserCostFactor)
agent:setLaserHitDuration(laserHitDuration)
agent:setKeepBodyOnExpirationDeath(keepBodyOnExpire)
--agent:setKeepBodyOnHardDeath(true)

agentColor = SymbolRGB(0, 0, 255)
targetColor = SymbolRGB(255, 255, 255)

symTable = SymbolTable(agentColor)
symTable:addSymbol(targetColor)
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
alphaSet = ComponentSet()
for i, comp in pairs(alphaComponents) do
    alphaSet:addComponent(comp)
end
alphaSet:addComponent(IN, Sim2D.PERCEPTION_POSITION)
alphaSet:addComponent(IN, Sim2D.PERCEPTION_DISTANCE)
--alphaSet:addComponent(IN, Sim2D.PERCEPTION_SIZE)
alphaSet:addComponent(IN, Sim2D.PERCEPTION_LTARGET)
alphaSet:addComponent(IN, Sim2D.PERCEPTION_LOF)
--alphaSet:addComponent(IN, Sim2D.PERCEPTION_CONV)
--alphaSet:addComponent(IN, Sim2D.PERCEPTION_CONVDIR)
--alphaSet:addComponent(IN, Sim2D.PERCEPTION_SYMEQ, TAB_TO_SYM, colorTableCode, agentColor:getID(), colorTableCode)
alphaSet:addComponent(IN, Sim2D.PERCEPTION_SYMEQ, SYM_TO_SYM, colorTableCode, targetColor:getID(), colorTableCode)
alphaSet:addComponent(IN, Sim2D.PERCEPTION_ID)
grid = Grid()
grid:init(ALPHA, 0, 0)
grid:setComponentSet(alphaSet)
brain:addGrid(grid, "objects");

-- Self Grid
if self then
    selfSet = ComponentSet()
    for i, comp in pairs(alphaComponents) do
        selfSet:addComponent(comp)
    end
    selfSet:addComponent(IN, Sim2D.PERCEPTION_ID)
    selfSet:addComponent(IN, Sim2D.PERCEPTION_BLOCKED)
    selfSet:addComponent(IN, Sim2D.PERCEPTION_COMPASS)
    selfGrid = Grid()
    selfGrid:init(ALPHA, 0, 0)
    selfGrid:setComponentSet(selfSet)
    brain:addGrid(selfGrid, "self")
end

-- Sounds grid
if comm then
    soundSet = ComponentSet()
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
end

-- Beta Grid
betaSet = ComponentSet()
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
target:setMaxAge(0)
target:setEnergyLimits(targetMinEnergy, targetMaxEnergy)
target:setEnergySizeFactor(targetSizeFactor)
target:setShape(SimObj2D.SHAPE_CIRCLE)
target:setColoringSymbolName("color")
target:setLaserHitDuration(laserHitDuration)
target:setSpeed(targetSpeed)

symTable = SymbolTable(targetColor, colorTableCode)
target:addSymbolTable(symTable)
target:setSymbolName("color", colorTableCode, targetColor:getID())


-- Population Dynamics
--------------------------------------------------------------------------------

popDyn = PopDynSEGA()
sim:setPopulationDynamics(popDyn)

agentSpecies = Species(agent, numberOfAgents)
agentSpecies:addGoal(SimObj2D.FITNESS_LASER_HIT_SCORE, bufferSize)
agentSpecies:addGoal(SimObj2D.FITNESS_LASER_EFF_SCORE, bufferSize)
agentSpecies:addGoal(SimObj2D.FITNESS_SYNCH_SCORE, bufferSize)
agentSpecies:setFitnessAging(fitnessAging)
agentSpecies:setRecombineProb(recombineProb)
agentSpecies:setGroupFactor(groupFactor)
agentSpeciesIndex = popDyn:addSpecies(agentSpecies)

targetSpecies = Species(target, numberOfTargets)
targetSpecies:addGoal(SimObj2D.FITNESS_RANDOM, 1)
popDyn:addSpecies(targetSpecies)

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
    human:setLaserSpeed(laserSpeed)
    human:setLaserRange(laserRange)
    human:setLaserStrengthFactor(laserStrengthFactor)
    human:setLaserCostFactor(laserCostFactor)
    human:setLaserHitDuration(laserHitDuration)

    humanColor = SymbolRGB(255, 255, 255)
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
    dummyBrain:addPerception("LTarget", 0, Sim2D.PERCEPTION_LTARGET)
    dummyBrain:addPerception("LOF", 0, Sim2D.PERCEPTION_LOF)
    dummyBrain:addPerception("Conv", 0, Sim2D.PERCEPTION_CONV)
    dummyBrain:addPerception("ConvDir", 0, Sim2D.PERCEPTION_CONVDIR)
    dummyBrain:addPerception("ID", 0, Sim2D.PERCEPTION_ID)
    dummyBrain:addPerception("Blocked", 2, Sim2D.PERCEPTION_BLOCKED)
    dummyBrain:addPerception("Compass", 2, Sim2D.PERCEPTION_COMPASS)

    human:setBrain(dummyBrain)

    sim:addObject(human)
    human:setPos(300, 300)
    sim:setHuman(human)
end

-- Logs and Statistics
--------------------------------------------------------------------------------

stats = StatCommon()
stats:setFile("log" .. logSuffix .. ".csv")
stats:addField("laser_hit_score")
stats:addField("laser_eff_score")
stats:addField("synch_score")
stats:addField("gb_connections")
stats:addField("gb_active_connections")
stats:addField("gb_active_components")
stats:addField("symtable_used_color")
stats:addField("friendly_fire")
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

agentSpecies:setBufferDump("bdump" .. logSuffix)

popDyn:setLogTimeInterval(logTimeInterval)

-- Start Simulation
--------------------------------------------------------------------------------

sim:initGraphics(screenWidth, screenHeight, fullScreen, noGraphics)
sim:run()
