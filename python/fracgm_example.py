import numpy as np
from fracgm.registration import FracGM


CLOUD_SRC_PATH = "../data/cloud_src.txt"
CLOUD_DST_PATH = "../data/cloud_dst.txt"
GT_PATH = "../data/gt.txt"


def get_registration_test_data():
    src = np.loadtxt(CLOUD_SRC_PATH)
    dst = np.loadtxt(CLOUD_DST_PATH)

    dst[:, 0] += 0.3
    dst[:, 1] += 0.2
    dst[:, 2] += 0.6

    gt = np.eye(4)
    gt[:3, :3] = np.loadtxt(GT_PATH)
    gt[0, 3] = 0.3
    gt[1, 3] = 0.2
    gt[2, 3] = 0.6

    return src, dst, gt


src_reg, dst_reg, gt_reg = get_registration_test_data()
print("Ground Truth:", end="\n\n")
print(gt_reg, end="\n\n")


max_iteration = 100
noise_bound = 0.1
c = 1.0
tol = 1e-6

est_reg = FracGM(max_iteration, tol, c, noise_bound).solve(src_reg, dst_reg)
print("FracGM:", end="\n\n")
print(est_reg, end="\n\n")
