#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

QNA_tool::QNA_tool(){
    ifstream file("unigram_freq.csv");
    if (!file.is_open()) {
        std::cerr << "ERROR" << std::endl;
    }
    string line;
    int i = 0;
    while (std::getline(file, line)) {
        i++;
        if (i ==1) continue;
        std::istringstream iss(line);
        std::string val1, val2;

        if (std::getline(iss, val1, ',') && std::getline(iss, val2, ',')) {
            
           refwords.insert(val1,stoll(val2));
        }
    }
    file.close(); 
    
}



QNA_tool::~QNA_tool(){
    // Implement your function here  
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here
    
    dict.insert_sentence(book_code,page,paragraph,sentence_no,sentence);
    
    if((paragraph != currentpara)||(book_code != currentbook)||(page != currentpage))
    {
        Dict tempdict;
        tempdict.insert_sentence(book_code,page,paragraph,sentence_no,sentence);
        paradict temp;
        temp  = {book_code,page,paragraph,tempdict};
        paradata.push_back(temp);
        // cout<<currentbook<<" "<<currentpage<<" "<<currentpara<<" "<<endl;
        if((currentpara != paragraph))currentpara = paragraph;
        if((currentpage != page))currentpage = page;
        if(currentbook != book_code) currentbook = book_code;
    }
    else if((paragraph == currentpara)&&(book_code == currentbook)&&(page == currentpage))
    {
        paradata.back().pdict.insert_sentence(book_code,page,paragraph,sentence_no,sentence);
    }
    
    
    return;

}
void lower2(string &str)
{
    for (char &c : str) {
        if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + 'a';
        }
    }
}
void insertathead(parainfo topk, Node *&tail,
                  Node *&head)
{
    if (head == NULL)
    {
        Node *temp = new Node(topk.book_code, topk.page, topk.paragraph, -1, -1);
        head = temp;
        tail = temp;
        head->right = tail;
        tail->left = head;
        tail->right = nullptr;
    }
    else
    {
        Node *temp = new Node(topk.book_code, topk.page, topk.paragraph, -1, -1);
        head->left = temp;
        temp->right = head;
        temp->left = nullptr;
        head = temp;

    }
}
Node* QNA_tool::get_top_k_para(string question, int k) {
//    cout<<"size   "<<paradata.size()<<endl;
    for(auto para:paradata)
            {
               
                para.pdict.score = 0;
                string seperators = " .,-:!\"\'()?—[]“”‘’˙;@";
                lower2(question);
                string s = "";
                // cout<<"page  ";
                // cout<<page<<endl;
                // cout<<"para  ";
                // cout<<index<<endl;
                double score = 0;
                for (int i = 0; i < question.size(); i++)
                {
                    int endmark = 0;
                    for (int j = 0; j < seperators.size(); j++)
                    {
                        if (question[i] != seperators[j])
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
                        score +=  static_cast<double>(para.pdict.get_word_count(s))*(static_cast<double>(static_cast<double>(dict.get_word_count(s)+1)/static_cast<double>(refwords.search(s)+1)));
                        
                        s = "";
                    }
                    else
                    {
                        s += question[i];
                    }
                }
                if(s.size()!=0)score +=  static_cast<double>(para.pdict.get_word_count(s))*(static_cast<double>(static_cast<double>(dict.get_word_count(s)+1)/(refwords.search(s)+1)));
                para.pdict.score = score;
                parainfo temp;
                temp.book_code = para.bookcode;
                temp.page = para.page;
                temp.paragraph = para.paragraph;
                temp.score = para.pdict.score;
                klarge(topk,temp,k);
    
   
}
 Node* tail = nullptr;
    Node* head = nullptr;
    for(int i = 0;i < k;i++)
    {
        parainfo temp = topk[0];
        // cout<<temp.page<<endl;
        // cout<<temp.paragraph<<endl;
        // cout<<temp.score<<endl;
        deleteTop(topk);
        insertathead(temp,tail,head);
    }
    return head;
}

void QNA_tool::query(string question, string filename){
    // Implement your function here  
    const std::string wordsToRemove[] = {"is", "are", "the", "of", "a", "an", "has", "have", "be", "been", "had", "were"};
    // Split the input string into words
    std::istringstream iss(question);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }
    // Remove specified words
    std::vector<std::string> modifiedWords;
    for (const auto& w : words) {
        bool removeWord = false;
        for (const auto& remove : wordsToRemove) {
            if (w == remove) {
                removeWord = true;
                break;
            }
        }
        if (!removeWord) {
            modifiedWords.push_back(w);
        }
    }
    // Combine the remaining words back into a string
    std::ostringstream oss;
    for (const auto& w : modifiedWords) {
        oss << w << " ";
    }
    std::string result = oss.str();

    // Remove trailing space
    if (!result.empty()) {
        result.pop_back();
    }
    std::istringstream iss1(question);
    std::string word1;
    int k = 6;  // Default value
    while (iss1 >> word1) {
        if (word1 == "what" || word1 == "why" || word1 == "how") {
            k = 10;
            break;  // Exit the loop once a match is found
        } else if (word1 == "where" || word1 == "when" || word1 == "who") {
            k = 4;
            break;  // Exit the loop once a match is found
        }
    }
    Node* x = get_top_k_para(result, k);
    std::cout << "Q: " << question << std::endl;
    query_llm("api_call.py", x, k, "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", question);
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question){

    // first write the k paragraphs into different files

    Node* traverse = root;
    int num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}
// int main()
// {
//     QNA_tool q;

    
//     q.insert_sentence(1,1,1,1,"the the the");
//     q.insert_sentence(1,1,0,0,"the and the");
//     q.insert_sentence(1,1,2,2,"sds");

//     Node* temp = q.get_top_k_para("the",3);

//     for(int i = 0; i < 3;i++)
//     {
//         cout<<temp->paragraph<<endl;
//         temp = temp->right;
//     }
// }