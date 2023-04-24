//
// Created by aojoie on 4/24/2023.
//

#ifndef ANG_STATE_HPP
#define ANG_STATE_HPP

#include <Grammar/Grammar.hpp>
#include <unordered_set>

#include <ostream>
#include <span>
#include <set>

namespace AN::LR {

class Item {

    const grammar::Symbol               *left;
    std::vector<const grammar::Symbol *> rights;
    int                                  dot_pos;

    const grammar::Production *_production;

public:
    Item() = default;

    template<std::ranges::range _Rng>
    Item(const grammar::Symbol *left, _Rng &&rightRng, int dot, const grammar::Production *production)
        : left(left), rights(std::ranges::begin(rightRng), std::ranges::end(rightRng)),
          dot_pos(dot), _production(production) {}

    size_t hashValue() const {
        std::hash<const void *> hasher;
        size_t                  result = 0;
        result ^= hasher(left);
        for (const grammar::Symbol *sym : rights) {
            result ^= hasher((void *) sym);
        }
        result ^= std::hash<int>()(dot_pos);
        return result;
    }

    const grammar::Symbol *getLeft() const { return left; }

    std::span<grammar::Symbol const *const> getRights() const { return rights; }

    const grammar::Symbol *getNext() const { return rights[dot_pos]; }

    int getDotPos() const { return dot_pos; }

    void setDotPos(int pos) { dot_pos = pos; }

    bool hasNext() const { return dot_pos < rights.size(); }

    /// get the next val if hasNext() == true
    std::string_view getNextVal() const { return rights[dot_pos]->getVal(); }

    const grammar::Production *getProduction() const { return _production; }

    bool operator== (const Item &rhs) const {
        return left == rhs.left &&
               rights == rhs.rights &&
               dot_pos == rhs.dot_pos;
    }

    bool operator!= (const Item &rhs) const {
        return !(rhs == *this);
    }

    bool operator < (const Item &other) const {
        return _production->getID() * 10 + dot_pos < other._production->getID() * 10 + other.dot_pos;
    }
};

}// namespace AN::LR

template<>
struct std::hash<AN::LR::Item> {
    size_t operator() (const AN::LR::Item &item) const {
        return item.hashValue();
    }
};

namespace AN::LR {

class State {
    int                      id;
    std::set<Item> commonItems;
    std::set<Item> _items;/// include closure

    mutable std::unordered_map<std::string_view, int> gotoMap;

public:
    State() = default;

    explicit State(int ID) : id(ID) {}

    void addCommonItem(const Item &item) { commonItems.insert(item); }

    void mergeItems(std::set<Item> &&items) { _items.merge(items); }

    const std::set<Item> &getCommonItems() const { return commonItems; }
    const std::set<Item> &getItems() const { return _items; }

    bool hasItem(const Item &item) const { return _items.contains(item); }

    void addGoto(std::string_view val, int aId) const { gotoMap[val] = aId; }

    int gotoState(std::string_view val) const { return gotoMap[val]; }

    int getID() const { return id; }

    bool operator< (const State &other) const {
        return id < other.id;
    }

    bool operator== (const State &) const = default;

    friend std::ostream &operator<< (std::ostream &os, const State &state);
};


}// namespace AN::LR

#endif//ANG_STATE_HPP
