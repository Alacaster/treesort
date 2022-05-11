#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#define SIZE 10000
#define TRIALS 1001
#define D printf("%d ", __LINE__);

void treesortint(int *list, size_t size);
int cmp(const void *a, const void *b){if(*(int*)a > *(int*)b) return -5; else if(*(int*)a < *(int*)b) return 5; else return 0;}

int main(){
    int list[SIZE];
    srand(time(NULL));
    double average[TRIALS + 1];
    struct timespec qtime = {0, 0}, q2time = {0, 0};
    /* for(int i = 0; i < SIZE; i++){list[i] = rand()%100;}
        for(int i = 0; i < SIZE; i++){
        printf("%d, ", list[i]);
    }
    putchar('\n');
    for(int i = 0; i < SIZE; i++){
        printf("%d, ", list[i]);
    }
    putchar('\n'); */
    for(int i = 0; i<TRIALS; i++){
        for(int i = 0; i < SIZE; i++){list[i] = rand()%100;}
        clock_gettime(CLOCK_MONOTONIC, &qtime);
        qsort(list, SIZE, sizeof(int), &cmp);
        clock_gettime(CLOCK_MONOTONIC, &q2time);
        average[i] = (q2time.tv_nsec-qtime.tv_nsec)*1e-9 + (q2time.tv_sec-qtime.tv_sec);
    }
    for(int i = 0; i<TRIALS; i++){
        average[TRIALS] += average[i];
    }
    printf("qsort took %.4f ms over 1000 trials of %d array size", average[TRIALS], SIZE);
    for(int i = 0; i<TRIALS; i++){
        for(int i = 0; i < SIZE; i++){list[i] = rand()%100;}
        clock_gettime(CLOCK_MONOTONIC, &qtime);
        treesortint(list, SIZE);
        clock_gettime(CLOCK_MONOTONIC, &q2time);
        average[i] = (q2time.tv_nsec-qtime.tv_nsec)*1e-9 + (q2time.tv_sec-qtime.tv_sec);
    }
    for(int i = 0; i<TRIALS; i++){
        average[TRIALS] += average[i];
    }
    printf("\ntreesort took %.4f ms over 1000 trials of %d array size", average[TRIALS], SIZE);
}



void treesortint(int *list, size_t size){
    typedef struct {
        int value;
        int lnode;
        int rnode;
    } node;
    node *tree = (node*)malloc(sizeof(node)*size); //the tree
    int *path = (int*)malloc(sizeof(int)*size); //how to go backwards in the tree when sorting
    int location = 1; //where we are in the tree
    int depth = 1; //where to put next node and how long path is
    int i = 0;
    tree[0].value = list[0]; //fill root node
    tree[0].rnode = tree[0].lnode = -1;
    //basically the largest number will always be on the rightmost node
    //traverse the nodes and invalidate the largest number over and over
    for(i = 1; i < size; i++){
        location = 0;
        while(1){
            if(list[i] > tree[location].value){
                if(tree[location].rnode != -1){
                    location = tree[location].rnode;
                    continue;
                }else {
                    tree[location].rnode = depth;
                }
            }else {
                if(tree[location].lnode != -1){
                    location = tree[location].lnode;
                    continue;
                }else {
                    tree[location].lnode = depth;
                }
            }
            tree[depth].value = list[i];
            tree[depth].lnode = -1;
            tree[depth].rnode = -1;
            depth++;
            goto exit;
        }
        exit: ;
    }

    location = i = depth = 0; //variable depth is now used differently
    while(1){
        if(tree[location].rnode != -1){ //if rnode is valid
            //printf("\nRight node of node %d was valid and is node %d", location, tree[location].rnode);
            path[depth++] = location; //go to the right node
            //printf("\n\tadded node %d to path history which is %d nodes long. ", location, depth);
            location=tree[location].rnode;
            //printf("\n\t\tWent to the right node!\n");
        }else if(tree[location].lnode != -1){ //if lnode is valid but not rnode the
            //printf("\nThe right node of node %d was not valid but there is a left node which is node %d", location, tree[location].lnode);
            list[i++] = tree[location].value; //go left and take the current node
            if(i >= size) break;
            //printf("\n\tSaved %d in list[%d] from node %d",tree[location].lnode, list[i-1], i - 1, location);
            location=tree[location].lnode;
            //printf("\n\tGoing to left node!\n");
        }else {
            list[i++] = tree[location].value;
            if(i >= size) break;
            //printf("\n~~~~~end of branch at node %d. saved %d in list[%d], going back to node %d", location, list[i-1], i - 1, path[depth-1]);
            location = path[--depth];
            tree[location].rnode = -1;
            //printf("\n\tInvalidating the right node of node %d cause we already went there!\n", location);
        }
    }
    free(tree);
    free(path);

    //putchar('\n');
    //for(int i = 0; i < SIZE; i++){
    //    printf("%d, ", list[i]);
    //}
}