#pragma once

template<typename T>
class Matrix {
public:
    Matrix(int rows, int cols)
            : rows_(rows), cols_(cols), data_(rows * cols) {
    }

    Matrix() = default;

    Matrix(std::initializer_list<std::initializer_list<T>>&& l) {
        rows_ = l.size();
        if (rows_ == 0) {
            cols_ = 0;
            return;
        }
        cols_ = (*l.begin()).size();
        if (cols_ == 0) {
            throw std::runtime_error("wrong matrix dimensions");
        }
        for (auto row : l) {
            if (row.size() != cols_) {
                throw std::runtime_error("wrong matrix dimensions");
            }
            data_.insert(data_.end(), row.begin(), row.end());
        }
    }

    int Rows() const {
        return rows_;
    }

    int Cols() const {
        return cols_;
    }

    Matrix BlockMultiply(const Matrix& other, int r) const {
        Matrix res(rows_, other.cols_);

        return {};
    }

    T& operator()(int i, int j) {
        return data_[i * cols_ + j];
    }

    T operator()(int i, int j) const {
        return data_[i * cols_ + j];
    }

    friend std::ostream& operator<<(std::ostream& out, const Matrix& m) {
        for (int i = 0; i < m.rows_; ++i) {
            for (int j = 0; j < m.cols_; ++j) {
                out << m(i, j) << '\t';
            }
            out << '\n';
        }
        return out;
    }

    bool operator==(const Matrix& other) const {
        if (rows_ != other.rows_ or cols_ != other.cols_) {
            return false;
        }
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j) {
                if ((*this)(i, j) != other(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }


private:
    int rows_, cols_;
    std::vector<T> data_;
};

template<typename T>
Matrix<T> SeqMultiply(const Matrix<T>& a, const Matrix<T>& b, Matrix<T>& res,
                      int type = 0, int threads_num = 6) {
    const int n1 = a.Rows(), n2 = a.Cols(), n3 = b.Cols();

#pragma omp parallel for if (type == 1) num_threads(threads_num)
    for (int i = 0; i < n1; ++i) {
#pragma omp parallel for if (type == 2) num_threads(threads_num)
        for (int j = 0; j < n3; ++j) {
            for (int k = 0; k < n2; ++k) {
                res(i, j) += a(i, k) * b(k, j);
            }
        }
    }
    return res;
}

template<typename T>
void BlockMultiply(
        const Matrix<T>& a, const Matrix<T>& b, Matrix<T>& res,
        int block_size, int type = 0, int threads_num = 6
) {
    auto n1 = a.Rows(), n2 = a.Cols(), n3 = b.Cols();
#pragma omp parallel for if (type == 1) num_threads(threads_num)
    for (int i1 = 0; i1 < n1; i1 += block_size) {
        int r1 = std::min(n1, block_size + i1);
#pragma omp parallel for if (type == 2) num_threads(threads_num)
        for (int j1 = 0; j1 < n3; j1 += block_size) {
            int r3 = std::min(n3, block_size + j1);
            for (int k1 = 0; k1 < n2; k1 += block_size) {
                int r2 = std::min(n2, block_size + k1);
                for (int i = i1; i < r1; i++) {
                    for (int j = j1; j < r3; j++) {
                        for (int k = k1; k < r2; k++) {
                            res(i, j) += a(i, k) * b(k, j);
                        }
                    }
                }
            }
        }
    }
}
