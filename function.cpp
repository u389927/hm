#include "function.h"
#include <iostream>
#include <vector>
#include <string>
#include <list>

using namespace std;

void Commit::Clear(TrieNode* node) {
    if (!node) return;
    for (int i = 0; i < 26; ++i) {
        if (node->children[i]) {
            Clear(node->children[i]);
        }
    }
    delete node;
}

int Commit::Insert(FileChange fileChange) {
    m_commits.push_back(std::move(fileChange));
    int idx = m_commits.size() - 1;

    TrieNode* p = m_root;
    for (string::const_iterator i = m_commits[idx].fileName.begin(); i != m_commits[idx].fileName.end(); i++) {
        char c = *i;
        int ci = c - 'a';
        if (!p->children[ci]) {
            p->children[ci] = new TrieNode();
        }
        p = p->children[ci];
    }
    p->idx = idx;
    return idx;
}

const Commit::FileChange* Commit::Search(const std::string& str) {
    TrieNode* p = m_root;
    for (std::string::const_iterator i = str.begin(); i != str.end(); i++) {
        char c = *i;
        int ci = c - 'a';
        if (ci < 0 || ci >= 26 || !p->children[ci]) {
            return nullptr;
        }
        p = p->children[ci];
    }
    return (p->idx != -1 ? &m_commits[p->idx] : nullptr);
}

Git::Git() {
    Commit* rootCommit = new Commit();
    GitNode* rootNode = new GitNode(-1, 0, rootCommit);
    m_gitnodes.push_back(rootNode);
    m_current = 0;
}

Git::~Git() {
    for (vector<GitNode*>::iterator i = m_gitnodes.begin(); i != m_gitnodes.end(); i++) {
        delete (*i)->commit;
        delete *i;
    }
}

void Git::Switch(int idx) {
    m_current = idx;
    cout << "Switched to Branch " << idx << endl;
}

void Git::CreateCommit(Commit* commit) {
    int parent = m_current;
    int lvl = m_gitnodes[parent]->level + 1;
    GitNode* node = new GitNode(parent, lvl, commit);
    m_gitnodes.push_back(node);
    int newIdx = m_gitnodes.size() - 1;
    m_gitnodes[parent]->children.push_back(newIdx);
    m_current = newIdx;
}

vector<string> Git::Open(const string& filename) {
    vector<string> content;
    vector<int> path;

    for (int i = m_current; i != -1; i = m_gitnodes[i]->parent) {
        path.push_back(i);
    }

    for (vector<int>::reverse_iterator i = path.rbegin(); i != path.rend(); i++) {
        const Commit::FileChange* fc = m_gitnodes[*i]->commit->Search(filename);
        if (!fc) continue;

        for (list<Commit::LineChange>::const_iterator j = fc->insertions.begin();
             j != fc->insertions.end(); j++) {
            int pos = j->index;
            if (pos >= 0 && pos <= content.size()) {
                content.insert(content.begin() + pos, j->content);
            }
        }

        for (list<Commit::LineChange>::const_reverse_iterator k = fc->deletions.rbegin(); k != fc->deletions.rend(); k++) {
            int pos = k->index;
            if (pos >= 0 && pos < content.size()) {
                content.erase(content.begin() + pos);
            }
        }
    }

    return content;
}
