/*
 * Copyright (c) 2015-2017 Leixiang Wu
 * Copyright (c) 2015-2016 Shubhi Rani
 * Copyright (c) 2015-2016 Sonam Mandal
 * Copyright (c) 2015-2017 Erez Zadok
 * Copyright (c) 2015-2017 Stony Brook University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This header file provides members and functions for implementing creat
 * system call.
 *
 * CreatSystemCallTraceReplayerModule is a class/module that
 * has members and functions of replaying creat system call.
 *
 * USAGE
 * A main program could initialize this class with a dataseries file
 * and call execute() function until all extents are processed.
 */
#ifndef CREAT_SYSTEM_CALL_TRACE_REPLAY_MODULE_HPP
#define CREAT_SYSTEM_CALL_TRACE_REPLAY_MODULE_HPP

#include "SystemCallTraceReplayModule.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

class CreatSystemCallTraceReplayModule : public SystemCallTraceReplayModule {
private:
  // Creat System Call Trace Fields in Dataseries file
  Variable32Field given_pathname_;
  Int32Field mode_value_;

  /**
   * Print creat sys call field values in a nice format
   */
  void print_specific_fields();

  /**
   * This function will gather arguments in the trace file
   * and replay an creat system call with those arguments.
   */
  void processRow();

public:
  CreatSystemCallTraceReplayModule(DataSeriesModule &source,
				   bool verbose_flag,
				   int warn_level_flag);
};

#endif /* CREAT_SYSTEM_CALL_TRACE_REPLAY_MODULE_HPP */
