#include "pdaf.h"
namespace algorithmBase {
void pdaf::prediction() {
    statePrediction();
    errorCovPrediction();
    measurementPrediction();
}
float pdaf::validationGateVolume() {
    // returns the validation gate volume of Sk
    return (3.1415925 * std::sqrt(abs((18.4 * S).determinant())));
}

void pdaf::update(const std::vector<Eigen::Vector2f>& mess) {
    innovationPrediction(mess);
    innovationErrorCovPrediction();
    kalmanGain();
    equivalentInnovation(mess);
    stateUpdate();
    errorCovUpdate(mess);
}

void pdaf::statePrediction() {
    X = A * X;
}
void pdaf::errorCovPrediction() {
    P = A * P * A.transpose() + Q;
}

void pdaf::measurementPrediction() {
    Z = H * X;
}

void pdaf::innovationPrediction(const std::vector<Eigen::Vector2f>& mess) {
    V.clear();
    for (auto& m : mess) {
        V.emplace_back(m - Z);
    }
}

void pdaf::innovationErrorCovPrediction() {
    S = H * P * H.transpose() + R;
}

void pdaf::kalmanGain() {
    K = P * H.transpose() * S.inverse();
}
void pdaf::equivalentInnovation(const std::vector<Eigen::Vector2f>& mess) {
    Eigen::Vector2f sum(mess.back().rows(), mess.back().cols());

    for (size_t i = 0; i < mess.size(); i++) {
        if (Bi[i] == 0.0) {
            Bi[i] = calculateBi(mess, i);
        }
        sum += Bi[i] * V[i];
    }
    EV = sum;
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

    lambda = mess.size() / validationGateVolume();
    ViZi = V[index].transpose() * S.inverse() * V[index];

    eViZi = std::exp(-0.5 * ViZi);
    NZi = eViZi / (std::pow((2 * 3.1415926), (2 / 2)) * sqrt(abs(S.determinant())));
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
        sumLi += calculateLi(mess, i);
    }
    return (sumLi);
}
void pdaf::stateUpdate() {
    X = X + K * EV;
}

void pdaf::errorCovUpdate(const std::vector<Eigen::Vector2f>& mess) {
    float B0;
    Eigen::Matrix2f Pc;  // cov matrix of the stated update with the correct measurement
    Eigen::Matrix2f Ps;  // spread of innovations
    Eigen::Matrix2f sum;

    Eigen::Matrix2f Pk;  // return value

    B0 = calculateBi(mess, 0);
    Pc = P - K * S * K.transpose();

    for (size_t i = 0; i < mess.size(); i++) {
        if (Bi[i] == 0.0) {
            Bi[i] = calculateBi(mess, i);
        }
        sum += Bi[i] * V[i] * V[i].transpose();
    }
    Ps = K * (sum - EV * EV.transpose()) * K.transpose();

    P = B0 * P + (1 - B0) * Pc + Ps;
}

}  // namespace algorithmBase
