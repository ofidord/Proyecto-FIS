#pragma once

#include <memory>

#include <boost/asio.hpp>

#include "message.h"

using boost::asio::ip::tcp;


class Participant
{
public:
    virtual ~Participant() = default;
    virtual void deliver(const Message & msg) = 0;

    virtual tcp::endpoint getDirection() const = 0;
};

typedef std::shared_ptr<Participant> participant_ptr;
