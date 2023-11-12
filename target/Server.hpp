#pragma once

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include "Subject.hpp"
#include "ServerEvent.hpp"


/**
 * TCP Server Implementation.
 */
class Server : public Subject<ServerEvent::EventInfo>
{

public:

    /**
     * @brief Constructor, assign port.
     * @param port Port to listen on.
     * @param queueLength Length of queue for waiting clients.
     */
    Server(const unsigned int port, const int queueLength=5);

    /**
     * @brief Destructor, disconnect client if needed.
     */
    ~Server();

    /**
     * @brief Start the server.
     * @return True if successful.
     */
    bool start();

    /**
     * @brief Send string to client.
     * @param Message to send.
     * @return True if successful.
     */
    bool sendString(const std::string& message);

    /**
     * @brief Send bytes to client.
     * @param Data to send.
     * @param Length of data to send.
     * @return True if successful.
     */
    bool sendBytes(char* data, unsigned int dataLength);

    /**
     * @brief Enable or disable receiver.
     * @param state True to enable, false to disable receiver.
     */
    void setReceiver(const bool state);

    /**
     * Check if a client is connected.
     * 
     * @return True if client is connected
     */
    bool clientConnected();

    /**
     * @brief Close existing connection to client.
     * @return True if successful.
     */
    bool closeConnection();

    /**
     * @brief Close server, disconnect client if still connected.
     * @return True if successful.
     */
    bool closeServer();

private:

    /**
     * @brief Run TCP server. Call this method in new thread.
     */
    void run();

    /**
     * @brief Initialize server socket.
     * @return True if successful.
     */
    bool initializeSocket();

    /**
     * @brief Accept new client. This function is blocking until a client is connected!
     * @return True if successful.
     */
    bool acceptClient();

    /**
     * @brief Run receiver in while loop. Should launched in new thread.
     * @return True if successful.
     */
    bool runReceiver();

    /**
     * @brief Receive message from client. The received message will be transmitted to registered observers. Function is blocking until a client sends a message.
     * @return True if successful.
     */
    int receiveMessage();

private:

    int mServerSocket; // Server socket handle
    int mClientSocket; // Client socket handle
    std::atomic<bool> mStateRun; // True if server is running
    std::atomic<bool> mStateReady; // True if server is initialized
    std::atomic<bool> mStateConnected; // True if client is connected
    std::atomic<bool> mStateReceiverEnable; // True if receiver is enabled
    std::atomic<bool> mStateReceiverRun; // True if receiver is running
    unsigned int mPort; // Port to listen for clients
    int mQueueLength; // Length of queue for incoming clients.
    std::thread mThdReceiver; // Thread of receiver
    std::thread mThdServer; // Thread of server
    
};
