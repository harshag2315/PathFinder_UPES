#include <iostream>
#include <vector>
#include <climits>
#include <string>
#include <sstream>

using namespace std;

// ... (include all the classes and functions from the original code)
class ListNode {
public:
    int vertex;
    int weight;
    ListNode* next;

    ListNode(int v, int w) : vertex(v), weight(w), next(nullptr) {}
};

// MinHeapNode class for storing node and f-cost
class MinHeapNode {
public:
    int v;
    int f;
    MinHeapNode(int vertex, int cost) : v(vertex), f(cost) {}
};

// Custom MinHeap class
class MinHeap {
public:
    vector<MinHeapNode> heap;

    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

    void insert(MinHeapNode node) {
        heap.push_back(node);
        int i = heap.size() - 1;

        while (i != 0 && heap[parent(i)].f > heap[i].f) {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void heapify(int i) {
        int l = left(i);
        int r = right(i);
        int smallest = i;

        if (l < heap.size() && heap[l].f < heap[smallest].f)
            smallest = l;
        if (r < heap.size() && heap[r].f < heap[smallest].f)
            smallest = r;

        if (smallest != i) {
            swap(heap[i], heap[smallest]);
            heapify(smallest);
        }
    }

    MinHeapNode extractMin() {
        if (heap.size() == 1) {
            MinHeapNode root = heap[0];
            heap.pop_back();
            return root;
        }

        MinHeapNode root = heap[0];
        heap[0] = heap.back();
        heap.pop_back();

        heapify(0);
        return root;
    }

    bool isEmpty() {
        return heap.empty();
    }
};

// Graph class with an adjacency list represented by an array of linked lists
class Graph {
public:
    int numNodes;
    vector<ListNode*> adjList;

    Graph(int n) {
        numNodes = n;
        adjList.resize(n, nullptr);
    }

    // Add edge to the graph using linked lists
    void addEdge(int u, int v, int weight) {
        ListNode* newNode = new ListNode(v, weight);
        newNode->next = adjList[u];
        adjList[u] = newNode;

        // Since it's undirected, add the reverse edge as well
        newNode = new ListNode(u, weight);
        newNode->next = adjList[v];
        adjList[v] = newNode;
    }

    // Utility to print the adjacency list (for debugging)
    void printGraph() {
        for (int i = 0; i < numNodes; ++i) {
            cout << "Node " << i << ": ";
            ListNode* temp = adjList[i];
            while (temp) {
                cout << "(" << temp->vertex << ", " << temp->weight << ") -> ";
                temp = temp->next;
            }
            cout << "NULL\n";
        }
    }
};

// Heuristic function (placeholder)
int heuristic(int node1, int node2) {
    return abs(node1 - node2);
}

// Print the path from source to target
void printPath(vector<int>& parent, int j) {
    if (parent[j] == -1) {
        cout << j << " ";
        return;
    }
    printPath(parent, parent[j]);
    cout << "-> " << j << " ";
}

// A* algorithm using custom min-heap and adjacency list as linked lists
void aStar(Graph& graph, int src, int target) {
    int numNodes = graph.numNodes;
    vector<int> g(numNodes, INT_MAX);  // Cost from start to a node
    vector<int> f(numNodes, INT_MAX);  // Estimated total cost (f = g + h)
    vector<int> parent(numNodes, -1);  // To store the path
    MinHeap minHeap;

    g[src] = 0;
    f[src] = heuristic(src, target);
    minHeap.insert(MinHeapNode(src, f[src]));

    while (!minHeap.isEmpty()) {
        int u = minHeap.extractMin().v;

        if (u == target) {
            cout << "Shortest path found. Distance from source to target: " << g[u] << endl;
            cout << "Path: ";
            printPath(parent, u);
            cout << endl;
            return;
        }

        ListNode* neighbor = graph.adjList[u];
        while (neighbor != nullptr) {
            int v = neighbor->vertex;
            int weight = neighbor->weight;

            if (g[u] + weight < g[v]) {
                g[v] = g[u] + weight;
                f[v] = g[v] + heuristic(v, target);
                parent[v] = u;
                minHeap.insert(MinHeapNode(v, f[v]));
            }

            neighbor = neighbor->next;
        }
    }

    cout << "No path found from source to target." << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <source_node> <target_node> <type> <time> <weather>" << std::endl;
        return 1;
    }

    int sourceNode = std::stoi(argv[1]);
    int targetNode = std::stoi(argv[2]);
    int typeNode = std::stoi(argv[3]);
    int time_type=std::stoi(argv[4]);
    int weather_type=std::stoi(argv[5]);
    
    int numNodes = 240;  // Example number of nodes
    Graph graph(numNodes);

    if(time_type<=10){
        time_type=1;
    }
    else{
        time_type=0;
    }

    // Add all the edges as in the original code
    int type_type= (typeNode==1) ? 1: 0;
    //for car
    if(type_type==0){
        graph.addEdge(0, 1, 19);    // Main Gate
    graph.addEdge(1, 2, 13);    // towards enrollment office
    graph.addEdge(2, 3, 35);    // towards enrollment office
    graph.addEdge(3, 4, 27);    // Front of enrollment office
    graph.addEdge(4, 6, 28);    // Front of amphitheater
    graph.addEdge(6, 7, 35);    // Front of sitting area
    graph.addEdge(7, 10, 12);   // Front of GYM & MAC
    graph.addEdge(10, 13, 10);  // Parking start front of MAC
    graph.addEdge(13, 14, 11);  // Front of box
    graph.addEdge(14, 15, 17);  // End of box
    graph.addEdge(15, 16, 16);  // Staircase front of MAC
    graph.addEdge(16, 17, 14);  // Front of Library
    graph.addEdge(17, 18, 17);  // End of Library
    graph.addEdge(18, 19, 32);  // Between blocks
    graph.addEdge(19, 20, 7);   // Last stairs
    graph.addEdge(20, 21, 23);  // Start of Gandhi Chowk
    graph.addEdge(21, 22, 10);  // Front of Gandhi Chowk
    graph.addEdge(22, 24, 5);   // Front of 9th block amphitheater
    graph.addEdge(24, 25, 20);  // Back side of 3rd block & front of amphitheater
    graph.addEdge(25, 28, 13);  // Backside of 9th block
    graph.addEdge(28, 29, 3);   // Backside of Frisco
    graph.addEdge(29, 30, 12);  // Backside of Frisco towards food court
    graph.addEdge(30, 31, 5);   // Backside of 5th block
    graph.addEdge(31, 33, 27);  // Backside of Food Court
    graph.addEdge(33, 34, 20);  // Front side of food court 1st gate
    graph.addEdge(34, 37, 7);   // Front of food court
    graph.addEdge(37, 38, 6);   // Towards Hubble
    graph.addEdge(43, 44, 24);  // Towards main gate back of Hubble
    graph.addEdge(44, 45, 10);  // Towards main gate back of Hubble
    graph.addEdge(45, 1, 57);   // Front of gate
    graph.addEdge(1, 46, 15);   // Towards front of Hubble
    graph.addEdge(46, 47, 25);  // Front of Hubble
    graph.addEdge(47, 49, 10);  // Hubble towards 1st block
    graph.addEdge(49, 3, 20);   // Hubble towards enrollment office
    graph.addEdge(53, 38, 20);  // Front side of 2nd gate of food court
    graph.addEdge(53, 43, 23);  // Backside of Hubble
    graph.addEdge(55, 49, 13);  // Towards Hubble
    graph.addEdge(54, 56, 9);   // Inside 1st block & front of wash room
    graph.addEdge(62, 55, 54);  // Front side of 1st block & towards Hubble
    graph.addEdge(63, 64, 30);  // Towards MAC
    graph.addEdge(64, 7, 17);   // Towards sitting area behind MAC
    graph.addEdge(64, 10, 9);   // Towards MAC/GYM
    graph.addEdge(63, 65, 17);  // Towards lib stairs
    graph.addEdge(65, 66, 61);  // Towards Gandhi Chowk
    graph.addEdge(66, 67, 10);  // Towards Gandhi Chowk
    graph.addEdge(66, 24, 28);  // Towards Gandhi Chowk
    graph.addEdge(61, 79, 46);  // Outside 1st block path
    graph.addEdge(79, 77, 10);  // Outside 1st block to 2nd block 2nd path
    graph.addEdge(95, 96, 17);  // Path 6th block main entrance
    graph.addEdge(96, 97, 31);  // Path from 6th block to main road
    graph.addEdge(95, 99, 31);  // 6th to library road
    graph.addEdge(99, 100, 15); // Main road from library to IT
    graph.addEdge(100, 101, 26); // Main road to food court 2
    graph.addEdge(105, 106, 43); // Towards power house
    graph.addEdge(115, 97, 66);  // Main road to front of game office
    graph.addEdge(116, 117, 8);  // Main road for placement activity
    graph.addEdge(131, 117, 28); // Entrance of ground to intersection of MDP Guest house
    graph.addEdge(131, 132, 5);  // Towards Guest house front of Ground
    graph.addEdge(132, 133, 34); // Front gate of guest house
    graph.addEdge(131, 135, 10); // Towards guest house and girls hostel
    graph.addEdge(135, 136, 28); // 2nd gate of guest house
    graph.addEdge(135, 136, 47); // Girls hostel main gate
    graph.addEdge(136, 137, 12); // Girls hostel entrance gate
    graph.addEdge(115, 139, 10); // 2nd gate of fire station
    graph.addEdge(97, 140, 29);  // Towards Gandhi Chowk
    graph.addEdge(140, 141, 22); // Towards Gandhi Chowk
    graph.addEdge(141, 143, 10); // Gandhi Chowk
    graph.addEdge(143, 22, 11);  // Towards other block
    graph.addEdge(141, 144, 17); // Towards 9th block
    graph.addEdge(141, 145, 10); // Towards body hostel
    graph.addEdge(145, 146, 38); // Towards play school
    graph.addEdge(146, 148, 14); // Front of canteen
    graph.addEdge(148, 150, 33); // Front of Tulip towards boys hotel 
    graph.addEdge(150, 152, 23); // Front of laboratory
    graph.addEdge(152, 154, 10); // Towards car gate of boys hostel
    graph.addEdge(152, 159, 10); // Sitting area towards Tulip
    graph.addEdge(159, 160, 18); // Front of Tulip
    graph.addEdge(160, 161, 2);  // Towards Tulip 1st gate
    graph.addEdge(163, 164, 3);  // Towards 11th block 1st gate
    graph.addEdge(164, 165, 21); // 11th and 10th block
    graph.addEdge(165, 166, 23); // Towards Gandhi Chowk 9th and 10th block
    graph.addEdge(166, 167, 36); // Towards Gandhi Chowk 9th block
    graph.addEdge(167, 168, 33); // Towards Gandhi Chowk 9th last gate
    graph.addEdge(168, 184, 4);  // Towards play school
    graph.addEdge(184, 185, 8);  // Towards amphitheater
    graph.addEdge(185, 24, 13);  // Towards 3rd block
    graph.addEdge(175, 195, 23); // Backroad of 11th and 10th block
    }


    else if(type_type==1 && weather_type==1 ){
        graph.addEdge(0, 1, 19);    // Main Gate
        graph.addEdge(1, 2, 13);    // towards enrollment office
        graph.addEdge(3, 4, 27);    // Front of enrollment office
        graph.addEdge(4, 5, 21);    // Door of Enrollment office
        graph.addEdge(7, 8, 7);     // Route for sitting area
        graph.addEdge(8, 9, 1);     // Sitting area
        graph.addEdge(7, 10, 12);   // Front of GYM & MAC
    graph.addEdge(10, 11, 11);  // GYM Gate
    graph.addEdge(10, 12, 11);  // MAC Gate
    graph.addEdge(10, 13, 10);  // Parking start front of MAC
    graph.addEdge(13, 14, 11);  // Front of box
    graph.addEdge(14, 15, 17);  // End of box
    graph.addEdge(15, 16, 16);  // Staircase front of MAC
    graph.addEdge(16, 17, 14);  // Front of Library
    graph.addEdge(17, 18, 17);  // End of Library
    graph.addEdge(18, 19, 32);  // Between blocks
    graph.addEdge(19, 20, 7);   // Last stairs
    graph.addEdge(20, 21, 23);  // Start of Gandhi Chowk
    graph.addEdge(21, 22, 10);  // Front of Gandhi Chowk
    graph.addEdge(22, 23, 6);   // Gandhi Chowk
    graph.addEdge(22, 24, 5);   // Front of 9th block amphitheater
    graph.addEdge(24, 25, 20);  // Back side of 3rd block & front of amphitheater
    graph.addEdge(25, 26, 3);   // Back of 3rd block
    graph.addEdge(25, 28, 13);  // Backside of 9th block
    graph.addEdge(28, 29, 3);   // Backside of Frisco
    graph.addEdge(29, 30, 12);  // Backside of Frisco towards food court
    graph.addEdge(30, 31, 5);   // Backside of 5th block
    graph.addEdge(31, 32, 3);   // Backside entrance of 5th block
    graph.addEdge(34, 35, 10);  // Main gate of food court
    graph.addEdge(34, 36, 9);   // Stair towards food court
    graph.addEdge(34, 37, 7);   // Front of food court
    graph.addEdge(37, 38, 6);   // Towards Hubble
    graph.addEdge(38, 39, 8);   // Food court 2nd gate towards Hubble
    graph.addEdge(39, 40, 4);   // 2nd gate of food court
    graph.addEdge(39, 41, 14);  // Narrow route towards main gate through food court
    graph.addEdge(41, 42, 17);  // Front route towards main gate
    graph.addEdge(42, 68, 14);  // Food court1 back gate 
    graph.addEdge(68, 69, 2);   // Inside food court through back gate
    graph.addEdge(69, 70, 2);   // Inside food court towards 2nd gate
    graph.addEdge(70, 40, 10);  // Inside food court towards 2nd gate outwards
    graph.addEdge(69, 71, 10);  // Inside food court towards 1st gate
    graph.addEdge(71, 35, 9);   // Inside food court towards 1st gate outwards
    graph.addEdge(1, 46, 15);   // Towards front of Hubble
    graph.addEdge(46, 47, 25);  // Front of Hubble
    graph.addEdge(47, 48, 4);   // Hubble main gate
    graph.addEdge(47, 49, 10);  // Hubble towards 1st block
    graph.addEdge(49, 3, 20);   // Hubble towards enrollment office
    graph.addEdge(49, 50, 9);   // Side of Hubble towards 1st block
    graph.addEdge(50, 51, 6);   // Towards 1st block
    graph.addEdge(51, 52, 52);  // Towards food court
    graph.addEdge(52, 53, 4);   // Towards food court backside of Hubble
    graph.addEdge(53, 38, 20);  // Front side of 2nd gate of food court
    graph.addEdge(51, 54, 9);   // 1st door of 1st block
    graph.addEdge(54, 55, 12);  // Towards amphitheater
    graph.addEdge(55, 49, 13);  // Towards Hubble
    graph.addEdge(54, 56, 9);   // Inside 1st block & front of wash room
    graph.addEdge(56, 57, 32);  // Inside 1st block & towards 4th block
    graph.addEdge(57, 58, 32);  // Inside 1st block towards 2nd block
    graph.addEdge(58, 59, 20);  // Inside 1st block towards 2nd block
    graph.addEdge(59, 56, 32);  // Inside 1st block towards Library stairs
    graph.addEdge(59, 60, 7);   // Exit of 1st block through library stairs
    graph.addEdge(60, 61, 7);   // Towards library stairs
    graph.addEdge(61, 62, 14);  // Front side of 1st block
    graph.addEdge(62, 55, 54);  // Front side of 1st block & towards Hubble
    graph.addEdge(62, 63, 15);  // Climbing through lib stairs
    graph.addEdge(63, 64, 30);  // Towards MAC
    graph.addEdge(64, 7, 17);   // Towards sitting area behind MAC
    graph.addEdge(64, 10, 9);   // Towards MAC/GYM
    graph.addEdge(63, 65, 17);  // Towards lib stairs
    graph.addEdge(65, 16, 9);   // Towards Library
    graph.addEdge(66, 67, 10);  // Towards Gandhi Chowk
    graph.addEdge(66, 20, 13);  // Towards design block
    graph.addEdge(66, 24, 28);  // Towards Gandhi Chowk
    graph.addEdge(66, 72, 18);  // From stairs to 3rd block
    graph.addEdge(72, 73, 2);   // Gate of 3rd block
    graph.addEdge(72, 74, 3);   // Back gate of 2nd block
    graph.addEdge(74, 75, 22);  // Towards 2nd block
    graph.addEdge(75, 76, 6);   // 2nd block stairs
    graph.addEdge(76, 77, 2);   // 2nd block gate
    graph.addEdge(77, 78, 4);   // 2nd block to 1st block
    graph.addEdge(78, 58, 16);  // Inside 1st block towards Hubble
    graph.addEdge(61, 79, 46);  // Outside 1st block path
    graph.addEdge(79, 77, 10);  // Outside 1st block to 2nd block 2nd path
    graph.addEdge(57, 80, 10);  // 1st block to 4th block
    graph.addEdge(80, 81, 8);   // 1st block path to 4th block path
    graph.addEdge(81, 82, 3);   // 4th block gate
    graph.addEdge(82, 83, 3);   // 4th block stairs
    graph.addEdge(83, 84, 9);   // 4th block stair continues
    graph.addEdge(84, 85, 22);  // 4th block last gate
    graph.addEdge(85, 86, 4);   // 4th to 5th block
    graph.addEdge(86, 87, 2);   // 5th block gate
    graph.addEdge(52, 81, 22);  // Hubble to 4th block gate & 1st block gate
    graph.addEdge(86, 87, 23);  // 5th block stairs
    graph.addEdge(87, 88, 5);   // 5th block stairs
    graph.addEdge(88, 32, 8);   // 5th block gate
    graph.addEdge(17, 89, 51);  // Main road to library (staircase)
    graph.addEdge(18, 93, 39);  // Main road to side of IT
    graph.addEdge(90, 93, 12);  // Main road to IT (staircase)
    graph.addEdge(89, 90, 18);  // Library to IT
    graph.addEdge(89, 91, 3);   // Main gate of Library
    graph.addEdge(90, 92, 3);   // Main gate of IT
    graph.addEdge(93, 94, 26);  // Path towards 6th block
    graph.addEdge(94, 95, 24);  // Path towards backside of 6th block
    graph.addEdge(95, 96, 17);  // Path 6th block main entrance
    graph.addEdge(96, 97, 31);  // Path from 6th block to main road
    graph.addEdge(96, 98, 5);   // 6th block main entrance
    graph.addEdge(95, 99, 31);  // 6th to library road
    graph.addEdge(99, 90, 18);  // IT to main road
    graph.addEdge(99, 100, 15); // Main road from library to IT
    graph.addEdge(100, 89, 15); // Main road to Library
    graph.addEdge(100, 101, 26); // Main road to food court 2
    graph.addEdge(101, 102, 31); // Main road towards to backside of foodcourt
    graph.addEdge(102, 103, 15); // Food court backside road
    graph.addEdge(103, 104, 16); // Backside of food court to backside of MAC
    graph.addEdge(104, 105, 11); // Backside of MAC
    graph.addEdge(105, 106, 43); // Towards power house
    graph.addEdge(106, 107, 5);  // Power house gate
    graph.addEdge(107, 108, 9);  // Power house to basketball court
    graph.addEdge(108, 109, 47); // Basketball court
    graph.addEdge(109, 110, 54); // Basketball court exit 
    graph.addEdge(110, 111, 10); // Towards Placement office
    graph.addEdge(111, 112, 3);  // Main road of placement office
    graph.addEdge(103, 113, 14); // Box from food court back
    graph.addEdge(113, 114, 11); // Box mid to box mid
    graph.addEdge(114, 14, 11);  // Box to main road
    graph.addEdge(115, 97, 66);  // Main road to front of game office
    graph.addEdge(115, 116, 7);  // Main road to game office
    graph.addEdge(116, 117, 8);  // Main road for placement activity
    graph.addEdge(117, 118, 47); // Main road towards placement activity
    graph.addEdge(118, 119, 3);  // Towards ground
    graph.addEdge(118, 112, 50); // Basketball court
    graph.addEdge(120, 112, 7);  // Basketball entrance to ground entrance
    graph.addEdge(112, 121, 36); // Basketball entrance to placement activity block
    graph.addEdge(121, 122, 33); // Inside placement block
    graph.addEdge(122, 123, 25); // Exit from placement block
    graph.addEdge(123, 124, 71); // Corner of ground
    graph.addEdge(124, 125, 104); // Other side of ground
    graph.addEdge(125, 126, 32); // Front side of ground
    graph.addEdge(126, 127, 18); // Towards ground
    graph.addEdge(127, 128, 25); // Front side of ground
    graph.addEdge(128, 129, 36); // Towards basketball ground
    graph.addEdge(129, 130, 10); // Basketball back
    graph.addEdge(130, 119, 13); // Basketball back
    graph.addEdge(131, 128, 40); // Intersection in front of MDP Guest house
    graph.addEdge(131, 117, 28); // Entrance of ground to intersection of MDP Guest house
    graph.addEdge(131, 132, 5);  // Towards Guest house front of Ground
    graph.addEdge(132, 133, 34); // Front gate of guest house
    graph.addEdge(117, 134, 8);  // Museum way
    graph.addEdge(131, 135, 10); // Towards guest house and girls hostel
    graph.addEdge(135, 136, 28); // 2nd gate of guest house
    graph.addEdge(136, 133, 15); // Gate to gate guest house
    graph.addEdge(135, 136, 47); // Girls hostel main gate
    graph.addEdge(136, 137, 12); // Girls hostel entrance gate
    graph.addEdge(136, 138, 27); // Temple
    graph.addEdge(115, 139, 10); // 2nd gate of fire station
    graph.addEdge(97, 140, 29);  // Towards Gandhi Chowk
    graph.addEdge(140, 141, 22); // Towards Gandhi Chowk
    graph.addEdge(141, 142, 6);  // Towards amphitheater
    graph.addEdge(141, 143, 10); // Gandhi Chowk
    graph.addEdge(143, 22, 11);  // Towards other block
    graph.addEdge(141, 144, 17); // Towards 9th block
    graph.addEdge(141, 145, 10); // Towards body hostel
    graph.addEdge(145, 146, 38); // Towards play school
    graph.addEdge(146, 147, 5);  // Entrance of play school
    graph.addEdge(146, 148, 14); // Front of canteen
    graph.addEdge(148, 149, 12); // Gate of canteen
    graph.addEdge(148, 150, 33); // Front of Tulip towards boys hotel 
    graph.addEdge(150, 151, 4);  // Tulip 2nd gate towards boys hostel
    graph.addEdge(150, 152, 23); // Front of laboratory
    graph.addEdge(152, 153, 4);  // Gate of Laboratory
    graph.addEdge(152, 154, 10); // Towards car gate of boys hostel
    graph.addEdge(154, 155, 5);  // Car gate of boys hostel
    graph.addEdge(154, 156, 6);  // Towards boys hostel entrance
    graph.addEdge(156, 157, 6);  // Boys hostel gate
    graph.addEdge(152, 159, 10); // Sitting area towards Tulip
    graph.addEdge(159, 160, 18); // Front of Tulip
    graph.addEdge(160, 161, 2);  // Towards Tulip 1st gate
    graph.addEdge(161, 162, 4);  // 1st gate of Tulip
    graph.addEdge(159, 163, 17); // Towards 11th block
    graph.addEdge(163, 164, 3);  // Towards 11th block 1st gate
    graph.addEdge(168, 184, 4);  // Towards play school
    graph.addEdge(184, 185, 8);  // Towards amphitheater
    graph.addEdge(185, 186, 13); // Towards 9th block
    graph.addEdge(185, 24, 13);  // Towards 3rd block
    graph.addEdge(186, 27, 4);   // Towards 9th block main entrance
    graph.addEdge(186, 25, 12);  // Towards 3rd block
    graph.addEdge(188, 179, 4);  // Back gate stair of 9th block
    graph.addEdge(179, 189, 8);  // Stair main gate of 10th block
    graph.addEdge(189, 190, 3);  // Main gate of 10th block
    graph.addEdge(188, 191, 8);  // Back gate of 9th block
    graph.addEdge(191, 192, 34); // Towards mid of 9th block
    graph.addEdge(192, 187, 11); // Mid gate of 9th block
    graph.addEdge(192, 182, 31); // Front gate of 9th block
    graph.addEdge(190, 193, 27); // Mid of 10th block
    graph.addEdge(193, 176, 24); // End gate of 10th block
    graph.addEdge(193, 194, 11); // Mid gate of 10th block
    graph.addEdge(194, 195, 3);  // Between 11th and 10th block
    graph.addEdge(195, 196, 5);  // Downside gate of 11th block
    graph.addEdge(196, 197, 4);  // Towards 11th block stairs
    graph.addEdge(197, 198, 6);  // Towards 11th block stairs
    graph.addEdge(198, 199, 4);  // Towards 11th block stairs
    graph.addEdge(199, 200, 20); // Towards 11th block main gate
    graph.addEdge(200, 201, 4);  // Towards 11th block gate
    graph.addEdge(201, 202, 1);  // Towards main gate of 11th block
    graph.addEdge(202, 203, 3);  // Main gate of 11th block
    graph.addEdge(202, 204, 12); // Towards intersection part
    graph.addEdge(203, 205, 2);  // Towards main road
    graph.addEdge(205, 164, 16); // 11th block main road
    graph.addEdge(175, 195, 23); // Backroad of 11th and 10th block
    graph.addEdge(206, 207, 3);  // Towards mid of 10th and 11th block
    graph.addEdge(207, 195, 23); // Mid of 10th and 11th block
    graph.addEdge(170, 204, 18); // Intersection point
    graph.addEdge(170, 208, 17); // Towards 3rd gate of 11th block
    graph.addEdge(170, 209, 19); // Mid gate of 11th block
    graph.addEdge(208, 210, 8);  // Last gate to entrance
    graph.addEdge(210, 211, 3);  // Last gate inside 11th block
    graph.addEdge(211, 212, 8);  // Inside 11th block
    graph.addEdge(212, 213, 25); // Inside 11th block
    graph.addEdge(213, 214, 23); // Mid gate of 11th block
    graph.addEdge(214, 209, 4);  // Mid gate entrance
    graph.addEdge(214, 215, 25); // Towards main gate of 11th block
    graph.addEdge(215, 199, 5);  // Towards stair of 11th block
    graph.addEdge(30, 216, 6);   // Towards Frisco
    graph.addEdge(216, 217, 4);  // Towards 3rd block
    graph.addEdge(216, 218, 7);  // Towards Frisco
    graph.addEdge(219, 31, 10);  // Towards 5th block
    graph.addEdge(219, 220, 5);  // 5th block gate
    graph.addEdge(219, 221, 7);  // Towards Frisco
    graph.addEdge(221, 222, 6);  // Towards Frisco entrance
    graph.addEdge(218, 222, 5);  // Towards Frisco entrance
    graph.addEdge(222, 223, 3);  // Frisco entrance
    graph.addEdge(218, 224, 16); //
    graph.addEdge(221, 225, 16); //
    graph.addEdge(218, 225, 18); 
    graph.addEdge(224, 221, 18); //
    graph.addEdge(224, 225, 12); //
    graph.addEdge(225, 85, 16); 
    graph.addEdge(224, 72, 15); //
    graph.addEdge(225, 226, 29); //
    graph.addEdge(226, 81, 5); 
    graph.addEdge(226, 227, 8); //
    graph.addEdge(227, 228, 6); //
    graph.addEdge(228, 229, 4);
    graph.addEdge(228, 224, 27); 
    graph.addEdge(229, 76, 4);
    graph.addEdge(228, 77, 4); 
    graph.addEdge(227, 230, 3); 
    graph.addEdge(231, 50, 12);
    graph.addEdge(231, 48, 9);
 
    }
    else if(type_type==1 && time_type==0 && weather_type==0){
        graph.addEdge(0, 1, 19);    // Main Gate
        graph.addEdge(1, 2, 13);    // towards enrollment office
        graph.addEdge(2, 3, 35);    // towards enrollment office
        graph.addEdge(3, 4, 27);    // Front of enrollment office
        graph.addEdge(4, 5, 21);    // Door of Enrollment office
        graph.addEdge(4, 6, 28);    // Front of amphitheater
        graph.addEdge(6, 7, 35);    // Front of sitting area
        graph.addEdge(7, 8, 7);     // Route for sitting area
        graph.addEdge(8, 9, 1);     // Sitting area
        graph.addEdge(7, 10, 12);   // Front of GYM & MAC
        graph.addEdge(10, 11, 11);  // GYM Gate
        graph.addEdge(10, 12, 11);  // MAC Gate
        graph.addEdge(10, 13, 10);  // Parking start front of MAC
    graph.addEdge(13, 14, 11);  // Front of box
    graph.addEdge(14, 15, 17);  // End of box
    graph.addEdge(15, 16, 16);  // Staircase front of MAC
    graph.addEdge(16, 17, 14);  // Front of Library
    graph.addEdge(17, 18, 17);  // End of Library
    graph.addEdge(18, 19, 32);  // Between blocks
    graph.addEdge(19, 20, 7);   // Last stairs
    graph.addEdge(20, 21, 23);  // Start of Gandhi Chowk
    graph.addEdge(21, 22, 10);  // Front of Gandhi Chowk
    graph.addEdge(22, 23, 6);   // Gandhi Chowk
    graph.addEdge(22, 24, 5);   // Front of 9th block amphitheater
    graph.addEdge(24, 25, 20);  // Back side of 3rd block & front of amphitheater
    graph.addEdge(25, 26, 3);   // Back of 3rd block
    graph.addEdge(25, 28, 13);  // Backside of 9th block
    graph.addEdge(28, 29, 3);   // Backside of Frisco
    graph.addEdge(29, 30, 12);  // Backside of Frisco towards food court
    graph.addEdge(30, 31, 5);   // Backside of 5th block
    graph.addEdge(31, 32, 3);   // Backside entrance of 5th block
    graph.addEdge(31, 33, 27);  // Backside of Food Court
    graph.addEdge(33, 34, 20);  // Front side of food court 1st gate
    graph.addEdge(34, 35, 10);  // Main gate of food court
    graph.addEdge(34, 36, 9);   // Stair towards food court
    graph.addEdge(34, 37, 7);   // Front of food court
    graph.addEdge(37, 38, 6);   // Towards Hubble
    graph.addEdge(38, 39, 8);   // Food court 2nd gate towards Hubble
    graph.addEdge(39, 40, 4);   // 2nd gate of food court
    graph.addEdge(39, 41, 14);  // Narrow route towards main gate through food court
    graph.addEdge(41, 42, 17);  // Front route towards main gate
    graph.addEdge(42, 43, 15);  // Connecting route towards main gate / back side of Hubble
    graph.addEdge(42, 68, 14);  // Food court1 back gate 
    graph.addEdge(68, 69, 2);   // Inside food court through back gate
    graph.addEdge(69, 70, 2);   // Inside food court towards 2nd gate
    graph.addEdge(70, 40, 10);  // Inside food court towards 2nd gate outwards
    graph.addEdge(69, 71, 10);  // Inside food court towards 1st gate
    graph.addEdge(71, 35, 9);   // Inside food court towards 1st gate outwards
    graph.addEdge(43, 44, 24);  // Towards main gate back of Hubble
    graph.addEdge(44, 45, 10);  // Towards main gate back of Hubble
    graph.addEdge(45, 1, 57);   // Front of gate
    graph.addEdge(1, 46, 15);   // Towards front of Hubble
    graph.addEdge(46, 47, 25);  // Front of Hubble
    graph.addEdge(47, 48, 4);   // Hubble main gate
    graph.addEdge(47, 49, 10);  // Hubble towards 1st block
    graph.addEdge(49, 3, 20);   // Hubble towards enrollment office
    graph.addEdge(49, 50, 9);   // Side of Hubble towards 1st block
    graph.addEdge(50, 51, 6);   // Towards 1st block
    graph.addEdge(51, 52, 52);  // Towards food court
    graph.addEdge(52, 53, 4);   // Towards food court backside of Hubble
    graph.addEdge(53, 38, 20);  // Front side of 2nd gate of food court
    graph.addEdge(53, 43, 23);  // Backside of Hubble
    graph.addEdge(51, 54, 9);   // 1st door of 1st block
    graph.addEdge(54, 55, 12);  // Towards amphitheater
    graph.addEdge(55, 49, 13);  // Towards Hubble
    graph.addEdge(54, 56, 9);   // Inside 1st block & front of wash room
    graph.addEdge(56, 57, 32);  // Inside 1st block & towards 4th block
    graph.addEdge(57, 58, 32);  // Inside 1st block towards 2nd block
    graph.addEdge(58, 59, 20);  // Inside 1st block towards 2nd block
    graph.addEdge(59, 56, 32);  // Inside 1st block towards Library stairs
    graph.addEdge(59, 60, 7);   // Exit of 1st block through library stairs
    graph.addEdge(60, 61, 7);   // Towards library stairs
    graph.addEdge(61, 62, 14);  // Front side of 1st block
    graph.addEdge(62, 55, 54);  // Front side of 1st block & towards Hubble
    graph.addEdge(62, 63, 15);  // Climbing through lib stairs
    graph.addEdge(63, 64, 30);  // Towards MAC
    graph.addEdge(64, 7, 17);   // Towards sitting area behind MAC
    graph.addEdge(64, 10, 9);   // Towards MAC/GYM
    graph.addEdge(63, 65, 17);  // Towards lib stairs
    graph.addEdge(65, 16, 9);   // Towards Library
    graph.addEdge(65, 66, 61);  // Towards Gandhi Chowk
    graph.addEdge(66, 67, 10);  // Towards Gandhi Chowk
    graph.addEdge(66, 20, 13);  // Towards design block
    graph.addEdge(66, 24, 28);  // Towards Gandhi Chowk
    graph.addEdge(66, 72, 18);  // From stairs to 3rd block
    graph.addEdge(72, 73, 2);   // Gate of 3rd block
    graph.addEdge(72, 74, 3);   // Back gate of 2nd block
    graph.addEdge(74, 75, 22);  // Towards 2nd block
    graph.addEdge(75, 76, 6);   // 2nd block stairs
    graph.addEdge(76, 77, 2);   // 2nd block gate
    graph.addEdge(77, 78, 4);   // 2nd block to 1st block
    graph.addEdge(78, 58, 16);  // Inside 1st block towards Hubble
    graph.addEdge(61, 79, 46);  // Outside 1st block path
    graph.addEdge(79, 77, 10);  // Outside 1st block to 2nd block 2nd path
    graph.addEdge(57, 80, 10);  // 1st block to 4th block
    graph.addEdge(80, 81, 8);   // 1st block path to 4th block path
    graph.addEdge(81, 82, 3);   // 4th block gate
    graph.addEdge(82, 83, 3);   // 4th block stairs
    graph.addEdge(83, 84, 9);   // 4th block stair continues
    graph.addEdge(84, 85, 22);  // 4th block last gate
    graph.addEdge(85, 86, 4);   // 4th to 5th block
    graph.addEdge(86, 87, 2);   // 5th block gate
    graph.addEdge(52, 81, 22);  // Hubble to 4th block gate & 1st block gate
    graph.addEdge(86, 87, 23);  // 5th block stairs
    graph.addEdge(87, 88, 5);   // 5th block stairs
    graph.addEdge(88, 32, 8);   // 5th block gate
    graph.addEdge(17, 89, 51);  // Main road to library (staircase)
    graph.addEdge(18, 93, 39);  // Main road to side of IT
    graph.addEdge(90, 93, 12);  // Main road to IT (staircase)
    graph.addEdge(89, 90, 18);  // Library to IT
    graph.addEdge(89, 91, 3);   // Main gate of Library
    graph.addEdge(90, 92, 3);   // Main gate of IT
    graph.addEdge(93, 94, 26);  // Path towards 6th block
    graph.addEdge(94, 95, 24);  // Path towards backside of 6th block
    graph.addEdge(95, 96, 17);  // Path 6th block main entrance
    graph.addEdge(96, 97, 31);  // Path from 6th block to main road
    graph.addEdge(96, 98, 5);   // 6th block main entrance
    graph.addEdge(95, 99, 31);  // 6th to library road
    graph.addEdge(99, 90, 18);  // IT to main road
    graph.addEdge(99, 100, 15); // Main road from library to IT
    graph.addEdge(100, 89, 15); // Main road to Library
    graph.addEdge(100, 101, 26); // Main road to food court 2
    graph.addEdge(101, 102, 31); // Main road towards to backside of foodcourt
    graph.addEdge(102, 103, 15); // Food court backside road
    graph.addEdge(103, 104, 16); // Backside of food court to backside of MAC
    graph.addEdge(104, 105, 11); // Backside of MAC
    graph.addEdge(105, 106, 43); // Towards power house
    graph.addEdge(106, 107, 5);  // Power house gate
    graph.addEdge(107, 108, 9);  // Power house to basketball court
    graph.addEdge(108, 109, 47); // Basketball court
    graph.addEdge(109, 110, 54); // Basketball court exit 
    graph.addEdge(110, 111, 10); // Towards Placement office
    graph.addEdge(111, 112, 3);  // Main road of placement office
    graph.addEdge(103, 113, 14); // Box from food court back
    graph.addEdge(113, 114, 11); // Box mid to box mid
    graph.addEdge(114, 14, 11);  // Box to main road
    graph.addEdge(115, 97, 66);  // Main road to front of game office
    graph.addEdge(115, 116, 7);  // Main road to game office
    graph.addEdge(116, 117, 8);  // Main road for placement activity
    graph.addEdge(117, 118, 47); // Main road towards placement activity
    graph.addEdge(118, 119, 3);  // Towards ground
    graph.addEdge(118, 112, 50); // Basketball court
    graph.addEdge(120, 112, 7);  // Basketball entrance to ground entrance
    graph.addEdge(112, 121, 36); // Basketball entrance to placement activity block
    graph.addEdge(121, 122, 33); // Inside placement block
    graph.addEdge(122, 123, 25); // Exit from placement block
    graph.addEdge(123, 124, 71); // Corner of ground
    graph.addEdge(124, 125, 104); // Other side of ground
    graph.addEdge(125, 126, 32); // Front side of ground
    graph.addEdge(126, 127, 18); // Towards ground
    graph.addEdge(127, 128, 25); // Front side of ground
    graph.addEdge(128, 129, 36); // Towards basketball ground
    graph.addEdge(129, 130, 10); // Basketball back
    graph.addEdge(130, 119, 13); // Basketball back
    graph.addEdge(131, 128, 40); // Intersection in front of MDP Guest house
    graph.addEdge(131, 117, 28); // Entrance of ground to intersection of MDP Guest house
    graph.addEdge(131, 132, 5);  // Towards Guest house front of Ground
    graph.addEdge(132, 133, 34); // Front gate of guest house
    graph.addEdge(117, 134, 8);  // Museum way
    graph.addEdge(131, 135, 10); // Towards guest house and girls hostel
    graph.addEdge(135, 136, 28); // 2nd gate of guest house
    graph.addEdge(136, 133, 15); // Gate to gate guest house
    graph.addEdge(135, 136, 47); // Girls hostel main gate
    graph.addEdge(136, 137, 12); // Girls hostel entrance gate
    graph.addEdge(136, 138, 27); // Temple
    graph.addEdge(115, 139, 10); // 2nd gate of fire station
    graph.addEdge(97, 140, 29);  // Towards Gandhi Chowk
    graph.addEdge(140, 141, 22); // Towards Gandhi Chowk
    graph.addEdge(141, 142, 6);  // Towards amphitheater
    graph.addEdge(141, 143, 10); // Gandhi Chowk
    graph.addEdge(143, 22, 11);  // Towards other block
    graph.addEdge(141, 144, 17); // Towards 9th block
    graph.addEdge(141, 145, 10); // Towards body hostel
    graph.addEdge(145, 146, 38); // Towards play school
    graph.addEdge(146, 147, 5);  // Entrance of play school
    graph.addEdge(146, 148, 14); // Front of canteen
    graph.addEdge(148, 149, 12); // Gate of canteen
    graph.addEdge(148, 150, 33); // Front of Tulip towards boys hotel 
    graph.addEdge(150, 151, 4);  // Tulip 2nd gate towards boys hostel
    graph.addEdge(150, 152, 23); // Front of laboratory
    graph.addEdge(152, 153, 4);  // Gate of Laboratory
    graph.addEdge(152, 154, 10); // Towards car gate of boys hostel
    graph.addEdge(154, 155, 5);  // Car gate of boys hostel
    graph.addEdge(154, 156, 6);  // Towards boys hostel entrance
    graph.addEdge(156, 157, 6);  // Boys hostel gate
    graph.addEdge(152, 159, 10); // Sitting area towards Tulip
    graph.addEdge(159, 160, 18); // Front of Tulip
    graph.addEdge(160, 161, 2);  // Towards Tulip 1st gate
    graph.addEdge(161, 162, 4);  // 1st gate of Tulip
    graph.addEdge(159, 163, 17); // Towards 11th block
    graph.addEdge(163, 164, 3);  // Towards 11th block 1st gate
    graph.addEdge(164, 165, 21); // 11th and 10th block
    graph.addEdge(165, 166, 23); // Towards Gandhi Chowk 9th and 10th block
    graph.addEdge(166, 167, 36); // Towards Gandhi Chowk 9th block
    graph.addEdge(167, 168, 33); // Towards Gandhi Chowk 9th last gate
    graph.addEdge(168, 184, 4);  // Towards play school
    graph.addEdge(184, 185, 8);  // Towards amphitheater
    graph.addEdge(185, 186, 13); // Towards 9th block
    graph.addEdge(185, 24, 13);  // Towards 3rd block
    graph.addEdge(186, 27, 4);   // Towards 9th block main entrance
    graph.addEdge(186, 25, 12);  // Towards 3rd block
    graph.addEdge(187, 167, 5);  // Mid gate of 9th block
    graph.addEdge(188, 179, 4);  // Back gate stair of 9th block
    graph.addEdge(179, 189, 8);  // Stair main gate of 10th block
    graph.addEdge(189, 190, 3);  // Main gate of 10th block
    graph.addEdge(188, 191, 8);  // Back gate of 9th block
    graph.addEdge(191, 192, 34); // Towards mid of 9th block
    graph.addEdge(192, 187, 11); // Mid gate of 9th block
    graph.addEdge(192, 182, 31); // Front gate of 9th block
    graph.addEdge(190, 193, 27); // Mid of 10th block
    graph.addEdge(193, 176, 24); // End gate of 10th block
    graph.addEdge(193, 194, 11); // Mid gate of 10th block
    graph.addEdge(194, 195, 3);  // Between 11th and 10th block
    graph.addEdge(195, 196, 5);  // Downside gate of 11th block
    graph.addEdge(196, 197, 4);  // Towards 11th block stairs
    graph.addEdge(197, 198, 6);  // Towards 11th block stairs
    graph.addEdge(198, 199, 4);  // Towards 11th block stairs
    graph.addEdge(199, 200, 20); // Towards 11th block main gate
    graph.addEdge(200, 201, 4);  // Towards 11th block gate
    graph.addEdge(201, 202, 1);  // Towards main gate of 11th block
    graph.addEdge(202, 203, 3);  // Main gate of 11th block
    graph.addEdge(202, 204, 12); // Towards intersection part
    graph.addEdge(203, 205, 2);  // Towards main road
    graph.addEdge(205, 164, 16); // 11th block main road
    graph.addEdge(175, 195, 23); // Backroad of 11th and 10th block
    graph.addEdge(165, 206, 19); // Main road towards mid of 11th and 10th block
    graph.addEdge(206, 207, 3);  // Towards mid of 10th and 11th block
    graph.addEdge(207, 195, 23); // Mid of 10th and 11th block
    graph.addEdge(170, 204, 18); // Intersection point
    graph.addEdge(170, 208, 17); // Towards 3rd gate of 11th block
    graph.addEdge(170, 209, 19); // Mid gate of 11th block
    graph.addEdge(208, 210, 8);  // Last gate to entrance
    graph.addEdge(210, 211, 3);  // Last gate inside 11th block
    graph.addEdge(211, 212, 8);  // Inside 11th block
    graph.addEdge(212, 213, 25); // Inside 11th block
    graph.addEdge(213, 214, 23); // Mid gate of 11th block
    graph.addEdge(214, 209, 4);  // Mid gate entrance
    graph.addEdge(214, 215, 25); // Towards main gate of 11th block
    graph.addEdge(215, 199, 5);  // Towards stair of 11th block
    graph.addEdge(30, 216, 6);   // Towards Frisco
    graph.addEdge(216, 217, 4);  // Towards 3rd block
    graph.addEdge(216, 218, 7);  // Towards Frisco
    graph.addEdge(219, 31, 10);  // Towards 5th block
    graph.addEdge(219, 220, 5);  // 5th block gate
    graph.addEdge(219, 221, 7);  // Towards Frisco
    graph.addEdge(221, 222, 6);  // Towards Frisco entrance
    graph.addEdge(218, 222, 5);  // Towards Frisco entrance
    graph.addEdge(222, 223, 3);  // Frisco entrance
    graph.addEdge(218, 224, 16); //
    graph.addEdge(221, 225, 16); //
    graph.addEdge(218, 225, 18); 
    graph.addEdge(224, 221, 18); //
    graph.addEdge(224, 225, 12); //
    graph.addEdge(225, 85, 16); 
    graph.addEdge(224, 72, 15); //
    graph.addEdge(225, 226, 29); //
    graph.addEdge(226, 81, 5); 
    graph.addEdge(226, 227, 8); //
    graph.addEdge(227, 228, 6); //
    graph.addEdge(228, 229, 4);
    graph.addEdge(228, 224, 27); 
    graph.addEdge(229, 76, 4);
    graph.addEdge(228, 77, 4); 
    graph.addEdge(227, 230, 3); 
    graph.addEdge(231, 50, 12);
    graph.addEdge(231, 48, 9);
    }
       // ... (add all other edges)

    else if(type_type==1 && time_type==1){
        graph.addEdge(0, 1, 19);    // Main Gate
    graph.addEdge(2, 3, 35);    // towards enrollment office
    graph.addEdge(3, 4, 27);    // Front of enrollment office
    graph.addEdge(4, 5, 21);    // Door of Enrollment office
    graph.addEdge(4, 6, 28);    // Front of amphitheater
    graph.addEdge(6, 7, 35);    // Front of sitting area
    graph.addEdge(7, 8, 7);     // Route for sitting area
    graph.addEdge(8, 9, 1);     // Sitting area
    graph.addEdge(7, 10, 12);   // Front of GYM & MAC
    graph.addEdge(10, 11, 11);  // GYM Gate
    graph.addEdge(10, 12, 11);  // MAC Gate
    graph.addEdge(10, 13, 10);  // Parking start front of MAC
    graph.addEdge(13, 14, 11);  // Front of box
    graph.addEdge(14, 15, 17);  // End of box
    graph.addEdge(15, 16, 16);  // Staircase front of MAC
    graph.addEdge(16, 17, 14);  // Front of Library
    graph.addEdge(17, 18, 17);  // End of Library
    graph.addEdge(18, 19, 32);  // Between blocks
    graph.addEdge(19, 20, 7);   // Last stairs
    graph.addEdge(20, 21, 23);  // Start of Gandhi Chowk
    graph.addEdge(21, 22, 10);  // Front of Gandhi Chowk
    graph.addEdge(22, 23, 6);   // Gandhi Chowk
    graph.addEdge(22, 24, 5);   // Front of 9th block amphitheater
    graph.addEdge(24, 25, 20);  // Back side of 3rd block & front of amphitheater
    graph.addEdge(25, 26, 3);   // Back of 3rd block
    graph.addEdge(25, 28, 13);  // Backside of 9th block
    graph.addEdge(28, 29, 3);   // Backside of Frisco
    graph.addEdge(29, 30, 12);  // Backside of Frisco towards food court
    graph.addEdge(30, 31, 5);   // Backside of 5th block
    graph.addEdge(31, 32, 3);   // Backside entrance of 5th block
    graph.addEdge(31, 33, 27);  // Backside of Food Court
    graph.addEdge(33, 34, 20);  // Front side of food court 1st gate
    graph.addEdge(34, 35, 10);  // Main gate of food court
    graph.addEdge(34, 36, 9);   // Stair towards food court
    graph.addEdge(34, 37, 7);   // Front of food court
    graph.addEdge(37, 38, 6);   // Towards Hubble
    graph.addEdge(38, 39, 8);   // Food court 2nd gate towards Hubble
    graph.addEdge(39, 40, 4);   // 2nd gate of food court
    graph.addEdge(39, 41, 14);  // Narrow route towards main gate through food court
    graph.addEdge(41, 42, 17);  // Front route towards main gate
    graph.addEdge(42, 43, 15);  // Connecting route towards main gate / back side of Hubble
    graph.addEdge(42, 68, 14);  // Food court1 back gate 
    graph.addEdge(68, 69, 2);   // Inside food court through back gate
    graph.addEdge(69, 70, 2);   // Inside food court towards 2nd gate
    graph.addEdge(70, 40, 10);  // Inside food court towards 2nd gate outwards
    graph.addEdge(69, 71, 10);  // Inside food court towards 1st gate
    graph.addEdge(71, 35, 9);   // Inside food court towards 1st gate outwards
    graph.addEdge(43, 44, 24);  // Towards main gate back of Hubble
    graph.addEdge(44, 45, 10);  // Towards main gate back of Hubble
    graph.addEdge(45, 1, 57);   // Front of gate
    graph.addEdge(46, 47, 25);  // Front of Hubble
    graph.addEdge(47, 48, 4);   // Hubble main gate
    graph.addEdge(47, 49, 10);  // Hubble towards 1st block
    graph.addEdge(49, 3, 20);   // Hubble towards enrollment office
    graph.addEdge(49, 50, 9);   // Side of Hubble towards 1st block
    graph.addEdge(50, 51, 6);   // Towards 1st block
    graph.addEdge(51, 52, 52);  // Towards food court
    graph.addEdge(52, 53, 4);   // Towards food court backside of Hubble
    graph.addEdge(53, 38, 20);  // Front side of 2nd gate of food court
    graph.addEdge(53, 43, 23);  // Backside of Hubble
    graph.addEdge(51, 54, 9);   // 1st door of 1st block
    graph.addEdge(54, 55, 12);  // Towards amphitheater
    graph.addEdge(55, 49, 13);  // Towards Hubble
    graph.addEdge(54, 56, 9);   // Inside 1st block & front of wash room
    graph.addEdge(56, 57, 32);  // Inside 1st block & towards 4th block
    graph.addEdge(57, 58, 32);  // Inside 1st block towards 2nd block
    graph.addEdge(58, 59, 20);  // Inside 1st block towards 2nd block
    graph.addEdge(59, 56, 32);  // Inside 1st block towards Library stairs
    graph.addEdge(59, 60, 7);   // Exit of 1st block through library stairs
    graph.addEdge(60, 61, 7);   // Towards library stairs
    graph.addEdge(61, 62, 14);  // Front side of 1st block
    graph.addEdge(62, 55, 54);  // Front side of 1st block & towards Hubble
    graph.addEdge(62, 63, 15);  // Climbing through lib stairs
    graph.addEdge(63, 64, 30);  // Towards MAC
    graph.addEdge(64, 7, 17);   // Towards sitting area behind MAC
    graph.addEdge(64, 10, 9);   // Towards MAC/GYM
    graph.addEdge(63, 65, 17);  // Towards lib stairs
    graph.addEdge(65, 16, 9);   // Towards Library
    graph.addEdge(65, 66, 61);  // Towards Gandhi Chowk
    graph.addEdge(66, 67, 10);  // Towards Gandhi Chowk
    graph.addEdge(66, 20, 13);  // Towards design block
    graph.addEdge(66, 24, 28);  // Towards Gandhi Chowk
    graph.addEdge(66, 72, 18);  // From stairs to 3rd block
    graph.addEdge(72, 73, 2);   // Gate of 3rd block
    graph.addEdge(72, 74, 3);   // Back gate of 2nd block
    graph.addEdge(74, 75, 22);  // Towards 2nd block
    graph.addEdge(75, 76, 6);   // 2nd block stairs
    graph.addEdge(76, 77, 2);   // 2nd block gate
    graph.addEdge(77, 78, 4);   // 2nd block to 1st block
    graph.addEdge(78, 58, 16);  // Inside 1st block towards Hubble
    graph.addEdge(61, 79, 46);  // Outside 1st block path
    graph.addEdge(79, 77, 10);  // Outside 1st block to 2nd block 2nd path
    graph.addEdge(57, 80, 10);  // 1st block to 4th block
    graph.addEdge(80, 81, 8);   // 1st block path to 4th block path
    graph.addEdge(81, 82, 3);   // 4th block gate
    graph.addEdge(82, 83, 3);   // 4th block stairs
    graph.addEdge(83, 84, 9);   // 4th block stair continues
    graph.addEdge(84, 85, 22);  // 4th block last gate
    graph.addEdge(85, 86, 4);   // 4th to 5th block
    graph.addEdge(86, 87, 2);   // 5th block gate
    graph.addEdge(52, 81, 22);  // Hubble to 4th block gate & 1st block gate
    graph.addEdge(86, 87, 23);  // 5th block stairs
    graph.addEdge(87, 88, 5);   // 5th block stairs
    graph.addEdge(88, 32, 8);   // 5th block gate
    graph.addEdge(17, 89, 51);  // Main road to library (staircase)
    graph.addEdge(18, 93, 39);  // Main road to side of IT
    graph.addEdge(90, 93, 12);  // Main road to IT (staircase)
    graph.addEdge(89, 90, 18);  // Library to IT
    graph.addEdge(89, 91, 3);   // Main gate of Library
    graph.addEdge(90, 92, 3);   // Main gate of IT
    graph.addEdge(93, 94, 26);  // Path towards 6th block
    graph.addEdge(94, 95, 24);  // Path towards backside of 6th block
    graph.addEdge(95, 96, 17);  // Path 6th block main entrance
    graph.addEdge(96, 97, 31);  // Path from 6th block to main road
    graph.addEdge(96, 98, 5);   // 6th block main entrance
    graph.addEdge(95, 99, 31);  // 6th to library road
    graph.addEdge(99, 90, 18);  // IT to main road
    graph.addEdge(99, 100, 15); // Main road from library to IT
    graph.addEdge(100, 89, 15); // Main road to Library
    graph.addEdge(100, 101, 26); // Main road to food court 2
    graph.addEdge(101, 102, 31); // Main road towards to backside of foodcourt
    graph.addEdge(102, 103, 15); // Food court backside road
    graph.addEdge(103, 104, 16); // Backside of food court to backside of MAC
    graph.addEdge(104, 105, 11); // Backside of MAC
    graph.addEdge(105, 106, 43); // Towards power house
    graph.addEdge(106, 107, 5);  // Power house gate
    graph.addEdge(107, 108, 9);  // Power house to basketball court
    graph.addEdge(108, 109, 47); // Basketball court
    graph.addEdge(109, 110, 54); // Basketball court exit 
    graph.addEdge(110, 111, 10); // Towards Placement office
    graph.addEdge(111, 112, 3);  // Main road of placement office
    graph.addEdge(103, 113, 14); // Box from food court back
    graph.addEdge(113, 114, 11); // Box mid to box mid
    graph.addEdge(114, 14, 11);  // Box to main road
    graph.addEdge(115, 97, 66);  // Main road to front of game office
    graph.addEdge(115, 116, 7);  // Main road to game office
    graph.addEdge(116, 117, 8);  // Main road for placement activity
    graph.addEdge(117, 118, 47); // Main road towards placement activity
    graph.addEdge(118, 119, 3);  // Towards ground
    graph.addEdge(118, 112, 50); // Basketball court
    graph.addEdge(120, 112, 7);  // Basketball entrance to ground entrance
    graph.addEdge(112, 121, 36); // Basketball entrance to placement activity block
    graph.addEdge(121, 122, 33); // Inside placement block
    graph.addEdge(122, 123, 25); // Exit from placement block
    graph.addEdge(123, 124, 71); // Corner of ground
    graph.addEdge(124, 125, 104); // Other side of ground
    graph.addEdge(125, 126, 32); // Front side of ground
    graph.addEdge(126, 127, 18); // Towards ground
    graph.addEdge(127, 128, 25); // Front side of ground
    graph.addEdge(128, 129, 36); // Towards basketball ground
    graph.addEdge(129, 130, 10); // Basketball back
    graph.addEdge(130, 119, 13); // Basketball back
    graph.addEdge(131, 128, 40); // Intersection in front of MDP Guest house
    graph.addEdge(131, 117, 28); // Entrance of ground to intersection of MDP Guest house
    graph.addEdge(131, 132, 5);  // Towards Guest house front of Ground
    graph.addEdge(132, 133, 34); // Front gate of guest house
    graph.addEdge(117, 134, 8);  // Museum way
    graph.addEdge(131, 135, 10); // Towards guest house and girls hostel
    graph.addEdge(135, 136, 28); // 2nd gate of guest house
    graph.addEdge(136, 133, 15); // Gate to gate guest house
    graph.addEdge(135, 136, 47); // Girls hostel main gate
    graph.addEdge(136, 137, 12); // Girls hostel entrance gate
    graph.addEdge(136, 138, 27); // Temple
    graph.addEdge(115, 139, 10); // 2nd gate of fire station
    graph.addEdge(97, 140, 29);  // Towards Gandhi Chowk
    graph.addEdge(140, 141, 22); // Towards Gandhi Chowk
    graph.addEdge(141, 142, 6);  // Towards amphitheater
    graph.addEdge(141, 143, 10); // Gandhi Chowk
    graph.addEdge(143, 22, 11);  // Towards other block
    graph.addEdge(141, 144, 17); // Towards 9th block
    graph.addEdge(141, 145, 10); // Towards body hostel
    graph.addEdge(145, 146, 38); // Towards play school
    graph.addEdge(146, 147, 5);  // Entrance of play school
    graph.addEdge(146, 148, 14); // Front of canteen
    graph.addEdge(148, 149, 12); // Gate of canteen
    graph.addEdge(148, 150, 33); // Front of Tulip towards boys hotel 
    graph.addEdge(150, 151, 4);  // Tulip 2nd gate towards boys hostel
    graph.addEdge(150, 152, 23); // Front of laboratory
    graph.addEdge(152, 153, 4);  // Gate of Laboratory
    graph.addEdge(152, 154, 10); // Towards car gate of boys hostel
    graph.addEdge(154, 155, 5);  // Car gate of boys hostel
    graph.addEdge(154, 156, 6);  // Towards boys hostel entrance
    graph.addEdge(156, 157, 6);  // Boys hostel gate
    graph.addEdge(152, 159, 10); // Sitting area towards Tulip
    graph.addEdge(159, 160, 18); // Front of Tulip
    graph.addEdge(160, 161, 2);  // Towards Tulip 1st gate
    graph.addEdge(161, 162, 4);  // 1st gate of Tulip
    graph.addEdge(159, 163, 17); // Towards 11th block
    graph.addEdge(163, 164, 3);  // Towards 11th block 1st gate
    graph.addEdge(164, 165, 21); // 11th and 10th block
    graph.addEdge(165, 166, 23); // Towards Gandhi Chowk 9th and 10th block
    graph.addEdge(166, 167, 36); // Towards Gandhi Chowk 9th block
    graph.addEdge(167, 168, 33); // Towards Gandhi Chowk 9th last gate
    graph.addEdge(168, 184, 4);  // Towards play school
    graph.addEdge(184, 185, 8);  // Towards amphitheater
    graph.addEdge(185, 186, 13); // Towards 9th block
    graph.addEdge(185, 24, 13);  // Towards 3rd block
    graph.addEdge(186, 27, 4);   // Towards 9th block main entrance
    graph.addEdge(186, 25, 12);  // Towards 3rd block
    graph.addEdge(187, 167, 5);  // Mid gate of 9th block
    graph.addEdge(188, 179, 4);  // Back gate stair of 9th block
    graph.addEdge(179, 189, 8);  // Stair main gate of 10th block
    graph.addEdge(189, 190, 3);  // Main gate of 10th block
    graph.addEdge(188, 191, 8);  // Back gate of 9th block
    graph.addEdge(191, 192, 34); // Towards mid of 9th block
    graph.addEdge(192, 187, 11); // Mid gate of 9th block
    graph.addEdge(192, 182, 31); // Front gate of 9th block
    graph.addEdge(190, 193, 27); // Mid of 10th block
    graph.addEdge(193, 176, 24); // End gate of 10th block
    graph.addEdge(193, 194, 11); // Mid gate of 10th block
    graph.addEdge(194, 195, 3);  // Between 11th and 10th block
    graph.addEdge(195, 196, 5);  // Downside gate of 11th block
    graph.addEdge(196, 197, 4);  // Towards 11th block stairs
    graph.addEdge(197, 198, 6);  // Towards 11th block stairs
    graph.addEdge(198, 199, 4);  // Towards 11th block stairs
    graph.addEdge(199, 200, 20); // Towards 11th block main gate
    graph.addEdge(200, 201, 4);  // Towards 11th block gate
    graph.addEdge(201, 202, 1);  // Towards main gate of 11th block
    graph.addEdge(202, 203, 3);  // Main gate of 11th block
    graph.addEdge(202, 204, 12); // Towards intersection part
    graph.addEdge(203, 205, 2);  // Towards main road
    graph.addEdge(205, 164, 16); // 11th block main road
    graph.addEdge(175, 195, 23); // Backroad of 11th and 10th block
    graph.addEdge(165, 206, 19); // Main road towards mid of 11th and 10th block
    graph.addEdge(206, 207, 3);  // Towards mid of 10th and 11th block
    graph.addEdge(207, 195, 23); // Mid of 10th and 11th block
    graph.addEdge(170, 204, 18); // Intersection point
    graph.addEdge(170, 208, 17); // Towards 3rd gate of 11th block
    graph.addEdge(170, 209, 19); // Mid gate of 11th block
    graph.addEdge(208, 210, 8);  // Last gate to entrance
    graph.addEdge(210, 211, 3);  // Last gate inside 11th block
    graph.addEdge(211, 212, 8);  // Inside 11th block
    graph.addEdge(212, 213, 25); // Inside 11th block
    graph.addEdge(213, 214, 23); // Mid gate of 11th block
    graph.addEdge(214, 209, 4);  // Mid gate entrance
    graph.addEdge(214, 215, 25); // Towards main gate of 11th block
    graph.addEdge(215, 199, 5);  // Towards stair of 11th block
    graph.addEdge(30, 216, 6);   // Towards Frisco
    graph.addEdge(216, 217, 4);  // Towards 3rd block
    graph.addEdge(216, 218, 7);  // Towards Frisco
    graph.addEdge(219, 31, 10);  // Towards 5th block
    graph.addEdge(219, 220, 5);  // 5th block gate
    graph.addEdge(219, 221, 7);  // Towards Frisco
    graph.addEdge(221, 222, 6);  // Towards Frisco entrance
    graph.addEdge(218, 222, 5);  // Towards Frisco entrance
    graph.addEdge(222, 223, 3);  // Frisco entrance
    graph.addEdge(218, 224, 16); //
    graph.addEdge(221, 225, 16); //
    graph.addEdge(218, 225, 18); 
    graph.addEdge(224, 221, 18); //
    graph.addEdge(224, 225, 12); //
    graph.addEdge(225, 85, 16); 
    graph.addEdge(224, 72, 15); //
    graph.addEdge(225, 226, 29); //
    graph.addEdge(226, 81, 5); 
    graph.addEdge(226, 227, 8); //
    graph.addEdge(227, 228, 6); //
    graph.addEdge(228, 229, 4);
    graph.addEdge(228, 224, 27); 
    graph.addEdge(229, 76, 4);
    graph.addEdge(228, 77, 4); 
    graph.addEdge(227, 230, 3); 
    graph.addEdge(231, 50, 12);
    graph.addEdge(231, 48, 9);
    
    }
    
    else{
        return 0;
    }
    aStar(graph, sourceNode, targetNode);

    return 0;
}