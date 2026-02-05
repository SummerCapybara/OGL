#include <gmt/Vector2.hpp>
#include <cassert>
#include <iostream>
#include <cmath>
#include <chrono>

int main() {
    using namespace gmt;

    auto start = std::chrono::high_resolution_clock::now();

    // Scale factors to stress floating-point math but avoid inf/NaN
    constexpr float large = 1e6f;
    constexpr float small = 1e-3f;
    constexpr int iterations = 100000; // 100k loops for heavy testing

    Vector2 v(large, large);

    for (int i = 0; i < iterations; ++i) {
        // Arithmetic
        Vector2 add = v + Vector2(large, -large);
        Vector2 sub = v - Vector2(-large, large);
        Vector2 mul = v * Vector2(1.1f, 0.9f);
        Vector2 mulScalar = v * 1.0001f;

        // Safe division
        Vector2 div = v / Vector2(large, large);
        Vector2 divScalar = v / 1.0001f;

        // Compound assignments
        Vector2 temp = v;
        temp += Vector2(small, small);
        temp -= Vector2(small, small);
        temp *= Vector2(1.0001f, 0.9999f);
        temp /= Vector2(1.0001f, 0.9999f);

        // Magnitude and normalization
        float mag = temp.Magnitude();
        Vector2 unit = temp.Unit();
        temp.Normalize();

        // Dot product
        float dot = temp.Dot(unit);

        // Power operations with positive exponents only (anti-NaN)
        Vector2 powVec = temp.Pow(Vector2(1.0001f, 0.9999f));
        Vector2 powScalar = temp.Pow(1.0001f);

        // Assertions (basic checks to avoid NaN/inf)
        assert(!std::isnan(mag) && !std::isinf(mag));
        assert(!std::isnan(dot) && !std::isinf(dot));
        assert(!std::isnan(powVec.x) && !std::isnan(powVec.y));
        assert(!std::isnan(powScalar.x) && !std::isnan(powScalar.y));
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Heavy stress test passed!\n";
    std::cout << "Total execution time: " << duration.count() << " ms.\n";
}
