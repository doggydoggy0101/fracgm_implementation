# :croissant: C++ Implementation
A C++ implementation for the FracGM-based registration solver.
```shell
cd cpp
```

### :gear: Build
```shell
mkdir build
cd build
cmake ..
make
```

### :checkered_flag: Run
```shell
./fracgm_example
```

This main function will read two point clouds from [`data`](../data) folder and
solve the registration problems with FracGM.

```shell
Ground Truth:
-0.0318679   0.852338   0.522019        0.3
 -0.518338  -0.460655   0.720501        0.2
  0.854582  -0.247622    0.45648        0.6
         0          0          0          1

FracGM:
-0.0322524   0.852456   0.521803   0.299632
 -0.520236  -0.460095   0.719491   0.200032
  0.853413  -0.248255   0.458318   0.600476
         0          0          0          1

```