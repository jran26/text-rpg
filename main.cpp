#include <iostream>
#include <string>
#include "Room.hpp"
#include "EmptyRoom.cpp"
#include "Item.hpp"

using namespace std;

int main(){

    Room tree("tree", "This room has a large tree in it. Two openings lie in front of you. One leads to a room with a large pool of water in it. The other leads deeper into the cave.");
    tree.interact();
    Room* currentRoom = &tree;

    Room pool("pool", "this room has a big pool");
    tree.addExit(pool);
    Room cave("cave", "this is a dark cave");
    tree.addExit(cave);
    currentRoom->printExits();

    Item stick("large stick", "it's a stick");
    stick.inspect();

    Entity joe("joe", "average joe");
    joe.inspect();
    return 0;
}
