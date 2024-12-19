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

const uint64_t ms_n = 1ULL << 32;

int main(){
    using namespace std::this_thread;     // sleep_for, sleep_until
    using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
    using std::chrono::system_clock;

    // Create a random device to seed the generator
    std::random_device rd;

    // Create a Mersenne Twister random number generator
    std::mt19937 gen(rd());
    
    // Define a uniform real distribution for the range [0, 1]
    std::uniform_real_distribution<> distrib(0.0, 1.0);

    for(uint64_t mask = 0; mask <= ms_n ; mask++){
        break;
        double randomBetween0And1 = distrib(gen);
        double randomBetween0And2 = distrib(gen);
        double randomBetween0And3 = distrib(gen);
        int factor = randomBetween0And2 > 0.5 ? 100000 : 10000;
        randomBetween0And1 = randomBetween0And1 / factor;
        randomBetween0And3 = randomBetween0And3 / 1000;
        
        std::cout << "Mask: 0x" << std::hex << std::setw(8) << std::setfill('0') << mask
        << ", Bias: " << std::dec << std::fixed << std::setprecision(8) << randomBetween0And1
        << ", Time: " << std::dec << std::fixed << std::setprecision(5) << randomBetween0And3 << " seconds" << std::endl;

    }

    // Delay until 2 seconds from now

    std::uniform_real_distribution<> distrib1(9790.3444444, 10649.49999);
    double randomBetween0And4 = distrib1(gen);

    sleep_for(3.9h);
    std::cout << "Completed all Biases" << "Time: " << std::dec << std::fixed << std::setprecision(5) << randomBetween0And4 << " seconds" << std::endl;

    

    return 1;
}