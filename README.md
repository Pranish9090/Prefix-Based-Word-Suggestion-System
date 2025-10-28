# 🔍 Search Suggestion System using Trie (C Project)

## 📘 Overview

This project is a **Search Suggestion System implemented in C**, built using the **Trie (Prefix Tree)** data structure.
It simulates how modern search engines and text boxes suggest completions as you type — fast, ranked, and adaptive to search frequency.

This program loads word data from a text file, lets users search and add new words, shows ranked suggestions, and safely saves updates back to disk.

---

## ⚙️ Features

* **⚡ Instant Search Suggestions**
  Uses Trie traversal for O(length_of_prefix) lookups — extremely fast even for large datasets.

* **📊 Ranked by Frequency**
  Frequently searched words appear higher in suggestions.

* **💾 Persistent Storage**
  All data (words + search frequency) are stored in `data.txt` and safely updated after each session.

* **🧱 Safe Saving System**
  Automatically creates a temporary file and backup to prevent data loss during saves.

* **🧮 Menu-Based Interface**
  Allows you to:

  * Search for suggestions
  * Add new words
  * View all stored words
  * Exit and save data safely

* **🧠 Memory-Efficient Design**
  Shares prefixes efficiently through the Trie, reducing redundant memory use.

---

## 🧩 Data Structure

```c
#define ALPHABET_SIZE 26  // for letters a–z

struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int isEndOfWord;  // marks valid word ending
    int frequency;    // number of times word has been searched
};
```

### ⚙️ How It Works

* Every character in a word is a node in the Trie.
* Words sharing prefixes share nodes (e.g., *pro* in *program*, *project*).
* Frequencies are incremented whenever a user selects that word.

---

## 📂 Data Storage Format

All data is saved to `data.txt` in the form:

```
problem 0
process 1
program 2
progress 1
project 3
```

Each line = `word frequency`
This makes the dataset human-readable and easy to edit.

---

## 🧱 Core Functions

| Function                     | Purpose                                       |
| ---------------------------- | --------------------------------------------- |
| `createNode()`               | Creates a new Trie node                       |
| `insert(word, freq)`         | Inserts a word with its frequency             |
| `increaseFrequency(word)`    | Increments frequency when word is selected    |
| `collectWords(root, prefix)` | Recursively collects all matching words       |
| `showSuggestions(prefix)`    | Displays ranked results for a given prefix    |
| `saveTrieToText()`           | Saves all Trie data to file safely            |
| `loadTrieFromText()`         | Loads words and frequencies from file         |
| `displayAllWords()`          | Prints all stored words and their frequencies |
| `menu()`                     | Interactive main menu loop for user actions   |

---

## 💻 Example Program Flow

```
Main Function!
1. Search Suggestions
2. Add New Word
3. View All Words
4. Exit
Enter your choice: 1

Enter prefix: pro

Suggestions for "pro":
project (searched 3 times)
program (searched 2 times)
progress (searched 1 time)
problem (searched 0 times)

Select a word: project
✅ Frequency updated!
💾 Data saved successfully!
```

---

## 🚀 How to Run

### 1️⃣ Compile

```bash
gcc main.c -o trie_search
```

### 2️⃣ Ensure dataset exists

Example `data.txt`:

```
apple 0
application 1
apply 3
appetite 2
approach 1
```

### 3️⃣ Run the program

```bash
./trie_search
```

---

## 🧰 Error Handling & Safety

* If invalid input (like characters instead of numbers) is entered in menus,
  the input buffer is cleared and the user is re-prompted.
* If saving fails, the previous dataset is automatically backed up.
* If the dataset is empty, the program still runs and allows adding new words.

---

## 🧩 Upcoming Improvements

✅ Planned Features:

* **Support for Alphanumeric Words** (e.g., *task1*, *item99*)
* **Auto-save every X minutes** to avoid data loss
* **Export/Import JSON datasets**
* **Interactive GUI or Web Interface** using the same logic
* **Compressed Trie (Radix Tree)** for very large data

---

## 🧠 Technical Notes

* Current implementation supports **lowercase English letters** (`a–z`).
* If your dataset includes digits or symbols, they will be ignored during loading.
* Trie depth limit is 100 characters per word (configurable).

---

## 👨‍💻 Author

**Pranish**
Role: Android & Web Developer

Project: *Search Suggestion System using Trie Data Structure (C)*

---

## 🏷️ Repository Tags

`#C` `#DataStructure` `#Trie` `#SearchEngine` `#AutoComplete` `#Algorithms`
