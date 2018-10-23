/*
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
 * LSeekSystemCallTraceReplayModule header file
 *
 * Read LSeekSystemCallTraceReplayModule.hpp for more information
 * about this class.
 */

#include "LSeekSystemCallTraceReplayModule.hpp"

LSeekSystemCallTraceReplayModule::
LSeekSystemCallTraceReplayModule(DataSeriesModule &source,
				 bool verbose_flag,
				 int warn_level_flag):
  SystemCallTraceReplayModule(source, verbose_flag, warn_level_flag),
  descriptor_(series, "descriptor"),
  offset_(series, "offset"),
  whence_(series, "whence") {
  sys_call_name_ = "lseek";
}

void LSeekSystemCallTraceReplayModule::print_specific_fields() {
  pid_t pid = executing_pid();
  int replayed_fd = replayer_resources_manager_.get_fd(pid, descriptor_.val());

  syscall_logger_->log_info("traced fd(", descriptor_.val(), "), ",
    "replayed fd(", replayed_fd, "), ",
    "offset(", offset_.val(), "), ", \
    "whence(", (unsigned) whence_.val(), ")");
}

void LSeekSystemCallTraceReplayModule::processRow() {
  replayed_fd = replayer_resources_manager_.get_fd(executingPidVal, descriptorVal);
  if (replayed_fd == SYSCALL_SIMULATED) {
    /*
     * FD for the lseek system call originated from a socket().
     * The system call will not be replayed.
     * Traced return value will be returned.
     */
    return;
  }
  // Replay the lseek system call
  replayed_ret_val_ = lseek(replayed_fd, offset, whence);
}

void LSeekSystemCallTraceReplayModule::prepareRow() {
  // Get replaying file descriptor.
  descriptorVal = descriptor_.val();
  offset = offset_.val();
  whence = whence_.val();
  replayed_ret_val_ = return_value_.val();
  SystemCallTraceReplayModule::prepareRow();
}
