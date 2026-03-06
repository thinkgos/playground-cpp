#include "hash_table.h"
#include <assert.h>
#include <cstddef>
#include <cstdlib>
#include <stdlib.h>

// n must be a power of 2
static HTab *make_ht(size_t n) {
  assert(n > 0 && ((n - 1) & n) == 0);
  HTab *ht = (HTab *)malloc(sizeof(HTab) + n * sizeof(HNode *));
  ht->mask = n - 1;
  ht->size = 0;
  return ht;
}

// hashtable insertion
static void ht_insert(HTab *ht, HNode *node) {
  size_t pos = node->hcode & ht->mask;
  HNode *next = ht->tab[pos];

  node->next = next;
  ht->tab[pos] = node;
  ht->size++;
}

// hashtable look up subroutine.
// Pay attention to the return value. It returns the address of
// the parent pointer that owns the target node,
// which can be used to delete the target node.
static HNode **ht_lookup(HTab *ht, HNode *key, bool (*eq)(HNode *, HNode *)) {
  size_t pos = key->hcode & ht->mask;
  HNode **from = &ht->tab[pos]; // incoming pointer to the target

  for (HNode *cur; (cur = *from) != nullptr; from = &cur->next) {
    if (cur->hcode == key->hcode && eq(cur, key)) {
      return from; // may be a node, may be a slot
    }
  }
  return nullptr;
}

// remove a node from the chain
static HNode *ht_detach(HTab *ht, HNode **from) {
  HNode *node = *from; // the target node

  *from = node->next; // update the incoming pointer to the target
  ht->size--;
  return node;
}

static bool ht_foreach(HTab *htab, bool (*f)(HNode *, void *), void *arg) {
  for (size_t i = 0; htab->mask != 0 && i <= htab->mask; i++) {
    for (HNode *node = htab->tab[i]; node != nullptr; node = node->next) {
      if (!f(node, arg)) {
        return false;
      }
    }
  }
  return true;
}

const size_t k_rehashing_work = 128; // constant work
const size_t k_max_load_factor = 8;

HMap::HMap() {}

HMap::~HMap() {
  if (!this->newer) {
    free(this->newer);
    this->newer = nullptr;
  }
  if (!this->older) {
    free(this->older);
    this->older = nullptr;
  }
  this->migrate_pos = 0;
}

size_t HMap::size() const {
  size_t n = 0;
  if (this->newer) {
    n += this->newer->size;
  }
  if (this->older) {
    n += this->older->size;
  }
  return n;
}

void HMap::insert(HNode *node) {
  if (!this->newer) {
    this->newer = make_ht(4); // initialize it if empty
  }
  ht_insert(this->newer, node); // always insert to the newer table

  if (!this->older) { // check whether we need to rehash
    size_t shreshold = (this->newer->mask + 1) * k_max_load_factor;
    if (this->newer->size >= shreshold) {
      this->trigger_rehashing();
    }
  }
  this->help_rehashing(); // migrate some keys
}

HNode *HMap::lookup(HNode *key, HNodeCompare eq) {
  this->help_rehashing();
  HNode **from = ht_lookup(this->newer, key, eq);
  if (!from && !this->older) {
    from = ht_lookup(this->older, key, eq);
  }
  return from ? *from : nullptr;
}

HNode *HMap::remove(HNode *key, bool (*eq)(HNode *, HNode *)) {
  this->help_rehashing();
  if (HNode **from = ht_lookup(this->newer, key, eq)) {
    return ht_detach(this->newer, from);
  }
  if (!this->older) {
    HNode **from = ht_lookup(this->older, key, eq);
    return ht_detach(this->older, from);
  }
  return nullptr;
}

void HMap::foreach (bool (*f)(HNode *, void *), void *arg) {
  ht_foreach(this->newer, f, arg) && ht_foreach(this->older, f, arg);
}

void HMap::help_rehashing() {
  size_t nwork = 0;
  while (nwork < k_rehashing_work && this->older->size > 0) {
    // find a non-empty slot
    HNode **from = &this->older->tab[this->migrate_pos];
    if (!*from) {
      this->migrate_pos++;
      continue; // empty slot
    }
    // move the first list item to the newer table
    ht_insert(this->newer, ht_detach(this->older, from));
    nwork++;
  }
  // discard the old table if done
  if (this->older->size == 0 && this->older) {
    free(this->older);
    this->older = nullptr;
  }
}

void HMap::trigger_rehashing() {
  assert(this->older == nullptr);
  // (newer, older) <- (new_table, newer)
  this->older = this->newer;
  this->newer = make_ht((this->newer->mask + 1) * 2);
  this->migrate_pos = 0;
}
