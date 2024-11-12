#include <Eigen/Dense>
#include <cmath>
#include <vector>

#include "fracgm/constant.h"
#include "fracgm/registration.h"
#include "fracgm/utils.h"

namespace fracgm {

registration::registration(const int &max_iteration, const double &tol,
                           const double &c, const double &noise_bound) {

  max_iteration_ = max_iteration;
  tol_ = tol;
  c_ = c;
  noise_bound_ = noise_bound;
}

std::vector<double>
registration::solve_beta_mu(std::vector<Fractional> *terms) {
  std::vector<double> vec_1; // beta
  std::vector<double> vec_2; // mu

  for (auto &term : *terms) {
    vec_1.push_back(term.f() / term.h());
    vec_2.push_back(1 / term.h());
  }

  vec_1.insert(vec_1.end(), vec_2.begin(), vec_2.end());
  return vec_1;
}

Eigen::VectorXd registration::solve_x(std::vector<double> *alpha,
                                      std::vector<Fractional> *terms) {

  Eigen::MatrixXd mat_a = Eigen::MatrixXd::Zero(13, 13);
  int n = terms->size();

  for (int i = 0; i < n; i++) {
    mat_a += alpha->at(n + i) * terms->at(i).f_mat() -
             alpha->at(n + i) * alpha->at(i) * terms->at(i).h_mat();
  }

  Eigen::Vector<double, 13> temp = mat_a.ldlt().solve(e);
  return (1 / (e.transpose() * temp)) * temp;
}

float registration::compute_psi_norm(std::vector<double> *alpha,
                                     std::vector<Fractional> *terms) {
  float a;
  float b;
  int n = terms->size();

  float loss = 0.0;
  for (int i = 0; i < n; i++) {
    a = -terms->at(i).f() + alpha->at(i) * terms->at(i).h();
    b = -1.0 + alpha->at(n + i) * terms->at(i).h();
    loss += a * a + b * b;
  }
  return sqrt(loss);
}

void registration::update_terms_cache(std::vector<Fractional> *terms,
                                      Eigen::VectorXd *vec) {
  for (auto &term : *terms) {
    term.update_cache(*vec);
  }
}

Eigen::Matrix4d registration::solve(const PointCloud &pcd1,
                                    const PointCloud &pcd2) {

  Eigen::VectorXd x;
  Eigen::Matrix4d se3;
  std::vector<double> alpha;
  float psi_norm;

  std::vector<Fractional> terms =
      compute_terms(pcd1, pcd2, noise_bound_ * noise_bound_, c_ * c_);

  Eigen::Matrix4d init_mat = compute_initial_guess(pcd1, pcd2);
  x = se3_mat_to_vec(init_mat);

  for (int i = 0; i < max_iteration_; i++) {
    // alternating solve alpha
    alpha = solve_beta_mu(&terms);
    // alternating solve x
    x = solve_x(&alpha, &terms);
    update_terms_cache(&terms, &x);
    // stopping criteria
    psi_norm = compute_psi_norm(&alpha, &terms);
    if (psi_norm < tol_) {
      break;
    }
  }

  se3 = se3_vec_to_mat(x);
  se3.block<3, 3>(0, 0) = project(se3.block<3, 3>(0, 0));
  return se3;
}

} // namespace fracgm