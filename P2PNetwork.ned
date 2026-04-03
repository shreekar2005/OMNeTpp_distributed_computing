network P2PNetwork {
    parameters:
        int numClients = 8;
        @display("bgb=600,600");
    submodules:
        node0: ClientNode { clientId = 0; numClients = 8; @display("p=500,300"); }
        node1: ClientNode { clientId = 1; numClients = 8; @display("p=441,441"); }
        node2: ClientNode { clientId = 2; numClients = 8; @display("p=300,500"); }
        node3: ClientNode { clientId = 3; numClients = 8; @display("p=158,441"); }
        node4: ClientNode { clientId = 4; numClients = 8; @display("p=100,300"); }
        node5: ClientNode { clientId = 5; numClients = 8; @display("p=158,158"); }
        node6: ClientNode { clientId = 6; numClients = 8; @display("p=299,100"); }
        node7: ClientNode { clientId = 7; numClients = 8; @display("p=441,158"); }
    connections:
        node0.out++ --> node1.in++;
        node0.out++ --> node2.in++;
        node0.out++ --> node4.in++;
        node1.out++ --> node2.in++;
        node1.out++ --> node3.in++;
        node1.out++ --> node5.in++;
        node2.out++ --> node3.in++;
        node2.out++ --> node4.in++;
        node2.out++ --> node6.in++;
        node3.out++ --> node4.in++;
        node3.out++ --> node5.in++;
        node3.out++ --> node7.in++;
        node4.out++ --> node5.in++;
        node4.out++ --> node6.in++;
        node4.out++ --> node0.in++;
        node5.out++ --> node6.in++;
        node5.out++ --> node7.in++;
        node5.out++ --> node1.in++;
        node6.out++ --> node7.in++;
        node6.out++ --> node0.in++;
        node6.out++ --> node2.in++;
        node7.out++ --> node0.in++;
        node7.out++ --> node1.in++;
        node7.out++ --> node3.in++;
}
