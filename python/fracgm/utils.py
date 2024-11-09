import numpy as np


# A structure that can be used to compute the quadratic form associated
# with a matrix, and keep track of the most recently computed value.
class R2Sym:
    def __init__(self, mat, cache=0.0):
        self.mat = mat
        self.cache = cache

    # Compute the quadratic form associated with self and x.
    def call(self, x):
        return x.T @ self.mat @ x

    # Update the cached value of the quadratic form associated with self.
    def update_cache(self, x):
        self.cache = self.call(x)


# A structure to represent a fractional term $f(x)/h(x)$ in the
# Geman-McClure-based objective function.
class Fractional:
    def __init__(self, r2, c2):
        self.r2 = r2
        self.c2 = c2

    # Updates the cache of the square of residual.
    def update_cache(self, x):
        self.r2.update_cache(x)

    # Computes the numerator $f(x)$.
    def f(self):
        return self.c2 * self.r2.cache

    # Computes the denominator $h(x)$.
    def h(self):
        return self.r2.cache + self.c2

    # Get the matrix associated with the numerator.
    def f_mat(self):
        return self.c2 * self.r2.mat

    # Get the matrix associated with the denominator.
    def h_mat(self):
        return self.r2.mat


def get_zero_mean_point_cloud(pcd):
    mean = np.mean(pcd, axis=0)
    pcd -= mean

    return pcd, mean


def project(mat):
    assert mat.shape[0] == mat.shape[1], "Matrix must be square"
    assert mat.shape[0] == 3, "Matrix must be 3x3"

    U, _, Vt = np.linalg.svd(mat)
    rot = U @ Vt

    # reflection case
    if np.linalg.det(rot) < 0:
        D = np.diag(np.array([1.0, 1.0, -1.0]))
        rot = U @ D @ Vt

    return rot


def compute_initial_guess(pcd1, pcd2):
    pcd1, mean1 = get_zero_mean_point_cloud(pcd1)
    pcd2, mean2 = get_zero_mean_point_cloud(pcd2)

    mat = np.eye(4)
    mat[:3, :3] = project(pcd2.T @ pcd1)
    mat[:3, 3] = mean2 - mean1

    return mat


def compute_terms(pcd1, pcd2, noise_bound_2, c2):
    terms = []
    id3 = np.eye(3)
    for i in range(pcd1.shape[0]):
        mat_n = np.zeros((3, 13))
        mat_n[:, :9] = np.kron(pcd1[i], id3)
        mat_n[:, 9:12] = id3
        mat_n[:, 12] = -pcd2[i]

        mat_m = (mat_n.T @ mat_n) / (noise_bound_2)
        terms.append(Fractional(R2Sym(mat_m), c2))

    return terms


def se3_mat_to_vec(mat):
    return np.array(
        [
            mat[0, 0],
            mat[1, 0],
            mat[2, 0],
            mat[0, 1],
            mat[1, 1],
            mat[2, 1],
            mat[0, 2],
            mat[1, 2],
            mat[2, 2],
            mat[0, 3],
            mat[1, 3],
            mat[2, 3],
            1.0,
        ]
    )


def se3_vec_to_mat(vec):
    return np.array(
        [
            [vec[0], vec[3], vec[6], vec[9]],
            [vec[1], vec[4], vec[7], vec[10]],
            [vec[2], vec[5], vec[8], vec[11]],
            [0.0, 0.0, 0.0, 1.0],
        ]
    )
