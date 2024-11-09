# FracGM
Python and C++ implementation of "FracGM: A Fast Fractional Programming Technique for Geman-McClure Robust Estimator." 

### :gear: Settings
```shell
sudo apt update
# python (optional)
sudo apt install -y python3 python3-dev
python3 -m pip install numpy scipy
# c++ (optional)
sudo apt install -y g++ build-essential cmake
sudo apt install -y libeigen3-dev
```

### :minidisc: Implementation
- [:crab: Official Implementation]([examples/rust](https://github.com/StephLin/FracGM))
- [:snake: Python Implementation](python)
- [:croissant: C++ Implementation](cpp)


### :memo: Note

- The implementation does not include the maximum clique inlier selection (MCIS) feature.
- For more information, please refer to our paper:

  [Bang-Shien Chen](https://dgbshien.com/), [Yu-Kai Lin](https://github.com/StephLin), [Jian-Yu Chen](https://github.com/Jian-yu-chen), [Chih-Wei Huang](https://sites.google.com/ce.ncu.edu.tw/cwhuang/), [Jann-Long Chern](https://math.ntnu.edu.tw/~chern/), [Ching-Cherng Sun](https://www.dop.ncu.edu.tw/en/Faculty/faculty_more/9), **FracGM: A Fast Fractional Programming Technique for Geman-McClure Robust Estimator**. _To appear in IEEE Robotics and Automation Letters (RA-L)_, 2024. (paper) ([preprint](https://arxiv.org/pdf/2409.13978)) ([code](https://github.com/StephLin/FracGM))