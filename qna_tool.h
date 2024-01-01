#pragma once
#include <iostream>
#include <fstream>
#include "Node.h"
#include "dict.h"
#include "search.h"

using namespace std;

struct paradict
{
    int bookcode;
    int page;
    int paragraph;
    Dict pdict;
};

class TrieNode {
public:
    std::vector<TrieNode*> children;
    bool end;
    long long freq = 0;
    TrieNode()
    {
        children.resize(26, nullptr);
        end = (false);
    };
};

class Trie {
private:
    TrieNode* root;
    int toindex(char ch) {
        return ch - 'a';
    }

public:
    Trie() : root(new TrieNode()) {}

    void insert(string word, long long ct) {
        TrieNode* current = root;
        for (char ch : word) {
            int index = toindex(ch);
            if (current->children[index] == nullptr) {
                current->children[index] = new TrieNode();
            }
            current = current->children[index];
        }
        current->freq = ct;
        current->end = true;
    }

    long long search(const std::string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            int index = toindex(ch);
            if (current->children[index] == nullptr) {
                return 0; 
            }
            current = current->children[index];
        }
        return current->freq; 
    }
};
struct parainfo
{
    int book_code;
    int page;
    int paragraph;
    double score;
    
};

class QNA_tool {

private:

    // You are free to change the implementation of this function
    void query_llm(string filename, Node* root, int k, string API_KEY, string question);
    // filename is the python file which will call ChatGPT API
    // root is the head of the linked list of paragraphs
    // k is the number of paragraphs (or the number of nodes in linked list)
    // API_KEY is the API key for ChatGPT
    // question is the question asked by the user

    // You can add attributes/helper functions here

public:

    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search 
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    std::string get_paragraph(int book_code, int page, int paragraph);
    // Given the book_code, page number, and the paragraph number, returns the string paragraph.
    // Searches through the corpus.

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
    void heapifyUp(int index,vector <parainfo> &heap)
{
    if (index ==0) return;

    if (heap[(index - 1)/2].score > heap[index].score)
    {
        parainfo a;
        a =  heap[index];
        heap[index] = heap[(index - 1)/2];
        heap[(index - 1)/2] = a;
        heapifyUp((index - 1)/2,heap);
    }
}

void heapifyDown(vector <parainfo> &heap,int index)
{
      int left = 2* index + 1;
      int right = 2 * index + 2;

      if (left >= heap.size() ) return;

      int minindex = index;
      if(heap[index].score > heap[left].score)
      {
          minindex = left;
      }
      if (right < heap.size() && (heap[minindex].score > heap[right].score)) minindex = right;

      if (index != minindex)
      {
        parainfo a;
        a =  heap[index];
        heap[index] = heap[minindex];
        heap[minindex] = a;

        heapifyDown(heap,minindex);
      }

}
void insert(vector <parainfo> &heap,parainfo para)
{
    heap.push_back({para.book_code,para.page,para.paragraph,para.score});
    heapifyUp(heap.size() - 1,heap);
}
void deleteTop(vector <parainfo> &heap)
{
    heap[0] = heap.back();
    heap.pop_back();
    heapifyDown(heap,0);
}
void  klarge(vector <parainfo>&arr,parainfo ele,int k)
{

    if(arr.size() < k)
    {
        insert(arr,ele);
    }
    else
    {
        if(arr[0].score < ele.score) 
        {
          arr[0] = ele;
          heapifyDown(arr,0);
        }
      
    }
      
    
}
    Dict dict;
    SearchEngine db;
    vector<paradict> paradata;
    Trie refwords;
    vector <parainfo> topk;
    
    int currentpara = -1;
    int currentbook = -1;
    int currentpage = -1;
};