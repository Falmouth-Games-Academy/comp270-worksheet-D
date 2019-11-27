# comp270-worksheet-D
Base repository for COMP270 worksheet D

## Task 1
### Part A
Firstly asteroids are constantly checking to see if they should be spawned. The asteroids should be pooled I.E. The number of asteroids that can be spawned should be capped, and a request should be made when you go below that number.
The asteroids that are spwaning can have a velocity away from the screen. Asteroids also aren't deleting themselves if they're off screen. Same with bullets.
Each ateroids is constantly check each bullet to see if there's one inside it.
Deleting asteroids once their off screen would also mean you don't really have to check if it's alive before updating it.
There are more efficient ways to check for if a point is within geometry.
The update loop is rather entangled and doesn't exactly comply with SRP. Consider shifting the implimentation of a lot of the algorithms to functions within the classes. (E.G. move the implimentation of spawning new asteroids after collision into the asteroids class.)

### Part B
UpdateExtra threw a read access violation error each time a bullet collided with an asteroid, therefore I commented it out and the game ran fine.
Shooting the asteroids was alright if you shot only one bullet at a time. If you held the space bar down for a period longer than 8 seconds after startup there was visible drop in performance. I would assume that this is because each asteroid checks for the position of each bullet.
Generating large numbers of asteroids (by shooting them) was fine.

### Part C
Asteroid spawn rate increased to 100 and pressing space spawns 3 bullets.

### Part D
![alt text](https://i.imgur.com/JC5jmA1.png)
![alt text](https://i.imgur.com/baM4Kna.png)

### Part E
![alt text](https://i.imgur.com/NyfDo46.png)
![alt text](https://i.imgur.com/77C3jIp.png)