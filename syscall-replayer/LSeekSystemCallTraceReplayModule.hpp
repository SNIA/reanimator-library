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
 * This header file provides members and functions for implementing lseek
 * system call.
 *
 * LSeekSystemCallTraceReplayerModule is a class/module that 
 * has members and functions of replaying lseek system call. 
 * 
 * USAGE
 * A main program could initialize this object with a dataseries file
 * and call execute() function until all extents are processed.
 */
#ifndef LSEEK_SYSTEM_CALL_TRACE_REPLAY_MODULE_HPP
#define LSEEK_SYSTEM_CALL_TRACE_REPLAY_MODULE_HPP

#include <unistd.h>

#include "SystemCallTraceReplayModule.hpp"

class LSeekSystemCallTraceReplayModule : public SystemCallTraceReplayModule {
private:
  /* LSeek System Call Trace Fields in Dataseries file */
  Int32Field descriptor_;
  Int64Field offset_;
  ByteField whence_;
  
  /*
   * This function will prepare things before replaying any
   * lseek system call. Right now it displays a starting
   * message.
   */
  void prepareForProcessing();

  /*
   * This function will gather arguments in the trace file 
   * and then replay lseek system call with those arguments.
   */
  void processRow();

  /*
   * This function will do things that have be done 
   * after finishing replaying all lseek system calls in the
   * trace files. Now, it only displays an ending message.
   */
  void completeProcessing();

public:
  LSeekSystemCallTraceReplayModule(DataSeriesModule &source,
				   bool verbose_flag,
				   int warn_level_flag);
};
#endif /* LSEEK_SYSTEM_CALL_TRACE_REPLAY_MODULE_HPP */
