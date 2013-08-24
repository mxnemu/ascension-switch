-- Fuck this lua setup this most retared thing I ever coded
-- 0 useful and fucks up the whole codeflow in any possible way

-- must be called scene! Otherwise the game won't detect it.
-- The gameEngine lightUserData is a pointer required for init. It's always set.
-- This is probably evil.
scene = Scene.create(gameEngine, lastScene);
scene:setColorPrefix("red");
scene:setMirrorTiles(0);
