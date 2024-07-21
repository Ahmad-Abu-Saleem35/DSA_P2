//ahmadabusaleem
//1201315
//sec2
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char word[100];
    char meanings[3][100];
} Entry;

typedef Entry *HashTable;

// Sizes of the linear and double hash tables
int linearTableSize = 2;
int doubleTableSize = 2;

// Count of entries in the linear and double hash tables
int linearEntryCount = 0;
int doubleEntryCount = 0;

// Load factors for the linear and double hash tables
double linearLoadFactor = 1.0;
double doubleLoadFactor = 1.0;

// Count of collisions in the linear and double hash tables
int linearCollisions = 0;
int doubleCollisions = 0;

// Pointers to the linear and double hash tables
HashTable *linearTable;
HashTable *doubleTable;

// Function to insert an entry into the linear hash table
void insertEntry(Entry *entry);

// Function to insert an entry into the double hash table
void insertEntryDoubleHash(Entry *entry);

// Function to rehash the double hash table
void rehashDouble();

// Function to search for an entry in the hash tables
int searchEntry(const char *word);

// Function to delete an entry from the hash tables
int deleteEntry(const char *word);

// Function to print the count of collisions in the hash tables
void printCollisions();

int isPrime(int num) {
    for (int i = 2; i < num / 2; i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

int nextPrime(int num) {
    int n = num;
    if (n % 2 == 0) {
        n += 1;
    }
    while (!isPrime(n)) {
        n = n + 2;
    }
    return n;
}
// Compute a simple hash value for a given word
int hashFunction(const char *word) {
    int len = strlen(word);
    int sum = 0;
    for (int i = 0; i < len; i++) {
        sum += word[i];
    }
    return sum;
}
// Compute linear hash value for a given word and index
int linearHash(int i, const char *word) {
    int j = hashFunction(word);
    return (j + i) % linearTableSize;
}
// Compute double hash value for a given word and index
int linearHashDouble(int k, const char *word) {
    int j = hashFunction(word);
    return j % doubleTableSize;
}
// Compute the second hash value for double hashing
int doubleEq(const char *word) {
    int j = hashFunction(word);
    int h2 = doubleTableSize - j % doubleTableSize;
    return h2;
}

void rehashLinear() {
    int s = linearTableSize;
    linearTableSize = nextPrime(linearTableSize * 2);

    HashTable *table = linearTable;

    linearTable = malloc(sizeof(HashTable) * linearTableSize);

    for (int i = 0; i < linearTableSize; ++i) {
        linearTable[i] = NULL;
    }

    linearEntryCount = 0;
    for (int i = 0; i < s; ++i) {
        if (table[i])
            insertEntry(table[i]);
    }
}

void rehashDouble() {
    int s = doubleTableSize;
    doubleTableSize = nextPrime(doubleTableSize * 2);

    HashTable *table = doubleTable;

    doubleTable = malloc(sizeof(HashTable) * doubleTableSize);

    for (int i = 0; i < doubleTableSize; ++i) {
        doubleTable[i] = NULL;
    }

    doubleEntryCount = 0;
    for (int i = 0; i < s; ++i) {
        if (table[i])
            insertEntryDoubleHash(table[i]);
    }
}
void readAndInsertEntries() {
    FILE *in = fopen("named words.txt", "r");
    char line[1000];

    while (fgets(line, sizeof(line), in) != NULL) {
        char *token = strtok(line, "\n");
        char *tk = strtok(token, ":");
        char word[100];
        strcpy(word, tk);

        tk = strtok(NULL, ":");
        char meanings[3][100];
        int count_meanings = 0;
        char *meaning = strtok(tk, "#");
        while (meaning != NULL && count_meanings < 3) {
            strcpy(meanings[count_meanings], meaning);
            meaning = strtok(NULL, "#");
            count_meanings++;
        }

        Entry *entry = malloc(sizeof(Entry));
        strcpy(entry->word, word);
        for (int i = 0; i < count_meanings; ++i) {
            strcpy(entry->meanings[i], meanings[i]);
        }

        insertEntry(entry);
        insertEntryDoubleHash(entry);
    }

    fclose(in);
}

void insertEntry(Entry *entry) {
    linearLoadFactor = (double)(linearEntryCount + 1) / linearTableSize;

    if (linearLoadFactor >= 0.75)
        rehashLinear();

    for (int i = 0; i < linearTableSize; i++) {
        int k = linearHash(i, entry->word);
        if (linearTable[k] == NULL) {
            linearTable[k] = entry;
            break;
        }
        linearCollisions++;
    }
    linearEntryCount++;
}
void insertEntryDoubleHash(Entry *entry) {
    doubleLoadFactor = (double)(doubleEntryCount + 1) / doubleTableSize;

    if (doubleLoadFactor >= 0.75)
        rehashDouble();

    for (int i = 0; i < doubleTableSize; i++) {
        int k = linearHash(i, entry->word);
        int j = doubleEq(entry->word);
        int m = k + i * j;
        int s = m % doubleTableSize;
        if (doubleTable[s] == NULL) {
            doubleTable[s] = entry;
            break;
        }
        doubleCollisions++;
    }
    doubleEntryCount++;
}

void printTable(const HashTable *table, int size) {
    for (int i = 0; i < size; i++) {
        if (table[i] == NULL)
            printf("%d] -----\n", i);
        else {
            printf("%d] %s: ", i, table[i]->word);
            for (int j = 0; j < 3; j++) {
                printf("%s ", table[i]->meanings[j]);
                if (j < 2 && table[i]->meanings[j + 1][0] != '\0')
                    printf("# ");
            }
            printf("\n");
        }
    }
}
void printTableSizeAndLoadFactor() {
    printf("\n LINEAR HASH FUNCTION \n");
    printf("Linear Table Size = %d      Elements Count  =  %d         Load Factor = %0.2f\n\n", linearTableSize, linearEntryCount, linearLoadFactor);

    printf("\nDOUBLE HASH FUNCTION \n");
    printf("Double Table Size = %d      Elements Count  =  %d         Load Factor = %0.2f\n\n", doubleTableSize, doubleEntryCount, doubleLoadFactor);
}

void printHashFunctions() {
    printf("  Linear Method   --->  (j + i) %% linearTableSize\n\n");
    printf("  Double  Method   ---> doubleTableSize - j %% doubleTableSize\n\n");
}

void addEntry() {
    Entry *entry = malloc(sizeof(Entry));
    printf("Enter The new word and its meanings (max 3) in the format 'word:meaning1#meaning2#meaning3':\n");
    scanf("%s", entry->word);
    char meanings[3][100];
    scanf(":%99[^#]#%99[^#]#%99[^\n]", meanings[0], meanings[1], meanings[2]);
    for (int i = 0; i < 3; i++) {
        strcpy(entry->meanings[i], meanings[i]);
    }

    insertEntry(entry);

    printf("\nLINEAR HASH FUNCTION \n");
    printTable(linearTable, linearTableSize);

    insertEntryDoubleHash(entry);

    printf("\n DOUBLE HASH FUNCTION \n");
    printTable(doubleTable, doubleTableSize);
}
// Search for an entry in the linear hash table
int searchEntry(const char *word) {
    for (int i = 0; i < linearTableSize; i++) {
        int k = linearHash(i, word);
        if (linearTable[k] != NULL && strcmp(linearTable[k]->word, word) == 0) {
            printf("Word: %s was found in index %d in Linear Table.\n", word, k);
            return 1;
        }
    }
    return 0;
}
// Search for an entry in the double hash table
int searchEntryDoubleHash(const char *word) {
    for (int i = 0; i < doubleTableSize; i++) {
        int k = linearHash(i, word);
        int j = doubleEq(word);
        int m = k + i * j;
        int s = m % doubleTableSize;
        if (doubleTable[s] != NULL && strcmp(doubleTable[s]->word, word) == 0) {
            printf("Word: %s was found in index %d in Double Table.\n", word, s);
            return 1;
        }
    }
    return 0;
}
// Menu for searching an entry in either table
void searchEntryMenu() {
    char word[100];
    printf("Enter the word you want to search:\n");
    scanf("%s", word);
    printf("Which Table You Want Search In? Choose (L) for Linear Table and (D) for Double Table:\n");
    char ch[2];
    scanf("%s", ch);

    int result;
    if (ch[0] == 'L' || ch[0] == 'l') {
        result = searchEntry(word);
        if (result == 0)
            printf("Word not found in Linear Table.\n");
    } else if (ch[0] == 'D' || ch[0] == 'd') {
        result = searchEntryDoubleHash(word);
        if (result == 0)
            printf("Word not found in Double Table.\n");
    }
}
// Delete an entry from the linear hash table
int deleteEntry(const char *word) {
    for (int i = 0; i < linearTableSize; i++) {
        int k = linearHash(i, word);
        if (linearTable[k] != NULL && strcmp(linearTable[k]->word, word) == 0) {
            printf("Word: %s was deleted from index %d in Linear Table.\n", word, k);
            free(linearTable[k]);
            linearTable[k] = NULL;
            linearEntryCount--;
            return 1;
        }
    }
    return 0;
}
// Delete an entry from the double hash table
int deleteEntryDoubleHash(const char *word) {
    for (int i = 0; i < doubleTableSize; i++) {
        int k = linearHash(i, word); // Compute hash values
        int j = doubleEq(word);
        int m = k + i * j;
        int s = m % doubleTableSize;
        if (doubleTable[s] != NULL && strcmp(doubleTable[s]->word, word) == 0) { // Check if the slot contains the target entry
            printf("Word: %s was deleted from index %d in Double Table.\n", word, s);
            free(doubleTable[s]);
            doubleTable[s] = NULL;
            doubleEntryCount--;
            return 1;
        }
    }
    return 0;
}

void deleteEntryMenu() {
    char word[100];
    printf("Enter the word you want to delete:\n");
    scanf("%s", word);
    int result1, result2;
    result1 = deleteEntry(word);
    result2 = deleteEntryDoubleHash(word);

    if (result1 == 0)
        printf("Word not found in Linear Table.\n");
    if (result2 == 0)
        printf("Word not found in Double Table.\n");
}

void compareCollisions() {
    printf("The Number Of Collisions in Linear Table: %d\n", linearCollisions);
    printf("The Number Of Collisions in Double Table: %d\n", doubleCollisions);
}
void readEntries()
 {
     readAndInsertEntries();
    printf("\n LINEAR HASH FUNCTION \n");
    printTable(linearTable, linearTableSize);

    printf("\n DOUBLE HASH FUNCTION \n");
    printTable(doubleTable, doubleTableSize);
}

void saveHashTable() {
    FILE *out = fopen("saved_words.txt", "w");

    fprintf(out, "\n DOUBLE HASH FUNCTION \n");

    for (int i = 0; i < doubleTableSize; i++) {
        if (doubleTable[i] == NULL) {
            fprintf(out, "%d] -----\n", i);
        } else {
            fprintf(out, "%d] %s: ", i, doubleTable[i]->word);
            for (int j = 0; j < 3; j++) {
                fprintf(out, "%s ", doubleTable[i]->meanings[j]);
                if (j < 2 && doubleTable[i]->meanings[j + 1][0] != '\0')
                    fprintf(out, "# ");
            }
            fprintf(out, "\n");
        }
    }

    fclose(out);
}

int main() {
    linearTable = malloc(sizeof(HashTable) * linearTableSize);
    for (int i = 0; i < linearTableSize; i++) {
        linearTable[i] = NULL;
    }

    doubleTable = malloc(sizeof(HashTable) * doubleTableSize);
    for (int i = 0; i < doubleTableSize; i++) {
        doubleTable[i] = NULL;
    }
int choice=0;
while (1) {
    printf("   1. Print hashed tables (including empty spots).\n");
    printf("   2. Print out table size and the load factor.\n");
    printf("   3. Print out the used hash functions.\n");
    printf("   4. Insert a new record to hash table (insertion will be done on both hash tables).\n");
    printf("   5. Search for a specific word (specify which table to search in).\n");
    printf("   6. Delete a specific word (from both tables).\n");
    printf("   7. Compare between the two methods in terms of the number of collisions occurred.\n");
    printf("   8. Save hash table back to a file named saved_entries.txt (of the double hashing).\n");
    printf("   9. Exit.\n");

    scanf("%d", &choice);

    switch (choice) {
    case 1:
       readEntries();
        break;
    case 2:
        printTableSizeAndLoadFactor();
        break;
    case 3:
        printHashFunctions();
        break;
    case 4:
        addEntry();
        break;
    case 5:
        searchEntryMenu();
        break;
    case 6:
        deleteEntryMenu();
        break;
    case 7:
        compareCollisions();
        break;
    case 8:
        saveHashTable();
        printf("\nSave To File Done\n");
        break;
    case 9:
        exit(0);
    }
}
}
