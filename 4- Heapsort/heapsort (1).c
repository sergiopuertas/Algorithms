#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <stdbool.h>
#define SZ 512000



double microseconds() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

typedef struct {
    int vector[SZ];
    int last;
} heap;

void init_seed() {
    srand(time(NULL));
}
void random_init(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}
void ascending_init(int v [], int n) {
    int i;
    for (i=0; i < n; i++)
        v[i] = i;
}
void descending_init (int v [], int n){
    for (int i=n-1; i>=0; i--){
        v[i]=n-i-1;
    }
}
void initarray(int arr[],int n,int i){
    switch (i){
        case 3:
            ascending_init(arr,n);
            break;
        case 2:
            descending_init(arr,n);
            break;
        case 1:
            random_init(arr,n);
            break;
    }
}
void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void percolate_down(heap *heap1, int N, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < N && heap1->vector[left] > heap1->vector[largest])
        largest = left;
    if (right < N && heap1->vector[right] > heap1->vector[largest])
        largest = right;

    if (largest != i) {
        swap(&heap1->vector[i], &heap1->vector[largest]);

        percolate_down(heap1, N, largest);
    }
}

int remove_max(heap* heap,int i) {

    swap(&heap->vector[0], &heap->vector[heap->last-1]);
    heap->last--;
    percolate_down(heap, heap->last, 0);
    return heap->vector[heap->last];

}

void printHeap(int arr[], int N)
{
    for (int i = 0; i < N; i++)
        printf("%d ", arr[i]);
    printf("\n");
}


void create_heap(const int array[], int size, heap * heap){
    heap->last=size;
    for(int i=0;i<size;i++){
        heap->vector[i]=array[i];
    }
    for (int i = size / 2 - 1; i >= 0; i--)
        percolate_down(heap, heap->last, i);
}

void heapSort(int arr[], int N)
{
    heap heap;
    create_heap(arr,N,&heap);
    for (int i = N - 1; i >= 0; i--) {

        arr[i]=remove_max(&heap,i);
    }
}

double PrintCreateBounds(double t,int n){
    double x,y,z;
    x = t / pow (n, 0.8);
    y = t / n;
    z = t / pow(n, 1.25);
    printf("%12d %15.4f%15.8f%15.8f %15.8f\n", n, t, x, y, z);
    return y;
}

double repeat(int arr[], int n, int i,heap* heap, double t,int times,bool create){
    double t3,t4;
    if (times == 0) return t;
    initarray(arr,n,i);

    t3 = microseconds();
    create_heap(arr,n,heap);
    t4 = microseconds();

    if(!create){
        heapSort(arr,n);
        double t2 = microseconds();
        return repeat (arr,n,i,heap,t+t2-t3,times-1,false);
    }
    return repeat (arr,n,3,heap,t+t4-t3,times-1,true);
}
void testCreateComplexity(heap* heap){
    double t1, t2,t,c=0;
    printf("Creating a heap of  of n elements:\n          n:           t(n):       t(n)/f(n):     t(n)/g(n):      t(n)/h(n):\n");
    for(int i=1000;i<=512000;i=i*2){
        int arr[i];
        ascending_init(arr,i);
        t1 = microseconds();
        create_heap(arr,i,heap);
        t2 = microseconds();
        t = t2-t1;
        if(t<500) {
            t=repeat(arr,i,3,heap,0,1000,true)/1000;
            printf("* ");
        }
        else printf("  ");
        c += PrintCreateBounds( t,i);
    }
    printf("Const = %.5f\n",c/10);
}
double PrintSortBounds(double t,int n,int i){
    double x,y,z;
    switch (i){
        case 3:
        x = t / (pow(n, 0.85)*log(n));
        y = t / (pow(n,0.98)*log(n));
        z = t / (pow(n, 1.2)*log(n));
            break;
        case 2:
        x = t / (pow(n, 0.85)*log(n));
        y = t / (pow(n,0.98)*log(n));
        z = t / (pow(n, 1.2)*log(n));
            break;
        case 1:
        x = t / (pow(n, 0.85)*log(n));
        y = t / (n*log(n));
        z = t / (pow(n, 1.2)*log(n));
            break;
    }
    printf("%12d %15.4f%15.8f%15.8f %15.8f\n", n, t, x, y, z);
    return y;
}

void testOrderComplexity(heap* heap,int i){
    double t2,t3,t,c=0;
    if(i==3) printf("\nAscendingly sorted array:\n");
    else if(i==2) printf("\nDescendingly sorted array:\n");
    else printf("\nRandomly sorted array:\n");
    printf("\nSorting a heap of  of n elements:\n          n:          t(n):        t(n)/f(n):     t(n)/g(n):      t(n)/h(n)\n");
    for(int n=1000;n<=512000;n=n*2){
        int arr[n];
        initarray(arr,n,i);
        t3 = microseconds();
        create_heap(arr,n,heap);
        heapSort(heap->vector,n);
        t2 = microseconds();
        t = t2-t3;
        if(t<500) {
            t=repeat(arr,n,i,heap,0,1000,false)/1000;
            printf("* ");
        }
        else printf("  ");
        c += PrintSortBounds(t,n,i);
    }
    printf("Const = %.5f\n",c/10);
    if(i==1) return;
    testOrderComplexity(heap,i-1);
}
int main()
{
    init_seed();
    int arr[] = { 12, 11, 13, 5, 6, 7 };
    int N = sizeof(arr) / sizeof(arr[0]);
    heap heap;
    printf("Initial heap: ");
    create_heap(arr,N,&heap);
    printHeap(heap.vector, heap.last);
    printf("Sorted array: ");
    heapSort(arr, N);
    printHeap(arr, heap.last);
    remove_max(&heap,N);
    printf("Deleting root and printing new heap: ");
    printHeap(heap.vector, heap.last);
    printf("\n");

    testCreateComplexity(&heap);
    testOrderComplexity(&heap,3);
}
