#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

void combine (int*, int , int , int );
void sort (int [], int );

enum choice {
    ASC,
    DESC,
    RANDOM
};
char* enumtochar(enum choice choice){
    switch (choice){
        case RANDOM:
            return "random";
        case ASC:
            return "ascending";
        case DESC:
            return  "descending";
    }
}
double microseconds() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

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
void initarray(int arr[],int n,enum choice choice){
    switch (choice){
        case ASC:
            ascending_init(arr,n);
            break;
        case DESC:
            descending_init(arr,n);
            break;
        case RANDOM:
            random_init(arr,n);
            break;
    }
}
void test_sort(enum choice choice){
    printf("Sorting with %s initialization\n", enumtochar(choice));
    int arr[20];
    initarray(arr,20,choice);
    for (int i=0; i<20; i++){
        printf("%d ", arr[i]);
    }
    printf("\nSorted?  0\n");
    printf("Sorting...\n");
    sort(arr,20);
    for (int i=0; i<20; i++){
        printf("%d ", arr[i]);
    }
    printf("\n\n");
}
void combine (int v[], int left, int right, int center) {
    int a = left;
    int b = center+1;
    int c = 0;
    int aux[(right - left)];
    while (a <= center && b <= right) {
        if (v[a] <= v[b]) {
            aux[c] = v[a];
            a++;
        }
        else {
            aux[c] = v[b];
            b++;
        }
        c++;
    }
    while (a <= center) {
        aux[c] = v[a];
        a++;
        c++;
    }
    while (b <= right) {
        aux[c] = v[b];
        b++;
        c++;
    }
    c = 0;
    for (a = left; a <= right; a++) {
        v[a] = aux[c];
        c++;
    }
}
void sortAux (int v[], int left, int right){
    int center;
    if((right - left)>0) {
        center = ((left +  right)/2);
        sortAux(v,left, center);
        sortAux(v,center+1,right);
        combine (v,left,right,center);
    }
}
void sort (int v[], int n){
    sortAux (v,0,n-1);
}
double PrintSortBounds(double t,int n,enum choice choice){
    double x,y,z;
    switch (choice){
        case ASC:
            x = t / (pow(n, 0.45)*log(n));
            y = t / (pow(n,0.98)*log(n));
            z = t / (pow(n, 1.3)*log(n));
            break;
        case DESC:
            x = t / (pow(n, 0.45)*log(n));
            y = t / (pow(n,0.98)*log(n));
            z = t / (pow(n, 1.3)*log(n));
            break;
        case RANDOM:
            x = t / (pow(n, 0.45)*log(n));
            y = t / (pow(n,0.98)*log(n));
            z = t / (pow(n, 1.3)*log(n));
            break;
    }
    printf("%12d %15.4f%15.8f%15.8f %15.8f\n", n, t, x, y, z);
    return y;
}
void PrintTimesSort(enum choice choice){
    printf("Sort with %s initialization\n", enumtochar(choice));
    double t1, t2, t ,c = 0;
    printf("             n:          t(n):      t(n)/f(n):    t(n)/g(n):    t(n)/h(n):\n");
    for(int n=500; n <= 32000; n= n * 2)
    {
        double t3=0,t4=0;
        int arr[n];
        initarray(arr,n,choice);
        t1 = microseconds();
        sort(arr, n);
        t2 = microseconds();
        t = t2-t1;
        if(t<500){
            t1 = microseconds();
            for(int i = 0; i<1000;i++){
                t3 = microseconds();
                initarray(arr,n,choice);
                t4 = microseconds();
                sort(arr, n);
            }
            t2 = microseconds();
            t = (t2-t1-(t4-t3))/1000;
            printf("*");
        }
        else printf(" ");
        c += PrintSortBounds(t,n,choice);
    }
    printf("Const = %.5f\n",c/7);
    printf("* = Measured an average of 1000 times\n");
}
int main() {
    init_seed();
    test_sort(ASC);
    test_sort(DESC);
    test_sort(RANDOM);

    PrintTimesSort(ASC);
    PrintTimesSort(DESC);
    PrintTimesSort(RANDOM);


    return 0;
}
