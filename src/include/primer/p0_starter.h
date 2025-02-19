//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// p0_starter.h
//
// Identification: src/include/primer/p0_starter.h
//
// Copyright (c) 2015-2020, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

#include "common/exception.h"

namespace bustub {

/**
 * The Matrix type defines a common
 * interface for matrix operations.
 */
template <typename T>
class Matrix {
 protected:
  /**
   * Construct a new Matrix instance.
   * @param rows The number of rows
   * @param cols The number of columns
   *
   */
  Matrix(int rows, int cols) {
    this->rows_ = rows;
    this->cols_ = cols;
    this->linear_ = (T *)malloc(rows*cols*sizeof(T));
  }

  /** The number of rows in the matrix */
  int rows_;
  /** The number of columns in the matrix */
  int cols_;

  /**
   * A flattened array containing the elements of the matrix.
   */
  T *linear_;

 public:
  /** @return The number of rows in the matrix */
  virtual auto GetRowCount() const -> int = 0;

  /** @return The number of columns in the matrix */
  virtual auto GetColumnCount() const -> int = 0;

  /**
   * Get the (i,j)th matrix element.
   *
   * Throw OUT_OF_RANGE if either index is out of range.
   *
   * @param i The row index
   * @param j The column index
   * @return The (i,j)th matrix element
   * @throws OUT_OF_RANGE if either index is out of range
   */
  virtual auto GetElement(int i, int j) const -> T = 0;

  /**
   * Set the (i,j)th matrix element.
   *
   * Throw OUT_OF_RANGE if either index is out of range.
   *
   * @param i The row index
   * @param j The column index
   * @param val The value to insert
   * @throws OUT_OF_RANGE if either index is out of range
   */
  virtual void SetElement(int i, int j, T val) = 0;

  /**
   * Fill the elements of the matrix from `source`.
   *
   * Throw OUT_OF_RANGE in the event that `source`
   * does not contain the required number of elements.
   *
   * @param source The source container
   * @throws OUT_OF_RANGE if `source` is incorrect size
   */
  virtual void FillFrom(const std::vector<T> &source) = 0;

  /**
   * Destroy a matrix instance.
   */
  virtual ~Matrix() {
    free(linear_);
  }
};

/**
 * The RowMatrix type is a concrete matrix implementation.
 * It implements the interface defined by the Matrix type.
 */
template <typename T>
class RowMatrix : public Matrix<T> {
 public:
  /**
   * Construct a new RowMatrix instance.
   * @param rows The number of rows
   * @param cols The number of columns
   */
  RowMatrix(int rows, int cols) : Matrix<T>(rows, cols) {
    this->data_ = (T **)malloc(this->rows_*sizeof(T*));
    for (int i = 0; i < this->rows_; i ++) {
      this->data_[i] = &this->linear_[i*this->cols_];
    }
  }

  /**
   * @return The number of rows in the matrix
   */
  auto GetRowCount() const -> int override {
    return this->rows_;
  }

  /**
   * @return The number of columns in the matrix
   */
  auto GetColumnCount() const -> int override {
    return this->cols_;
  }

  /**
   * Get the (i,j)th matrix element.
   *
   * Throw OUT_OF_RANGE if either index is out of range.
   *
   * @param i The row index
   * @param j The column index
   * @return The (i,j)th matrix element
   * @throws OUT_OF_RANGE if either index is out of range
   */
  auto GetElement(int i, int j) const -> T override {
    if (i >= this->rows_ || j >= this->cols_ || i < 0 || j < 0) {
      throw OutOfRangeException{"i or j out of range."};
    }
    return data_[i][j];
  }

  /**
   * Set the (i,j)th matrix element.
   *
   * Throw OUT_OF_RANGE if either index is out of range.
   *
   * @param i The row index
   * @param j The column index
   * @param val The value to insert
   * @throws OUT_OF_RANGE if either index is out of range
   */
  void SetElement(int i, int j, T val) override {
    if (i >= this->rows_ || j >= this->cols_ || i < 0 || j < 0) {
      throw OutOfRangeException{"i or j out of range."};
    }
    data_[i][j] = val;
  }

  /**
   * Fill the elements of the matrix from `source`.
   *
   * Throw OUT_OF_RANGE in the event that `source`
   * does not contain the required number of elements.
   *
   * @param source The source container
   * @throws OUT_OF_RANGE if `source` is incorrect size
   */
  void FillFrom(const std::vector<T> &source) override {
    int size = source.size();
    if (this->rows_ * this->cols_ != size) {
      throw OutOfRangeException{"i or j out of range."};
    }
    for (int i = 0; i < this->rows_; i ++) {
      for (int j = 0; j < this->cols_; j ++) {
        data_[i][j] = source[i * this->cols_ + j];
      }
    }
  }

  /**
   * Destroy a RowMatrix instance.
   */
  ~RowMatrix() override {
    free(this->data_);
  };

 private:
  /**
   * A 2D array containing the elements of the matrix in row-major format.
   *
   * TODO(P0):
   * - Allocate the array of row pointers in the constructor.
   * - Use these pointers to point to corresponding elements of the `linear` array.
   * - Don't forget to deallocate the array in the destructor.
   */
  T **data_;
};

/**
 * The RowMatrixOperations class defines operations
 * that may be performed on instances of `RowMatrix`.
 */
template <typename T>
class RowMatrixOperations {
 public:
  /**
   * Compute (`matrixA` + `matrixB`) and return the result.
   * Return `nullptr` if dimensions mismatch for input matrices.
   * @param matrixA Input matrix
   * @param matrixB Input matrix
   * @return The result of matrix addition
   */
  static auto Add(const RowMatrix<T> *matrixA, const RowMatrix<T> *matrixB) -> std::unique_ptr<RowMatrix<T>> {
    int rowCountA = matrixA->GetRowCount();
    int columnCountA = matrixA->GetColumnCount();
    int rowCountB = matrixB->GetRowCount();
    int columnCountB = matrixB->GetColumnCount();
    if (rowCountA != rowCountB || columnCountA != columnCountB) {
      return std::unique_ptr<RowMatrix<T>>(nullptr);
    }
    auto res = std::make_unique<RowMatrix<T>>(rowCountA, columnCountA);
    for (int i = 0; i < rowCountA; i ++) {
      for (int j = 0; j < columnCountA; j ++) {
        T elem1 = matrixA->GetElement(i, j);
        T elem2 = matrixB->GetElement(i, j);
        res->SetElement(i, j, elem1 + elem2);
      }
    }
    return res;
  }

  /**
   * Compute the matrix multiplication (`matrixA` * `matrixB` and return the result.
   * Return `nullptr` if dimensions mismatch for input matrices.
   * @param matrixA Input matrix
   * @param matrixB Input matrix
   * @return The result of matrix multiplication
   */
  static auto Multiply(const RowMatrix<T> *matrixA, const RowMatrix<T> *matrixB) -> std::unique_ptr<RowMatrix<T>> {
    int rowCountA = matrixA->GetRowCount();
    int columnCountA = matrixA->GetColumnCount();
    int rowCountB = matrixB->GetRowCount();
    int columnCountB = matrixB->GetColumnCount();
    if (rowCountB != columnCountA) {
      return std::unique_ptr<RowMatrix<T>>(nullptr);
    }
    auto res = std::make_unique<RowMatrix<T>>(rowCountA, columnCountB);
    for (int i = 0; i < rowCountA; i ++) {
      for (int j = 0; j < columnCountB; j ++) {
        int sum = 0;
        for (int k = 0; k < rowCountB; k ++) {
          sum += (matrixA->GetElement(i, k) * matrixB->GetElement(k, j));
        }
        res->SetElement(i, j, sum);
      }
    }
    return res;
  }

  /**
   * Simplified General Matrix Multiply operation. Compute (`matrixA` * `matrixB` + `matrixC`).
   * Return `nullptr` if dimensions mismatch for input matrices.
   * @param matrixA Input matrix
   * @param matrixB Input matrix
   * @param matrixC Input matrix
   * @return The result of general matrix multiply
   */
  static auto GEMM(const RowMatrix<T> *matrixA, const RowMatrix<T> *matrixB, const RowMatrix<T> *matrixC)
      -> std::unique_ptr<RowMatrix<T>> {
    return Add(Multiply(matrixA, matrixB), matrixC);
  }
};
}  // namespace bustub
