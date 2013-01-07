
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

-- TODO remove the keys and use ["name"] in the 2nd table instead
joystickHotkeys = {
    -- movement
    {["name"]="horizontal", ["axis"]=0},
    {["name"]="horizontal", ["axis"]=6},
    {["name"]="vertical", ["axis"]=1},
    {["name"]="vertical", ["axis"]=7},
    
    {["name"]="kickLeft", ["button"]= 0},
    {["name"]="kickRight", ["button"]=1},
    {["name"]="hitLeft", ["button"]=2},
    {["name"]="hitRight", ["button"]=3},
    {["name"]="action", ["button"]=4}
}
print("hotkeys are valid");
