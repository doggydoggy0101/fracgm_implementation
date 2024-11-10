# pragma once

#include <Eigen/Dense>

namespace fracgm {

const Eigen::Vector<double, 13> e = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0};

const Eigen::MatrixXd eye3 = Eigen::MatrixXd::Identity(3, 3);

const Eigen::MatrixXd eye13 = Eigen::MatrixXd::Identity(13, 13);

}; // namespace fracgm