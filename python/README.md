# :snake: Python Implementation
A Python implementation for the FracGM-based registration solver.
```shell
cd python
```

### :checkered_flag: Run
```shell
python3 ./fracgm_example.py
```

This main function will read two point clouds from [`data`](../data) folder and
solve the registration problems with FracGM.

```shell
Ground Truth:
[[-0.0318679   0.85233803  0.52201947  0.3       ]
 [-0.51833845 -0.46065527  0.7205012   0.2       ]
 [ 0.85458159 -0.2476219   0.45647969  0.6       ]
 [ 0.          0.          0.          1.        ]]

FracGM:
[[-0.03225242  0.85245618  0.52180289  0.29963157]
 [-0.52023615 -0.46009533  0.71949053  0.20003242]
 [ 0.85341322 -0.24825542  0.45831771  0.60047561]
 [ 0.          0.          0.          1.        ]]

 ```