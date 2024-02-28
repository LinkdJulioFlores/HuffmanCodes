#include <stdio.h>
#include <iostream>
using namespace std;

struct Node {
    char data; // Character
    unsigned freq; // Frequency of the character
    Node *left, *right; // Left and right child

    Node(char data, unsigned freq) {
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

Node* heap[100]; // Heap now stores Node pointers
int heap_size = 0;

// Modify swap function to work with Node pointers
void swap(int idx1, int idx2) {
    Node* tmp = heap[idx1];
    heap[idx1] = heap[idx2];
    heap[idx2] = tmp;
}

// Modify heap_insert and heapify to work with Node pointers and their frequencies
// ...

// Function to insert a new node into the heap
void heap_insert(Node* node) {
    int idx = heap_size;
    while (idx > 0 && node->freq < heap[(idx - 1) / 2]->freq) {
        heap[idx] = heap[(idx - 1) / 2];
        idx = (idx - 1) / 2;
    }
    heap[idx] = node;
    heap_size++;
}

// Function to maintain the heap property
void heapify(int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap_size && heap[left]->freq < heap[smallest]->freq)
        smallest = left;
    if (right < heap_size && heap[right]->freq < heap[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swap(idx, smallest);
        heapify(smallest);
    }
}

// Function to remove and return the node with the smallest frequency
Node* heap_delete() {
    if (heap_size == 0) return NULL;

    Node* root = heap[0];
    heap[0] = heap[heap_size - 1];
    heap_size--;
    heapify(0);

    return root;
}

// Function to build the Huffman Tree
Node* buildHuffmanTree(char data[], int freq[], int size) {
    Node *left, *right, *top;

    // Step 1: Create leaf nodes for each character and insert them into the heap
    for (int i = 0; i < size; ++i)
        heap_insert(new Node(data[i], freq[i]));

    // Step 2: Build the Huffman tree
    while (heap_size != 1) {
        // Step 2.a: Remove the two nodes of the highest priority (lowest frequency) from the heap
        left = heap_delete();
        right = heap_delete();

        // Step 2.b: Create a new internal node with these two nodes as children and with frequency equal to the sum of their frequencies. Add this node to the min heap
        top = new Node('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        heap_insert(top);
    }

    // Step 3: The remaining node is the root node and the tree is complete.
    return heap_delete();
}

// Function to print the codes from the root of the Huffman Tree. It uses s to store the current code
void printCodes(Node* root, string str) {
    if (!root) return;

    if (root->data != '$')
        cout << root->data << ": " << str << "\n";

    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}

int main() {
    char arr[] = {  'A', 'B', 'C', 'D', 'E', 'F', 'G',
                    'H', 'I', 'J', 'K', 'M', 'N', 'L',
                    'O', 'P', 'Q', 'R', 'S', 'T', 'U',
                    'V', 'W', 'X', 'Y', 'Z'
                 };
    int freq[] = { 77, 17, 32, 42, 120, 24, 17, 50 ,76, 4, 7, 42, 24,
                   67, 67, 20, 5, 59, 67, 85, 37, 12, 22, 4, 22, 2 
                 };

    int size = sizeof(arr) / sizeof(arr[0]);

    Node* root = buildHuffmanTree(arr, freq, size);

    printCodes(root, "$");

    return 0;
}
