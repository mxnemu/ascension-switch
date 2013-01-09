
keyboardHotkeys = {
    -- movement
    {["name"]="horizontal", ["key"]="a", ["axisValue"]=-1},
    {["name"]="horizontal", ["key"]="d", ["axisValue"]=1},
    {["name"]="vertical", ["key"]="w", ["axisValue"]=-1},
    {["name"]="vertical", ["key"]="s", ["axisValue"]=1},
    
    {["name"]="kickLeft", ["key"]="j"},
    {["name"]="kickRight", ["key"]="k"},
    {["name"]="hitLeft", ["key"]="s"},
    {["name"]="hitRight", ["key"]="s"},
    {["name"]="action", ["key"]="e"}
}

-- TODO maybe use this for more control about different hat buttons
--    {["name"]="horizontal", ["hat"]=0, ["hatDirection"]=4, ["hatAxisValue"]=1},
joystickHotkeys = {
    -- movement
    {["name"]="horizontal", ["axis"]=0},
    {["name"]="vertical", ["axis"]=1},
    {["name"]="horizontal", ["hat"]=0, ["hatDirection"]="horizontal"},
    {["name"]="vertical", ["hat"]=0, ["hatDirection"]="vertical"},
    
    {["name"]="kickLeft", ["button"]= 0},
    {["name"]="kickRight", ["button"]=1},
    {["name"]="hitLeft", ["button"]=2},
    {["name"]="hitRight", ["button"]=3},
    {["name"]="action", ["button"]=4}
}
print("hotkeys are valid");
