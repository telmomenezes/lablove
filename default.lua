experiment = "poison_buf"

expParam = getCommandLineParameter("exp");
if expParam ~= "" then experiment = expParam end

dofile("experiments/" .. experiment .. ".lua")
