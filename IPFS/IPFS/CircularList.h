#include<cstring>
#include<cmath>
#include "B_TREE.h"
#include "BigBin.h"
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;
using namespace std;

///////////////////--Including Classes--//////////////////

struct Pair;
class RoutingTable;
class Machine;
class RoutingNode;
class DHT;

/////////////////////////////////////////////////////

struct File {
    BigBin key;
    std::filesystem::path filePath;

    // Comparison operators
    bool operator<(const File& obj) const {
        return key < obj.key;
    }

    bool operator==(const File& obj) const {
        return key == obj.key && filePath == obj.filePath;
    }

    bool operator<=(const File& obj) const {
        return !(obj < *this);
    }

    bool operator>(const File& obj) const {
        return obj < *this;
    }

    bool operator>=(const File& obj) const {
        return !(*this < obj);
    }

    friend std::ostream& operator<<(std::ostream& os, const File& file) {
        os << "Key: " << file.key << ", FilePath: " << file.filePath << endl;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, File& file) {
        is >> file.key >> file.filePath;
        return is;
    }
};

struct Pair {
    BTree<File>* Files;
    BigBin Key;
    string Name;
    int id_space;
    fs::path machinePath;

    Pair(int o = 3) {
        Files = nullptr;
        Key = "";
        Name = "";
        id_space = 0;
    }

    bool operator==(Pair obj) {
        return Key == obj.Key;
    }

    bool operator<= (Pair obj) {
        return Key <= obj.Key;
    }

    bool operator>= (Pair obj) {
        return Key >= obj.Key;
    }

    bool operator > (Pair obj) {
        return Key > obj.Key;
    }

    bool operator < (Pair obj) {
        return Key < obj.Key;
    }

    friend std::ostream& operator<<(std::ostream& os, const Pair& p) {
        os << "Key: " << p.Key << ", Name: " << p.Name << " , " << p.id_space << " ";

        return os;
    }

    friend std::istream& operator>>(std::istream& is, Pair& p) {
        std::cout << "Enter Key: ";
        is >> p.Key;
        std::cout << "Enter Name: ";
        is >> p.Name;
        return is;
    }
};


/////////////////////////////////////////////////////


class Machine
{
public:
    Pair data;
    Machine* next;
    RoutingTable* RT;

    Machine()
    {
        next = nullptr;
        data.Files = nullptr;
        data.id_space = 0;
        data.Key = "";
        data.Name = "";
        RT = nullptr;
    }

    Machine(const Pair& value) : data(value), next(nullptr), RT(nullptr) {}

    ~Machine() {}
};


class RoutingNode
{
public:
    Machine* data;
    RoutingNode* prev;
    RoutingNode* next;

    RoutingNode()
    {
        data = nullptr;
        prev = nullptr;
        next = nullptr;
    }

    ~RoutingNode()
    {

    }
};

/////////////////////////////////////////////////////


class RoutingTable
{
public:
    RoutingNode* head;

    RoutingTable()
    {
        head = nullptr;
    }
    RoutingTable(int id_space)
    {
        //cout << id_space;
        for (int i = 0; i < id_space; ++i)
        {
            Machine* temp = new Machine;
            this->insert(temp);
        }
    }
    void insert(Machine* data)
    {

        RoutingNode * newNode = new RoutingNode;
        newNode->data = data;

        if (head == nullptr )
        {
            head = newNode;
        }

        else
        {

            RoutingNode * traverse = head;

            while (traverse->next != nullptr)
            {
                traverse = traverse->next;
            }

            traverse->next = newNode;
            newNode->prev = traverse;
            
        }
        
    }


    void remove(Machine* data)
    {
        RoutingNode* traverse = head;

        if (head == nullptr)
        {
            return;
        }

        if (head->data == data)
        {
            head = head->next;
            return;
        }
        while (traverse != nullptr)
        {

            if (traverse->next->data == data)
            {
                break;
            }
            traverse = traverse->next;
        }

        RoutingNode* temp = traverse->next;
        traverse->next = traverse->next->next;
        traverse->next->next->prev = traverse;
        delete temp;
        temp = nullptr;
    }

    void displayHead()
    {

        RoutingNode* traverse = head;

        while (traverse != nullptr)
        {
            cout << traverse->data << " -> ";
            traverse = traverse->next;
        }
    }

    void display()
    {
        cout << "RT DIsplay\n";
        RoutingNode* traverse = head;

        int i = 1;
        while (traverse != nullptr)
        {
            cout << i << "-> ";
            ++i;
            cout << traverse->data->data.Key;
            if (traverse->next != nullptr)
            {
                cout << " <-> ";
            }
            traverse = traverse->next;
        }
    }
    ~RoutingTable()
    {
    }
};


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////


class DHT
{
public:
    Machine* head;
    DHT()
    {
        head = nullptr;
    }
    void CreateRoutingTable() {
        if (head == nullptr) {
            cout << "DHT is empty." << endl;
            return;
        }

        Machine* ptr = head;
        do {
            ptr->RT = new RoutingTable(ptr->data.id_space);

            RoutingNode* RT_NODE = ptr->RT->head;
            for (int i = 1; i <= ptr->data.id_space; ++i) {
                BigBin temp = ptr->data.Key;
                BigBin temp1 = BigBin().powerOfTwo(i - 1);
                temp = temp + temp1; 

                BigBin max_Limit = BigBin().powerOfTwo(ptr->data.id_space);

                //cout << "max " << max_Limit << endl;


                if (temp > max_Limit) {
                    temp = temp - max_Limit;
                    //cout <<"Machine name " << ptr->data.Name << endl;
                    //cout << "That hash after sub " << temp.binary << endl;
                }
                //cout << ptr->data.Name << endl;
                //cout << temp.binary << endl;

                Machine* RightMachine = head;
                do {
                    cout << " machine " << RightMachine->data.Key.binary << endl;
                    cout << " temp " << temp.binary << endl;
                    if (RightMachine->data.Key.binary == temp.binary) {
                        RT_NODE->data = RightMachine;
                        break;
                    }
                    else if (RightMachine->data.Key.binary < temp.binary && RightMachine->next->data.Key.binary == head->data.Key.binary) {
                        RT_NODE->data = head;
                        break;
                    }
                    else {
                        if (temp.binary < RightMachine->data.Key.binary)
                        {
                            RT_NODE->data = RightMachine;
                            break;
                        }

                    }

                    RightMachine = RightMachine->next;
                    //if (RightMachine == head) {
                    //    // Reached the end of the circular list, loop back to the beginning
                    //    RightMachine = head->next;
                    //}
                } while (RightMachine != head);

                RT_NODE = RT_NODE->next;
            }
            ptr = ptr->next;
        } while (ptr != head);
    }

    Machine* search(const BigBin& fileHash, Machine* startMachine) {
        RoutingNode* startNode = startMachine->RT->head;
        RoutingNode* currentNode = startNode;

        do {
            if (currentNode->data->data.Key == fileHash) {
                return currentNode->data;
            }
            else if (currentNode->next == startNode) {
                break; 
            }
            else if (fileHash < currentNode->data->data.Key) {
                if (currentNode->prev != nullptr) {
                    currentNode = currentNode->prev; 
                }
                else {
                    break;
                }
            }
            else {
                currentNode = currentNode->next;
            }
        } while (currentNode != startNode);

        return currentNode->data;
    }

    void insert(Pair& value)
    {
        Machine* newMachine = new Machine(value);
        if (head == nullptr)
        {
            head = newMachine;
            head->next = head;
        }
        else if (value <= head->data)
        {
            newMachine->next = head;
            Machine* temp = head;
            while (temp->next != head)
            {
                temp = temp->next;
            }
            temp->next = newMachine;
            head = newMachine;
        }
        else
        {
            Machine* temp = head;
            while (temp->next != head && temp->next->data < value)
            {
                temp = temp->next;
            }
            newMachine->next = temp->next;
            temp->next = newMachine;

            //if (newMachine->next == head || newMachine->data < head->data) {
            //    head = newMachine;
            //}
        }
    }
    bool search(const Pair& value, Pair& returnval) {
        if (head == nullptr) {
            return false; 
        }

        Machine* temp = head;
        do {
            if (temp->data == value) {
                returnval = temp->data;
                return true;
            }
            temp = temp->next;
        } while (temp != head);

        return false; 
    }

    void display() {
        if (head == nullptr) {
            cout << "List is empty." << std::endl;
            return;
        }
        Machine* temp = head;
        do {
            cout << temp->data.Key<<" , ( "<<temp->data.Name << " ) -> ";
            temp = temp->next;
        } while (temp != head);
        cout << std::endl;
    }

    void remove(const Pair& value) {
        if (head == nullptr) {
            std::cout << "List is empty. Cannot delete from an empty list." << std::endl;
            return;
        }

        Machine* current = head;
        Machine* previous = nullptr;

        do {
            if (current->data == value) {
                break;
            }
            previous = current;
            current = current->next;
        } while (current != head);

        if (current == head) {
            Machine* temp = head;
            while (temp->next != head) {
                temp = temp->next;
            }
            if (head->next == head) {
                delete head;
                head = nullptr;
            }
            else {
                temp->next = head->next;
                delete head;
                head = temp->next;
            }
        }
        else if (current != nullptr) {
            previous->next = current->next;
            delete current;
        }
        else {
            cout << "Value not found in the list." << std::endl;
        }
    }

    ~DHT()
    {
    }
};