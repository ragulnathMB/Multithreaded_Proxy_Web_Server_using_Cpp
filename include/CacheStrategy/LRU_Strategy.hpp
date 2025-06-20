#pragma once
#include "CacheStrategy.hpp"
#include <list>
#include <unordered_map>
#include <mutex>

class LRUCacheStrategy : public CacheStrategy {
public:
    LRUCacheStrategy(size_t capacity);

    bool find(const std::string& key) override;
    std::string get(const std::string& key) override;
    void put(const std::string& key, const std::string& value) override;

private:
    size_t maxSize;
    std::list<std::string> lruList;
    std::unordered_map<std::string, std::pair<std::string, std::list<std::string>::iterator>> cache;
    std::mutex mtx;
};
