#include "../../include/CacheStrategy/LRU_Strategy.hpp"

LRUCacheStrategy::LRUCacheStrategy(size_t capacity)
    : maxSize(capacity) {}

bool LRUCacheStrategy::find(const std::string& key) {
    std::lock_guard<std::mutex> lock(mtx);
    return cache.find(key) != cache.end();
}

std::string LRUCacheStrategy::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(mtx);

    auto it = cache.find(key);
    if (it == cache.end()) {
        return "";
    }

    // Move the accessed key to the front (most recently used)
    lruList.erase(it->second.second);
    lruList.push_front(key);
    it->second.second = lruList.begin();

    return it->second.first;  // return the value
}

void LRUCacheStrategy::put(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(mtx);

    auto it = cache.find(key);
    if (it != cache.end()) {
        // Key already exists, update value and move to front
        lruList.erase(it->second.second);
        lruList.push_front(key);
        it->second = { value, lruList.begin() };
    } else {
        // Evict if over capacity
        if (cache.size() >= maxSize) {
            std::string leastUsed = lruList.back();
            lruList.pop_back();
            cache.erase(leastUsed);
        }

        // Insert new
        lruList.push_front(key);
        cache[key] = { value, lruList.begin() };
    }
}
