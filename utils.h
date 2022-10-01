#pragma once

#include <random>
#include <limits>
#include <chrono>

#include "mul.h"

template<typename T>
class Gen {
public:
    explicit Gen(T lower = std::numeric_limits<T>::min(),
        T upper = std::numeric_limits<T>::max()) {
        gen = std::mt19937(std::random_device{}());
        dist = std::uniform_int_distribution<T>{lower, upper};
    }

    Matrix<T> GenMatrix(int rows, int cols) {
        Matrix<T> m(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                m(i, j) = dist(gen);
            }
        }
        return m;
    }

    T Get() {
        return dist(gen);
    }

private:
    std::mt19937 gen;
    std::uniform_int_distribution<T> dist;
};



class Timer {
public:

    explicit Timer(std::string label = "", std::ostream& out = std::cout)
            : label_(std::move(label)), out_(out), start_(std::chrono::steady_clock::now()) {
    }

    ~Timer() {
        using namespace std::chrono;
        auto end = steady_clock::now();
        out_ << label_ << duration_cast<milliseconds>(end - start_).count() << "ms" << std::endl;
    }

private:
    std::string label_;
    std::ostream& out_;
    std::chrono::time_point<std::chrono::steady_clock> start_;
};


#define TIME_SCOPE(label, out) Timer __UNIQUE_TIMER_NAME__(label, out)
