#include <iostream>

#define MAX_QUEUE_SIZE 100
struct node{
    int data;
    struct node* right;
    struct node* left;
    node(int data) : data(data), right(nullptr), left(nullptr) {}
};

node* insert(node* root, int data){
    if(root == nullptr)
        return new node(data);
    node *curr = root;
    node *parent = nullptr;
    node* newnode = new node(data);

    while(curr != nullptr){
        parent = curr;
        if(data < curr->data)
            curr = curr->left;
        else
            curr = curr->right;
    }
    if(data < parent->data){
        parent->left = newnode;
    }
    else{
        parent->right = newnode;
    }
    return root;
}

void inorder(node* root){
    node *temp = root;
    node* stack[100];
    int top = -1;

    while(top != -1 || temp != nullptr){
        while(temp != nullptr){
            if(top >= 99){
                return;
            }
            stack[++top] = temp;
            temp = temp->left;
        }
        temp = stack[top--];
        std::cout << temp->data << " ";

        temp = temp->right;
    }
    std::cout << std::endl;
}
int height(node* root){
    if(root == nullptr){
        return 0;
    }
    else{
        int l = height(root->left);
        int r = height(root->right);
        if(l >= r){
            return 1 + l;
        }
        else{
            return 1 + r;
        }
    }
}
int getbalance(node* root){
    return height(root->left) - height(root->right);
}

void level(node* root){
    if(root == nullptr)
        return;

    int front = 0, rear = 0;
    node* queue[MAX_QUEUE_SIZE];

    queue[rear] = root;

    rear = (rear + 1) % MAX_QUEUE_SIZE;
    while(front != rear){
        int levelsize = (rear >= front) ? rear - front: MAX_QUEUE_SIZE - front + rear;
        for(int i = 0; i < levelsize; ++i){
            node *temp = queue[front];
            front = (front + 1) % MAX_QUEUE_SIZE;
            std::cout << temp->data << " ";


            if(temp->left){
                queue[rear] = temp->left;
                rear = (rear + 1) % MAX_QUEUE_SIZE;
            }
            if(temp->right){
                queue[rear] = temp->right;
                rear = (rear + 1) % MAX_QUEUE_SIZE;
            }
        }
        std::cout << std::endl;
    }
}

int main(){
    node* root;
    root = insert(root, 10);
    root = insert(root, 1);
    root = insert(root, 11);
    root = insert(root, 21);
    root = insert(root, 17);

    std::cout << height(root) << std::endl;
    std::cout << getbalance(root) << std::endl;
    level(root);
}
