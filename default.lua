experiment = "experiments/predprey.lua"

expParam = getCommandLineParameter("exp");
if expParam ~= "" then experiment = expParam end

dofile(experiment)
