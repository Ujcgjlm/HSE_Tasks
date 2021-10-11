#include <algorithm>
#include <tuple>
#include "sort_students.h"

auto DateStudentTuple(const Student& a) {
    return std::tie(a.birth_date.year, a.birth_date.month, a.birth_date.day, a.last_name, a.name);
}

auto NameStudentTuple(const Student& a) {
    return std::tie(a.last_name, a.name, a.birth_date.year, a.birth_date.month, a.birth_date.day);
}

void SortStudents(std::vector<Student>& students, SortKind sort_kind) {
    std::sort(students.begin(), students.end(),
              [&sort_kind](const Student& a, const Student& b){
                  if(sort_kind == SortKind::Date){
                      return DateStudentTuple(a) < DateStudentTuple(b);
                  } else {
                      return NameStudentTuple(a) < NameStudentTuple(b);
                  }
              }
    );
}
