#pragma once

#include <stddef.h>

struct DList
{
    DList *prev = NULL;
    DList *next = NULL;
    DList()
    {
        this->prev = this;
        this->next = this;
    }
    ~DList() = default;

    inline bool empty()
    {
        return this->next == this;
    }
    inline void detach()
    {
        DList *prev = this->prev;
        DList *next = this->next;

        this->prev->next = next;
        this->next->prev = prev;
        this->prev = this;
        this->next = this;
    }
    inline void insert_back(DList *rookie)
    {
        DList *prev = this->prev;

        prev->next = rookie;
        rookie->prev = prev;
        rookie->next = this;
        this->prev = rookie;
    }
};
