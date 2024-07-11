#define __INTERNAL
#include "avltree.h"
struct node{
    struct node* left;
    void *dataptr;
    int balance;
    struct node* right;
};

struct AVL_TREE{
    int count;
    NODE* root;
    int (*compare)(void* argu1, void* argu2);
};

AVL_TREE* AVL_Create(int (*compare)(void* argu1, void* argu2)){
    AVL_TREE* tree;

    tree = malloc(sizeof(AVL_TREE));

    if(tree)
    {
        tree->compare = compare;
        tree->count   = 0;
        tree->root    = NULL;
    }
    return tree;
}



bool AVL_Insert(AVL_TREE* tree, void* data){
    NODE* newptr;
    bool forTaller;

    newptr = malloc(sizeof(NODE));
    
    if(!newptr)
        return false;

    newptr->balance = EH;
    newptr->right   = NULL;
    newptr->left    = NULL;
    newptr->dataptr = data;

    tree->root = _insert(tree, tree->root, newptr, &forTaller);

    (tree->count)++;
    return true;
}


//internal insert function

NODE* _insert(AVL_TREE* tree, NODE* root, NODE* newptr, bool *taller){
    if(!root)
    {
        root    = newptr;
        *taller = true;
        return root;
    }

    if(tree->compare(newptr->dataptr, root->dataptr) < 0){
        root->left = _insert(tree, root->left, newptr, taller);

        if(*taller){
            switch(root->balance){
                case LH:
                    root = insLeftBal(root, taller);
                    break;
                case EH:
                    root->balance = EH;
                    break;
                case RH:
                    root->balance = EH;
                    *taller = false;
                    break;
            }
            return root;
        } // new < node
    }
    else{
        root->right = _insert(tree, root->right, newptr, taller);
        if(*taller){
            switch(root->balance){
                case LH:
                    root->balance = EH;
                    *taller = false;
                    break;
                case EH:
                    root->balance = EH;
                    break;
                case RH:
                    root = insRightBal(root, taller);
                    break;
            }
            return root;
        }
    }
    return root;
}


NODE* insLeftBal(NODE* root, bool *taller){
    NODE* rightTree;
    NODE* leftTree;

    leftTree = root->left;

    switch(leftTree->balance){
        case LH:
            root->balance     = EH;
            leftTree->balance = EH;
            root    = rotateRight(root);
            *taller = false;
            break;
        case EH: // this is an error
            printf("\n\aError in insLeftBal\n");
            exit(100);
        case RH:
            rightTree = leftTree->right;
            switch(rightTree->balance){
                case LH:
                    root->balance     = RH;
                    leftTree->balance = EH;
                    break;
                case EH:
                    root->balance     = EH;
                    leftTree->balance = LH;
                    break;
                case RH:
                    root->balance     = EH;
                    leftTree->balance = LH;
                    break;
            } // switch right tree
            rightTree->balance = EH;

            // left rotate
            root->left = rotateLeft(leftTree);

            //right rotate
            root    = rotateRight(root);
            *taller = false;
    } // switch
    return root;
}

NODE* insRightBal(NODE* root, bool *taller){
    NODE* righttree;
    NODE* lefttree;

    righttree = root->right;

    switch(righttree->balance){
        case LH:
            lefttree = righttree->left;
            switch(lefttree->balance){
                case LH:
                    root->balance = EH;
                    righttree->balance = RH;
                    break;;
                case EH:
                    root->balance = EH;
                    righttree->balance = LH;
                    break;
                case RH:
                    root->balance = LH;
                    righttree->balance= EH;
                    break;
            }
            lefttree->balance = EH;
            root->right = rotateRight(righttree);
            root = rotateLeft(root);
            *taller = false;
            break;
        case EH:
            printf("\n\aError in insRightBal\n");
            exit(100);
        case RH:
            root->balance = EH;
            righttree->balance = EH;

            root = rotateLeft(root);
            *taller = false;
            break;
    }
    return root;
}

NODE* rotateLeft(NODE* root){
    NODE* tempPtr;

    tempPtr     = root->right;
    root->right = tempPtr->left;
    
    tempPtr->left = root;

    return tempPtr;
}

NODE* rotateRight(NODE* root){
    NODE* tempPtr;

    tempPtr = root->left;
    root->left = tempPtr->right;
    tempPtr->right = root;

    return tempPtr;
}








bool AVL_Delete(AVL_TREE* tree, void* data)
{
    bool shorter;
    bool success;
    NODE* newnode;

    newnode = _delete(tree, tree->root, data, &shorter, &success);

    if(success){
        tree->root = newnode;
        (tree->count)--;
        return true;
    }
    return false;
}


NODE* _delete(AVL_TREE* tree, NODE* root, void* data, bool *shorter, bool *success)
{
    NODE* delptr;
    NODE* exchptr;
    NODE* newroot;

    if(!root){
        *shorter = false;
        *success = false;
        return NULL;
    }

    if(tree->compare(data, root->dataptr) < 0)
    {
        root->left = _delete(tree, root->left, data, shorter, success);

        if(*shorter)
            root = dltRightBal(root, shorter);
    }
    else if(tree->compare(data, root->dataptr) > 0){
        root->right = _delete(tree, root->right, data, shorter, success);
        if(*shorter)
            root = dltLeftBal(root, shorter);
    }
    else{
        delptr = root;
        if(!root->right){
            newroot = root->left;
            *success= true;
            *shorter= true;
            free(delptr);
            return newroot;
        }
        else{
            if(!root->left){
                newroot = root->right;
                *success= true;
                *shorter= true;
                return newroot;
            }
            else{
                exchptr = root->left;

                while(exchptr->right){
                    exchptr = exchptr->right;
                }
                root->dataptr = exchptr->dataptr;
                root->left    = _delete(tree, root->left, exchptr->dataptr, shorter, success);

                if(*shorter){
                    root = dltRightBal(root, shorter);

                }
            } // else
        }
    } // else equal node

    return root;
}


NODE* dltRightBal(NODE* root, bool* shorter){
    NODE* rightTree;
    NODE* leftTree;
    switch(root->balance){
        case LH:
            root->balance = EH;
            break;
        case EH:
            root->balance = RH;
            *shorter = false;
            break;
        case RH:
            rightTree = root->right;
            if(rightTree->balance == LH){
                leftTree = rightTree->left;
                switch(leftTree->balance){
                    case LH:
                        rightTree->balance = RH;
                        root->balance = EH;
                        break;
                    case EH:
                        root->balance = EH;
                        rightTree->balance = EH;
                        break;
                    case RH:
                        root->balance = LH;
                        rightTree->balance = EH;
                        break;
                } // switch
                leftTree->balance = EH;

                root->right = rotateRight(rightTree);
                root = rotateLeft(root);
            } // righttree == LH
            else{
                switch(rightTree->balance){
                    case LH:
                    case RH:
                        root->balance = EH;
                        rightTree->balance = EH;
                        break;
                    case EH:
                        root->balance = RH;
                        rightTree->balance = LH;
                        *shorter = false;
                        break;
                } // switch righttree->balance

                root = rotateLeft(root);
            }// else
    } //switch
    return root;
}

NODE* dltLeftBal(NODE* root, bool *shorter){
    NODE* lefttreeptr;
    NODE* righttreeptr;
    switch(root->balance){
        case LH:
            lefttreeptr = root->left;
            if(lefttreeptr->balance == RH){
                righttreeptr = lefttreeptr->right;
                switch(righttreeptr->balance){
                    case LH:
                        root->balance = RH;
                        lefttreeptr->balance = EH;
                        break;
                    case EH:
                        root->balance = EH;
                        lefttreeptr->balance = EH;
                        break;
                    case RH:
                        lefttreeptr->balance = LH;
                        root->balance = EH;
                        break;
                } // switch

                root->left = rotateLeft(lefttreeptr);
                root = rotateRight(root);
            } // lefttree == RH

            else{
                switch(lefttreeptr->balance){
                    case LH:
                    case RH:
                        root->balance = EH;
                        lefttreeptr->balance = EH;
                        break;
                    case EH:
                        root->balance = LH;
                        lefttreeptr->balance = RH;
                        *shorter = false;
                        break;
                } // switch left->balance

                root = rotateRight(root);
            } // else
    } // switch

    return root;
}


void* AVL_Retrieve(AVL_TREE* tree, void* data){
    if(tree->root){
        return _retrieve(tree, data, tree->root);
    }
    else{
        return NULL;
    }
}


void* _retrieve(AVL_TREE* tree, void* data, NODE* root){
    
    if(root){
        if(tree->compare(data, root->dataptr) < 0)
            return _retrieve(tree, data, root->left);
        
        else if(tree->compare(data, root->dataptr) > 0)
            return _retrieve(tree, data, root->right);
        
        else
            return root->dataptr;
    } 
    else
        return NULL;
}



void AVL_Traverse(AVL_TREE* tree, void (*process)(void* dataptr)){
    _traversal(tree->root, process);
    return;
}

void _traversal(NODE* root, void (*process)(void* data)){
    if(root){
        _traversal(root->left, process);
        process(root->dataptr);
        _traversal(root->right, process);
    }
   return;
}


AVL_TREE* AVL_Destroy(AVL_TREE *tree){
    if(tree){
        _destroy(tree->root);
        free(tree);
    }
    return NULL;
}


void _destroy(NODE* root){
    if(root){
        _destroy(root->left);
        free(root->dataptr);
        _destroy(root->right);
        free(root);
    }
    return;
}

