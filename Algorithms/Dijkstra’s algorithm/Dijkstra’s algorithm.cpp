#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    system("chcp 1251 > nul");

    vector<int> parent(5, -1);
    int big_num(10000);
    int matrix[5][5] = { {0,5,12,0,0},
                         {5,0,4,0,1},
                         {12,4,0,2,0},
                         {0,0,2,0,5},
                         {0,1,0,5,0} };

    int pos[5], node[5], min(0), index_min(0);
    for (int i = 0; i < 5; ++i) {
        pos[i] = big_num;
        node[i] = 0;
    }
    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i < 5; ++i) {
            cout << setw(4) << matrix[i][j];
        }
        cout << "\n";
    }
    int x;

    cout << "Write x1";

    cin >> x;
    pos[x - 1] = 0; // start pos
    cout << "\n";
    for (int i = 0; i < 4; ++i) {
        min = big_num;
        for (int j = 0; j < 5; ++j) {
            if (!node[j] && pos[j] < min) {
                min = pos[j];
                index_min = j;
            }
        }
        node[index_min] = true;
        for (int j = 0; j < 5; ++j) {
            if (!node[j] && matrix[index_min][j] > 0 && pos[index_min] != big_num && pos[index_min] + matrix[index_min][j] < pos[j]) {
                pos[j] = pos[index_min] + matrix[index_min][j];
                parent.at(j) = index_min;
            }
        }
    }
    int n(0);
    cout << "\nNumbers of peaks? : ";
    cin >> n;

    vector<int>temp;
    for (int i = n - 1; i != -1; i = parent.at(i))temp.push_back(i + 1);
    reverse(temp.begin(), temp.end());
    for (int i = 0; i < temp.size(); ++i)cout << temp.at(i) << " ";

    cout << "\nWeight : " << pos[n - 1] << "\n";

    cout << endl;
    return 0;
}
