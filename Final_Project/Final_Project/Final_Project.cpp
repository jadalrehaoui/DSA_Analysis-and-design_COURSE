// Final_Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "CSVParser.hpp"
using namespace std;

// course struct
struct Course {
    string id;
    string name;
    vector<string> prereq;
    // to string function
    string to_string() {
        string course = id + " | " + name + "\n";
        // each prerequisite on a line
        for (unsigned int i = 0; i < prereq.size(); i++) {
            int index = i + 1;
            course += "\t" + prereq[i] + "\n";
        }
        // return the string
        return course;
    }
};
// doubly linked list
class LinkedList {
    // it should have previous and next for each node
private: 
    struct Node {
        Course course;
        Node* next;
        Node* prev;
        Node() {
            next = nullptr;
            prev = nullptr;
        }
        Node(Course aCourse) {
            course = aCourse;
            next = nullptr;
            prev = nullptr;
        }
    };
    Node* head;
    Node* tail;
    int size = 0;
public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Course course);
    void Prepend(Course course);
    // Insert after will help us with sorting
    void InsertAfter(Node* currentNode, Node* newNode);
    // Remove will also help us with sorting
    void Remove(string id);
    void PrintList();
    // to sort the list in alphanumerical order
    void sortList();
    // search for a specific course
    Course Lookup(string id);
    int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
    // FIXME (1): Initialize housekeeping variables
    //set head and tail equal to null
    head = nullptr;
    tail = nullptr;
    size = 0;
}
/**
 * Destructor
 */
LinkedList::~LinkedList() {
    // start at the head
    Node* current = head;
    Node* temp;

    // loop over each node, detach from list then delete
    while (current != nullptr) {
        temp = current; // hang on to current node
        current = current->next; // make current the next node
        delete temp; // delete the orphan node
    }
}
/**
 * Append a new course to the end of the list
 */
void LinkedList::Append(Course course) {
    // FIXME (2): Implement append logic
    //Create new node
    Node* node = new Node();
    node->course = course;
    node->next = nullptr;
    node->prev = nullptr;
    //if there is nothing at the head...

    if (head == nullptr) {
        // new node becomes the head 
        head = node;
        // and the tail
        tail = node;
    }
    //else 
    else {
        node->prev = tail;
        // make current tail node point to the new node
        tail->next = node;
        // and tail becomes the new node
        tail = node;
    }
    //increase size count
    size = size + 1;
}

/**
 * Prepend a new course to the start of the list
 */
void LinkedList::Prepend(Course course) {
    // FIXME (3): Implement prepend logic
    // Create new node
    Node* node = new Node();
    node->course = course;
    node->next = nullptr;
    node->prev = nullptr;
    // if there is already something at the head...
    if (head != nullptr) {
        // new node points to current head as its next node
        node->next = head;
        head = node;
    }
    // if there is nothing in the head, it means the list is empty
    else {
        // new node becomes the head and tail
        // head now becomes the new node
        head = node;
        tail = node;
    }
    //increase size count
    size = size + 1;
}

/**
 * Remove a specified course
 *
 * @param courseId The course id to remove from the list
 */
void LinkedList::Remove(string id) {
    // FIXME (5): Implement remove logic
    // special case if matching node is the head
    if (head->course.id == id) {
        // make head point to the next node in the list
        head = head->next;
        //decrease size count
        size = size - 1;
        return;
    }
    // start with the head
    Node* previousNode = head;
    // while loop over each node looking for a match
    while (previousNode->next != nullptr) {
        Node* currentNode = previousNode->next;
        // if the next node courseID is equal to the current courseID
        if (currentNode->course.id == id) {
            // make current node point beyond the next node
            previousNode->next = currentNode->next;
            // decrease size count
            size = size - 1;
            return;
        }
        // curretn node is equal to next node
        previousNode = previousNode->next;
    }
}

void LinkedList::InsertAfter(Node* currentNode, Node* newNode) {
    // if the head is null
    if (head == nullptr) {
        // let head and tail be the new node
        head = newNode;
        tail = newNode;
    }
    // if the current node  is the tail
    else if (currentNode == tail) {
        // let it have the next node as the new node
        tail->next = newNode;
        // the new node's previous will be the tail
        newNode->prev = tail;
        // the tail of the list will become the new node
        tail = newNode;
    }
    else {
        Node* sucNode = currentNode->next;
        newNode->next = sucNode;
        newNode->prev = currentNode;
        currentNode->next = newNode;
        sucNode->prev = newNode;
    }
}
/**
 * Simple output of all courses in the list
 */
void LinkedList::PrintList() {
    // FIXME (4): Implement print logic
    sortList();
    // start at the head
    Node* currentNode = head;
    // while loop over each node looking for a match
    while (currentNode != nullptr) {
        Course course = currentNode->course;
        cout << course.to_string();
        //set current equal to next
        currentNode = currentNode->next;
    }
}

/**
 * Search for the specified courseId
 *
 * @param id The course id to search for
 */
Course LinkedList::Lookup(string id) {
    // FIXME (6): Implement search logic
    Course course;
    Node* currentNode = head;
    // special case if matching node is the head
    if (currentNode->course.id == id) {
        return currentNode->course;
    }

    // start at the head of the list
    // keep searching until end reached with while loop (next != nullptr
    while (currentNode != nullptr) {
        // if the current node matches, return it
        if (currentNode->course.id == id) {
            return currentNode->course;
        }
        // else current node is equal to next node
        currentNode = currentNode->next;

    }
    return course;
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    return size;
}
/**
 * Will sort the list in place
 */
void LinkedList::sortList() {
    // current node is head's next
    Node* currentNode = head->next;
    // loop when current node is not null
    while (currentNode != nullptr) {
        // set next node to curr next
        Node* nextNode = currentNode->next;
        // set search node to curr prev
        Node* searchNode = currentNode->prev;

        while (searchNode != nullptr && searchNode->course.id > currentNode->course.id) {
            searchNode = searchNode->prev;
        }
        // remove current node
        Remove(currentNode->course.id);
        // if the search node is null
        if (searchNode == nullptr) {
            // set curr prev to null
            currentNode->prev = nullptr;
            // prepend the current node
            Prepend(currentNode->course);
        }
        else {
            // insert after search node the current node
            InsertAfter(searchNode, currentNode);
        }
        // continue 
        currentNode = nextNode;
    }
}
/**
 * Loads data from excel to the list
 *
 * @param csvPath is the path for the csv file 
 * The data structure Linked list => list
 */
void loadCourses(string csvPath, LinkedList* list) {
    csv::Parser file = csv::Parser(csvPath);
    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            // initiate a course
            Course c;
            // set respectively the fields from row to params
            c.id = file[i][0];
            c.name = file[i][1];
            // set the prerequisites in the vector
            for (unsigned int p = 2; p < file[i].size(); p++) {
                if (file[i][p] != "") {
                    c.prereq.push_back(file[i][p]);
                }
            }
            // append it to the list
            list->Append(c);
        }
    }
    // catch if any exception
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}
/**
 * It just prints a menu, we are using it many time so it's good to have seperately
 *
 */
void printMenu() {
    cout << "----------------------------" << endl << "\t1. Load Data Structure." << endl
        << "\t2. Print Course List." << endl
        << "\t3. Print Course." << endl
        << "\t9. Exit." << endl << endl
        << "What would you like to do? ";
}

// handle menu and input function
void navigation() {
    // structure instantiation
    LinkedList list;
    // previewing menu
    printMenu();
    int choice;
    cin >> choice;
    // setting the number 9 as the quitting number
    while (choice != 9) {
        string id;
        Course course;
        switch (choice) {
        case 1:
            // call the function that loads data to Linked list
            loadCourses("CourseInformation.csv", &list);
            // print the menu again to keep the program running
            printMenu();
            // read the new choice
            cin >> choice;
            break;
        case 2:
            // call the function that sorts and prints the courses in alphabetical order
            list.PrintList();
            // print the menu again to keep the program running
            printMenu();
            // read the new choice
            cin >> choice;
            break;
        case 3:
            // prompt user to enter the course (case sensitive)
            cout << "What course do you want to know about ? ";
            // read course id
            cin >> id;
            // call the function that looks up the course and returns it
            course = list.Lookup(id);
            // check if the course is not null
            if (!course.id.empty()) {
                // print it
                cout << course.to_string();
            }
            else {
                // was not found
                cout << "Course not found." << endl;
            }
            // print the menu again to keep the program running
            printMenu();
            // read the new choice
            cin >> choice;
            break;
        default:
            cout << choice << " is not a valid option." << endl;
            // print the menu again to keep the program running
            printMenu();
            // read the new choice
            cin >> choice;
            break;
        }
    }
    return;
    
}

int main()
{
    // welcome the user
    cout << "Welcome to course planner." << endl;
    // Program
    navigation();
    // Goodbye.
    cout << "Thank you for using course planner!" << endl;
    return 0;
}
