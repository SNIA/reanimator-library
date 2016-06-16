/*
 * Copyright (c) 2015-2016 Leixiang Wu
 * Copyright (c) 2015-2016 Erez Zadok
 * Copyright (c) 2015-2016 Stony Brook University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * FIX ME: fix following comments
 * 2ds.
 * This program is similar to DataSeries's csv2ds utility, but handles
 * extents with different types and nullable fields and is primarily used
 * for converting system call csv traces.
 *
 * Usage: ./csv2ds <outputfile> <tablefile> <spec_string_file>
 *        <xml directory> <inputfiles...>
 *
 * <outputfile>: name of the dataseries output file
 * <tablefile>: name of the table file to refer to
 * <spec_string_file>: name of the file that contains a string
 *                    that specifies the format of the input file
 * <xml directory>: directory path that contains extent xml.
 *                 Remember to '/' should be the last character.
 * <inputfiles...>: input CSV files
 */

#ifndef DATA_SERIES_OUTPUT_MODULE_HPP
#define DATA_SERIES_OUTPUT_MODULE_HPP

#include <cassert>

#include <iostream>
#include <utility>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

#include <DataSeries/ExtentType.hpp>
#include <DataSeries/DataSeriesFile.hpp>
#include <DataSeries/DataSeriesModule.hpp>

#include <fcntl.h>

/* map<fieldname, pair<nullable, ExtentType> */
typedef std::map<std::string, std::pair<bool, ExtentType::fieldType> > config_table_entry_type;
/* map<extentname, config_table_entry_type> */
typedef std::map<std::string,config_table_entry_type > config_table_type;

/* map<fieldName, <DS Field, DS field type>*/
typedef std::map<std::string, std::pair<void *, ExtentType::fieldType> > FieldMap;

/* map<syscallName, FieldMap> */
typedef std::map<std::string, FieldMap> ExtentMap;

// map<extent name, OutputModule>
typedef std::map<std::string, OutputModule*> OutputModuleMap;

class DataSeriesOutputModule {
public:
  // Constructor to set up all extents and fields
  DataSeriesOutputModule(std::ifstream &table_stream,
			 const std::string xml_dir,
			 const char *output_file);

  // Register the record and field values in into DS fields
  bool writeRecord(const char *extent_name, long *args, void **v_args);

  // Destructor to delete the module
  ~DataSeriesOutputModule();

private:
  OutputModuleMap modules_;
  ExtentMap extents_;
  /* Sink is a wrapper for a DataSeries output file. */
  DataSeriesSink ds_sink_;
  config_table_type config_table_;
  unsigned int record_num_;

  // Disable copy constructor
  DataSeriesOutputModule(const DataSeriesOutputModule&);

  // Initialize config table
  void initConfigTable(std::ifstream &table_stream);

  // Add a extent(system call)
  void addExtent(const std::string &extent_name, ExtentSeries &series);

  // Add a field(system call arg) to the desired extent
  void addField(const std::string &extent_name,
		const std::string &field_name,
		void *field,
		const ExtentType::fieldType field_type);

  // Set corresponding DS field to the given value
  void setField(const std::string &extent_name,
		const std::string &field_name,
		void *field_value);

  // Set corresponding DS field to null
  void setFieldNull(const std::string &extent_name,
		    const std::string &field_name);

  template <typename FieldType, typename ValueType>
  void doSetField(const std::string &extent_name,
		  const std::string &field_name,
		  void* field_value);

  // Initialize args map for given system call
  void initArgsMap(std::map<std::string, void *> &args_map,
		   const char *extent_name);

  // Maps Close Sytem Call field value pair
  void makeCloseArgsMap(std::map<std::string, void *> &args_map, long *args);

  // Maps Open System Call field value pair
  void makeOpenArgsMap(std::map<std::string, void *> &args_map,
		       long *args,
		       void **v_args);

  // Process individual flag and mode bits
  void process_Flag_and_Mode_Args(std::map<std::string, void *> &args_map,
				  unsigned int &num,
				  int value,
				  std::string field_name);

  // Maps individual flag value for Open system call to its corresponding field name
  u_int processOpenFlags(std::map<std::string, void *> &args_map, u_int flag);

  // Maps individual mode bits of mode argument to its corresponding field name
  mode_t processMode(std::map<std::string, void *> &args_map,
		     long *args,
		     u_int offset);

  // Maps Read System Call field value pair
  void makeReadArgsMap(std::map<std::string, void *> &args_map,
		       long *args,
		       void **v_args);

  // Maps Write System Call field value pair
  void makeWriteArgsMap(std::map<std::string, void *> &args_map,
			long *args,
			void **v_args);
};

#endif // DATA_SERIES_OUTPUT_MODULE_HPP
