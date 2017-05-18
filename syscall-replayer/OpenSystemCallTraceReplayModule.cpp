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
 * OpenSystemCallTraceReplayModule header file
 *
 * Read OpenSystemCallTraceReplayModule.hpp for more information
 * about this class.
 */

#include "OpenSystemCallTraceReplayModule.hpp"

OpenSystemCallTraceReplayModule::
OpenSystemCallTraceReplayModule(DataSeriesModule &source,
  bool verbose_flag,
  int warn_level_flag):
  SystemCallTraceReplayModule(source, verbose_flag, warn_level_flag),
  given_pathname_(series, "given_pathname"),
  open_value_(series, "open_value", Field::flag_nullable),
  mode_value_(series, "mode_value", Field::flag_nullable) {
  sys_call_name_ = "open";
}

void OpenSystemCallTraceReplayModule::print_specific_fields() {
  syscall_logger_->log_info("pathname(", given_pathname_.val(), "), flags(", \
    open_value_.val(), "),",
    "traced mode(", mode_value_.val(), "), ",
    "replayed mode(", get_mode(mode_value_.val()), ")");
}

void OpenSystemCallTraceReplayModule::processRow() {
  const char *pathname = (char *)given_pathname_.val();
  int flags = open_value_.val();
  mode_t mode = get_mode(mode_value_.val());
  int traced_fd = (int)return_value_.val();

  // replay the open system call
  replayed_ret_val_ = open(pathname, flags, mode);
  if (traced_fd == -1 && replayed_ret_val_ != -1) {
    /*
     * Original system open failed, but replay system succeeds.
     * Therefore, we will close the replayed fd.
     */
    close(replayed_ret_val_);
  } else {
    /*
     * Even if traced fd is valid, but replayed fd is -1,
     * we will still add the entry and replay it.
     * Add a mapping from fd in trace file to actual replayed fd
     */
    pid_t pid = executing_pid();
    replayer_resources_manager_.add_fd(pid, traced_fd, replayed_ret_val_, flags);
  }
}

OpenatSystemCallTraceReplayModule::
OpenatSystemCallTraceReplayModule(DataSeriesModule &source,
  bool verbose_flag,
  int warn_level_flag):
  OpenSystemCallTraceReplayModule(source, verbose_flag, warn_level_flag),
  descriptor_(series, "descriptor") {
  sys_call_name_ = "openat";
}

void OpenatSystemCallTraceReplayModule::print_specific_fields() {
  pid_t pid = executing_pid();
  int replayed_fd = replayer_resources_manager_.get_fd(pid, descriptor_.val());
  syscall_logger_->log_info("traced fd(", descriptor_.val(), "), ",
    "replayed fd(", replayed_fd, "), ",
    "pathname(", given_pathname_.val(), "), flags(", \
    open_value_.val(), "), ",
    "traced mode(", mode_value_.val(), "), ",
    "replayed mode(", get_mode(mode_value_.val()), ")");
}

void OpenatSystemCallTraceReplayModule::processRow() {
  pid_t pid = executing_pid();
  int dirfd = replayer_resources_manager_.get_fd(pid, descriptor_.val());
  const char *pathname = (char *)given_pathname_.val();
  int flags = open_value_.val();
  mode_t mode = get_mode(mode_value_.val());
  int traced_fd = (int)return_value_.val();

  // replay the openat system call
  replayed_ret_val_ = openat(dirfd, pathname, flags, mode);
  if (traced_fd == -1 && replayed_ret_val_ != -1) {
    /*
     * Original system open failed, but replay system succeeds.
     * Therefore, we will close the replayed fd.
     */
    close(replayed_ret_val_);
  } else {
    /*
     * Even if traced fd is valid, but replayed fd is -1,
     * we will still add the entry and replay it.
     * Add a mapping from fd in trace file to actual replayed fd
     */
    replayer_resources_manager_.add_fd(pid, traced_fd, replayed_ret_val_, flags);
  }
}
