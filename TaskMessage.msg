message TaskMessage {
    int sourceId;
    int targetId;
    int subtaskId;
    
    // We can just pass the array chunk directly
    int arrayChunk[];
    
    // Flag to know if this message is sending the chunk or returning the max value
    bool isResult;
    int maxResult;
}