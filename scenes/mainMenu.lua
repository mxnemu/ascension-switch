-- check if required variables are provided
if nil == Scene_addBackground or 
   nil == scenePointer then 
    -- cancel if required variables are not provided
    print("required scene variables are not provided");
    return nil;
end

print("Hello world from lua");
--[[
backgrounds = {
    "images/mainmenu.png",
    "images/mainmenu.png",
    "images/mainmenu.png"
}
--]]
print(scenePointer);
print(Scene_addBackground);
Scene_addBackground(scenePointer, "images/mainmenu.png");
Scene_addBackground(scenePointer, "images/mainmenu.png");
Scene_addBackground(scenePointer, "images/mainmenu.png");

function createNextTrigger(lastTrigger)
    local newX = lastTrigger.x + 800;
    Scene_popFrontAndTranslateBack(scenePointer);
    Scene_addBackground(scenePointer, "images/mainmenu.png");
    Scene_addTrigger(scenePointer, {
        ["x"] = newX,
        ["w"] = lastTrigger.w,
        ["onTrigger"] = createNextTrigger
    });
end
