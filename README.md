# TASK 1

## OBVIOUS INEFFICIENCIES
1. Using division where it can be substituted with multiplication - uses more processing power
2. Creating unnecessary reference variables - wastes memory
3. Constantly applying a matrix transformation to render the player, even if no change occured - wastes processing power
4. Updating objects even if they are off screen - big waste of processing power
5. Constantly spawning new objects and not properly destroying them - clogs up memory, very wasteful
6. Checking every bullet for collision against every asteroid every frame - very big waste of processing power
7. The asteroids seem to have no limit on how many times they can split - can clog up memory

I think most of the work will have to be done in the update() function where objects are managed.

## GAME PROBLEMS
1. Game often crashes on object collision 
- possible cause: a problem with the code execution order or data passing
2. Game slows down considerably after shooting a lot of bullets and destroying  lot of asteroids
- possible cause: too many objects being rendered and updated

## CPU PROFILER RESULTS
As I predicted, the function that utilizes the most processing power is the update() function.
I ran a 26.035second long diagnostics session and it showed that it took up as much as 88.98% of total CPU usage.
Next up was the pointIsInside() function with 87.40%.

## MEMORY PROFILER RESULTS
I ran a 2.33 minute memory diagnostics session and took snapshots at 10second intervals.
This is what I observed:
- In the first 5 snapshots there were  regular spikes in used data ranging from as low a 64.4kb to as high as 101.7kb, with an average of 79.195kb
- In the next 5 the spikes started slightly decreasing in size, now ranging from as low as 37.38kb to as high as 97.76, with an average of 59.784kb
- In the last 5 snapshots, there was a considerable drop in the size of the spikes, now ranging from as low as 2.26 to as high as 41.08, with an average of 12.974kb

The results matched my expectations, because when there is no limit on how many objects can be in the game at one time, as you keep spawning them, the performance is gradually going to drop.
Then as the performance drops, less instructions are executed in the same amount of time, therefore resulting in smaller and more irregular spikes in data usage.

# TASK 4

## BENCHMARK EXAMINATION
After further examination of my benchmarks, I found that in the update() function, most time is taken up by calling the pointIsInside() function,
this could be improved by reducing the number of objects being compared for collision at any one time.
Going more in depth into the pointIsInside() function itself, most time is taken by Vector2D operations such as normalise and magnitude.
Next are Matrix2D operations, mainly the Matrix2D operator* types and the setTransform function. 

## FURTHER ENHANCEMENTS
To improve the game's performance even more, I could reduce the number of calls to the pointIsInside() function by either comparing the distance from each bullet to each asteroid,
and only check for collision with the closest asteroid. Alternatively, I could split the game area into a grid, and compare the bullets only with the objects in the section the bullet is currently in.
I could also create an object pool for the asteroids, and implement a limit on how many asteroids can be on the screen at any one time to free up some memory and processing power.
