//11327104林采寧 11327111林方晴

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <cstring>

struct Edge {
    char putID[12];
    char getID[12];
    float weight;
};

struct AdjacencyNode {
    char ID[12];
    float weight = 0;
    AdjacencyNode *next = nullptr;
};

struct PutID {
    char ID[12];
    AdjacencyNode *next = nullptr;
    int connectCount = 0;
    std::vector <char[12]> connectList;
};

class AdjacencyList {
  private:
    std::vector<PutID> adjList;
    int listSize = 0;
    int edgeSum = 0;

    int GetIDIndex(char temp[12]);
    void Insert(Edge edge, int index); //cur這個邊從第index位置的ID發送
    void Insert(Edge edge); //edge.putID是全新的發送者
    void PushIDtoList(char ID[12]);

  public:
    ~AdjacencyList() {
        clear();
    }
    void clear();
    int getListSize() {return listSize;}
    void CreatList(std::vector<Edge> &Info);
    void WriteFileAdj(std::string num);
    void Connection(int index);
    void WriteFileCnt(std::string num);
};

/********************************************/
void PrintMenu();
std::string RemoveSpace(std::string target);
std::string RemoveDotTab(std::string target);
bool IsInt(std::string num);
void GetN(std::string temp , int &n);
bool SetInfo(std::vector<Edge> &Info , std::string &num);
bool ChangeFile(std::string filename , std::string outputName);
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
            std::string num;
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
                    adjacencyList.Connection(i);
                }
                adjacencyList.WriteFileCnt(num);
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
//private:
int AdjacencyList::GetIDIndex(char temp[12]) {
    for (int i = 0 ; i < adjList.size() ; i++) {
        if (strcmp(temp, adjList[i].ID) == 0) return i;
    }
    return -1;
}

void AdjacencyList::Insert(Edge edge, int index) {//cur這個邊從第index位置的ID發送
    AdjacencyNode *toInsert = new AdjacencyNode;
    strcpy(toInsert->ID , edge.getID);
    toInsert->weight = edge.weight;

    if (adjList[index].next == nullptr || strcmp(toInsert->ID, adjList[index].next->ID) < 0) {
        toInsert->next = adjList[index].next;
        adjList[index].next = toInsert;
        return;
    }
    
    // 找插入位置
    AdjacencyNode *temp = adjList[index].next;
    while (temp->next != nullptr && strcmp(temp->next->ID, toInsert->ID) < 0) {
        temp = temp->next;
    }
    toInsert->next = temp->next;
    temp->next = toInsert;
}

void AdjacencyList::Insert(Edge edge) {//edge.putID是全新的發送者
    int i = 0;
    for ( ; i < adjList.size() ; i++) {
        if (strcmp(edge.putID , adjList[i].ID) < 0) {
            break;
        }
    } // 找位置
    PutID temp;
    strcpy(temp.ID, edge.putID);

    AdjacencyNode *node = new AdjacencyNode;
    strcpy(node->ID, edge.getID);
    node->weight = edge.weight;
    temp.next = node;

    adjList.insert(adjList.begin() + i , temp);
}

void AdjacencyList::PushIDtoList(char ID[12]) {
    int i = 0;
    for ( ; i < adjList.size() ; i++) {
        if (strcmp(ID , adjList[i].ID) < 0) {
            break;
        }
    }
    PutID temp;
    strcpy(temp.ID , ID);
    adjList.insert(adjList.begin() + i , temp);
}

//public:
void AdjacencyList::clear() {
    for (int i = 0; i < adjList.size(); i++) {
        AdjacencyNode *temp = adjList[i].next;
        while (temp != nullptr) {
            AdjacencyNode *next = temp->next;
            delete temp;
            temp = next;
        }
        adjList[i].next = nullptr;
    }
    adjList.clear();
    listSize = 0;
    edgeSum = 0;
}

void AdjacencyList::CreatList(std::vector<Edge> &Info) {
    for (int i = 0 ; i < Info.size() ; i++) {
        Edge cur = Info[i];
        int index = GetIDIndex(cur.putID); //看有沒有發送過(-1是沒有)
        if (index != -1) {//發送過資料
            Insert(cur, index); //加入到某一特定串列
            int getIDindex = GetIDIndex(cur.getID); //收訊者有沒有出現
            if (getIDindex == -1) {
                PushIDtoList(cur.getID);//存在的節點但是沒發過訊號
            }
        } else {
            Insert(cur); //加入到adjList
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
    for (int i = 0 ; i < adjList.size() ; i++) {
        fout << "[" << std::setw(3) << i + 1 << "] " << adjList[i].ID << ": " << std::endl;

        AdjacencyNode *temp = adjList[i].next;
        int count = 1;
        fout << "\t";
        while (temp != nullptr) {
            fout << "(" << std::setw(2) << count << ") " << temp->ID << "," << std::setw(7) << temp->weight;
            if (count == 12) fout << std::endl;
            temp = temp->next;
            count++;

            if (temp != nullptr) {
                fout << "\t";
            }
        }
        fout << std::endl;
    }
    fout << "<<< There are " << edgeSum << " nodes in total. >>>" << std::endl;
    fout.close();
}

void AdjacencyList::Connection(int index) {
    std::vector<AdjacencyNode*> queue;
    AdjacencyNode *temp;
    strcpy(temp->ID, adjList[index].ID);
    temp->next = adjList[index].next;
    queue.push_back(temp);
    while (queue.front()) { // queue裡還有東西
        AdjacencyNode *toAdd = queue.front()->next;
        while(toAdd) { // 把目前 queue的top 的收訊者都記下來
            queue.push_back(toAdd);
            adjList[index].connectList.push_back(toAdd->ID);
            adjList[index].connectCount++;
            toAdd = toAdd->next;
        }
        queue.erase(queue.begin());
    }
}

void AdjacencyList::WriteFileCnt(std::string num) {
    std::string outputName = "pairs" + num + ".cnt";
    std::ofstream fout(outputName);
    fout << "<<< There are " << listSize << " IDs in total. >>>" << std::endl;
    for (int i = 0 ; i < adjList.size() ; i++) {
        fout << "[" << std::setw(3) << i + 1 << "] " << adjList[i].ID << "(" <<  adjList[i].connectCount <<"): " << std::endl;
        for (int j = 0; j < adjList[i].connectList.size(); j++) {
            fout << "(" << std::setw(2) << i + 1 << ") " << adjList[i].connectList[j];
            if (i + 1 == 12) fout << std::endl;
        }
        fout << std::endl;
    }
    fout.close();
}
