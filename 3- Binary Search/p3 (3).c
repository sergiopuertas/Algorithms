#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>
double microseconds() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 ) return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}
struct node {
    int elem;
    int num_repetitions;
    struct node *left, *right;
};
typedef struct node * position;
typedef struct node * tree;

static struct node *createnode(int e) {
    struct node *p = malloc(sizeof(struct node));
    if (p == NULL) {
        printf("out of memory\n"); exit(EXIT_FAILURE);
    }
    p->elem = e;
    p->num_repetitions = 1;
    p->left = NULL;
    p->right = NULL;
    return p;
}
tree LeftChild(tree tree){
    return tree->left;
}
tree RightChild(tree tree){
    return tree->right;
}
int Element (position p){
    return p->elem;
}
int Numrep(position p){
    return p->num_repetitions;
}
bool isEmptyTree (tree tree){
    return (tree == NULL);
}

tree insert(int e, tree a){
	if (isEmptyTree(a))
		return createnode(e);
	else if (e < Element(a))
		a->left = insert(e, LeftChild(a));
	else if (e > Element(a))
		a->right = insert(e, RightChild(a));
	else
		a->num_repetitions++;
	return a;
}

tree createTree(tree tree){
    return tree=NULL;

}

position findKey(int key, tree tree){
    if (isEmptyTree(tree)){
        return NULL;
    }
    else if (key == Element(tree)) {
        return tree;
    }
    else if (key < Element(tree))
        return findKey(key,  LeftChild(tree));
    else
        return findKey(key, RightChild(tree));
}

tree deleteTree(tree tree){
    if(tree == NULL)
        return NULL;
    deleteTree( LeftChild(tree));
    deleteTree(RightChild(tree));
    printf("Deleting Node : %d\n", Element(tree));
    free(tree);
    tree = NULL;
    return tree;
}

int height(struct node* node)
{
    if (node == NULL)
        return -1;
    else {
        int lDepth = height( LeftChild(node));
        int rDepth = height(RightChild(node));
        if (lDepth > rDepth)
            return (lDepth + 1);
        else
            return (rDepth + 1);
    }
}
void visualize(tree tree){
    if (tree != NULL) {
        printf("(");
        visualize( LeftChild(tree));
        printf(" %d ", Element(tree));
        visualize(RightChild(tree));
        printf(")");
    }
}
void printFind(int num,tree tree){
    if(isEmptyTree(tree) || findKey(num,tree) == NULL){
        printf("Searching for element %d and found nothing\n",num);
    }
    else printf("Searching for element %d and finding %d, number of times = %d\n",num, Element(findKey(num,tree)),Numrep(findKey(num,tree)));

}
void printTree(tree tree){
    if (!isEmptyTree(tree)){
        printf("Tree: ");
        visualize(tree);
    }
    else printf("Empty BST\n");
}
void testImplementation(tree tree){
    printTree(tree);
    printf("Height: %d\n", height(tree));
    tree = insert(1, tree);
    printf("Inserting element 1\n");
    for(int i = 1; i< 4;i++){
        tree = insert(i, tree);
            printf("Inserting element %d\n", i);
        tree = insert(i+4, tree);
            printf("Inserting element %d\n",i+4);
    }
    printTree(tree);
    printf("\nheight: %d\n", height(tree));
    for(int i = 0; i< 8;i++){
        printFind(i, tree);
    }
    tree = deleteTree(tree);
    printTree(tree);
    printf("Height: %d\n", height(tree));
}

double PrintBounds(bool ins, double t,int n){
    double x,y,z;
    if (ins){
        x = t / (pow (n, 0.6)*log(n));
        y = t /(pow(n,1.07)*log(n));
        z = t / (pow(n, 1.5)*log(n));
    }
    else {//search
        x = t / (pow(n, 0.6)*log(n));
        y = t / (pow(n,1.07)*log(n));
        z = t / (pow(n, 1.6)*log(n));
    }
    printf("%12d %15.4f%15.8f%15.8f %15.8f\n", n, t, x, y, z);
    return y;
}
void PrintInsertElements(bool ins,tree tr){
    double t1, t2,t,c=0;
    if (ins) printf("Insertion of n elements:\n        n:         t(n):         t(n)/f(n):     t(n)/g(n):      t(n)/h(n):\n");
    else printf("Search of n elements:\n        n:         t(n):         t(n)/f(n):     t(n)/g(n):      t(n)/h(n):\n");
    for(int i=8000;i<=256000;i=i*2){
        if(ins) tr=createTree(tr);
        t1 = microseconds();
        for (int w=0; w<i;w++){
            if (ins) tr = insert(rand() % i,tr);
            else findKey(rand() % i,tr);
        }
        t2 = microseconds();
        t = t2-t1;
        c += PrintBounds(ins, t,i);
    }
    printf("Const = %.5f\n",c/6);
    if(!ins) return;
    PrintInsertElements(false,tr);
}


int main (){
    tree tree = NULL;
    tree = createTree(tree);
    testImplementation(tree);
    printf("\n");
    PrintInsertElements(true,tree);
    return 0;
}
