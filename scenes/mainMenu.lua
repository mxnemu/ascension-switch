print("Hello world from lua");
-- must be called scene! Otherwise the game won't detect it.
-- The gameEngine lightUserData is a pointer required for init. It's always set.
-- This is probably evil.
scene = Scene.create(gameEngine);

print("adding da backgrounds");
scene:addBackground("images/mainmenu.png");
scene:addBackground("images/mainmenu.png");
scene:addBackground("images/mainmenu.png");

function createNextTrigger(lastTrigger)
    local trigger = Trigger.create(newX, createNextTrigger);
    local newX = lastTrigger.x + 800;
    
    scene:popFrontAndTranslateBack();
    scene:addBackground("images/mainmenu.png");
    scene:addTrigger(trigger);
end

trigger = Trigger.create(800, createNextTrigger);
print(scene);
scene.addTrigger(scene, trigger);
print(trigger);
