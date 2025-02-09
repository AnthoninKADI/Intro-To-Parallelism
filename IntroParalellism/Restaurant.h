// Create an Actor-Model application replicating a restaurant. Actors are :
//
// Customer : they orders meals, composed of a mix of 3 ingredient among 10. They also eat the meal then exit the restaurant.
//
// Waiter : they take the orders to trigger the preparation and bring the completed meals.
//
// Cooker : they prepare each of the ingredient, which take time.
//
// Chief : they mix the 3 chosen ingredient to compose a meal, which takes time.
//
// To simplify the problem, you can consider there are only one actor of each sort and set a queue for each different operation.
//
// Then try to manage multiple actors with multiple queues and add a graphical representation of the restaurant.

#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <random>
#include <chrono>

class Customer
{
public:
    void order_meal(std::queue<int>& order_queue)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 9);

        std::vector<int> meal;
        for (int i = 0; i < 3; ++i)
        {
            meal.push_back(dis(gen));
        }

        for (int ingredient : meal)
        {
            order_queue.push(ingredient);
        }
        std::cout << "Meal with ingredients: ";
        for (int ingredient : meal)
        {
            std::cout << ingredient << " ";
        }
        std::cout << "\n";
    }
};

class Waiter
{
public:
    void take_order(std::queue<int>& order_queue, std::queue<int>& preparation_queue, std::mutex& mtx, std::condition_variable& cv)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
        std::lock_guard<std::mutex> lock(mtx);
        while (!order_queue.empty())
        {
            int ingredient = order_queue.front();
            preparation_queue.push(ingredient);
            order_queue.pop();
        }
        cv.notify_all();
        std::cout << "Ingredients in preparation.\n";
    }
};

class Cooker
{
public:
    void prepare_ingredient(std::queue<int>& preparation_queue, std::mutex& mtx, std::condition_variable& cv)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2)); 
        std::lock_guard<std::mutex> lock(mtx);
        while (!preparation_queue.empty())
        {
            int ingredient = preparation_queue.front();
            preparation_queue.pop();
            std::cout << "Prepared ingredient: " << ingredient << "\n";
        }
    }
};

class Chief
{
public:
    void mix_ingredients(std::queue<int>& preparation_queue, std::mutex& mtx, std::condition_variable& cv)
    {
        std::this_thread::sleep_for(std::chrono::seconds(3)); 
        std::lock_guard<std::mutex> lock(mtx);
        if (preparation_queue.size() == 3)
        {
            std::cout << "Mixed the ingredients\n";
        }
    }
};

void restaurant_simulation()
{
    std::queue<int> order_queue;
    std::queue<int> preparation_queue;
    std::mutex mtx;
    std::condition_variable cv;

    Customer customer;
    Waiter waiter;
    Cooker cooker;
    Chief chief;

    std::thread customer_thread(&Customer::order_meal,
        &customer,
        std::ref(order_queue));
    std::thread waiter_thread(&Waiter::take_order,
        &waiter,
        std::ref(order_queue),
        std::ref(preparation_queue),
        std::ref(mtx),
        std::ref(cv));
    std::thread cooker_thread(&Cooker::prepare_ingredient,
        &cooker,
        std::ref(preparation_queue),
        std::ref(mtx),
        std::ref(cv));
    std::thread chief_thread(&Chief::mix_ingredients,
        &chief,
        std::ref(preparation_queue),
        std::ref(mtx),
        std::ref(cv));

    customer_thread.join();
    waiter_thread.join();
    cooker_thread.join();
    chief_thread.join();

    std::cout << "Meal ready\n";
}