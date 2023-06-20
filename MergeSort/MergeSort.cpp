#include <iostream>
#include <chrono>
#include <future>

#define SIZE 2000000
#define LIMIT 10000
#define STOP_THREADING 500000

bool threads = false;

void merge(int* arr, int l, int m, int r){
    int nl = m - l + 1;
    int nr = r - m;

    // создаем временные массивы
    int* left = new int[nl];
    int* right = new int[nr];

    // копируем данные во временные массивы
    for (int i = 0; i < nl; i++)
        left[i] = arr[l + i];

    for (int j = 0; j < nr; j++)
        right[j] = arr[m + 1 + j];

    int i = 0, j = 0;
    int k = l;  // начало левой части

    while (i < nl && j < nr) {
        // записываем минимальные элементы обратно во входной массив
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        }
        else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
    // записываем оставшиеся элементы левой части
    while (i < nl) {
        arr[k] = left[i];
        i++;
        k++;
    }
    // записываем оставшиеся элементы правой части
    while (j < nr) {
        arr[k] = right[j];
        j++;
        k++;
    }
    delete[] left;
    delete[] right;
}

void mergeSort(int* arr, int l, int r){
    if (l >= r){
        return;
    }
    int m = (l + r - 1) / 2;

    if (threads && (m - l) > STOP_THREADING){
        std::future<void> fut = std::async(std::launch::async, [&]() {mergeSort(arr, l, m); });
        mergeSort(arr, m + 1, r);
        fut.get();
    }
    else{
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
    }
    merge(arr, l, m, r);
}

int main(){
    int* rand_arr = new int[SIZE];
    std::srand(time(nullptr));

    for (long i = 0; i < SIZE; i++){
        rand_arr[i] = rand() % LIMIT;
    }

    // многопоточный запуск сортировки
    threads = true;
    std::cout << "multiply threads sorting for array dimension N = " << SIZE << " elements" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();    // сохраняем время начала работы алгоритма
    mergeSort(rand_arr, 0, SIZE - 1);                          // запускаем сортировку
    auto finish = std::chrono::high_resolution_clock::now();   // сохраняем время конца работы алгоритма
    std::chrono::duration<double>  elapsed = finish - start;

    std::cout << "Elapsed time: " << elapsed.count() << " sec" << std::endl << std::endl; // выводим продолжительность работы

    for (long i = 0; i < SIZE; i++){
        rand_arr[i] = rand() % LIMIT;
    }

    // однопоточный запуск сортировки
    threads = false;
    std::cout << "single-threaded sorting by array dimension N = " << SIZE << " elements" << std::endl;
    start = std::chrono::high_resolution_clock::now();    // время начала работы алгоритма
    mergeSort(rand_arr, 0, SIZE - 1);                     // запускаем сортировку
    finish = std::chrono::high_resolution_clock::now();   // время конца работы алгоритма
    elapsed = finish - start;

    std::cout << "Elapsed time: " << elapsed.count() << " sec" << std::endl; // выводим продолжительность работы

    delete[] rand_arr;
    return 0;
}