#include <stdio.h>

int main() {
    FILE *f1 = fopen("trie_dataset.txt", "r");   // open for reading (binary)
    FILE *f2 = fopen("data.txt", "w"); // open for writing (binary)
    if (f1 == NULL) {
        printf("Error: could not open source file.\n");
        return 1;
    }
    if (f2 == NULL) {
        printf("Error: could not create destination file.\n");
        fclose(f1);
        return 1;
    }

    char ch;
    while (fread(&ch, 1, 1, f1) == 1) {
        fwrite(&ch, 1, 1, f2);
    }

    printf("Copy successful!\n");

    fclose(f1);
    fclose(f2);
    return 0;
}
