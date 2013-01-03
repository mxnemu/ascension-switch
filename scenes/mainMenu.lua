print("Hello world from lua");

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
