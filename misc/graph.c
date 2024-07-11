#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_WORDS 1000
#define MAX_LEN   20


typedef struct node{
    int index;
    struct node* next;
} node;

typedef struct{
    node* head[MAX_WORDS];
    int size;
}graph;


void init_graph(graph* g){
    g->size = 0;
    for(int i = 0; i < MAX_WORDS; i++){
        g->head[i] = NULL;
    }
}


void add_edge(graph* graph, int src, int dest){
    node* newnode = (node*)malloc(sizeof(node));
    newnode->index = dest;
    newnode->next = graph->head[src];
    graph->head[src] = newnode;
}


bool is_one_letter(const char* word1, const char* word2){
    int len1 = strlen(word1);
    int len2 = strlen(word2);

    if(len1 != len2){
        return false;
    }
    int cnt = 0;

    for(int i = 0; i < len1; i++){
        if(word1[i] != word2[i]){
            cnt++;
            if(cnt > 1){
                return false;
            }
        }
    }
    return cnt == 1;
}


void build_graph(graph* Graph, char words[MAX_WORDS][MAX_LEN], int word_count){

    Graph->size = word_count;

    for(int i = 0; i < word_count; i++){
        for(int j = i + 1; j < word_count; j++){
            if(is_one_letter(words[i], words[j])){
                add_edge(Graph, i, j);
                add_edge(Graph, j, i);
            }
        }
    }
}

typedef struct{
    int items[MAX_WORDS];

    int front, rear;
}qptr;

void init_q(qptr* q){
    q->front = -1;
    q->rear  = -1;
}

bool isempty(qptr *q){
    return q->front == -1;
}

void enqueue(qptr* q, int data){
    if(q->rear == MAX_WORDS - 1){
        return;
    }
    if(q->front == -1){
        q->front = 0;
    }
    q->items[++q->rear] = data;
}

int deque(qptr* q){
    if(isempty(q)){
        return -1;
    }
    int item = q->items[q->front];

    if(q->front == q->rear){
        q->front = q->rear = -1;
    }
    else{
        q->front++;
    }
    return item;
}

void bfs(graph* Graph, char words[MAX_WORDS][MAX_LEN], const char* start_word, const char* end_word){
    bool visited[MAX_WORDS] = {false};
    int parent[MAX_WORDS];

    qptr q;
    init_q(&q);

    int start_index = -1, end_index = -1;

    for(int i = 0; i < Graph->size; i++){

        if(strcmp(words[i], start_word) == 0){
            start_index = i;
        }
        if(strcmp(words[i], end_word) == 0){
            end_index = i;
        }
    }
    if(start_index == -1 || end_index == -1){
        printf("start or end word not found in the word list\n");
        return;
    }


    enqueue(&q, start_index);
    visited[start_index] = true;

    parent[start_index] = -1;
    bool found = false;
    while(!isempty(&q)){
        int current = deque(&q);
        node* adjlist = Graph->head[current];
        while(adjlist != NULL){
            int adj_index = adjlist->index;
            if(!visited[adj_index]){
                visited[adj_index] = true;
                parent[adj_index]  = current;
                enqueue(&q, adj_index);

                if(adj_index == end_index){
                    found = true;
                    break;
                }
            }
            adjlist = adjlist->next;
        }
        if(found) break;
    }
    

    if(!visited[end_index]){
        printf("no word ladder found.\n");
        return;
    }

    int path[MAX_WORDS];
    int path_len = 0;
    for(int at = end_index; at != -1; at = parent[at]){
        path[path_len++] = at;
    }

    for(int i = path_len - 1; i >= 0; i--){
        printf("%s\n", words[path[i]]);
    }
}


int main(){
    char words[MAX_WORDS][MAX_LEN] = {"hit", "hot", "dot", "dog", "lot", "log", "cog"};

    int w_count = 7;

    graph G;
    init_graph(&G);
    build_graph(&G, words, w_count);

    const char* start_word = "hit";
    const char* end_word   = "cog";

    bfs(&G, words, start_word, end_word);
    return 0;

}
