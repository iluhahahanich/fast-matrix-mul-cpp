#include <iostream>
#include <fstream>
#include <gtest/gtest.h>

#include "mul.h"
#include "utils.h"


TEST(Multiply, 1) {
    Matrix a = {{1, 2},
                {2, 3},
                {1, 3}};

    Matrix b = {{2, 2},
                {3, 1}};

    Matrix mul = {{8,  4},
                  {13, 7},
                  {11, 5}};

    Matrix<int> res(a.Rows(), b.Cols());
    SeqMultiply(a, b, res, 0);
    ASSERT_EQ(res, mul);

    res = Matrix<int>(a.Rows(), b.Cols());
    BlockMultiply(a, b, res, 2);
    ASSERT_EQ(res, mul);
}

TEST(Multiply, 2) {
    Matrix a = {{1, 5, 3},
                {2, 8, -4}};

    Matrix b = {{21, 4},
                {-8, -1},
                {1,  3}};

    Matrix mul = {{-16,  8},
                  {-26, -12}};

    Matrix<int> res(a.Rows(), b.Cols());
    SeqMultiply(a, b, res, 0);
    ASSERT_EQ(res, mul);

    res = Matrix<int>(a.Rows(), b.Cols());
    BlockMultiply(a, b, res, 2);
    ASSERT_EQ(res, mul);
}


TEST(Parallel, All) {
    std::ofstream out("../log.txt", std::ios_base::app);

    Gen<int> gen(-500, 500);

    for (int N : {500, 1024, 1500}) {
        const auto blocks = {1, 5, 10, 16, 20, 25, 50, 100, 150};
        out << N << ' ' << blocks.size() << '\n';

        auto a = gen.GenMatrix(N, N);
        auto b = gen.GenMatrix(N, N);
        Matrix<int> mul_seq(N, N), mul_seq1(N, N), mul_seq2(N, N);
        {
            TIME_SCOPE("seq: ", out);
            SeqMultiply(a, b, mul_seq, 0);
        }
        {
            TIME_SCOPE("seq1: ", out);
            SeqMultiply(a, b, mul_seq1, 1);
        }
        {
            TIME_SCOPE("seq2: ", out);
            SeqMultiply(a, b, mul_seq2, 2);
        }

        for (int BLOCK: blocks) {
            out << BLOCK << '\n';

            Matrix<int> mul_bl(N, N), mul_bl1(N, N), mul_bl2(N, N);
            {
                TIME_SCOPE("block: ", out);
                BlockMultiply(a, b, mul_bl, BLOCK, 0);
            }
            {
                TIME_SCOPE("block1: ", out);
                BlockMultiply(a, b, mul_bl1, BLOCK, 1);
            }
            {
                TIME_SCOPE("block2: ", out);
                BlockMultiply(a, b, mul_bl2, BLOCK, 2);
            }

            ASSERT_EQ(mul_seq, mul_seq1);
            ASSERT_EQ(mul_seq, mul_seq2);
            ASSERT_EQ(mul_seq, mul_bl);
            ASSERT_EQ(mul_bl, mul_bl1);
            ASSERT_EQ(mul_bl, mul_bl2);
        }
    }
}

