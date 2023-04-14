#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
using namespace std;

class ListItem {
public:
    int value;
    ListItem* next;

    ListItem(int value) {
        this->value = value;
        this->next = nullptr;
    }
};

class ThreadSafeList {
public:
    ListItem* head;
    mutex list_mutex;

    ThreadSafeList() {
        this->head = nullptr;
    }

    void add_item(int value) {
        unique_lock<mutex> lock(list_mutex);
        ListItem* new_item = new ListItem(value);

        if (head == nullptr || head->value > value) {
            new_item->next = head;
            head = new_item;
        } else {
            ListItem* current = head;
            while (current->next != nullptr && current->next->value < value) {
                current = current->next;
            }
            new_item->next = current->next;
            current->next = new_item;
        }
    }

    bool remove_item(int value) {
        unique_lock<mutex> lock(list_mutex);
        if (head == nullptr) return false;

        if (head->value == value) {
            ListItem* to_delete = head;
            head = head->next;
            delete to_delete;
            return true;
        }

        ListItem* current = head;
        while (current->next != nullptr && current->next->value != value) {
            current = current->next;
        }

        if (current->next != nullptr) {
            ListItem* to_delete = current->next;
            current->next = to_delete->next;
            delete to_delete;
            return true;
        }

        return false;
    }

    bool contains(int value) {
        unique_lock<mutex> lock(list_mutex);
        ListItem* current = head;
        while (current != nullptr) {
            if (current->value == value) return true;
            current = current->next;
        }
        return false;
    }
};

mutex counter_mutex;
int counter = 0;

void worker_task(ThreadSafeList& list, vector<int>& items) {
    for (int value : items) {
        list.add_item(value);
        list.remove_item(value);
        {
            unique_lock<mutex> lock(counter_mutex);
            counter++;
        }
    }
}

int main() {
    ThreadSafeList list;
    int total_items = 500000;
    int items_per_worker = total_items / 4;

    vector<int> values(total_items);
    for (int i = 0; i < total_items; ++i) {
        values[i] = i + 1;
    }

    // Shuffle the items
    random_shuffle(values.begin(), values.end());

    vector<vector<int>> items_per_workers(4, vector<int>(items_per_worker));
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < items_per_worker; ++j) {
            items_per_workers[i][j] = values[i * items_per_worker + j];
        }
    }

    vector<thread> workers;
    for (int i = 0; i < 4; ++i) {
        workers.push_back(thread(worker_task, ref(list), ref(items_per_workers[i])));
    }

    for (auto& worker : workers) {
        worker.join();
    }

    // Print number of items and counter
    cout << "Number of items: " << total_items << endl;
    cout << "Counter: " << counter << endl;

    return 0;
}
