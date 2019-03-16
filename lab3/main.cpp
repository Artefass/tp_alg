#include <cstdlib>
#include <iostream>

#define BASE_SIZE 16

class Stack
{
    int *stack = nullptr;
    int size = 0;
    int capacity = 0;

    void resize() {
        int *tmp = (int*)realloc(stack, sizeof(int) * capacity * 2);
        if ( tmp == nullptr ) {
            throw "Stack error: could not reallocate memmory";
        }
        capacity *= 2;
        stack = tmp;
    }

public:
    Stack() {
        capacity = BASE_SIZE;
        stack = new int[BASE_SIZE];
    }

    ~Stack() { delete [] stack; }

    void push(int element) {
        if ( size == capacity ) {
            resize();
        }

        stack[size] = element;
        size++;
    }

    int pop() {
        if ( isEmpty() ) {
            return -1;
        } else {
            size--;
            return stack[size];
        }
    }

    bool isEmpty() {
        return size == 0;
    }
};

class Queue
{
    Stack stackIn;
    Stack stackOut;

public:
    void pushBack(int element) {
        stackIn.push(element);
    }

    void pushFront(int element) {
        stackOut.push(element);
    }

    int popFront() {
        if ( stackOut.isEmpty() && stackIn.isEmpty() ) {
            return -1;
        } else if ( stackOut.isEmpty() && !stackIn.isEmpty() ) {
            while ( !stackIn.isEmpty() ) {
                stackOut.push(stackIn.pop());
            }
        }

        return stackOut.pop();
    }

    int popBack() {
        if ( stackIn.isEmpty() && stackIn.isEmpty() ) {
            return -1;
        } else if ( stackIn.isEmpty() && !stackOut.isEmpty() ) {
            while ( !stackOut.isEmpty() ) {
                stackIn.push(stackOut.pop());
            }
        }

        return stackIn.pop();
    }

    bool isEmpty() { return stackIn.isEmpty() && stackOut.isEmpty(); }
};

int main() {

    Queue queue;

    int nCommands;
    std::cin >> nCommands;

    int command, element, popElement;
    bool errflag = false;

    for (int i = 0; i < nCommands; i++) {
        std::cin >> command >> element;

        switch (command) {
        case 1:
            queue.pushFront(element);
            break;
        case 2:
            popElement = queue.popFront();
            if ( popElement != element ) {
                errflag = true;
            }
            break;
        case 3:
            queue.pushBack(element);
            break;
        case 4:
            popElement = queue.popBack();
            if ( popElement != element ) {
                errflag = true;
            }
            break;
        default:
            break;
        }
    }

    if ( errflag )
        std::cout << "NO";
    else
        std::cout << "YES";

    return  0;
}