// Do NOT add any other includes
#include "search.h"
SearchEngine::SearchEngine() { searchtree.resize(1000); }

SearchEngine::~SearchEngine()
{
    // Implement your function here
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph,
                                   int sentence_no, string sentence)
{
    for (int i = 0; i < sentence.size(); i++)
    {
        if (sentence[i] >= 'A' && sentence[i] <= 'Z')
        {
            sentence[i] = 'a' + sentence[i] - 'A';
        }
    }
    if (sentence.size() > searchtree.size())
    {
        searchtree.resize(sentence.size()+1);
    }
        searchtree[sentence.size()].push_back(
        {sentence, book_code, page, paragraph, sentence_no});
    
    
}
void insertattail(int b_code, int pg, int para, int s_no, int off, Node *&tail,
                  Node *&head)
{
    if (tail == NULL)
    {
        Node *temp = new Node(b_code, pg, para, s_no, off);
        head = temp;
        tail = temp;
    }
    else
    {
        Node *temp = new Node(b_code, pg, para, s_no, off);
        temp->left = tail;
        tail->right = temp;
        tail = temp;
    }
}
void boyermoore(string pattern, int &n, Node *&head, Node *&tail,
                string sentence, int book_code, int page, int paragraph,
                int sentence_no)
{
    int sz = pattern.size();
    int i = 0;
    vector<int> v1(256);
    while (i < 256)
    {
        v1[i] = -1;
        i++;
    }
    for (int j = 0; j < sz; j++)
    {
        v1[(int)pattern[j]] = j;
    }
    int s = 0;
    while (s <= (sentence.size() - sz))
    {
        int j = sz - 1;
        while (j >= 0 && pattern[j] == sentence[s + j])
        {
            j--;
        }
        if (j < 0)
        {
            insertattail(book_code, page, paragraph, sentence_no, s, tail, head);
            n++;
            s += (s + sz < sentence.size()) ? sz - v1[sentence[s + sz]] : 1;
        }
        else
        {
            s += std::max(1, j - v1[sentence[s + j]]);
        }
    }
}
Node *SearchEngine::search(string pattern, int &n_matches)
{
    Node *tail = NULL;
    Node *head = NULL;
    for (int i = pattern.size(); i < searchtree.size(); i++)
    {
        for (int j = 0; j < searchtree[i].size(); j++)
        {
            boyermoore(pattern, n_matches, head, tail, searchtree[i][j].sentence,
                       searchtree[i][j].book_code, searchtree[i][j].page,
                       searchtree[i][j].paragraph, searchtree[i][j].sentence_no);
        }
    }
    return head;
}
// int main()
// {
//     SearchEngine a;
//     a.insert_sentence(1, 1, 1, 1, "how are you01293784now");
//     // a.insert_sentence(1, 1, 1, 2, "how are you wufgwfj");
//     // a.insert_sentence(1, 1, 1, 3, "how are you wufgwfj");
//     // a.insert_sentence(1, 1, 1, 3, "how are you hsjdghj");
//     // a.insert_sentence(1, 1, 1, 5, "how are you nbnfjkgj");
//     // a.insert_sentence(1, 1, 1, 6, "how are you qdqffj");
//     // a.insert_sentence(1, 1, 1, 7, "how are you mfsegk");
//     // a.insert_sentence(1, 1, 1, 8, "how are you vnmdnbd");
//     // a.insert_sentence(1, 1, 1, 9, "how are you jfhhfdjj");
//     int n = 0;
//     Node *temp = a.search("you", n);
//     cout << n << endl;
//     return 0;
// }