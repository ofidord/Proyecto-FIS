#pragma once

#include <algorithm>
#include <iostream>
#include <cstring>

class Message
{
private:
    static const size_t BODY_LENGTH_ = 2048;

    char body_[BODY_LENGTH_];

public:

    Message(const std::string & msg = std::string())
    {
        std::strcpy(body_, msg.c_str());
    }


    Message & operator=(const std::string & msg)
    {
        std::strcpy(body_, msg.c_str());

        return * this;
    }


    const char * data() const
    {
        return body_;
    }


    char * data()
    {
        return body_;
    }


    const size_t length() const
    {
        return BODY_LENGTH_;
    }



   friend std::ostream & operator<<(std::ostream & os, const Message & m)
   {
       os << m.body_ << std::endl;
       return os;
   }

};
