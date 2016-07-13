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
 * WriteSystemCallTraceReplayModule header file
 *
 * Read WriteSystemCallTraceReplayModule.hpp for more information
 * about this class.
 */

#include "WriteSystemCallTraceReplayModule.hpp"

WriteSystemCallTraceReplayModule::
WriteSystemCallTraceReplayModule(DataSeriesModule &source,
				 bool verbose_flag,
				 bool verify_flag,
				 int warn_level_flag,
				 std::string pattern_data):
  SystemCallTraceReplayModule(source, verbose_flag, warn_level_flag),
  verify_(verify_flag),
  pattern_data_(pattern_data),
  descriptor_(series, "descriptor"),
  data_written_(series, "data_written", Field::flag_nullable),
  bytes_requested_(series, "bytes_requested") {
  sys_call_name_ = "write";
}

void WriteSystemCallTraceReplayModule::print_specific_fields() {
  std::cout << "descriptor(" << descriptor_.val() << "), ";
  std::cout << "data(" << data_written_.val() << "), ";
  std::cout << "nbytes(" << bytes_requested_.val() << ")";
}

void WriteSystemCallTraceReplayModule::processRow() {
  size_t nbytes = bytes_requested_.val();
  int fd = SystemCallTraceReplayModule::fd_map_[descriptor_.val()];
  char *data_buffer;

  if (pattern_data_ == "random") {
#ifdef DEV_URANDOM
    // Pattern is random, so open urandom device
    random_file_.open("/dev/urandom");
    if (!random_file_.is_open()) {
      std::cerr << "Unable to open file '/dev/urandom/'.\n";
      exit(EXIT_FAILURE);
    }
#endif
  }

  // Check to see if write data was in DS and user didn't specify pattern
  if (data_written_.isNull() && pattern_data_.empty()) {
    // Let's write zeros.
    pattern_data_ = "0x0";
  }

  // Check to see if user wants to use pattern
  if (!pattern_data_.empty()) {
    data_buffer = new char[nbytes];
    if (pattern_data_ == "random") {
#ifdef DEV_URANDOM
      random_file_.read(data_buffer, nbytes);
#else
      data_buffer = random_fill_buffer(data_buffer, nbytes);
#endif
    } else {
      int pattern_hex;
      std::stringstream pattern_stream;
      pattern_stream << std::hex << pattern_data_;
      pattern_stream >> pattern_hex;
      memset(data_buffer, pattern_hex, nbytes);
    }
  } else {
    data_buffer = (char *)data_written_.val();
  }

  replayed_ret_val_ = write(fd, data_buffer, nbytes);

  // Free the buffer
  if (!pattern_data_.empty()){
    delete[] data_buffer;
  }

  if (pattern_data_ == "random") {
#ifdef DEV_URANDOM
    // Close the urandom device
    random_file_.close();
#endif
  }
}

PWriteSystemCallTraceReplayModule::
PWriteSystemCallTraceReplayModule(DataSeriesModule &source,
				  bool verbose_flag,
				  bool verify_flag,
				  int warn_level_flag,
				  std::string pattern_data):
  WriteSystemCallTraceReplayModule(source, verbose_flag,
				   verify_flag, warn_level_flag, pattern_data),
  offset_(series, "offset") {
  sys_call_name_ = "pwrite";
}

void PWriteSystemCallTraceReplayModule::print_specific_fields() {
  WriteSystemCallTraceReplayModule::print_specific_fields();
  std::cout << "offset(" << offset_.val() << ")";
}

void PWriteSystemCallTraceReplayModule::processRow() {
  // Get replaying file descriptor.
  int fd = SystemCallTraceReplayModule::fd_map_[descriptor_.val()];
  size_t nbytes = bytes_requested_.val();
  char *data_buffer;

  if (pattern_data_ == "random") {
#ifdef DEV_URANDOM
    // Pattern is random, so open urandom device
    random_file_.open("/dev/urandom");
    if (!random_file_.is_open()) {
      std::cerr << "Unable to open file '/dev/urandom/'.\n";
      exit(EXIT_FAILURE);
    }
#endif
  }

  // Check to see if write data is NULL in DS or user didn't specify pattern
  if (data_written_.isNull() && pattern_data_.empty() ) {
    // Let's write zeros.
    pattern_data_ = "0x0";
  }

  // Check to see if user wants to use pattern
  if (!pattern_data_.empty()) {
    data_buffer = new char[nbytes];
    if (pattern_data_ == "random") {
#ifdef DEV_URANDOM
      random_file_.read(data_buffer, nbytes);
#else
      data_buffer = random_fill_buffer(data_buffer, nbytes);
#endif
    } else {
      int pattern_hex;
      std::stringstream pattern_stream;
      pattern_stream << std::hex << pattern_data_;
      pattern_stream >> pattern_hex;
      memset(data_buffer, pattern_hex, nbytes);
    }
  } else {
    data_buffer = (char *)data_written_.val();
  }

  int offset = offset_.val();
  replayed_ret_val_ = pwrite(fd, data_buffer, nbytes, offset);

  // Free the buffer
  if (!pattern_data_.empty())
    delete[] data_buffer;

  if (pattern_data_ == "random") {
#ifdef DEV_URANDOM
    // Close the urandom device
    random_file_.close();
#endif
  }
}
