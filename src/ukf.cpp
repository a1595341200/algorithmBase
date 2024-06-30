#include "ukf.h"

float polarAngleCal(float x, float y) {
    float PI = 3.1415926;
    float temp_tangle = 0;
    if (x == 0 && y == 0) {
        temp_tangle = 0;
    } else if (y >= 0) {
        temp_tangle = (float)atan2(y, x);
    } else if (y <= 0) {
        temp_tangle = (float)atan2(y, x) + 2 * PI;
    }
    return temp_tangle;
}

/*float normalangle(float angle, float v){
        std::cout<<"noramel########### "<<angle<<std::endl;
        if(angle>2*M_PI || angle<0){
                float vx = v * std::cos(angle);
                float vy = v * std::sin(angle);
                angle = Polar_angle_cal(vx,vy);
        }
        std::cout<<"noramel###########END "<<angle<<std::endl;
        return angle;
}*/
float normalangle(float angle) {
    const auto times =
        std::round(std::fabs(angle / (2. * M_PI)));  // for the case when angle is very very large

    if (angle > M_PI) {
        angle -= times * 2.0 * M_PI;
    }

    if (angle < -M_PI) {
        angle += times * 2.0 * M_PI;
    }
    return angle;
}

UKF::UKF(std::string smodel, int state_n, int mea_n, Eigen::MatrixXd Q, Eigen::MatrixXd R,
         Eigen::MatrixXd P) {
    if (model_hash_.count(smodel))
        model_ = model_hash_[smodel];
    parameter.kfp.measurementDimension = mea_n;
    parameter.kfp.stateDimension = state_n;

    parameter.kfp.X = Eigen::VectorXd(parameter.kfp.stateDimension);
    parameter.kfp.X.fill(0.0);

    // parameter.kfp.P = Eigen::MatrixXd(parameter.kfp.stateDimension,
    // parameter.kfp.stateDimension);
    parameter.kfp.P = P;
    //噪声
    // parameter.kfp.Q = Eigen::MatrixXd(parameter.kfp.stateDimension,
    // parameter.kfp.stateDimension);
    parameter.kfp.Q = Q;

    //  parameter.kfp.R = Eigen::MatrixXd(parameter.kfp.measurementDimension,
    //  parameter.kfp.measurementDimension);
    parameter.kfp.R = R;

    parameter.S = Eigen::MatrixXd(mea_n, mea_n);
    parameter.S.fill(0.0);

    parameter.zminus = Eigen::VectorXd(mea_n);
    parameter.zminus.fill(0.0);
}

// state: position_x, position_y,velocity, yaw, yaw_rate
void UKF::initialization(Eigen::VectorXd& X, Eigen::MatrixXd& P, float time) {
    parameter.kfp.X = X;
    parameter.kfp.P = P;

    pretime = time;

    parameter.lamda = 3 - parameter.kfp.stateDimension;

    //权重
    parameter.wm = Eigen::VectorXd(2 * parameter.kfp.stateDimension + 1);
    parameter.wc = Eigen::VectorXd(2 * parameter.kfp.stateDimension + 1);
    parameter.wm(0) = (parameter.lamda / (parameter.lamda + parameter.kfp.stateDimension));
    parameter.wc(0) = (parameter.lamda / (parameter.lamda + parameter.kfp.stateDimension)) + 1 -
                      std::pow(parameter.alpha, 2) + parameter.beta;
    float w = 0.5 / (parameter.lamda + parameter.kfp.stateDimension);
    for (int i = 1; i < 2 * parameter.kfp.stateDimension + 1; ++i) {
        parameter.wm(i) = (w);
        parameter.wc(i) = (w);
    }
    is_initalized_ = true;
}

bool UKF::isinitalized() {
    return is_initalized_;
}

//生成sigma点
void UKF::makeSigmaPoints() {
    // cholskey分解，或得协方差矩阵的根
    Eigen::VectorXd X = parameter.kfp.X;
    Eigen::MatrixXd P = parameter.kfp.P;

    Eigen::MatrixXd L = P.llt().matrixL();
    parameter.sigmaPoints =
        Eigen::MatrixXd(parameter.kfp.stateDimension, 2 * parameter.kfp.stateDimension + 1);
    // std::cout<<"MakeSigmaPoints\n"<<L<<std::endl;

    parameter.sigmaPoints.col(0) = X;
    const float c = std::sqrt(parameter.lamda + parameter.kfp.stateDimension);

    for (int i = 0; i < parameter.kfp.stateDimension; ++i) {
        parameter.sigmaPoints.col(i + 1) = X + c * L.col(i);
        parameter.sigmaPoints.col(i + parameter.kfp.stateDimension + 1) = X - c * L.col(i);
    }

    // std::cout << "MakeSigmaPointsresult: " << '\n' << parameter.sigmaPoints << std::endl;
}

//
void UKF::prediction(float ntime) {
    // std::cout<<"Prediction"<<'\n'<<std::endl;
    float deltat = ntime - pretime;

    pretime = ntime;

    // Prediction sigam points
    parameter.sigmaPointsPre =
        Eigen::MatrixXd(parameter.kfp.stateDimension, 2 * parameter.kfp.stateDimension + 1);

    for (int i = 0; i < 2 * parameter.kfp.stateDimension + 1; ++i) {
        float px_k(0.0), py_k(0.0), velo_k(0.0), yaw_k(0.0), yawd_k(0.0), a_k(0.0);
        px_k = parameter.sigmaPoints(0, i);
        py_k = parameter.sigmaPoints(1, i);
        velo_k = parameter.sigmaPoints(2, i);
        yaw_k = parameter.sigmaPoints(3, i);
        if (parameter.kfp.stateDimension == 5)
            yawd_k = parameter.sigmaPoints(4, i);
        if (parameter.kfp.stateDimension == 6) {
            yawd_k = parameter.sigmaPoints(4, i);
            a_k = parameter.sigmaPoints(5, i);
        }

        if (model_ == 1) {  // CV
            float px_pre{}, py_pre{}, velo_pre{}, yaw_pre{}, yawd_pre{}, a_pre{};
            px_pre = px_pre + velo_k * deltat * cos(yaw_k);
            py_pre = py_k + velo_k * deltat * sin(yaw_k);

            velo_pre = velo_k;
            yaw_pre = yaw_k;

            parameter.sigmaPointsPre(0, i) = px_pre;
            parameter.sigmaPointsPre(1, i) = py_pre;
            parameter.sigmaPointsPre(2, i) = velo_pre;
            parameter.sigmaPointsPre(3, i) = yaw_pre;  // normalangle(yaw_pre);//,velo_pre);
            parameter.sigmaPointsPre(4, i) = yawd_k;
            parameter.sigmaPointsPre(5, i) = a_k;

        } else if (model_ == 2) {  // CTRV
            float px_pre{}, py_pre{}, velo_pre{}, yaw_pre{}, yawd_pre{}, a_pre{};
            if (fabs(yawd_k) > 0.001) {
                px_pre = px_pre +
                         velo_k / yawd_k * (std::sin(yaw_k + yawd_k * deltat) - std::sin(yaw_k));
                py_pre =
                    py_k + velo_k / yawd_k * (std::cos(yaw_k) - std::cos(yaw_k + yawd_k * deltat));
            } else {
                px_pre = px_pre + velo_k * deltat * cos(yaw_k);
                py_pre = py_k + velo_k * deltat * sin(yaw_k);
            }
            velo_pre = velo_k;
            yaw_pre = yaw_k + yawd_k * deltat;
            yawd_pre = yawd_k;

            parameter.sigmaPointsPre(0, i) = px_pre;
            parameter.sigmaPointsPre(1, i) = py_pre;
            parameter.sigmaPointsPre(2, i) = velo_pre;
            parameter.sigmaPointsPre(3, i) = yaw_pre;  // normalangle(yaw_pre);//,velo_pre);
            parameter.sigmaPointsPre(4, i) = yawd_pre;
            parameter.sigmaPointsPre(5, i) = a_k;

        } else if (model_ == 3) {  // CTRA
            float px_pre, py_pre, velo_pre, yaw_pre, yawd_pre, a_pre;
            if (fabs(yawd_k) > 0.001) {
                px_pre = px_k + ((velo_k * yawd_k + a_k * yawd_k * deltat) *
                                     std::sin(yaw_k + yawd_k * deltat) +
                                 a_k * std::cos(yaw_k + yawd_k * deltat) -
                                 velo_k * yawd_k * std::sin(yaw_k) - a_k * std::cos(yaw_k)) /
                                    (yawd_k * yawd_k);
                py_pre = py_k + ((-velo_k * yawd_k - a_k * yawd_k * deltat) *
                                     std::cos(yaw_k + yawd_k * deltat) +
                                 a_k * std::sin(yaw_k + yawd_k * deltat) +
                                 velo_k * yawd_k * std::cos(yaw_k) - a_k * std::sin(yaw_k)) /
                                    (yawd_k * yawd_k);
            } else {
                px_pre = px_k + (velo_k * deltat + 0.5 * a_k * deltat * deltat) * cos(yaw_k);
                py_pre = py_k + (velo_k * deltat + 0.5 * a_k * deltat * deltat) * sin(yaw_k);
            }

            velo_pre = velo_k + a_k * deltat;
            yaw_pre = yaw_k + yawd_k * deltat;
            yawd_pre = yawd_k;
            a_pre = a_k;

            parameter.sigmaPointsPre(0, i) = px_pre;
            parameter.sigmaPointsPre(1, i) = py_pre;
            parameter.sigmaPointsPre(2, i) = velo_pre;
            parameter.sigmaPointsPre(3, i) = yaw_pre;  // normalangle(yaw_pre);//,velo_pre);
            parameter.sigmaPointsPre(4, i) = yawd_pre;
            parameter.sigmaPointsPre(5, i) = a_pre;
            for (int k = 6; k < parameter.kfp.stateDimension; ++k) {
                parameter.sigmaPointsPre(k, i) = 0;
            }
        }
    }

    //计算预测均值
    parameter.kfp.X.fill(0.0);
    for (int i = 0; i < 2 * parameter.kfp.stateDimension + 1; ++i) {
        parameter.kfp.X += parameter.wm(i) * parameter.sigmaPointsPre.col(i);
    }

    // std::cout << "########## 计算预测均值parameter.kfp.X ##########" << '\n' << parameter.kfp.X
    // << std::endl;
    //计算预测协方差
    try {
        parameter.kfp.P.fill(0.0);
        for (int i = 0; i < 2 * parameter.kfp.stateDimension + 1; ++i) {
            Eigen::VectorXd xdiff = parameter.sigmaPointsPre.col(i) - parameter.kfp.X;
            xdiff(3) = normalangle(xdiff(3));  //,parameter.kfp.Xdiff(2));
            parameter.kfp.P += parameter.wm(i) * xdiff * xdiff.transpose();
        }

        parameter.kfp.P += parameter.kfp.Q;
        // std::cout << "计算预测协方差P_ " << '\n' << parameter.kfp.P << std::endl;
    } catch (std::bad_alloc& e) {
        std::cout << "erro " << e.what() << std::endl;
        return;
    }
}

void UKF::predictionZ(Eigen::VectorXd& X, Eigen::MatrixXd& P, float ntime) {
    parameter.kfp.X = X;
    parameter.kfp.P = P;

    makeSigmaPoints();

    prediction(ntime);

    parameter.zSigma =
        Eigen::MatrixXd(parameter.kfp.measurementDimension, 2 * parameter.kfp.stateDimension + 1);
    parameter.zSigma.fill(0.0);
    //预测的量测点
    for (int i = 0; i < 2 * parameter.kfp.stateDimension + 1; ++i) {
        for (int j = 0; j < parameter.kfp.measurementDimension; ++j) {
            parameter.zSigma(j, i) = parameter.sigmaPoints(j, i);
        }
    }
    parameter.zPre = Eigen::VectorXd(parameter.kfp.measurementDimension);
    parameter.zPre.fill(0.0);

    //量测均值
    for (int i = 0; i < 2 * parameter.kfp.stateDimension + 1; ++i) {
        parameter.zPre += parameter.wm(i) * parameter.zSigma.col(i);
    }

    //计算量测协方差
    parameter.S.fill(0.0);
    for (int i = 0; i < 2 * parameter.kfp.stateDimension + 1; ++i) {
        Eigen::VectorXd z_diff = parameter.zSigma.col(i) - parameter.zPre;
        parameter.S += (parameter.wc(i) * z_diff * z_diff.transpose());
    }

    parameter.S += parameter.kfp.R;
}

void UKF::update(Eigen::VectorXd& Z) {
    //最后更新
    Eigen::MatrixXd T =
        Eigen::MatrixXd(parameter.kfp.stateDimension, parameter.kfp.measurementDimension);
    T.fill(0.0);
    for (int i = 0; i < 2 * parameter.kfp.stateDimension + 1; ++i) {
        Eigen::VectorXd xdiff = parameter.sigmaPointsPre.col(i) - parameter.kfp.X;
        // parameter.kfp.Xdiff(3) =  normalangle(parameter.kfp.Xdiff(3));//,parameter.kfp.Xdiff(2));
        Eigen::VectorXd z_diff = parameter.zSigma.col(i) - parameter.zPre;
        T += parameter.wc(i) * xdiff * z_diff.transpose();
    }
    //卡曼增益

    Eigen::MatrixXd K =
        Eigen::MatrixXd(parameter.kfp.stateDimension, parameter.kfp.measurementDimension);
    K = T * parameter.S.inverse();

    parameter.zminus.fill(0.0);
    parameter.zminus = (Z - parameter.zPre);
    parameter.kfp.X(3) = normalangle(parameter.kfp.X(3));  //,parameter.kfp.X(2));
    parameter.kfp.X += K * parameter.zminus;
    parameter.kfp.P -= K * parameter.S * K.transpose();

}

Eigen::VectorXd UKF::getState() {
    return parameter.kfp.X;
}

Eigen::MatrixXd UKF::getCovariance() {
    return parameter.kfp.P;
}

Eigen::VectorXd UKF::getZminus() {
    return parameter.zminus;
}

Eigen::MatrixXd UKF::getS() {
    return parameter.S;
}

Eigen::VectorXd UKF::getPredictionZ() {
    return parameter.zPre;
}
