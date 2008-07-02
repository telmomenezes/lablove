experiment = "experiments/programed_targets.lua"

expParam = getCommandLineParameter("exp");
if expParam ~= "" then experiment = expParam end

dofile(experiment)
