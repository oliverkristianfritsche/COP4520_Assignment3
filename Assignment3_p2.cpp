#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <random>
#include <condition_variable>

// Time scale: 1 ms of real time = 1 minute of simulated time
using namespace std;
class recorder {
private:
    
    vector<int> lt;
    vector<int> temps;
    vector<int> ht;
    int temp_dif_time_interval;
    int interval_start;
    mutex mtx;
    condition_variable cv; // condition variable to notify all threads

public:
    recorder(){
        this->interval_start = 0;
        this->temp_dif_time_interval = 0;
    }

    void record(int temperature) {
        unique_lock<mutex> lock(mtx);
        temps.push_back(temperature);
        lock.unlock();
        cv.notify_all();
    }

    void report() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return temps.size() == 8 * 60; });
        
        for (int i = 0; i < 8 * 60; i += 8) {
            int min_temp = *std::min_element(temps.begin() + i, temps.begin() + i + 8);
            int max_temp = *std::max_element(temps.begin() + i, temps.begin() + i + 8);
            int temp_diff = max_temp - min_temp;

            if (temp_diff > temp_dif_time_interval) {
                temp_dif_time_interval = temp_diff;
                interval_start = i / 8;
            }
        }

        std::partial_sort(temps.begin(), temps.begin() + 5, temps.end(), std::greater<int>());
        ht.assign(temps.begin(), temps.begin() + 5);

        std::partial_sort(temps.begin(), temps.begin() + 5, temps.end());
        lt.assign(temps.begin(), temps.begin() + 5);

        lock.unlock();
    }

    void print_report() {
        unique_lock<mutex> lock(mtx);
        cout << "Top 5 highest temps: ";
        for (int temp : ht) {
            cout << temp << " ";
        }
        cout << endl;

        cout << "Top 5 lowest temps: ";
        for (int temp : lt) {
            cout << temp << " ";
        }
        cout << endl;

        cout << "Largest temperature difference observed in the 10-minute interval: " << temp_dif_time_interval << endl;
        cout << "Interval start time: " << interval_start << " minutes" << endl;
        lock.unlock();
    }
};

void temperature_sensor(recorder& recorder, int sensor_id) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(-100,70);

    for (int i = 0; i < 60; ++i) {
        int temperature = distr(gen);
        recorder.record(temperature);
        this_thread::sleep_for(chrono::milliseconds(1)); // Sleep for 1 ms (simulating 1 minute)
    }
}

int main() {
    recorder recorder;
    vector<thread> sensors;

    for (int i = 0; i < 8; ++i) {
        sensors.push_back(thread(temperature_sensor, ref(recorder), i));
    }

    thread report_generator(&recorder::report, &recorder);

    for (auto &sensor : sensors) {
        sensor.join();
    }

    report_generator.join();
    recorder.print_report();

    return 0;
}