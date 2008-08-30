-- PREDATORS AND PREYS
-- Two species with antagonic goals coevolve
--------------------------------------------------------------------------------

-- Experiment Parameters
--------------------------------------------------------------------------------

numberOfPredators = 20
numberOfPreys = 20
numberOfPlants = 20

agentSize = 10.0
plantSize = 10.0

worldWidth = 1000
worldHeight = 1000

alphaComponents = {AND(), NOT(), SUM(), MUL(), INV(), NEG(), MOD(), AMP(), RAND(), EQ(), GTZ(), ZERO(), MAX(), MIN(), AVG(), DMUL(), SEL(), MEM()}
betaComponents = {AND(), NOT(), SUM(), MUL(), INV(), NEG(), MOD(), AMP(), RAND(), EQ(), GTZ(), ZERO(), CLK(), DMUL(), MEM(), TMEM()}

viewRange = 300.0
viewAngle = 170.0

maxAgeLow = 5000
maxAgeHigh = 5000

initialEnergy = 1.0
goCost = 0.001
rotateCost = 0.001
goForceScale = 0.3
rotateForceScale = 0.006

drag = 0.05
rotDrag = 0.05

fireInterval = 1
laserLength = 25
laserSpeed = 100.0
laserRange = 300.0
laserStrengthFactor = 2.0
laserCostFactor = 0.1
laserHitDuration = 5

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

agentBirthRadius = 0--100.0

keepBodyOnExpire = false

preyRand = false;
predRand = false;

humanAgent = false

-- Command line, log file names, etc
--------------------------------------------------------------------------------

dofile("basic_command_line.lua")

groupFactor = getNumberParameter("grp", groupFactor, "grp")
preyRand = getBoolParameter("preyrand", preyRand, "preyrand")
predRand = getBoolParameter("predrand", predRand, "predrand")

logBaseName = "_predprey_"

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

-- Food network
--------------------------------------------------------------------------------

predFood = 0
predFeed = 1
preyFood = 0
preyFeed = 0
plantFood = 1
humanFeed = 0

-- Colors
--------------------------------------------------------------------------------

predColor = SymbolRGB(255, 0, 0)
preyColor = SymbolRGB(255, 255, 255)
plantColor = SymbolRGB(0, 255, 0)
fireColor = SymbolRGB(255, 100, 0)

-- Plants
--------------------------------------------------------------------------------

plant = SimObj2D()
--plant:setBirthRadius(50.0)
plant:setSize(plantSize)
plant:setInitialEnergy(1.0)
plant:setMaxAge(maxAgeHigh)
plant:setShape(SimObj2D.SHAPE_SQUARE)
plant:setLaserHitDuration(laserHitDuration)
plant:setColoringSymbolName("color")

plantFoodSym = SymbolFloat(plantFood)
plantFoodTable = SymbolTable(plantFoodSym, foodTableCode)
foodTableCode = plantFoodTable:getID()
plant:addSymbolTable(plantFoodTable)
plant:setSymbolName("food", foodTableCode, plantFoodSym:getID())

plantSymTable = SymbolTable(plantColor)
plant:addSymbolTable(plantSymTable)
colorTableCode = plantSymTable:getID()
plantSymTable:setName("color")
plant:setSymbolName("color", colorTableCode, plantColor:getID())

-- Preys
--------------------------------------------------------------------------------

prey = SimObj2D()
prey:setBirthRadius(agentBirthRadius)
prey:setSize(agentSize)
prey:setDrag(drag)
prey:setRotDrag(rotDrag)
prey:setInitialEnergy(initialEnergy)
prey:setMaxAge(maxAgeLow, maxAgeHigh)
prey:setViewRange(viewRange)
prey:setViewAngle(viewAngle)
prey:setGoCost(goCost)
prey:setRotateCost(rotateCost)
prey:setGoForceScale(goForceScale)
prey:setRotateForceScale(rotateForceScale)
prey:setShape(SimObj2D.SHAPE_TRIANGLE)
prey:setColoringSymbolName("color")
prey:setSoundRange(500)
prey:setFireInterval(fireInterval)
prey:setLaserLength(laserLength)
prey:setLaserSpeed(laserSpeed)
prey:setLaserRange(laserRange)
prey:setLaserStrengthFactor(laserStrengthFactor)
prey:setLaserCostFactor(laserCostFactor)
prey:setLaserHitDuration(laserHitDuration)
prey:setKeepBodyOnExpirationDeath(keepBodyOnExpire)
prey:setFeedCenter(0.5)

preyFoodSym = SymbolFloat(preyFood)
preyFoodTable = SymbolTable(preyFoodSym, foodTableCode)
prey:addSymbolTable(preyFoodTable)
prey:setSymbolName("food", foodTableCode, preyFoodSym:getID())

preyFeedSym = SymbolFloat(preyFeed)
preyFeedTable = SymbolTable(preyFeedSym)
feedTableCode = preyFeedTable:getID()
prey:addSymbolTable(preyFeedTable)
prey:setSymbolName("feed", feedTableCode, preyFeedSym:getID())

preySymTable = SymbolTable(preyColor, colorTableCode)
preySymTable:addSymbol(fireColor)
prey:addSymbolTable(preySymTable)
preySymTable:setDynamic(true)
preySymTable:setName("color")
prey:setSymbolName("color", colorTableCode, preyColor:getID())

-- Symbols acquisition
prey:addObjectSymbolAcquisition(colorTableCode, colorTableCode)
prey:addMessageSymbolAcquisition(colorTableCode)

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
alphaSet:addComponent(PER(Sim2D.PERCEPTION_TARGET))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_LTARGET))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_LOF))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_ORIENTATION))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_SYMEQ, colorTableCode, preyColor:getID(), colorTableCode, true))
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
soundSet:addComponent(PER(Sim2D.PERCEPTION_SYMEQ, colorTableCode, preyColor:getID(), colorTableCode, true))
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
betaSet:addComponent(ACT(Sim2D.ACTION_EATB))
betaSet:addComponent(ACT(Sim2D.ACTION_FIREB, colorTableCode, fireColor:getID(), colorTableCode, false))
betaSet:addComponent(ACT(Sim2D.ACTION_SPEAK, colorTableCode, preyColor:getID(), colorTableCode, true))
    
grid2 = Grid()
grid2:init(Grid.BETA, 0, 0)
grid2:setComponentSet(betaSet)

brain:addGrid(grid2, "beta")

prey:setBrain(brain)

-- Predators
--------------------------------------------------------------------------------

pred = SimObj2D()
pred:setBirthRadius(agentBirthRadius)
pred:setSize(agentSize)
pred:setDrag(drag)
pred:setRotDrag(rotDrag)
pred:setInitialEnergy(initialEnergy)
pred:setMaxAge(maxAgeLow, maxAgeHigh)
pred:setViewRange(viewRange)
pred:setViewAngle(viewAngle)
pred:setGoCost(goCost)
pred:setRotateCost(rotateCost)
pred:setGoForceScale(goForceScale)
pred:setRotateForceScale(rotateForceScale)
pred:setShape(SimObj2D.SHAPE_TRIANGLE)
pred:setColoringSymbolName("color")
pred:setSoundRange(500)
pred:setFireInterval(fireInterval)
pred:setLaserLength(laserLength)
pred:setLaserSpeed(laserSpeed)
pred:setLaserRange(laserRange)
pred:setLaserStrengthFactor(laserStrengthFactor)
pred:setLaserCostFactor(laserCostFactor)
pred:setLaserHitDuration(laserHitDuration)
pred:setKeepBodyOnExpirationDeath(keepBodyOnExpire)
pred:setFeedCenter(0.5)

predFoodSym = SymbolFloat(predFood)
predFoodTable = SymbolTable(predFoodSym, foodTableCode)
pred:addSymbolTable(predFoodTable)
pred:setSymbolName("food", foodTableCode, predFoodSym:getID())

predFeedSym = SymbolFloat(predFeed)
predFeedTable = SymbolTable(predFeedSym, feedTableCode)
pred:addSymbolTable(predFeedTable)
pred:setSymbolName("feed", feedTableCode, predFeedSym:getID())

predSymTable = SymbolTable(predColor, colorTableCode)
predSymTable:addSymbol(fireColor)
pred:addSymbolTable(predSymTable)
predSymTable:setDynamic(true)
predSymTable:setName("color")
pred:setSymbolName("color", colorTableCode, predColor:getID())

-- Symbols acquisition
pred:addObjectSymbolAcquisition(colorTableCode, colorTableCode)
pred:addMessageSymbolAcquisition(colorTableCode)

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
alphaSet:addComponent(PER(Sim2D.PERCEPTION_TARGET))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_LTARGET))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_LOF))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_ORIENTATION))
alphaSet:addComponent(PER(Sim2D.PERCEPTION_SYMEQ, colorTableCode, predColor:getID(), colorTableCode, true))
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
soundSet:addComponent(PER(Sim2D.PERCEPTION_SYMEQ, colorTableCode, predColor:getID(), colorTableCode, true))
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
betaSet:addComponent(ACT(Sim2D.ACTION_EATB))
betaSet:addComponent(ACT(Sim2D.ACTION_FIREB, colorTableCode, fireColor:getID(), colorTableCode, false))
betaSet:addComponent(ACT(Sim2D.ACTION_SPEAK, colorTableCode, predColor:getID(), colorTableCode, true))
    
grid2 = Grid()
grid2:init(Grid.BETA, 0, 0)
grid2:setComponentSet(betaSet)

brain:addGrid(grid2, "beta")

pred:setBrain(brain)

-- Population Dynamics
--------------------------------------------------------------------------------

popDyn = PopDynSEGA()
sim:setPopulationDynamics(popDyn)

predSpecies = Species(pred, numberOfPredators, bufferSize)
if predRand then
    predSpecies:addGoal(SimObj2D.FITNESS_RANDOM)
else
    predSpecies:addGoal(SimObj2D.FITNESS_ENERGY_GAINED_SUM)
end
predSpecies:setFitnessAging(fitnessAging)
predSpecies:setRecombineProb(recombineProb)
predSpecies:setKinFactor(kinFactor)
predSpecies:setKinMutation(kinMutation)
predSpecies:setGroupFactor(groupFactor)
popDyn:addSpecies(predSpecies)

preySpecies = Species(prey, numberOfPreys, bufferSize)
if preyRand then
    preySpecies:addGoal(SimObj2D.FITNESS_RANDOM)
else
    preySpecies:addGoal(SimObj2D.FITNESS_ENERGY_GAINED_SUM)
end
preySpecies:setFitnessAging(fitnessAging)
preySpecies:setRecombineProb(recombineProb)
preySpecies:setKinFactor(kinFactor)
preySpecies:setKinMutation(kinMutation)
preySpecies:setGroupFactor(groupFactor)
popDyn:addSpecies(preySpecies)

plantSpecies = Species(plant, numberOfPlants, 1)
plantSpecies:addGoal(SimObj2D.FITNESS_RANDOM)
popDyn:addSpecies(plantSpecies)

popDyn:setEvolutionStopTime(evolutionStopTime)

-- Human Agent
--------------------------------------------------------------------------------

if humanAgent then
    human = SimObj2D()
    human:setSize(agentSize)
    human:setDrag(drag)
    human:setRotDrag(rotDrag)
    human:setInitialEnergy(1.0)
    human:setMaxAge(maxAgeHigh)
    human:setViewRange(viewRange)
    human:setViewAngle(viewAngle)
    human:setGoCost(goCost)
    human:setRotateCost(rotateCost)
    human:setGoForceScale(goForceScale)
    human:setRotateForceScale(rotateForceScale)
    human:setFeedCenter(0.5)
    human:setShape(SimObj2D.SHAPE_TRIANGLE)
    human:setSoundRange(500)
    human:setFireInterval(fireInterval)
    human:setLaserLength(laserLength)
    human:setLaserSpeed(laserSpeed)
    human:setLaserRange(laserRange)
    human:setLaserStrengthFactor(laserStrengthFactor)
    human:setLaserCostFactor(laserCostFactor)
    human:setLaserHitDuration(laserHitDuration)
    human:setColoringSymbolName("color")

    humanColor = SymbolRGB(82, 228, 241)
    symTable = SymbolTable(humanColor, colorTableCode)
    human:addSymbolTable(symTable)
    human:setSymbolName("color", colorTableCode, humanColor:getID())

    humanFeed = SymbolFloat(humanFeed)
    humanFeedTable = SymbolTable(humanFeed, feedTableCode)
    human:addSymbolTable(humanFeedTable)
    human:setSymbolName("feed", feedTableCode, humanFeed:getID())

    -- Human Brain
    brain = Gridbrain()

    alphaSet = ComponentSet()
    for i, comp in pairs(alphaComponents) do
        alphaSet:addComponent(comp)
    end
    grid = Grid()
    grid:init(Grid.ALPHA, 0, 0)
    grid:setComponentSet(alphaSet)
    brain:addGrid(grid, "objects");
    betaSet = ComponentSet()
    for i, comp in pairs(betaComponents) do
        betaSet:addComponent(comp)
    end
    grid2 = Grid()
    grid2:init(Grid.BETA, 0, 0)
    grid2:setComponentSet(betaSet)
    brain:addGrid(grid2, "beta")
    human:setBrain(brain)

    sim:addObject(human)
    human:setPos(300, 300)
    sim:setHuman(human)
end

-- Logs and Statistics
--------------------------------------------------------------------------------

stats = StatCommon()
stats:setFile("log_pred" .. logSuffix .. ".csv")
stats:addField("energy_gained_sum")
stats:addField("gb_connections")
stats:addField("gb_active_connections")
stats:addField("gb_active_components")
stats:addField("gb_grid_width_objects")
stats:addField("gb_grid_height_objects")
stats:addField("gb_grid_width_beta")
stats:addField("gb_grid_height_beta")
stats:addField("symtable_used_color")
predSpecies:addDeathLog(stats)

stats = StatCommon()
stats:setFile("log_prey" .. logSuffix .. ".csv")
stats:addField("energy_gained_sum")
stats:addField("gb_connections")
stats:addField("gb_active_connections")
stats:addField("gb_active_components")
stats:addField("gb_grid_width_objects")
stats:addField("gb_grid_height_objects")
stats:addField("gb_grid_width_beta")
stats:addField("gb_grid_height_beta")
stats:addField("symtable_used_color")
preySpecies:addDeathLog(stats)

if logBrains then
    logBrain = LogBestBrain()
    logBrain:setFileNameSuffix(".svg")
    if logOnlyLastBrain then
        logBrain:setLogOnlyLast(true)
        logBrain:setFileNamePrefix("brain_pred" .. logSuffix)
    else
        logBrain:setFileNamePrefix("brain_pred" .. logSuffix .. "t")
    end
    predSpecies:addDeathLog(logBrain)

    logBrain = LogBestBrain()
    logBrain:setFileNameSuffix(".svg")
    if logOnlyLastBrain then
        logBrain:setLogOnlyLast(true)
        logBrain:setFileNamePrefix("brain_prey" .. logSuffix)
    else
        logBrain:setFileNamePrefix("brain_prey" .. logSuffix .. "t")
    end
    preySpecies:addDeathLog(logBrain)
end

predSpecies:setBufferDump("bdump_pred" .. logSuffix)

preySpecies:setBufferDump("bdump_prey" .. logSuffix)

popDyn:setLogTimeInterval(logTimeInterval)

-- Start Simulation
--------------------------------------------------------------------------------

sim:initGraphics(screenWidth, screenHeight, fullScreen, noGraphics)
sim:run()
