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
 * This file implements all the functions in the SymlinkSystemCallTraceReplayModule
 * header file
 *
 * Read SymlinkSystemCallTraceReplayModule.hpp for more information about this class.
 */

#include "SymlinkSystemCallTraceReplayModule.hpp"

SymlinkSystemCallTraceReplayModule::SymlinkSystemCallTraceReplayModule(DataSeriesModule &source,
								       bool verbose_flag,
								       int warn_level_flag):
  SystemCallTraceReplayModule(source, verbose_flag, warn_level_flag),
  target_pathname_(series, "target_pathname"),
  given_pathname_(series, "given_pathname") {
  sys_call_name_ = "symlink";
}

void SymlinkSystemCallTraceReplayModule::print_specific_fields() {
  std::cout << "target path(" << target_pathname_.val() << "), ";
  std::cout << "link path(" << given_pathname_.val() << ")";
}

void SymlinkSystemCallTraceReplayModule::prepareForProcessing() {
  std::cout << "-----Symlink System Call Replayer starts to replay...-----" << std::endl;
}

void SymlinkSystemCallTraceReplayModule::processRow() {
  char *target_path = (char *)target_pathname_.val();
  char *link_path = (char *)given_pathname_.val();

  // Replay symlink system call
  replayed_ret_val_ = symlink(target_path, link_path);
}

void SymlinkSystemCallTraceReplayModule::completeProcessing() {
  std::cout << "-----Symlink System Call Replayer finished replaying...-----" << std::endl;
}