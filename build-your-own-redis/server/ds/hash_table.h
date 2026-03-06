#pragma once

#include <cstddef>
#include <stddef.h>
#include <stdint.h>

// hashtable node, should be embedded into the payload
struct HNode {
  HNode *next = nullptr;
  uint64_t hcode = 0;
};

// a simple fixed-sized hashtable
struct HTab {
  size_t mask = 0; // power of 2 array size, 2^n - 1
  size_t size = 0; // number of keys
  HNode *tab[];    // array of slots

  HTab() = delete;
  ~HTab() = delete;
  HTab(const HTab &) = delete;
  HTab(HTab &&) = delete;
  HTab &operator=(const HTab &) = delete;
  HTab &operator=(HTab &&) = delete;
};

using HNodeCompare = bool (*)(HNode *, HNode *);

class HMap {
private:
  HTab *newer = nullptr;
  HTab *older = nullptr;
  size_t migrate_pos = 0;

public:
  HMap();
  ~HMap();
  HMap(const HMap &) = delete;
  HMap(const HMap &&) = delete;
  HMap &operator=(const HMap &) = delete;
  HMap &operator=(HMap &&) = delete;
  size_t size() const;
  void insert(HNode *node);
  HNode *lookup(HNode *key, HNodeCompare eq);
  HNode *remove(HNode *key, HNodeCompare eq);
  // invoke the callback on each node until it returns false
  void foreach (bool (*f)(HNode *, void *), void *arg);

private:
  void help_rehashing();
  void trigger_rehashing();
};
