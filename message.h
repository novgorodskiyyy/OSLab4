#ifndef MESSAGE_H
#define MESSAGE_H

#define MAX_MSG_LEN 20
#define MAX_SENDERS 10

struct Message {
    int sender_id;
    char text[MAX_MSG_LEN + 1];
    bool active;
};

#endif