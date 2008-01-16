dofile("experiments/scripts/basic_defines.lua")

numberOfPlants = 50
numberOfAgents = 50

agentMinSize = 7.0
agentMaxSize = 20.0
plantSize = 7.0

worldWidth = 2000
worldHeight = 2000

gridHeight = 5
gridAlpha = 5
gridBeta = 5

viewRange = 150.0
viewAngle = 170.0

maxAge = 5000

metabolism = 0.0
goCost = 0.0
rotateCost = 0.0
goForceScale = 0.3
rotateForceScale = 0.006

friction = 0.000--3
drag = 0.05
rotFriction = 0.00000--3
rotDrag = 0.05

initialConnections = 10

compCount = 1
bufferSize = 100
fitnessAging = 0.1

addConnectionProb = 0.02
removeConnectionProb = 0.02
changeWeightProb = 0.02
changeComponentProb = 0.02
weightMutationStanDev = 1.0
mutateBiomoprhSymbolProb = 0.1
mutateSizeSymbolProb = 0.1
mutateColorSymbolProb = 0.0

timeLimit = 0

----------------------------------------------

dofile("experiments/scripts/basic_command_line.lua")

sim = SimCont2D()
sim:setWorldDimensions(worldWidth, worldHeight, 150)
sim:setViewRange(viewRange)
sim:setViewAngle(viewAngle)
sim:setGoCost(goCost)
sim:setRotateCost(rotateCost)
sim:setGoForceScale(goForceScale)
sim:setRotateForceScale(rotateForceScale)
sim:setSeedIndex(seedIndex)
sim:setTimeLimit(timeLimit)

popDyn = PopDynSpeciesBuffers()
sim:setPopulationDynamics(popDyn)
popDyn:setCompCount(compCount)
popDyn:setFitnessAging(fitnessAging)

sizeTableCode = 0
physicsTableCode = 1 
energyTableCode = 2
ageTableCode = 3
colorTableCode = 4
feedTableCode = 5
biomorphTableCode = 6

function addAgentSpecies(red, green, blue, feed, food)
    agent = Agent()

    agent:addGraphic(GraphicBiomorph())

    symSize = SymbolFloat(agentMinSize, agentMinSize, agentMaxSize)
    symSize:initRandom()
    symTable = SymbolTable(symSize, sizeTableCode)
    symTable:setMutateSymbolProb(mutateSizeSymbolProb)
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

    symInitialEnergy = SymbolFloat(1.0)
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
    symTable:setMutateSymbolProb(mutateColorSymbolProb)
    agent:addSymbolTable(symTable)
    agent:setSymbolName("color", colorTableCode, 0)

    agentFeed = SymbolFixedString("abc", feed)
    agentFood = SymbolFixedString("abc", food)
    agentFeedTable = SymbolTable(agentFeed, feedTableCode)
    agentFeedTable:addSymbol(agentFood)
    agent:addSymbolTable(agentFeedTable)
    agent:setSymbolName("feed", feedTableCode, 0)
    agent:setSymbolName("food", feedTableCode, 1)

    symBiomorph = SymbolFloatVector(19)
    symBiomorph:initRandom()
    symTable = SymbolTable(symBiomorph, biomorphTableCode)
    symTable:setMutateSymbolProb(mutateBiomoprhSymbolProb)
    agent:addSymbolTable(symTable)
    agent:setSymbolName("biomorph", biomorphTableCode, 0)

    brain = Gridbrain()

    brain:setMutateAddConnectionProb(addConnectionProb)
    brain:setMutateRemoveConnectionProb(removeConnectionProb)
    brain:setMutateChangeConnectionWeightProb(changeWeightProb)
    brain:setMutateChangeComponentProb(changeComponentProb)
    brain:setWeightMutationStanDev(weightMutationStanDev)

    alphaSet = GridbrainComponentSet()
    alphaSet:addComponent(PER, SimCont2D.PERCEPTION_POSITION)
    alphaSet:addComponent(PER, SimCont2D.PERCEPTION_DISTANCE)
    alphaSet:addComponent(PER, SimCont2D.PERCEPTION_TARGET)
    alphaSet:addComponent(PER, SimCont2D.PERCEPTION_SYMBOL, SYM_TO_SYM, feedTableCode, 0, feedTableCode, 1)
    alphaSet:addComponent(PER, SimCont2D.PERCEPTION_SYMBOL, SYM_TO_SYM, feedTableCode, 1, feedTableCode, 0)
    alphaSet:addComponent(GridbrainComponent.TAND)
    alphaSet:addComponent(GridbrainComponent.MMAX)
    alphaSet:addComponent(GridbrainComponent.TNAND)

    grid = Grid()
    grid:init(Grid.ALPHA, gridAlpha, gridHeight)
    grid:setComponentSet(alphaSet)

    brain:addGrid(grid, "objects");

    betaSet = GridbrainComponentSet()
    betaSet:addComponent(GridbrainComponent.ACT, SimCont2D.ACTION_GO)
    betaSet:addComponent(GridbrainComponent.ACT, SimCont2D.ACTION_ROTATE)
    betaSet:addComponent(GridbrainComponent.ACT, SimCont2D.ACTION_EAT)
    betaSet:addComponent(GridbrainComponent.TAND)
    betaSet:addComponent(GridbrainComponent.TNAND)
    
    grid2 = Grid()
    grid2:init(Grid.BETA, gridBeta, gridHeight)
    grid2:setComponentSet(betaSet)

    brain:addGrid(grid2, "beta")

    agent:setBrain(brain)

    brain:addRandomConnections(initialConnections)

    popDyn:addSpecies(agent, numberOfAgents, bufferSize)
end

addAgentSpecies(255, 0, 0, "aaa", "bbb")
addAgentSpecies(10, 10, 10, "bbb", "ccc")
addAgentSpecies(10, 100, 255, "ccc", "aaa")

plant = GraphicalObject()

symSize = SymbolFloat(plantSize)
symTable = SymbolTable(symSize, sizeTableCode)
plant:addSymbolTable(symTable)
plant:setSymbolName("size", sizeTableCode, 0)

symPlantInitialEnergy = SymbolFloat(1.0)
symTable = SymbolTable(symPlantInitialEnergy, energyTableCode)
plant:addSymbolTable(symTable)
plant:setSymbolName("initial_energy", energyTableCode, 0)

plantColor = SymbolRGB(0, 255, 0)
symTable = SymbolTable(plantColor, colorTableCode)
plant:addSymbolTable(symTable)
plant:setSymbolName("color", colorTableCode, 0)

plantFeed = SymbolFixedString("abcd", "ddd")
plantFood = SymbolFixedString("abc", "aaa")
plantFeedTable = SymbolTable(plantFeed, feedTableCode)
plantFeedTable:addSymbol(plantFood)
plant:addSymbolTable(plantFeedTable)
plant:setSymbolName("feed", feedTableCode, 0)
plant:setSymbolName("food", feedTableCode, 1)

plant:addGraphic(GraphicSquare())

popDyn:addSpecies(plant, numberOfPlants, 1)

human = Agent()
dummyBrain = DummyBrain(1)
dummyBrain:setChannelName(0, "objects")
dummyBrain:addPerception("Position", 0, SimCont2D.PERCEPTION_POSITION)
dummyBrain:addPerception("Distance", 0, SimCont2D.PERCEPTION_DISTANCE)
dummyBrain:addPerception("Contact", 0, SimCont2D.PERCEPTION_IN_CONTACT)
dummyBrain:addPerception("Color", 0, SimCont2D.PERCEPTION_SYMBOL, colorTableCode, 0, colorTableCode, 0)
dummyBrain:addPerception("Food", 0, SimCont2D.PERCEPTION_SYMBOL, feedTableCode, 0, feedTableCode, 1)
dummyBrain:addPerception("Predator", 0, SimCont2D.PERCEPTION_SYMBOL, feedTableCode, 1, feedTableCode, 0)

human:setBrain(dummyBrain)

symSize = SymbolFloat(10.0)
symTable = SymbolTable(symSize, sizeTableCode)
human:addSymbolTable(symTable)
human:setSymbolName("size", sizeTableCode, 0)

symHFriction = SymbolFloat(friction)
symHDrag = SymbolFloat(drag)
symHRotFriction = SymbolFloat(rotFriction)
symHRotDrag = SymbolFloat(rotDrag)
symTable = SymbolTable(symHFriction, physicsTableCode)
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

humanFeed = SymbolFixedString("abc", "aaa")
humanFood = SymbolFixedString("abc", "bbb")
humanFeedTable = SymbolTable(humanFeed, feedTableCode)
humanFeedTable:addSymbol(humanFood)
human:addSymbolTable(humanFeedTable)
human:setSymbolName("feed", feedTableCode, 0)
human:setSymbolName("food", feedTableCode, 1)

human:addGraphic(GraphicTriangle())
sim:addObject(human)
sim:setPos(human, 300, 300)
sim:setHuman(human)

sim:initGraphics(screenWidth, screenHeight, fullScreen, noGraphics)
sim:run()
