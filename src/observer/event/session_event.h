/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Longda on 2021/4/13.
//

#pragma once

#include <string.h>
#include <string>

#include "common/seda/stage_event.h"
#include "sql/executor/sql_result.h"
#include "event/sql_debug.h"

class Session;
class Communicator;

/**
 * @brief 表示一个SQL请求
 * 
 */
class SessionEvent : public common::StageEvent 
{
public:
  SessionEvent(Communicator *client);//构造函数 接受客户端通讯对象
  virtual ~SessionEvent();

  Communicator *get_communicator() const;//获取客户端通讯对象
  Session *session() const;//获取会话

  void set_query(const std::string &query) { query_ = query; }//设置SQL语句

  const std::string &query() const { return query_; }//获取SQL语句

  SqlResult *sql_result() { return &sql_result_; }//获取SQL执行结果

  SqlDebug &sql_debug() { return sql_debug_; }//获取SQL调试信息

private:
  Communicator *communicator_ = nullptr;  ///< 与客户端通讯的对象
  SqlResult     sql_result_;              ///< SQL执行结果
  SqlDebug      sql_debug_;               ///< SQL调试信息
  std::string   query_;                   ///< SQL语句
};
