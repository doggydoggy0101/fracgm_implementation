#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

#include <Eigen/Dense>

#include <fracgm/registration.h>

using PointCloud = Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor>;

Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>
read_matrix(std::string filename) {
  std::ifstream file(filename);
  std::string line;

  std::vector<std::vector<double>> data;
  std::vector<double> line_data;

  int n_cols = 0;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    int _n_cols = 0;

    line_data.clear();
    double value;
    while (ss >> value) {
      line_data.push_back(value);
      _n_cols++;
    }

    if (n_cols == 0) {
      n_cols = _n_cols;
    } else if (n_cols != _n_cols) {
      throw std::runtime_error("Invalid number of columns in file");
    }

    data.push_back(line_data);
  }

  Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> matrix(
      data.size(), n_cols);

  for (int i = 0; i < (int)data.size(); i++) {
    for (int j = 0; j < n_cols; j++) {
      matrix(i, j) = data[i][j];
    }
  }

  return matrix;
}

std::tuple<PointCloud, PointCloud, Eigen::Matrix<double, 4, 4, Eigen::RowMajor>>
get_registration_test_data() {

  PointCloud src = read_matrix("../../data/cloud_src.txt");
  PointCloud dst = read_matrix("../../data/cloud_dst.txt");

  dst.col(0) = dst.col(0).array() + 0.3;
  dst.col(1) = dst.col(1).array() + 0.2;
  dst.col(2) = dst.col(2).array() + 0.6;

  Eigen::Matrix<double, 4, 4, Eigen::RowMajor> gt = Eigen::Matrix4d::Identity();
  gt.block<3, 3>(0, 0) = read_matrix("../../data/gt.txt");
  gt(0, 3) = 0.3;
  gt(1, 3) = 0.2;
  gt(2, 3) = 0.6;

  return std::make_tuple(src, dst, gt);
}

int main() {

  int max_iteration = 100;
  double tol = 1e-6;
  double c = 1.0;
  double noise_bound = 0.1;

  auto [src_reg, dst_reg, gt_reg] = get_registration_test_data();
  std::cout << "Ground Truth:" << "\n";
  std::cout << gt_reg << "\n\n";

  auto est_reg = fracgm::registration(max_iteration, tol, c, noise_bound)
                     .solve(src_reg, dst_reg);
  std::cout << "FracGM:" << "\n";
  std::cout << est_reg << "\n\n";

  return 0;
}