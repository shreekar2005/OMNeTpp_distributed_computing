# CSL3080 Assignment 2 - P2P Distributed Task Execution

**Team Members:**
* Shreekar (B23CS1069)
* Tavishi Srivastava (B23CS1101)

### Project Overview
This project simulates a fully peer-to-peer (P2P) network using the OMNeT++ Discrete Event Simulator. The network is built on a ring topology optimized with a Chord-like DHT routing table (using finger tables) to achieve O(log N) message complexity. 

Nodes divide a large computational task (e.g., finding the maximum element in an array) into subtasks, distribute them across the network, and consolidate the results. Once the task is fully complete, the originating node triggers a gossip protocol to terminate the network, using a Message List (ML) hashed log to drop duplicates and prevent infinite routing loops.

### Files Description
* `topo.txt` - Configuration file defining the number of nodes and their connections.
* `generate_topo.py` - Python script that reads topo.txt and dynamically generates the OMNeT++ `P2PNetwork.ned` file, placing nodes in a perfect graphical ring OR as specified in topo.txt.
* `ClientNode.cc` / `.h` - The core C++ logic for the client nodes (routing, subtask processing, gossiping).
* `TaskMessage.msg` - Custom OMNeT++ message definition for routing arrays and results.
* `GossipMessage.msg` - Custom OMNeT++ message for the termination broadcast.
* `outputfile.txt` - The dynamically generated log file containing subtask results, the final consolidated task result, and the gossip message tracking.

---

### How to Compile and Run

1. Open your Ubuntu terminal and navigate to the project directory:
   `cd <project directory>`

2. (optional, you dont need this if using omnetpp IDE) Generate the Makefile (if not already done):
   `opp_makemake -f --deep`

3. (option, click on build in omnetpp IDE) Compile the C++ code:
   `make`

4. Generate the dynamic network topology:
   `python3 generate_topo.py` or
   `./generate_topo.py`

5. Run the simulation:
   You can either open `omnetpp.ini` in the OMNeT++ IDE and click the green Run button, or run it directly from the terminal using:
   `./CN_Assignmnet2`

---

### How to Update or Modify the Project

#### 1. Changing the Number of Nodes or Topology
You do not need to rewrite any C++ code to change the network structure. 
* Open `topo.txt`.
* Change the first line to your new total number of nodes (N).
* Update the subsequent lines to define the new connections. To maintain O(log N) routing, ensure each node connects to its successor, plus nodes at distances of powers of 2 (e.g., distance 1, 2, 4, 8...). The format is `SourceNode : Dest1 Dest2 Dest3`.
* Run `./generate_topo.py` in the terminal to rebuild the `.ned` file.
* Run the simulation. The python script will automatically recalculate the exact coordinates to keep the nodes in a perfect visual ring.

#### 2. Changing the Computational Task (e.g., Max to Min)
If you want to change the task from finding the Maximum element to finding the Minimum element, you need to update the logic in `ClientNode.cc`.

* Open `ClientNode.cc`.
* Go to the `initiateTask` function. If you want to change the dummy array values that get sent out, modify the math inside `task->setArrayChunk(0, ...)`.
* Go to the `processTaskMessage` function. Find the block where the node calculates the result (look for `int maxVal = -1;`). 
* Change that logic to find the minimum. For example, set a high initial value like `int minVal = 9999;` and flip the comparison operator from `>` to `<`.
* Further down in `processTaskMessage`, find where the originating node tracks the overall result (look for `if (taskMsg->getMaxResult() > currentTaskMax)`). Update this to track the absolute minimum instead.
* Save the file, run `make` in the terminal to recompile the C++ code, and run the simulation.

#### 3. Checking the Results
After every successful run, open `outputfile.txt`. The code is written to overwrite this file at the start of every simulation, so you will always see the freshest logs tracking which node computed what chunk, the final aggregated answer, and the exact path the gossip protocol took to shut the nodes down.