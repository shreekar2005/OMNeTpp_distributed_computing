#ifndef __CLIENTNODE_H_
#define __CLIENTNODE_H_

#include <omnetpp.h>
#include <vector>
#include <map>
#include <string>
#include <fstream>

// letting the compiler know these message classes exist
class GossipMessage;
class TaskMessage;

using namespace omnetpp;

class ClientNode : public cSimpleModule {
  protected:
    int myClientId;
    int numClients;

    std::map<int, int> routingTable;
    std::map<std::string, bool> messageList;

    int expectedSubtasks;
    int receivedSubtasks;
    int currentTaskMax;
    int gossipsReceived;

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    void buildFingerTable();
    int getNextHopGate(int targetId);

    void initiateTask();
    void processTaskMessage(cMessage *msg);
    void handleGossip(cMessage *msg);
    void broadcastGossip(GossipMessage *msg, int excludeGateIndex);
    
    // New helper to check if we can terminate
    void checkTermination();

    std::string generateHash(std::string content);
    void writeToFile(std::string text);
};

#endif