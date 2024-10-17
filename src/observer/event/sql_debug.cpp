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
// Created by Wangyunlai on 2023/6/29.
//

#include <stdarg.h>

#include "event/sql_debug.h"
#include "session/session.h"
#include "event/session_event.h"

using namespace std;

void SqlDebug::add_debug_info(const std::string &debug_info)//增加调试信息
{
  debug_infos_.push_back(debug_info);
}

void SqlDebug::clear_debug_info()
{
  debug_infos_.clear();//清空调试信息
}

const list<string> &SqlDebug::get_debug_infos() const//返回调试信息的引用
{
  return debug_infos_;
}

void sql_debug(const char *fmt, ...)
{
  Session *session  = Session::current_session();//获取当前会话
  if (nullptr == session) {
    return;
  }

  SessionEvent *request = session->current_request();//获取当前请求
  if (nullptr == request) {
    return ;
  }

  SqlDebug &sql_debug = request->sql_debug();//获取sql调试信息

  const int buffer_size = 4096;//格式化调试信息：定义一个固定大小的缓冲区（4096字节），并使用vsnprintf函数将可变参数列表fmt格式化为字符串，存储在缓冲区str中。
  char *str = new char[buffer_size];

  va_list ap;/*va_list 是一个类型，用于存储可变参数列表的信息。ap 是一个变量，用于指向当前参数的位置。*/
  va_start(ap, fmt);/*va_start 是一个宏，用于初始化 ap，使其指向可变参数列表的第一个参数。
                        fmt 是可变参数列表之前的最后一个固定参数，用于确定参数列表的开始位置*/
  vsnprintf(str, buffer_size, fmt, ap);

  /*vsnprintf 是一个函数，用于将格式化的字符串存储在指定的缓冲区中。str 是目标缓冲区的指针，
  buffer_size 是缓冲区的大小，fmt 是格式化字符串，ap 是可变参数列表。vsnprintf 会根据 fmt 和 ap 中的参数生成格式化后的字符串，
  并将其存储在 str 中。如果生成的字符串长度超过了 buffer_size，vsnprintf 会截断字符串，并在末尾添加一个空字符*/

  va_end(ap);//va_end 是一个宏，用于清理 ap，使其不再指向可变参数列表。

  sql_debug.add_debug_info(str);
  LOG_DEBUG("sql debug info: [%s]", str);//将调试信息添加到调试信息列表中，并使用LOG_DEBUG宏将调试信息打印到日志中。
  /*这行代码的工作原理是，通过宏或函数LOG_DEBUG，将格式化字符串和变量str组合起来，生成一条完整的日志信息。
  然后，这条日志信息会被记录到指定的日志文件或控制台，以便开发人员或运维人员查看和调试程序。*/
  delete[] str;
}
