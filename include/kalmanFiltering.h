#include <Eigen/Dense>

class KalmanFilter {
 public:
  KalmanFilter(const Eigen::VectorXf& _x, const Eigen::MatrixXf& _P, const Eigen::MatrixXf& _Q,
               const Eigen::MatrixXf& _R, const Eigen::MatrixXf& _H, const Eigen::MatrixXf& _A)
      : x(_x), P(_P), Q(_Q), R(_R), H(_H),A(_A) {}
  KalmanFilter();

  void predict(double dt);
  void update(Eigen::VectorXf measurement);

  Eigen::VectorXf getState() const;

 public:
  Eigen::VectorXf x;      // 状态估计值
  Eigen::MatrixXf P;      // 状态估计误差的协方差
  Eigen::MatrixXf Q;      // 过程噪声的方差
  Eigen::MatrixXf R;      // 测量噪声的方差
  Eigen::MatrixXf x_hat;  // 预测的状态估计值
  Eigen::MatrixXf P_hat;  // 预测的状态估计误差的协方差
  Eigen::MatrixXf K;      // 卡尔曼增益
  Eigen::MatrixXf H;      // 观测矩阵

  // 状态转移矩阵
  Eigen::MatrixXf A;
};
