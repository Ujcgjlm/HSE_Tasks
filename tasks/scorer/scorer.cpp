#include <unordered_map>
#include "scorer.h"

struct State {
    bool is_accept = false;
    bool has_merge = false;
    time_t accept_time = 0;
    time_t merge_time = 0;
};

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    std::unordered_map<std::string, std::unordered_map<std::string, State>> event_table;
    ScoreTable score_table;
    for (auto& event : events) {
        if (event.time <= score_time) {
            auto& current_task = event_table[event.student_name][event.task_name];
            switch (event.event_type) {
                case (EventType::CheckFailed):
                    if (current_task.accept_time <= event.time) {
                        current_task.is_accept = false;
                        current_task.accept_time = event.time;
                    }
                    break;
                case (EventType::CheckSuccess):
                    if (current_task.accept_time < event.time) {
                        current_task.is_accept = true;
                        current_task.accept_time = event.time;
                    }
                    break;
                case (EventType::MergeRequestClosed):
                    if (current_task.accept_time <= event.time) {
                        current_task.has_merge = false;
                        current_task.merge_time = event.time;
                    }
                    break;
                case (EventType::MergeRequestOpen):
                    if (current_task.accept_time < event.time) {
                        current_task.has_merge = true;
                        current_task.merge_time = event.time;
                    }
                    break;
            }
        }
    }
    for (auto& [student_name, tasks] : event_table) {
        for (auto& [task, state] : tasks) {
            if (state.is_accept && !state.has_merge) {
                score_table[student_name].insert(task);
            }
        }
    }
    return score_table;
}
