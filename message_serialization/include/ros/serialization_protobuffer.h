/*
 * Copyright (C) 2009, Willow Garage, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the names of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ROSCPP_SERIALIZATION_PROTOBUFFER_H
#define ROSCPP_SERIALIZATION_PROTOBUFFER_H

#include <google/protobuf/message.h>

#include "serialization.h"

namespace ros {
namespace serialization {
// protobuffer serialization
template <typename T>
struct Serializer<T, typename std::enable_if<std::is_base_of<
                         ::google::protobuf::Message, T>::value>::type> {
  template <typename Stream>
  inline static void write(Stream &stream, const T &t) {
/*     static auto total_duration = std::chrono::microseconds(0);
    static int count = 1;
    auto start = std::chrono::high_resolution_clock::now();
    std::string pb_str;
    t.SerializeToString(&pb_str);
    // 4个字节
    uint32_t len = (uint32_t)pb_str.size();
    stream.next(len);

    if (len > 0) {
      memcpy(stream.advance((uint32_t)len), pb_str.data(), len);
    }
    // std::cout << "pb_str" << std::endl;
    // stream.next(pb_str);
    auto end = std::chrono::high_resolution_clock::now();
    total_duration += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Serialization took: " << total_duration.count()/count << " microseconds" << std::endl;
    std::cout << "serial count:" << count << std::endl;
    count++;  */

    // 优化2  : 优化效率 计算
    // advance 是预留空间 ​​返回移动前的指针位置​​（即写入数据的起始地址）, data_指针移动到末尾
#if 0
    static auto total_duration = std::chrono::microseconds(0);
    static int count = 1;
    auto start = std::chrono::high_resolution_clock::now();
    uint32_t len_data = t.ByteSizeLong();
    // 向数据中写入头部的4字节数据
    stream.next(len_data);
    if(!t.SerializeToArray(stream.advance(len_data), len_data)) {
      throw std::runtime_error("Serialization failed");
    }
    auto end = std::chrono::high_resolution_clock::now();
    total_duration += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Serialization took: " << total_duration.count()/count << " microseconds" << std::endl;
    std::cout << "serial count:" << count << std::endl;
    count++; 
#else
      uint32_t len_data = t.ByteSizeLong();
    // 向数据中写入头部的4字节数据
    stream.next(len_data);
    if(!t.SerializeToArray(stream.advance(len_data), len_data)) {
      throw std::runtime_error("Serialization failed");
    }
    std::cout << "len: " << len_data << std::endl;
    // std::cout << "pb:"<<(stream.getData() - len_data)<< std::endl;
#endif
    
  }

  // ros反序列化的接口
  template <typename Stream>
  inline static void read(Stream &stream, T &t) {

/*      uint32_t len;
    // IStream
    stream.next(len);
    // std::cout << "len: " << len << std::endl;

    std::string pb_str;
    if (len > 0) {
      const char *data_ptr =
          reinterpret_cast<const char *>(stream.advance(len));

      pb_str = std::string(data_ptr, len);
    } else {
      pb_str.clear();
    }
    // stream.next(pb_str);
    t.ParseFromString(pb_str);  */


    // 优化3 将字节流数组反序列化到proto数据块中
    uint32_t len_data;
    stream.next(len_data);  // ISream 将前4个字节写入到 len_data变量
    const uint8_t *data_ptr =
        reinterpret_cast<const uint8_t *>(stream.advance(len_data));
    if(!t.ParseFromArray(data_ptr, len_data)){
      throw std::runtime_error("Parse failed"); 
    } 
  }

  inline static uint32_t serializedLength(const T &t) {
    // std::string pb_str;
    // t.SerializeToString(&pb_str);
    // return 4 + (uint32_t)pb_str.size();
    // 优化 1
    uint32_t len = 4 + t.ByteSizeLong();
    std::cout << "len: " << len << std::endl;
    return len;
  }
};

}  // namespace serialization
}  // namespace ros

#endif  // ROSCPP_SERIALIZATION_H
