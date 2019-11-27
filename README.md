# comp270-worksheet-D
Base repository for COMP270 worksheet D

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
Taking up as much as 98.01% of all CPU usage.

This is clearly caused by the large number of asteroids and bullets being created every update which causes the collisiond detection to have such high CPU usage.

## Benchmark results - Memory

A steady, continuous increase was seen in the memory benchmark (from 20mB to 23 mB) - certainly caused by the lack of garbage collection, as no bullets or asteroids are ever 'destroyed'

Overall, the results of the benchmarks were well within expectations.
