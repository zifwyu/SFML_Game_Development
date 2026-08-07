#pragma once

#include "entity.h"

class Aircraft : public Entity {
public:
    enum class Type {
        kEagle,
        kRaptor,
    };

public:
    explicit Aircraft(Type type);

private:
    Type type_;
};