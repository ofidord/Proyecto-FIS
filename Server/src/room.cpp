#include "room.h"


void Room::join(participant_ptr participant)
{
    participants_.insert(participant);
    for(auto msg : recent_msgs_)
        participant->deliver(msg);
}

void Room::leave(participant_ptr participant)
{
    participants_.erase(participant);
}

void Room::deliver(const Message & msg, tcp::endpoint remote_endpoint)
{
    recent_msgs_.push_back(msg);
    while(recent_msgs_.size() > max_recent_msgs)
        recent_msgs_.pop_front();

    for(auto participant : participants_)
        if(participant->getDirection() != remote_endpoint)
            participant->deliver(msg);
}
