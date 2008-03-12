parameterString = ""

function getNumberParameter(paramName, paramDefault, shortName)
    param = getCommandLineParameter(paramName)

    paramOut = 0.0
    if param == "" then
        paramOut = paramDefault
    else
        paramOut = tonumber(param)
    end

    if ((shortName ~= nil) and (paramOut ~= 0)) then
        parameterString = parameterString .. shortName .. paramOut
    end

    return paramOut
end

function getBoolParameter(paramName, paramDefault, shortName)
    param = getCommandLineParameter(paramName)
    
    result = false

    if param == "" then
        result = paramDefault
    else
        if param == "true" then
            result = true
        else
            result = false
        end
    end

    if result then
        parameterString = parameterString .. "_" .. shortName .. "_"
    end
    return result
end

function getStringParameter(paramName, paramDefault, shortName)
    param = getCommandLineParameter(paramName)

    paramOut = param
    if param == "" then
        paramOut = paramDefault
    end

    if ((shortName ~= nil) and (paramOut ~= "")) then
        parameterString = parameterString .. shortName .. paramOut
    end

    return paramOut
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
