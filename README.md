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
In the first test the pointIsInside function is run many times, which was what I was expecting as for every bullet that is shot, each asteroid checks its position.
![alt text](https://i.imgur.com/JC5jmA1.png)
I then ran the profiler with the tests active and was surprised to see that the matrix multiplication function was running the most. Upon investigating this, it was because the test had the asteroid probability set to 100, and the matrix multiplication is called in the asteroids draw function. This is further cause for reducing the amount of asteroids there should be.
![alt text](https://i.imgur.com/baM4Kna.png)

### Part E
For the memory test I expected the memory to spike whenever a bullet collided with an asteroid as it would then generate a new asteroid. I also noticed that when shooting the memory usage increased slightly. This becomes an issue, you can see in snapshot 4, when you hold down the shoot button.
![alt text](https://i.imgur.com/NyfDo46.png)
Similarly, I ran the test case with the asteroid spawn rate at 100, and it was entirely what I expected. The memory allocation increased exponentially as more asteroids were spawned.
![alt text](https://i.imgur.com/77C3jIp.png)