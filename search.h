// Do NOT add any other includes
#include <string>
#include <vector>
#include <iostream>
#include "Node.h"
#pragma once
using namespace std;

struct sinfo
{
    string sentence;
    int book_code;
    int page;
    int paragraph;
    int sentence_no;
};

class SearchEngine
{
private:
    // You can add attributes/helper functions here
    vector<vector<sinfo> > searchtree;
    // vector<vector<parainfo> > reftree;

public:
    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    SearchEngine();

    ~SearchEngine();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node *search(string pattern, int &n_matches);

    /* -----------------------------------------*/
};
