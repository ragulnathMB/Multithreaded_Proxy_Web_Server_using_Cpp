#include "../../include/CacheStrategy/LFU_Strategy.hpp"
#include <iostream>

LFUCacheStrategy::LFUCacheStrategy(size_t capacity)
    : maxSize(capacity), timeCounter(0) {}

// Check if key exists
bool LFUCacheStrategy::find(const std::string& key) {
    std::lock_guard<std::mutex> lock(mtx);
    return cacheMap.find(key) != cacheMap.end();
}

// Get the value associated with a key
std::string LFUCacheStrategy::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(mtx);

    auto it = cacheMap.find(key);
    if (it == cacheMap.end()) {
        return "";
    }

    // Remove from frequency set before updating
    frequencySet.erase(it->second);

    // Update frequency and timestamp
    it->second.frequency++;
    it->second.timestamp = ++timeCounter;

    // Reinsert with updated values
    frequencySet.insert(it->second);

    return it->second.value;
}

// Put a new key-value pair into cache
void LFUCacheStrategy::put(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(mtx);

    auto it = cacheMap.find(key);

    if (it != cacheMap.end()) {
        // Update existing
        frequencySet.erase(it->second);
        it->second.value = value;
        it->second.frequency++;
        it->second.timestamp = ++timeCounter;
        frequencySet.insert(it->second);
    } else {
        // Check capacity
        if (cacheMap.size() >= maxSize) {
            // Evict the least frequently used
            auto evict = frequencySet.begin();
            if (evict != frequencySet.end()) {
                cacheMap.erase(evict->key);
                frequencySet.erase(evict);
            }
        }

        // Insert new entry
        CacheEntry newEntry = {key, value, 1, ++timeCounter};
        cacheMap[key] = newEntry;
        frequencySet.insert(newEntry);
    }
}
