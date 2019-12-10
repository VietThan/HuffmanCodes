#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <fstream>

// This constant can be avoided by explicitly
// calculating height of Huffman Tree
int MAX_TREE_HT  = 100;


// A Huffman tree node
struct MinHeapNode {

    // One of the input characters
    std::string data;

    // Frequency of the character
    unsigned freq;

    // Left and right child of this node
    struct MinHeapNode *left, *right;
};

// A Min Heap:  Collection of
// min-heap (or Huffman tree) nodes
struct MinHeap {

    // Current size of min heap
    unsigned size;

    // capacity of min heap
    unsigned capacity;

    // Array of minheap node pointers
    struct MinHeapNode** array;
};

struct MinHeap* createMinHeap(unsigned capacity);
struct MinHeapNode* newNode(std::string data, unsigned freq);
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b);
void minHeapify(struct MinHeap* minHeap, int idx);
int isSizeOne(struct MinHeap* minHeap);
struct MinHeapNode* extractMin(struct MinHeap* minHeap);
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode);
void buildMinHeap(struct MinHeap* minHeap);
void printArr(int arr[], int n, std::ofstream &EncodeFile);
int isLeaf(struct MinHeapNode* root);
struct MinHeap* createAndBuildMinHeap(std::string data[], int freq[], int size);
struct MinHeapNode* buildHuffmanTree(std::string data[], int freq[], int size);
void HuffmanCodes(std::string data[], int freq[], int size, std::ofstream &EncodeFile);
void printCodes(struct MinHeapNode* root, int arr[], int top, std::ofstream &EncodeFile);





int main() {

//    char arr[] = {'f', 'e', 'd', 'c', 'b', 'a'};
//    int freq[] = {45, 16, 13, 12, 9, 5};

//    char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
//    int freq[] = { 5, 9, 12, 13, 16, 45 };

    std::map<std::string, int> symbolFreq;

    std::string InputFileName = "../input.txt";
    std::string OutputFileName = "../output.txt";
    std::string EncodeFileName = "../encode.txt";

    std::ifstream InputFile (InputFileName);
    std::ofstream OutputFile(OutputFileName);
    std::ofstream EncodeFile(EncodeFileName);

    if (!OutputFile.is_open()){
        std::cout << "Could not open output file\n";
        return EXIT_FAILURE;
    }

    if (!InputFile.is_open()){
        std::cout << "Could not open input file\n";
        return EXIT_FAILURE;
    }

    char c1, c2;
    std::string symbol ("");

    while(!InputFile.eof()){
        c1 = InputFile.get();
        symbol = c1;
        if (!InputFile.eof()){
            c2 = InputFile.get();
            symbol += c2;
        } else {
        }

        if (c1 != -1){

            auto itr = symbolFreq.find(symbol);

            if (itr == symbolFreq.end()){
                symbolFreq.insert({symbol, 1});
            } else {
                itr->second += 1;
            }
        }

    }

    std::string arr[symbolFreq.size()];
    int freq[symbolFreq.size()];

    int i = 0;

    for (auto itr = symbolFreq.begin(); itr != symbolFreq.end(); ++itr) {
        OutputFile << itr->first;
        OutputFile << '\t' << itr->second << '\n';
        arr[i] =    itr->first;
        freq[i] =   itr->second;
        ++i;
    }


    InputFile.close();
    OutputFile.close();

    // find the size
    int size = symbolFreq.size();
//    int size = 6;

    // build and print out the HuffmanCodes
    HuffmanCodes(arr, freq, size, EncodeFile);

    EncodeFile.close();

    return 0;
}


/**
 * The function that builds a Huffman Tree and print codes by traversing the built Huffman Tree
 *
 * @param data      std::string[] (pointer), the symbols sorted by frequencies
 * @param freq      int[] (pointer), the frequencies
 * @param size      int, the number of symbols (and frequencies)
 */
void HuffmanCodes(std::string data[], int freq[], int size, std::ofstream &EncodeFile){
    // Construct Huffman Tree
    // get root pointer
    struct MinHeapNode* root
            = buildHuffmanTree(data, freq, size);

    // Print Huffman codes using
    // the Huffman tree built above
    // Initialize needed array
    int arr[MAX_TREE_HT];
    int top = 0;

    // pass root and initialized variables to print out
    printCodes(root, arr, top, EncodeFile);
}

// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
/**
 * Prints huffman codes from root of Huffman tree
 * uses arr[] to store the codes
 * It is a recursive operation to store code into arr[]
 *
 * @param root      struct MinHeapNode *,  root of current Huffman tree
 * @param arr       int[] (pointer),                buffer to store code in
 * @param top       int,                            the height of pointer to root
 */
void printCodes(struct MinHeapNode* root, int arr[], int top, std::ofstream &EncodeFile){

    // Assign 0 to left edge and recursively call on left side of tree
    if (root->left) {

        arr[top] = 0;
        printCodes(root->left, arr, top + 1, EncodeFile);
    }

    // Assign 1 to right edge and recursively call on right side of tree
    if (root->right) {

        arr[top] = 1;
        printCodes(root->right, arr, top + 1, EncodeFile);
    }

    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]
    if (isLeaf(root)) {
        EncodeFile << root->data << ": \t\t";

        printArr(arr, top, EncodeFile);
    }
}

/**
 * The main function to build a Huffman tree
 *
 * @param data      std::string[],                 the symbols sorted by frequencies
 * @param freq      int[],                  the frequencies associated with symbols
 * @param size      int,                    the number of symbols
 * @return          struct MinHeapNode*,    the root node
 */
struct MinHeapNode* buildHuffmanTree(std::string data[], int freq[], int size){
    // create the left and right childs
    // create the top node that will join these childs
    struct MinHeapNode *left, *right, *top;

    // Step 1: Create a min heap of capacity
    // equal to size.  Initially, there are
    // modes equal to size.
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // Iterate over and build the tree while heap is not 1
    while (!isSizeOne(minHeap)) {

        // Step 2: Extract the two minimum
        // freq items from min heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Step 3:  Create a new internal
        // node with frequency equal to the
        // sum of the two nodes frequencies.
        // Make the two extracted node as
        // left and right children of this new node.
        // Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        top = newNode("$", left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    // Step 4: The remaining node is the
    // root node and the tree is complete.
    return extractMin(minHeap);
}

/**
 * Create a minimal heap of capacity equal to "size" and inserts all units of data[]
 * in min heap. Initially min heap is equal to capacity
 *
 * @param data      std::string[],                 the symbols sorted by frequencies
 * @param freq      int[],                  the frequencies associated with symbols
 * @param size      int,                    the number of symbols
 * @return          struct MinHeap *        the minimum heap
 */
struct MinHeap* createAndBuildMinHeap(std::string data[], int freq[], int size){

    // create a binary minimal heap of the same size as number of symbols
    struct MinHeap* minHeap = createMinHeap(size);

    // for each symbol, create new node for it in minHeap
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    // make sure minHeap size is correct
    minHeap->size = size;

    // build minHeap from internal array
    buildMinHeap(minHeap);

    return minHeap;
}


/**
 * Check if node of MinHeap is leaf or not
 * @param root      struct MinHeapNode *, the node
 * @return          C style int bool true or false
 */
int isLeaf(struct MinHeapNode* root){
    // if leaf, there would be no left and no right child
    return !(root->left) && !(root->right);
}


/**
 * Function print out an array, only up to index n
 *
 * @param arr   int[], the array of stuff
 * @param n     int, the number up to which to print out
 */
void printArr(int arr[], int n, std::ofstream &EncodeFile)
{
    for (int i = 0; i < n; ++i)
        EncodeFile << arr[i];

    EncodeFile << "\n";
}

/**
 * build the heap by calling minHeapify repeatedly
 *
 * @param minHeap       struct MinHeap *, the minimum heap to be built
 */
void buildMinHeap(struct MinHeap* minHeap){

    int n = minHeap->size - 1;

    // minHeapify the current nodes
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

/**
 * A function to insert a new node into minimum heap
 *
 * @param minHeap           struct MinHeap *,       the heap to be added to
 * @param minHeapNode       struct MinHeapNode *,   the node to be added
 */
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode){

    // increase the heap size
    ++minHeap->size;
    int i = minHeap->size - 1;

    // while walking up the height of heap
    // and node to be added has frequency less than the node at that height
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {

        // point current node to parent
        minHeap->array[i] = minHeap->array[(i - 1) / 2];

        // i now is of parent
        i = (i - 1) / 2;
    }

    // assign the current array[i] to node that need inserting
    minHeap->array[i] = minHeapNode;
}

/**
 * Extract the minimum node in heap
 * @param minHeap
 * @return
 */
struct MinHeapNode* extractMin(struct MinHeap* minHeap){

    // take the root of minheap (array)
    // cause it would be the smallest
    struct MinHeapNode* temp = minHeap->array[0];

    // put the furthest node at array root
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    // decrease size
    --minHeap->size;

    // heapify the minheap from root
    minHeapify(minHeap, 0);

    return temp;
}

/**
 * Return if size of heap is 1
 * @param minHeap   struct MinHeap *,   the min heap
 * @return          C style bool,       true or false
 */
int isSizeOne(struct MinHeap* minHeap){

    return (minHeap->size == 1);
}

/**
 * A recursive method to heapify the minheap
 * @param minHeap   struct MinHeap *,   the minheap need min-heapifying
 * @param idx       int,                the index
 */
void minHeapify(struct MinHeap* minHeap, int idx){

    // assume the index given is smallest
    int smallest = idx;

    // calculate the appropriate index for left and right child
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    // check if theoretical child is bigger than current smallest
    if (left < minHeap->size && minHeap->array[left]->
            freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->
            freq < minHeap->array[smallest]->freq)
        smallest = right;

    // if need to, swap smallest with current
    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest],
                        &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

/**
 * A function to swap two MinHeapNode
 * @param a
 * @param b
 */
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b){

    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}


// A utility function to create
// a min heap of given capacity
struct MinHeap* createMinHeap(unsigned capacity){

    struct MinHeap* minHeap
            = (struct MinHeap*)malloc(sizeof(struct MinHeap));

    // current size is 0
    minHeap->size = 0;

    minHeap->capacity = capacity;

    minHeap->array
            = (struct MinHeapNode**)malloc(minHeap->
            capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// A utility function allocate a new
// min heap node with given character
// and frequency of the character
struct MinHeapNode* newNode(std::string data, unsigned freq){
    struct MinHeapNode* temp
            = new MinHeapNode();

    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;

    return temp;
}