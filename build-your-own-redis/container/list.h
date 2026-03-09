#pragma once

#include <stddef.h>

struct ListNode {
  ListNode *prev = this;
  ListNode *next = this;

  ListNode() = default;
  ListNode(const ListNode &) = delete;
  ListNode &operator=(const ListNode &) = delete;
  ListNode(ListNode &&) noexcept = delete;
  ListNode &operator=(ListNode &&) noexcept = delete;

  inline bool empty() const { return this->next == this; }
  inline void detach() {
    this->prev->next = this->next;
    this->next->prev = this->prev;
    this->prev = this;
    this->next = this;
  }
  inline void insert_back(ListNode *rookie) {
    rookie->prev = this->prev;
    rookie->next = this;
    this->prev->next = rookie;
    this->prev = rookie;
  }
};
