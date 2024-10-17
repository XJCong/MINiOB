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
// Created by Wangyunlai on 2021/5/12.
//

#include "session/session.h"
#include "storage/trx/trx.h"
#include "storage/db/db.h"
#include "storage/default/default_handler.h"
#include "common/global_context.h"

Session &Session::default_session()//返回一个静态的session对象
{
  static Session session;
  return session;
}

Session::Session(const Session &other) : db_(other.db_)//拷贝构造函数，将db_指向other的db_
{}

Session::~Session()
{
  if (nullptr != trx_) {
    GCTX.trx_kit_->destroy_trx(trx_);
    trx_ = nullptr;
  }
}

const char *Session::get_current_db_name() const//返回当前数据库的名字
{
  if (db_ != nullptr)
    return db_->name();
  else
    return "";
}

Db *Session::get_current_db() const//返回当前数据库的指针
{
  return db_;
}

void Session::set_current_db(const std::string &dbname)//设置当前数据库
{
  DefaultHandler &handler = DefaultHandler::get_default();//获取默认的handler，即当前数据库
  Db *db = handler.find_db(dbname.c_str());//从建立的数据库中查找数据库
  if (db == nullptr) {
    LOG_WARN("no such database: %s", dbname.c_str());//如果找不到，则输出警告信息
    return;
  }

  LOG_TRACE("change db to %s", dbname.c_str());//输出切换数据库的信息
  db_ = db;
}

void Session::set_trx_multi_operation_mode(bool multi_operation_mode)//设置事务多操作模式
{
  trx_multi_operation_mode_ = multi_operation_mode;
}

bool Session::is_trx_multi_operation_mode() const//返回事务多操作模式
{
  return trx_multi_operation_mode_;
}

Trx *Session::current_trx()//返回当前事务的指针
{
  if (trx_ == nullptr) {
    trx_ = GCTX.trx_kit_->create_trx(db_->clog_manager());//如果当前事务为空，则创建一个新的事务
  }
  return trx_;
}

thread_local Session *thread_session = nullptr;//定义一个线程局部变量，用于存储当前线程的session

void Session::set_current_session(Session *session)//设置当前线程的session
{
  thread_session = session;
}

Session *Session::current_session()
{
  return thread_session;//返回当前线程的session
}

void Session::set_current_request(SessionEvent *request)
{
  current_request_ = request;
}

SessionEvent *Session::current_request() const
{
  return current_request_;
}
