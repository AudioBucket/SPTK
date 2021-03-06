// ----------------------------------------------------------------- //
//             The Speech Signal Processing Toolkit (SPTK)           //
//             developed by SPTK Working Group                       //
//             http://sp-tk.sourceforge.net/                         //
// ----------------------------------------------------------------- //
//                                                                   //
//  Copyright (c) 1984-2007  Tokyo Institute of Technology           //
//                           Interdisciplinary Graduate School of    //
//                           Science and Engineering                 //
//                                                                   //
//                1996-2018  Nagoya Institute of Technology          //
//                           Department of Computer Science          //
//                                                                   //
// All rights reserved.                                              //
//                                                                   //
// Redistribution and use in source and binary forms, with or        //
// without modification, are permitted provided that the following   //
// conditions are met:                                               //
//                                                                   //
// - Redistributions of source code must retain the above copyright  //
//   notice, this list of conditions and the following disclaimer.   //
// - Redistributions in binary form must reproduce the above         //
//   copyright notice, this list of conditions and the following     //
//   disclaimer in the documentation and/or other materials provided //
//   with the distribution.                                          //
// - Neither the name of the SPTK working group nor the names of its //
//   contributors may be used to endorse or promote products derived //
//   from this software without specific prior written permission.   //
//                                                                   //
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            //
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       //
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          //
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          //
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS //
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          //
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   //
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     //
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON //
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   //
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    //
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           //
// POSSIBILITY OF SUCH DAMAGE.                                       //
// ----------------------------------------------------------------- //

#ifndef SPTK_MATH_MATRIX_H_
#define SPTK_MATH_MATRIX_H_

#include <vector>  // std::vector

namespace sptk {

class Matrix {
 public:
  //
  explicit Matrix(int num_row = 0, int num_column = 0);

  //
  Matrix(int num_row, int num_column, const std::vector<double>& vector);

  //
  Matrix(const Matrix& matrix);

  //
  Matrix& operator=(const Matrix& matrix);

  //
  virtual ~Matrix() {
  }

  //
  int GetNumRow() const {
    return num_row_;
  }

  //
  int GetNumColumn() const {
    return num_column_;
  }

  //
  void Resize(int num_row, int num_column);

  //
  double* operator[](int row) {
    return index_[row];
  }

  //
  const double* operator[](int row) const {
    return index_[row];
  }

  //
  double& At(int row, int column);

  //
  const double& At(int row, int column) const;

  //
  Matrix operator+(const Matrix& matrix) const;

  //
  Matrix operator-(const Matrix& matrix) const;

  //
  Matrix operator*(const Matrix& matrix) const;

  //
  void FillZero();

  //
  bool Transpose(Matrix* transposed_matrix) const;

  //
  bool GetSubmatrix(int row_offset, int num_row_of_submatrix, int column_offset,
                    int num_column_of_submatrix, Matrix* submatrix) const;

 private:
  //
  int num_row_;

  //
  int num_column_;

  //
  std::vector<double> data_;

  //
  std::vector<double*> index_;
};

}  // namespace sptk

#endif  // SPTK_MATH_MATRIX_H_
