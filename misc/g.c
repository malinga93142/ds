#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_W 10
#define MAX_L 10
#define STRINGIFY(x) #x
#define N_DIFF(str1, str2, n)\
    bool is_##n_word_diff(str1, str2){\
    }

#define N_DIFF(str1, str2, one)

typedef struct node{
    int index;
    struct node* next;
}node_ptr;


typedef struct{
    node_ptr* words[MAX_W];
    int size;
}g_ptr;

void init_graph(g_ptr* g){
    g->size = 0;
    for(int i = 0; i < MAX_W; i++){
        g->words[i] = NULL;
    }
}

void add_edge(g_ptr* g, int src, int dest){
    node_ptr* nodeptr = (node_ptr*)malloc(sizeof(node_ptr));

    nodeptr->index = dest;
    nodeptr->next  = g->words[src];
    g->words[src]  = nodeptr;
}

bool is_two_letter_diff(const char* word1, const char* word2){
    int len1 = strlen(word1);
    int len2 = strlen(word2);

    if(len1 != len2){
        return false;
    }

    int c = 0;
    for(int i = 0; i < len1; i++){
        if(word1[i] != word2[i]){
            c++;
            if(c > 2){
                return false;
            }
        }
    }
    return (c == 2 || c == 1);
}

void build_graph(g_ptr* graph, char words[MAX_W][MAX_L], int wc){
    graph->size = wc;

    for(int i = 0; i < wc; i++){
        for(int j = i + 1; j < wc; j++){
            if(is_two_letter_diff(words[i], words[j])){
                add_edge(graph, i, j);
                add_edge(graph, j, i);
            }
        }
    }
}


typedef struct{
    int front;
    int rear;
    int arr[MAX_W];
}queue_ptr;


void init_q(queue_ptr* q){
    q->front = -1;
    q->rear  = -1;
}

bool isempty(queue_ptr* q){
    return q->front == -1;
}

void enqueue(queue_ptr* q, int data){
    if(q->rear == MAX_W - 1){
        return;
    }
    if(q->front == -1){
        q->front = 0;
    }
    q->arr[++q->rear] = data;
}

int dequeue(queue_ptr* q){
    if(isempty(q)){
        return -1;
    }
    int item = q->arr[q->front];

    if(q->front ==  q->rear){
        q->front = q->rear = -1;
    }
    else{
        q->front++;
    }
    return item;
}

void bfs(g_ptr* graph, char words[MAX_W][MAX_L], const char *start){
    bool visited[MAX_W] = {false};

    queue_ptr queue;
    init_q(&queue);
    int parent[MAX_W] = {0};
    int start_index = -1;

    // this for loop will search for the string
    for(int i = 0; i < graph->size; i++){
        if(strcmp(words[i], start) == 0){
            start_index = i;
            break;
        }
    }


    if(start_index == -1){
        fprintf(stderr, "the word \'%s\' your searching is not in the graph.\n", start);
        return;
    }

    enqueue(&queue, start_index);
    visited[start_index] = true;

    parent[start_index] = -1;
    while(!isempty(&queue)){
        int current = dequeue(&queue);
        printf("%s\n", words[current]);
        node_ptr* adjlist = graph->words[current];
        while(adjlist != NULL){
            int adj_index = adjlist->index;
            if(!visited[adj_index]){
                visited[adj_index] = true;
                parent[adj_index]  = current;
                enqueue(&queue, adj_index);
            }
            adjlist = adjlist->next;
        }
    }

}

int main(int argc, char **argv){
    char words[MAX_W][MAX_L] = {"Malinga", "malinga", "nalinga"};

    int c = 3;
    g_ptr g;
    init_graph(&g);
    build_graph(&g, words, c);

    bfs(&g, words, argv[1]);
    return 0;
}