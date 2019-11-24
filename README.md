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

#### #1 (d)

#### #1 (e)
