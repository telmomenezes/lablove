-- CATS & MICE
-- Two species with antagonic goals coevolve
-- Cats want to be as close to mice as possible
-- Mice want to be as far from cats as possible
--------------------------------------------------------------------------------

-- Experiment Parameters
--------------------------------------------------------------------------------

numberOfAgents = 10

agentSize = 10.0

worldWidth = 1000
worldHeight = 1000

alphaComponents = {AND(), NOT(), SUM(), MUL(), INV(), NEG(), MOD(), AMP(), RAND(), EQ(), GTZ(), ZERO(), MAX(), MIN(), AVG(), DMUL(), SEL(), MEM()}
betaComponents = {AND(), NOT(), SUM(), MUL(), INV(), NEG(), MOD(), AMP(), RAND(), EQ(), GTZ(), ZERO(), CLK(), DMUL(), MEM(), TMEM()}

viewRange = 300.0
viewAngle = 350.0

maxAgeLow = 5000
maxAgeHigh = 5000

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
laserHitDuration = 2

bufferSize = 100
fitnessAging = 0.5

addConnectionProb = 0.01
removeConnectionProb = 0.01
changeParamProb = 0.01
paramMutationStanDev = 1.0
splitConnectionProb = 0.01
joinConnectionsProb = 0.01
changeInComponentProb = 0.2

recombineProb = 0.25

kinFactor = 0.0
kinMutation = false
groupFactor = 0.5

timeLimit = 0
logTimeInterval = 100
logBrains = true
logOnlyLastBrain = true

evolutionStopTime = 0

agentBirthRadius = 100.0

keepBodyOnExpire = true

-- Command line, log file names, etc
--------------------------------------------------------------------------------

dofile("basic_command_line.lua")

logBaseName = "_catmice_"

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

-- Colors
--------------------------------------------------------------------------------

catColor = SymbolRGB(0, 0, 0)
mouseColor = SymbolRGB(255, 255, 255)
fireColor = SymbolRGB(255, 0, 0)

-- Cats
--------------------------------------------------------------------------------

cat = SimObj2D()
--agent:setBirthRadius(agentBirthRadius)
cat:setSize(agentSize)
cat:setDrag(drag)
cat:setRotDrag(rotDrag)
cat:setInitialEnergy(initialEnergy)
cat:setMaxAge(maxAgeLow, maxAgeHigh)
cat:setViewRange(viewRange)
cat:setViewAngle(viewAngle)
cat:setGoCost(goCost)
cat:setRotateCost(rotateCost)
cat:setGoForceScale(goForceScale)
cat:setRotateForceScale(rotateForceScale)
cat:setShape(SimObj2D.SHAPE_TRIANGLE)
cat:setColoringSymbolName("color")
cat:setSoundRange(500)
cat:setFireInterval(fireInterval)
cat:setLaserLength(laserLength)
cat:setLaserSpeed(laserSpeed)
cat:setLaserRange(laserRange)
cat:setLaserStrengthFactor(laserStrengthFactor)
cat:setLaserCostFactor(laserCostFactor)
cat:setLaserHitDuration(laserHitDuration)
cat:setKeepBodyOnExpirationDeath(keepBodyOnExpire)

catSymTable = SymbolTable(catColor)
catSymTable:addSymbol(fireColor)
cat:addSymbolTable(catSymTable)
catSymTable:setDynamic(true)
catSymTable:setName("color")
colorTableCode = catSymTable:getID()
cat:setSymbolName("color", colorTableCode, catColor:getID())

-- Symbols acquisition
cat:addObjectSymbolAcquisition(colorTableCode, colorTableCode)
cat:addMessageSymbolAcquisition(colorTableCode)

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
alphaSet:addComponent(PER(Sim2D.PERCEPTION_POSITION))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_DISTANCE))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_LTARGET))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_LOF))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_ORIENTATION))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_SYMEQ, colorTableCode, catColor:getID(), colorTableCode, true))
grid = Grid()
grid:init(Grid.ALPHA, 0, 0)
grid:setComponentSet(alphaSet)
brain:addGrid(grid, "objects");

soundSet = ComponentSet()
for i, comp in pairs(alphaComponents) do
    soundSet:addComponent(comp)
end
soundSet:addComponent(PER(Sim2D.PERCEPTION_POSITION))
soundSet:addComponent(PER(Sim2D.PERCEPTION_DISTANCE))
soundSet:addComponent(PER(Sim2D.PERCEPTION_VALUE))
soundSet:addComponent(PER(Sim2D.PERCEPTION_SYMEQ, colorTableCode, catColor:getID(), colorTableCode, true))
soundGrid = Grid()
soundGrid:init(Grid.ALPHA, 0, 0)
soundGrid:setComponentSet(soundSet)
brain:addGrid(soundGrid, "sounds");

-- Beta Grid
betaSet = ComponentSet()
for i, comp in pairs(betaComponents) do
    betaSet:addComponent(comp)
end
betaSet:addComponent(ACT(Sim2D.ACTION_GO))
betaSet:addComponent(ACT(Sim2D.ACTION_ROTATE))
betaSet:addComponent(ACT(Sim2D.ACTION_FIREB, colorTableCode, fireColor:getID(), colorTableCode, false))
betaSet:addComponent(ACT(Sim2D.ACTION_SPEAK, colorTableCode, catColor:getID(), colorTableCode, true))
    
grid2 = Grid()
grid2:init(Grid.BETA, 0, 0)
grid2:setComponentSet(betaSet)

brain:addGrid(grid2, "beta")

cat:setBrain(brain)

-- Mice
--------------------------------------------------------------------------------

mouse = SimObj2D()
--agent:setBirthRadius(agentBirthRadius)
mouse:setSize(agentSize)
mouse:setDrag(drag)
mouse:setRotDrag(rotDrag)
mouse:setInitialEnergy(initialEnergy)
mouse:setMaxAge(maxAgeLow, maxAgeHigh)
mouse:setViewRange(viewRange)
mouse:setViewAngle(viewAngle)
mouse:setGoCost(goCost)
mouse:setRotateCost(rotateCost)
mouse:setGoForceScale(goForceScale)
mouse:setRotateForceScale(rotateForceScale)
mouse:setShape(SimObj2D.SHAPE_TRIANGLE)
mouse:setColoringSymbolName("color")
mouse:setSoundRange(500)
mouse:setFireInterval(fireInterval)
mouse:setLaserLength(laserLength)
mouse:setLaserSpeed(laserSpeed)
mouse:setLaserRange(laserRange)
mouse:setLaserStrengthFactor(laserStrengthFactor)
mouse:setLaserCostFactor(laserCostFactor)
mouse:setLaserHitDuration(laserHitDuration)
mouse:setKeepBodyOnExpirationDeath(keepBodyOnExpire)

mouseSymTable = SymbolTable(mouseColor, colorTableCode)
mouseSymTable:addSymbol(fireColor)
mouse:addSymbolTable(mouseSymTable)
mouseSymTable:setDynamic(true)
mouseSymTable:setName("color")
mouse:setSymbolName("color", colorTableCode, mouseColor:getID())

-- Symbols acquisition
mouse:addObjectSymbolAcquisition(colorTableCode, colorTableCode)
mouse:addMessageSymbolAcquisition(colorTableCode)

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
alphaSet:addComponent(PER(Sim2D.PERCEPTION_POSITION))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_DISTANCE))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_LTARGET))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_LOF))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_ORIENTATION))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_SYMEQ, colorTableCode, mouseColor:getID(), colorTableCode, true))
grid = Grid()
grid:init(Grid.ALPHA, 0, 0)
grid:setComponentSet(alphaSet)
brain:addGrid(grid, "objects");

soundSet = ComponentSet()
for i, comp in pairs(alphaComponents) do
    soundSet:addComponent(comp)
end
soundSet:addComponent(PER(Sim2D.PERCEPTION_POSITION))
soundSet:addComponent(PER(Sim2D.PERCEPTION_DISTANCE))
soundSet:addComponent(PER(Sim2D.PERCEPTION_VALUE))
soundSet:addComponent(PER(Sim2D.PERCEPTION_SYMEQ, colorTableCode, mouseColor:getID(), colorTableCode, true))
soundGrid = Grid()
soundGrid:init(Grid.ALPHA, 0, 0)
soundGrid:setComponentSet(soundSet)
brain:addGrid(soundGrid, "sounds");

-- Beta Grid
betaSet = ComponentSet()
for i, comp in pairs(betaComponents) do
    betaSet:addComponent(comp)
end
betaSet:addComponent(ACT(Sim2D.ACTION_GO))
betaSet:addComponent(ACT(Sim2D.ACTION_ROTATE))
betaSet:addComponent(ACT(Sim2D.ACTION_FIREB, colorTableCode, fireColor:getID(), colorTableCode, false))
betaSet:addComponent(ACT(Sim2D.ACTION_SPEAK, colorTableCode, mouseColor:getID(), colorTableCode, true))
    
grid2 = Grid()
grid2:init(Grid.BETA, 0, 0)
grid2:setComponentSet(betaSet)

brain:addGrid(grid2, "beta")

mouse:setBrain(brain)

-- Population Dynamics
--------------------------------------------------------------------------------

popDyn = PopDynSEGA()
sim:setPopulationDynamics(popDyn)

catSpecies = Species(cat, numberOfAgents)
catSpecies:addGoal(bufferSize, SimObj2D.FITNESS_PROXIMITY)
catSpecies:setFitnessAging(fitnessAging)
catSpecies:setRecombineProb(recombineProb)
catSpecies:setKinFactor(kinFactor)
catSpecies:setKinMutation(kinMutation)
catSpecies:setGroupFactor(groupFactor)
popDyn:addSpecies(catSpecies)

mouseSpecies = Species(mouse, numberOfAgents)
mouseSpecies:addGoal(bufferSize, SimObj2D.FITNESS_DISTANCE)
mouseSpecies:setFitnessAging(fitnessAging)
mouseSpecies:setRecombineProb(recombineProb)
mouseSpecies:setKinFactor(kinFactor)
mouseSpecies:setKinMutation(kinMutation)
mouseSpecies:setGroupFactor(groupFactor)
popDyn:addSpecies(mouseSpecies)

popDyn:setEvolutionStopTime(evolutionStopTime)

-- Logs and Statistics
--------------------------------------------------------------------------------

stats = StatCommon()
stats:setFile("log_cat" .. logSuffix .. ".csv")
stats:addField("proximity_fitness")
stats:addField("gb_connections")
stats:addField("gb_active_connections")
stats:addField("gb_active_components")
stats:addField("symtable_used_color")
catSpecies:addDeathLog(stats)

stats = StatCommon()
stats:setFile("log_mouse" .. logSuffix .. ".csv")
stats:addField("distance_fitness")
stats:addField("gb_connections")
stats:addField("gb_active_connections")
stats:addField("gb_active_components")
stats:addField("symtable_used_color")
mouseSpecies:addDeathLog(stats)

if logBrains then
    logBrain = LogBestBrain()
    logBrain:setFileNameSuffix(".svg")
    if logOnlyLastBrain then
        logBrain:setLogOnlyLast(true)
        logBrain:setFileNamePrefix("brain_cat" .. logSuffix)
    else
        logBrain:setFileNamePrefix("brain_cat" .. logSuffix .. "t")
    end
    catSpecies:addDeathLog(logBrain)

    logBrain = LogBestBrain()
    logBrain:setFileNameSuffix(".svg")
    if logOnlyLastBrain then
        logBrain:setLogOnlyLast(true)
        logBrain:setFileNamePrefix("brain_mouse" .. logSuffix)
    else
        logBrain:setFileNamePrefix("brain_mouse" .. logSuffix .. "t")
    end
    mouseSpecies:addDeathLog(logBrain)
end

catSpecies:setBufferDump("bdump_cat" .. logSuffix)

mouseSpecies:setBufferDump("bdump_mouse" .. logSuffix)

popDyn:setLogTimeInterval(logTimeInterval)

-- Start Simulation
--------------------------------------------------------------------------------

sim:initGraphics(screenWidth, screenHeight, fullScreen, noGraphics)
sim:run()
