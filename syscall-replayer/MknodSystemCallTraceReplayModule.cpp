/*
 * Copyright (c) 2016 Nina Brown
 * Copyright (c) 2015-2017 Leixiang Wu
 * Copyright (c) 2015-2016 Shubhi Rani
 * Copyright (c) 2015-2016 Sonam Mandal
 * Copyright (c) 2015-2016 Erez Zadok
 * Copyright (c) 2015-2017 Stony Brook University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This file implements all the functions in the
 * MknodSystemCallTraceReplayModule header file
 *
 * Read MknodSystemCallTraceReplayModule.hpp for more information
 * about this class.
 */

#include "MknodSystemCallTraceReplayModule.hpp"

MknodSystemCallTraceReplayModule::MknodSystemCallTraceReplayModule(
    DataSeriesModule &source, bool verbose_flag, int warn_level_flag)
    : SystemCallTraceReplayModule(source, verbose_flag, warn_level_flag),
      given_pathname_(series, "given_pathname", Field::flag_nullable),
      mode_value_(series, "mode_value", Field::flag_nullable),
      type_(series, "type"),
      dev_(series, "dev", Field::flag_nullable) {
  sys_call_name_ = "mknod";
}

void MknodSystemCallTraceReplayModule::print_specific_fields() {
  std::string file_type = "";
  // Decode the file type field from the encoding specified by SNIA
  switch (type_.val()) {
    case DS_FILE_TYPE_REG:
      file_type = "regular";
      break;
    case DS_FILE_TYPE_CHR:
      file_type = "character special";
      break;
    case DS_FILE_TYPE_BLK:
      file_type = "block special";
      break;
    case DS_FILE_TYPE_FIFO:
      file_type = "FIFO";
      break;
    case DS_FILE_TYPE_SOCK:
      file_type = "socket";
      break;
  }

  syscall_logger_->log_info(
      "pathname(", given_pathname_.val(), "), ", "file type(", file_type, "), ",
      "traced mode(", mode_value_.val(), ")", "replayed mode(",
      get_mode(mode_value_.val()), ")", "dev(", dev_.val(), ")");
}

void MknodSystemCallTraceReplayModule::processRow() {
  const char *pathname = (char *)given_pathname_.val();
  mode_t mode = get_mode(mode_value_.val());
  dev_t dev = dev_.val();

  // replay the mknod system call
  replayed_ret_val_ = mknod(pathname, mode, dev);
}
