experiment = "poison_from_scratch"

expParam = getCommandLineParameter("exp");
if expParam ~= "" then experiment = expParam end

dofile("experiments/" .. experiment .. ".lua")
