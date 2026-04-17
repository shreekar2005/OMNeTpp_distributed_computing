#include "ClientNode.h"
#include "TaskMessage_m.h"
#include "GossipMessage_m.h"
#include <iostream>
#include <vector>
#include <map>

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

    // node 0 clears the output file at the start of the simulation
    if (myClientId == 0) {
        std::ofstream outfile("outputfile.txt", std::ios_base::trunc);
        outfile.close();
    }

    // all nodes now schedule their tasks, at intervals of 500ms
    cMessage *timer = new cMessage("startTask");
    scheduleAt(simTime() + 1.0 + (myClientId * 0.5), timer);
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
    int totalElements = 20; // total size of the array to process

    std::vector<int> fullArray(totalElements);
    std::string fullArrayStr = "{";
    
    // 1. Generate the full random array
    for (int i = 0; i < totalElements; i++) {
        fullArray[i] = intuniform(1, 1000);
        fullArrayStr += std::to_string(fullArray[i]);
        if (i < totalElements - 1) {
            fullArrayStr += ", ";
        }
    }
    fullArrayStr += "}";

    std::string initLog = "Node " + std::to_string(myClientId) + " is generating its random task array: " + fullArrayStr;
    EV << initLog << "\n";
    writeToFile(initLog);

    // 2. Group the array elements by target node
    // This map links a targetNodeId to the vector of numbers they need to process
    std::map<int, std::vector<int>> targetPayloads;
    for (int i = 0; i < totalElements; i++) {
        int targetId = i % numClients; // Round-robin distribution
        targetPayloads[targetId].push_back(fullArray[i]);
    }

    // 3. Set how many result messages we expect back (one per target node)
    expectedSubtasks = targetPayloads.size();
    int subtaskIdCounter = 0;

    // 4. Send exactly ONE message to each target node, containing all their assigned numbers
    for (auto const& [targetId, payload] : targetPayloads) {
        TaskMessage *task = new TaskMessage("subtask");
        task->setSourceId(myClientId);
        task->setTargetId(targetId);
        task->setSubtaskId(subtaskIdCounter++); // Unique ID for each chunk sent out
        task->setIsResult(false);

        // Dynamically set the array chunk size to however many numbers this specific node got
        task->setArrayChunkArraySize(payload.size());
        for (size_t j = 0; j < payload.size(); j++) {
            task->setArrayChunk(j, payload[j]);
        }

        int gateIdx = getNextHopGate(targetId);
        send(task, "out", gateIdx);
    }
}

void ClientNode::processTaskMessage(cMessage *msg) {
    TaskMessage *taskMsg = check_and_cast<TaskMessage*>(msg);

    // if we are not the target, forward the message
    if (taskMsg->getTargetId() != myClientId) {
        int nextGate = getNextHopGate(taskMsg->getTargetId());
        send(taskMsg, "out", nextGate);
        return;
    }

    // if it is a request to process a chunk
    if (!taskMsg->isResult()) {
        int maxVal = -1;
        std::string chunkStr = "{";
        
        for (unsigned int i = 0; i < taskMsg->getArrayChunkArraySize(); i++) {
            int currentVal = taskMsg->getArrayChunk(i);
            if (currentVal > maxVal) {
                maxVal = currentVal;
            }
            
            chunkStr += std::to_string(currentVal);
            if (i < taskMsg->getArrayChunkArraySize() - 1) {
                chunkStr += ", ";
            }
        }
        chunkStr += "}";

        std::string log = "Node " + std::to_string(myClientId) + " computed max " + std::to_string(maxVal) + 
                          " for Node " + std::to_string(taskMsg->getSourceId()) + "'s subtask " + std::to_string(taskMsg->getSubtaskId()) + 
                          " with array chunk " + chunkStr;
        EV << log << "\n";
        writeToFile(log);

        TaskMessage *reply = new TaskMessage("result");
        reply->setSourceId(myClientId);
        reply->setTargetId(taskMsg->getSourceId()); // Send back to the boss who requested it
        reply->setSubtaskId(taskMsg->getSubtaskId());
        reply->setIsResult(true);
        reply->setMaxResult(maxVal);

        int nextGate = getNextHopGate(reply->getTargetId());
        send(reply, "out", nextGate);

        delete msg;
    } 
    // if it is a result coming back to us (the boss)
    else {
        receivedSubtasks++;
        if (taskMsg->getMaxResult() > currentTaskMax) {
            currentTaskMax = taskMsg->getMaxResult();
        }

        std::string log = "Node " + std::to_string(myClientId) + " got result for its subtask " + std::to_string(taskMsg->getSubtaskId()) + ". Progress: " + std::to_string(receivedSubtasks) + "/" + std::to_string(expectedSubtasks);
        EV << log << "\n";
        writeToFile(log);

        // when our specific task is fully complete
        if (receivedSubtasks == expectedSubtasks) {
            std::string finalLog = "NODE " + std::to_string(myClientId) + " TASK COMPLETE! Overall Max Element for its array is: " + std::to_string(currentTaskMax);
            EV << finalLog << "\n";
            writeToFile(finalLog);

            // Trigger the Gossip Protocol
            GossipMessage *gossip = new GossipMessage("gossip");
            gossip->setOriginatorId(myClientId);

            std::string mockIp = "192.168.0." + std::to_string(myClientId);
            gossip->setIpAddress(mockIp.c_str());
            gossip->setTimestamp(simTime().str().c_str());

            std::string content = std::string(gossip->getTimestamp()) + ":" + gossip->getIpAddress() + ":" + std::to_string(gossip->getOriginatorId());
            messageList[generateHash(content)] = true;
            gossipsReceived++;

            broadcastGossip(gossip, -1);
            delete gossip;
            
            // checking if we can terminate, if we are the last node to finish
            checkTermination();
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
        // add msg to ML when received first time
        messageList[hash] = true;
        gossipsReceived++;

        int senderId = msg->getSenderModule()->par("clientId");
        std::string log = "Gossip Received - Node: " + std::to_string(myClientId) + " | From: Node " + std::to_string(senderId) + " | Msg: <" + content + ">";
        EV << log << "\n";
        writeToFile(log);

        // forward to all peers except the one it came from
        broadcastGossip(gossipMsg, msg->getArrivalGate()->getIndex());
        
        // check if we have received all N gossips
        checkTermination();
    }

    delete msg;
}

void ClientNode::checkTermination() {
    if (gossipsReceived == numClients) {
        std::string log = "Node " + std::to_string(myClientId) + " has received gossips from all " + std::to_string(numClients) + " clients. Terminating as per protocol.";
        EV << log << "\n";
        writeToFile(log);
    }
}

std::string ClientNode::generateHash(std::string content) {
    return content + "_hash";
}