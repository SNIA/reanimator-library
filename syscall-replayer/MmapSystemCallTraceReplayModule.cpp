/*
 * Copyright (c) 2015-2016 Leixiang Wu
 * Copyright (c) 2015-2016 Shubhi Rani
 * Copyright (c) 2015-2016 Sonam Mandal
 * Copyright (c) 2015-2016 Erez Zadok
 * Copyright (c) 2015-2016 Stony Brook University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This file implements all the functions in the
 * MmapSystemCallTraceReplayModule header file.
 *
 * Read MmapSystemCallTraceReplayModule.hpp for more information
 * about this class.
 */

#include "MmapSystemCallTraceReplayModule.hpp"

MmapSystemCallTraceReplayModule::
MmapSystemCallTraceReplayModule(DataSeriesModule &source,
				bool verbose_flag,
				int warn_level_flag):
  SystemCallTraceReplayModule(source, verbose_flag, warn_level_flag),
  start_address_(series, "start_address"),
  length_(series, "length"),
  protection_value_(series, "protection_value", Field::flag_nullable),
  flags_value_(series, "flags_value", Field::flag_nullable),
  descriptor_(series, "descriptor"),
  offset_(series, "offset") {
  sys_call_name_ = "mmap";
}

void MmapSystemCallTraceReplayModule::print_specific_fields() {
  std::cout << "start_address(0x" << std::hex << start_address_.val() << "), ";
  std::cout << "length(" << std::dec << length_.val() << "), ";
  std::cout << "protection_value(" << protection_value_.val() << "), ";
  std::cout << "flags_value(" << flags_value_.val() << "), ";
  std::cout << "descriptor(" << descriptor_.val() << "), ";
  std::cout << "offset(0x" << std::hex << offset_.val() << std::dec;
  std::cout << ")" << std::endl;
}

void MmapSystemCallTraceReplayModule::processRow() {
  /*
   * NOTE: It is inappropriate to replay mmap system call.
   * Hence we do not replay mmap system call.
   */
  return;
}