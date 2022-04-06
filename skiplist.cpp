//Sush Mullur
//Homework 4

#include "skiplist.h"
#include <sstream>
#include <random>
#include <climits>

//Node constructor that stores int as data.
SkipList::SNode::SNode(int data) {
    this->data = data;
    this->next = nullptr;
}

//Overloaded ostream operator.
ostream& operator<<(ostream &os, const SkipList &list) {
    //Iterates through each level and calls a helper method to iterate through the nodes within the levels.
    for(int i = list.depth-1; i >= 0; i--) {
        os << "Level: " << i << " -- ";
        os << list.getLevel(i);
        os << "\n";
    }
    return os;
}

//SkipList constructor which takes the depth of a skiplist and creates respective levels and nodes.
 SkipList::SkipList(int depth) {
    this ->depth = depth;
    //0 and other invalid depths are set to 1.
    if(depth < 1) {
        cout << "Invalid depth! Depth has been updated to 1" << endl;
        this ->depth = 1;
    }
    //Rear and front guards are set up.
    this -> rearGuards = new SNode*[depth];
    this -> frontGuards = new SNode*[depth];
    createGuards();
}

//Destructor.
SkipList::~SkipList() {
    //Iterates through each level and calls a helper method to delete nodes.
    for(int i = 0; i < depth; i++) {
        deleteNodes(rearGuards[i]);
    }
    //Guard arrays are deleted.
    delete[] this-> rearGuards;
    delete[] this-> frontGuards;
    int counter = 0;
    cout << counter;
}

//Helper method that creates guard nodes based on depth.
void SkipList::createGuards() {
    //Nodes are horizontally created and linked here.
    for(int i = 0; i < this->depth; i++) {
        rearGuards[i] = new SNode(INT_MIN);
        frontGuards[i] = new SNode(INT_MAX);
        rearGuards[i]->next = frontGuards[i];
        frontGuards[i] -> prev = rearGuards[i];
    }

    //Nodes are vertically linked in this iteration.
    for(int i = 0; i < this-> depth; i++) {
        if(i <= depth -2) {
            rearGuards[i] -> upLevel = rearGuards[i+1];
            frontGuards[i] -> upLevel = frontGuards[i+1];
        }
        if(i > 0) {
            rearGuards[i] -> downLevel = rearGuards[i-1];
            frontGuards[i] -> downLevel = frontGuards[i-1];
        }
    }
}

//Return true if successfully added. Duplicates are not allowed.
bool SkipList::Add(int data) {
    //Node is added at the bottom level.
    SNode* node = rearGuards[0];
    auto* newNode = new SNode(data);
    while(node->next->data < data) {
        node = node->next;
    }
    //Validates that duplicate values cannot be added.
    if(node -> next -> data == data) {
        cout << "Duplicates not allowed: " << data << endl;
        return false;
    }
    //Pointer magic that inserts a node.
    newNode->next = node->next;
    newNode->next->prev = newNode;
    newNode->prev = node;
    node->next = newNode;

    //Adding nodes to higher levels.
    if(depth > 1) {
        int currentLevel = 1;
        while(currentLevel < depth) {
            if(alsoHigher()) {
                SNode *innerNode = rearGuards[currentLevel];
                auto *innerNewNode = new SNode(data);
                //Since the outer loop only iterates k times, it still maintains O(N) complexity at worst.
                while (innerNode->next->data < data) {
                    innerNode = innerNode->next;
                }
                innerNewNode->next = innerNode->next;
                innerNewNode->next->prev = innerNewNode;
                innerNewNode->prev = innerNode;
                innerNode->next = innerNewNode;
                SNode* downNode = rearGuards[currentLevel-1];
                while(downNode->data != data) {
                    downNode = downNode->next;
                }
                innerNewNode->downLevel = downNode;
                downNode->upLevel = innerNewNode;
                currentLevel++;
            } else {
                //Exits loop if random returns false.
                break;
            }
        }
    }
    return true;
}

//Returns a particular level as a string, takes int level as a parameter.
string SkipList::getLevel(int level) const{
    SNode* node = rearGuards[level];
    stringstream ss;
    //Iterates through each node from the rear to front guard.
    while(node != nullptr) {
        ss << node->data << ", ";
        node = node->next;
    }
    return ss.str();

}

//Coin toss function that determines if a node should be added to the next highest level.
bool SkipList::alsoHigher() const {
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(1, 100);
    int rdm = dist(mt);
    return rdm % 2 == 0;
}

//Checks whether a value exists in the skiplist.
//Returns a boolean representing the existence of the value.
bool SkipList::Contains(int value) {
    SNode* node = rearGuards[depth-1];
    int currentLevel = depth-1;
    while(node-> data != value || currentLevel >= 0) {
        //Node has been found.
        if(node-> data == value) return true;
        if(node->next-> data > value) {
            if(currentLevel == 0) {
                return false;
            }
            //Skips through to bottom level
            node = node -> downLevel;
            currentLevel--;
        } else {
            //Moves forward within level.
            node = node -> next;
        }

    }
    //Node is not found.
    return false;
}

bool SkipList::Remove(int data) {
    if(data == INT_MAX || data == INT_MIN) return false;
    //Iterates through levels and removes all occurrences of the node.
    for(int i = 0; i < depth; i++) {
        SNode* node = rearGuards[i];
        while(node->data != data && node -> data != INT_MAX) {
            node = node->next;
        }
        if(node -> data != INT_MAX) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            delete node;
        } else if(i == 0) {
            return false;
        }
    }
    return true;
}

//Helper method that deletes a linked list given the head as a parameter.
void SkipList::deleteNodes(SkipList::SNode *head) {
    SNode* node = head;
    SNode* next;
    //Frees up the doubly linked list in each level.
    while(node != nullptr) {
        next = node->next;
        free(node);
        node = next;
    }

}






