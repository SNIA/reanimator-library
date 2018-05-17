/*
 * Copyright (c) 2016-2016 Nina Brown
 * Copyright (c) 2015-2017 Leixiang Wu
 * Copyright (c) 2015-2016 Shubhi Rani
 * Copyright (c) 2015-2018 Erez Zadok
 * Copyright (c) 2015-2018 Stony Brook University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This file implements all the functions in the DataSeriesOutputModule.hpp
 * header file.
 *
 * Read the DataSeriesOutputModule.hpp file for more information about this
 * class.
 */

#include "DataSeriesOutputModule.hpp"

void DataSeriesOutputModule::makeSendmsgArgsMap(void **args_map,
					      long *args,
					      void **v_args) {
  int iov_number = -1;
  if (v_args[0] != NULL) {
    iov_number = *(int *) v_args[0];
  }

  /*
   * iov_number equal to '-1' denotes the first record for the
   * sendmsg system call. For first record, we save the file
   * descriptor, iov_number and total number of bytes
   * requested and do not set the iov_data_written field.
   */
  if (iov_number == -1) {
    // Initialize all non-nullable boolean fields to False.
    initArgsMap(args_map, "sendmsg");
    args_map[SYSCALL_FIELD_DESCRIPTOR] = &args[0];
    args_map[SYSCALL_FIELD_FLAGS_VALUE] = &args[2];
    // Set individual send flag bits
    u_int flag = processSendFlags(args_map, args[2]);
    if (flag != 0) {
      std::cerr << "Sendmsg: These flag are not processed/unknown->0x";
      std::cerr << std::hex << flag << std::dec << std::endl;
    }
    if (v_args[0] != NULL && v_args[1] != NULL ) {
      args_map[SYSCALL_FIELD_IOV_NUMBER] = v_args[0];
      args_map[SYSCALL_FIELD_BYTES_REQUESTED] = v_args[1];
    } else {
      std::cerr << "Sendmsg: struct msghdr buffer is set as NULL!!" << std::endl;
    }
  } else {
    /*
     * For rest of the records, we do not save file descriptor and
     * count fields. We only save the iov_number, bytes_requested
     * and iov_data_written fields.
     */
    args_map[SYSCALL_FIELD_IOV_NUMBER] = v_args[0];
    args_map[SYSCALL_FIELD_BYTES_REQUESTED] = v_args[1];
    args_map[SYSCALL_FIELD_IOV_DATA_WRITTEN] = &v_args[2];
  }
}
