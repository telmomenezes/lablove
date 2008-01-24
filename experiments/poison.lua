-- POISON
-- Agents evolve to find and eat certain food items
-- Amount of energy gathered from food is realtive to symbol_distance(feed,food)
-- Food will be poisonous if symbol_binding(feed,food) < feedCenter
--------------------------------------------------------------------------------

dofile("basic_defines.lua")

-- Experiment Parameters
--------------------------------------------------------------------------------

numberOfPlants = 200
numberOfAgents = 50

agentSize = 10.0
plantSize = 10.0

worldWidth = 3000
worldHeight = 3000

alphaWidth = 4
betaWidth = 2
alphaHeight = 3
betaHeight = 3

alphaComponents = {TAND, TNAND, NOT, MMAX}
betaComponents = {TAND, TNAND, NOT}

viewRange = 150.0
viewAngle = 170.0

maxAge = 5000

initialEnergy = 1.0
metabolism = 0.0
goCost = 0.005
rotateCost = 0.0
goForceScale = 0.3
rotateForceScale = 0.006

friction = 0.0
drag = 0.05
rotFriction = 0.0
rotDrag = 0.05

feedCenter = 0.25

initialConnections = 0

compCount = 1
bufferSize = 100
fitnessAging = 0.1

addConnectionProb = 0.01
removeConnectionProb = 0.01
changeWeightProb = 0.0
weightMutationStanDev = 0.0--1.0
newWeightProb = 0.0
moveConnectionOrigProb = 0.0
splitConnectionProb = 0.0
joinConnectionsProb = 0.0
changeComponentProb = 0.01
swapComponentProb = 0.0

recombineProb = 0.0

timeLimit = 0
logTimeInterval = 100
logBrains = true
logOnlyLastBrain = true

humanAgent = false

-- Command line, log file names, etc
--------------------------------------------------------------------------------

dofile("basic_command_line.lua")

if random then
    r = randomZeroToOne()
    r = r * r * 0.05
    addConnectionProb = 0.01
    removeConnectionProb = 0.01
    r = randomZeroToOne()
    r = r * r * 0.05
    changeWeightProb = r
    r = randomZeroToOne()
    r = r * 5.0
    weightMutationStanDev = r
    r = randomZeroToOne()
    r = r * r * 0.05
    moveConnectionOrigProb = r
    r = randomZeroToOne()
    r = r * r * 0.05
    changeComponentProb = r
    r = randomZeroToOne()
    r = r * r * 0.1
    swapComponentProb = r
    r = randomZeroToOne()
    r = r * r * 0.1
    recombineProb = r
end

addConnectionProb = getNumberParameter("addconnprob", addConnectionProb, "con+")
removeConnectionProb = getNumberParameter("removeconnprob", removeConnectionProb, "con-")
changeWeightProb = getNumberParameter("changeweightprob", changeWeightProb, "wgt")
weightMutationStanDev = getNumberParameter("weightmutstandev", weightMutationStanDev, "wsd")
newWeightProb = getNumberParameter("newweightprob", newWeightProb, "new")
moveConnectionOrigProb = getNumberParameter("moveconnorigprob", moveConnectionOrigProb, "mor")
splitConnectionProb = getNumberParameter("splitconnprob", splitConnectionProb, "spl")
joinConnectionsProb = getNumberParameter("joinconnprob", splitConnectionProb, "joi")
changeComponentProb = getNumberParameter("changecompprob", changeComponentProb, "chg")
swapComponentProb = getNumberParameter("swapcompprob", swapComponentProb, "swp")
swapComponentProb = getNumberParameter("recombineprob", recombineProb, "rec")
bufferSize = getNumberParameter("buffersize", bufferSize, "buf")
compCount = getNumberParameter("compcount", compCount, "cc")
fitnessAging = getNumberParameter("fitnessaging", fitnessAging, "agi")

logSuffix = "_poison_"
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

agent:addGraphic(GraphicTriangle())

symSize = SymbolFloat(agentSize)
symTable = SymbolTable(symSize)
agent:addSymbolTable(symTable)
sizeTableCode = symTable:getID()
agent:setSymbolName("size", sizeTableCode, 0)

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
agent:setSymbolName("friction", physicsTableCode, 0)
agent:setSymbolName("drag", physicsTableCode, 1)
agent:setSymbolName("rot_friction", physicsTableCode, 2)
agent:setSymbolName("rot_drag", physicsTableCode, 3)

symInitialEnergy = SymbolFloat(initialEnergy)
symMetabolism = SymbolFloat(metabolism)
symTable = SymbolTable(symInitialEnergy)
symTable:addSymbol(symMetabolism)
agent:addSymbolTable(symTable)
energyTableCode = symTable:getID()
agent:setSymbolName("initial_energy", energyTableCode, 0)
agent:setSymbolName("metabolism", energyTableCode, 1)

symLowAgeLimit = SymbolUL(maxAge)
symHighAgeLimit = SymbolUL(maxAge)
symTable = SymbolTable(symLowAgeLimit)
symTable:addSymbol(symHighAgeLimit)
agent:addSymbolTable(symTable)
ageTableCode = symTable:getID()
agent:setSymbolName("low_age_limit", ageTableCode, 0)
agent:setSymbolName("high_age_limit", ageTableCode, 1)

agentColor = SymbolRGB(0, 0, 255)
symTable = SymbolTable(agentColor)
agent:addSymbolTable(symTable)
colorTableCode = symTable:getID()
agent:setSymbolName("color", colorTableCode, 0)

agentFeed = SymbolFloat(0.0)
agentFeedTable = SymbolTable(agentFeed)
agent:addSymbolTable(agentFeedTable)
feedTableCode = agentFeedTable:getID()
dummyTable = SymbolTable(agentFeed)
foodTableCode = dummyTable:getID()
agent:setSymbolName("feed", feedTableCode, 0)

-- Agent Brain

brain = Gridbrain()

brain:setMutateAddConnectionProb(addConnectionProb)
brain:setMutateRemoveConnectionProb(removeConnectionProb)
brain:setMutateChangeConnectionWeightProb(changeWeightProb)
brain:setWeightMutationStanDev(weightMutationStanDev)
brain:setMutateNewConnectionWeightProb(newWeightProb)
brain:setMutateMoveConnectionOriginProb(moveConnectionOrigProb)
brain:setMutateSplitConnectionProb(splitConnectionProb)
brain:setMutateJoinConnectionsProb(joinConnectionsProb)
brain:setMutateChangeComponentProb(changeComponentProb)
brain:setMutateSwapComponentProb(swapComponentProb)

alphaSet = GridbrainComponentSet()
for i, comp in pairs(alphaComponents) do
    alphaSet:addComponent(comp)
end
alphaSet:addComponent(PER, SimCont2D.PERCEPTION_POSITION)
alphaSet:addComponent(PER, SimCont2D.PERCEPTION_DISTANCE)
alphaSet:addComponent(PER, SimCont2D.PERCEPTION_TARGET)
alphaSet:addComponent(PER, SimCont2D.PERCEPTION_SYMBOL, SYM_TO_SYM, feedTableCode, 0, foodTableCode, 0)

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
betaSet:addComponent(ACT, SimCont2D.ACTION_EATB)
    
grid2 = Grid()
grid2:init(BETA, betaWidth, betaHeight)
grid2:setComponentSet(betaSet)

brain:addGrid(grid2, "beta")

agent:setBrain(brain)

brain:addRandomConnections(initialConnections)

-- Plants
--------------------------------------------------------------------------------

plant = GraphicalObject()

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

plantFood = SymbolFloat(0.0)
plantFood:setAlwaysRandom()
plantFoodTable = SymbolTable(plantFood, foodTableCode)
plantFoodTable:addSymbol(plantFood)
plant:addSymbolTable(plantFoodTable)
plant:setSymbolName("food", foodTableCode, 0)

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
popDyn:addSpecies(plant, numberOfPlants, 1)

-- Human Agent
--------------------------------------------------------------------------------

if humanAgent then
    human = Agent()
    dummyBrain = DummyBrain(1)
    dummyBrain:setChannelName(0, "objects")
    dummyBrain:addPerception("Position", 0, SimCont2D.PERCEPTION_POSITION)
    dummyBrain:addPerception("Distance", 0, SimCont2D.PERCEPTION_DISTANCE)
    dummyBrain:addPerception("Feed", 0, SimCont2D.PERCEPTION_SYMBOL, feedTableCode, 0, foodTableCode, 0)
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

    humanFeed = SymbolFloat(0.0)
    humanFeedTable = SymbolTable(humanFeed, feedTableCode)
    human:addSymbolTable(humanFeedTable)
    human:setSymbolName("feed", feedTableCode, 0)

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
