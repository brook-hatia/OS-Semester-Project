#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

int main()
{
    struct sockaddr_in servaddr;

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9004); // Use htons for "host to network short"
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    int connectfd = connect(socketfd, (sockaddr *)&servaddr, sizeof(servaddr));

    if (socketfd == -1 || connectfd == -1)
    {
        cout << "connection failed" << endl;
        return 1;
    }

    cout << "connection successful" << endl;

    while (true)
    {
        string sendMsg;
        getline(cin, sendMsg);

        if (sendMsg == "shutdown")
        {
            // Send the shutdown message to the server
            write(socketfd, sendMsg.c_str(), sendMsg.size());
            // Client closes its own socket and exits the loop
            close(socketfd);
            break;
        }

        // Send message to the server
        write(socketfd, sendMsg.c_str(), sendMsg.size());

        // Receive and display the server's response
        char readMsg[4000] = {};
        int readfd = read(socketfd, readMsg, sizeof(readMsg));
        cout << "Server: " << readMsg << endl;
    }

    // Close the client socket before exiting
    close(socketfd);

    return 0;
}
