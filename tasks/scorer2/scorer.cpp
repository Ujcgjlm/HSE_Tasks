#include <list>
#include "scorer.h"

void Scorer::OnCheckFailed(const StudentName& student_name, const TaskName& task_name) {
    std::string_view task_view{*tasks_name_.insert(task_name).first};
    if (auto table_it = students_table_.find(student_name); table_it != students_table_.end()) {
        if (auto task_it = table_it->second.find(task_view); task_it != table_it->second.end()) {
            table_it->second.erase(task_it);
            if (table_it->second.empty()) {
                students_table_.erase(table_it);
            }
        }
    }
}

void Scorer::OnCheckSuccess(const StudentName& student_name, const TaskName& task_name) {
    std::string_view task_view{*tasks_name_.insert(task_name).first};
    students_table_[student_name][task_view].is_accept = true;
}

void Scorer::OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name) {
    std::string_view task_view{*tasks_name_.insert(task_name).first};
    students_table_[student_name][task_view].has_merge = true;
}

void Scorer::OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name) {
    std::string_view task_view{*tasks_name_.insert(task_name).first};
    students_table_[student_name][task_view].has_merge = false;
}

void Scorer::Reset() {
    students_table_.clear();
    tasks_name_.clear();
}

ScoreTable Scorer::GetScoreTable() const {
    ScoreTable answer;
    for (auto [student, task_map] : students_table_) {
        for (auto [task_name, state] : task_map) {
            if (state.is_accept && !state.has_merge) {
                answer[student].insert(TaskName(task_name));
            }
        }
    }
    return answer;
}
