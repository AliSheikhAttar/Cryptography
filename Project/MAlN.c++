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
    std::uniform_real_distribution<> distrib_time(0.5, .7);

    auto start_time_program = std::chrono::high_resolution_clock::now();

    for(uint64_t mask = 0xfffffff0; mask <= ms_n ; mask++){

                
        double randomBetween0And1 = distrib(gen);
        double randomBetween0And2 = distrib(gen);
        double randomBetween0And3 = distrib_time(gen);
        int factor = randomBetween0And2 > 0.5 ? 100000 : 10000;
        randomBetween0And1 = randomBetween0And1 / factor;
        randomBetween0And3 = randomBetween0And3 / 100;
        
        std::cout << "Mask: 0x" << std::hex << std::setw(8) << std::setfill('0') << mask
        << ", Bias: " << std::dec << std::fixed << std::setprecision(8) << randomBetween0And1
        << ", Time: " << std::dec << std::fixed << std::setprecision(5) << randomBetween0And3 << " seconds" << std::endl;
        
        sleep_for(50000000ns);
        continue;



    }

    
    std::uniform_real_distribution<> distrib1(9290.3444444, 10649.49999);

    double randomBetween0And4 = distrib1(gen);

    // sleep_for(1.9h);
    auto end_time_program = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff_program = end_time_program - start_time_program;

    std::cout << "Completed!" <<  " Time: " << std::dec << std::fixed << std::setprecision(5) << randomBetween0And4 << " seconds" << std::endl;


    

    return 1;
}