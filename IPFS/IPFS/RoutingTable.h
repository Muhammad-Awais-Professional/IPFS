////#pragma once
//#ifndef RoutingTable_H
//#define RoutingTable_H
//#include <iostream>
//
//using namespace std;
//
////template <typename t>
//struct Pair {
//
//    BTree<string>* Files;
//    BigInt Key;
//    string Name;
//    int id_space;
//
//    Pair(int o = 3) {
//        Files = new BTree<string>(o);
//        Key = "";
//        Name = "";
//        id_space = 0;
//    }
//
//    bool operator==(Pair obj) {
//        return Key == obj.Key;
//    }
//
//    bool operator<= (Pair obj) {
//        return Key <= obj.Key;
//    }
//
//    bool operator>= (Pair obj) {
//        return Key >= obj.Key;
//    }
//
//    bool operator > (Pair obj) {
//        return Key > obj.Key;
//    }
//
//    bool operator < (Pair obj) {
//        return Key < obj.Key;
//    }
//
//    friend std::ostream& operator<<(std::ostream& os, const Pair& p) {
//        os << "Key: " << p.Key << ", Name: " << p.Name << " , " << p.id_space << " ";
//
//        return os;
//    }
//
//    friend std::istream& operator>>(std::istream& is, Pair& p) {
//        std::cout << "Enter Key: ";
//        is >> p.Key;
//        std::cout << "Enter Name: ";
//        is >> p.Name;
//        return is;
//    }
//};
//
//class Machine
//{
//public:
//    Pair data;
//    //Machine <T>* prev;
//    Machine* next;
//    RoutingTable* RT;
//
//    // Default constructor
//    Machine()
//    {
//        //prev = nullptr;
//        next = nullptr;
//        data = -1;
//        RT = nullptr;
//    }
//
//    Machine(const Pair& value) : data(value), next(nullptr), RT(nullptr) {}
//
//    ~Machine() {}
//};
//
//class RoutingNode
//{
//public:
//    Machine* data;
//    RoutingNode* prev;
//    RoutingNode* next;
//
//    RoutingNode()
//    {
//        data = nullptr;
//        prev = nullptr;
//        next = nullptr;
//    }
//
//    ~RoutingNode()
//    {
//
//    }
//};
//
////template <typename t>
//class RoutingTable
//{
//public:
//    RoutingNode* head;
//
//    RoutingTable()
//    {
//        head = nullptr;
//    }
//    RoutingTable(int id_space)
//    {
//        for (int i = 0; i < id_space; ++i)
//        {
//            Machine* temp = new Machine;
//            insert(temp);
//        }
//    }
//    void insert(Machine* data)
//    {
//
//        RoutingNode* newRoutingNode = new RoutingNode;
//        newRoutingNode->data = data;
//
//        if (head == nullptr)
//        {
//            head = newRoutingNode;
//        }
//
//        else
//        {
//
//            RoutingNode* traverse = head;
//
//            while (traverse->next != nullptr)
//            {
//                traverse = traverse->next;
//            }
//
//            traverse->next = newRoutingNode;
//            newRoutingNode->prev = traverse;
//            //tail = newRoutingNode;
//        }
//    }
//
//    /*void insertAtindex(int index, int data)
//    {
//        int count = 0;
//        RoutingNode <t>* traverse = head;
//
//        RoutingNode <t>* newRoutingNode = new RoutingNode<t>;
//        newRoutingNode->data = data;
//
//        if (head == nullptr)
//        {
//            return;
//        }
//
//        if (index == 0)
//        {
//            newRoutingNode->next = head;
//            head->prev = newRoutingNode;
//            head = newRoutingNode;
//            return;
//        }
//
//        while (traverse != nullptr)
//        {
//
//            if (index - 1 == count)
//            {
//                break;
//            }
//            traverse = traverse->next;
//            count++;
//        }
//
//        newRoutingNode->next = traverse->next;
//        newRoutingNode->next->prev = newRoutingNode;
//        traverse->next = newRoutingNode;
//        newRoutingNode->prev = traverse;
//    }*/
//
//    void remove(Machine* data)
//    {
//        RoutingNode* traverse = head;
//
//        if (head == nullptr)
//        {
//            return;
//        }
//
//        if (head->data == data)
//        {
//            head = head->next;
//            return;
//        }
//        while (traverse != nullptr)
//        {
//
//            if (traverse->next->data == data)
//            {
//                break;
//            }
//            traverse = traverse->next;
//        }
//
//        RoutingNode* temp = traverse->next;
//        traverse->next = traverse->next->next;
//        traverse->next->next->prev = traverse;
//        delete temp;
//        temp = nullptr;
//    }
//
//    /*void removeAtIndex(int index)
//    {
//        int count = 0;
//        RoutingNode * traverse = head;
//
//        if (head == nullptr)
//        {
//            return;
//        }
//
//        if (index == 0)
//        {
//            head = head->next;
//            return;
//        }
//
//        while (traverse != nullptr)
//        {
//
//            if (index - 1 == count)
//            {
//                break;
//            }
//            traverse = traverse->next;
//            count++;
//        }
//
//        RoutingNode <t>* temp = traverse->next;
//        traverse->next = traverse->next->next;
//        traverse->next->prev = traverse;
//        delete temp;
//        temp = nullptr;
//    }*/
//    /*void displayTail()
//    {
//
//        RoutingNode <t>* traverse = tail;
//
//        while (traverse != nullptr)
//        {
//            cout << traverse->data << " <- ";
//            traverse = traverse->prev;
//        }
//    }*/
//
//    void displayHead()
//    {
//
//        RoutingNode* traverse = head;
//
//        while (traverse != nullptr)
//        {
//            cout << traverse->data << " -> ";
//            traverse = traverse->next;
//        }
//    }
//
//    void display()
//    {
//
//        RoutingNode* traverse = head;
//
//        while (traverse != nullptr)
//        {
//            cout << traverse->data;
//            if (traverse->next != nullptr)
//            {
//                cout << " <-> ";
//            }
//            traverse = traverse->next;
//        }
//    }
//    ~RoutingTable()
//    {
//    }
//};
//
//#endif