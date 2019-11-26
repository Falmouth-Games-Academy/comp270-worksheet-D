# comp270-worksheet-D
### Base repository for COMP270 worksheet D
---  

#### #1 (a) 
- Update 
> when splitting asteroids it never checks the scale, meaning it will keep splitting them when hit by a bullet until they are not visible, although they are still there in the scene being updated. they should be destroyed/killed once they go below a certain scale
> - spawnAsteroid  
> spawnAsteroid is called at random intervals and when a bullet intersects an asteroid. It adds a new asteroid to the vector each time which resizes the list (destroys and re-creates creating garbage) without checking if any dead (isAlive = false) asteroids can be recycled 

> - asteroid.pointIsInside
> it is called for every bullet to each asteroid (bullet_count * asteroid_count times), even if the asteroids and bullets are not close enough to intersect or the bullet is not heading for the asteroid. If there are a lot of bullets and/or asteroids the game will slow down dramatically.

> there are two calls to get the magnitude of a vector to check if its length is less than zero, which is not possible and getting the magnitude is a fairly slow operation. 

- Render
> - Both bullet and asteroid draws 
> Are called without checking if it is visible on the screen, if there are a lot of asteroids off screen it will case the game to slow down

#### #1 (b)
> if you hold down the spacebar it continually fires every frame. if you do this while aiming at an asteroid, when the bullets intersect the asteroid the games slows down extremely quickly.
##### Potential causes: (some stated in part a)
> There is no fire rate, meaning we can spawn a silly amount of bullets in a short period of time.
> There is no min scale on asteroids, meaning they will keep splitting even when barely or even not visible
> Bullets are never recycled, it always adds new ones to the vector (so the vector is resized every time a bullet is fired)
> Asteroids are never recycled, it always adds new ones to the vector (so the vector is resized every time a asteroid is spawned)
> Objects are drawn even when not visible
> Bullets are not destroyed (set to isAlive = false) when not visible

#### #1 (c)

> Spawns 1000 android on setup (normal spawning remained the same)  
> ship fires 150 bullets in burst while moving forwards and turning counter-clockwise (firers for 150 frames, pauses for 150 frames, also only moves while firering)  
> No users input is required  

> set 'Application::c_debuging' to true to run the test case.  
> The amount of initial asteroids and bullets to firer are defined just below.  

#### #1 (d)
Benchmark CPU
> ![CPU-MinSize](/Screenshoots/CPU%20-%20[Bench].png)  
> Looking at the test results we can see that  
> - Application::Update is taking up the most time. but its only spending ~1.2% in the function it self.  
> - after that Asteroid::PointInside is taking the longest (which is called from Application::Update)  
> - from there results everything else looks fairly neglectable, but we see.  
> this was expected as Update calls pretty much everything apart from draw and pointIsInside is called for every bullet to asteroids.  

#### #1 (e)
- Benchmark Memory
> ![CPU-MinSize](/Screenshoots/MEM%20-%20[Bench].png)  
> The memory is continually increasing since we are constantly add bullets and asteroids to their respected vectors.  


#### #2 (a) (implemented last)
> After implementing the barycentric Convex collision method there is a slight performance increase. I choose this since convex collision is one of the faster methods, but it is not most accurate, but accurate enough.  

![CPU-MinSize](/Screenshoots/CPU%20-%20[Convex].png)


#### #2 (b) (implemented with angles test and after the object recycling/pooling was implemented)

![CPU-MinSize](/Screenshoots/CPU%20-%20[ErlyExit].png)


#### #3 (a)
Pros:
> there’s no "maximum" amount of objects that can be spawned (within reason)
>

cons:
> Memory gets reallocated when objects are added to the list
> objects are not re used
> 

#### #3 (b)
> there are less objects active in the scene than in the vector. This is because objects are not recycled and used again nor removed from the vector/scene. This affects the performance since it has to resize the vectors when elements are added, and the memory will increase overtime as the objects are not removed.  

#### #3 (c)
> 1. you could use a fixed array and fill the array with objects set to not alive. Then you would do a look up to find the first object that is not active, move that object to the "spawn" location bring it back to life.  
> On the downside this would mean you would have a limited number of objects to spawn;  
> Or you could do the same thing with a vector so you can add to it if you run out of objects, maybe it would be better to resize the vector larger than the amount of object that are going to be added so there are less reallocation  
...  
  
2. I guess you could remove/delete the objects from the vector. :`(

#### #3 (d)
> So now asteroids and bullets are re-used after they have been killed, there is not all that much different and the memory still increases.

![CUP-array](/Screenshoots/CPU%20-%20[Array].png)
![MEM-array](/Screenshoots/MEM%20-%20[Array].png)

> i think this will be because there is no min size on asteroids, so we keep splitting them making more and more. some will be re-used, but more will be added.  

> After implementing the min size, we can that the memory is more stable now, and the performance is much smoother :)  

[CPU-MinSize](/Screenshoots/CPU%20-%20[MinSize].png)
[MEM-MinSize](/Screenshoots/MEM%20-%20[MinSize].png)

> But the bullet vector is still increasing even after fire a large number of bullets. I think this could be due to bullets being drawn/updated when off the screen.  

#### 4 (a)
> asteroids have min size.
> objects should be destroyed when off the screen
> In asteroid::draw we could work out the world verts in the same for loop as drawing and working out the first vert just before the loop :)

#### 4 (b)
> after implementing destroy when off screen, we can see that we spend less time in the update function as a whole and that the memory allocation is not pretty much stable  
![CPU-MinSize](/Screenshoots/CPU%20-%20[offScreen].png)
![CPU-MinSize](/Screenshoots/MEM%20-%20[OffScreen].png)

> After working out the worldVerts of the asteroid in the same loop as drawing we can see that we spend ~400units less time in the draw function. 
![CPU-MinSize](/Screenshoots/CPU%20-%20[vertAndDraw].png)


> Final Test results
> CPU
![CPU-allops](/Screenshoots/CPU%20-%20[all%20ops].png)

#####################

All Test where performed using the "Angle Summation Test"
Testing conditions:
Spawned 1000 android on setup (normal spawning remained the same)
ship fires 150 bullets in burst while moving forwards and turning counter-clockwise (firers for 150 frames, pauses for 150 frames, also only moves while firering)
No users input is required
All test preformed where ~1min





