#include <iostream>
#include <vector>

using namespace std;

int main()
{
  setlocale(LC_ALL, "Russian");
  cout « "введите массив пар(для остановки введите 0 0): " « endl;
  vector < pair < string, string» mas; //вектор с контейнерами

  string ss, s;
  while ((s != "0") && (cin » s » ss)) { // ввод массива значений
  mas.push_back(make_pair(s, ss));
}
mas.pop_back(); // удаляет "0 0" из массива значений

for (pair<string, string> a : mas) { // случай когда пункт назначения = пункту отправки
if (a.first == a.second) {
cout « "Обратитесь к специалисту";
return 0;
  }
}

if (mas.size() == 1) { // если только 1 маршрут, то выводит конечную
cout « mas[0].second;
}

string city;
int count = 0, count1 = 0;

for (int i = 0; i < mas.size(); i++) { // поиск длинейшего маршрута
int j = 0;

string first = mas[i].first;
while (j < mas.size() - 1) {
  if (j == i && first == mas[i].first) {
  j++;
  continue;
}

if (first == mas[j].second) {
  first = mas[j].first;
  count1++;
  j = 0;
  }
j++;
}

if (count1 > count) {
  city = mas[i].second;
  count = count1;
}
count1 = 0;
}
cout « city;
}
