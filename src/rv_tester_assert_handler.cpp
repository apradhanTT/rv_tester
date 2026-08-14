// SPDX-FileCopyrightText: 2026 Tenstorrent USA, Inc.
// SPDX-License-Identifier: Apache-2.0

#include "cvm/logger.hpp"
#include "cvm/plusargs.hpp"
#include <sstream> // stringstream
#include <regex>
#include <stacktrace> // std::stacktrace (C++23; needs -std=c++23 and -lstdc++exp)

DEFINE_string(assert_ignore, "", "Downgrade asserts matching any string from this list");

extern "C" {
void rv_tester_cvm_terminate(char* msg) {
  std::string msg_str(msg);

  // Ignore assert if it matches +assert_ignore
  if (FLAGS_assert_ignore != "") {
    std::stringstream ss(FLAGS_assert_ignore);
    std::regex pattern("error", std::regex_constants::icase);
    while (ss.good()) {
      std::string s;
      std::getline(ss, s, ',');

      if (msg_str.find(s) != std::string::npos) {
        cvm::log(cvm::NONE, "Ignoring assert due to +assert_ignore: {}\n", std::regex_replace(msg_str, pattern, ""));
        return;
      }
    }
  }

  // If not waived, cvm::ERROR
  cvm::log(cvm::ERROR, "\n{}\n", msg_str);

  // Dump the native C++ call stack at the point of termination. This surfaces the
  // host-side frames (transaction handlers, callbacks, DPI entry) that led here,
  // with file:line when built with -g. std::stacktrace only sees native frames,
  // not the SV call chain.
  std::ostringstream trace;
  trace << std::stacktrace::current();
  cvm::log(cvm::ERROR, "C++ stack trace:\n{}\n", trace.str());
}
}
