-- TARGETS
-- Agents evolve to shoot at targets
--------------------------------------------------------------------------------

dofile("basic_defines.lua")

-- Experiment Parameters
--------------------------------------------------------------------------------

numberOfTargets = 5
numberOfAgents = 10

agentSize = 10.0

targetMinEnergy = 3.0
targetMaxEnergy = 3.0
targetSizeFactor = 2.0
targetSpeed = 0.05

worldWidth = 500
worldHeight = 500

viewRange = 300.0
viewAngle = 350.0

maxAgeLow = 4500
maxAgeHigh = 5500

initialEnergy = 1.0
goCost = 0.001
rotateCost = 0.001
goForceScale = 0.3
rotateForceScale = 0.05

drag = 0.05
rotDrag = 0.05

fireInterval = 1000
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
fitnessAging = 0.1

addConnectionProb = 0.01
removeConnectionProb = 0.01
changeParamProb = 0.01
paramMutationStanDev = 2.5
splitConnectionProb = 0.01
joinConnectionsProb = 0.01
changeInComponentProb = 0.2

recombineProb = 0.0

groupFactor = 0.8

timeLimit = 0
logTimeInterval = 100
logBrains = true
logOnlyLastBrain = true

humanAgent = false

evolutionStopTime = 0

self = true
comm = true

agentBirthRadius = 0-100.0

keepBodyOnExpire = false

-- Command line, log file names, etc
--------------------------------------------------------------------------------

dofile("basic_command_line.lua")

logBaseName = "_programed_targets_"

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

brain:setMutateAddConnectionProb(0)
brain:setMutateRemoveConnectionProb(0)
brain:setMutateChangeParamProb(0)
brain:setParamMutationStanDev(0)
brain:setMutateSplitConnectionProb(0)
brain:setMutateJoinConnectionsProb(0)
brain:setMutateChangeInactiveComponentProb(0)

grid1 = Grid()
grid1:init(Grid.ALPHA, 4, 4)
brain:addGrid(grid1, "objects");

grid2 = Grid()
grid2:init(Grid.ALPHA, 1, 1)
brain:addGrid(grid2, "sounds");

grid3 = Grid()
grid3:init(Grid.BETA, 3, 4)
brain:addGrid(grid3, "beta")

brain:init()

brain:setComponent(0, 0, 0, IN, 0, Sim2D.PERCEPTION_LTARGET)
brain:setComponent(0, 1, 0, IN, 0, Sim2D.PERCEPTION_SYMEQ, TAB_TO_SYM, colorTableCode, agentColor:getID(), colorTableCode)
brain:setComponent(0, 3, 0, IN, 0, Sim2D.PERCEPTION_LOF)
brain:setComponent(1, 3, 0, NEG)
brain:setComponent(1, 0, 0, IN, 0, Sim2D.PERCEPTION_POSITION)
brain:setComponent(1, 1, 0, NOT)
brain:setComponent(2, 1, 0, SEL, 0, 0, 0, 0, 0, 0)
brain:setComponent(3, 1, 0, MUL)
brain:setComponent(3, 2, 0, AND)

brain:setComponent(0, 0, 1, IN, 0, Sim2D.PERCEPTION_SYMEQ, TAB_TO_SYM, colorTableCode, agentColor:getID(), colorTableCode)

brain:setComponent(0, 0, 2, CLK, 0.2)
brain:setComponent(1, 0, 2, DAND)
brain:setComponent(2, 0, 2, OUT, 0, Sim2D.ACTION_FIREB)
brain:setComponent(2, 1, 2, OUT, 0, Sim2D.ACTION_ROTATE)
brain:setComponent(2, 2, 2, OUT, 0, Sim2D.ACTION_GO)
brain:setComponent(2, 3, 2, OUT, 0, Sim2D.ACTION_SPEAK, SYM_TO_SYM, colorTableCode, agentColor:getID())

brain:addConnection(0, 0, 0, 3, 2, 0)
brain:addConnection(0, 1, 0, 1, 1, 0)
brain:addConnection(1, 0, 0, 3, 1, 0)
brain:addConnection(1, 1, 0, 2, 1, 0)
brain:addConnection(2, 1, 0, 3, 1, 0)
brain:addConnection(2, 1, 0, 3, 2, 0)
brain:addConnection(3, 1, 0, 2, 1, 2)
brain:addConnection(3, 2, 0, 1, 0, 2)
brain:addConnection(0, 3, 0, 1, 3, 0)
brain:addConnection(1, 3, 0, 2, 2, 2)

brain:addConnection(0, 0, 1, 0, 0, 2)

brain:addConnection(1, 0, 2, 2, 0, 2)
brain:addConnection(0, 0, 2, 1, 0, 2)
brain:addConnection(0, 0, 2, 2, 3, 2)

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

targetColor = SymbolRGB(255, 255, 255)
symTable = SymbolTable(targetColor, colorTableCode)
target:addSymbolTable(symTable)
target:setSymbolName("color", colorTableCode, targetColor:getID())


-- Population Dynamics
--------------------------------------------------------------------------------

popDyn = PopDynSEGA()
sim:setPopulationDynamics(popDyn)

agentSpecies = Species(agent, numberOfAgents)
agentSpecies:addGoal(SimObj2D.FITNESS_LASER_AGE_SCORE, bufferSize)
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
stats:addField("laser_age_score")
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

popDyn:setLogTimeInterval(logTimeInterval)

-- Start Simulation
--------------------------------------------------------------------------------

sim:initGraphics(screenWidth, screenHeight, fullScreen, noGraphics)
sim:run()
