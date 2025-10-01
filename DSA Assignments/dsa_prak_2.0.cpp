#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>



// Preprocess bad-character rule (for ASCII bytes)
static std::vector<int> buildBadChar(const std::wstring &pat) {
    const int ALPH = 256;
    std::vector<int> bad(ALPH, -1);
    for (int i = 0; i < (int)pat.size(); ++i) bad[(unsigned char)pat[i]] = i;
    return bad;
}

// Preprocess good-suffix rule
// suffix[k] = the starting index of the longest suffix of pat[0..k-1] that is also a suffix of whole pattern
// prefix[k] = true if pat[k..m-1] is a prefix of pat
static void buildGoodSuffix(const std::wstring &pat, std::vector<int> &suffix, std::vector<bool> &prefix) {
    int m = pat.size();
    suffix.assign(m, -1);
    prefix.assign(m, false);
    for (int i = 0; i < m - 1; ++i) {
        int j = i, k = 0; // try to match suffix ending at i
        while (j >= 0 && pat[j] == pat[m - 1 - k]) {
            --j; ++k;
            suffix[k] = j + 1; // substring pat[j+1 .. i] matches suffix of length k
        }
        if (j == -1) prefix[k] = true; // whole prefix of length k matches suffix
    }
}

// compute shift using good-suffix rule when mismatch at position j (0-based)
static int moveByGoodSuffix(int j, int m, const std::vector<int> &suffix, const std::vector<bool> &prefix) {
    int k = m - 1 - j; // length of matched suffix
    if (k == 0) return 0; // no suffix matched -> rely on bad-character
    // Case 1: there is a suffix of pat[0..j] that matches suffix of pat
    if (suffix[k] != -1) return j + 1 - suffix[k];
    // Case 2: find smallest r > j such that pat[r..m-1] is prefix
    for (int r = j + 2; r <= m - 1; ++r) {
        if (prefix[m - r]) return r;
    }
    return m; // default shift
}

// Core search that finds all occurrences in text between allowed s in [0 .. n-m] (caller restricts range)
static std::vector<int> bm_search_all(const std::wstring &text, const std::wstring &pat, int start_s, int end_s) {
    std::vector<int> result;
    int m = (int)pat.size();
    if (m == 0) return result; // treat empty pattern as no-op per assignment (or decide differently)
    auto bad = buildBadChar(pat);
    std::vector<int> suffix; std::vector<bool> prefix;
    buildGoodSuffix(pat, suffix, prefix);

    int s = start_s;
    while (s <= end_s) {
        int j = m - 1;
        // compare from right to left
        while (j >= 0 && pat[j] == text[s + j]) --j;
        if (j < 0) {
            // match at shift s
            result.push_back(s);
            // shift by amount determined by good-suffix (case when full match)
            int shift = m;
            // We can try to use the good-suffix shift for full match:
            // If prefix[?] true then shift = m - longest prefix that is suffix
            for (int r = 1; r <= m - 1; ++r) {
                if (prefix[r]) { shift = m - r; break; }
            }
            // If none, shift = m
            s += std::max(1, shift);
            // NOTE: Galil optimization point: we could remember how many characters of the pattern were matched
            // and avoid re-checking them on the next alignment if shift < matchedLength.
        } else {
            int bc = j - bad[(unsigned char)text[s + j]];
            int gs = moveByGoodSuffix(j, m, suffix, prefix);
            int shift = std::max(1, std::max(bc, gs));
            s += shift;
        }
    }
    return result;
}

// 1) index of first occurrence, return -1 if not found
int bm_findFirst(const std::wstring &text, const std::wstring &pat) {
    if (pat.empty()) return 0; // choose convention: first index 0
    auto matches = bm_search_all(text, pat, 0, (int)text.size() - (int)pat.size());
    return matches.empty() ? -1 : matches.front();
}

// 2) indices of all occurrences (entire text)
std::vector<int> bm_findAll(const std::wstring &text, const std::wstring &pat) {
    if ((int)text.size() < (int)pat.size()) return {};
    int end_s = (int)text.size() - (int)pat.size();
    return bm_search_all(text, pat, 0, end_s);
}

// 3) indices of occurrences inside inclusive range [start, end]
// Throws std::invalid_argument if range invalid (your requested policy)
std::vector<int> bm_findAllInRange(const std::wstring &text, const std::wstring &pat, int start, int end) {
    int n = (int)text.size();
    if (start < 0 || end < 0 || start >= n || end >= n || start > end) {
        throw std::invalid_argument("range out of bounds or invalid");
    }
    int m = (int)pat.size();
    if (m == 0) return {}; // convention: no matches for empty
    // The leftmost shift s we can test is start; the maximal s is end - m + 1 (must fit fully)
    int max_s = end - m + 1;
    if (max_s < start) return {}; // pattern cannot fit inside the inclusive range
    return bm_search_all(text, pat, start, max_s);
}

int main() {
    setlocale(LC_ALL,"");

    std::wstring txt = L"во дворе трава на траве дрова";
    std::wstring pat = L"дрова";
    auto a = bm_findAll(txt, pat);
    for (int i : a) std::cout << i << " ";
    std::cout << "\n";
    // auto b = bm_findAllInRange(txt, pat, 17, 91);
    // for (int i : b) std::cout << i << " ";
    // std::cout << "\n";
    return 0;
}

