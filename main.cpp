#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

enum ID {
    ComeIn = 1,
    ExplicitOccupyTable = 2,
    Waiting = 3,
    ExplicitComeAway = 4,
    ImplicitComeAway = 11,
    ImplicitOccupyTable =12,
    Error = 13,
};

struct Time {
    uint8_t hours;
    uint8_t minutes;

    bool operator<(const Time& other) const {
        if (hours != other.hours) {
            return hours < other.hours;
        }
        return minutes < other.minutes;
    }

    bool operator>(const Time& other) const {
        if (hours != other.hours) {
            return hours > other.hours;
        }
        return minutes > other.minutes;
    }

    Time& operator+=(const Time& other) {
        hours = hours + other.hours;
        minutes = minutes + other.minutes;
        if (minutes >= 60) {
            ++hours;
            minutes -= 60;
        }
        if (hours >= 24) {
            hours -= 24;
        }
        return *this;
    }

    Time operator-(const Time& other) const {
        if (*this < other) {
            throw std::runtime_error(
                "Wrong operands for time subtraction operator!");
        }

        Time result;
        if (this->minutes < other.minutes) {
            result.minutes = this->minutes + 60 - other.minutes;
            result.hours = this->hours - 1 - other.hours;
        } else {
            result.minutes = this->minutes - other.minutes;
            result.hours = this->hours - other.hours;
        }
        return result;
    }
};

struct Event {
    Event(Time time, ID id, const std::string& client_name, uint32_t table_number)
        : time(time),
          id(id),
          client_name(client_name),
          table_number(table_number) {}
    Event(Time time, ID id, const std::string& error_message)
        : time(time), id(id), error_message(error_message) {}
    Time time;
    ID id;
    std::string client_name;
    uint32_t table_number;      // 0 for events that don't have table property
    std::string error_message;  // empty for all events except with id == Error
};

struct Input {
    bool input_is_correct;
    uint32_t tables_count;
    Time opening_time;
    Time closing_time;
    uint32_t price_per_hour;
    std::vector<Event> incoming_events;
};

struct Table {
    bool occupied = false;
    uint32_t paid_hours = 0;
    Time total_time_occupied;
    Time occupied_start_time;
};

void OccupyTheTable(Table* table, Time time, uint32_t* free_tables_count) {
    table->occupied = true;
    table->occupied_start_time = time;
    --(*free_tables_count);
}

void FreeTheTable(Table* table, Time time, uint32_t* free_tables_count) {
    table->occupied = false;
    Time occupied_time = time - table->occupied_start_time;
    table->total_time_occupied += occupied_time;
    table->paid_hours += occupied_time.minutes == 0 ? occupied_time.hours
                                                    : occupied_time.hours + 1;
    ++(*free_tables_count);
}

std::pair<std::vector<Event>, std::vector<Table>> ProcessEvents(
    const Input& input) {
    std::vector<Event> events;

    std::queue<std::string> waiting_clients;
    std::map<std::string, uint32_t>
        current_clients;  // (client : table they occupy or 0 if the client
                          // doesn't occupy any table)
    std::vector<Table> tables(input.tables_count + 1);
    uint32_t free_tables_count = input.tables_count;

    for (const Event& incoming_event : input.incoming_events) {
        events.push_back(incoming_event);
        switch (incoming_event.id) {
            case ComeIn:
                if (current_clients.contains(incoming_event.client_name)) {
                    events.emplace_back(incoming_event.time, Error, "YouShallNotPass");
                } else if (incoming_event.time < input.opening_time ||
                           incoming_event.time > input.closing_time) {
                    events.emplace_back(incoming_event.time, Error, "NotOpenYet");
                } else {
                    current_clients[incoming_event.client_name] = 0;
                }
                break;

            case ExplicitOccupyTable:
                if (!current_clients.contains(incoming_event.client_name)) {
                    events.emplace_back(incoming_event.time, Error, "ClientUnknown");
                } else if (tables[incoming_event.table_number].occupied) {
                    events.emplace_back(incoming_event.time, Error, "PlaceIsBusy");
                } else {
                    uint32_t previous_table_of_this_client = current_clients[incoming_event.client_name];
                    if (previous_table_of_this_client != 0) {
                        // client moves to another table, so the previous table should be freed
                        FreeTheTable(&tables[previous_table_of_this_client], incoming_event.time, &free_tables_count);
                        current_clients.erase(incoming_event.client_name);
                    }
                    OccupyTheTable(&tables[incoming_event.table_number],
                                   incoming_event.time, &free_tables_count);
                    current_clients[incoming_event.client_name] = incoming_event.table_number;
                }
                break;

            case Waiting:
                if (free_tables_count > 0) {
                    events.emplace_back(incoming_event.time, Error, "ICanWaitNoLonger!");
                } else if (waiting_clients.size() >= input.tables_count) {
                    events.emplace_back(incoming_event.time, ImplicitComeAway,
                                           incoming_event.client_name, 0);
                    current_clients.erase(incoming_event.client_name);
                } else {
                    waiting_clients.push(incoming_event.client_name);
                }
                break;

            case ExplicitComeAway:
                if (!current_clients.contains(incoming_event.client_name)) {
                    events.emplace_back(incoming_event.time, Error, "ClientUnknown");
                } else {
                    uint32_t freed_table =
                        current_clients[incoming_event.client_name];
                    FreeTheTable(&tables[freed_table], incoming_event.time, &free_tables_count);
                    current_clients.erase(incoming_event.client_name);
                    if (!waiting_clients.empty()) {
                        std::string waiting_client = waiting_clients.front();
                        waiting_clients.pop();
                        OccupyTheTable(&tables[freed_table],
                                       incoming_event.time, &free_tables_count);
                        current_clients[waiting_client] = freed_table;
                        events.emplace_back(incoming_event.time, ImplicitOccupyTable, waiting_client, freed_table);
                    }
                }
        }
    }

    for (const auto& [client, table_number] : current_clients) {
        FreeTheTable(&tables[table_number], input.closing_time, &free_tables_count);
        events.emplace_back(input.closing_time, ImplicitComeAway, client, 0);
    }

    return {events, tables};
}

Time ParseTime(std::string time) {
    uint8_t hours = (time[0] - '0') * 10 + (time[1] - '0');
    uint8_t minutes = (time[3] - '0') * 10 + (time[4] - '0');
    return Time{hours, minutes};
}

std::string TimeToString(Time time) {
    std::string result;
    if (time.hours < 10) {
        result += '0';
    }
    result += std::to_string(time.hours) + ':';
    if (time.minutes < 10) {
        result += '0';
    }
    result += std::to_string(time.minutes);
    return result;
}

Input ParseInput(const std::string& input_file_name) {
    std::ifstream fin(input_file_name);
    if (!fin) {
        throw std::runtime_error("Failed to open the file!");
    }

    Input input;
    std::string line;

    try {
        std::getline(fin, line);
        input.tables_count = std::stoi(line);
        std::getline(fin, line);
        input.opening_time = ParseTime(line.substr(0, 5));
        input.closing_time = ParseTime(line.substr(6, 5));
        std::getline(fin, line);
        input.price_per_hour = std::stoi(line);

        char delimiter = ' ';
        while (std::getline(fin, line)) {
            if (line.empty()) {
                continue;
            }
            size_t last_pos = 0;
            size_t next_pos = line.find(delimiter, last_pos);
            Time event_time =
                ParseTime(line.substr(last_pos, next_pos - last_pos));
            last_pos = next_pos + 1;
            next_pos = line.find(delimiter, last_pos);
            ID id = static_cast<ID>(
                std::stoi(line.substr(last_pos, next_pos - last_pos)));
            last_pos = next_pos + 1;
            next_pos = line.find(delimiter, last_pos);
            std::string client_name =
                line.substr(last_pos, next_pos - last_pos);
            uint32_t table_number = 0;
            if (id == ExplicitOccupyTable) {
                last_pos = next_pos + 1;
                table_number =
                    std::stoi(line.substr(last_pos, next_pos - last_pos));
            }
            input.incoming_events.emplace_back(event_time, id, client_name,
                                               table_number);
        }
    } catch (...) {
        std::cout << line << '\n';
        input.input_is_correct = false;
        return input;
    }

    input.input_is_correct = true;
    return input;
}

void PrintEvents(const std::vector<Event>& events) {
    for (const Event& event : events) {
        std::cout << TimeToString(event.time) << ' ' << event.id << ' ';
        if (event.id == Error) {
            std::cout << event.error_message;
        } else {
            std::cout << event.client_name;
            if (event.id == ExplicitOccupyTable || event.id == ImplicitOccupyTable) {
                std::cout << ' ' << event.table_number;
            }
        }
        std::cout << '\n';
    }
}

void PrintTablesInfo(const std::vector<Table>& tables,
                     uint32_t price_per_hour) {
    for (size_t table_number = 1; table_number < tables.size();
         ++table_number) {
        std::cout << table_number << ' '
                  << tables[table_number].paid_hours * price_per_hour << ' '
                  << TimeToString(tables[table_number].total_time_occupied)
                  << '\n';
    }
}

void PrintOutput(const Input& input, const std::vector<Event>& events,
                 const std::vector<Table>& tables) {
    std::cout << TimeToString(input.opening_time) << '\n';
    PrintEvents(events);
    std::cout << TimeToString(input.closing_time) << '\n';
    PrintTablesInfo(tables, input.price_per_hour);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        throw std::runtime_error("No input file specified!");
    }

    Input input = ParseInput(argv[1]);
    if (!input.input_is_correct) {
        return 0;
    }
    auto[events, tables] = ProcessEvents(input);
    PrintOutput(input, events, tables);

    return 0;
}