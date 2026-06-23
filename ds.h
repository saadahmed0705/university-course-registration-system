#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct QueueNode {
    int student_id;
    QueueNode* next;

    QueueNode(int id) : student_id(id), next(nullptr) {}
};

class CustomQueue {

    QueueNode* frontNode;
    QueueNode* rearNode;
    int qsize;

public:
    CustomQueue() : frontNode(nullptr), rearNode(nullptr), qsize(0) {}

    void enqueue(int id) {
        QueueNode* newnode = new QueueNode(id);
        if (!rearNode) {
            frontNode = rearNode = newnode;
        }

        else {
            rearNode->next = newnode;
            rearNode = newnode;
        }
        qsize++;
    }

    void dequeue() {
        if (!frontNode) return;
        QueueNode* temp = frontNode;
        frontNode = frontNode->next;
        if (!frontNode) rearNode = nullptr;
        delete temp;
        qsize--;
    }

    int front() {
        if (frontNode) return frontNode->student_id;
        return -1;
    }

    bool isEmpty() { return frontNode == nullptr; }
    int size() { return qsize; }

    void clear() {
        while (!isEmpty()) dequeue();
    }

    void display() {
        if (isEmpty()) {
            cout << "Queue is empty.\n";
            return;
        }
        QueueNode* temp = frontNode;
        int pos = 1;
        while (temp) {
            cout << pos << ". Student ID: " << temp->student_id << "\n";
            temp = temp->next;
            pos++;
        }
    }

    bool contains(int id) {
        QueueNode* temp = frontNode;
        while (temp) {
            if (temp->student_id == id) return true;
            temp = temp->next;
        }
        return false;
    }

    void copyFrom(CustomQueue& other) {
        clear();
        QueueNode* t = other.frontNode;
        while (t) {
            enqueue(t->student_id);
            t = t->next;
        }
    }

    ~CustomQueue() { clear(); }

};

struct BSTNode {
    string course_id;
    string name;
    string instructor;
    int capacity;
    int enrolled;
    string day;
    int time_slot;
    CustomQueue waitlistQueue;
    int feedback_count;
    double avg_rating;
    BSTNode* left;
    BSTNode* right;

    BSTNode(string id, string n, string instr, int cap, string d, int t)
        : course_id(id), name(n), instructor(instr), capacity(cap), enrolled(0), day(d), time_slot(t), feedback_count(0), avg_rating(0.0), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {

public:
    BSTNode* root;

    BinarySearchTree() : root(nullptr) {}

    BSTNode* insert(BSTNode* node, string id, string n, string instr, int cap, string d, int t) {
        if (!node) return new BSTNode(id, n, instr, cap, d, t);
        if (id < node->course_id) node->left = insert(node->left, id, n, instr, cap, d, t);
        else if (id > node->course_id) node->right = insert(node->right, id, n, instr, cap, d, t);

        return node;
    }

    BSTNode* search(BSTNode* node, string id) {
        if (!node || node->course_id == id) return node;
        if (id < node->course_id) return search(node->left, id);
        return search(node->right, id);
    }

    void show(BSTNode* node) {
        cout << "Course: " << node->course_id << " - " << node->name << " | Instructor: " << node->instructor
             << " | " << node->enrolled << "/" << node->capacity << " | " << node->day << " " << node->time_slot
             << ":00 | Rating: ";
        if (node->avg_rating > 0) cout << node->avg_rating;
        else cout << "N/A";
        cout << " | Queue Size: " << node->waitlistQueue.size() << endl;
    }

    void inorder(BSTNode* node) {
        if (!node) return;
        inorder(node->left);
        show(node);
        inorder(node->right);
    }

    void preorder(BSTNode* node) {
        if (!node) return;
        show(node);
        preorder(node->left);
        preorder(node->right);
    }

    void postorder(BSTNode* node) {
        if (!node) return;
        postorder(node->left);
        postorder(node->right);
        show(node);
    }

    void searchByTime(BSTNode* node, string day, int time, bool& found) {
        if (!node) return;
        searchByTime(node->left, day, time, found);
        if (node->day == day && node->time_slot == time) {
            show(node);
            found = true;
        }
        searchByTime(node->right, day, time, found);
    }

    BSTNode* findMin(BSTNode* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    BSTNode* remove(BSTNode* node, string id) {
        if (!node) return nullptr;
        if (id < node->course_id) node->left = remove(node->left, id);
        else if (id > node->course_id) node->right = remove(node->right, id);
        else {

            if (!node->left) {
                BSTNode* temp = node->right;
                node->waitlistQueue.clear();
                delete node;
                return temp;
            }

            else if (!node->right) {
                BSTNode* temp = node->left;
                node->waitlistQueue.clear();
                delete node;
                return temp;
            }

            BSTNode* temp = findMin(node->right);
            node->course_id = temp->course_id;
            node->name = temp->name;
            node->instructor = temp->instructor;
            node->capacity = temp->capacity;
            node->enrolled = temp->enrolled;
            node->day = temp->day;
            node->time_slot = temp->time_slot;
            node->avg_rating = temp->avg_rating;
            node->feedback_count = temp->feedback_count;
            node->waitlistQueue.copyFrom(temp->waitlistQueue);
            node->right = remove(node->right, temp->course_id);
        }

        return node;
    }

    int countNodes(BSTNode* node) {
        if (!node) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

};

struct TimetableNode {
    BSTNode* course;
    TimetableNode* next;

    TimetableNode(BSTNode* c) : course(c), next(nullptr) {}
};

class TimetableList {

    TimetableNode* head;
    int count;

public:
    TimetableList() : head(nullptr), count(0) {}

    int compare(BSTNode* a, BSTNode* b) {
        string days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
        int da = 0, db = 0;
        for (int i = 0; i < 5; i++) {
            if (a->day == days[i]) da = i;
            if (b->day == days[i]) db = i;
        }
        if (da != db) return da - db;
        return a->time_slot - b->time_slot;
    }

    void insert(BSTNode* c) {
        TimetableNode* newnode = new TimetableNode(c);
        if (!head || compare(c, head->course) < 0) {
            newnode->next = head;
            head = newnode;
            count++;
            return;
        }
        TimetableNode* cur = head;
        while (cur->next && compare(c, cur->next->course) >= 0) {
            cur = cur->next;
        }
        newnode->next = cur->next;
        cur->next = newnode;
        count++;
    }

    void remove(string course_id) {
        TimetableNode* temp = head;
        TimetableNode* prev = nullptr;
        while (temp && temp->course->course_id != course_id) {
            prev = temp;
            temp = temp->next;
        }
        if (!temp) return;
        if (!prev) head = temp->next;
        else prev->next = temp->next;
        delete temp;
        count--;
    }

    bool hasClash(string day, int time_slot) {
        TimetableNode* temp = head;
        while (temp) {
            if (temp->course->day == day && temp->course->time_slot == time_slot) return true;
            temp = temp->next;
        }
        return false;
    }

    bool isAlreadyRegistered(string course_id) {
        TimetableNode* temp = head;
        while (temp) {
            if (temp->course->course_id == course_id) return true;
            temp = temp->next;
        }
        return false;
    }

    bool isFull(int max = 6) { return count >= max; }

    void display() {
        if (!head) {
            cout << "List is empty.\n";
            return;
        }
        cout << "=== WEEKLY LIST ===\n";
        string curDay = "";
        TimetableNode* temp = head;
        while (temp) {
            if (temp->course->day != curDay) {
                curDay = temp->course->day;
                cout << curDay << ":\n";
            }
            cout << "  " << temp->course->time_slot << ":00 - " << temp->course->course_id << " (" << temp->course->name << ")\n";
            temp = temp->next;
        }
        cout << "Total items: " << count << "\n";
    }

    TimetableNode* getHead() { return head; }

    ~TimetableList() {
        TimetableNode* temp = head;
        while (temp) {
            TimetableNode* next = temp->next;
            delete temp;
            temp = next;
        }
    }

};

struct Student {
    int student_id;
    string name;
    int year;
    TimetableList coursesList;

    Student() : student_id(0), name(""), year(1) {}
    Student(int id, string n, int y) : student_id(id), name(n), year(y) {}
};

// This node proves the Hash Map uses Linked Lists for collisions (Separate Chaining)
struct StudentNode {
    Student data;
    StudentNode* next;

    StudentNode(Student s) : data(s), next(nullptr) {}
};

class StudentLinkedList {

public:
    StudentNode* head;

    StudentLinkedList() : head(nullptr) {}

    void insert(Student s) {
        StudentNode* newnode = new StudentNode(s);
        newnode->next = head;
        head = newnode;
    }

    Student* search(int id) {
        StudentNode* temp = head;
        while (temp) {
            if (temp->data.student_id == id) return &(temp->data);
            temp = temp->next;
        }
        return nullptr;
    }

    ~StudentLinkedList() {
        StudentNode* temp = head;
        while (temp) {
            StudentNode* next = temp->next;
            delete temp;
            temp = next;
        }
    }

};

class StudentHashMap {

    static const int size = 20;
    StudentLinkedList table[size];

public:
    int hashfunction(int id) { return id % size; }

    void insert(int id, string name, int year) {
        int index = hashfunction(id);
        Student s(id, name, year);
        table[index].insert(s);
    }

    Student* search(int id) {
        int index = hashfunction(id);
        return table[index].search(id);
    }

    int getAllItems(Student** arr, int maxSize) {
        int idx = 0;
        for (int i = 0; i < size && idx < maxSize; i++) {
            StudentNode* temp = table[i].head;
            while (temp && idx < maxSize) {
                arr[idx++] = &(temp->data);
                temp = temp->next;
            }
        }
        return idx;
    }

    StudentLinkedList* getTable() { return table; }
    int getSize() { return size; }

};

// REGISTRATION SYSTEM CONTROLLER

class RegistrationSystem {

public:
    BinarySearchTree coursetree;
    StudentHashMap students;

    void initialiseSystem() {
        loadstudents();
        loadcourses();
        loadRegistrations();
        loadfeedback();
    }

    void loadstudents() {
        ifstream file("students.txt");
        if (!file) return;
        int id, year;
        string name;
        while (file >> id >> name >> year) {
            students.insert(id, name, year);
        }
        file.close();
    }

    void savestudents() {
        ofstream file("students.txt");
        Student* all[1000];
        int cnt = students.getAllItems(all, 1000);
        for (int i = 0; i < cnt; i++) {
            file << all[i]->student_id << " " << all[i]->name << " " << all[i]->year << endl;
        }
        file.close();
    }

    void loadcourses() {
        ifstream file("courses.txt");
        if (!file) return;
        string id, name, instructor, day;
        int cap, time, enrolled;
        while (file >> id >> name >> instructor >> cap >> day >> time >> enrolled) {
            coursetree.root = coursetree.insert(coursetree.root, id, name, instructor, cap, day, time);
        }
        file.close();
    }

    void saveCoursesRec(BSTNode* node, ofstream& file) {
        if (!node) return;
        file << node->course_id << " " << node->name << " " << node->instructor << " " << node->capacity << " " << node->day << " " << node->time_slot << " " << node->enrolled << endl;
        saveCoursesRec(node->left, file);
        saveCoursesRec(node->right, file);
    }

    void savecourses() {
        ofstream file("courses.txt");
        saveCoursesRec(coursetree.root, file);
        file.close();
    }

    void loadfeedback() {
        ifstream file("feedback.txt");
        if (!file) return;
        int sid, rating;
        string cid, comment;
        while (file >> sid >> cid >> rating) {
            getline(file, comment);
            BSTNode* c = coursetree.search(coursetree.root, cid);
            if (c) {
                c->avg_rating = ((c->avg_rating * c->feedback_count) + rating) / (c->feedback_count + 1);
                c->feedback_count++;
            }
        }
        file.close();
    }

    void saveall() {
        savestudents();
        savecourses();
        saveRegistrations();
    }

    void saveRegistrations() {
        ofstream file("registrations.txt");
        Student* all[1000];
        int cnt = students.getAllItems(all, 1000);
        for (int i = 0; i < cnt; i++) {
            TimetableNode* t = all[i]->coursesList.getHead();
            while (t) {
                file << all[i]->student_id << " " << t->course->course_id << endl;
                t = t->next;
            }
        }
        file.close();
    }

    void loadRegistrations() {
        ifstream file("registrations.txt");
        if (!file) return;
        int sid;
        string cid;
        while (file >> sid >> cid) {
            Student* st = students.search(sid);
            BSTNode* course = coursetree.search(coursetree.root, cid);
            if (st && course) {
                st->coursesList.insert(course);
                course->enrolled++;
            }
        }
        file.close();
    }

    void addStudent(int id, string name, int year) {
        if (students.search(id)) {
            cout << "Student ID already exists.\n";
            return;
        }
        students.insert(id, name, year);
        cout << "Student " << name << " added.\n";
    }

    void addCourse(string id, string name, string instr, int cap, string day, int time) {
        if (coursetree.search(coursetree.root, id)) {
            cout << "Course already exists.\n";
            return;
        }
        coursetree.root = coursetree.insert(coursetree.root, id, name, instr, cap, day, time);
        cout << "Course " << id << " added.\n";
    }

    void cancelCourse(string cid) {
        BSTNode* course = coursetree.search(coursetree.root, cid);
        if (!course) {
            cout << "Course not found.\n";
            return;
        }
        cout << "\n*** COURSE CANCELLATION: " << course->course_id << " - " << course->name << " ***\n";
        for (int i = 0; i < students.getSize(); i++) {
            StudentNode* temp = students.getTable()[i].head;
            while (temp) {
                temp->data.coursesList.remove(cid);
                temp = temp->next;
            }
        }
        course->waitlistQueue.clear();
        coursetree.root = coursetree.remove(coursetree.root, cid);
        cout << "Course removed successfully.\n";
    }

    int registerCourse(int sid, string cid) {
        Student* st = students.search(sid);
        if (!st) {
            cout << "Student not found.\n";
            return -1;
        }
        BSTNode* course = coursetree.search(coursetree.root, cid);
        if (!course) {
            cout << "Course not found.\n";
            return -2;
        }

        // BUG FIX: Prevent Double Registration
        if (st->coursesList.isAlreadyRegistered(cid)) {
            cout << "Error: You are already registered for this course.\n";
            return -5;
        }

        if (st->coursesList.isFull()) {
            cout << "Max courses reached.\n";
            return -3;
        }
        if (st->coursesList.hasClash(course->day, course->time_slot)) {
            cout << "Timetable clash detected on " << course->day << " at " << course->time_slot << ":00.\n";
            return -4;
        }

        if (course->enrolled < course->capacity) {
            course->enrolled++;
            st->coursesList.insert(course);
            cout << "Success! Registered " << st->name << " in " << course->course_id << endl;
            return 0;
        }

        else {
            if (course->waitlistQueue.contains(sid)) {
                cout << "You are already on the waitlist for this course.\n";
                return 2;
            }
            course->waitlistQueue.enqueue(sid);
            cout << "Course is full. Added to Waitlist Queue (Position " << course->waitlistQueue.size() << ").\n";
            return 1;
        }
    }

    void dropCourse(int sid, string cid) {
        Student* st = students.search(sid);
        BSTNode* course = coursetree.search(coursetree.root, cid);

        if (!st || !course) {
            cout << "Student or course not found.\n";
            return;
        }

        // BUG FIX: Ensure the student actually is taking the course before dropping
        if (!st->coursesList.isAlreadyRegistered(cid)) {
            cout << "You are not registered for " << cid << ".\n";
            return;
        }

        st->coursesList.remove(cid);
        course->enrolled--;
        cout << st->name << " successfully dropped " << course->course_id << endl;

        processQueue(course);
    }

    void processQueue(BSTNode* course) {
        while (!course->waitlistQueue.isEmpty() && course->enrolled < course->capacity) {
            int nextid = course->waitlistQueue.front();
            Student* nxt = students.search(nextid);

            if (!nxt) {
                course->waitlistQueue.dequeue();
                continue;
            }
            
            if (nxt->coursesList.hasClash(course->day, course->time_slot) || nxt->coursesList.isFull()) {
                cout << "Next in queue (" << nxt->name << ") can't enroll (clash/max). Seat held.\n";
                break;
            }

            course->waitlistQueue.dequeue();
            course->enrolled++;
            nxt->coursesList.insert(course);
            cout << "Queue Update: " << nxt->name << " automatically enrolled in " << course->course_id << endl;
        }
    }

    void submitFeedback(int sid, string cid, int rating, string comment) {
        Student* st = students.search(sid);
        BSTNode* course = coursetree.search(coursetree.root, cid);
        if (!st || !course) {
            cout << "Invalid student or course ID.\n";
            return;
        }

        if (rating < 1 || rating > 5) {
            cout << "Rating must be between 1 and 5.\n";
            return;
        }

        if (!st->coursesList.isAlreadyRegistered(cid)) {
            cout << "You can only review a course you are enrolled in.\n";
            return;
        }

        course->avg_rating = ((course->avg_rating * course->feedback_count) + rating) / (course->feedback_count + 1);
        course->feedback_count++;

        ofstream f("feedback.txt", ios::app);
        f << sid << " " << cid << " " << rating << " " << comment << endl;
        f.close();

        cout << "Feedback submitted successfully.\n";
    }

    void viewFeedback(string cid) {
        BSTNode* course = coursetree.search(coursetree.root, cid);
        if (!course) {
            cout << "Course not found.\n";
            return;
        }
        cout << "\n--------------- COURSE FEEDBACK ---------------\n";
        cout << "Course: " << course->course_id << " - " << course->name << "\n";
        cout << "Total Reviews: " << course->feedback_count << "  | Avg Rating: " << course->avg_rating << "/5\n";
        cout << "----------------------------------------------\n";
    }

    void displayAllCourses() {
        if (!coursetree.root) {
            cout << "No courses available.\n";
            return;
        }
        cout << "\n--------------- COURSES (Inorder) ---------------\n";
        coursetree.inorder(coursetree.root);
        cout << "-------------------------------------------------\n";
    }

    void displayCoursesPre() {
        if (!coursetree.root) {
            cout << "No courses available.\n";
            return;
        }
        cout << "\n--------------- COURSES (Preorder) ---------------\n";
        coursetree.preorder(coursetree.root);
        cout << "--------------------------------------------------\n";
    }

    void displayCoursesPost() {
        if (!coursetree.root) {
            cout << "No courses available.\n";
            return;
        }
        cout << "\n--------------- COURSES (Postorder) ---------------\n";
        coursetree.postorder(coursetree.root);
        cout << "---------------------------------------------------\n";
    }

    void viewTimetable(int sid) {
        Student* st = students.search(sid);
        if (!st) {
            cout << "Student not found.\n";
            return;
        }
        cout << "\n--------------- " << st->name << " (Year " << st->year << ") - TIMETABLE ---------------\n";
        st->coursesList.display();
        cout << "------------------------------------------------------------\n";
    }

    void viewWaitlist(string cid) {
        BSTNode* course = coursetree.search(coursetree.root, cid);
        if (!course) {
            cout << "Course not found.\n";
            return;
        }
        cout << "\n--------------- WAITLIST: " << course->course_id << " ---------------\n";
        course->waitlistQueue.display();
        cout << "------------------------------------------------\n";
    }

    void searchByTime(string day, int time) {
        bool found = false;
        cout << "\n--------------- COURSES ON " << day << " AT " << time << ":00 ---------------\n";
        coursetree.searchByTime(coursetree.root, day, time, found);
        if (!found) cout << "No courses available at that time.\n";
        cout << "----------------------------------------------------\n";
    }

    void stats() {
        cout << "\n--------------- SYSTEM STATS ---------------\n";
        cout << "Total Courses in System: " << coursetree.countNodes(coursetree.root) << "\n";
        Student* all[1000];
        int cnt = students.getAllItems(all, 1000);
        cout << "Total Registered Students: " << cnt << "\n";
        cout << "--------------------------------------------\n";
    }

};
