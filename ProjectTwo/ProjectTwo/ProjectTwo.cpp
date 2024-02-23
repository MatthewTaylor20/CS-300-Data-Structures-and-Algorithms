// ProjectTwo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "CSVparser.hpp"

using namespace std;

//course structure
struct Course {
    string id;
    string name;
    vector<string> prerequisites;
};

//internal structure for tree node
struct Node {
    Course course;
    Node *left;
    Node* right;
    //default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }
    //initialize with a course
    Node(Course course) : Node() {
        this->course = course;
    }
};

//binary search tree class
class BinarySearchTree {

private:
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void validateCourseList(Node* node);
    
public:
    BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseId);
    void ValidateCourseList();
    
};

//forward declaration
void displayCourse(Course course);

//default constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

//traverse the tree in order
void BinarySearchTree::InOrder() {
    this->inOrder(root);
}
//traverse the tree validating the prerequisites 
void BinarySearchTree::ValidateCourseList() {
    validateCourseList(root);
}
//insert course to the tree
void BinarySearchTree::Insert(Course course) {
    //if root is nullptr
    if (root == nullptr) {
        //root is equal to new course node
        root = new Node(course);
    }
    else {
        //otherwise add course
        this->addNode(root, course);
    }
}

//search for a course
Course BinarySearchTree::Search(string courseId) {
    //set current node equal to root
    Node* current = root;

    //keep looping downwards until bottom reqched or matching courseId found
    while (current != nullptr) {
        //if match found, return current course
        if (current->course.id.compare(courseId) == 0) {
            return current->course;
        }

        //if course is smaller than current node, traverse left
        if (current->course.id.compare(courseId) < 0) {
            current = current->left;
        }
        //otherwise it is larger so traverse right
        else {
            current = current->right;
        }
    }
    Course course;
    return course;
}

// recursively add a course to some node
void BinarySearchTree::addNode(Node* node, Course course) {
    //if node is larger then add to left
    if (node->course.id.compare(course.id) > 0) {
        //if no left node
        if (node->left == nullptr) {
            //this node bocomes the left
            node->left = new Node(course);
        }
        //else recurse down the left node
        else {
            addNode(node->left, course);
        }
    }
    else {
        //if no right node
        if (node->right == nullptr) {
            //this node becomes the right
            node->right = new Node(course);
        }
        else {
            //recurse down the right side
            addNode(node->right, course);
        }
    }
}
void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to nullptre
    if (node == nullptr) {
        return;
    }
    //InOrder left
    inOrder(node->left);
    //display course info
    displayCourse(node->course);
    //InOrder right
    inOrder(node->right);
}

void displayCourse(Course course) {
    //print course id and name
    cout << course.id << ": " << course.name << endl;
    //print out each prerequisite followed by a comma on all but the last one. 
    cout << "Course prerequisites: ";
    for (unsigned int i = 0; i < course.prerequisites.size(); i++) {
        if (i = course.prerequisites.size() - 1) {
            cout << course.prerequisites[i];
        }
        else {
            cout << course.prerequisites[i] << ", ";
        }
    }
    
}

// make sure all prerequisites line up to an acutal course
void BinarySearchTree::validateCourseList(Node* node) {
    //if node is nullptr
    if (node == nullptr) {
        return;
    }
    validateCourseList(node->left);
    //traverse the tree and check to make sure that the prerequisites exist
    for (unsigned int i = 0; i < node->course.prerequisites.size(); i++) {
        if (Search(node->course.prerequisites[i]).id == "") {
            cout << node->course.name << " has a prerequisite " << node->course.prerequisites[i] << " that is not an actual course. It will be removed from the course list." << endl;
            //if the prerequisites dont exist in the tree, delete them. 
            node->course.prerequisites.erase(node->course.prerequisites.begin() + i);
        } 
    }
    validateCourseList(node->right);
}


void parseText(string fileName, BinarySearchTree* bst) {
    cout << "Loading CSV file " << fileName << endl;

    //initialize the CSV Parser with the fileName
    csv::Parser file = csv::Parser(fileName);

    try {
        //loop through each line of the CSV file to add each line as a course
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            Course course;
            //make sure there is more than one argument on the line or else skip it. 
            if (file[i].size() > 1) {
                
                course.id = file[i][0];
                course.name = file[i][1];
                //if there are more than two arguments, the remaining arguments go into the course prerequisit vector
                if (file[i].size() > 2) {
                    for (unsigned int j = 2; j < file[i].size(); j++) {
                        course.prerequisites.insert(course.prerequisites.end(), file[i][j]);
                    }
                }
            }
            bst->Insert(course);
        }
        bst->ValidateCourseList();
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}



int main()
{
    BinarySearchTree* bst = new BinarySearchTree();
    int choice = 0;
    string csvPath;
    string courseId;
    Course course;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load courses" << endl;
        cout << "  2. Find course" << endl;
        cout << "  3. Display all courses" << endl;
        cout << "  9. Exit" << endl;
        cin >> choice;
        switch (choice) {
        case 1:
            //get the file name and call parseText to read the file
            cout << "Please enter the name of the file from which you would like to load the courses:" << endl;
            cin >> csvPath;
            parseText(csvPath, bst);
            break;
        case 2:
            //get the search criteria and try to find the course. if the course is not blank, it will display the course ifo
            cout << "Please enter the course ID you would like to find:" << endl;
            cin >> courseId;
            course = bst->Search(courseId);
            if (course.id.compare("") == 0) {
                cout << "No course with that ID was found." << endl << endl;
            }
            else {
                displayCourse(bst->Search(courseId));
            }
            
            break;
        case 3:
            //display all the courses in order
            bst->InOrder();
            break;
        case 9:
            //print exit message
            cout << "Thank you! Goodbye.";
            break;
        default:
            //tell the user there was an error
            cout << "Please enter either 1, 2, 3, or 9. " << endl << endl;
        }
    }
    
}

