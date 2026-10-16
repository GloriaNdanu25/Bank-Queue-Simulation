#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

struct Customer {
int id;
int interarrivalTime;
int arrivalTime;
int serviceTime;
int serviceStartTime;
int serviceEndTime;
int queueWaitTime;
int systemTime;
};

int getRandomNumber(int min, int max, std::mt19937& rng) {
std::uniform_int_distribution<int> dist(min, max);
return dist(rng);
}

int main() {
int totalCustomers = 100;
std::mt19937 rng(42); // Seed for reproducible results

std::vector<Customer> customers(totalCustomers);

int cumulativeArrival = 0;
int totalWaitTime = 0;
int totalServiceTime = 0;
int totalSystemTime = 0;
int waitingCustomers = 0;

for (int i = 0; i < totalCustomers; ++i) {
customers[i].id = i + 1;

// Customer 1 arrives at time 0
customers[i].interarrivalTime = (i == 0) ? 0 : getRandomNumber(1, 5, rng);
cumulativeArrival += customers[i].interarrivalTime;
customers[i].arrivalTime = cumulativeArrival;

customers[i].serviceTime = getRandomNumber(1, 6, rng);

if (i == 0) {
customers[i].serviceStartTime = customers[i].arrivalTime;
} else {
customers[i].serviceStartTime = std::max(customers[i].arrivalTime, customers[i - 1].serviceEndTime);
}

customers[i].queueWaitTime = customers[i].serviceStartTime - customers[i].arrivalTime;
customers[i].serviceEndTime = customers[i].serviceStartTime + customers[i].serviceTime;
customers[i].systemTime = customers[i].queueWaitTime + customers[i].serviceTime;

totalWaitTime += customers[i].queueWaitTime;
totalServiceTime += customers[i].serviceTime;
totalSystemTime += customers[i].systemTime;
if (customers[i].queueWaitTime > 0) {
waitingCustomers++;
}
}

// Print Simulation Table
std::cout << std::left
<< std::setw(6) << "Cust#"
<< std::setw(6) << "IAT"
<< std::setw(10) << "Arrival"
<< std::setw(10) << "Service"
<< std::setw(12) << "Start Time"
<< std::setw(10) << "End Time"
<< std::setw(10) << "Wait Time"
<< std::setw(12) << "System Time" << "\n";
std::cout << std::string(76, '-') << "\n";

for (const auto& c : customers) {
std::cout << std::left
<< std::setw(6) << c.id
<< std::setw(6) << c.interarrivalTime
<< std::setw(10) << c.arrivalTime
<< std::setw(10) << c.serviceTime
<< std::setw(12) << c.serviceStartTime
<< std::setw(10) << c.serviceEndTime
<< std::setw(10) << c.queueWaitTime
<< std::setw(12) << c.systemTime << "\n";
}

int totalSimulationTime = customers[totalCustomers - 1].serviceEndTime;

// Queue Performance Statistics
std::cout << "\n================ QUEUE STATISTICS ================\n";
std::cout << std::fixed << std::setprecision(2);
std::cout << "Total Customers Simulated: " << totalCustomers << "\n";
std::cout << "Average Queue Waiting Time: " << (double)totalWaitTime / totalCustomers << " mins\n";
std::cout << "Average Service Time: " << (double)totalServiceTime / totalCustomers << " mins\n";
std::cout << "Average Time in System: " << (double)totalSystemTime / totalCustomers << " mins\n";
std::cout << "Probability of Waiting: " << ((double)waitingCustomers / totalCustomers) * 100.0 << "%\n";
std::cout << "Server Utilization: " << ((double)totalServiceTime / totalSimulationTime) * 100.0 << "%\n";

return 0;
}