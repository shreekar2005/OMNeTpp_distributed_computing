#ifndef __CLIENTNODE_H_
#define __CLIENTNODE_H_

#include <omnetpp.h>
#include <vector>
#include <map>
#include <string>
#include <fstream>

class GossipMessage;
class TaskMessage;

using namespace omnetpp;

class ClientNode : public cSimpleModule {
  protected:
    int myClientId;
    int numClients;

    std::map<int, int> routingTable;
    std::map<std::string, bool> messageList;

    int expectedSubtasks; //no. of answers a node expects to get (one per node)
    int receivedSubtasks; //no. of responses received to a node till now
    int currentTaskMax; //max of all subtask responses received till now
    int gossipsReceived;

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    void buildFingerTable();
    int getNextHopGate(int targetId);

    void initiateTask();
    void processTaskMessage(cMessage *msg);
    void handleGossip(cMessage *msg);
    void broadcastGossip(GossipMessage *msg, int excludeGateIndex);
    
    void checkTermination();

    std::string generateHash(std::string content);
    void writeToFile(std::string text);
};

#endif