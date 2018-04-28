/*
 * Copyright (c) 2017-2018 Kevin Sun
 * Copyright (c) 2015-2018 Erez Zadok
 * Copyright (c) 2015-2018 Stony Brook University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This file implements the recv function in the DataSeriesOutputModule.hpp
 * header file.
 *
 * Read the DataSeriesOutputModule.hpp file for more information about this
 * class.
 */

#include "DataSeriesOutputModule.hpp"

/*
 * NOTE: support for replaying the recv(2) system call is
 * incomplete.
 */

void DataSeriesOutputModule::makeRecvArgsMap(void **args_map,
					     long *args,
					     void **v_args) {
  initArgsMap(args_map, "recv");
  args_map[SYSCALL_FIELD_DESCRIPTOR] = &args[0];
  args_map[SYSCALL_FIELD_DATA_READ] = &v_args[0];
  args_map[SYSCALL_FIELD_BYTES_REQUESTED] = &args[2];
  args_map[SYSCALL_FIELD_FLAGS_VALUE] = &args[3];
  // Set individual recv flag bits
  u_int flag = processRecvFlags(args_map, args[3]);
  if (flag != 0) {
    std::cerr << "Recv: These flags are not processed/unknown->0x";
    std::cerr << std::hex << flag << std::dec << std::endl;
  }
}

u_int DataSeriesOutputModule::processRecvFlags(void **args_map,
					       u_int recv_flags) {
  /*
   * Process each individual recv flag bit that has been set
   * in the argument recv_flags.
   */
  // set recv cmsg_cloexec flag
  process_Flag_and_Mode_Args(args_map, recv_flags, MSG_CMSG_CLOEXEC,
			     SYSCALL_FIELD_FLAG_CMSG_CLOEXEC);
  // set recv dontwait flag
  process_Flag_and_Mode_Args(args_map, recv_flags, MSG_DONTWAIT,
			     SYSCALL_FIELD_FLAG_DONTWAIT);
  // set recv errqueue flag
  process_Flag_and_Mode_Args(args_map, recv_flags, MSG_ERRQUEUE,
			     SYSCALL_FIELD_FLAG_ERRQUEUE);
  // set recv oob flag
  process_Flag_and_Mode_Args(args_map, recv_flags, MSG_OOB,
			     SYSCALL_FIELD_FLAG_OOB);
  // set recv peek flag
  process_Flag_and_Mode_Args(args_map, recv_flags, MSG_PEEK,
			     SYSCALL_FIELD_FLAG_PEEK);
  // set recv trunc flag
  process_Flag_and_Mode_Args(args_map, recv_flags, MSG_TRUNC,
			     SYSCALL_FIELD_FLAG_TRUNC);
  // set recv waitall flag
  process_Flag_and_Mode_Args(args_map, recv_flags, MSG_WAITALL,
			     SYSCALL_FIELD_FLAG_WAITALL);
  /*
   * Return remaining recv flags so that caller can
   * warn of unknown flags if the recv_flags is not set as zero.
   */
  return recv_flags;
}
