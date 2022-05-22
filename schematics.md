An general block consists of six sides: 0, 1, 2, 3, 4, 5
    2
   ---
1 | 4 | 3
   ---
    0
(front: 0)
(left: 1)
(back: 2)
(right: 3)
(top: 4)
(bottom: 5)

A single tile consists of three parts: top, middle, bottom.

Entities are 2d flat textures without depths.

Implementation spec:

class physics: handle body movements of entities (based on map)

class monster_ai: an abstract class which handles monster movements

class monster_ai_shortest?: impl of monster_ai which support shortest route algos