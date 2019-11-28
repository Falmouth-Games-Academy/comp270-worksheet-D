# comp270-worksheet-D
Base repository for COMP270 worksheet D

### NOTE: Drifter::updateExtra() has been causing crashes throughout the game and was therefore commented out due to being unable to pinpoint the exact issue - my guess is a derived class pointer is invalid.

# Task 1

* Application.Update() - performs collision detection between every asteroid and every bullet which exponentially slows down the game at runtime, as more bullets and asteroids are created. Additionally, when objects are 'killed' they are only marked as to not render on screen. However, they are never actually removed or repurposed. Both of these issues will cause large memory usage over time at runtime;
* Player.Update() - creates 2 entirely new vectors (plus an additional vector and matrix from Player.getDirection()) every update in order to update its position and speed - this will cause large drawbacks on the CPU;
* Player.draw() - is called every update frame and generates a new set of vertices as well as a transformation matrix upon being called, which is highly inefficient from a runtime speed viewpoint as it increases the CPU's workload;
* Asteroid.draw() - similarly, this function generates an entirely new set of vertices and transformation matrix upon being called. Additionally, this function will render the asteroid regardless of whether or not they are visible on the screen. This means more work for the CPU to render on the screen;

I believe the greatest workload will occur in the Update() function, in the nested foreach loop used for collision detection. It uses a sophisticated algorithm and has O(n^2) complexity from what I could tell, as the number of calculations increases exponentially with both the number of asteroids and bullets in the game (not just on screen)

## Notes on initial playthrough

* Player can shoot bullets at an extremely high frequency which slows down the entire game considerably after only a couple of seconds of firing - potentially caused by the number of collision detections performed between each bullet and asteroid on each update;
* Frequent crashes on hitting asteroid - caused by the updateExtra() method, cause of this appears to be the object reference being an invalid pointer;

## Benchmark results - CPU

As expected, out of the total CPU usage, the most expensive functions were, in order:
* Application::run
* Application::update
* Asteroid::pointIsInside
Taking up as much as 98.01% of all CPU usage (Figure 1)

This is clearly caused by the large number of asteroids and bullets being created every update which causes the collisiond detection to have such high CPU usage.

## Benchmark results - Memory

A steady, continuous increase was seen in the memory benchmark (from 20mB to 23 mB) - certainly caused by the lack of garbage collection, as no bullets or asteroids are ever 'destroyed'

Overall, the results of the benchmarks were well within expectations.

# Task 2

## Notes on chosen collision detection - Triangle fan method

Computationally, I believe it is very well suited for this project as the number of vertices on each asteroid is quite low. The asteroid's triangle areas are computed upon being spawned and are readily available when collision detection is required. This means that in the case of a 10 vertex asteroid, the total cost of a collision detection comes down to 60 multiplications and 150 additions.

The new benchmark results are:
* Application::run - 98.95%
* Application::Update - 97.89%
* Asterpod::pointIsInside - 96.41%
(Figure 2)

## Notes on early exit implementation

Upon adding the condition of bullets being within a certain range of an asteroid to collide, a significat incerase in performance was seen.

The new benchmark results are:
* Application::run - 94.23%
* Application::update - 87.65%
* Drifter::getPosition - 30%
(Figure 3)

As it can be deduced from these numbers, the bulk of calculations are now performed for the early exit and, unsurprisingly, this has tremendously increased overall performance.

# Task 3

## Notes on using std::vector

Vectors are dynamic in size, therefore will automatically update themselves to accommodate new variables. In the case of the benchmark, the number of asteroids increases dramatically and this requires large amounts of vector resizes (aka copy the contents of the old vector into a new, larger one)

Pros:
* Requires little maintenance;
* Doesn't require you to keep track of where items have been placed;
* Doesn't require manual resizing

Cons:
* Can easily become more expensive than arrays through constant resizing
* Read operations are slower than array reads (linear vs. constant complexity)
* Lack of control over number of elements

## Notes on number of asteroids

Upon further inspection of the code, only the asteroids that are 'alive' are drawn and updated, but they are still located in the asteroids vector.

In order to amend this situation, I propose the following solutions:

* Use of a fixed size array with a length predetermined as a macro. New asteroids are only created IF the array has empty slots;
* Removal of 'dead' OR 'out of map' asteroids from the vector/array - this approach will reasonably limit the memory usage while not having too much of an impact on performance;
* Usage of a single Drifter vector/fixed size array for both asteroids and bullets, with garbage collection and a limited allowance of asteroids;

## Notes upon asteroid system improvements

Upon running the benchmark again, the amount of used memory remained at a consistent level of 23mB, allowing me to run it for over a minute (Figure 4)

The implementation I chose was to create a fixed size pointer array to maximize efficiency, and I believe it paid off in the end as the game was actually able to run for over a minute with a maximum of 200 asteroids on screen while benchmarking. The cap can be easily adjusted from Application.h

# Task 4

## Notes on potential improvements

As seen in: (Figure 5)
* I believe that the game can be further improved by applying the same treatment I previously applied to the asteroids to bullets as well: this will increase render speed and memory allocation efficiency.
* Additionally, I believe I can further improve CPU efficiency by adding a magnitudeSquared emthod that does NOT perform the square root to get the magnitude, as the sqrt() method was seemingly the cause of much inefficiency.

## Switching to use of fixed size array for bullets

The effect (Figure 6) was similar in nature to what was noticed in the previous benchmark - the amount of updates performed dropped quite significantly, as well as the render calls.

## Swithing to the use of squared magnitudes

As seen in Figure 7, we have effectively removed the use of squared roots altogether by instead doing our early exit checks with a squared range and a squared magnitude instead.

## Further improvements

* I personally decided against bounding boxes because I believe that for such irregular and possibly random shapes such as asteroids a polygon collider would be more suitable.