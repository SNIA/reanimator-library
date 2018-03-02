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

void DataSeriesOutputModule::makeRenameArgsMap(void **args_map,
					       long *args,
					       void **v_args) {
  if (v_args[0] != NULL) {
    args_map[SYSCALL_FIELD_GIVEN_OLDNAME] = &v_args[0];
  } else {
    std::cerr << "Rename: Old name is set as NULL!!" << std::endl;
  }

  if (v_args[1] != NULL) {
    args_map[SYSCALL_FIELD_GIVEN_NEWNAME] = &v_args[1];
  } else {
    std::cerr << "Rename: New name is set as NULL!!" << std::endl;
  }
}