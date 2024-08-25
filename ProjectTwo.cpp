//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Ryan Howard
// Description : Project Two
// Date        : 8/18/2024
//============================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>


using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// Define a structure to hold course information
struct Course {
	string courseName = "none";
	string courseNum;
	vector<string> preReqs;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string courseNum);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseNum);
    Course Search(string courseNum);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // Initialize housekeeping variables
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    while (root != nullptr) {
        Remove(root->course.courseNum);
    }
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root
    this->inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // Implement inserting a course into the tree
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        this->addNode(root, course);
    }
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseNum) {
    // call to removeNode
    this->removeNode(root, courseNum);
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseNum) {
    // Implement searching the tree for a course
    // set current node equal to root
    Node* curNode = root;

    // keep looping downwards until bottom reached or matching courseNum found
    while (curNode != nullptr) {
        // if match found, return current course
        if (curNode->course.courseNum.compare(courseNum) == 0) {
            return curNode->course;
        }

        // if course is smaller than current node then traverse left
        else if (curNode->course.courseNum.compare(courseNum) > 0) {
            curNode = curNode->left;
        }

        // else larger so traverse right
        else {
            curNode = curNode->right;
        }
    }



    Course course;
    return course;
}

/**
 * Add a course to some node
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // Implement inserting a course into the tree
    // if node is larger then add to left
    if (node->course.courseNum.compare(course.courseNum) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            this->addNode(node->left, course);
        }
    }
    // else node is smaller add to the right
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        else {
            // recurse down the left node
            this->addNode(node->right, course);
        }
    }
}

/*
* Print nodes in alphanumeric order
*/
void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //recursive call to left
        inOrder(node->left);

        //output courseNum, courseName, and prerequisites
        cout << node->course.courseNum << ", " << node->course.courseName << endl;

        //recursive call to right
        inOrder(node->right);
    }
}

/**
 * Remove a course from some node
 */
Node* BinarySearchTree::removeNode(Node* node, string courseNum) {
    // Implement removing a course from the tree
    // if node = nullptr return node
    if (node == nullptr) {
        return node;
    }

    // check for match and if so, remove left node using recursive call
    if (courseNum.compare(node->course.courseNum) < 0) {
        node->left = removeNode(node->left, courseNum);
    }
    // check for match and if so, remove right node using recursive call
    else if (courseNum.compare(node->course.courseNum) > 0) {
        node->right = removeNode(node->right, courseNum);
    }
    else {
        // if left node = nullptr && right node = nullptr delete node 
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        // if left node != nullptr && right node = nullptr delete node 
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }
        // if left node = nullptr && right node != nullptr delete node
        else if (node->left == nullptr && node->right != nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }
        // (otherwise more than one child so find the minimum)
        // create temp node to right
        else {
            Node* temp = node->right;

            // while left node is not nullptr keep moving temp left
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            // make node course (right) equal to temp course (left)
            node->course = temp->course;

            // remove right node using recursive call
            node->right = removeNode(node->right, temp->course.courseNum);
        }
    }

    return node;
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    //create instream
    ifstream inFile;
    //create variable to hold lines from file
    string line;
    //Create a vector to hold course numbers to compare for pre requisites
    vector<string> preCheck;
    
    //Open file passed
    inFile.open(csvPath);

    //Error handling file not open
    if (!inFile.is_open()) {
        cout << "File could not open." << endl;
        return;
    }

    //Loop to store all courseNum in vector to check against prerequisites
    while (getline(inFile, line)) {
        //Create string to hold course number
        string courseNum = "";
        //Create stringstream to hold each line
        stringstream ss(line);
        //Extract courseNumber from each line
        getline(ss, courseNum, ',');
        //Insert courseNum into the vector
        preCheck.push_back(courseNum);
    }

    inFile.clear();
    //Reset position cursor in file
    inFile.seekg(0);

    //Loop to read lines and load data into structure
    while (getline(inFile, line)) {
        //Create a temp course object
        Course course;
        //size_t variable to count fields
        size_t field = 0;
        //Variable to hold tokens
        string lineToken = "";
        //Stringstream to read lines
        stringstream ss(line);


        //Loop to read through each token and store in course object
        while (getline(ss, lineToken, ',')) {
            //if the field is zero set course number
            if (field == 0) {
                course.courseNum = lineToken;
            }//otherwise if the field is one set course name
            else if (field == 1) {
                course.courseName = lineToken;
            }//otherwise the field is a prerequisite
            else {
                for (auto& i : preCheck) {
                    if (lineToken.compare(i) == 0) {
                        course.preReqs.push_back(lineToken);
                    }
                }
            }
            field++;
        }
        if (field > 1) {
            bst->Insert(course);
        }
    }

    //Close the file
    inFile.close();
}

/**
 * Display the course information to the console
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {
    cout << course.courseNum << ", " << course.courseName << endl;
    cout << "Prerequisites: ";

    //If there are no prereqs display
    if (course.preReqs.empty()) {
        cout << "no prerequisites." << endl;
    } //otherwise display the prereqs formatted
    else {
        for (auto& i : course.preReqs) {
            cout << i;
            //If more than one prereq print comma
            if (i.compare(course.preReqs.back()) != 0) {
                cout << ", ";
            }
        }
    }

    cout << endl;
}
/*
* Convert case of any undercase letters typed into uppercase
*/
void convertUpper(string& courseNum) {
    //For loop to iterate through the string
    for (auto& i : courseNum) {
        if (isalpha(i)) {
            i = toupper(i);
        }
    }
}

/**
 * main() method
 */
int main(int argc, char* argv[]) {

    string csvPath, courseNum;

    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do? ";

        // Try statement to throw error when user input is incorrect
        try {
            cin >> choice;

            if (choice < 0 || choice > 3 && choice != 9) {
                throw (choice);
            }

            switch (choice) {

            case 1:
                // Take user input for file
                cout << "Enter file to be read: ";
                cin >> csvPath;
                // Complete the method call to load the courses
                loadCourses(csvPath, bst);
                cout << endl;
                break;

            case 2:
                cout << endl;
                bst->InOrder();
                cout << endl;
                break;

            case 3:
                // Search for course using user input
                cout << "Enter the course number: ";
                cin >> courseNum;

                convertUpper(courseNum);

                course = bst->Search(courseNum);
                cout << endl;

                if (course.courseName.compare("none") == 0) {
                    cout << "Course number " << courseNum << " not found." << endl;
                }
                else {
                    displayCourse(course);
                }

                cout << endl;
                break;

            case 9:
                break;

            default:
                throw (choice);
            }
        }
        // Catch statement to alert user of errors in input
        catch (int err) {
            cout << endl;
            cout << "Input is not valid, Enter a choice from the menu." << endl;
            cout << endl;
            // Clear the input before continuing to loop
            cin.clear();
            cin.ignore(100, '\n');
        }

    }

    cout << "Thank you for using the course planner!" << endl;

    return 0;
}