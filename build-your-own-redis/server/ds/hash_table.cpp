#include "hash_table.h"
#include <assert.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdlib.h>

// n must be a power of 2
static HashTab *ht_make(size_t n) {
  assert(n > 0 && ((n - 1) & n) == 0);
  HashTab *ht = (HashTab *)malloc(sizeof(HashTab) + n * sizeof(HashNode *));
  ht->mask = n - 1;
  ht->size = 0;
  memset(ht->tab, 0, n * sizeof(HashNode *));
  return ht;
}

static void ht_free(HashTab *ht) { free(ht); }

// hashtable insertion
static void ht_insert(HashTab *ht, HashNode *node) {
  size_t pos = node->hash_code & ht->mask;
  HashNode *next = ht->tab[pos];

  node->next = next;
  ht->tab[pos] = node;
  ht->size++;
}

// hashtable look up subroutine.
// Pay attention to the return value. It returns the address of
// the parent pointer that owns the target node,
// which can be used to delete the target node.
static HashNode **ht_lookup(HashTab *ht, HashNode *key,
                            bool (*eq)(HashNode *, HashNode *)) {
  size_t pos = key->hash_code & ht->mask;
  HashNode **from = &ht->tab[pos]; // incoming pointer to the target

  for (HashNode *cur; (cur = *from) != nullptr; from = &cur->next) {
    if (cur->hash_code == key->hash_code && eq(cur, key)) {
      return from; // may be a node, may be a slot
    }
  }
  return nullptr;
}

// remove a node from the chain
static HashNode *ht_detach(HashTab *ht, HashNode **from) {
  HashNode *node = *from; // the target node

  *from = node->next; // update the incoming pointer to the target
  ht->size--;
  return node;
}

static bool ht_foreach(HashTab *ht, bool (*f)(HashNode *, void *), void *arg) {
  for (size_t i = 0; ht->mask != 0 && i <= ht->mask; i++) {
    for (HashNode *node = ht->tab[i]; node != nullptr; node = node->next) {
      if (!f(node, arg)) {
        return false;
      }
    }
  }
  return true;
}

const size_t k_rehashing_work = 128;
const size_t k_max_load_factor = 8;

HashMap::HashMap() : newer(ht_make(4)) {}

HashMap::~HashMap() {
  free(this->newer);
  this->newer = nullptr;
  if (this->older) {
    free(this->older);
    this->older = nullptr;
  }
  this->migrate_pos = 0;
}

size_t HashMap::size() const {
  return this->newer->size + (this->older ? this->older->size : 0);
}

void HashMap::insert(HashNode *node) {
  ht_insert(this->newer, node); // always insert to the newer table
  this->inspect_rehashing();    // trigger rehashing if needed
  this->rehashing();            // migrate some keys
}

HashNode *HashMap::lookup(HashNode *key, HashNodeCompare eq) {
  this->rehashing();

  HashNode **from = ht_lookup(this->newer, key, eq);
  if (!from && this->older) {
    from = ht_lookup(this->older, key, eq);
  }
  return from ? *from : nullptr;
}

HashNode *HashMap::remove(HashNode *key, bool (*eq)(HashNode *, HashNode *)) {
  this->rehashing();
  if (HashNode **from = ht_lookup(this->newer, key, eq); from) {
    return ht_detach(this->newer, from);
  }
  if (this->older) {
    if (HashNode **from = ht_lookup(this->older, key, eq); from) {
      return ht_detach(this->older, from);
    }
  }
  return nullptr;
}

void HashMap::foreach (bool (*f)(HashNode *, void *), void *arg) {
  ht_foreach(this->newer, f, arg);
  if (this->older) {
    ht_foreach(this->older, f, arg);
  }
}

void HashMap::inspect_rehashing() {
  if (!this->older) {
    size_t shreshold = (this->newer->mask + 1) * k_max_load_factor;
    if (this->newer->size >= shreshold) {
      this->older = this->newer;
      this->newer = ht_make((this->newer->mask + 1) * 2);
      this->migrate_pos = 0;
    }
  }
}

void HashMap::rehashing() {
  if (!this->older) {
    return; // no rehashing
  }
  for (size_t nwork = 0; nwork < k_rehashing_work && this->older->size > 0;) {
    // find a non-empty slot
    HashNode **from = &this->older->tab[this->migrate_pos];
    if (!*from) {
      this->migrate_pos++;
      continue; // empty slot
    }
    // move the first list item to the newer table
    ht_insert(this->newer, ht_detach(this->older, from));
    nwork++;
  }
  // discard the old table if done
  if (this->older->size == 0) {
    ht_free(this->older);
    this->older = nullptr;
    this->migrate_pos = 0;
  }
}
