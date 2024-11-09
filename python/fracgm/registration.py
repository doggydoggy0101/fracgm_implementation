import numpy as np
import scipy as sp

from fracgm.utils import compute_initial_guess, compute_terms, project
from fracgm.utils import se3_mat_to_vec, se3_vec_to_mat


class FracGM:
    def __init__(self, max_iteration=1000, tol=1e-7, c=1, noise_bound=0.1):
        self.max_iter = max_iteration
        self.tol = tol
        self.c = c
        self.noise_bound = noise_bound

    def solve_beta_mu(self, terms):
        beta = [term.f() / term.h() for term in terms]
        mu = [1 / term.h() for term in terms]

        return beta, mu

    def solve_x(self, beta, mu, terms):
        mat_a = np.zeros((13, 13))
        for i in range(self.num):
            mat_a += mu[i] * terms[i].f_mat() - mu[i] * beta[i] * terms[i].h_mat()

        lu, piv = sp.linalg.lu_factor(mat_a)
        temp = sp.linalg.lu_solve((lu, piv), self.e)
        return (1 / (self.e @ temp)) * temp

    def compute_psi_norm(self, beta, mu, terms):
        assert len(beta) == len(mu)
        assert len(beta) == len(terms)

        loss = 0.0
        for i, term in enumerate(terms):
            a = -term.f() + beta[i] * term.h()
            b = -1.0 + mu[i] * term.h()
            loss += a * a + b * b

        return np.sqrt(loss)

    def update_terms_cache(self, terms, vec):
        for term in terms:
            term.update_cache(vec)

    def solve(self, pcd1, pcd2):
        assert pcd1.shape == pcd2.shape, "Input point clouds must have the same shape."
        self.num = pcd1.shape[0]
        self.e = np.array(
            [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0]
        )

        terms = compute_terms(pcd1, pcd2, self.noise_bound**2, self.c**2)

        init_mat = compute_initial_guess(pcd1, pcd2)
        x = se3_mat_to_vec(init_mat)
        beta, mu = self.solve_beta_mu(terms)

        for _ in range(self.max_iter):
            # alternating solve x
            x = self.solve_x(beta, mu, terms)
            self.update_terms_cache(terms, x)

            # stopping criteria
            psi_norm = self.compute_psi_norm(beta, mu, terms)
            if psi_norm < self.tol:
                break

            # alternating solve beta & mu
            beta, mu = self.solve_beta_mu(terms)

        se3 = se3_vec_to_mat(x)
        se3[:3, :3] = project(se3[:3, :3])

        return se3
