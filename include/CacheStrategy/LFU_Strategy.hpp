#pragma once
#include "CacheStrategy.hpp"
#include <unordered_map>
#include <set>
#include <mutex>

class LFUCacheStrategy : public CacheStrategy {
public:
    LFUCacheStrategy(size_t capacity);

    bool find(const std::string& key) override;
    std::string get(const std::string& key) override;
    void put(const std::string& key, const std::string& value) override;

private:
    struct CacheEntry {
        std::string key;
        std::string value;
        int frequency;
        size_t timestamp;

        bool operator<(const CacheEntry& other) const {
            return frequency == other.frequency ?
                   timestamp < other.timestamp : frequency < other.frequency;
        }
    };

    size_t maxSize;
    size_t timeCounter;
    std::unordered_map<std::string, CacheEntry> cacheMap;
    std::set<CacheEntry> frequencySet;
    std::mutex mtx;
};
