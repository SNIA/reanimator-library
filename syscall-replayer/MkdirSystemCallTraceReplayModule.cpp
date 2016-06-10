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
 * This file implements all the functions in the MkdirSystemCallTraceReplayModule 
 * header file 
 *
 * Read MkdirSystemCallTraceReplayModule.hpp for more information about this class.
 */

#include "MkdirSystemCallTraceReplayModule.hpp"

MkdirSystemCallTraceReplayModule::MkdirSystemCallTraceReplayModule(DataSeriesModule &source,
								   bool verbose_flag, 
								   int warn_level_flag):
  SystemCallTraceReplayModule(source, verbose_flag, warn_level_flag),
  given_pathname_(series, "given_pathname"),
  mode_value_(series, "mode_value", Field::flag_nullable) {
  sys_call_name_ = "mkdir";
}

void MkdirSystemCallTraceReplayModule::prepareForProcessing() {
  std::cout << "-----Mkdir System Call Replayer starts to replay...-----" << std::endl;
}

void MkdirSystemCallTraceReplayModule::print_specific_fields() {
  std::cout << "pathname(" << given_pathname_.val() << "), ";
  std::cout << "mode(" << mode_value_.val() << ")";
}

void MkdirSystemCallTraceReplayModule::processRow() {
  const char *pathname = (char *)given_pathname_.val();
  mode_t mode = mode_value_.val();

  // replay the mkdir system call
  replayed_ret_val_ = mkdir(pathname, mode);
}

void MkdirSystemCallTraceReplayModule::completeProcessing() {
  std::cout << "-----Mkdir System Call Replayer finished replaying...-----" << std::endl;
}