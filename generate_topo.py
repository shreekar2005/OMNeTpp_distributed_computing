#!/usr/bin/env python3
import math

with open('topo.txt', 'r') as f:
    lines = [line.strip() for line in f if line.strip()]

num_nodes = int(lines[0])
radius = 200
center_x = 300
center_y = 300

ned_content = "network P2PNetwork {\n"
ned_content += "    parameters:\n"
ned_content += f"        int numClients = {num_nodes};\n"
ned_content += '        @display("bgb=600,600");\n'
ned_content += "    submodules:\n"

# generating nodes with exact x y coordinates for a perfect circle
for i in range(num_nodes):
    angle = 2 * math.pi * i / num_nodes
    x = int(center_x + radius * math.cos(angle))
    y = int(center_y + radius * math.sin(angle))
    ned_content += f'        node{i}: ClientNode {{ clientId = {i}; numClients = {num_nodes}; @display("p={x},{y}"); }}\n'

ned_content += "    connections:\n"

# reading connections safely even if there is no trailing newline
for line in lines[1:]:
    parts = line.split(':')
    if len(parts) == 2:
        src = parts[0].strip()
        dests = parts[1].strip().split()
        for dest in dests:
            ned_content += f"        node{src}.out++ --> node{dest}.in++;\n"

ned_content += "}\n"

with open('P2PNetwork.ned', 'w') as f:
    f.write(ned_content)