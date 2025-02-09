#include <iostream>
#include <vector>
#include "MatrixProduct.h"
#include "SumTable.h" 
#include "Restaurant.h" 

void print_matrix(const std::vector<std::vector<int>>& matrix)
{
    for (const auto& row : matrix)
    {
        for (int val : row) std::cout << val << " ";
        std::cout << "\n";
    }
}

void print_vector(const std::vector<int>& vec)
{
    for (int val : vec) std::cout << val << " ";
    std::cout << "\n";
}

int main() {
    std::string exo = "matrix_product";
    // 1 . matrix_product
    // 2 . sum_table
    // 3 . restaurant

    // 01.ThreadManagement
    if (exo == "matrix_product") {
        std::vector<std::vector<int>> A = {{1, 2}, {3, 4}};
        std::vector<std::vector<int>> B = {{5, 6}, {7, 8}};

        std::cout << "Produit Sequentiel :\n";
        auto P_sequential = matrix_multiplication(A, B);
        print_matrix(P_sequential);

        std::cout << "\nProduit Parallele :\n";
        auto P_parallel = parallel_multiplication(A, B);
        print_matrix(P_parallel);
    }

    // 02.ShareData
    if (exo == "sum_table")
        {
        
        std::vector<int> table = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        
        std::cout << "Séquentielle : " << sequential_sum(table) << "\n";
        
        int num_threads = 2; 
        std::cout << "Threads Locaux : " << thread_local_sum_main(table, num_threads) << "\n";
        
        global_sum = 0;
        std::cout << "Mutex Global : " << global_sum_main(table, num_threads) << "\n";
        
        std::cout << "\npairs avec mutex :\n";
        print_even_numbers_main();
    }
    
   // 03.SynchronizeOperations
   if (exo == "restaurant")
    {
        std::cout << "Starting the restaurant simulation...\n";
        restaurant_simulation();  
    }

    return 0;
}
