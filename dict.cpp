// Do NOT add any other includes
#include "dict.h"
Dict::Dict()
{
    // Implement your function here

   
}

Dict::~Dict()
{

}

void lower(string &str)
{
    for (char &c : str) {
        if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + 'a';
        }
    }
}
void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    string seperators = " .,-:!\"\'()?—[]“”‘’˙;@";
    lower(sentence);
    string s = "";
    for (int i = 0; i < sentence.size(); i++)
    {
        int endmark = 0;
        for (int j = 0; j < seperators.size(); j++)
        {
            if (sentence[i] != seperators[j])
            {
                continue;
            }
            else
            {
                endmark = 1;
            }
        }
        if (endmark == 1)
        {
            para.addstr(s, book_code, page, paragraph, sentence_no);
            s = "";
        }
        else
        {
            s += sentence[i];
        }
    }
    if(s.size()!=0)para.addstr(s, book_code, page, paragraph, sentence_no);
}
long long Dict::get_word_count(string word)
{
    return para.returnct(word);
}
void inorder(SymNode *temp, std::fstream &outfile)
{
    if (temp == NULL)
    {
        return;
    }
    inorder(temp->left, outfile);
    if (temp->key != " " && temp->key != "") outfile << temp->key << ", " << temp->count << "\n";
    inorder(temp->right, outfile);
}
void Dict::dump_dictionary(string filename)
{
    std::fstream outfile(filename,ios_base::app);
    if (outfile.is_open())
    {
        for (int i = 0; i < para.bankStorage2d.size(); i++)
        {

            SymbolTable *j = para.bankStorage2d[i]; 
            SymNode *temp = j->root;
            if (temp != NULL) {inorder(temp, outfile);}
        }
        ////cout<<"Done?"<<endl;
        outfile.close();
    }
}

// int main()
// {
//     Dict d;
//     cout<<d.paras.size()<<endl;

// }