// Write a program that compute the product of two matrices P = A x B. You remark the Pij are independant, which allow you to parallel the
// code.
//
// 1. Implement a sequential version, without threads
//
// 2. Implement a by-value version: each value of the matrix is computed by a thread
#pragma once
#include <vector>
#include <thread>

std::vector<std::vector<int>> matrix_multiplication(const std::vector<std::vector<int>>& matA, const std::vector<std::vector<int>>& matB) {
    int rowsA = matA.size();
    int colsB = matB[0].size();
    int common = matB.size();
    
    std::vector<std::vector<int>> result(rowsA, std::vector<int>(colsB, 0));

    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            for (int k = 0; k < common; ++k) {
                result[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    return result;
}

void calculate_element(const std::vector<std::vector<int>>& matA, const std::vector<std::vector<int>>& matB, std::vector<std::vector<int>>& result, int i, int j) {
    int common = matB.size();
    result[i][j] = 0;
    for (int k = 0; k < common; ++k) {
        result[i][j] += matA[i][k] * matB[k][j];
    }
}

std::vector<std::vector<int>> parallel_multiplication(const std::vector<std::vector<int>>& matA, const std::vector<std::vector<int>>& matB) {
    int rowsA = matA.size(), colsB = matB[0].size();
    std::vector<std::vector<int>> result(rowsA, std::vector<int>(colsB, 0));
    std::vector<std::thread> threads;

    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            threads.emplace_back(calculate_element, std::cref(matA), std::cref(matB), std::ref(result), i, j);
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    return result;
}



