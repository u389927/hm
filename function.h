#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <list>
#include <string>
#include <iostream>

#define MAX_NODE_SIZE 10005


#include <vector>
#include <stack>
#include <string>
#include <list>

class Commit {
public:
    struct LineChange {
        int index;
        std::string content;
    };

    struct FileChange {
        std::string fileName;
        std::list<LineChange> insertions;
        std::list<LineChange> deletions;

        FileChange(std::string name) : fileName(name) {}

        void addInsertion(int index, const std::string& line) {
            insertions.push_back({index, line});
        }

        void addDeletion(int index, const std::string& line) {
            deletions.push_back({index, line});
        }
    };

private:
    // Trie Data Structure
    struct TrieNode {
        TrieNode* children[26];
        int idx;
        
        TrieNode() {
            idx = -1;
            for(int i = 0; i < 26; i++) children[i] = nullptr;
        }
    };
    
    TrieNode* m_root;
    std::vector<Commit::FileChange> m_commits;

public:
    // Init Trie
    Commit() {
        m_root = new TrieNode();
    }
    // Destroy Trie
    ~Commit() {
        Clear(m_root);
    }
    // Copy Constructor
    Commit(const Commit&) = delete;
    Commit& operator=(const Commit&) = delete;


    void Clear(TrieNode* node); // TODO: Clear a tree by given root node
    int Insert(FileChange fileChange);  // TODO: Insert a file change to a commit
    const Commit::FileChange* Search(const std::string& str);   // TODO: retrun the address of the FileChange, given string name
};

class GitNode {
private:
    std::vector<int> children;  // Childerns of the node
    int parent;                 // Parent of the node
    int level;                  // Level of that node in that tree
    Commit* commit;             // Commit content
public:
    GitNode() : parent(-1), level(-1), commit(nullptr) {}
    GitNode(int p, int l) : parent(p), level(l), commit(nullptr) {}
    GitNode(int p, int l, Commit* c) : parent(p), level(l), commit(c) {}

    friend class Git;
};

class Git{
private:
    std::vector<GitNode*> m_gitnodes;   // Store every Git
    int m_current;                      // Current branch and commit in (int) index term

public:
    // Constructor
    Git();
    Git(const Git&) = delete;
    Git& operator=(const Git&) = delete;
    // Destructor
    ~Git();

    void Switch(int idx); // TODO: Switch Command
    void CreateCommit(Commit* commit); // TODO: Commit Command
    std::vector<std::string> Open(const std::string& filename); // TODO: Git Open
};

#endif