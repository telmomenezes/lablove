numberOfPlants = 100
numberOfAgents = 100

agentSize = 10.0
plantSize = 10.0

worldWidth = 3000
worldHeight = 3000

gridHeight = 3
gridAlpha = 3
gridBeta = 2

THR = false
MAX = false
MMAX = true
MUL = false
AND = false
NOT = true
TAND = true
TNAND = true

viewRange = 150.0
viewAngle = 170.0

highAgeLimit = 5500
lowAgeLimit = 4500

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

tournamentSize = 2

addConnectionProb = 0.01
removeConnectionProb = 0.01
changeWeightProb = 0.01
weightMutationStanDev = 1.0
splitConnectionProb = 0.01
joinConnectionsProb = 0.01
changeComponentProb = 0.01
swapComponentProb = 0.01

timeLimit = 0
logTimeInterval = 100

humanAgent = false

----------------------------------------------

dofile("experiments/aux/basic_command_line.lua")

addConnectionProb = getNumberParameter("addconnprob", addConnectionProb)
removeConnectionProb = getNumberParameter("removeconnprob", removeConnectionProb)
changeWeightProb = getNumberParameter("changeweightprob", changeWeightProb)
weightMutationStanDev = getNumberParameter("weightmutstandev", weightMutationStanDev)
splitConnectionProb = getNumberParameter("splitconnprob", splitConnectionProb)
joinConnectionsProb = getNumberParameter("joinconnprob", joinConnectionsProb)
changeComponentProb = getNumberParameter("changecompprob", changeComponentProb)
swapComponentProb = getNumberParameter("swapcompprob", swapComponentProb)

logSuffix = "_foragers_"
            .. addConnectionProb
            .. "_"
            .. removeConnectionProb
            .. "_"
            .. changeWeightProb
            .. "_"
            .. splitConnectionProb
            .. "_"
            .. joinConnectionsProb
            .. "_"
            .. changeComponentProb
            .. "_"
            .. swapComponentProb
            .. "s"
            .. seedIndex

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

symInitialEnergy = SymbolFloat(1.0)
symMetabolism = SymbolFloat(metabolism)
symTable = SymbolTable(symInitialEnergy)
symTable:addSymbol(symMetabolism)
agent:addSymbolTable(symTable)
energyTableCode = symTable:getID()
agent:setSymbolName("initial_energy", energyTableCode, 0)
agent:setSymbolName("metabolism", energyTableCode, 1)

symLowAgeLimit = SymbolUL(lowAgeLimit)
symHighAgeLimit = SymbolUL(highAgeLimit)
symTable = SymbolTable(symLowAgeLimit)
symTable:addSymbol(symHighAgeLimit)
agent:addSymbolTable(symTable)
ageTableCode = symTable:getID()
agent:setSymbolName("low_age_limit", ageTableCode, 0)
agent:setSymbolName("high_age_limit", ageTableCode, 1)

agentColor = SymbolRGB(255, 0, 0)
symTable = SymbolTable(agentColor)
agent:addSymbolTable(symTable)
colorTableCode = symTable:getID()
agent:setSymbolName("color", colorTableCode, 0)

agentFeed = SymbolFixedString("abc", "aaa")
agentFood = SymbolFixedString("abc", "bbb")
agentFeedTable = SymbolTable(agentFeed)
agentFeedTable:addSymbol(agentFood)
agent:addSymbolTable(agentFeedTable)
feedTableCode = agentFeedTable:getID()
agent:setSymbolName("feed", feedTableCode, 0)
agent:setSymbolName("food", feedTableCode, 1)

brain = Gridbrain()

brain:setMutateAddConnectionProb(addConnectionProb)
brain:setMutateRemoveConnectionProb(removeConnectionProb)
brain:setMutateChangeConnectionWeightProb(changeWeightProb)
brain:setWeightMutationStanDev(weightMutationStanDev)
brain:setMutateSplitConnectionProb(splitConnectionProb)
brain:setMutateJoinConnectionsProb(joinConnectionsProb)
brain:setMutateChangeComponentProb(changeComponentProb)
brain:setMutateSwapComponentProb(swapComponentProb)

alphaSet = GridbrainComponentSet()
if THR then
    alphaSet:addComponent(GridbrainComponent.THR)
end
if MAX then
    alphaSet:addComponent(GridbrainComponent.MAX)
end
if MMAX then
    alphaSet:addComponent(GridbrainComponent.MMAX)
end
if MUL then
    alphaSet:addComponent(GridbrainComponent.MUL)
end
if AND then
    alphaSet:addComponent(GridbrainComponent.AND)
end
if NOT then
    alphaSet:addComponent(GridbrainComponent.NOT)
end
if TAND then
    alphaSet:addComponent(GridbrainComponent.TAND)
end
if TNAND then
    alphaSet:addComponent(GridbrainComponent.TNAND)
end
alphaSet:addComponent(GridbrainComponent.PER, SimCont2D.PERCEPTION_POSITION)
alphaSet:addComponent(GridbrainComponent.PER, SimCont2D.PERCEPTION_DISTANCE)
alphaSet:addComponent(GridbrainComponent.PER, SimCont2D.PERCEPTION_OBJECT_FEATURE, feedTableCode, 0, 1)

grid = Grid()
grid:init(Grid.ALPHA, gridAlpha, gridHeight)
grid:addComponentSet(alphaSet)

brain:addGrid(grid, "objects");

betaSet = GridbrainComponentSet()
if THR then
    betaSet:addComponent(GridbrainComponent.THR)
end
if AND then
    alphaSet:addComponent(GridbrainComponent.AND)
end
if MUL then
    betaSet:addComponent(GridbrainComponent.MUL)
end
if NOT then
    betaSet:addComponent(GridbrainComponent.NOT)
end
if TAND then
    betaSet:addComponent(GridbrainComponent.TAND)
end
if TNAND then
    betaSet:addComponent(GridbrainComponent.TNAND)
end
betaSet:addComponent(GridbrainComponent.ACT, SimCont2D.ACTION_GO)
betaSet:addComponent(GridbrainComponent.ACT, SimCont2D.ACTION_ROTATE)
betaSet:addComponent(GridbrainComponent.ACT, SimCont2D.ACTION_EAT)
    
grid2 = Grid()
grid2:init(Grid.BETA, gridBeta, gridHeight)
grid2:addComponentSet(betaSet)

brain:addGrid(grid2, "beta")

agent:setBrain(brain)

brain:addRandomConnections(initialConnections)

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

popDyn = PopDynFixedSpecies()
sim:setPopulationDynamics(popDyn)

popDyn:setTournamentSize(10)
agentSpeciesIndex = popDyn:addSpecies(agent, numberOfAgents)
popDyn:addSpecies(plant, numberOfPlants)

if humanAgent then
    human = Agent()
    dummyBrain = DummyBrain(1)
    dummyBrain:setChannelName(0, "objects")
    dummyBrain:addPerception("Position", 0, SimCont2D.PERCEPTION_POSITION)
    dummyBrain:addPerception("Distance", 0, SimCont2D.PERCEPTION_DISTANCE)
    dummyBrain:addPerception("Feed", 0, SimCont2D.PERCEPTION_OBJECT_FEATURE, feedTableCode, 0, 1)

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

    symHLowAgeLimit = SymbolUL(lowAgeLimit)
    symHHighAgeLimit = SymbolUL(highAgeLimit)
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
end

stats = StatCommon()
stats:setFile("logs/energy" .. logSuffix .. ".csv")
stats:addField("energy")
stats:addField("gb_connections")
popDyn:addDeathLog(agentSpeciesIndex, stats)

logBrain = LogBestBrain()
logBrain:setFileNamePrefix("logs/brains/brain" .. logSuffix .. "t")
logBrain:setFileNameSuffix(".svg")
popDyn:addDeathLog(agentSpeciesIndex, logBrain)

popDyn:setLogTimeInterval(logTimeInterval)

sim:initGraphics(screenWidth, screenHeight, fullScreen, noGraphics)
sim:run()
