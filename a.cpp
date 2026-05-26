//11327104林采寧 11327111林方晴
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <algorithm>
#include <chrono> // 計時

struct Edge {
    char putID[12];
    char getID[12];
    float weight;
};

struct AdjacencyNode {
    char ID[12];
    float weight = 0;
};

struct PutID {
    char ID[12];
    int connectCount = 0;
    std::set<std::string> connectList;
    
    int infCount = 0;
    std::set<std::string> infList;
};

class AdjacencyList {
  private:
    std::unordered_map<std::string, std::vector<AdjacencyNode>> adjList;//任務一 
    std::vector<PutID> sender;
    std::vector<PutID> infsender;
    int listSize = 0;
    int edgeSum = 0;

    std::unordered_map<std::string, std::unordered_set<std::string>> memo;

    void SortPutID();
    void SortAdjNode(std::vector<AdjacencyNode> &temp);
    void Sort();

    void Connection(PutID &todo);
    
    void DFS(std::string cur, float threshold, std::unordered_set<std::string> &visited);
    void SortByInfluence(); // 任務三專用排序
    void WriteFileInf(std::string num);

  public:
    ~AdjacencyList() {
        clear();
    }
    void clear();
    int getListSize() {return listSize;}
    int getInfSize() {return infsender.size();}
    void CreatList(std::vector<Edge> &Info);
    void WriteFileAdj(std::string num);
    void WriteFileCnt(std::string num);
    void ConnectForEach();
    
    void ComputeInfluence(float threshold, std::string num);//任務三
    void ComputeInfluence(float threshold); //任務四
    void TopKInfluence(int k);//任務四
};

/********************************************/
void PrintMenu();
std::string RemoveSpace(std::string target);
std::string RemoveDotTab(std::string target);
bool IsInt(std::string num);
bool IsDouble(std::string coefficient);
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
                adjacencyList.ConnectForEach();
                adjacencyList.WriteFileCnt(num);
            }
        } else if (verb == "3") {
            if (adjacencyList.getListSize() == 0) {
                std::cout << "### There is no graph and choose 1 first. ###\n";
            } else {
                std::string temp;
                float threshold = 0;
                std::cout << "Input a real number in [0.66,1.0]:";
                while (std::cin >> temp) {
                    if (IsDouble(temp)) {
                        threshold = std::stof(temp);
                        if (threshold < 0.66 || threshold > 1.0) {
                            std::cout << "### It is NOT in [0.66,1.0] ###\n";
                        } else break;
                    }
                    std::cout << "Input a real number in [0.66,1.0]:";
                }
                adjacencyList.ComputeInfluence(threshold, num);
            }
        } else if (verb == "4") {
            if (adjacencyList.getListSize() == 0) {
                std::cout << "### There is no graph and choose 1 first. ###\n";
            } else {
                auto start = std::chrono::high_resolution_clock::now();
                adjacencyList.ComputeInfluence(0.66, num);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                std::cout << "[Elapsed time] " << duration.count() << " ms" << std::endl;
    
                int k;
                std::cout << "Input an integer to show top-K in [1," << adjacencyList.getInfSize() << "]: ";
                std::string temp;
                while (std::cin >> temp) {
                    if (IsInt(temp)) {
                        k = std::stoi(temp);
                        if (k > adjacencyList.getInfSize() || k < 1) {
                            std::cout << "### " << k << " is NOT in [1," << adjacencyList.getInfSize() << "] ###\n\n";
                        } else break;
                    }
                    std::cout << "Input an integer to show top-K in [1," << adjacencyList.getInfSize() << "]: ";
                }

                adjacencyList.TopKInfluence(k);

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
    std::cout << "* 3. Estimate influence values   *" << std::endl;
    std::cout << "* 4. Find top-k influence values *" << std::endl;
    std::cout << "**********************************" << std::endl;
    std::cout << "Input a choice(0, 1, 2, 3, 4): ";
    return;
}

bool IsInt(std::string num) {
    if(num.size() == 0) {
        return false;
    }
    for (int i = 0; i < num.size(); i++) {
        if (i == 0 && (num[i] == '+')) {
            continue;
        }
        if(num[i] > '9' || num[i] < '0') {
            return false;
        }
    }
    return true;
}

bool IsDouble(std::string coefficient) {
  if (coefficient == "." || coefficient == "+" || coefficient == "-") {
    return false;
  }

  bool has_a_dot = false;
  for (int i = 0; i < coefficient.size(); i++) {
    if ((coefficient[i] > '9' || coefficient[i] < '0') && coefficient[i] != '.' && coefficient[i] != '+') {
      return false; // 非數字且非小數點(但是是小數點也不一定對)
    }

    else if (coefficient[i] == '.' && has_a_dot) {
      return false; // 已經有一個小數點
    }

    else if (coefficient[i] == '+' && i != 0) {
      return false;
    }

    else if (coefficient[i] == '.') {
      has_a_dot = true;
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
    }
}

void AdjacencyList::SortAdjNode(std::vector<AdjacencyNode> &temp) {
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

void AdjacencyList::SortByInfluence() {
    for (int i = 0; i < infsender.size() - 1; i++) {
        for (int j = i + 1; j < infsender.size(); j++) {
            if (infsender[i].infCount < infsender[j].infCount) {
                std::swap(infsender[i], infsender[j]);
            } else if (infsender[i].infCount == infsender[j].infCount && strcmp(infsender[i].ID, infsender[j].ID) > 0) {
                std::swap(infsender[i], infsender[j]);
            }
        }
    }
}

void AdjacencyList::Connection(PutID &todo) {
    todo.connectList.clear();
    todo.connectCount = 0;

    std::string startID = todo.ID;

    if (memo.find(startID) != memo.end()) {
        for (std::string reachableID : memo[startID]) {
            if (reachableID != startID) {
                todo.connectList.insert(reachableID); //直接 insert，set 會自動排序
            }
        }
        todo.connectCount = todo.connectList.size();
        return; 
    }

    //BFS
    std::queue<std::string> queue; 
    std::unordered_set<std::string> visited; 

    queue.push(startID);
    visited.insert(startID);
  
    while (!queue.empty()) {
        std::string cur = queue.front();
        queue.pop();

        if (memo.find(cur) != memo.end()) { //目前要做的節點做過
            for (std::string reachableID : memo[cur]) {
                if (reachableID != startID) {
                    visited.insert(reachableID);//connectList本身是set所以重複地會被忽略
                }
            }
            continue;
        }

        std::vector<AdjacencyNode> adjNodes = adjList[cur];
        for (int i = 0; i < adjNodes.size(); i++) {
            std::string nextID = adjNodes[i].ID;
            if (visited.find(nextID) == visited.end()) {//相鄰點沒有走過
                visited.insert(nextID);
                queue.push(nextID);
            }
        }
    }

    memo[startID] = visited;

    //寫入todo.connectList
    for (std::string reachableID : memo[startID]) {
        if (reachableID != startID) {
            todo.connectList.insert(reachableID); // 修改：直接 insert
        }
    }
    todo.connectCount = todo.connectList.size();
}

void AdjacencyList::DFS(std::string cur, float threshold, std::unordered_set<std::string> &visited) {
    std::vector<AdjacencyNode> adjNodes = adjList[cur];
    for (int i = 0; i < adjNodes.size(); i++) {
        if (adjNodes[i].weight >= threshold) {//有效邊
            std::string nextID = adjNodes[i].ID;
            if (visited.find(nextID) == visited.end()) {
                visited.insert(nextID);
                DFS(nextID, threshold, visited); //遞迴深入走訪
            }
        }
    }
}

void AdjacencyList::WriteFileInf(std::string num) {
    std::string outputName = "pairs" + num + ".inf";
    std::ofstream fout(outputName);
    fout << "<<< There are " << infsender.size() << " IDs in total. >>>" << std::endl;
    
    int rankCount = 1;
    for (int i = 0; i < infsender.size(); i++) {
        fout << "[" << std::setw(3) << rankCount << "] " << infsender[i].ID 
                << " has influence " << infsender[i].infCount << ":" << std::endl;
        
        int j = 0;
        for (std::string id : infsender[i].infList) {
            fout << "\t(" << std::setw(2) << j + 1 << ") " << id;
            if ((j + 1) % 12 == 0 && j != 0) fout << std::endl;
            j++;
        }
        fout << std::endl;
        rankCount++;
    }
    fout.close();
    std::cout << "<<< There are "<< infsender.size() <<" IDs in total. >>>" << std::endl;
}

//public:
void AdjacencyList::clear() {
    adjList.clear();
    sender.clear();
    memo.clear();
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

void AdjacencyList::WriteFileCnt(std::string num) {
    std::string outputName = "pairs" + num + ".cnt";
    std::ofstream fout(outputName);
    fout << "<<< There are " << listSize << " IDs in total. >>>" << std::endl;
    for (int i = 0 ; i < sender.size() ; i++) {
        fout << "[" << std::setw(3) << i + 1 << "] " << sender[i].ID << "(" <<  sender[i].connectCount <<"): " << std::endl;
        int j = 0;
        for (std::string id : sender[i].connectList) {
            fout << "\t(" << std::setw(2) << j + 1 << ") " << id;
            if ((j + 1) % 12 == 0 && j != 0) fout << std::endl;
            j++;
        }
        fout << std::endl;
    }
    fout.close();
}

void AdjacencyList::ConnectForEach() {
    for (int i = 0; i < sender.size(); i++) {
        Connection(sender[i]);
    }
    Sort();
}

void AdjacencyList::ComputeInfluence(float threshold, std::string num) {
    for (int i = 0; i < sender.size(); i++) {
        sender[i].infList.clear();
        std::unordered_set<std::string> visited;
        std::string startID = sender[i].ID;
        
        visited.insert(startID);
        DFS(startID, threshold, visited);
        
        for (std::string reachableID : visited) {
            if (reachableID != startID) {
                sender[i].infList.insert(reachableID); // set會自動依學號從小到大排序
            }
        }
        sender[i].infCount = sender[i].infList.size();
        if (sender[i].infCount > 0) infsender.push_back(sender[i]);
    }
    
    SortByInfluence();
    WriteFileInf(num);
}

void AdjacencyList::ComputeInfluence(float threshold) {
    for (int i = 0; i < sender.size(); i++) {
        sender[i].infList.clear();
        std::unordered_set<std::string> visited;
        std::string startID = sender[i].ID;
        
        visited.insert(startID);
        DFS(startID, threshold, visited);
        
        for (std::string reachableID : visited) {
            if (reachableID != startID) {
                sender[i].infList.insert(reachableID); // set會自動依學號從小到大排序
            }
        }
        sender[i].infCount = sender[i].infList.size();
        if (sender[i].infCount > 0) infsender.push_back(sender[i]);
    }
    SortByInfluence();
}

void AdjacencyList::TopKInfluence(int k) {
    int printed = 0;
    int lastInfCount = -1;
    
    for (int i = 0; i < infsender.size(); i++) {
        if (infsender[i].infCount <= 0) continue;//確保正輸出
        
        if (printed >= k && infsender[i].infCount != lastInfCount) {
            break;
        }
        
        std::cout << "<" << printed + 1 << "> " << infsender[i].ID << ": " << infsender[i].infCount << std::endl;
                  
        lastInfCount = infsender[i].infCount;
        printed++;
    }
}
