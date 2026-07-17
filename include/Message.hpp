#pragma once

class Message
{
    private:
        std::string prefix;
        std::string command;
        std::vector<std::string> params;
    public:
        Message();

        const std::string &getPrefix() const;
        const std::string &getCommand() const ;
        const std::vector<std::string> &getParams() const;
        const std::string &getParameter(size_t index) const;

        void setPrefix(const std::string &p);
        void setCommand(const std::string &c);
        void setParams(const std::vector<std::string> &p);
        void pushBackParam(const std::string &param);

        void printMessage() const;
};
