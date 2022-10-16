//============================================================================
// Name        : DSA_Project.cpp
// Author      : Jad Alrehaoui
// Version     : 1.0
// Description : Project Submission
//============================================================================


#include <iostream>
#include "CSVParser.hpp"
#include <string>
using namespace std;


int convertStringToInt(string str) {

    int val = 0;
    for (unsigned i = 0; i < str.length() - 3; i++) {
        int letter_num = (int)str[i] - 48;
        val += letter_num;
    }
    int added_course_num = stoi(str.substr(str.size() - 3));
    return val + added_course_num;
}

struct Course {
    string id;
    string name;
    vector<string> prereq;

    void print() {
        cout << id << " | " << name << endl;
        for (unsigned int i = 0; i < prereq.size(); i++) {
            cout << "\t" << i + 1 << " " << prereq[i] << endl;
        }
    }
};
const unsigned int DEFAULT_SIZE = 500;

class HashTable {

private:
    // Define structures to hold courses
    struct Node {
        Course course;
        unsigned int key;
        Node* next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a course
        Node(Course aCourse) : Node() {
            course = aCourse;
        }

        // initialize with a course and a key
        Node(Course aCourse, unsigned int aKey) : Node(aCourse) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Course course);
    void Lookup();
    void PrintAll();
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // FIXME (1): Initialize the structures used to hold courses
    nodes.resize(tableSize);
    // Initalize node structure by resizing tableSize
}
/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    // invoke local tableSize to size with this->
    // resize nodes size
    this->tableSize = size;
    nodes.resize(tableSize);
}


/**
 * Destructor
 */
HashTable::~HashTable() {
    // FIXME (2): Implement logic to free storage when class is destroyed
    // erase nodes beginning
    nodes.erase(nodes.begin());
}
unsigned int HashTable::hash(int key) {
    // FIXME (3): Implement logic to calculate a hash value
    // return key tableSize
    return key % tableSize;
}

/**
 * Insert a course
 *
 * @param course The course to insert
 */
void HashTable::Insert(Course course) {
    //string id_num = to_string(convertStringToInt(course.id));
    string id_num = course.id.substr(course.id.size() - 3);
    int key = hash(atoi(id_num.c_str())); // c_str is called to insure that the key is treated as a string.
    // retrieve node using key
    Node* node_at_key = &(nodes.at(key)); // *** StackOverflow *** suggested to put & character on the nodes.at() 
    // if no entry found for the key
    if (node_at_key == nullptr) {
        // assign this node to the key position
        Node* to_be_filled = new Node(course, key);
        nodes.insert(nodes.begin() + key, (*to_be_filled));
    }
    // else if node is not used
    else {
        // assing old node key to UNIT_MAX, set to key, set old node to course and old node next to null pointer
        if (node_at_key->key == UINT_MAX) {
            node_at_key->key = key;
            node_at_key->course = course;
            node_at_key->next = nullptr;
        }
        // else find the next open node
        else {
            while (node_at_key->next != nullptr) {
                // add new newNode to end
                node_at_key = node_at_key->next;
            }
        }
    }
}

void HashTable::Lookup() {
    // FIXME (8): Implement logic to search for and return a course
    string id;
    cout << "What course do you want to know about? ";
    cin >> id;
    string id_num = id.substr(id.size() - 3);
    // create the key for the given course
    unsigned key = hash(atoi(id_num.c_str()));
    // if no entry found for the key
    if (&(nodes.at(key)) == nullptr) {
        // print course not found
        cout << "Course not found." << endl;
        return;
    }
    else {
        //print course information
        nodes.at(key).course.print();
        return;
    }
    // while node not equal to nullptr
    Node* currentNode = nodes.at(key).next;
    while (currentNode != nullptr) {
        // if the current node matches, return it
        if (currentNode->course.id == id) {
            //node is equal to next node
            currentNode->course.print();
            return;
        }
        currentNode = currentNode->next;
    }
    cout << "Course not found." << endl;
    return;
}

/**
 * Print all courses
 */
void HashTable::PrintAll() {
    // FIXME (6): Implement logic to print all courses
    // for node begin to end iterate
    for (int i = 0; i < nodes.size(); ++i) {
        //   if key not equal to UINT_MAx
        if (nodes.at(i).key != UINT_MAX) {
            // output key, courseID, title, amount and fund
            nodes.at(i).course.print();
        }
    }
}

/*
 * Display the menu of options
 */
void DisplayMenu() {
    cout << "----------------------------" << endl << "\t1. Load Data Structure." << endl
        << "\t2. Print Course List." << endl
        << "\t3. Print Course." << endl
        << "\t9. Exit." << endl << endl
        << "What would you like to do? ";
}
/*
 * Loading Courses from CSV to Binary Tree
 */
void loadCoursesFromCSV(string csvPath, HashTable* ht) {
    csv::Parser file = csv::Parser(csvPath);
    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            Course c;
            c.id = file[i][0];
            c.name = file[i][1];
            for (unsigned int p = 2; p < file[i].size(); p++) {
                if (file[i][p] != "") {
                    c.prereq.push_back(file[i][p]);
                }
            }
            ht->Insert(c);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}
/*
 * Reading the selection input by the user
 */
void readChoice(HashTable* ht) {
    // choice is the selected
    int choice;
    // reading the choice
    cin >> choice;
    // branching on the option
    // 1 - load 2- print list 3- print course 9- exit
    switch (choice) {
        case 1: 
            // load selected
            // calling the function that loads the courses
            loadCoursesFromCSV("CourseInformation.csv", ht);
            cout << "Courses loaded successfully." << endl;
            // displaying the menu again
            DisplayMenu();
            // reading the choice again
            readChoice(ht);
            break;
        case 2: 
;           // displaying the menu again
            ht->PrintAll();
            DisplayMenu();
            // reading the choice again
            readChoice(ht);
            break;
        case 3:
            // start look up process
            ht->Lookup();
            // displaying the menu again
            DisplayMenu();
            // reading the choice again
            readChoice(ht);
            break;
        case 9: 
            // exited
            cout << "Thank you for using course planner!" << endl;
            // ending
            return;
            break;
        default: 
            // option not valid => user selected an invalid option
            cout << choice << " is not a valid option." << endl;
            // displaying the menu again
            DisplayMenu();
            // reading the choice again
            readChoice(ht);
    }
}

int main()
{
    // welcome phrase
    cout << "Welcome to course planner." << endl;
    HashTable* ht = new HashTable();
    // call display menu
    DisplayMenu();
    // call read choide
    readChoice(ht);
    return 0;
}

