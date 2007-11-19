function getNumberParameter(paramName, paramDefault)
    param = getCommandLineParameter(paramName)

    if param == "" then
        return paramDefault
    else
        return tonumber(param)
    end
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
