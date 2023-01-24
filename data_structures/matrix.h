#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <numeric>

namespace jr {

template <std::size_t N, std::size_t M, typename T>
class Matrix {
 protected:
  std::array<std::array<T, M>, N> data_;

 public:
  constexpr Matrix() noexcept : data_({}){};

  constexpr explicit Matrix(std::array<std::array<T, M>, N> const& arr) noexcept : data_(arr) {}

  constexpr explicit Matrix(std::array<T, N * M> const& arr) noexcept {
    for (auto y = 0; y < N; y++) {
      for (auto x = 0; x < M; x++) {
        data_[y][x] = arr[y * M + x];
      }
    }
  }

  template <std::size_t X>
  constexpr auto operator*(Matrix<M, X, T> const& rhs) const noexcept -> Matrix<N, X, T> {
    Matrix<N, X, T> res;
    multiply(*this, rhs, res);
    return res;
  }

  constexpr auto operator*(const T scalar) const noexcept -> Matrix {
    auto res = *this;
    multiply(res, scalar);
    return res;
  }

  constexpr auto operator*=(Matrix<M, M, T> const& rhs) noexcept -> Matrix {
    multiply(*this, rhs, *this);
    return *this;
  }

  constexpr auto operator*=(const T scalar) noexcept -> Matrix& {
    multiply(*this, scalar);
    return *this;
  }

  constexpr auto operator+(Matrix const& rhs) const noexcept -> Matrix {
    auto res = *this;
    add(*this, rhs, res);
    return res;
  }

  constexpr auto operator+=(Matrix const& rhs) noexcept -> Matrix& {
    add(*this, rhs, *this);
    return *this;
  }

  constexpr auto operator-(Matrix const& rhs) const noexcept -> Matrix {
    auto res = *this;
    subtract(*this, rhs, res);
    return res;
  }

  constexpr auto operator-=(Matrix const& rhs) noexcept -> Matrix& {
    subtract(*this, rhs, *this);
    return *this;
  }

  constexpr auto operator[](const std::size_t y) const noexcept -> std::array<T, M> const& { return data_[y]; }

  constexpr auto operator[](const std::size_t y) noexcept -> std::array<T, M>& { return data_[y]; }

  constexpr auto operator[](const std::pair<std::size_t, std::size_t> cell) const noexcept -> T const& {
    return data_[cell.first][cell.second];
  }

  constexpr auto operator[](const std::pair<std::size_t, std::size_t> cell) noexcept -> T& {
    return data_[cell.first][cell.second];
  }

  constexpr auto operator==(Matrix const& m) const noexcept -> bool {
    for (auto y = 0; y < N; y++) {
      for (auto x = 0; x < M; x++) {
        if (data_[y][x] != m[y][x]) return false;
      }
    }
    return true;
  }

  /**
   * @brief takes O(N^2*M) time and O(N^2*M) memory
   *
   */
  constexpr auto determinant() const noexcept
    requires(N == M && N > 3)
  {
    /**
     * @brief copies matrix with erased row and column
     *
     */
    auto make_matrix = [](const auto y_erased, const auto x_erased, Matrix const& m) -> Matrix<N - 1, M - 1, T> {
      Matrix<N - 1, M - 1, T> res;
      for (auto y_dest = 0, y_from = 0; y_dest < N - 1; y_dest++, y_from++) {
        if (y_from == y_erased) y_from++;
        for (auto x_dest = 0, x_from = 0; x_dest < N - 1 && y_from < N; x_dest++, x_from++) {
          if (x_from == x_erased) x_from++;
          res[y_dest][x_dest] = m[y_from][x_from];
        }
      }
      return res;
    };

    auto det = T();
    const auto x = static_cast<std::size_t>(0);  // fixed column for row reducing. Why not
    for (auto y = 0; y < N; y++) {
      det += data_[y][x] * std::pow(-1, y + x) * make_matrix(y, x, *this).determinant();
    }
    return det;
  }

  /**
   * @brief takes O(3*3) time and O(1) memory
   *
   */
  constexpr auto determinant() const noexcept
    requires(N == M && N == 3)
  {
    auto det = T();
    for (auto x = 0; x < N; x++) {
      auto tmp1 = T(1);
      auto tmp2 = T(1);
      for (auto y = 0; y < N; y++) {
        tmp1 *= data_[y][(x + y) % M];
        tmp2 *= data_[y][(N - 1 + x - y) % M];
      }
      det += tmp1 - tmp2;
    }
    return det;
  }

  /**
   * @brief takes O(2*2) time and O(1) memory
   *
   */
  constexpr auto determinant() const noexcept
    requires(N == M && N == 2)
  {
    return data_[0][0] * data_[1][1] - data_[0][1] * data_[1][0];
  }
  constexpr auto determinant() const noexcept
    requires(N == M && N == 1)
  {
    return data_[0][0];
  }
  constexpr auto gaussElimination() noexcept -> Matrix<N, M, T>& {
    auto& mat = *this;

    /**
     * @brief sorts the rows in non ascending order.
     * takes O(N*M) additional memory and O(N*M*log(N)) time complexity
     */
    auto sort_rows = [](Matrix& mat) noexcept -> void {
      using row_type = std::array<std::pair<std::bitset<M>, int>, N>;
      row_type bits;
      for (auto i = 0; i < N; i++) {
        for (auto j = 0; j < M; j++) bits[i].first[j] = mat[i][j];
        bits[i].second = i;
      }
      std::ranges::sort(
          bits, [](auto const& r1, auto const& r2) { return r1.first.to_ullong() > r2.first.to_ullong(); });

      auto res = Matrix();
      for (auto y = 0; y < N; y++) res[y] = mat[bits[y].second];
      mat = res;
    };

    sort_rows(mat);
    rowEchelon(mat);
    // reduced_row_echelon(mat);
    // std::cout<<mat<<'\n';

    return *this;
  }
  constexpr auto elementWiseXor(Matrix const& m) noexcept -> Matrix& {
    for (auto x = 0; x < M; ++x) {
      for (auto y = 0; y < N; ++y) {
        data_[y][x] ^= m[y][x];
      }
    }
    return *this;
  }

  constexpr auto transpose() const noexcept -> Matrix<M, N, T> {
    Matrix<M, N, T> res;
    for (auto y = 0; y < N; ++y) {
      for (auto x = 0; x < M; ++x) {
        res[x][y] = data_[y][x];
      }
    }
    return res;
  }

  constexpr auto inverse() noexcept -> Matrix<N, M, T>& {}

 private:
  /**
   * @param mat matrix with sorted rows in non ascending order.
   */
  constexpr static inline auto rowEchelon(Matrix& mat) noexcept -> void {}

  /**
   * @param mat matrix with sorted rows in non ascending order.
   */
  constexpr static inline auto reducedRowEchelon(Matrix& mat) noexcept -> void {}

  constexpr static inline auto addRows(std::array<T, M>& lhs, std::array<T, M> const& rhs) noexcept -> void {
    for (auto i = 0; i < M; i++) lhs[i] += rhs[i];
  }

  constexpr static inline auto multiplyRow(std::array<T, M>& lhs, const T rhs) noexcept -> void {
    for (auto i = 0; i < M; i++) lhs[i] *= rhs;
  }

  template <std::size_t X>
  constexpr static inline auto multiply(Matrix const& lhs, Matrix<M, X, T> const& rhs, Matrix<N, X, T>& res) noexcept
      -> void {  // TODO (jrazek) implement Strassens
    for (auto y = 0; y < N; y++) {
      for (auto x = 0; x < X; x++) {
        auto sum = T();
        for (auto i = 0; i < M; i++) sum += lhs[y][i] * rhs[i][x];
        res[y][x] = sum;
      }
    }
  }
  constexpr static inline auto multiply(Matrix& res, const T scalar) noexcept -> void {
    for (auto y = 0; y < N; y++)
      for (auto x = 0; x < M; x++) res[y][x] *= scalar;
  }
  constexpr static inline auto add(Matrix const& lhs, Matrix const& rhs, Matrix& res) noexcept -> void {
    for (auto y = 0; y < N; y++)
      for (auto x = 0; x < M; x++) res[y][x] = lhs[y][x] + rhs[y][x];
  }
  constexpr static inline auto subtract(Matrix const& lhs, Matrix const& rhs, Matrix& res) noexcept -> void {
    for (auto y = 0; y < N; y++)
      for (auto x = 0; x < M; x++) res[y][x] = lhs[y][x] - rhs[y][x];
  }

  template <bool Constant>
  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using differece_type = std::ptrdiff_t;
    using value_type = std::array<T, M>;
    using pointer = std::conditional_t<Constant, const value_type*, value_type*>;
    using reference = std::conditional_t<Constant, value_type const&, value_type&>;

    explicit Iterator(pointer ptr) : ptr_(ptr) {}

    auto operator*() -> reference { return *ptr_; };

    auto operator->() -> pointer { return ptr_; };
    auto operator++() -> Iterator& {
      ++ptr_;
      return *this;
    };  // preincrement ++it
    auto operator++(int) -> Iterator {
      auto tmp = *this;
      this->operator++;
      return tmp;
    };  // postincrement it++

    friend auto operator==(Iterator const& rhs, Iterator const& lhs) { return rhs.ptr_ == lhs.ptr_; };

    friend auto operator!=(Iterator const& rhs, Iterator const& lhs) { return rhs.ptr_ != lhs.ptr_; };

   private:
    pointer ptr_;
  };
  using iterator = Iterator<false>;
  using const_iterator = Iterator<true>;

 public:
  constexpr auto begin() noexcept -> iterator { return iterator(&data_.front()); }

  constexpr auto end() noexcept -> iterator { return iterator(&data_.back()); }

  constexpr auto cbegin() const noexcept -> const_iterator { return const_iterator(&data_.front()); }

  constexpr auto cend() const noexcept -> const_iterator { return const_iterator(&data_.back()); }
};

template <std::size_t N, std::size_t M, typename T>
auto operator<<(std::ostream& os, Matrix<N, M, T> const& m) -> std::ostream& {
  os << "{\n";
  for (auto y = 0; y < N; y++) {
    os << "\t{";
    for (auto x = 0; x < M; x++) {
      os << m[y][x];
      if (x + 1 != M) os << ", ";
    }
    os << "},\n";
  }
  os << '}';
  return os;
}

}  // namespace jr
