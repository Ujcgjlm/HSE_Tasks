#include <deque>
#include <algorithm>
#include <iostream>
#include "admission.h"

auto StudentTieWithoutPoints(const Student* a) {
    return std::tie(a->birth_date.year, a->birth_date.month, a->birth_date.day, a->name);
}

bool ApplicantsComparator(const Applicant* a, const Applicant* b) {
    if (a->points == b->points) {
        return StudentTieWithoutPoints(&a->student) < StudentTieWithoutPoints(&b->student);
    } else {
        return a->points > b->points;
    }
}

bool StudentsComparator(const Student* a, const Student* b) {
    return StudentTieWithoutPoints(a) > StudentTieWithoutPoints(b);
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    AdmissionTable table_of_students_in_university;
    std::unordered_map<std::string_view, size_t> universities_occupancy;
    for (const auto& current_university : universities) {
        universities_occupancy[current_university.name] = current_university.max_students;
    }
    std::vector<const Applicant*> students(applicants.size());
    for (size_t i = 0; i < applicants.size(); ++i) {
        students[i] = &applicants[i];
    }
    std::sort(students.begin(), students.end(), ApplicantsComparator);
    for (const Applicant* current_student : students) {
        for (const std::string_view current_wish : current_student->wish_list) {
            if (auto it = universities_occupancy.find(current_wish);
                it != universities_occupancy.end() && it->second != 0) {
                table_of_students_in_university[std::string{current_wish}].push_back(&current_student->student);
                --it->second;
                break;
            }
        }
    }
    for (auto& [_, i] : table_of_students_in_university) {
        sort(i.begin(), i.end(), StudentsComparator);
    }
    return table_of_students_in_university;
}
