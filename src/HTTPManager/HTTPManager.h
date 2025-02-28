#ifndef HTTP_MANAGER_H
#define HTTP_MANAGER_H

#include <string>

class HTTPManager
{
public:
    HTTPManager();
    ~HTTPManager();
    void sendDisplayData(const std::string &data);
    void sendLightCommand(bool on);

private:
};

#endif // HTTP_MANAGER_H
