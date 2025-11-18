#include <cstddef>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

static std::vector<int> buildBadChar(const std::string &pat) {
    const int ALPH = 256;
    std::vector<int> TAB(ALPH, (int)pat.size());
    for (size_t i = 0; i < pat.size() - 1; i++) TAB[(unsigned char)pat[i]] = (int)pat.size() - 1 - i;
    return TAB;
}

static std::vector<int> bm_search_all(const std::string &text, const std::string &pat, int start_s, int end_s) {
    std::vector<int> result;
    int m = (int)pat.size();
    if (m == 0) return result;
    auto TAB = buildBadChar(pat);

    int s = start_s; // start of string // i

    while (s <= end_s/*end_s = n*/) {
        int j = m - 1;
        while (j >= 0 && pat[j] == text[s + j]) --j;

        if (j < 0) { // when result found.
            // match at shift s
            result.push_back(s);
            s += m; // simple shift after a full match
        } else { // shift according to tablet.
            s += TAB[(unsigned char)text[s + j]];
        }
    }
    
    return result;
}

int bm_findFirst(const std::string &text, const std::string &pat) {
    if (pat.empty()) return 0;
    auto matches = bm_search_all(text, pat, 0, (int)text.size() - (int)pat.size());
    return matches.empty() ? -1 : matches.front();
}

std::vector<int> bm_findAll(const std::string &text, const std::string &pat) {
    if ((int)text.size() < (int)pat.size()) return {};
    int end_s = (int)text.size() - (int)pat.size();
    return bm_search_all(text, pat, 0, end_s);
}

std::vector<int> bm_findAllInRange(const std::string &text, const std::string &pat, int start, int end) {
    int n = (int)text.size();
    if (start < 0 || end < 0 || start >= n || end >= n || start > end) {
        throw std::invalid_argument("range out of bounds or invalid");
    }
    int m = (int)pat.size();
    if (m == 0) return {};
    int max_s = end - m + 1;
    if (max_s < start) return {};
    return bm_search_all(text, pat, start, max_s);
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::string txt = "std::move_iterator is an iterator adaptor which behaves exactly like the underlying iterator";
    std::string pat = "tor";

    auto all = bm_findAll(txt,pat);
    std::cout <<"All: ";
    for (int i : all) std::cout << i << " ";
    std::cout << "\n";

    auto range = bm_findAllInRange(txt, pat, 17, 91);
    std::cout <<"From 17 to 91: ";
    for (int i : range) std:: cout << i << " ";
    std::cout << ".\n";

    auto first = bm_findFirst(txt, pat);
    std::cout <<"First : " << first <<  ".\n";


    return 0;
}