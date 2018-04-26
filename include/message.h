#pragma once

#include <iostream>
#include <cstring>

class Message
{
private:
    enum Pad { MESSAGE = 0, MAX_LENGTH = 2048 };
    char data_[Pad::MAX_LENGTH];

public:

    Message() = default;

    const char * getMessage() const
    {
        return (data_ + Pad::MESSAGE);
    }

    void setMessage(const char * message)
    {
        std::strcpy((data_ + Pad::MESSAGE), message);
    }

    char * data()
    {
        return data_;
    }

    const char * data() const
    {
        return data_;
    }

    size_t max_length() const
    {
        return Pad::MAX_LENGTH;
    }


    friend std::ostream & operator<<(std::ostream & os, const Message & msg)
    {
        os << msg.getMessage() << std::endl;

        return os;
    }
};
