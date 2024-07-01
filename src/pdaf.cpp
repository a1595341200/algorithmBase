#include "pdaf.h"

#include "Log.h"
namespace algorithmBase {

pdaf::pdaf() {
    X << 1, 1;
    A = Eigen::Matrix2f::Identity();
    Q << 0.1f, 0, 0, 0.1f;
    R << 0.1f, 0, 0, 0.1f;
    K = Eigen::Matrix2f::Zero();
    P << 0.01f, 0, 0, 0.01f;
    H = Eigen::Matrix2f::Identity();
}

void pdaf::prediction(float dt) {
    statePrediction(dt);
    errorCovPrediction();
    measurementPrediction();
}
float pdaf::validationGateVolume() {
    // returns the validation gate volume of Sk
    return (3.1415925f * std::sqrt(std::abs((18.4f * S).determinant())));
}

void pdaf::update(const std::vector<Eigen::Vector2f>& mess) {
    if (mess.empty()) {
        return;
    }
    Bi.clear();
    Bi.resize(mess.size());
    innovationPrediction(mess);
    innovationErrorCovPrediction();
    kalmanGain();
    equivalentInnovation(mess);
    stateUpdate();
    errorCovUpdate(mess);
}

void pdaf::statePrediction(float dt) {
    (void)dt;
    X = A * X;
    // log(std::cout, "X \n", X);
}
void pdaf::errorCovPrediction() {
    P = A * P * A.transpose() + Q;
    // log(std::cout, "P \n", P);
}

void pdaf::measurementPrediction() {
    Z = H * X;
}

void pdaf::innovationPrediction(const std::vector<Eigen::Vector2f>& mess) {
    V.clear();
    V.resize(mess.size() + 1);
    for (size_t i = 0; i < mess.size(); i++) {
        V[i + 1] = (mess[i] - Z);
    }
}

void pdaf::innovationErrorCovPrediction() {
    S = H * P * H.transpose() + R;
}

void pdaf::kalmanGain() {
    K = P * H.transpose() * S.inverse();
    // log(std::cout, "K \n", K);
}
void pdaf::equivalentInnovation(const std::vector<Eigen::Vector2f>& mess) {
    Eigen::Vector2f sum(0.0f, 0.0f);
    Bi[0] = static_cast<float>(calculateBi(mess, 0));
    LOG_INFO("start");
    for (size_t i = 0; i < mess.size(); i++) {
        Bi[i + 1] = static_cast<float>(calculateBi(mess, i + 1));
        sum += Bi[i + 1] * V[i + 1];
        LOG_INFO("Bi[", i + 1, "]=", Bi[i + 1] * V[i + 1]);
    }
    LOG_INFO("stop");
    EV = sum;
    // log(std::cout, "EV \n", EV);
}

double pdaf::calculateBi(const std::vector<Eigen::Vector2f>& mess, size_t index) {
    double SUMLi;
    double retBi;
    double L;

    SUMLi = sumLi(mess);

    if (index == 0) {  // required to update P(k|k)
        retBi = (1 - Pd * Pg) / (1 - Pd * Pg + SUMLi);
    } else {  // required to calc. the combined innovation
        L = calculateLi(mess, index);
        retBi = L / (1 - Pd * Pg + SUMLi);

        // cout << "L:" << L << endl;
    }

    // cout << "retBi:" << retBi << endl;

    return (retBi);
    // needed to transpote SUM (Li)? no in n=2 because gaussian parametrization are 1x1
}

double pdaf::calculateLi(const std::vector<Eigen::Vector2f>& mess, size_t index) {
    float lambda;
    double ViZi;
    double eViZi;
    double NZi;
    double ret;

    lambda = static_cast<float>(mess.size()) / validationGateVolume();
    ViZi = V[index].transpose() * S.inverse() * V[index];

    eViZi = std::exp(-0.5 * ViZi);
    NZi = eViZi / (std::pow((2 * 3.1415926), (2 / 2)) * sqrt(std::abs(S.determinant())));
    ret = NZi * Pd / lambda;
    /*
            cout << "Zk=" << Zk.Z[i] << " pZk=" << pZk << endl;
            cout << "pZkZi=" << pZkZi << " lambda=" << lambda << endl;
            cout << "Sk=" << Sk << endl;
            cout << "ViZi=" << ViZi << "eViZi=" << eViZi << endl;
            cout << "NZi=" << NZi << endl;
            cout << "Li: " << ret << endl;

            if (abs(ViZi.at<float>(0, 0)) > C2) {
                print_validation_gate(Zk);
                cout << "ViZi fuera de VG" << endl;
            }
     */
    if (ret > 1.0) {
        ret = 1.0;
    }
    return ret;
}
double pdaf::sumLi(const std::vector<Eigen::Vector2f>& mess) {
    double sumLi{};

    for (size_t i = 0; i < mess.size(); i++) {
        sumLi += calculateLi(mess, i + 1);
    }
    return sumLi;
}
void pdaf::stateUpdate() {
    X = X + K * EV;
}

void pdaf::errorCovUpdate(const std::vector<Eigen::Vector2f>& mess) {
    float B0;
    Eigen::Matrix2f sum = Eigen::Matrix2f::Zero();
    B0 = static_cast<float>(calculateBi(mess, 0));
    Pc = P - K * S * K.transpose();
    LOG_INFO("begin Ps calculation");
    for (size_t i = 0; i < mess.size(); i++) {
        Bi[i + 1] = static_cast<float>(calculateBi(mess, i + 1));
        LOG_INFO("Bi[", i + 1, "]=", Bi[i + 1]);

        sum += Bi[i + 1] * V[i + 1] * V[i + 1].transpose();
    }
    LOG_INFO("end");

    Ps = K * (sum - EV * EV.transpose()) * K.transpose();

    P = B0 * P + (1 - B0) * Pc + Ps;
}

}  // namespace algorithmBase
