/*
 * Copyright (c) 2017      Darshan Godhia
 * Copyright (c) 2016-2019 Erez Zadok
 * Copyright (c) 2011      Jack Ma
 * Copyright (c) 2019      Jatin Sood
 * Copyright (c) 2017-2018 Kevin Sun
 * Copyright (c) 2015-2017 Leixiang Wu
 * Copyright (c) 2020      Lukas Velikov
 * Copyright (c) 2017-2018 Maryia Maskaliova
 * Copyright (c) 2017      Mayur Jadhav
 * Copyright (c) 2016      Ming Chen
 * Copyright (c) 2017      Nehil Shah
 * Copyright (c) 2016      Nina Brown
 * Copyright (c) 2011-2012 Santhosh Kumar
 * Copyright (c) 2015-2016 Shubhi Rani
 * Copyright (c) 2018      Siddesh Shinde
 * Copyright (c) 2014      Sonam Mandal
 * Copyright (c) 2012      Sudhir Kasanavesi
 * Copyright (c) 2020      Thomas Fleming
 * Copyright (c) 2018-2020 Ibrahim Umit Akgun
 * Copyright (c) 2011-2012 Vasily Tarasov
 * Copyright (c) 2019      Yinuo Zhang
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This header file provides members and functions for implementing
 * readv system call.
 *
 * ReadvSystemCallTraceReplayerModule is a class/module that
 * has members and functions of replaying readv system call.
 *
 * USAGE
 * A main program could initialize this object with a dataseries file
 * and call execute() function until all extents are processed.
 *
 */
#ifndef READV_SYSTEM_CALL_TRACE_REPLAY_MODULE_HPP
#define READV_SYSTEM_CALL_TRACE_REPLAY_MODULE_HPP

#include <sys/uio.h>
#include <unistd.h>

#include "SystemCallTraceReplayModule.hpp"

class ReadvSystemCallTraceReplayModule : public SystemCallTraceReplayModule {
 private:
  bool verify_;
  // Readv System Call Trace Fields in Dataseries file
  Int32Field descriptor_;
  Int32Field count_;
  Int32Field iov_number_;
  Variable32Field data_read_;
  Int64Field bytes_requested_;

  /**
   * Print readv sys call field values in a nice format
   */
  void print_specific_fields() override;

  /**
   * This function will gather arguments in the trace file
   * and then replay an readv system call with those arguments.
   */
  void processRow() override;

 public:
  ReadvSystemCallTraceReplayModule(DataSeriesModule &source, bool verbose_flag,
                                   bool verify_flag, int warn_level_flag);
};
#endif /* READV_SYSTEM_CALL_TRACE_REPLAY_MODULE_HPP */
