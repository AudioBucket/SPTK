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

#include <getopt.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "SPTK/math/levinson_durbin_recursion.h"
#include "SPTK/utils/sptk_utils.h"

namespace {

enum WarningType { kIgnore = 0, kWarn, kExit, kNumWarningTypes };

const int kDefaultNumOrder(25);
const double kDefaultEpsilon(0.0);
const WarningType kDefaultWarningType(kIgnore);

void PrintUsage(std::ostream* stream) {
  // clang-format off
  *stream << std::endl;
  *stream << " levdur - solve an autocorrelation normal equation" << std::endl;
  *stream << "          using Levinson-Durbin recursion" << std::endl;
  *stream << std::endl;
  *stream << "  usage:" << std::endl;
  *stream << "       levdur [ options ] [ infile ] > stdout" << std::endl;
  *stream << "  options:" << std::endl;
  *stream << "       -m m  : order of autocorrelation         (   int)[" << std::setw(5) << std::right << kDefaultNumOrder    << "][   0 <= m <=   ]" << std::endl;  // NOLINT
  *stream << "       -f f  : minimum value of the determinant (double)[" << std::setw(5) << std::right << kDefaultEpsilon     << "][ 0.0 <= f <=   ]" << std::endl;  // NOLINT
  *stream << "               of normal matrix" << std::endl;
  *stream << "       -e e  : warning type of unstable index   (   int)[" << std::setw(5) << std::right << kDefaultWarningType << "][   0 <= e <= 2 ]" << std::endl;  // NOLINT
  *stream << "                 0 (no warning)" << std::endl;
  *stream << "                 1 (output the index to stderr)" << std::endl;
  *stream << "                 2 (output the index to stderr" << std::endl;
  *stream << "                    and exit immediately)" << std::endl;
  *stream << "       -h    : print this message" << std::endl;
  *stream << "  infile:" << std::endl;
  *stream << "       autocorrelation sequence                 (double)[stdin]" << std::endl;  // NOLINT
  *stream << "  stdout:" << std::endl;
  *stream << "       linear predictive coefficients           (double)" << std::endl;  // NOLINT
  *stream << std::endl;
  *stream << " SPTK: version " << sptk::kVersion << std::endl;
  *stream << std::endl;
  // clang-format on
}

}  // namespace

int main(int argc, char* argv[]) {
  int num_order(kDefaultNumOrder);
  double epsilon(kDefaultEpsilon);
  WarningType warning_type(kDefaultWarningType);

  for (;;) {
    const int option_char(getopt_long(argc, argv, "m:f:e:h", NULL, NULL));
    if (-1 == option_char) break;

    switch (option_char) {
      case 'm': {
        if (!sptk::ConvertStringToInteger(optarg, &num_order) ||
            num_order < 0) {
          std::ostringstream error_message;
          error_message << "The argument for the -m option must be a "
                        << "non-negative integer";
          sptk::PrintErrorMessage("levdur", error_message);
          return 1;
        }
        break;
      }
      case 'f': {
        if (!sptk::ConvertStringToDouble(optarg, &epsilon) || epsilon < 0.0) {
          std::ostringstream error_message;
          error_message
              << "The argument for the -f option must be a non-negative number";
          sptk::PrintErrorMessage("levdur", error_message);
          return 1;
        }
        break;
      }
      case 'e': {
        const int min(0);
        const int max(static_cast<int>(kNumWarningTypes) - 1);
        int tmp;
        if (!sptk::ConvertStringToInteger(optarg, &tmp) ||
            !sptk::IsInRange(tmp, min, max)) {
          std::ostringstream error_message;
          error_message << "The argument for the -e option must be an integer "
                        << "in the range of " << min << " to " << max;
          sptk::PrintErrorMessage("levdur", error_message);
          return 1;
        }
        warning_type = static_cast<WarningType>(tmp);
        break;
      }
      case 'h': {
        PrintUsage(&std::cout);
        return 0;
      }
      default: {
        PrintUsage(&std::cerr);
        return 1;
      }
    }
  }

  // get input file
  const int num_input_files(argc - optind);
  if (1 < num_input_files) {
    std::ostringstream error_message;
    error_message << "Too many input files";
    sptk::PrintErrorMessage("levdur", error_message);
    return 1;
  }
  const char* input_file(0 == num_input_files ? NULL : argv[optind]);

  // open stream
  std::ifstream ifs;
  ifs.open(input_file, std::ios::in | std::ios::binary);
  if (ifs.fail() && NULL != input_file) {
    std::ostringstream error_message;
    error_message << "Cannot open file " << input_file;
    sptk::PrintErrorMessage("levdur", error_message);
    return 1;
  }
  std::istream& input_stream(ifs.fail() ? std::cin : ifs);

  sptk::LevinsonDurbinRecursion levinson_durbin_recursion(num_order, epsilon);
  sptk::LevinsonDurbinRecursion::Buffer buffer;
  if (!levinson_durbin_recursion.IsValid()) {
    std::ostringstream error_message;
    error_message << "Failed to set the condition";
    sptk::PrintErrorMessage("levdur", error_message);
    return 1;
  }

  const int length(num_order + 1);
  std::vector<double> autocorrelation_sequence(length);
  std::vector<double> linear_predictive_coefficients(length);

  for (int frame_index(0); sptk::ReadStream(
           false, 0, 0, length, &autocorrelation_sequence, &input_stream, NULL);
       ++frame_index) {
    bool is_stable(false);
    if (!levinson_durbin_recursion.Run(autocorrelation_sequence,
                                       &linear_predictive_coefficients,
                                       &is_stable, &buffer)) {
      std::ostringstream error_message;
      error_message << "Failed to solve autocorrelation normal equations";
      sptk::PrintErrorMessage("levdur", error_message);
      return 1;
    }

    if (!is_stable && kIgnore != warning_type) {
      std::ostringstream error_message;
      error_message << frame_index << "th frame is unstable";
      sptk::PrintErrorMessage("levdur", error_message);
      if (kExit == warning_type) return 1;
    }

    if (!sptk::WriteStream(0, length, linear_predictive_coefficients,
                           &std::cout, NULL)) {
      std::ostringstream error_message;
      error_message << "Failed to write linear predictive coefficients";
      sptk::PrintErrorMessage("levdur", error_message);
      return 1;
    }
  }

  return 0;
}
