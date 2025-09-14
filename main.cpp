#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <functional>

using namespace std;

/*
 * Configuration: number of digits to analyze.
 * Change DIGITS to explore different digit lengths.
 * Note: maximum is 10 (since we only have 10 unique digits).
 */
const int DIGITS = 4;

/*
 * Helper: Normalize a cycle by rotating it
 * so the smallest element appears first.
 * This ensures uniqueness when comparing cycles.
 */
vector<string> normalize_cycle(const vector<string>& cycle) {
    if (cycle.empty()) return cycle;
    auto it = min_element(cycle.begin(), cycle.end());
    vector<string> normalized;
    for (auto i = it; i != cycle.end(); i++) normalized.push_back(*i);
    for (auto i = cycle.begin(); i != it; i++) normalized.push_back(*i);
    return normalized;
}

/*
 * Convert a cycle into a string key, e.g.,
 * [1234, 4321, 3087] -> "1234-4321-3087-"
 * Used for hashing cycles in maps.
 */
string cycle_to_key(const vector<string>& cycle) {
    string key;
    for (auto& s : cycle) key += s + "-";
    return key;
}

/*
 * Build descending and ascending numbers from digit histogram.
 * Example: histogram of {1,1,2} digits gives:
 *   desc = "211"
 *   asc  = "112"
 */
string make_desc(const array<int, 10> &hist) {
    string res;
    for (int d = 9; d >= 0; d--) res.append(hist[d], char('0' + d));
    return res;
}
string make_asc(const array<int, 10> &hist) {
    string res;
    for (int d = 0; d <= 9; d++) res.append(hist[d], char('0' + d));
    return res;
}

/*
 * Subtract two non-negative integers represented as strings.
 * Both inputs must be of the same length (padding applied earlier).
 * Example: "4321" - "1234" = "3087"
 */
string subtract_strings(string a, string b) {
    while (b.size() < a.size()) b.insert(0, "0");
    int n = a.size();
    int carry = 0;
    string res(n, '0');

    for (int i = n - 1; i >= 0; i--) {
        int x = (a[i] - '0') - (b[i] - '0') - carry;
        if (x < 0) { x += 10; carry = 1; }
        else carry = 0;
        res[i] = char('0' + x);
    }
    size_t pos = res.find_first_not_of('0');
    return (pos == string::npos) ? "0" : res.substr(pos);
}

/*
 * Perform one step of Kaprekar's routine:
 * 1. Build histogram of digits
 * 2. Create descending and ascending numbers
 * 3. Subtract (desc - asc)
 * 4. Pad with zeros to maintain digit count
 */
string kaprekar_step(const string& num, int digits) {
    array<int, 10> hist = {0};
    for (char c : num) hist[c - '0']++;

    string big = make_desc(hist);
    string small = make_asc(hist);

    while (big.size() < digits) big.insert(0, "0");
    while (small.size() < digits) small.insert(0, "0");

    string res = subtract_strings(big, small);
    while (res.size() < digits) res.insert(0, "0");
    return res;
}

/*
 * Given a starting number, repeatedly apply Kaprekar steps
 * until either:
 *  - A cycle is detected
 *  - The result becomes "0"
 * Returns the cycle or {"0"} if it ends in zero.
 */
vector<string> find_kaprekar_cycle(const string& start, vector<string>& steps) {
    set<string> seen;
    string current = start;
    int digits = start.size();

    while (true) {
        if (seen.count(current)) {
            // cycle detected
            auto it = find(steps.begin(), steps.end(), current);
            vector<string> cycle(it, steps.end());
            return cycle;
        }
        steps.push_back(current);
        seen.insert(current);

        current = kaprekar_step(current, digits);
        if (current == "0") return {"0"};
    }
}

/*
 * Generate all unique-digit numbers with given length.
 * Example for DIGITS=3:
 *   123, 132, 145, 987, ...
 */
void generate_numbers(int digits, vector<string>& results) {
    function<void(string, int, set<int>&)> dfs = [&](string cur, int rem, set<int>& used) {
        if (rem == 0) {
            if (cur[0] != '0') results.push_back(cur); // skip leading zero
            return;
        }
        for (int d = 0; d <= 9; d++) {
            if (!used.count(d)) {
                used.insert(d);
                dfs(cur + char('0' + d), rem - 1, used);
                used.erase(d);
            }
        }
    };

    set<int> used;
    dfs("", digits, used);
}

/*
 * Main program:
 * 1. Generate all unique-digit numbers of length DIGITS
 * 2. For each number, find its Kaprekar cycle
 * 3. Group numbers by resulting cycle
 * 4. Print summary: cycles found, counts, sample inputs
 */
int main() {
    if (DIGITS > 10) {
        cout << "Maximum 10 digits allowed (since only 10 unique digits exist).\n";
        return 1;
    }

    cout << "Kaprekar analysis for " << DIGITS << "-digit numbers (unique digits only)\n";
    cout << "=======================================================\n";

    vector<string> all_numbers;
    generate_numbers(DIGITS, all_numbers);
    cout << "Total numbers generated: " << all_numbers.size() << "\n\n";

    unordered_map<string, vector<string>> cycle_to_inputs;
    unordered_map<string, string> key_to_display;

    int counter = 0;
    for (auto& num : all_numbers) {
        vector<string> steps;
        auto cycle = find_kaprekar_cycle(num, steps);

        auto norm = normalize_cycle(cycle);
        string key = cycle_to_key(norm);

        cycle_to_inputs[key].push_back(num);
        if (!key_to_display.count(key)) {
            string disp;
            for (auto& c : norm) disp += c + " -> ";
            if (!disp.empty()) disp.erase(disp.size() - 4); // remove last arrow
            key_to_display[key] = disp;
        }

        if (++counter % 5000 == 0) {
            cout << "Processed: " << counter << " numbers...\n";
        }
    }

    cout << "\nFinal Results:\n";
    int cycle_id = 1;
    for (auto& kv : cycle_to_inputs) {
        cout << "Cycle " << cycle_id++ << ": " << key_to_display[kv.first] << "\n";
        cout << "  Count of inputs: " << kv.second.size() << "\n";
        cout << "  Sample inputs: ";
        for (int i = 0; i < min<int>(5, kv.second.size()); i++)
            cout << kv.second[i] << " ";
        cout << "\n\n";
    }

    return 0;
}
