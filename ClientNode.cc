#include "ClientNode.h"
#include "TaskMessage_m.h"
#include "GossipMessage_m.h"
#include <iostream>

Define_Module(ClientNode);

void ClientNode::initialize() {
    myClientId = par("clientId");
    numClients = par("numClients");

    expectedSubtasks = 0;
    receivedSubtasks = 0;
    currentTaskMax = -1;
    gossipsReceived = 0;

    char buf[32];
    sprintf(buf, "Node %d", myClientId);
    getDisplayString().setTagArg("t", 0, buf);

    buildFingerTable();

    // Node 0 clears the output file at the start of the simulation
    if (myClientId == 0) {
        std::ofstream outfile("outputfile.txt", std::ios_base::trunc);
        outfile.close();

        cMessage *timer = new cMessage("startTask");
        scheduleAt(simTime() + 1.0, timer);
    }
}

void ClientNode::writeToFile(std::string text) {
    std::ofstream outfile("outputfile.txt", std::ios_base::app);
    outfile << text << "\n";
    outfile.close();
}

void ClientNode::buildFingerTable() {
    for (int i = 0; i < gateSize("out"); i++) {
        cGate *outGate = gate("out", i);
        cGate *nextInGate = outGate->getNextGate();
        if (nextInGate) {
            cModule *nextModule = nextInGate->getOwnerModule();
            int peerId = nextModule->par("clientId");
            routingTable[peerId] = i;
        }
    }
}

int ClientNode::getNextHopGate(int targetId) {
    if (routingTable.count(targetId)) {
        return routingTable[targetId];
    }

    int bestPeer = -1;
    int minDistance = numClients + 1;

    for (auto const& [peerId, gateIdx] : routingTable) {
        int dist = (targetId - peerId + numClients) % numClients;
        if (dist > 0 && dist < minDistance) {
            minDistance = dist;
            bestPeer = peerId;
        }
    }
    return routingTable[bestPeer];
}

void ClientNode::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        initiateTask();
        delete msg;
    } else if (dynamic_cast<TaskMessage*>(msg)) {
        processTaskMessage(msg);
    } else if (dynamic_cast<GossipMessage*>(msg)) {
        handleGossip(msg);
    }
}

void ClientNode::initiateTask() {
    EV << "Node " << myClientId << " is generating the task array.\n";

    int k = 20;
    int x = 10;
    expectedSubtasks = x;

    for (int i = 0; i < x; i++) {
        TaskMessage *task = new TaskMessage("subtask");
        task->setSourceId(myClientId);

        int target = i % numClients;
        task->setTargetId(target);
        task->setSubtaskId(i);
        task->setIsResult(false);

        task->setArrayChunkArraySize(2);
        task->setArrayChunk(0, i * 10);
        task->setArrayChunk(1, (i * 10) + 5);

        int gateIdx = getNextHopGate(target);
        send(task, "out", gateIdx);
    }
}

void ClientNode::processTaskMessage(cMessage *msg) {
    TaskMessage *taskMsg = check_and_cast<TaskMessage*>(msg);

    if (taskMsg->getTargetId() != myClientId) {
        int nextGate = getNextHopGate(taskMsg->getTargetId());
        send(taskMsg, "out", nextGate);
        return;
    }

    if (!taskMsg->isResult()) {
        int maxVal = -1;
        for (unsigned int i = 0; i < taskMsg->getArrayChunkArraySize(); i++) {
            if (taskMsg->getArrayChunk(i) > maxVal) {
                maxVal = taskMsg->getArrayChunk(i);
            }
        }

        std::string log = "Node " + std::to_string(myClientId) + " computed max " + std::to_string(maxVal) + " for subtask " + std::to_string(taskMsg->getSubtaskId());
        EV << log << "\n";
        writeToFile(log);

        TaskMessage *reply = new TaskMessage("result");
        reply->setSourceId(myClientId);
        reply->setTargetId(taskMsg->getSourceId());
        reply->setSubtaskId(taskMsg->getSubtaskId());
        reply->setIsResult(true);
        reply->setMaxResult(maxVal);

        int nextGate = getNextHopGate(reply->getTargetId());
        send(reply, "out", nextGate);

        delete msg;
    } else {
        receivedSubtasks++;
        if (taskMsg->getMaxResult() > currentTaskMax) {
            currentTaskMax = taskMsg->getMaxResult();
        }

        std::string log = "Node " + std::to_string(myClientId) + " got result for subtask " + std::to_string(taskMsg->getSubtaskId()) + ". Progress: " + std::to_string(receivedSubtasks) + "/" + std::to_string(expectedSubtasks);
        EV << log << "\n";
        writeToFile(log);

        if (receivedSubtasks == expectedSubtasks) {
            std::string finalLog = "TASK COMPLETE! Overall Max Element is: " + std::to_string(currentTaskMax);
            EV << finalLog << "\n";
            writeToFile(finalLog);

            // Trigger the Gossip Protocol
            GossipMessage *gossip = new GossipMessage("gossip");
            gossip->setOriginatorId(myClientId);

            // Simulating an IP address based on Node ID for the assignment format
            std::string mockIp = "192.168.0." + std::to_string(myClientId);
            gossip->setIpAddress(mockIp.c_str());
            gossip->setTimestamp(simTime().str().c_str());

            std::string content = std::string(gossip->getTimestamp()) + ":" + gossip->getIpAddress() + ":" + std::to_string(gossip->getOriginatorId());
            messageList[generateHash(content)] = true;
            gossipsReceived++;

            broadcastGossip(gossip, -1);
            delete gossip;
        }
        delete msg;
    }
}

void ClientNode::broadcastGossip(GossipMessage *msg, int excludeGateIndex) {
    for (int i = 0; i < gateSize("out"); i++) {
        if (i != excludeGateIndex) {
            send(msg->dup(), "out", i);
        }
    }
}

void ClientNode::handleGossip(cMessage *msg) {
    GossipMessage *gossipMsg = check_and_cast<GossipMessage*>(msg);

    std::string content = std::string(gossipMsg->getTimestamp()) + ":" + gossipMsg->getIpAddress() + ":" + std::to_string(gossipMsg->getOriginatorId());
    std::string hash = generateHash(content);

    if (messageList.find(hash) == messageList.end()) {
        // First time receiving this message - add to ML
        messageList[hash] = true;
        gossipsReceived++;

        int senderId = msg->getSenderModule()->par("clientId");
        std::string log = "Gossip Received - Node: " + std::to_string(myClientId) + " | From: Node " + std::to_string(senderId) + " | Msg: <" + content + ">";
        EV << log << "\n";
        writeToFile(log);

        // Forward to all peers except the one it came from
        broadcastGossip(gossipMsg, msg->getArrivalGate()->getIndex());

        // The assignment asks nodes to terminate when they receive gossips from all N clients.
        // Since we are running 1 test task from Node 0, we will terminate when we process this one.
        if (gossipsReceived == 1) {
            EV << "Node " << myClientId << " terminating as per gossip protocol.\n";
            // endSimulation(); // Uncomment this if you want the OMNeT++ run to instantly force quit here
        }
    }

    delete msg;
}

std::string ClientNode::generateHash(std::string content) {
    return content + "_hash";
}
