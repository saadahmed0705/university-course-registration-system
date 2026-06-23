#include "ds.h"

int main() {

    RegistrationSystem ttt;
    ttt.initialiseSystem();

    int choice;
    while (true) {
        cout << "\n--------------- MAIN MENU ---------------\n";
        cout << "1. Student Login\n2. Instructor\n3. Admin\n4. Stats\n5. Exit\n";
        cout << "-----------------------------------------\n";
        cout << "Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            int sid;
            cout << "Student ID: ";
            cin >> sid;
            Student* st = ttt.students.search(sid);
            if (!st) {
                cout << "Student record not found.\n";
                continue;
            }

            cout << "Welcome " << st->name << "\n";
            
            int op;

            while (true) {
                cout << "\n--------------- STUDENT MENU ---------------\n";
                cout << "1. View Courses\n2. Register\n3. Drop\n4. My Timetable\n5. Search by Time\n6. View Waitlist Queue\n7. Submit Feedback\n8. Logout\n";
                cout << "--------------------------------------------\n";
                cout << "Choice: ";
                if (!(cin >> op)) { cin.clear(); cin.ignore(10000, '\n'); continue; }

                if (op == 1) ttt.displayAllCourses();
                else if (op == 2) { string cid; cout << "Course ID: "; cin >> cid; ttt.registerCourse(sid, cid); ttt.saveall(); }
                else if (op == 3) { string cid; cout << "Course ID: "; cin >> cid; ttt.dropCourse(sid, cid); ttt.saveall(); }
                else if (op == 4) ttt.viewTimetable(sid);
                else if (op == 5) { string d; int t; cout << "Day (e.g., Monday): "; cin >> d; cout << "Time (e.g., 9): "; cin >> t; ttt.searchByTime(d, t); }
                else if (op == 6) { string cid; cout << "Course ID: "; cin >> cid; ttt.viewWaitlist(cid); }
                else if (op == 7) {
                    string cid, com; int r;
                    cout << "Course ID: "; cin >> cid;
                    cout << "Rating (1-5): "; cin >> r;
                    cin.ignore();
                    cout << "Comment: "; getline(cin, com);
                    ttt.submitFeedback(sid, cid, r, com);
                    ttt.saveall();
                }
                else if (op == 8) break;
            }
        }

        else if (choice == 2) {

            int op;

            while (true) {
                cout << "\n--------------- INSTRUCTOR MENU ---------------\n";
                cout << "1. View Courses\n2. Course Feedback\n3. View Waitlist Queue\n4. Back\n";
                cout << "-----------------------------------------------\n";
                cout << "Choice: ";
                if (!(cin >> op)) { cin.clear(); cin.ignore(10000, '\n'); continue; }

                if (op == 1) ttt.displayAllCourses();
                else if (op == 2) { string cid; cout << "Course ID: "; cin >> cid; ttt.viewFeedback(cid); }
                else if (op == 3) { string cid; cout << "Course ID: "; cin >> cid; ttt.viewWaitlist(cid); }
                else if (op == 4) break;
            }
        }

        else if (choice == 3) {

            int op;
            while (true) {
                cout << "\n--------------- ADMIN MENU ---------------\n";
                cout << "1. Add Student\n2. Add Course\n3. Cancel Course\n4. Inorder\n5. Preorder\n6. Postorder\n7. Stats\n8. Back\n";
                cout << "------------------------------------------\n";
                cout << "Choice: ";
                if (!(cin >> op)) { cin.clear(); cin.ignore(10000, '\n'); continue; }

                if (op == 1) { int id, y; string n; cout << "ID: "; cin >> id; cout << "Name: "; cin >> n; cout << "Year: "; cin >> y; ttt.addStudent(id, n, y); ttt.saveall(); }
                else if (op == 2) { string id, n, ins, d; int c, t; cout << "Course ID: "; cin >> id; cout << "Name: "; cin >> n; cout << "Instructor: "; cin >> ins; cout << "Capacity: "; cin >> c; cout << "Day: "; cin >> d; cout << "Time: "; cin >> t; ttt.addCourse(id, n, ins, c, d, t); ttt.saveall(); }
                else if (op == 3) { string cid; cout << "Course ID: "; cin >> cid; ttt.cancelCourse(cid); ttt.saveall(); }
                else if (op == 4) ttt.displayAllCourses();
                else if (op == 5) ttt.displayCoursesPre();
                else if (op == 6) ttt.displayCoursesPost();
                else if (op == 7) ttt.stats();
                else if (op == 8) break;
            }
        }

        else if (choice == 4) ttt.stats();
        
        else if (choice == 5) {
            cout << "\nbyeeee \n";
            ttt.saveall();
            break;
        }
    }

    return 0;
}
