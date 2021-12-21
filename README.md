
# Raytracer

<img src="./images/sphere-heavy.png" width=650 alt="Example"></img>

To compile and run the raytracer locally, you can execute the bash file:
```bash
run.sh
```
The [outside2.png](./images/outside2.png) example is described in [main.cpp](./src/main.cpp) with the following code:
```cpp
...

Scene scene = Scene(1e3);

Props matte = Props(0.0, {0.8, 0.8, 0.8}, 0.0);
Props light = matte.glowed(2.5);
Props mirror = Props(0.8, {0.0, 0.0, 0.0}, 0.0, 0.9);

scene.add_sphere(point3{-0.6, 0, -2}, 0.5, mirror.colored({1.0, 0.5, 0.5}));
scene.add_sphere(point3{0, -100.5, -2}, 100, matte);
scene.add_sphere(point3{0.6, 0, -2}, 0.5, light.colored({1, 1, 1}));

scene.add_rectangle(point3{-2.0, -0.7, -2.75}, 4, 1.5, z_side, matte);
scene.add_rectangle(point3{-2.0, -0.7, -2.75}, -4, 1.5, x_side, matte);
scene.add_rectangle(point3{2.0, -0.7, -2.75}, -4, 1.5, x_side, matte);

scene.add_cuboid(point3{0.75, -0.6, -1.5}, 0.25, 0.75, 0.25, matte.colored({0.5, 1, 0.5}));
scene.add_cuboid(point3{-0.175, 0.5, -2.25}, 0.35, 0.35, 0.35, matte.colored({0.5, 0.5, 1.0}));
...
```
