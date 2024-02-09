using namespace std;

template <typename T>
class Node
{
public:
    int data;
    Node <T>* prev;
    Node <T>* next;

    Node()
    {
        data = -1;
        prev = nullptr;
        next = nullptr;
    }

    ~Node()
    {
    }
};

template <typename t>
class DoublyLinkList
{
public:
    Node <t>* head;
    Node <t>* tail;

    DoublyLinkList()
    {

        head = nullptr;
        tail = nullptr;
    }

    void insert(int data)
    {

        Node <t>* newNode = new Node<t>;
        newNode->data = data;

        if (head == nullptr && tail == nullptr)
        {
            head = newNode;
            tail = newNode;
        }

        else
        {

            Node <t>* traverse = head;

            while (traverse->next != nullptr)
            {
                traverse = traverse->next;
            }

            traverse->next = newNode;
            newNode->prev = traverse;
            tail = newNode;
        }
    }

    void insertAtindex(int index, int data)
    {
        int count = 0;
        Node <t>* traverse = head;

        Node <t>* newNode = new Node<t>;
        newNode->data = data;

        if (head == nullptr)
        {
            return;
        }

        if (index == 0)
        {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
            return;
        }

        while (traverse != nullptr)
        {

            if (index - 1 == count)
            {
                break;
            }
            traverse = traverse->next;
            count++;
        }

        newNode->next = traverse->next;
        newNode->next->prev = newNode;
        traverse->next = newNode;
        newNode->prev = traverse;
    }

    void remove(int data)
    {
        Node <t>* traverse = head;

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

        Node <t>* temp = traverse->next;
        traverse->next = traverse->next->next;
        traverse->next->next->prev = traverse;
        delete temp;
        temp = nullptr;
    }

    void removeAtIndex(int index)
    {
        int count = 0;
        Node <t>* traverse = head;

        if (head == nullptr)
        {
            return;
        }

        if (index == 0)
        {
            head = head->next;
            return;
        }

        while (traverse != nullptr)
        {

            if (index - 1 == count)
            {
                break;
            }
            traverse = traverse->next;
            count++;
        }

        Node <t>* temp = traverse->next;
        traverse->next = traverse->next->next;
        traverse->next->prev = traverse;
        delete temp;
        temp = nullptr;
    }
    void displayTail()
    {

        Node <t>* traverse = tail;

        while (traverse != nullptr)
        {
            cout << traverse->data << " <- ";
            traverse = traverse->prev;
        }
    }

    void displayHead()
    {

        Node <t>* traverse = head;

        while (traverse != nullptr)
        {
            cout << traverse->data << " -> ";
            traverse = traverse->next;
        }
    }

    void display()
    {

        Node <t>* traverse = head;

        while (traverse != nullptr)
        {
            cout << traverse->data;
            if (traverse->next != nullptr)
            {
                cout << " <-> ";
            }
            traverse = traverse->next;
        }
    }
    ~DoublyLinkList()
    {
    }
};