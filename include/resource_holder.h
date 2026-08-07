#pragma once

#include <map>
#include <memory>
#include <string>
#include <stdexcept>
#include <cassert>

template<typename Resource, typename Identifier>
class ResourceHolder {
public:
    void Load(Identifier id, const std::string& filename) {
        auto res = std::make_unique<Resource>();
        if (!res->loadFromFile(filename)) {
            throw std::runtime_error("Failed to load: " + filename);
        }
        auto [it, ok] = resources_.emplace(id, std::move(res));
        assert(ok);
    }

    template<typename Param>
    void Load(Identifier id, const std::string& filename, const Param& extra) {
        auto res = std::make_unique<Resource>();
        if (!res->loadFromFile(filename, extra)) {
            throw std::runtime_error("Failed to load: " + filename);
        }
        auto [it, ok] = resources_.emplace(id, std::move(res));
        assert(ok);
    }

    Resource& Get(Identifier id) {
        auto it = resources_.find(id);
        assert(it != resources_.end());
        return *it->second;
    }

    const Resource& Get(Identifier id) const {
        auto it = resources_.find(id);
        assert(it != resources_.end());
        return *it->second;
    }

private:
    std::map<Identifier, std::unique_ptr<Resource>> resources_;
};