#pragma once

#include <Eigen/Dense>

namespace fracgm {

using PointCloud = Eigen::Matrix<double, Eigen::Dynamic, 3>;

// A structure that can be used to compute the quadratic form associated
// with a matrix, and keep track of the most recently computed value.
struct R2Sym {

public:
  Eigen::MatrixXd mat;
  double cache;

  R2Sym(); // default
  R2Sym(Eigen::MatrixXd);
  R2Sym(Eigen::MatrixXd, double);

  // Compute the quadratic form associated with self and x.
  double call(Eigen::VectorXd);

  // Update the cached value of the quadratic form associated with self.
  void update_cache(Eigen::VectorXd);
};

// A structure to represent a fractional term $f(x)/h(x)$ in the
// Geman-McClure-based objective function.
struct Fractional {

public:
  R2Sym r2;
  double c2;

  Fractional(R2Sym, double);

  // Updates the cache of the square of residual.
  void update_cache(Eigen::VectorXd);

  // Computes the numerator $f(x)$.
  double f();
  // Computes the denominator $h(x)$.
  double h();
  // Get the matrix associated with the numerator.
  Eigen::MatrixXd f_mat();
  // Get the matrix associated with the denominator.
  Eigen::MatrixXd h_mat();
};

std::pair<Eigen::MatrixXd, Eigen::Vector3d>
get_zero_mean_point_cloud(const Eigen::MatrixXd);

Eigen::Matrix3d project(const Eigen::Matrix3d);

Eigen::Matrix4d compute_initial_guess(PointCloud, PointCloud);

std::vector<Fractional> compute_terms(PointCloud, PointCloud, double, double);

Eigen::VectorXd se3_mat_to_vec(Eigen::Matrix4d);

Eigen::Matrix4d se3_vec_to_mat(Eigen::VectorXd);

}; // namespace fracgm