#pragma once

#include <iostream>
#include <jsoncpp/json/json.h>

enum LOG { INFO = 0, WARNING = 1, ERROR = 2 };
extern std::string LOG_name[];

class bot;

/**
 * message_type: "group" or "private"
 * user_id & group_id
 * message_id
 * p
 */
struct msg_meta {
    std::string message_type;
    int64_t user_id;
    int64_t group_id;
    int64_t message_id;
    bot *p;
};

class bot {
protected:
    int receive_port, send_port;

    uint64_t botqq;

public:
    bot() = delete;
    bot(bot &bot) = delete;
    bot(bot &&bot) = delete;
    bot(int recv_port, int send_port);

    /**
     * Starter of the bot.
     * ATTENTION: run() must not quit. Otherwise the main() will call it again.
     */
    virtual void run() = 0;

    /**
     * Is this user the bot's operator?
     */
    virtual bool is_op(const int64_t a) const;

    /**
     * send(POST) to gocq
     */
    virtual std::string cq_send(const std::string &message,
                                const msg_meta &conf) const;

    /**
     * send(POST) to gocq
     */
    virtual std::string cq_send(const std::string &end_point,
                                const Json::Value &J) const;

    /**
     * GET from gocq
     */
    virtual std::string cq_get(const std::string &end_point) const;

    /**
     * Write down logs.
     */
    virtual void setlog(LOG type, std::string message);

    /**
     * get mine qq.
     */
    virtual int64_t get_botqq() const;

    /**
     * receive a message, how to process
     */
    virtual void input_process(std::string *input) = 0;

    virtual ~bot();
};