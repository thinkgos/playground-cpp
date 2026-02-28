#pragma once

#include <stddef.h>
#include <list>

struct ListNode
{
    ListNode *prev = NULL;
    ListNode *next = NULL;

    ListNode() : prev(this), next(this) {}
    // 禁止拷贝构造函数
    ListNode(const ListNode &) = delete;
    // 禁止赋值运算符
    ListNode &operator=(const ListNode &) = delete;

    inline bool empty()
    {
        return this->next == this;
    }
    inline void detach()
    {
        this->prev->next = this->next;
        this->next->prev = this->prev;
        this->prev = this;
        this->next = this;
    }
    inline void insert_back(ListNode *rookie)
    {
        rookie->prev = this->prev;
        rookie->next = this;
        this->prev->next = rookie;
        this->prev = rookie;
    }
};
