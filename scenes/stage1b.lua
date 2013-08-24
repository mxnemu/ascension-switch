print("Hello world from lua");
-- must be called scene! Otherwise the game won't detect it.
-- The gameEngine lightUserData is a pointer required for init. It's always set.
-- This is probably evil.
scene = Scene.create(gameEngine);

print("adding the bg");
scene:setBackground("images/bgBlue.png");
