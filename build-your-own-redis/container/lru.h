#include <cstddef>
#include <list>
#include <optional>
#include <unordered_map>
#include <utility>

template <typename K, typename V> class lru {
private:
  std::list<std::pair<const K, V>> m_cache;
  std::unordered_map<K, typename std::list<std::pair<const K, V>>::iterator>
      m_map;
  size_t m_capacity;

public:
  using iterator = typename std::list<std::pair<const K, V>>::iterator;
  using const_iterator =
      typename std::list<std::pair<const K, V>>::const_iterator;
  using reverse_iterator =
      typename std::list<std::pair<const K, V>>::reverse_iterator;
  using const_reverse_iterator =
      typename std::list<std::pair<const K, V>>::const_reverse_iterator;

  iterator begin() noexcept { return m_cache.begin(); }
  iterator end() noexcept { return m_cache.end(); }
  const_iterator begin() const noexcept { return m_cache.begin(); }
  const_iterator end() const noexcept { return m_cache.end(); }
  const_iterator cbegin() const noexcept { return m_cache.cbegin(); }
  const_iterator cend() const noexcept { return m_cache.cend(); }
  reverse_iterator rbegin() noexcept { return m_cache.rbegin(); }
  reverse_iterator rend() noexcept { return m_cache.rend(); }
  const_reverse_iterator rbegin() const noexcept { return m_cache.rbegin(); }
  const_reverse_iterator rend() const noexcept { return m_cache.rend(); }

  lru(size_t capacity) : m_capacity(capacity) {}
  size_t size() const { return m_cache.size(); }
  std::optional<V> get(K key) {
    auto it = this->m_map.find(key);
    if (it == this->m_map.end()) {
      return std::nullopt;
    } else {
      // 已找到, 移到最头
      this->m_cache.splice(this->m_cache.begin(), this->m_cache, it->second);
      return it->second->second;
    }
  }

  void put(K key, V value) {
    auto it = this->m_map.find(key);
    if (it == this->m_map.end()) {
      // 容量满, 先淘汰最尾元素.
      if (this->m_cache.size() == this->m_capacity) {
        auto last = this->m_cache.back();
        this->m_map.erase(last.first);
        this->m_cache.pop_back();
      }
      this->m_cache.emplace_front(key, value);
      this->m_map[key] = this->m_cache.begin();
    } else {
      // 已找到, 更新值, 并移到最头
      it->second->second = value;
      this->m_cache.splice(this->m_cache.begin(), this->m_cache,
                           this->m_map[key]);
    }
  }
};