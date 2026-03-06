#pragma once

#include <assert.h>
#include <cstring>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>

// data types of serialized data
enum {
  TAG_NIL = 0,    // nil
  TAG_ERROR = 1,  // error code + msg
  TAG_STRING = 2, // string
  TAG_INT64 = 3,  // int64
  TAG_DOUBLE = 4, // double
  TAG_ARRAY = 5,  // array
};

class Buffer {
private:
  std::vector<uint8_t> buf;
  std::vector<char> buf1;

public:
  Buffer() = default;
  ~Buffer() = default;
  Buffer(const Buffer &) = delete;
  Buffer(Buffer &&) = delete;
  Buffer &operator=(const Buffer &) = delete;
  Buffer &operator=(Buffer &&) = delete;

  inline size_t size() const { return buf.size(); }
  inline void reserve(size_t n) { buf.reserve(n); }
  inline void resize(size_t size) { buf.resize(size); }
  inline const uint8_t *data() const { return buf.data(); }
  uint8_t operator[](size_t i) const { return buf[i]; }

  // remove from the front
  inline void consume(size_t n) { buf.erase(buf.begin(), buf.begin() + n); }
  // append to the back
  inline void write(const uint8_t *data, size_t len) {
    buf.insert(buf.end(), data, data + len);
  }
  inline void write(const std::string &s) {
    write((const uint8_t *)s.data(), s.size());
  }
  inline void write(uint8_t data) { buf.insert(buf.end(), data); }
  inline void write(uint32_t data) {
    write((const uint8_t *)&data, sizeof(uint32_t));
  }
  inline void write(int64_t data) {
    write((const uint8_t *)&data, sizeof(int64_t));
  }
  inline void write(double data) {
    write((const uint8_t *)&data, sizeof(double));
  }

  // frame
  inline void write_frame() { write((uint8_t)TAG_NIL); }
  inline void write_frame(const char *s, size_t size) {
    write((uint8_t)TAG_STRING);
    write((uint32_t)size);
    write((const uint8_t *)s, size);
  }
  inline void write_frame(const std::string &s) {
    write((uint8_t)TAG_STRING);
    write((uint32_t)s.size());
    write(s);
  }
  inline void write_frame(int64_t val) {
    write((uint8_t)TAG_INT64);
    write(val);
  }
  inline void write_frame(double val) {
    write((uint8_t)TAG_DOUBLE);
    write(val);
  }
  inline void write_frame_array(uint32_t n) {
    write((uint8_t)TAG_ARRAY);
    write(n);
  }
  inline size_t write_frame_array_begin() {
    write((uint8_t)TAG_ARRAY);
    write((uint32_t)0);    // filled by write_frame_array_end()
    return buf.size() - 4; // the position of the array size
  }
  inline void write_frame_array_end(size_t pos, uint32_t n) {
    assert(buf[pos - 1] == TAG_ARRAY);
    memcpy(&buf[pos], &n, 4);
  }
  inline void write_frame(uint32_t code, const std::string &msg) {
    write((uint8_t)TAG_ERROR);
    write(code);
    write((uint32_t)msg.size());
    write(msg);
  }
};
