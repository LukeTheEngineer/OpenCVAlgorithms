#include<iostream>

void ShowLoading(int total) {
    int progress = 0;
    int last_progress = -1;
    std::cout << "Progress: [          ]  0%";
    while (progress < total) {
        int current_progress = progress * 10 / total;
        if (current_progress > last_progress) {
            last_progress = current_progress;
            std::cout << "\rProgress: [";
            for (int i = 0; i < 10; i++) {
                if (i <= current_progress) {
                    std::cout << "#";
                } else {
                    std::cout << " ";
                }
            }
            std::cout << "]  " << current_progress * 10 << "%";
        }
        // Simulate some work being done
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        progress++;
    }
    std::cout << std::endl;
}
