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
 * This header file provides members and functions for implementing read
 * system call.
 *
 * ReadSystemCallTraceReplayerModule is a class/module that 
 * has members and functions of replaying read system call. 
 * 
 * USAGE
 * A main program could initialize this object with a dataseries file
 * and call execute() function until all extents are processed.
 *
 */
#ifndef READ_SYSTEM_CALL_TRACE_REPLAY_MODULE_HPP
#define READ_SYSTEM_CALL_TRACE_REPLAY_MODULE_HPP

#include <unistd.h>

#include "SystemCallTraceReplayModule.hpp"

class ReadSystemCallTraceReplayModule : public SystemCallTraceReplayModule {
private:
  bool verify_;
  /* Read System Call Trace Fields in Dataseries file */
  Int32Field descriptor_;
  Variable32Field data_read_;
  Int64Field bytes_requested_;
  
  /*
   * This function will prepare things before replaying any
   * read system call. Right now it displays a starting
   * message.
   */
  void prepareForProcessing();

  /*
   * This function will gather arguments in the trace file 
   * and then replay an read system call with those arguments.
   */
  void processRow();

  /*
   * This function will do things that have be done 
   * after finishing replaying all read system calls.
   * Now, it only displays an ending message.
   */
  void completeProcessing();

public:
  ReadSystemCallTraceReplayModule(DataSeriesModule &source, 
				  bool verbose_flag, 
				  bool verify_flag, 
				  int warn_level_flag);
};
#endif /* READ_SYSTEM_CALL_TRACE_REPLAY_MODULE_HPP */