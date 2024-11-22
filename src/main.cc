#include "pcsc.hh"
#include <iostream>
#include "sampling/uniform_sampler.hh"

int main(int argc, char **argv) {
    uniform sampler(20, -2,5);
    for (int i = 1; i < 30; i++) {
        std::cout << sampler.sample() << std::endl;
    }
}
