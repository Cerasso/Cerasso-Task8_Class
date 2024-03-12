#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

struct Document {
    int id;
    int relevance;
};
bool HasDocumentGreaterRelevance(const Document& lhs, const Document& rhs) {
        return lhs.relevance > rhs.relevance;
    }
class SearchServer {
private:
    struct DocumentContent {
        int id = 0;
        vector<string> words;
    };

    vector<DocumentContent> documents_;
    set<string> stop_words_;

    vector<string> SplitIntoWords(const string& text) {
        vector<string> words;
        string word;
        for (const char c : text) {
            if (c == ' ') {
                if (!word.empty()) {
                    words.push_back(word);
                    word.clear();
                }
            } else {
                word += c;
            }
        }
        if (!word.empty()) {
            words.push_back(word);
        }

        return words;
    }
    set<string> ParseStopWords(const string& text) {
        set<string> stop_words;
        for (const string& word : SplitIntoWords(text)) {
            stop_words.insert(word);
        }
        return stop_words;
    }
    vector<string> SplitIntoWordsNoStop(const string& text, const set<string>& stop_words) {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (stop_words.count(word) == 0) {
                words.push_back(word);
            }
        }
        return words;
    }

public:
    string ReadLine() {
        string s;
        getline(cin, s);
        return s;
    }
    int ReadLineWithNumber() {
        int result = 0;
        cin >> result;
        ReadLine();
        return result;
    }
    set<string> ParseQuery(const string& text, const set<string>& stop_words) {
        set<string> query_words;
        for (const string& word : SplitIntoWordsNoStop(text, stop_words)) {
            query_words.insert(word);
        }
        return query_words;
    }

    void AddDocument(int document_id, const string& document){
        const vector<string> words = SplitIntoWordsNoStop(document, stop_words_);
        documents_.push_back({document_id, words});
    }

    void SetStopWords(const string& text) {
        stop_words_ = ParseStopWords(text);
    }

    vector<Document> FindAllDocuments(const vector<DocumentContent>& documents,
                                  const set<string>& query_words) {
        vector<Document> matched_documents;
        for (const auto& document : documents) {
            const int relevance = MatchDocument(document, query_words);
            if (relevance > 0) {
                matched_documents.push_back({document.id, relevance});
            }
        }
        return matched_documents;
    }
    int MatchDocument(const DocumentContent& content, const set<string>& query_words) {
        if (query_words.empty()) {
            return 0;
        }
        set<string> matched_words;
        for (const string& word : content.words) {
            if (matched_words.count(word) != 0) {
                continue;
            }
            if (query_words.count(word) != 0) {
                matched_words.insert(word);
            }
        }
        return static_cast<int>(matched_words.size());
    }
    
    vector<Document> FindTopDocuments(const string& raw_query) {

        const set<string> query_words = ParseQuery(raw_query, stop_words_);
        vector<Document> matched_documents = FindAllDocuments(documents_, query_words);
        sort(matched_documents.begin(), matched_documents.end(), HasDocumentGreaterRelevance);
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }    
};

SearchServer CreateSearchServer(){
    
    SearchServer search_server;

    search_server.SetStopWords(search_server.ReadLine());

    const int document_count = search_server.ReadLineWithNumber();

    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, search_server.ReadLine());
    }
    return search_server;
}

int main() {
    SearchServer server = CreateSearchServer();
        for (auto [id, relevance] : server.FindTopDocuments(server.ReadLine())) {
        cout << "{ document_id = "s << id << ", relevance = "s << relevance << " }"s << endl;
    }
}