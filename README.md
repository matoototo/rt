
# Raytracer

<img src="./images/example.png" width=500 alt="Example"></img>

To compile and run the raytracer locally, you can execute the bash file:
```bash
run.sh
```
The scene is described in the [main.cpp](./main.cpp) file:
```cpp
...

Scene scene = Scene(1e3);
scene.add_sphere(point3{-0.6, 0, -2}, 0.5, color{1, 0, 0}, 0.2);
scene.add_sphere(point3{0, -100.5, -2}, 100, color{0, 1, 0}, 0.0);
scene.add_sphere(point3{0.6, 0, -2}, 0.5, color{1, 1, 1}, 2.5);

scene.add_rectangle(point3{-2.0, -0.7, -2.75}, 4, 1.5, color{0, 0, 0}, 1.1, z_side);
scene.add_rectangle(point3{-2.0, -0.7, -2.75}, -4, 1.5, color{0, 0, 0}, 1.1, x_side);
scene.add_rectangle(point3{2.0, -0.7, -2.75}, -4, 1.5, color{0, 0, 0}, 1.1, x_side);

scene.add_cuboid(point3{0.75, -0.6, -1.5}, 0.25, 0.75, 0.25, color{0, 1, 0}, 0.2);
scene.add_cuboid(point3{-0.175, 0.5, -2.25}, 0.35, 0.35, 0.35, color{0, 0, 1.0}, 0.2);

...
```
The description above creates the example image.
