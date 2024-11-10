#pragma once

#include "fracgm/utils.h"

namespace fracgm {

using PointCloud = Eigen::Matrix<double, Eigen::Dynamic, 3>;

class registration {

public:
  int max_iteration_;
  double tol_;
  double c_;
  double noise_bound_;

  registration(const int &max_iteration, const double &tol, const double &c,
               const double &noise_bound);

  std::vector<double> solve_beta_mu(std::vector<Fractional> *terms);

  Eigen::VectorXd solve_x(std::vector<double> *alpha,
                          std::vector<Fractional> *terms);

  float compute_psi_norm(std::vector<double> *alpha,
                         std::vector<Fractional> *terms);

  void update_terms_cache(std::vector<Fractional> *terms, Eigen::VectorXd *vec);

  Eigen::Matrix4d solve(const PointCloud &pcd1, const PointCloud &pcd2);
};

}; // namespace fracgm