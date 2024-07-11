#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_WORDS 10
#define MAX_LEN   10
typedef struct{
    int front;
    int rear;
    int arr[MAX_LEN];
}queue;

void init_queue(queue *queue){
    queue->front = -1;
    queue->rear  = -1;
}

void enq(queue* queue, int index){
    if(queue->front == -1){
        queue->front = 0;
    }
    if(queue->rear == MAX_LEN - 1){
        return;
    }
    queue->arr[++queue->rear] = index;
}


bool isempty(queue* q){
    return q->front == -1;
}
int deq(queue* q){
    if(isempty(q)){
        return -1;
    }
    int v = q->arr[q->front];
    if(q->front >= q->rear){
        q->rear = q->rear = -1;
    }
    else{
        q->front++;
    }
    return v;
}

typedef struct node{
    int index;
    struct node* next;
}node;

typedef struct{
    node* verts[MAX_LEN];
    int size;
}Graph;


void init_graph(Graph* graph){
    graph->size = 0;
    for(int i = 0; i < MAX_LEN; i++){
        graph->verts[i] = NULL;
    }
}

void addedge(Graph* graph, int src, int dest){
    node* newnode = (node*)malloc(sizeof(node));
    newnode->index= dest;

    newnode->next = graph->verts[src];
    graph->verts[src] = newnode;
}

bool is_one_letter_diff(const char* str1, const char* str2){
    int n1 = strlen(str1);
    int n2 = strlen(str2);

    int count = 0;
    if(n1 != n2){
        return false;
    }
    for(int i = 0; i < n1; i++){
        if(str1[i] != str2[i]){
            count++;
            if(count > 1){
                return false;
            }
        }
    }
    return count == 1;
}

void build_graph(Graph* graph, char words[MAX_WORDS][MAX_LEN], int wc){
    graph->size = wc;

    for(int i = 0; i < graph->size; i++){
        for(int j = i + 1; j < graph->size; j++){
            if(is_one_letter_diff(words[i], words[j])){
                addedge(graph, i, j);
                addedge(graph, j, i);
            }
        }
    }
}


void bfs(Graph* graph, char words[MAX_LEN][MAX_WORDS], const char* src){
    bool visited[MAX_LEN] = {false};

    queue queue;
    init_queue(&queue);

    int start_index = -1;

    for(int i = 0; i < graph->size; i++){
        if(strcmp(words[i], src)==0){
            start_index = i;
            break;
        }
    }

    if(start_index == -1){
        fprintf(stderr, "\'%s\':not found in the graph.\n", src);
        return;
    }

    enq(&queue, start_index);
    visited[start_index] = true;

    while(!isempty(&queue)){
        int curr = deq(&queue);
        printf("%s", words[curr]);
        node* adj= graph->verts[curr];
        while(adj != NULL){
            int adjindex = adj->index;
            if(!visited[adjindex]){
                visited[adjindex] = true;
                enq(&queue, adjindex);
            }
            adj = adj->next;
        }
        // if(!isempty(&queue)){
        //     printf("->");
        // } 
    }
}

int main(int argc, char** argv){
    char words[MAX_LEN][MAX_LEN] = {"mca", "bca", "mba"};

    int c = 3;

    Graph g;
    init_graph(&g);
    build_graph(&g, words, c);

    bfs(&g, words, argv[1]);
}