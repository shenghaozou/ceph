// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2004-2006 Sage Weil <sage@newdream.net>
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software 
 * Foundation.  See file COPYING.
 * 
 */

//class C_MDC_Recover;
//
#ifndef RECOVERY_QUEUE_H
#define RECOVERY_QUEUE_H

#include <set>

#include "osdc/Filer.h"

class CInode;
class MDSRank;
class PerfCounters;

class RecoveryQueue {
public:
  void enqueue(CInode *in);
  void advance();
  void prioritize(CInode *in);   ///< do this inode now/soon
  explicit RecoveryQueue(MDSRank *mds_);

  void set_logger(PerfCounters *p) {logger=p;}

private:
  void _start(CInode *in);  ///< start recovering this file
  void _recovered(CInode *in, int r, uint64_t size, utime_t mtime);

  size_t file_recover_queue_size = 0;
  size_t file_recover_queue_front_size = 0;

  elist<CInode*> file_recover_queue;   ///< the queue
  elist<CInode*> file_recover_queue_front;  ///< elevated priority items
  std::set<CInode*> file_recovering;

  MDSRank *mds;
  PerfCounters *logger;
  Filer filer;

  friend class C_MDC_Recover;
};

#endif // RECOVERY_QUEUE_H
