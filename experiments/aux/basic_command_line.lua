screenWidth = 800
paramScreenWidth = getCommandLineParameter("swidth")
if paramScreenWidth ~= "" then screenWidth = tonumber(paramScreenWidth) end

screenHeight = 600
paramScreenHeight = getCommandLineParameter("sheight")
if paramScreenHeight ~= "" then screenHeight = tonumber(paramScreenHeight) end

fullScreen = false
paramFullScreen = getCommandLineParameter("fullscreen")
if paramFullScreen == "true" then fullScreen = true end

noGraphics = false
paramNoGraphics = getCommandLineParameter("nograph")
if paramNoGraphics == "true" then noGraphics = true end

seedIndex = -1
paramSeedIndex = getCommandLineParameter("seedindex")
if paramSeedIndex ~= "" then seedIndex = tonumber(paramSeedIndex) end
