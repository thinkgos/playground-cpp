#pragma once

#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <string>
#include <memory>
#include <cstring>
#include <assert.h>

// data types of serialized data
enum
{
    TAG_NIL = 0, // nil
    TAG_ERR = 1, // error code + msg
    TAG_STR = 2, // string
    TAG_INT = 3, // int64
    TAG_DBL = 4, // double
    TAG_ARR = 5, // array
};

class Buffer
{
private:
    std::vector<uint8_t> buf;

public:
    Buffer() = default;
    ~Buffer() = default;

    inline size_t size() const
    {
        return buf.size();
    }
    inline void resize(size_t size)
    {
        buf.resize(size);
    }
    inline const uint8_t *data() const
    {
        return buf.data();
    }
    uint8_t operator[](size_t i)
    {
        return buf[i];
    }

    // remove from the front
    inline void consume(size_t n)
    {
        buf.erase(buf.begin(), buf.begin() + n);
    }
    // append to the back
    inline void push_back(const uint8_t *data, size_t len)
    {
        buf.insert(buf.end(), data, data + len);
    }
    inline void push_back(uint8_t data)
    {
        buf.push_back(data);
    }
    inline void push_back(uint32_t data)
    {
        push_back((const uint8_t *)&data, 4);
    }
    inline void push_back(int64_t data)
    {
        push_back((const uint8_t *)&data, 8);
    }
    inline void push_back(double data)
    {
        push_back((const uint8_t *)&data, 8);
    }

    // append serialized data types to the back
    inline void out_nil()
    {
        push_back((uint8_t)TAG_NIL);
    }
    inline void out_str(const char *s, size_t size)
    {
        push_back((uint8_t)TAG_STR);
        push_back((uint32_t)size);
        push_back((const uint8_t *)s, size);
    }
    inline void out_int(int64_t val)
    {
        push_back((uint8_t)TAG_INT);
        push_back(val);
    }
    inline void out_dbl(double val)
    {
        push_back((uint8_t)TAG_DBL);
        push_back(val);
    }

    inline void out_err(uint32_t code, const std::string &msg)
    {
        push_back((uint8_t)TAG_ERR);
        push_back(code);
        push_back((uint32_t)msg.size());
        push_back((const uint8_t *)msg.data(), msg.size());
    }
    inline void out_arr(uint32_t n)
    {
        push_back((uint8_t)TAG_ARR);
        push_back(n);
    }
    inline size_t out_begin_arr()
    {
        push_back((uint8_t)TAG_ARR);
        push_back((uint32_t)0); // filled by out_end_arr()
        return buf.size() - 4;  // the `ctx` arg
    }
    inline void out_end_arr(size_t ctx, uint32_t n)
    {
        assert(buf[ctx - 1] == TAG_ARR);
        memcpy(&buf[ctx], &n, 4);
    }
};
