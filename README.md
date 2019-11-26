# comp270-worksheet-D
Base repository for COMP270 worksheet D

# OBVIOUS INEFFICIENCIES
1. Using division where it can be substituted with multiplication - uses more processing power
2. Creating unnecessary reference variables - wastes memory
3. Constantly applying a matrix transformation to render the player, even if no change occured - wastes processing power
4. Constantly spawning new objects and not properly destroying them - clogs up memory, very wasteful

I think most of the work will have to be done in the update() function where objects are managed.

# GAME PROBLEMS
1. Game often crashes on object collision 
- possible cause: a problem with the code execution order or data passing
2. Game slows down considerably after shooting a lot of bullets and destroying  lot of asteroids
- possible cause: too many objects being rendered and updated