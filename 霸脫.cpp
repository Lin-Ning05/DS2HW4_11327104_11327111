//11327104林采寧 11327111林方晴

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <cstring>
#include <unordered_map>
struct Edge {
    char putID[12];
    char getID[12];
    float weight;
};

struct AdjacencyNode {
    char ID[12];
    float weight = 0;
};

struct ConnectNode {
    char ID[12];
};

struct PutID {
    char ID[12];
    int connectCount = 0;
    std::vector <ConnectNode> connectList;
};

class AdjacencyList {
  private:
    std::unordered_map<std::string, std::vector<AdjacencyNode>> adjList;
    std::unordered_map<std::string, int> visitedIndex;
    std::vector<PutID> sender;
    int listSize = 0;
    int edgeSum = 0;

    void SortPutID();
    void SortAdjNode(std::vector<AdjacencyNode> temp);
  public:
    ~AdjacencyList() {
        clear();
    }
    void clear();
    int getListSize() {return listSize;}
    void CreatList(std::vector<Edge> &Info);
    void WriteFileAdj(std::string num);
    void Connection(PutID todo);
    void WriteFileCnt(std::string num);
    void Sort();
};

/********************************************/
void PrintMenu();
std::string RemoveSpace(std::string target);
std::string RemoveDotTab(std::string target);
bool IsInt(std::string num);
void GetN(std::string temp , int &n);
bool SetInfo(std::vector<Edge> &Info , std::string &num);
bool ReadBin(std::string outputName , std::vector<Edge> &Info);
/*********************************************/

int main() {
    std::string verb;
    std::vector<Edge> Info;

    std::string num; // 檔名,Ex.301
    AdjacencyList adjacencyList;
    while(true) {
        PrintMenu();
        std::cin >> verb;
        verb = RemoveSpace(verb);
        if (verb == "0") break;
        else if (verb == "1") {
            Info.clear();
            adjacencyList.clear();
            if (!SetInfo(Info , num)) {//讀到0或檔案不存在的情況要停止執行
                continue; 
            } 
            adjacencyList.CreatList(Info);
            adjacencyList.WriteFileAdj(num);
        } else if (verb == "2") {
            if (adjacencyList.getListSize() == 0) {
                std::cout << "### There is no graph and choose 1 first. ###\n";
            } else {
                std::cout << "\n<<< There are " << adjacencyList.getListSize() << " IDs in total. >>>" << std::endl;
                for (int i = 0; i < adjacencyList.getListSize(); i++) {
                    //adjacencyList.Connection(i);
                }
                //adjacencyList.Sort();
                //adjacencyList.WriteFileCnt(num);
            }
        } else {
            std::cout << "\nCommand does not exist!\n";
        }
    }

    return 0;
}

/**********************************************************************/

void PrintMenu() {
    std::cout << std::endl;
    std::cout << "* Data Structures and Algorithms *" << std::endl;
    std::cout << "**** Graph data manipulation *****" << std::endl;
    std::cout << "* 0. QUIT                        *" << std::endl;
    std::cout << "* 1. Build adjacency lists       *" << std::endl;
    std::cout << "* 2. Compute connection counts   *" << std::endl;
    std::cout << "**********************************" << std::endl;
    std::cout << "Input a choice(0, 1, 2): ";
    return;
}

bool IsInt(std::string num) {
    if(num.size() == 0) {
        return false;
    }
    for (int i = 0; i < num.size(); i++) {
        if (i == 0 && (num[i] == '+' || num[i] == '-')) {
        continue;
    }
    if(num[i] > '9' || num[i] < '0') {
        return false;
    }
}
return true;
}

std::string RemoveSpace(std::string target) {
    std::string to_return;
    for (int i = 0; i < target.size() ;i++) {
        if (target[i] != ' ' && target[i] != '\t' && target[i] != '\n' && target[i] != '\r') {
            to_return = to_return + target[i];
        }
    }
    return to_return;
}

std::string RemoveDotTab(std::string target) {
    std::string to_return;
    for (int i = 0 ; i < target.size() ; i++) {
        if (target[i] == '.' || target[i] == '\t' || target[i] == ' ' || (target[i] == '\"' && i != 0)) {
            break;
        }
        if (target[i] != ',' && target[i] != '\"')
            to_return = to_return + target[i];
    }
    return to_return;
}

void GetN(std::string temp , int &n) {
    temp = RemoveDotTab(temp);
    temp = RemoveSpace(temp);
    if (IsInt(temp)) {
        n = stoi(temp);
    }
}

bool SetInfo(std::vector<Edge> &Info , std::string &num) {
    std::string fileName;
    std::string outputName;

    std::cout << "\nInput a file number ([0] Quit): ";
    std::cin >> num;
    num = RemoveSpace(num);
    fileName = "pairs" + num + ".bin";
    if (fileName == "pairs0.bin") return false;

    std::ifstream fin(fileName);
    if (ReadBin(fileName , Info)) {
        return true;
    } else {
        std::cout << std::endl << "### " << fileName << " does not exist! ###" << std::endl;
        return false;
    }
}

bool ReadBin(std::string fileName , std::vector<Edge> &Info) {
    std::ifstream fin(fileName, std::ios::binary);

    Edge edge;
    if (!fin.is_open()) return false;
    while (fin.read((char*)&edge, sizeof(Edge))) {
        Info.push_back(edge);
    }
    return true;
}

//-------------------AdjacencyList--------------------------//

//private
void AdjacencyList::SortPutID() {
   for (int i = 0; i < sender.size() - 1; i++) {
        for (int j = i + 1; j < sender.size(); j++) {
            if (strcmp(sender[i].ID , sender[j].ID) > 0) std::swap(sender[i], sender[j]);
        }
        visitedIndex[sender[i].ID] = i; // 第i個的資料確定
    } 
    visitedIndex[sender[sender.size() - 1].ID] = sender.size() - 1;// 設定最後一個
}

void AdjacencyList::SortAdjNode(std::vector<AdjacencyNode> temp) {
   for (int i = 0; i < temp.size() - 1; i++) {
        for (int j = i + 1; j < temp.size(); j++) {
            if (strcmp(temp[i].ID , temp[j].ID) > 0) 
                std::swap(temp[i], temp[j]);
        }
    } 
}

void AdjacencyList::Sort() {
   for (int i = 0; i < sender.size() - 1; i++) {
        for (int j = i + 1; j < sender.size(); j++) {
            if (sender[i].connectCount < sender[j].connectCount) std::swap(sender[i], sender[j]);
            else if (sender[i].connectCount == sender[j].connectCount && strcmp(sender[i].ID , sender[j].ID) > 0) std::swap(sender[i], sender[j]);
        }
    } 
}

//public:
void AdjacencyList::clear() {
    adjList.clear();
    sender.clear();
    listSize = 0;
    edgeSum = 0;
}

void AdjacencyList::CreatList(std::vector<Edge> &Info) {
    for (int i = 0 ; i < Info.size() ; i++) { 
        Edge cur = Info[i];
        AdjacencyNode temp;
        strcpy(temp.ID, cur.getID);
        temp.weight = cur.weight;
        if (adjList.find(cur.putID) == adjList.end()) { //getId不存在sender
            PutID toAdd;
            strcpy(toAdd.ID, cur.putID);
            sender.push_back(toAdd);
        }
        adjList[cur.putID].push_back(temp); //如果找不到這個發訊者:建新的 找到:塞進去

        if (adjList.find(cur.getID) == adjList.end()) { //getId不存在sender
            adjList[cur.getID];
            PutID toAdd2;
            strcpy(toAdd2.ID, cur.getID);
            sender.push_back(toAdd2);
        }
        edgeSum++;
    }
    listSize = adjList.size();

    std::cout << "\n<<< There are " << listSize << " IDs in total. >>>" << std::endl << std::endl;
    std::cout << "<<< There are " << edgeSum << " nodes in total. >>>" << std::endl;
}

void AdjacencyList::WriteFileAdj(std::string num) {
    std::string outputName = "pairs" + num + ".adj";
    std::ofstream fout(outputName);
    fout << "<<< There are " << listSize << " IDs in total. >>>" << std::endl;

    SortPutID();
    for (int i = 0 ; i < sender.size() ; i++) {
        fout << "[" << std::setw(3) << i + 1 << "] " << sender[i].ID << ": " << std::endl;

        std::vector<AdjacencyNode> temp = adjList[sender[i].ID];
        SortAdjNode(temp);
        int count = 1;
        fout << "\t";

        for (int j = 0; j < temp.size(); j++) {
            fout << "(" << std::setw(2) << count << ") " << temp[j].ID << "," << std::setw(7) << temp[j].weight;
            if (count % 12 == 0) fout << std::endl;
            count++;

            if (j != temp.size() - 1) {
                fout << "\t";
            }
        }
        fout << std::endl;
    }
    fout << "<<< There are " << edgeSum << " nodes in total. >>>" << std::endl;
    fout.close();
}

void AdjacencyList::Connection(PutID todo) {
    todo.connectList.clear();
    todo.connectCount = 0;

    std::vector<std::string> queue; 
    std::vector<bool> visited(adjList.size(), false);

    int index = visitedIndex[todo.ID];
    visited[index] = true; 
    queue.push_back(todo.ID);
  
    while (!queue.empty()) {
        std::string cur = queue.front();// FIFO
        queue.erase(queue.begin());
        std::vector<AdjacencyNode> adjNodes = adjList[cur];//cur直接連接到的東西
        for (int i = 0; i < adjNodes.size(); i++) {
            if (!visited[ visitedIndex[adjNodes[i].ID] ]) {
                queue.push_back(adjNodes[i].ID);
                visited[visitedIndex[adjNodes[i].ID]] = true;
                ConnectNode temp;
                strcpy(temp.ID, adjNodes[i].ID);
                todo.connectList.push_back(temp);
                todo.connectCount++;
            }
        }
    }
}

void AdjacencyList::WriteFileCnt(std::string num) {
    std::string outputName = "pairs" + num + ".cnt";
    std::ofstream fout(outputName);
    fout << "<<< There are " << listSize << " IDs in total. >>>" << std::endl;
    for (int i = 0 ; i < sender.size() ; i++) {
        fout << "[" << std::setw(3) << i + 1 << "] " << sender[i].ID << "(" <<  sender[i].connectCount <<"): " << std::endl;
        for (int j = 0; j < sender[i].connectList.size(); j++) {
            fout << "(" << std::setw(2) << j + 1 << ") " << sender[i].connectList[j].ID;
            if ((j + 1) % 12 == 0 && j != 0) fout << std::endl;
        }
        fout << std::endl;
    }
    fout.close();
}


