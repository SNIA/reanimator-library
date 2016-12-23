/*
 * Copyright (c) 2015-2016 Leixiang Wu
 * Copyright (c) 2015-2016 Shubhi Rani
 * Copyright (c) 2015-2016 Nina Brown
 * Copyright (c) 2015-2016 Erez Zadok
 * Copyright (c) 2015-2016 Geoff Kuenning
 * Copyright (c) 2015-2016 Stony Brook University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This header file provides members and functions for implementing file
 * descriptor manager.
 *
 * ReplayerResourcesManager is a class that has members and functions of
 * managing file descriptors during replaying.
 *
 * USAGE
 * A main program could initialize this class and call desired
 * function to add/delete/update file descriptor table.
 */

#ifndef REPLAYER_RESOURCES_MANAGER_HPP
#define REPLAYER_RESOURCES_MANAGER_HPP

#include <utility>
#include <map>
#include <string>
#include <vector>
#include <iterator>
#include <assert.h>
#include <iostream>
#include "SystemCallTraceReplayLogger.hpp"

class BasicEntry {
protected:
  unsigned int rc_;

public:
  // Constructor
  BasicEntry();

  /*
   * Increment the reference count by 1.
   */
  void increment_rc();

  /*
   * Decrement the reference count by 1.
   * Return the reference count after decrement
   */
  unsigned int decrement_rc();
};

class UmaskEntry : public BasicEntry{
private:
  mode_t umask_;

public:
  // Constructor
  UmaskEntry(mode_t m);

  /*
   * Return mask
   */
  mode_t get_umask();

  /*
   * This function will update mask value to the given mask.
   */
  void set_umask(mode_t m);
};

class FileDescriptorEntry {
private:
  int fd_;
  int flags_;

public:
  // Constructor
  FileDescriptorEntry(int fd, int flags);

  // Copy Constructor
  FileDescriptorEntry(FileDescriptorEntry& fd_entry);

  /*
   * Return file descriptor
   */
  int get_fd();

  /*
   * This function will update file descriptor value to the given fd.
   */
  void set_fd(int fd);

  /*
   * Return file descriptor flags
   */
  int get_flags();

  /*
   * This function will update file descriptor flags value to the given flags.
   */
  void set_flags(int flags);

  /*
   * Get string representation of this FileDescriptorEntry object
   */
  std::string to_string();
};

class FileDescriptorTableEntry : public BasicEntry{
private:
  // <traced fd, replayed FileDescriptorEntry>
  std::map<int, FileDescriptorEntry*> fd_table_;

public:
  // Basic Constructor
  FileDescriptorTableEntry();

  // Copy constructor
  FileDescriptorTableEntry(FileDescriptorTableEntry& fd_table_entry);

  /*
   * Destructor
   * Free all the memory used by this table.
   */
  ~FileDescriptorTableEntry();

  /*
   * Return fd table
   */
  std::map<int, FileDescriptorEntry*>& get_fd_table();

  /*
   * Add a fd entry to the fd table.
   */
  void add_fd_entry(int traced_fd, int replayed_fd, int flags);

  /*
   * Remove file descriptor entry from the table
   */
  int remove_fd_entry(int traced_fd);

  /*
   * Get replayed fd for the given traced fd
   */
  int get_fd(int traced_fd);

  /*
   * Get all fds in this process.
   */
  std::vector<int> get_all_fds();

  /*
   * Update the replayed that is mapped to traced_fd
   */
  void update_fd(int traced_fd, int replayed_fd);

  /*
   * Get fd flags for the given traced fd
   */
  int get_flags(int traced_fd);

  /*
   * Update fd flags for the given traced fd
   */
  void update_flags(int traced_fd, int flags);

  /*
   * Get string representation of this FileDescriptorTableEntry object
   */
  std::string to_string();
};

// <pid, FileDescriptorTableEntry>
typedef std::map<pid_t, FileDescriptorTableEntry*> PerPidFileDescriptorTableMap;
// <pid, umask entry>
typedef std::map<pid_t, UmaskEntry*> UmaskTable;

class ReplayerResourcesManager {
private:
  PerPidFileDescriptorTableMap fd_table_map_;
  UmaskTable umask_table_;
  SystemCallTraceReplayLogger *logger_;

public:
  // Constructor
  ReplayerResourcesManager();
  /*
   * Note:
   * 1. fds are int because fds can be negative. Ex: FDCWD == -100
   * 2. flags are also int because flags in open man page is int.
   ****************************************************************
   *        All methods assume that fds are VALID                 *
   *        Ex: close(5)  we will assume that 5 is a valid fd     *
   ****************************************************************
   */

  /*
   * This function needs to be called first before
   * using any method in this class for the following reason:
   * when sys call replayer starts, there is no fd table
   * for any process. We need to create the fd table for
   * the first process before replaying.
   */
  void initialize(SystemCallTraceReplayLogger *logger, pid_t pid, std::map<int, int>& fd_map);

  /*
   * This function will create a file descriptor entry for
   * traced fd and adds that entry to the correct fd table.
   */
  void add_fd(pid_t pid, int traced_fd, int replayed_fd, int flags);

  /*
   * This function will return replayed file descriptor that corrsponds
   * to given traced file descriptor.
   */
  int get_fd(pid_t pid, int traced_fd);

  /*
   * This function will update the replayed file descriptor
   * for the given traced_fd.
   */
  void update_fd(pid_t pid, int traced_fd, int replayed_fd);

  /*
   * This function will remove the file descriptor entry
   * from pid fd table.
   *
   * Return a fd to close.
   *
   * Future work:
   * Return a pair where the first element is true of false. True means that
   * caller needs to close the file, false means no need to close.
   * The second element is replayed fd if no error occurs.
   * <need to close, replayed fd>
   */
  int remove_fd(pid_t pid, int traced_fd);

  /*
   * This function will return flags of a file descriptor.
   */
  int get_flags(pid_t pid, int traced_fd);

  /*
   * This function will update a file descriptor flags.
   */
  void update_flags(pid_t pid, int traced_fd, int flag);

  /*
   * This function will clone file descriptor table for a process.
   * If shared is true, increment the reference count for the fd table
   * without creating a new fd table for pid. This means
   * that they both share same fd table. If shared is false,
   * a new fd table will be created and the content of new
   * fd table is same as ppid fd table.
   */
  void clone_fd_table(pid_t ppid, pid_t pid, bool shared);

  /*
   * This function will destroy a file descriptor table
   * for a process.
   * It will decrement the reference count for pid fd table
   * and remove the pid fd table entry in the map.
   * It will only destroy fd table and return
   * a list of fds for replayer to close if the reference count
   * of fd table reaches to 0.
   * Return a list of fd for replayer to close
   */
  std::vector<int> remove_fd_table(pid_t pid);

  /*
   * print_fd_manager() function will print out the
   * content of file descriptor manager. This is very useful
   * for debug.
   */
  void print_fd_manager();

  /*
   * Return umask value for a particular process
   */
  mode_t get_umask(pid_t pid);

  /*
   * Set umask value for a particular process
   */
  void set_umask(pid_t pid, mode_t mode);

  /*
   * Clone mask value for a particular process given its parent process
   * If shared is True, then two processes same mask entry, otherwise,
   * this function will create a new entry for pid.
   * pid is child process pid
   * ppid is parent process pid
   */
  void clone_umask(pid_t ppid, pid_t pid, bool shared);

  /*
   * Decrement the mask reference count for the given pid.
   * Only remove the entry if rc reaches to 0.
   */
  void remove_umask(pid_t pid);
};
#endif /* REPLAYER_RESOURCES_MANAGER_HPP */
