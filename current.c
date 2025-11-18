#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define FILENAME "data.txt"

struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int isEndOfWord;
    int frequency;
};

struct TrieNode* root;

// function prototypes
struct TrieNode* createNode();
void insert(const char* word, int freq);
void increaseFrequency(const char* word);
void collectWords(struct TrieNode* root, char* prefix);
int compare(const void* a, const void* b);
void showSuggestions(const char* prefix);
void saveTrieToText();
void loadTrieFromText();

// store words for sorting
struct Word {
    char text[100];
    int freq;
};

struct Word words[1000];
int wordCount = 0;

// create a new node
struct TrieNode* createNode() {
    struct TrieNode* node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (!node) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    node->isEndOfWord = 0;
    node->frequency = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    return node;
}

// insert word into trie
void insert(const char* word, int freq) {
    struct TrieNode* current = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) continue;
        if (current->children[index] == NULL)
            current->children[index] = createNode();
        current = current->children[index];
    }
    current->isEndOfWord = 1;
    if(freq!=0) current->frequency = freq;
}

// increase frequency when a word is searched
void increaseFrequency(const char* word) {
    struct TrieNode* current = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (index < 0 || index >= ALPHABET_SIZE || current->children[index] == NULL)
            return;
        current = current->children[index];
    }
    if (current->isEndOfWord)
        current->frequency++;
}

void collectWords(struct TrieNode* root, char* prefix) {
    if (root == NULL) return;

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

int compare(const void* a, const void* b) {
    struct Word* w1 = (struct Word*)a;
    struct Word* w2 = (struct Word*)b;
    return w2->freq - w1->freq;
}

void showSuggestions(const char* prefix) {
    struct TrieNode* current = root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = prefix[i] - 'a';
        if (index < 0 || index >= ALPHABET_SIZE || current->children[index] == NULL) {
            printf("No suggestions for \"%s\".\n", prefix);
            return;
        }
        current = current->children[index];
    }

    wordCount = 0;
    collectWords(current, (char*)prefix);
    qsort(words, wordCount, sizeof(struct Word), compare);

    printf("\nSuggestions for \"%s\":\n", prefix);
    for (int i = 0; i < wordCount && i < 10; i++) {
        printf("%s (searched %d times)\n", words[i].text, words[i].freq);
    }
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

void saveTrieToText() {
    FILE *fp = fopen("data_temp.txt", "w");
    if (!fp) {
        printf("Unable to save data!\n");
        return;
    }
    char buffer[100];
    saveWords(root, buffer, 0, fp);
    fclose(fp);

    // Replace old file only after successful write
    remove("data.txt");
    rename("data_temp.txt", "data.txt");

    printf("Data saved successfully!\n");
}

void loadTrieFromText() {
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        printf("File %s not found! Starting with empty Trie.\n", FILENAME);
        return;
    }

    char word[100];
    int freq;
    int count = 0;

    while (fscanf(file, "%s %d", word, &freq) == 2) {
        insert(word, freq);
        count++;
        if (count % 100 == 0) printf("Loaded %d words...\n", count);
    }

    fclose(file);
    printf("Loaded %d words from %s\n", count, FILENAME);
}


int main() {
    root = createNode();
    loadTrieFromText();

    int choice;
    char word[100], prefix[100];
    char input[100]; // for safe input handling

    printf("\n==============================\n");
    printf("Trie Search Suggestion System\n");
    printf("==============================\n");

    do {
        printf("\nChoose an option:\n");
        printf("1 Search suggestions\n");
        printf("2  Add a new word\n");
        printf("3  Increase frequency of a word\n");
        printf("4  View top words (by frequency)\n");
        printf("5  Save data\n");
        printf("6  Exit\n");
        printf("Enter choice: ");

        // safer than scanf("%d", &choice)
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Input error!\n");
            continue;
        }

        // remove newline
        input[strcspn(input, "\n")] = '\0';

        // validate all digits
        int valid = 1;
        for (int i = 0; input[i]; i++) {
            if (!isdigit((unsigned char)input[i])) {
                valid = 0;
                break;
            }
        }

        if (!valid || strlen(input) == 0) {
            printf("Invalid input! Please enter a number between 1 and 6.\n");
            continue;
        }

        choice = atoi(input);

        switch (choice) {
            case 1:
                printf("\nEnter prefix: ");
                scanf("%s", prefix);
                getchar(); // clear leftover newline
                showSuggestions(prefix);
                break;

            case 2:
                printf("\nEnter new word: ");
                scanf("%s", word);
                getchar();
                insert(word, 0);
                printf("Word '%s' added successfully!\n", word);
                break;

            case 3:
                printf("\nEnter word to increase frequency: ");
                scanf("%s", word);
                getchar();
                increaseFrequency(word);
                printf("Frequency of '%s' updated!\n", word);
                break;

            case 4:
                printf("\nTop Words (Most Searched):\n");
                wordCount = 0;
                collectWords(root, "");
                qsort(words, wordCount, sizeof(struct Word), compare);
                for (int i = 0; i < wordCount && i < 10; i++)
                    printf("%d. %s (searched %d times)\n", i + 1, words[i].text, words[i].freq);
                break;

            case 5:
                saveTrieToText();
                break;

            case 6:
                printf("\nSaving data before exit...\n");
                saveTrieToText();
                printf("Exiting program. Goodbye!\n");
                break;

            default:
                printf("Invalid choice. Please enter between 1 and 6.\n");
        }
    } while (choice != 6);

    return 0;
}
