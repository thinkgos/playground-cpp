#pragma once

#include <cstddef>
#include <stddef.h>
#include <stdint.h>

// hashtable node, should be embedded into the payload
struct HashNode {
  HashNode *next = nullptr; // next node in the chain
  uint64_t hash_code = 0;   // hash code of the key, for quick comparison
};

using HashNodeCompare = bool (*)(HashNode *, HashNode *);

struct HashTab {
  size_t mask = 0; // power of 2 array size, 2^n - 1
  size_t size = 0; // the number of keys
  HashNode *tab[]; // flexible array of slots

  HashTab() = delete;
  ~HashTab() = delete;
  HashTab(const HashTab &) = delete;
  HashTab(HashTab &&) = delete;
  HashTab &operator=(const HashTab &) = delete;
  HashTab &operator=(HashTab &&) = delete;
};

class HashMap {
private:
  HashTab *newer = nullptr;
  HashTab *older = nullptr;
  size_t migrate_pos = 0;

public:
  HashMap();
  ~HashMap();
  HashMap(const HashMap &) = delete;
  HashMap(const HashMap &&) = delete;
  HashMap &operator=(const HashMap &) = delete;
  HashMap &operator=(HashMap &&) = delete;
  size_t size() const;
  void insert(HashNode *node);
  HashNode *lookup(HashNode *key, HashNodeCompare eq);
  HashNode *remove(HashNode *key, HashNodeCompare eq);
  // invoke the callback on each node until it returns false
  void foreach (bool (*f)(HashNode *, void *), void *arg);

private:
  void inspect_rehashing();
  void rehashing();
};
