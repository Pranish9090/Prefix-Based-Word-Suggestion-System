#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int isEndOfWord;
    int frequency;  // how often this word was searched
};

void saveTrieToText(struct TrieNode* root);

// create a new node
struct TrieNode* createNode() {
    struct TrieNode* node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    node->isEndOfWord = 0;
    node->frequency = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    return node;
}

// insert word into trie
void insert(struct TrieNode* root, const char* word) {
    struct TrieNode* current = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (current->children[index] == NULL)
            current->children[index] = createNode();
        current = current->children[index];
    }
    current->isEndOfWord = 1;
}

// increase frequency when a word is searched
void increaseFrequency(struct TrieNode* root, const char* word) {
    struct TrieNode* current = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (current->children[index] == NULL)
            return; // word not found
        current = current->children[index];
    }
    if (current->isEndOfWord)
        current->frequency++;
}

// store words for sorting
struct Word {
    char text[100];
    int freq;
};

struct Word words[100];
int wordCount = 0;

void collectWords(struct TrieNode* root, char* prefix) {
    if (root->isEndOfWord) {
        strcpy(words[wordCount].text, prefix);
        words[wordCount].freq = root->frequency;
        wordCount++;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]) {
            char next[100];
            sprintf(next, "%s%c", prefix, i + 'a');
            collectWords(root->children[i], next);
        }
    }
}

// sort by frequency (descending)
int compare(const void* a, const void* b) {
    struct Word* w1 = (struct Word*)a;
    struct Word* w2 = (struct Word*)b;
    return w2->freq - w1->freq;
}

// show ranked suggestions for a prefix
void showSuggestions(struct TrieNode* root, const char* prefix) {
    struct TrieNode* current = root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = prefix[i] - 'a';
        if (current->children[index] == NULL) {
            printf("No suggestions for \"%s\".\n", prefix);
            return;
        }
        current = current->children[index];
    }

    wordCount = 0;
    collectWords(current, (char*)prefix);
    qsort(words, wordCount, sizeof(struct Word), compare);

    printf("\nSuggestions for \"%s\":\n", prefix);
    for (int i = 0; i < wordCount; i++) {
        printf("%s (searched %d times)\n", words[i].text, words[i].freq);
    }
}

int main() {
    struct TrieNode* root = createNode();

    insert(root, "project");
    insert(root, "program");
    insert(root, "process");
    insert(root, "progress");
    insert(root, "problem");

    // simulate previous searches
    increaseFrequency(root, "project");
    increaseFrequency(root, "project");
    increaseFrequency(root, "project");
    increaseFrequency(root, "program");
    increaseFrequency(root, "program");
    increaseFrequency(root, "process");

    char prefix[100];
    printf("Enter prefix: ");
    scanf("%s", prefix);

    showSuggestions(root, prefix);
    saveTrieToText(root);
    return 0;
}

void saveWords(struct TrieNode* root, char* buffer, int depth, FILE* file) {
    if (root == NULL) return;

    if (root->isEndOfWord) {
        buffer[depth] = '\0';
        fprintf(file, "%s %d\n", buffer, root->frequency);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]) {
            buffer[depth] = i + 'a';
            saveWords(root->children[i], buffer, depth + 1, file);
        }
    }
}

void saveTrieToText(struct TrieNode* root) {
    FILE* file = fopen("words.txt", "w");
    char buffer[100];
    saveWords(root, buffer, 0, file);
    fclose(file);
}