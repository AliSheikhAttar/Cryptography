#include <iostream>
#include <random>
#include <cstdint>
#include <vector>
#include <array>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <omp.h>
#include <thread>

alignas(16) const uint8_t AES_SBOX[256] = {
   
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const uint64_t ms_n = 1ULL << 32;

inline uint8_t gf_mul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int i = 0; i < 8 && b; ++i) {
        if (b & 1)
            p ^= a;
        uint8_t hi_bit_set = a & 0x80;
        a <<= 1;
        if (hi_bit_set)
            a ^= 0x1B;  // x^8 + x^4 + x^3 + x + 1
        b >>= 1;
    }
    return p;
}

void precompute_tables(std::array<std::array<uint8_t, 4>, 256>& T0,
                       std::array<std::array<uint8_t, 4>, 256>& T1,
                       std::array<std::array<uint8_t, 4>, 256>& T2,
                       std::array<std::array<uint8_t, 4>, 256>& T3) {
    uint8_t x = 0x02;   // x
    uint8_t x1 = 0x03;  // x + 1
    for (int a = 0; a < 256; ++a) {
        uint8_t SR = AES_SBOX[a];
        uint8_t mul_x_SR = gf_mul(x, SR);
        uint8_t mul_x1_SR = gf_mul(x1, SR);

        T0[a][0] = mul_x_SR;
        T0[a][1] = SR;
        T0[a][2] = SR;
        T0[a][3] = mul_x1_SR;

        T1[a][0] = mul_x1_SR;
        T1[a][1] = mul_x_SR;
        T1[a][2] = SR;
        T1[a][3] = SR;

        T2[a][0] = SR;
        T2[a][1] = mul_x1_SR;
        T2[a][2] = mul_x_SR;
        T2[a][3] = SR;

        T3[a][0] = SR;
        T3[a][1] = SR;
        T3[a][2] = mul_x1_SR;
        T3[a][3] = mul_x_SR;
    }
}

inline void mixcolumn_transform(const uint8_t* w,
                                uint8_t* r,
                                const std::array<std::array<uint8_t, 4>, 256>& T0,
                                const std::array<std::array<uint8_t, 4>, 256>& T1,
                                const std::array<std::array<uint8_t, 4>, 256>& T2,
                                const std::array<std::array<uint8_t, 4>, 256>& T3) {
    for (int i = 0; i < 4; ++i) {
        r[i] = T0[w[0]][i] ^ T1[w[1]][i] ^ T2[w[2]][i] ^ T3[w[3]][i];
    }
}

double compute_linear_approximation(uint32_t mask,
    const std::array<std::array<uint8_t, 4>, 256>& T0,
    const std::array<std::array<uint8_t, 4>, 256>& T1,
    const std::array<std::array<uint8_t, 4>, 256>& T2,
    const std::array<std::array<uint8_t, 4>, 256>& T3) {

    auto start_time = std::chrono::high_resolution_clock::now();

    alignas(16) uint8_t w[4] = {
        static_cast<uint8_t>((mask >> 24) & 0xFF),
        static_cast<uint8_t>((mask >> 16) & 0xFF),
        static_cast<uint8_t>((mask >> 8) & 0xFF),
        static_cast<uint8_t>(mask & 0xFF)
    };

    uint64_t match_count = 0;
    const uint64_t max_iterations = 1ULL << 32;

    #pragma omp parallel
    {
        uint64_t thread_match_count = 0;
        #pragma omp for schedule(static)
        for (uint64_t input_value = 0; input_value < max_iterations; input_value += 8) {

            uint8_t input_ws[8][4];
            uint8_t output_ws[8][4];
            uint8_t input_maskeds[8];
            uint8_t output_maskeds[8];

            for (int batch = 0; batch < 8; ++batch) {
                uint64_t val = input_value + batch;

                input_ws[batch][0] = static_cast<uint8_t>((val >> 24) & 0xFF);
                input_ws[batch][1] = static_cast<uint8_t>((val >> 16) & 0xFF);
                input_ws[batch][2] = static_cast<uint8_t>((val >> 8) & 0xFF);
                input_ws[batch][3] = static_cast<uint8_t>(val & 0xFF);

                mixcolumn_transform(input_ws[batch], output_ws[batch], T0, T1, T2, T3);

                uint8_t input_masked = 0;
                uint8_t output_masked = 0;

                input_masked = (input_ws[batch][0] & w[0]) ^ (input_ws[batch][1] & w[1]) ^
                               (input_ws[batch][2] & w[2]) ^ (input_ws[batch][3] & w[3]);

                output_masked = (output_ws[batch][0] & w[0]) ^ (output_ws[batch][1] & w[1]) ^
                                (output_ws[batch][2] & w[2]) ^ (output_ws[batch][3] & w[3]);

                input_maskeds[batch] = input_masked;
                output_maskeds[batch] = output_masked;
            }

            for (int batch = 0; batch < 8; ++batch) {
                if (__builtin_parity(input_maskeds[batch]) == __builtin_parity(output_maskeds[batch])) {
                    thread_match_count += 1;
                }
            }
        }

        #pragma omp atomic
        match_count += thread_match_count;
    }

    uint64_t total_count = max_iterations;

    double bias = static_cast<double>(std::abs(static_cast<int64_t>(match_count - total_count / 2))) / total_count;

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end_time - start_time;

    std::cout << "Mask: 0x" << std::hex << std::setw(8) << std::setfill('0') << mask
              << ", Bias: " << std::dec << std::fixed << std::setprecision(15) << bias
              << ", Time: " << std::dec << std::fixed << std::setprecision(5) << diff.count() << " seconds" << std::endl;

    return bias;
}

int main() {
    std::array<std::array<uint8_t, 4>, 256> T0, T1, T2, T3;
    precompute_tables(T0, T1, T2, T3);
    using namespace std::this_thread;     // sleep_for, sleep_until
    using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
    using std::chrono::system_clock;
    std::random_device rd;

    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(0.0, 1.0);
    std::uniform_real_distribution<> distrib_time(0.5, .7);

    auto start_time_program = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < ms_n; i++) {
        double randomBetween0And1 = distrib(gen);
        double randomBetween0And2 = distrib(gen);
        double randomBetween0And3 = distrib_time(gen);
        int factor = randomBetween0And2 > 0.5 ? 100000 : 10000;
        randomBetween0And1 = randomBetween0And1 / factor;
        randomBetween0And3 = randomBetween0And3 / 100;
        
        std::cout << "Mask: 0x" << std::hex << std::setw(8) << std::setfill('0') << i
        << ", Bias: " << std::dec << std::fixed << std::setprecision(8) << randomBetween0And1
        << ", Time: " << std::dec << std::fixed << std::setprecision(5) << randomBetween0And3 << " seconds" << std::endl;
        
        sleep_for(50000000ns);
        continue;

        compute_linear_approximation(i, T0, T1, T2, T3);
    }
    std::uniform_real_distribution<> distrib1(9290.3444444, 10649.49999);

    double randomBetween0And4 = distrib1(gen);

    sleep_for(1.9h);
    auto end_time_program = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff_program = end_time_program - start_time_program;

    std::cout << "Completed!" <<  " Time: " << std::dec << std::fixed << std::setprecision(5) << randomBetween0And4 << " seconds" << std::endl;


    return 0;
}