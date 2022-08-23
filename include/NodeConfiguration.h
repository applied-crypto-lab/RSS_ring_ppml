#ifndef NODECONFIGURATION_H_
#define NODECONFIGURATION_H_

#include <cstdlib>
#include <fstream>
#include <iostream>
#include "setringsize.h"
#include <string.h>
#include <string>
#include <vector>

class NodeConfiguration {

public:
    NodeConfiguration();
    NodeConfiguration(int nodeID, std::string configFile, int bits);
    int getID();
    void setBits(int); // add for 3p
    std::string getIP();
    int getPort();
    int getBits();
    int getPeerCount();
    int getPeerIndex(int id);
    std::string getPeerIP(int id);
    int getPeerPort(int id);
    virtual ~NodeConfiguration();
    std::string getPubKey();
    std::string getPeerPubKey(int id);

private:
    int id;
    int bits;
    std::string ip;
    int port;
    std::vector<std::string> peerIP;
    std::vector<int> peerPort;
    std::vector<int> peerID;
    void loadConfig(std::string configFile);
    std::string pubKey;
    std::vector<std::string> peerPubKey;
};

#endif
