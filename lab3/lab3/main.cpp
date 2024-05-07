#include <mpi.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

void create_matrix(int size, vector<int>& m) {
    for (auto& val : m) {
        val = rand() % 500;
    }
}

void read_matrix(vector<int>& matrix, string file_name) {
    ifstream data(file_name);
    for (auto& val : matrix) {
        data >> val;
    }
}

void mul_matrix(const vector<int>& matrix_1, const vector<int>& matrix_2, vector<int>& matrix_result, int size, int row_start, int row_end) {
    for (int i = row_start; i < row_end; i++) {
        for (int j = 0; j < size; j++) {
            matrix_result[i * size + j] = 0;
            for (int k = 0; k < size; k++) {
                matrix_result[i * size + j] += matrix_1[i * size + k] * matrix_2[k * size + j];
            }
        }
    }
}

int main() {
    MPI_Init(NULL, NULL);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    srand(static_cast<unsigned int>(time(nullptr)) + world_rank);
    for (int i = 0; i < 10; i++)
    {
        int SIZE[9] = { 10, 50, 100, 500, 600, 700, 800, 900, 1000 };
        for (int i = 0; i < 9; i++)
        {
            cout << "размер - " << SIZE[i] << endl;
            vector<int> matrix_1(SIZE[i] * SIZE[i]);
            vector<int> matrix_2(SIZE[i] * SIZE[i]);
            vector<int> matrix_result(SIZE[i] * SIZE[i]);

            if (world_rank == 0) {
                create_matrix(SIZE[i], matrix_1);
                create_matrix(SIZE[i], matrix_2);
            }

            MPI_Bcast(matrix_1.data(), SIZE[i] * SIZE[i], MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(matrix_2.data(), SIZE[i] * SIZE[i], MPI_INT, 0, MPI_COMM_WORLD);

            int local_size = SIZE[i] / world_size;
            int row_start = world_rank * local_size;
            int row_end = (world_rank + 1) * local_size;

            auto start = chrono::high_resolution_clock::now();
            mul_matrix(matrix_1, matrix_2, matrix_result, SIZE[i], row_start, row_end);
            auto end = chrono::high_resolution_clock::now();

            MPI_Gather(&matrix_result[row_start * SIZE[i]], local_size * SIZE[i], MPI_INT, matrix_result.data(), local_size * SIZE[i], MPI_INT, 0, MPI_COMM_WORLD);

            if (world_rank == 0) {
                double duration = chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0;
                cout << duration << ", ";
            }
            cout << endl << endl;
        }
    }

    MPI_Finalize();
}