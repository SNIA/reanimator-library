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

void DataSeriesOutputModule::makeExecveArgsMap(void **args_map, long *args,
                                               void **v_args) {
  int continuation_number = *(int *)v_args[0];
  args_map[SYSCALL_FIELD_CONTINUATION_NUMBER] = v_args[0];

  /*
   * Continuation number equal to '0' denotes the first record of
   * single execve system call. For first record, we only save the
   * continuation number and given pathname fields.
   */
  if (continuation_number == 0) {
    if (v_args[1] != NULL)
      args_map[SYSCALL_FIELD_GIVEN_PATHNAME] = &v_args[1];
    else
      std::cerr << "Execve: Pathname is set as NULL!!" << std::endl;
  } else if (continuation_number > 0) {
    /*
     * If continuation number is greater than '0', then add
     * record to set the argument or environment variables.
     */
    char *arg_env = (char *)v_args[2];
    if (strcmp(arg_env, "arg") == 0) {
      /*
       * If arg_env is equal to "arg", then we only save the
       * continuation number and argument fields in the new
       * record.
       */
      if (v_args[1] != NULL) {
        args_map[SYSCALL_FIELD_ARGUMENT] = &v_args[1];
      }
    } else if (strcmp(arg_env, "env") == 0) {
      /*
       * If arg_env is equal to "env", then we only save the
       * continuation number and environment fields in the
       * new record.
       */
      if (v_args[1] != NULL)
        args_map[SYSCALL_FIELD_ENVIRONMENT] = &v_args[1];
      else
        std::cerr << "Execve : Environment is set as NULL!!" << std::endl;
    }
  }
}
