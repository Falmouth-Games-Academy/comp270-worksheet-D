# comp270-worksheet-D
Base repository for COMP270 worksheet D


## Task 1
### A)
-   Could render using multiple layers so that The entire screen doesnt need to be be cleared every frame? Could be excessive that everything is being redrawn every frame
-   Guarenteed to spawn a new asteroid every frame?
-   Every asteroid is checking for a collision with every bullet, every frame...

### B)
-   CPU(?) usage is a constant and steady climb
-   Takes about a minute until the game is running about ~half speed
-   These may be caused by the asteroids not having a minimum size, or asteroids&bullets not despawning when off-screen
-   Sometimes when shooting an asteroid, an error will be thrown in Drifter::Update
-   Sometimes exception thrown at Drifter::Kill, dont know why

### C) done