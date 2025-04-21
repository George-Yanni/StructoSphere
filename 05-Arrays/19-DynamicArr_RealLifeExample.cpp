#include <iostream>
#include <string>
using namespace std;

// Our custom dynamic array class to hold float grades
class GradeArray {
private:
    float* grades;    // Dynamic array to store grades
    int size;         // Number of grades added
    int capacity;     // Allocated memory size

public:
    GradeArray(int initialCapacity = 5) {
        size = 0;
        capacity = initialCapacity;
        grades = new float[capacity];
    }

    ~GradeArray() {
        delete[] grades;
    }

    void addGrade(float grade) {
        if (size == capacity) {
            resize();
        }
        grades[size++] = grade;
    }

    float getGrade(int index) const {
        if (index < 0 || index >= size) {
            cerr << "Invalid index!" << endl;
            return -1;
        }
        return grades[index];
    }

    int getSize() const {
        return size;
    }

    void printGrades() const {
        cout << "Student Grades: ";
        for (int i = 0; i < size; ++i) {
            cout << grades[i] << " ";
        }
        cout << endl;
    }

    float getAverage() const {
        if (size == 0) return 0.0f;
        float sum = 0;
        for (int i = 0; i < size; ++i)
            sum += grades[i];
        return sum / size;
    }

private:
    void resize() {
        int newCapacity = capacity * 2;
        float* newGrades = new float[newCapacity];
        for (int i = 0; i < size; ++i)
            newGrades[i] = grades[i];
        delete[] grades;
        grades = newGrades;
        capacity = newCapacity;
    }
};

// Main function: simulate entering grades
int main() {
    GradeArray gradebook;

    int numStudents;
    cout << "Enter number of students: ";
    cin >> numStudents;

    // Input grades for each student
    for (int i = 0; i < numStudents; ++i) {
        float grade;
        cout << "Enter grade for student #" << (i + 1) << ": ";
        cin >> grade;
        gradebook.addGrade(grade);
    }

    // Show grades
    gradebook.printGrades();

    // Show average
    cout << "Class average: " << gradebook.getAverage() << endl;

    return 0;
}
