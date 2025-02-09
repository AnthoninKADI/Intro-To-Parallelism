// Write a program that computes the sum of the element of a table of n elements
//
// 1. Write a sequential version ;
//
// 2. Write a version using thread local variable: the table is divided into m sub-tables, and each thread
// compute the sum into its own local variable and return it ;
//
// 3. Write a version using a global sum variable and mutexes.
// Ordering with a mutex

// Write a program that launch 2 threads : the first one displays even numbers between 0 and 1000, the second
// one displays even numbers between 0 and 1000.
//
// 1. Observe the result of a first naive version of this program ;
//
// 2. Use mutexes to ensure the two threads display the numbers in the right order.

#pragma once
#include <vector>
#include <thread>
#include <iostream>
#include <mutex>

std::mutex mtx;  

int sequential_sum(const std::vector<int>& table) {
    int sum = 0;
    for (int num : table) {
        sum += num;
    }
    return sum;
}

void thread_local_sum(const std::vector<int>& table, int start, int end, int& local_sum) {
    for (int i = start; i < end; ++i) {
        local_sum += table[i];
    }
}

int thread_local_sum_main(const std::vector<int>& table, int num_threads) {
    int table_size = table.size();
    int chunk_size = table_size / num_threads;
    std::vector<std::thread> threads;
    std::vector<int> local_sums(num_threads, 0);

    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1) ? table_size : (i + 1) * chunk_size;
        threads.push_back(std::thread(thread_local_sum, std::ref(table), start, end, std::ref(local_sums[i])));
    }

    for (auto& t : threads) {
        t.join();
    }

    int total_sum = 0;
    for (int sum : local_sums) {
        total_sum += sum;
    }
    return total_sum;
}

int global_sum = 0;

void global_sum_func(const std::vector<int>& table, int start, int end) {
    int local_sum = 0;
    for (int i = start; i < end; ++i) {
        local_sum += table[i];
    }

    std::lock_guard<std::mutex> guard(mtx);
    global_sum += local_sum;
}

int global_sum_main(const std::vector<int>& table, int num_threads) {
    int table_size = table.size();
    int chunk_size = table_size / num_threads;
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1) ? table_size : (i + 1) * chunk_size;
        threads.push_back(std::thread(global_sum_func, std::ref(table), start, end));
    }

    for (auto& t : threads) {
        t.join();
    }

    return global_sum;
}

void print_even_numbers(int id) {
    for (int i = id; i <= 1000; i += 2) {
        std::lock_guard<std::mutex> guard(mtx);
        std::cout << i << "\n";
    }
}

void print_even_numbers_main() {
    std::thread t1(print_even_numbers, 0); 
    std::thread t2(print_even_numbers, 2); 

    t1.join();
    t2.join();
}