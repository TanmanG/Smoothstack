#include <iostream>
#include <vector>

struct Student {
	std::string studentId;
	int score;
	bool comparator(Student x, Student y) {
		return (x.score < y.score);
	}
};

class MergeSort {
	private:
		std::vector<Student> merge(std::vector<Student> arr1, std::vector<Student> arr2) {
			bool (Student::*lessFunc)(Student, Student) = &Student::comparator;
			std::vector<Student> students;
			Student funcStudent;

			int lCounter = 0;
			int rCounter = 0;

			for (int i = 0; i < arr1.size() + arr2.size(); i++) {
				// Check if one vector has run out of entries.
				if (lCounter == arr1.size() || rCounter == arr2.size()) {
					break;
				}

				if ((funcStudent.*lessFunc)(arr1.at(lCounter), arr2.at(rCounter))) {
					// Left is smaller
					students.push_back(arr1.at(lCounter));
					lCounter++;
				}
				else {
					// Right is smaller
					students.push_back(arr2.at(rCounter));
					rCounter++;
				}
			}

			// Push any remaining students.
			while (lCounter < arr1.size()) {
				students.push_back(arr1.at(lCounter));
				lCounter++;
			}
			while (rCounter < arr2.size()) {
				students.push_back(arr2.at(rCounter));
				rCounter++;
			}

			return students;
		}
		std::vector<Student> recurSort(std::vector<Student>& arr1) {
			if (arr1.size() == 1) {
				return arr1;
			}
			std::vector<Student> sortedStudents1(arr1.begin(), arr1.begin() + (arr1.size() / 2));
			sortedStudents1 = recurSort(sortedStudents1);
			std::vector<Student> sortedStudents2(arr1.begin() + (arr1.size() / 2), arr1.end());
			sortedStudents2 = recurSort(sortedStudents2);

			return merge(sortedStudents1, sortedStudents2);
		}
	public:
		void sort(std::vector<Student>& students) {
			students = recurSort(students);
		}
};

int main() {
	Student steve, sarah, carrel, ryan;
	steve.score = 50;
	steve.studentId = "Steven";
	sarah.score = 30;
	sarah.studentId = "Sarah";
	carrel.score = 90;
	carrel.studentId = "Carrey";
	ryan.score = 20;
	ryan.studentId = "Ryan";


	std::vector<Student> students;
	students.push_back(steve);
	students.push_back(sarah);
	students.push_back(carrel);
	students.push_back(ryan);

	std::cout << "Pre-Sort" << '\n';
	for (int i = 0; i < students.size(); i++) {
		std::cout << students.at(i).studentId << " " << students.at(i).score << '\n';
	}

	MergeSort sorter;
	sorter.sort(students);

	std::cout << "----------" << '\n' << "Post-Sort" << '\n';
	for (int i = 0; i < students.size(); i++) {
		std::cout << students.at(i).studentId << " " << students.at(i).score << '\n';
	}
}
