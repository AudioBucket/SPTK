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

#ifndef SPTK_INPUT_INPUT_SOURCE_PREPROCESSING_FOR_FILTER_GAIN_H_
#define SPTK_INPUT_INPUT_SOURCE_PREPROCESSING_FOR_FILTER_GAIN_H_

#include <vector>  // std::vector

#include "SPTK/input/input_source_interface.h"
#include "SPTK/utils/sptk_utils.h"

namespace sptk {

class InputSourcePreprocessingForFilterGain : public InputSourceInterface {
 public:
  //
  enum FilterGainType {
    kLinear = 0,
    kLog,
    kUnity,
    kUnityForAllZeroFilter,
  };

  //
  InputSourcePreprocessingForFilterGain(FilterGainType gain_type,
                                        InputSourceInterface* source)
      : gain_type_(gain_type), source_(source), is_valid_(true) {
    if (NULL == source_ || !source_->IsValid()) {
      is_valid_ = false;
      return;
    }
  }

  //
  virtual ~InputSourcePreprocessingForFilterGain() {
  }

  //
  FilterGainType GetFilterGainType() const {
    return gain_type_;
  }

  //
  virtual int GetSize() const {
    return source_ ? source_->GetSize() : 0;
  }

  //
  virtual bool IsValid() const {
    return is_valid_;
  }

  //
  virtual bool Get(std::vector<double>* buffer);

 private:
  //
  const FilterGainType gain_type_;

  //
  InputSourceInterface* source_;

  //
  bool is_valid_;

  //
  DISALLOW_COPY_AND_ASSIGN(InputSourcePreprocessingForFilterGain);
};

}  // namespace sptk

#endif  // SPTK_INPUT_INPUT_SOURCE_PREPROCESSING_FOR_FILTER_GAIN_H_
