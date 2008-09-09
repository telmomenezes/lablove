-- Battle
-- Two species with antagonic goals coevolve
--------------------------------------------------------------------------------

-- Experiment Parameters
--------------------------------------------------------------------------------

numberOfReds = 20
numberOfBlues = 20
numberOfPlants = 20

agentSize = 10.0
plantSize = 10.0

worldWidth = 1000
worldHeight = 1000

alphaComponents = {AND(), NOT(), SUM(), MUL(), INV(), NEG(), MOD(), AMP(), RAND(), EQ(), GTZ(), ZERO(), MAX(), MIN(), AVG(), DMUL(), SEL(), MEM()}
betaComponents = {AND(), NOT(), SUM(), MUL(), INV(), NEG(), MOD(), AMP(), RAND(), EQ(), GTZ(), ZERO(), CLK(), DMUL(), MEM(), TMEM()}

viewRange = 300.0
viewAngle = 350.0

maxAgeLow = 9500
maxAgeHigh = 10500

initialEnergy = 1.0
goCost = 0.001
rotateCost = 0.001
goForceScale = 0.3
rotateForceScale = 0.006

drag = 0.05
rotDrag = 0.05

fireInterval = 2000
laserLength = 25
laserSpeed = 100.0
laserRange = 300.0
laserStrengthFactor = 2.0
laserCostFactor = 0.1
laserHitDuration = 2

bufferSize = 100
fitnessAging = 0.5

addConnectionProb = 0.01
removeConnectionProb = 0.01
splitConnectionProb = 0.01
joinConnectionsProb = 0.01
changeInComponentProb = 0.2

recombineProb = 0.25

groupFactor = 0.5

timeLimit = 0
logTimeInterval = 100
logBrains = true
logOnlyLastBrain = true

evolutionStopTime = 0

agentBirthRadius = 0--100.0

keepBodyOnExpire = false

redRand = false
blueRand = false

humanAgent = false

-- Command line, log file names, etc
--------------------------------------------------------------------------------

dofile("basic_command_line.lua")

groupFactor = getNumberParameter("grp", groupFactor, "grp")
redRand = getBoolParameter("redrand", redRand, "redrand")
blueRand = getBoolParameter("bluerand", blueRand, "bluerand")

logBaseName = "_battle_"

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

blueFood = 0
blueFeed = 0
redFood = 1
redFeed = 1
plantFood = 1
humanFeed = 0

-- Colors
--------------------------------------------------------------------------------

blueColor = SymbolRGB(0, 0, 255)
redColor = SymbolRGB(255, 0, 0)
plantColor = SymbolRGB(0, 255, 0)

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
plant:setTarget(false)

plantFoodSym = SymbolFloat(0.0)
plantFoodSym:setAlwaysRandom()
plantFoodTable = SymbolTable(plantFoodSym, foodTableCode)
foodTableCode = plantFoodTable:getID()
plant:addSymbolTable(plantFoodTable)
plant:setSymbolName("food", foodTableCode, plantFoodSym:getID())

plant:setColoringScale("food", plantFoodSym, 0.5, 255, 255, 255, 0, 255, 0)

plantSymTable = SymbolTable(plantColor)
plant:addSymbolTable(plantSymTable)
colorTableCode = plantSymTable:getID()
plantSymTable:setName("color")
plant:setSymbolName("color", colorTableCode, plantColor:getID())

-- Reds
--------------------------------------------------------------------------------

red = SimObj2D()
red:setBirthRadius(agentBirthRadius)
red:setSize(agentSize)
red:setDrag(drag)
red:setRotDrag(rotDrag)
red:setInitialEnergy(initialEnergy)
red:setMaxAge(maxAgeLow, maxAgeHigh)
red:setViewRange(viewRange)
red:setViewAngle(viewAngle)
red:setGoCost(goCost)
red:setRotateCost(rotateCost)
red:setGoForceScale(goForceScale)
red:setRotateForceScale(rotateForceScale)
red:setShape(SimObj2D.SHAPE_TRIANGLE)
red:setColoringSymbolName("color")
red:setSoundRange(500)
red:setFireInterval(fireInterval)
red:setLaserLength(laserLength)
red:setLaserSpeed(laserSpeed)
red:setLaserRange(laserRange)
red:setLaserStrengthFactor(laserStrengthFactor)
red:setLaserCostFactor(laserCostFactor)
red:setLaserHitDuration(laserHitDuration)
red:setKeepBodyOnExpirationDeath(keepBodyOnExpire)
red:setFeedCenter(0.5)

redFoodSym = SymbolFloat(redFood)
redFoodTable = SymbolTable(redFoodSym, foodTableCode)
red:addSymbolTable(redFoodTable)
red:setSymbolName("food", foodTableCode, redFoodSym:getID())

redFeedSym = SymbolFloat(redFeed)
redFeedTable = SymbolTable(redFeedSym)
feedTableCode = redFeedTable:getID()
red:addSymbolTable(redFeedTable)
red:setSymbolName("feed", feedTableCode, redFeedSym:getID())

redSymTable = SymbolTable(redColor, colorTableCode)
red:addSymbolTable(redSymTable)
redSymTable:setDynamic(true)
redSymTable:setName("color")
red:setSymbolName("color", colorTableCode, redColor:getID())

-- Symbols acquisition
red:addObjectSymbolAcquisition(colorTableCode, colorTableCode)
red:addMessageSymbolAcquisition(colorTableCode)

-- Agent Brain

brain = Gridbrain()

brain:setMutateAddConnectionProb(addConnectionProb)
brain:setMutateRemoveConnectionProb(removeConnectionProb)
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
alphaSet:addComponent(PER(Sim2D.PERCEPTION_SYMEQ, colorTableCode, redColor:getID(), colorTableCode, true))
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
soundSet:addComponent(PER(Sim2D.PERCEPTION_SYMEQ, colorTableCode, redColor:getID(), colorTableCode, true))
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
betaSet:addComponent(ACT(Sim2D.ACTION_FIREB, colorTableCode, redColor:getID(), colorTableCode, false))
betaSet:addComponent(ACT(Sim2D.ACTION_SPEAK, colorTableCode, redColor:getID(), colorTableCode, true))
    
grid2 = Grid()
grid2:init(Grid.BETA, 0, 0)
grid2:setComponentSet(betaSet)

brain:addGrid(grid2, "beta")

red:setBrain(brain)

-- Blues
--------------------------------------------------------------------------------

blue = SimObj2D()
blue:setBirthRadius(agentBirthRadius)
blue:setSize(agentSize)
blue:setDrag(drag)
blue:setRotDrag(rotDrag)
blue:setInitialEnergy(initialEnergy)
blue:setMaxAge(maxAgeLow, maxAgeHigh)
blue:setViewRange(viewRange)
blue:setViewAngle(viewAngle)
blue:setGoCost(goCost)
blue:setRotateCost(rotateCost)
blue:setGoForceScale(goForceScale)
blue:setRotateForceScale(rotateForceScale)
blue:setShape(SimObj2D.SHAPE_TRIANGLE)
blue:setColoringSymbolName("color")
blue:setSoundRange(500)
blue:setFireInterval(fireInterval)
blue:setLaserLength(laserLength)
blue:setLaserSpeed(laserSpeed)
blue:setLaserRange(laserRange)
blue:setLaserStrengthFactor(laserStrengthFactor)
blue:setLaserCostFactor(laserCostFactor)
blue:setLaserHitDuration(laserHitDuration)
blue:setKeepBodyOnExpirationDeath(keepBodyOnExpire)
blue:setFeedCenter(0.5)

blueFoodSym = SymbolFloat(blueFood)
blueFoodTable = SymbolTable(blueFoodSym, foodTableCode)
blue:addSymbolTable(blueFoodTable)
blue:setSymbolName("food", foodTableCode, blueFoodSym:getID())

blueFeedSym = SymbolFloat(blueFeed)
blueFeedTable = SymbolTable(blueFeedSym, feedTableCode)
blue:addSymbolTable(blueFeedTable)
blue:setSymbolName("feed", feedTableCode, blueFeedSym:getID())

blueSymTable = SymbolTable(blueColor, colorTableCode)
blue:addSymbolTable(blueSymTable)
blueSymTable:setDynamic(true)
blueSymTable:setName("color")
blue:setSymbolName("color", colorTableCode, blueColor:getID())

-- Symbols acquisition
blue:addObjectSymbolAcquisition(colorTableCode, colorTableCode)
blue:addMessageSymbolAcquisition(colorTableCode)

-- Agent Brain

brain = Gridbrain()

brain:setMutateAddConnectionProb(addConnectionProb)
brain:setMutateRemoveConnectionProb(removeConnectionProb)
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
alphaSet:addComponent(PER(Sim2D.PERCEPTION_SYMEQ, colorTableCode, blueColor:getID(), colorTableCode, true))
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
soundSet:addComponent(PER(Sim2D.PERCEPTION_SYMEQ, colorTableCode, blueColor:getID(), colorTableCode, true))
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
betaSet:addComponent(ACT(Sim2D.ACTION_FIREB, colorTableCode, blueColor:getID(), colorTableCode, false))
betaSet:addComponent(ACT(Sim2D.ACTION_SPEAK, colorTableCode, blueColor:getID(), colorTableCode, true))
    
grid2 = Grid()
grid2:init(Grid.BETA, 0, 0)
grid2:setComponentSet(betaSet)

brain:addGrid(grid2, "beta")

blue:setBrain(brain)

-- Population Dynamics
--------------------------------------------------------------------------------

popDyn = PopDynSEGA()
sim:setPopulationDynamics(popDyn)

blueSpecies = Species(blue, numberOfBlues, bufferSize)
if blueRand then
    blueSpecies:addGoal(SimObj2D.FITNESS_RANDOM)
else
    blueSpecies:addGoal(SimObj2D.FITNESS_LASER_SCORE)
    blueSpecies:addGoal(SimObj2D.FITNESS_ENERGY_SUM_ABOVE_INIT)
end
blueSpecies:setFitnessAging(fitnessAging)
blueSpecies:setRecombineProb(recombineProb)
blueSpecies:setGroupFactor(groupFactor)
popDyn:addSpecies(blueSpecies)

redSpecies = Species(red, numberOfReds, bufferSize)
if redRand then
    redSpecies:addGoal(SimObj2D.FITNESS_RANDOM)
else
    redSpecies:addGoal(SimObj2D.FITNESS_LASER_SCORE)
    redSpecies:addGoal(SimObj2D.FITNESS_ENERGY_SUM_ABOVE_INIT)
end
redSpecies:setFitnessAging(fitnessAging)
redSpecies:setRecombineProb(recombineProb)
redSpecies:setGroupFactor(groupFactor)
popDyn:addSpecies(redSpecies)

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
stats:setFile("log_blue" .. logSuffix .. ".csv")
stats:addField("energy_sum_above_init")
stats:addField("laser_score")
stats:addField("target_score")
stats:addField("gb_connections")
stats:addField("gb_active_connections")
stats:addField("gb_grid_width_objects")
stats:addField("gb_grid_height_objects")
stats:addField("gb_grid_width_beta")
stats:addField("gb_grid_height_beta")
stats:addField("symtable_used_color")
blueSpecies:addDeathLog(stats)

stats = StatCommon()
stats:setFile("log_red" .. logSuffix .. ".csv")
stats:addField("energy_sum_above_init")
stats:addField("laser_score")
stats:addField("target_score")
stats:addField("gb_connections")
stats:addField("gb_active_connections")
stats:addField("gb_grid_width_objects")
stats:addField("gb_grid_height_objects")
stats:addField("gb_grid_width_beta")
stats:addField("gb_grid_height_beta")
stats:addField("symtable_used_color")
redSpecies:addDeathLog(stats)

if logBrains then
    logBrain = LogBestBrain()
    logBrain:setFileNameSuffix(".svg")
    if logOnlyLastBrain then
        logBrain:setLogOnlyLast(true)
        logBrain:setFileNamePrefix("brain_blue" .. logSuffix)
    else
        logBrain:setFileNamePrefix("brain_blue" .. logSuffix .. "t")
    end
    blueSpecies:addDeathLog(logBrain)

    logBrain = LogBestBrain()
    logBrain:setFileNameSuffix(".svg")
    if logOnlyLastBrain then
        logBrain:setLogOnlyLast(true)
        logBrain:setFileNamePrefix("brain_red" .. logSuffix)
    else
        logBrain:setFileNamePrefix("brain_red" .. logSuffix .. "t")
    end
    redSpecies:addDeathLog(logBrain)
end

blueSpecies:setBufferDump("bdump_blue" .. logSuffix)

redSpecies:setBufferDump("bdump_red" .. logSuffix)

popDyn:setLogTimeInterval(logTimeInterval)

-- Start Simulation
--------------------------------------------------------------------------------

sim:initGraphics(screenWidth, screenHeight, fullScreen, noGraphics)
sim:run()
