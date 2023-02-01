#include <stdio.h>
#include <math.h>
#include <sys/time.h>
double microseconds() {
	struct timeval t;
	if (gettimeofday(&t, NULL) < 0 )
		return 0.0;
	return (t.tv_usec + t.tv_sec * 1000000.0);
}


int fib1(int n){
	if(n<2) return n;
	else return(fib1(n-1) + fib1(n-2));
}
void PrintTimesFib1(){
	int n;
	double t1, t2, t = 0, x, y, z,c = 0;
	printf("	    n:	        t(n):       t(n)/f(n):     t(n)/g(n):     t(n)/h(n):       	     \n");
	for(int k = 1; k <= 5;k++){
		n = pow(2,k);
		t1 = microseconds();			
		fib1(n);
		t2 = microseconds();
		t = t2-t1;
		if(t<500){
			t1 = microseconds();		
			for(int i = 0; i<100000;i++){
				fib1(n);
			}
			t2 = microseconds();
			t = (t2-t1)/100000;
			printf("*");
		}
		else printf(" ");
		x = t / pow(1.1,n);//underestimated bound
		y = t / pow((1+sqrt(5))/2,n);//adjusted bound
		z = t / pow(2, n);//overestimated bound.
		c += y;
		printf("%12d %15.3f%15.6f%15.6f%15.6f\n", n, t, x, y, z);
	}
	printf("Const = %.4f\n",c/5);
	printf("Measured an average of 100000 times\n\n");
}

int fib2 (int n){
	int i = 1;
	int j = 0;
	for(int k = 1 ; k <= n ;k++){
		j = i + j;
		i = j - i;
	}
	return j;
}

void PrintTimesFib2(){
	double t1, t2, t = 0, x, y, z,c =0;
	printf("	   n:	        t(n):      t(n)/f(n):     t(n)/g(n):     t(n)/h(n):       	     \n");	    
	for(int n = 1000; n <= 10000000; n = n*10){
		t1 = microseconds();			
		fib2(n);
		t2 = microseconds();
		t = t2-t1;
		if(t<500){
			t1 = microseconds();		
			for(int i = 0; i<1000;i++){
				fib2(n);
			}
			t2 = microseconds();
			t = (t2-t1)/1000;
			printf("*");
		}
		else printf(" ");
		x = t / pow(n, 0.8);//underestimated bound
		y = t / n;//adjusted bound
		z = t / (n*log(n));//overestimated bound.
		c += y;
		printf("%12d %15.3f%15.6f%15.6f%15.6f\n", n, t, x, y, z);
		}
		printf("Const = %.4f\n",c/5);
		printf("Measured an average of 1000 times\n\n");

}

int fib3 (int n){
	int i = 1, j = 0, k = 0, h = 1, t = 0;
	while(n > 0){
		if (n % 2 != 0){
			t = j*h;
			j = i*h + j*k + t;
			i = i*k + t;
		}
		t = h*h;
		h = 2*k*h + t;
		k = k*k + t;
		n = n/2;
	}
	return j;
}	

void PrintTimesFib3(){
	double t1, t2,t = 0, x, y, z,c=0;
	printf("	   n:	        t(n):      t(n)/f(n):     t(n)/g(n):     t(n)/h(n):       	     \n");
	for(int n = 1000; n <= 10000000; n = n*10)
	{
		t1 = microseconds();			
		fib3(n);
		t2 = microseconds();
		t = t2-t1;
		if(t<500){
			t1 = microseconds();		
			for(int i = 0; i<100000;i++){
				fib3(n);
			}
			t2 = microseconds();
			t = (t2-t1)/100000;
			printf("*");
		}
		else printf(" ");
		x = t / sqrt(log(n));//underestimated bound
		y = t / log(n);//adjusted bound
		z = t / pow(n, 0.5);//overestimated bound.
		c += y;
		printf("%12d %15.3f%15.6f%15.6f%15.6f\n", n, t, x, y, z);
	}
	printf("Const = %.4f\n",c/5);
	printf("Measured an average of 100000 times\n");
}	

int main() {
	printf("Table of times of the first algorithm (measured in μs)\n");
	PrintTimesFib1();
	
	printf("Table of times second algorithm (measured in μs)\n");
	PrintTimesFib2();
	
	printf("Table of times third algorithm (measured in μs)\n");
	PrintTimesFib3();
	return 0;
}






















