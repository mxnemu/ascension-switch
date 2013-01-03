print("Hello world from lua");
print(Scene);
print(Scene.create);
scene = Scene.create()
print("created");
print(scene);
print(getmetatable(scene));
print(scene.addBackground);
print("adding da backgrounds");
scene.addBackground(scene, "images/mainmenu.png");
scene.addBackground(scene, "images/mainmenu.png");
scene.addBackground(scene, "images/mainmenu.png");

function createNextTrigger(lastTrigger)
    local trigger = Trigger.create(newX, createNextTrigger);
    local newX = lastTrigger.x + 800;
    
    scene.popFrontAndTranslateBack(scenePointer);
    scene.addBackground(scenePointer, "images/mainmenu.png");
    scene.addTrigger(trigger);
end
