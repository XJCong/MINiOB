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
// Created by Longda on 2021/4/14.
//

#pragma once

#include <string>
#include <memory>
#include "common/seda/stage_event.h"
#include "sql/operator/physical_operator.h"

class SessionEvent;//会话事件
class Stmt;//语句
class ParsedSqlNode;//解析后的SQL节点

/**
 * @brief 与SessionEvent类似，也是处理SQL请求的事件，只是用在SQL的不同阶段
 */
class SQLStageEvent : public common::StageEvent
{
public:
  SQLStageEvent(SessionEvent *event, const std::string &sql);//构造函数
  virtual ~SQLStageEvent() noexcept;    //析构函数，表示不会抛出异常。

  SessionEvent *session_event() const//获取会话事件
  {
    return session_event_;
  }

  const std::string &sql() const//获取SQL语句
  {
    return sql_;
  }
  const std::unique_ptr<ParsedSqlNode> &sql_node() const//获取解析后的SQL节点
  {
    return sql_node_;
  }
  Stmt *stmt() const//返回指向解析后的数据结构的指针。
  {
    return stmt_;
  }
  std::unique_ptr<PhysicalOperator> &physical_operator()//获取生成的执行计划
  {
    return operator_;
  }
  const std::unique_ptr<PhysicalOperator> &physical_operator() const//获取生成的执行计划
  {
    return operator_;
  }

  void set_sql(const char *sql)//设置SQL语句
  {
    sql_ = sql;
  }
  void set_sql_node(std::unique_ptr<ParsedSqlNode> sql_node)//设置解析后的SQL节点
  {
    sql_node_ = std::move(sql_node);//将sql_node移动到sql_node_中，并释放sql_node的资源。
  }
  void set_stmt(Stmt *stmt)//设置解析后的数据结构
  {
    stmt_ = stmt;
  }
  void set_operator(std::unique_ptr<PhysicalOperator> oper)//设置生成的执行计划
  {
    operator_ = std::move(oper);//将oper移动到operator_中，并释放oper的资源。
  }
private:
  SessionEvent *session_event_ = nullptr;// 会话事件，SQLStageEvent是会话事件的一个子事件，指向 SessionEvent 的指针。
  std::string sql_;  ///< 处理的SQL语句 存储SQL语句字符串
  std::unique_ptr<ParsedSqlNode> sql_node_;  ///< 语法解析后的SQL命令
  Stmt *stmt_ = nullptr;  ///< Resolver之后生成的数据结构，
  std::unique_ptr<PhysicalOperator> operator_; ///< 生成的执行计划，也可能没有

};
