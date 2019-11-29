# comp270-worksheet-D
Base repository for COMP270 worksheet D

Part A:
The issue with spawning the meteors the way they have been implemented could cause performance issues if too many meteors were to be spawned as there is no cap on the number of meteors that can enter the game, it is good to have a minimum and maximum amount of meteors in the game so that it flows smoothly and it's enjoyable to play. 

Part B:
The game would crash when the bullet collided with the some asteroids due to a "read access violation" the "updateExtra" function being called was where the crash would occur, commenting this out seemed to make the game run fine. The turning of the character was rather slow so I sped it up. There needed to be a delay on the shooting because when holding down the space bar you ran into performance issues.

Part C:
The character now fires 5 bullets per shot. Made asteroids spawn much more frequently, these changes effected performance massively.

Part D:
