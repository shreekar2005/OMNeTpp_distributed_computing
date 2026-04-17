network P2PNetwork {
    parameters:
        int numClients = 5;
        @display("bgb=600,600");
    submodules:
        node0: ClientNode { clientId = 0; numClients = 5; @display("p=500,300"); }
        node1: ClientNode { clientId = 1; numClients = 5; @display("p=361,490"); }
        node2: ClientNode { clientId = 2; numClients = 5; @display("p=138,417"); }
        node3: ClientNode { clientId = 3; numClients = 5; @display("p=138,182"); }
        node4: ClientNode { clientId = 4; numClients = 5; @display("p=361,109"); }
    connections:
        node0.out++ --> node1.in++;
        node0.out++ --> node2.in++;
        node0.out++ --> node4.in++;
        node1.out++ --> node2.in++;
        node1.out++ --> node3.in++;
        node1.out++ --> node0.in++;
        node2.out++ --> node3.in++;
        node2.out++ --> node4.in++;
        node2.out++ --> node1.in++;
        node3.out++ --> node4.in++;
        node3.out++ --> node0.in++;
        node3.out++ --> node2.in++;
        node4.out++ --> node0.in++;
        node4.out++ --> node1.in++;
        node4.out++ --> node3.in++;
}
