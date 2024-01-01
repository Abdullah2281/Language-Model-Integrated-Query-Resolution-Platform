// Do NOT add any other includes
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#pragma once
using namespace std;


struct info
{
    int book_code;
    int page;
    int paragraph;
    int sentence_no;
};
class SymNode
{
public:
    string key;
    int height;
    SymNode *left;
    SymNode *right;
    long long count;
    vector<info> v;
    SymNode()
    {
        key = "NULL";
        height = -1;
        left = NULL;
        right = NULL;
    }

    SymNode(string k, int book_cod, int pag, int paragrap, int sentence_n)
    {
        key = k;
        height = 0;
        left = NULL;
        right = NULL;
        count = 1;
        // for(auto para : v)
        // {
        //    if((para.book_code == book_cod)&&(para.page == pag)&&(para.paragraph == paragrap))
        //    {
        //         para.count++;
        //    }
        //    else
        //    {
                
        //    }
        // }
        info at = {book_cod,pag,paragrap,sentence_n};
        v.push_back(at);
    }
    int het(SymNode *root)
    {
        if (root == NULL)
        {
            return -1;
        }
        return root->height;
    }
    SymNode *LeftLeftRotation()
    {
        SymNode *y = this->right;
        SymNode *T2 = y->left;
        y->left = this;
        this->right = T2;

        this->height = 1 + std::max(het(this->left), het(this->right));
        y->height = 1 + std::max(het(y->left), het(y->right));

        return y;
    }
    SymNode *RightRightRotation()
    {
        SymNode *x = this->left;
        SymNode *T2 = x->right;

        x->right = this;
        this->left = T2;

        this->height = 1 + std::max(het(this->left), het(this->right));
        x->height = 1 + std::max(het(x->left), het(x->right));

        return x;
    }

    SymNode *LeftRightRotation()
    {
        SymNode *temp = left->LeftLeftRotation();
        this->left = temp;
        return this->RightRightRotation();
    }

    SymNode *RightLeftRotation()
    {
        SymNode *temp = right->RightRightRotation();
        this->right = temp;
        return this->LeftLeftRotation();
    }

    ~SymNode()
    {
    }
};
class SymbolTable : private SymNode
{
public:
    SymNode *root;
    SymbolTable()
    {
        root = NULL;
    }
    int ht(SymNode *root)
    {
        if (root == NULL)
        {
            return -1;
        }
        return root->height;
    }
    int differe(SymNode *root)
    {
        if (root == NULL)
        {
            return 0;
        }
        return (ht(root->right) - ht(root->left));
    }
    SymNode *inserting(SymNode *root, string k, int book_cod, int pag, int paragrap, int sentence_n)
    {
        if (root == NULL)
        {
            root = new SymNode(k, book_cod, pag, paragrap, sentence_n);
            root->height = 1 + std::max(ht(root->left), ht(root->right));
            return root;
        }
        else if (root->key == k)
        {
            root->count++;
            struct info at = {book_cod, pag, paragrap, sentence_n};
            root->v.push_back(at);
            return root;
        }
        else if (root->key < k)
        {
            root->right = inserting(root->right, k, book_cod, pag, paragrap, sentence_n);
            root->height = 1 + std::max(ht(root->left), ht(root->right));
        }
        else
        {
            root->left = inserting(root->left, k, book_cod, pag, paragrap, sentence_n);
            root->height = 1 + std::max(ht(root->left), ht(root->right));
        }
        if (differe(root) > 1)
        {
            if (k > root->right->key)
            {
                root = root->LeftLeftRotation();
            }
            else
            {
                root = root->RightLeftRotation();
            }
        }
        else if (differe(root) < -1)
        {
            if (k < root->left->key)
            {
                root = root->RightRightRotation();
            }
            else
            {
                root = root->LeftRightRotation();
            }
        }
        root->height = 1 + std::max(ht(root->left), ht(root->right));
        return root;
    }
    void insert(string k, int book_cod, int pag, int paragrap, int sentence_n)
    {
        root = inserting(root, k, book_cod, pag, paragrap, sentence_n);
    }
    SymNode *searching(SymNode *root, string k)
    {
        if (root == NULL)
        {
            return NULL;
        }
        if (root->key == k)
        {
            return root;
        }
        else if (root->key > k)
        {
            return searching(root->left, k);
        }
        else
        {
            return searching(root->right, k);
        }
        return NULL;
    }
    SymNode *search(string k)
    {
        return searching(root, k);
    }
    void deletesym(SymNode *root)
    {
        if (root == NULL)
        {
            return;
        }
        deletesym(root->left);
        deletesym(root->right);
        delete root;
        root = NULL;
    }
    ~SymbolTable()
    {
        if(root!= NULL) deletesym(root);
        root = NULL;
    }

};
class chaining
{
public:
    vector<SymbolTable *> bankStorage2d;
    chaining()
    {
        bankStorage2d.resize(769, new SymbolTable());
    }
};
class hashobj : public chaining
{
public:
    void addstr(string id, int book_code, int page, int paragraph, int sentence_no)
    {

        int h = hash(id);
        if (bankStorage2d[h]->root == NULL)
        {
            SymbolTable *temp = new SymbolTable();
            temp->insert(id, book_code, page, paragraph, sentence_no);
            bankStorage2d[h] = temp;
        }
        else
        {
            bankStorage2d[h]->insert(id, book_code, page, paragraph, sentence_no);
        }
    }

    long long returnct(string id)
    {
        int h = hash(id);
        SymbolTable *temp = bankStorage2d[h];
        if (temp == NULL)
        {
            return 0;
        }
        else
        {
            SymNode *node = temp->search(id);
            // cout<<node<<endl;
            if (node == NULL)
            {
                return 0;
            }
            // cout<<node<<endl;
            return node->count;
        }
    }
    // void updatescore(string id,double c)
    // {
    //     int h = hash(id);
    //     SymbolTable *temp = bankStorage2d[h];
    //     if (temp == NULL)
    //     {
    //         return ;
    //     }
    //     else
    //     {
    //         SymNode *node = temp->search(id);
    //         // cout<<node<<endl;
    //         if (node == NULL)
    //         {
    //             return;
    //         }
    //         // cout<<node<<endl;
    //         for(auto para:node->v)
    //         {
    //             para.score += static_cast<double>(para.count)*c;
    //         }
    //     }

    // }
    int hash(std::string id)
    {
        unsigned int hashcode = 5381;
        for (char c : id)
        {
            hashcode += ((hashcode << 5)) + (unsigned int)c;
        }
        hashcode %= 769;
        return hashcode;
    }
    // ~hashobj()
    // {
    //     for(auto c:bankStorage2d)
    //     {
    //         cout<<"start"<<endl;
    //         if (c != NULL) delete c;
    //         c = NULL;
    //     }
    // }
};
class Dict
{
private:
    
public:
    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    long long get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
    hashobj para; 
    int empty = 1;
    double score;

    // void update(string word,double c)
    // {
    //     para.updatescore(word,c);
    // }
};