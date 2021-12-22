
# Raytracer

<img src="./images/sphere-heavy.png" width=650 alt="Example"></img>

To compile and run the raytracer locally, first install [nlohmann/json](https://github.com/nlohmann/json) and then execute the bash file:
```bash
run.sh
```
Scenes are described in a JSON format.
It looks like this: <br>
(full example [config.json](./config.json))
```json
{
    "width": 500,
    "height": 250,
    "vpw": 4,
    "vph": 2,
    "focal_length": 2,
    "cam_origin": [0.0, 0.0, 0.0],
    "sky_gradient": [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],

    "n_threads": 4,
    "samples": 10,
    "max_bounces": 50,
    "fog_factor": 0.2,

    "objects": [
        {
            "type": "sphere",
            "pos": [-0.6, 0.0, -2.0],
            "radius": 0.5,
            "props": {
                "smooth": 0.0,
                "color": [0.8, 0.2, 0.2],
                "glow": 0.0,
                "reflect": 0.5
            }
        },
        ...
        {
            "type": "cuboid",
            "pos": [0.0, -0.5, -2.25],
            "w": 0.25,
            "h": 0.25,
            "d": 0.25,
            "props": {
                "smooth": 0.0,
                "color": [0.2, 0.4, 0.8],
                "glow": 0.0,
                "reflect": 0.5
            }
        }
    ]
}
```
