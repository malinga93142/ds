#include <avltree.h>
#include <ctype.h>
#include <string.h>

bool iscommanded_file = false;
char filename[25];

typedef struct{
    char word[51];
    int count;
}DATA;

typedef AVL_TREE avltree;
void buildList(avltree*  wordlist);
void insertWord(avltree* words);
void deleteword(avltree* words);
void printList(avltree* wordlist);
void printWord(void *aWord);
bool getWord(DATA* aWord, FILE* fpWords);
int compareWords(void *arguptr, void* listPtr);



void buildList(avltree* wordlist){

    FILE* fpwords;
    bool success;
    DATA* aWord;
    DATA newWord;
    if(iscommanded_file == false){
        printf("Enter file name\n");
        scanf("%24s", filename);
    }
    fpwords = fopen(filename, "r");
    if(!fpwords){
        printf("-%s could not be opened.\a\n", filename);
        printf("Please verify name and try again.\n");
        exit(100);
    }
    while(getWord(&newWord, fpwords)){
        aWord = AVL_Retrieve(wordlist, &(newWord));
        if(aWord){
            (aWord->count)++;
        }
        else{
            aWord = (DATA*)malloc(sizeof(DATA));
            if(!aWord){
                printf("Error 120 in buildList\n");
                exit(120);
            }
            *aWord = newWord;
            aWord->count = 1;
            success = AVL_Insert(wordlist, aWord);
            if(!success){
                printf("Error 121 in buildList\n");
                // free(aWord);
                exit(121);
            } // if overflow test
        } // else
    } // while

    // free(aWord);
    printf("END AVL TREE\n");
    return;
}

bool getWord(DATA* word, FILE* fpwords){
    char strIn[51];
    int ioResult;
    int lastChar;

    ioResult = fscanf(fpwords, "%50s", strIn);
    if(ioResult != 1)
        return false;
    
    strcpy(word->word, strIn);
    lastChar = strlen(word->word) - 1;
    if(ispunct(word->word[lastChar]))
        word->word[lastChar] = '\0';
    return true;
}

int compareWords(void* argptr, void* listptr){
    DATA arguvalue;
    DATA listvalue;
    arguvalue = *(DATA*)argptr;
    listvalue = *(DATA*)listptr;

    return (strcmp(arguvalue.word, listvalue.word));
}


void printList(avltree* wordlist){
    printf("\nWords found in list\n");
    AVL_Traverse(wordlist, printWord);
    printf("\nEnd of word list\n");
}

void printWord(void* data){
    printf("%-30s %3d\n", ((DATA*)data)->word, ((DATA*)data)->count);
    return;
}
int main(int argc, char* argv[]){

    if(argc != 2){
        iscommanded_file = false;
    }
    else{
        iscommanded_file = true;
        strcpy(filename, argv[1]);
    }
    avltree *wordlist;

    printf("Start count words in document\n");

    wordlist = AVL_Create(compareWords);

    buildList(wordlist);
    printList(wordlist);

    AVL_Destroy(wordlist);
    return 0;
}


