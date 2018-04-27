#pragma once

#include <iostream>
#include <cstring>

class Message
{
private:
    enum Pad { BODY = 0, MAX_LENGTH = 2048 };
    char data_[Pad::MAX_LENGTH];

public:

    // Constructors

    Message() = default;

    Message(const char * msg)
    {
        body(msg);
    }

    Message(const std::string & msg)
    {
        body(msg.c_str());
    }

    Message(const Message & msg)
    {
        body(msg.data_);
    }


    Message & operator=(const char * msg)
    {
        body(msg);

        return * this;
    }

    Message & operator=(const std::string & msg)
    {
        body(msg.c_str());

        return * this;
    }

    Message & operator=(const Message & msg)
    {
        body(msg.data_);

        return * this;
    }


    // Getters and Setters

    const char * body() const
    {
        return (data_ + Pad::BODY);
    }

    void body(const char * msg)
    {
        std::strcpy((data_ + Pad::BODY), msg);
    }

    char * data()
    {
        return data_;
    }

    const char * data() const
    {
        return data_;
    }


    // Utilities

    size_t max_length() const
    {
        return Pad::MAX_LENGTH;
    }


    bool empty() const
    {
        return (data_[0] == '\n') || (data_[0] == '\0');
    }


    bool compare(const char * str, size_t len)
    {
        if(len > max_length())
            return false;

        for(size_t i = 0; i < len; ++i)
            if(body()[i] != str[i])
                return false;

        return true;
    }


    // Operators

    bool operator==(const char * str) const
    {
        return (std::strcmp(body(), str) == 0);
    }


    bool operator==(const std::string & str) const
    {
        return (*this == str.c_str());
    }


    friend std::ostream & operator<<(std::ostream & os, const Message & msg)
    {
        os << msg.body() << std::endl;

        return os;
    }
};
