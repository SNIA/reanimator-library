/*
 * Copyright (c) 2016-2016 Nina Brown
 * Copyright (c) 2015-2017 Leixiang Wu
 * Copyright (c) 2015-2016 Shubhi Rani
 * Copyright (c) 2015-2017 Erez Zadok
 * Copyright (c) 2015-2017 Stony Brook University
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

void DataSeriesOutputModule::makeLSetxattrArgsMap(void **args_map, long *args,
                                                  void **v_args) {
  // Initialize all non-nullable boolean fields to False.
  initArgsMap(args_map, "lsetxattr");

  if (v_args[0] != NULL) {
    args_map[SYSCALL_FIELD_GIVEN_PATHNAME] = &v_args[0];
  } else {
    std::cerr << "LSetxattr: Pathname is set as NULL!!" << std::endl;
  }

  if (v_args[1] != NULL) {
    args_map[SYSCALL_FIELD_XATTR_NAME] = &v_args[1];
  } else {
    std::cerr << "LSetxattr: Attribute name is set as NULL!!" << std::endl;
  }

  args_map[SYSCALL_FIELD_VALUE_WRITTEN] = &v_args[2];
  args_map[SYSCALL_FIELD_VALUE_SIZE] = &args[3];

  /* Setting flag values */
  args_map[SYSCALL_FIELD_FLAG_VALUE] = &args[4];
  u_int flag = args[4];
  process_Flag_and_Mode_Args(args_map, flag, XATTR_CREATE,
                             SYSCALL_FIELD_FLAG_XATTR_CREATE);
  process_Flag_and_Mode_Args(args_map, flag, XATTR_REPLACE,
                             SYSCALL_FIELD_FLAG_XATTR_REPLACE);
  if (flag != 0) {
    std::cerr << "LSetxattr: These flags are not processed/unknown->0x";
    std::cerr << std::hex << flag << std::dec << std::endl;
  }
}
