//Sush Mullur
//Homework 4

#ifndef SKIPLIST_H
#define SKIPLIST_H
#include <iostream>
using namespace std;
class SkipList {

public:
    //Overloaded ostream operator.
    friend ostream& operator<<(ostream &os, const SkipList &list);
    //Default constructor that creates a 1 deep SkipList with just a doubly-linked list.
    explicit SkipList(int depth = 1);
    //Destructor.
    virtual ~SkipList();
    //Return true if successfully added. Duplicates are not allowed.
    bool Add(int data);
    //Return true if removal was successful.
    bool Remove(int data);
    //Return true if found in SkipList.
    bool Contains(int value);

private:
    class SNode {
    public:
        //Constructor stores int as data.
        explicit SNode(int data);
        //Data for SNode.
        int data;
        //Link to next node.
        SNode *next{};
        //Link to previous node.
        SNode *prev{};
        //Link to up level.
        SNode *upLevel{};
        //Link to down level.
        SNode *downLevel{};
    };

    //Depth of SkipList, levels are 0 to depth-1.
    int depth;
    //Array of depth SNode* objects as frontGuards linking levels.
    //If depth = 2, we'd have frontGuards[0] and frontGuards[1].
    SNode **frontGuards;
    //Array of depth SNode* objects as rearGuards linking levels.
    SNode **rearGuards;
    //Helper method that takes the level of the skiplist and returns it as a string.
    string getLevel(int level) const;
    //Return true 50% of the time.
    //Each node has a 50% chance of being at a higher level.
    bool alsoHigher() const;
    //Helper method that creates the front and rear guards.
    void createGuards();
    //Helper method that deletes all the nodes in a given level.
    //Takes the head of the given level as a parameter.
    void deleteNodes(SNode *head);
};


#endif
