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

### D)
-   30 second run tested, Application::run is said to take the most time,  most likely just because everything is called from it.
-   Application::render is second most frequent, with 99.6% of its time taken up by Asteroid::draw. Not terribly suprising, as there quickly becomes many (>100) asteroids on the screen which all need to be re-rendered every frame.
-   The next most used functions are just Matrix functions
-   Everything else is near irrelevent (< 5% cpu)

### E)
-   For the first 5 seconds, no significant increase in memory usage. From 5 seconds onwards, a steady climb of 1 - 2 MB per second. With the rapidly and exponentially increasing number of asteroids to track, this is not all too surprising