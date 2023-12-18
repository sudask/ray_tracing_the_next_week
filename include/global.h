#ifndef GLOBALH
#define GLOBALH
#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using std::pow;

float get_random_float() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    return dist(gen);
};

float schlick(float cosine, float ref_idx) {
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
};

#endif