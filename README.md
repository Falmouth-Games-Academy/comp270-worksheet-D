# comp270-worksheet-D
### Base repository for COMP270 worksheet D
---  

#### #1 (a) 
- Update 
> when spliting astroids it never checks the scale, meaning it will keep spliting them when hit by a bullet until they are not visable, although they are still there in the scene being updated. they should be destroyed/killed once they go below a certain scale

-- spwanAsteroid
> spwanAsteroid is called at random intervals and when a bullet intersects an astroid. It adds a new astroid to the vector each time which resizes the list (destroys and re-creats creating garbage) without checking if any dead (isAlive = flase) astroids can be recycled 

-- asteroid.pointIsInside
> it is called for every bullet to each asteroid (bullet_count * asteroid_count times), even if the astroids and bullets are not close enought to intersect or the bullet is not heading for the astroid. If there ar a lot of bullets and/or astroids the game will slow down dramatically.

> there are two calls to get the magitude of a vector to check if its len is less than zero, which is not possible and geting the magitude is a farly slow operation. (although they could be used for an erly exit)

- Render
-- Both bullet and astroid draws 
> Are called without checking if it is visible on the screen, if there are a lot of astroids off screen it will case the game to slow down

# NOTE.
-- Asteroid.draw
> is tranforming from local space to world space every draw, when it could store the points in world space and we could have a function to convert to local space on a need-be. theres a lot of multiplication converting to and from world space.
(it also happens in pointIsInside)

#### #1 (b)
if you hold down the spacebar it continuely fires every frame. if you do this while aiming at an astroid, when the bullets intersect the astdroid the games slows down extreamly quickly.
##### Potential causes: (some stated in part a)
> There is no fire rate, meaning we can spwan a silly amount of bullets in a shot period of time.
> There is no min scale on astdroids, meaning they will keep spliting even when barely or even not visable
> Bullets are never never recycled, it always adds new ones to the vector (so the vector is resized every time a bullet is fired)
> Astroids are never never recycled, it always adds new ones to the vector (so the vector is resized every time a astroid is spawned)
> Objects are drawn even when not visable
> Bullets are not destroyed (set to isAlive = false) when not visable

#### #1 (c)

Spwaned 1000 anderiods on setup (normal spwaning remained the same)
ship fires 150 bullets in burst while moving forwards and turning conter-clockwise (firers for 150 frames, pauses for 150 frames, also only moves while friering)
No users input is required

set 'Application::c_debuging' to true to run the test case.
The amount of initial asteriods and bullets to firer are defined just below.

#### #1 (d)
Benchmark CPU
![CPU-MinSize](/Screenshoots/CPU%20-%20[Bench].png]


#### #1 (e)
Benchmark Memory
![CPU-MinSize](/Screenshoots/MEM%20-%20[Bench].png]


#### #2 (a) (implermented last)
> Affter implermenting the barycentric Convex collision method there is a sligt preformance incress. I choose this since convex collision is one of the faster methods but it is not most accurate, but accurate enought.

![CPU-MinSize](/Screenshoots/CPU%20-%20[Convex].png]


#### #2 (b) (implermented with angles test and affter the object recyleing/pooling was implermented)

![CPU-MinSize](/Screenshoots/CPU%20-%20[ErlyExit].png]


#### #3 (a)
Pros:
> theres no "maximum" amount of objects that can be spwaned (within reason)
>

cons:
> Memory gets realocated when objects are added to the list
> objects are not re used
> 

#### #3 (b)
> there are less objects active in the scene than in the vector. This is because objects are not recycled and used again nor removed from the vector/scene. This affects the preformance since it has to resize the vectors when elements are added and the memory will incress overtime as the objects are not removed.

#### #3 (c)
> 1. you could use a fixed array and fill the array with objects set to not alive. Then you would do a look up to find the fist object that is not active, move that obejct to the "spawn" location bring it back to life.  
On the down side this would mean you would have a limited number of objects to spawn;
Or you could do the same thing with a vector so you can add to it if you run out of objects, maybe it would be better to resize the vector larger than the amount of object that are going to be added so there are less reallocation
...  
  
2. I guess you could removed/delete the objects from the vector. :`(

#### #3 (d)
So now asteroids and bullets are re-used after they have been killed, there is not all that much differents and the memmory still icresses.

(CUP-array)[/screenshoots/CPU - [Array].jpg]
(MEM-array)[/screenshoots/Mem - [Array].jpg]

i think this will be because there is no min size on asdroids so we keep spliting them makeing more and more. some will be re-used, but more will be added.

After implermenting the min size we can that the memory is more stable now, and the preformance is much smoother :)

(CPU-MinSize)[/screenshoots/CPU - [MinSize].jpg]
(MEM-MinSize)[/screenshoots/CPU - [MinSize].jpg]

But the bullet vector is still incressing even affter fire a larg amount of bullets. i think this could be due to bullets being dwarn/updated when off the screen.

#### 4 (a)
> asdroids have min size.
> objects should be destroyed when off the screen
> In asteroid::draw we could work out the world verts in the same for loop as drawing and working out the first vert just befor the loop :)

#### 4 (b)
affter implermenting destroy when off screen, we can see that we spend less time in the update function as a whole and that the memory allocation is not prity much stable
![CPU-MinSize](/Screenshoots/CPU%20-%20[OffScreen].png]

Affter working out the worldVerts of the astroid in the same loop as drawing we can see that we spend ~400units less time in the draw function. 
![CPU-MinSize](/Screenshoots/CPU%20-%20[vertAndDraw].png]



#####################

All Test where performed using the "Angle Summation Test"
Testing condisions:
Spwaned 1000 anderiods on setup (normal spwaning remained the same)
ship fires 150 bullets in burst while moving forwards and turning conter-clockwise (firers for 150 frames, pauses for 150 frames, also only moves while friering)
No users input is required
All test preformed where ~1min





