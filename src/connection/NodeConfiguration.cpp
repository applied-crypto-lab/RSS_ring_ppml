#include "NodeConfiguration.h"

NodeConfiguration::NodeConfiguration() {}

NodeConfiguration::NodeConfiguration(int nodeID, std::string configFile, int sizeOfModulus) {
    bits = sizeOfModulus;
    id = nodeID;
    loadConfig(configFile);
}
int NodeConfiguration::getBits() {
    return 128;
}

void NodeConfiguration::setBits(int imputbits) {
    bits = imputbits;
}

int NodeConfiguration::getID() {
    return id;
}

std::string NodeConfiguration::getIP() {
    return ip;
}

int NodeConfiguration::getPort() {
    return port;
}

int NodeConfiguration::getPeerCount() {
    return peerIP.size();
}

std::string NodeConfiguration::getPeerIP(int id) {
    // Get the index of the id;
    int index = -1;
    for (int i = 0; i < peerID.size(); ++i) {
        if (peerID[i] == id) {
            index = i;
            break;
        }
    }
    return peerIP[index];
}

int NodeConfiguration::getPeerPort(int id) {
    // Get the index of the id;
    int index = -1;
    for (int i = 0; i < peerID.size(); ++i) {
        if (peerID[i] == id) {
            index = i;
            break;
        }
    }
    return peerPort[index];
}

int NodeConfiguration::getPeerIndex(int id) {
    // Get the index of the id
    for (int i = 0; i < peerID.size(); ++i) {
        if (peerID[i] == id)
            return i;
    }
    return -1;
}

std::string NodeConfiguration::getPubKey() {
    return pubKey;
}

std::string NodeConfiguration::getPeerPubKey(int id) {
    // Get the index of the id;
    int index = -1;
    for (int i = 0; i < peerID.size(); ++i) {
        if (peerID[i] == id) {
            index = i;
            break;
        }
    }
    return peerPubKey[index];
}

/*
 * Loads the network configuration from the file.
 * Assumes that the config file is sorted
 */
void NodeConfiguration::loadConfig(std::string configFile) {
    std::ifstream configIn(configFile.c_str(), std::ios::in);
    // Make sure the file exists and can be opened
    if (!configIn) {
        std::cout << "File could not be opened";
        std::exit(1);
    }
    std::string line;
    std::vector<std::string> tokens;
    // Read each line of the configuration file
    while (std::getline(configIn, line)) {
        tokens.clear();
        char *s = strdup(line.c_str());
        char *tok = strtok(s, ",");
        std::string str;
        while (tok != NULL) {
            str = tok;
            tokens.push_back(str);
            tok = strtok(NULL, ",");
        }
        // free(original_copy);
        free(s);
        // delete tok;
        // free((char *) tok);
        if (id == atoi(tokens[0].c_str())) {
            ip = tokens[1];
            port = atoi(tokens[2].c_str());
            pubKey = tokens[3];
        } else {
            peerID.push_back(atoi(tokens[0].c_str()));
            peerIP.push_back(tokens[1]);
            peerPort.push_back(atoi(tokens[2].c_str()));
            peerPubKey.push_back(tokens[3]);
        }
    }
    configIn.close();
}

NodeConfiguration::~NodeConfiguration() {
}
