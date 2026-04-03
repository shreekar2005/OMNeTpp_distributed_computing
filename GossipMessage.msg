message GossipMessage {
    int originatorId;
    string ipAddress;
    string timestamp;
    
    // We will hash these contents in the C++ code to maintain the ML list
}