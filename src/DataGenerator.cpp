#include "DataGenerator.h"
namespace algorithmBase {

double generateNormalDistributionData(double mean, double stdDev) {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<double> d{mean, stdDev};
    return d(gen);
}

std::vector<double> generateNormalDistributionData(double mean, double stdDev, size_t size) {
    std::vector<double> result(size);
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<double> d{mean, stdDev};
    std::generate(result.begin(), result.end(), [&]() {
        return d(gen);
    });
    return result;
}
}  // namespace algorithmBase
