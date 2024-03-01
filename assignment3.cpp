#include <cstdlib>
#include<vector>
#include <stdio.h>
#include <iostream>
#include<algorithm>
#include <string>
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

// This structure is what we need for our sorted data
struct HuffmanCode{
    char data;
    string code;
    unsigned freq;

    HuffmanCode(char data, string code, unsigned freq)
    {
        this->data = data;
        this->code = code;
        this->freq = freq;
    }

    // operator overloading for sort() comparison
    // https://www.geeksforgeeks.org/operator-overloading-cpp/
    bool operator<(const HuffmanCode &huffy) const 
    {
        return data < huffy.data;
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

// Function to insert a new node into the heap
void heap_insert(Node* node) {
    int idx = heap_size;
    // (idx-2) / 2 is the used to find the parent of an index
    // (2*idx) + 1 left child
    // (2*idx) + 2 right child
    // Move parent nodes down if child is less than parent
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
        smallest = left; // index of node
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
    // heap_size - 1 is because of zero index
    heap[0] = heap[heap_size - 1];
    heap_size--;
    heapify(0);

    return root;
}

// Function to build the Huffman Tree
Node* buildHuffmanTree(char data[], int freq[], int size) {
    Node *left, *right, *top; // top = Parent

    // Create leaf nodes for each character and insert them into the heap
    for (int i = 0; i < size; ++i)
        heap_insert(new Node(data[i], freq[i]));

    // Build the Huffman tree
    while (heap_size != 1) {
        // Remove the two nodes of the highest priority (lowest frequency) from the heap
        left = heap_delete();
        right = heap_delete();

        // $ means that the node is internal node
        // this is used for when we print out the codes
        // if the $ is not found then we hit a character for display
        top = new Node('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        heap_insert(top);
    }

    // The remaining node is the root node and the tree is complete.
    return heap_delete();
}

// Function to print the codes from the root of the Huffman Tree. It uses s to store the current code
void storeCodes(Node* root, string str, vector<HuffmanCode> &huffy) {
    if (!root) return;

    // Store data into a map for later use
    if (root->data != '$')
        huffy.push_back(HuffmanCode(root->data, str, root->freq));


    // Recursively call the function to atttain the codes
    storeCodes(root->left, str + "0", huffy);
    storeCodes(root->right, str + "1", huffy);
}

void printSortedCodes(Node *root)
{
    // Dynamic array
    vector<HuffmanCode> huffy;

    // Store the built huffman tree codes into an array
    storeCodes(root, "", huffy);

    // algorithm library and got the idea from leetcode questions
    // We will use the struct operator overloading for sort to work
    // https://stackoverflow.com/questions/36056448/how-overload-operator-for-sort-method-for-objects
    sort(huffy.begin(), huffy.end());

    cout << "Letter      ";
    cout << "Frequency   ";
    cout << "Code        ";
    cout << "Length      ";
    cout << "Freq X Len" << endl;

    // To answer last question
    unsigned weightedPathLength = 0;
    for(const auto &h : huffy)
    {
        unsigned freqXLen = h.freq * h.code.length();
        cout << h.data                    << "           ";
        cout << h.freq                    << "          ";
        cout << h.code                    << "          ";
        cout << h.code.length()           << "            ";
        cout << freqXLen << "            ";
        cout << endl;
        weightedPathLength += freqXLen;
    }

    cout << "The weighted minimum path length is: " << weightedPathLength << endl;

}

int main() {
    char arr[] = {  'A', 'B', 'C', 'D', 'E', 'F', 'G',
                    'H', 'I', 'J', 'K', 'M', 'N', 'L',
                    'O', 'P', 'Q', 'R', 'S', 'T', 'U',
                    'V', 'W', 'X', 'Y', 'Z'
                 };
    int freq[] = { 77, 17, 32, 42, 120, 24, 17, 50 ,76, 4, 7, 42, 24,
                    67, 67, 20, 5, 59, 67, 85, 37, 12, 22, 4, 22, 2 };

    int size = sizeof(arr) / sizeof(arr[0]);

    Node* root = buildHuffmanTree(arr, freq, size);

    printSortedCodes(root);

    // This assignment was really hard
    return 0;
}
