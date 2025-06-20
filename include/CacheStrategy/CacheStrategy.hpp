#pragma once
#include<string>

class CacheStrategy{
    public:
    virtual ~CacheStrategy()=default;
    virtual std::string get(const std::string& key)=0;
    virtual void put(const std::string& key, const std::string& val)=0;
    virtual bool find(const std::string& key)=0;
};