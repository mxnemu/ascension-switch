print("Hello world from lua");
-- must be called scene! Otherwise the game won't detect it.
-- The gameEngine lightUserData is a pointer required for init. It's always set.
-- This is probably evil.
print("now its");
print(scene);
scene = Scene.create(gameEngine);
print("later its");
print(scene);

print("adding da backgrounds");
scene:addBackground("images/background2.png");
