
   7--------6
  /|       /|
 / |      / |
5--------4  |
|  3-----|--2
| /      | /
|/       |/
1--------0

A single tile consists of three parts: top, middle, bottom.

Entities are 2d flat textures without depths.

Implementation spec:

class physics: handle body movements of entities (based on map)

class monster_ai: an abstract class which handles monster movements

class monster_ai_shortest?: impl of monster_ai which support shortest route algos