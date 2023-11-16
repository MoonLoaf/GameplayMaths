# GameplayMaths

## Assignment for my course in Gameplay mathematics

This turned out as more of an art installation than a game. I'm using several mathematical concepts to make this scene in a visually pleasing way. These concepts can be read about in the source code but the basic concepts are as follows:

- Actors with cube and sphere meshes update their size based on the context of the "average" position of all actors in the scene
- Their colors are updated in realtime to represent their position in a 2d grid made up of the actors positions, this is done by lerping individual RGB values based on X and Y coordinates
- Their colors are inverted whenever they intersect with another actor
- The scene is walled off by planes that collide with specific actors, using a bitmask enum to select which to collide with. The outer ones collide with all types but the plane in the middle only collide with spheres
- The actors have a material using noise to get an artsy effect, parts of this material behaves based on the actors's speed

[Showcase Video](https://youtu.be/5GwJaHBKHzg)
[Noise Material](https://blueprintue.com/blueprint/k40kucf_/)
