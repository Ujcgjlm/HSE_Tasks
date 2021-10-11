#include <deque>
#include "tests_checking.h"

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::vector<std::string> answer_queries;
    std::deque<std::string> order;
    for (StudentAction current_student : student_actions) {
        if (current_student.side == Side::Bottom) {
            order.push_back(current_student.name);
        } else {
            order.push_front(current_student.name);
        }
    }
    for (size_t next_query : queries) {
        answer_queries.push_back(order[next_query - 1]);
    }
    return answer_queries;
}
