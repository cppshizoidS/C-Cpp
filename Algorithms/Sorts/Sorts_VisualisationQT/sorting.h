#ifndef SORTING_H
#define SORTING_H

#include <QObject>
#include <QThread>
#include <QMutex>

const int ANIM_SPEED_STEP = 100;
const int MAX_ANIM_DELAY = ANIM_SPEED_STEP * 100;
const int MAX_ANIM_SPEED =  100;

const int DEFAULT_ANIM_SPEED = 99;
const int DEFAULT_SIZE= 32;

class Sorting : public QThread {
    Q_OBJECT

private:
    QObject* parent;
    QStringList algorithms = {"Bubble Sort","Insertion Sort","Selection Sort","Merge Sort","Heap Sort","Quicksort","Radix Sort (Base 2)","Radix Sort (Base 10)","Counting Sort","Gnome Sort","Cocktail Sort","Comb Sort","Odd Even Sort"};
    QString algorithm;
    QStringList shuffles = {"Random", "Reverse", "Almost sorted"};
    QString shuffleType;
    QStringList sizelist = {"2","4","8","16","32","64","128","256","512","1024"};
    int size = DEFAULT_SIZE;
    int* arr = nullptr;
    const int MAX_NUMBER = 512;
    int animDelay = ANIM_SPEED_STEP;
    unsigned int num_comparisons = 0;
    unsigned int num_changes = 0;
    int* color_index = nullptr;
    int color_size = 0;

    void swap(int* arr,int size,int i,int j);
    void insert(int* arr,int size,int i);
    void merge(int* arr,int size,int start,int end);
    void heapify(int* arr,int size,int heapsize,int i);

public:
    explicit Sorting(QObject* parent = nullptr);
    QStringList getAlgorithms() {return algorithms;}
    QStringList getShuffles() {return shuffles;}
    QStringList getSizeList() {return sizelist;}

    void run() override;

    void shuffle();

    void setSize(int size) {this->size = size;}
    void setAnimSpeed(int speed) {this->animDelay = MAX_ANIM_DELAY - speed * ANIM_SPEED_STEP;}
    void setAlgorithm(QString option) {algorithm = option;}
    void setShuffle(QString option) {shuffleType = option;}
    void createArray();
    int getDefaultAnimSpeed() {return DEFAULT_ANIM_SPEED;}
    int getMaxAnimSpeed() {return MAX_ANIM_SPEED;}
    unsigned int get_num_comparisons() {return num_comparisons;}
    unsigned int get_num_changed() {return num_changes;}

    void sort_bubble(int* arr,int size);
    void sort_insertion(int* arr,int size);
    void sort_selection(int* arr,int size);
    void sort_merge(int* arr,int size,int start,int end);
    void sort_heap(int* arr,int size);
    void sort_quick(int* arr,int size,int start,int end);
    void sort_radix(int* arr,int size,int base);
    void sort_counting(int* arr,int size,int base,int exp);
    void sort_gnome(int* arr,int size);
    void sort_cocktail(int* arr,int size);
    void sort_comb(int* arr,int size);
    void sort_oddeven(int* arr,int size);

    void visualize(int* arr,int size_arr,int* color_index,int size_color);

signals:
    void changed(int* arr,int size_arr,int* index,int size_color);
    void done();
};

#endif // SORTING_H
