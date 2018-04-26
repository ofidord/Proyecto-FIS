#pragma once

#include <memory>

#include "message.h"


class Participant
{
public:
    virtual ~Participant() = default;
    virtual void deliver(const Message & msg) = 0;
};

typedef std::shared_ptr<Participant> participant_ptr;
