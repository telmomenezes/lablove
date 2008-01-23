parameterString = ""

function getNumberParameter(paramName, paramDefault, shortName)
    param = getCommandLineParameter(paramName)

    paramOut = 0.0
    if param == "" then
        paramOut = paramDefault
    else
        paramOut = tonumber(param)
    end

    if (shortName ~= nil) then
        parameterString = parameterString .. shortName .. paramOut
    end

    return paramOut
end

function getBoolParameter(paramName, paramDefault)
    param = getCommandLineParameter(paramName)

    if param == "" then
        return paramDefault
    elseif param == "true" then
        return true
    else
        return false
    end
end

screenWidth = getNumberParameter("swidth", 800)
screenHeight = getNumberParameter("sheight", 600)
fullScreen = getBoolParameter("fullscreen", false)
noGraphics = getBoolParameter("nograph", false)
seedIndex = getNumberParameter("seedindex", -1)
timeLimit = getNumberParameter("timelimit", 0)
random = getBoolParameter("random", false)

if (seedIndex >= 0) then
    seedIndex = seedIndex % 30
end
