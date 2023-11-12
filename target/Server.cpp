#include <cstdio>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <iostream>
#include <thread>
#include <algorithm>

#include "Server.hpp"


#define MAX_LEN_BUFFER 512

Server::Server(const unsigned int port, const int queueLength) :
    mServerSocket(-1),
    mClientSocket(-1),
    mStateRun(false),
    mStateReady(false),
    mStateConnected(false),
    mStateReceiverEnable(false),
    mStateReceiverRun(false),
    mPort(port),
    mQueueLength(queueLength)
{
}

Server::~Server()
{
    this->closeServer();
}

bool Server::start()
{
    if (!mStateRun.load()) {
        std::cout << "Running server" << std::endl;
        mThdServer = std::thread(&Server::run, this);
        mThdServer.detach();
        mStateRun.store(true);
        return true;
    }

    std::cout << "Server already running" << std::endl;
    return false;
}

void Server::run()
{
    while (true) {
        if (!mStateReady.load() && !mStateConnected.load()) {
            this->initializeSocket();
        }

        if (mStateReady.load() && !mStateConnected.load()) {
            this->acceptClient();
        }

        if (!mStateReady.load() && mStateConnected.load()) {
            this->closeConnection();
            this->closeServer();
        }

        if (mStateReady.load() && mStateConnected.load()) {

            if (mStateReceiverEnable.load() && !mStateReceiverRun.load()) {
                mThdReceiver = std::thread(&Server::runReceiver, this);
                mThdReceiver.detach();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

bool Server::initializeSocket()
{
    mServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mServerSocket < 0) {
        std::cerr << std::strerror(errno) << std::endl;
        return false;
    }

    int someTrueValue = 1;
    if (setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR, &someTrueValue, sizeof(int)) < 0) {
        std::cerr << std::strerror(errno) << std::endl;
        return false;
    }

    struct sockaddr_in srv;

    srv.sin_family = AF_INET;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
    srv.sin_addr.s_addr = INADDR_ANY;
#pragma GCC diagnostic pop
    srv.sin_port = htons(static_cast<uint16_t>(mPort));

    if (bind(mServerSocket, reinterpret_cast<struct sockaddr*>(&srv), sizeof(srv)) < 0) {
        std::cerr << std::strerror(errno) << std::endl;
        return false;
    }

    if (listen(mServerSocket, mQueueLength) < 0) {
        std::cerr << std::strerror(errno) << std::endl;
        return false;
    }

    std::cout << "Listening for connections on port " << std::to_string(mPort) << std::endl;

    mStateReady.store(true);

    return true;
}

bool Server::acceptClient()
{
    if (!mStateReady.load()) {
        return false;
    }

    struct sockaddr_in cli;
    
    int len = 0;
    len = sizeof(struct sockaddr_in);
    mClientSocket = accept(mServerSocket, reinterpret_cast<struct sockaddr*>(&cli), reinterpret_cast<socklen_t*>(&len));
    if (mClientSocket < 0) {
        std::cerr << std::strerror(errno) << std::endl;
        mStateConnected.store(false);
        return false;
    }

    ServerEvent::EventInfo evt;
    evt.message = "";
    notifyObservers(ServerEvent::CLIENT_CONNECTED, evt);

    mStateConnected.store(true);

    return true;
}

void Server::setReceiver(const bool state)
{
    mStateReceiverEnable.store(state);
}

bool Server::clientConnected()
{
    return mStateConnected.load();
}

bool Server::closeConnection()
{
    if (!mStateConnected.load()) {
        return true;
    }

    if (close(mClientSocket) < 0) {
        std::cerr << std::strerror(errno) << std::endl;
        return false;
    }

    std::cout << "Closed existing connection" << std::endl;

    mStateConnected.store(false);

    return true;
}

bool Server::closeServer()
{
    this->closeConnection();

    if (close(mServerSocket)) {
        std::cerr << std::strerror(errno) << std::endl;
        return false;
    }

    std::cout << "Closed server" << std::endl;

    mStateReady.store(false);
    mStateConnected.store(false);

    return true;
}

bool Server::sendString(const std::string& message)
{
    char messageBuffer[MAX_LEN_BUFFER];

    strncpy(messageBuffer, message.c_str(), MAX_LEN_BUFFER);
    unsigned int length = static_cast<unsigned int>(strlen(messageBuffer));

    return sendBytes(messageBuffer, length);
}

bool Server::sendBytes(char *data, unsigned int dataLength)
{
    if (!mStateConnected.load()) {
        return false;
    }

    char *dataPtr = data;

    while (dataLength > 0) {
        ssize_t numberOfBytes = send(mClientSocket, dataPtr, dataLength, 0);
        if (numberOfBytes < 0) {
            std::cerr << std::strerror(errno) << std::endl;
            mStateConnected.store(false);
            return false;
        }

        dataPtr += numberOfBytes;
        dataLength -= static_cast<size_t>(numberOfBytes);
    }

    return true;
}

int Server::receiveMessage()
{
    if(!mStateReady.load() || !mStateConnected.load()) {
        return -1;
    }

    char msg_buf[MAX_LEN_BUFFER+1];

    auto numberOfBytes = static_cast<int>(recv(mClientSocket, msg_buf, MAX_LEN_BUFFER, 0));
    if (numberOfBytes < 0) {
        std::cerr << std::strerror(errno) << std::endl;
        mStateConnected.store(false);
        return -1;
    }

    if (numberOfBytes == 0) {
        mStateConnected.store(false);

        ServerEvent::EventInfo evt;
        evt.message = "";
        notifyObservers(ServerEvent::CLIENT_DISCONNECTED, evt);

        return 0;
    }

    ServerEvent::EventInfo evt;
    msg_buf[numberOfBytes] = '\0';
    evt.message = std::string(msg_buf);
    notifyObservers(ServerEvent::RECEIVED_MESSAGE, evt);

    return numberOfBytes;
}

bool Server::runReceiver()
{
    mStateReceiverRun.store(true);

    while (mStateReceiverEnable.load() && receiveMessage() > 0);

    mStateReceiverRun.store(false);

    return true;
}
