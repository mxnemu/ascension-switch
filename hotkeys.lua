
keyboardHotkeys = {
    -- movement
    ["left"] = {["key"]="a", ["axisValue"]=-1},
    ["right"] = {["key"]="d", ["axisValue"]=1},
    ["up"] = {["key"]="w", ["axisValue"]=1},
    ["down"] = {["key"]="s", ["axisValue"]=-1},
    
    ["kickLeft"] = {["key"]="j"},
    ["kickRight"] = {["key"]="k"},
    ["hitLeft"] = {["key"]="s"},
    ["hitRight"] = {["key"]="s"},
    ["action"] = {["key"]="e"}
}

-- TODO remove the keys and use ["name"] in the 2nd table instead
joystickHotkeys = {
    -- movement
    ["left"] = {["axis"]=0},
    ["right"] = {["axis"]=0},
    ["up"] = {["axis"]=1},
    ["down"] = {["axis"]=1},
    
    ["kickLeft"] = {["button"]= 0},
    ["kickRight"] = {["button"]=1},
    ["hitLeft"] = {["button"]=2},
    ["hitRight"] = {["button"]=3},
    ["action"] = {["button"]=4}
}
print("hotkeys are valid");
