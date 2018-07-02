#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

// Scoring values
#define GAP_SCORE -5
#define MISMATCH -1
#define MATCHING 2

/*
 * Packages the score and the instruction string that the align function 
 * returns.
 */
struct align_result {
    int score;      // Score of this alignment
    string inst;    // Instruction on how to align the inputs

    align_result(int s, string i) {
        this->score = s;
        this->inst = i;
    }
    align_result() {
        this->score = 0;
        this->inst = "";
    }

    // Define the equals operation for two align_result objects 
    align_result operator=(const align_result& a)
    {
        score = a.score;
        inst = a.inst;
        return *this;
    }

    // Define the addition operation for two align_result objects 
    align_result operator+(const align_result& a) 
    {
        return align_result(score + a.score, inst + a.inst);
    }
};

// The memo_type dictionary memoizes the strings and their align_struct objects. 
typedef unordered_map<string, align_result> memo_type;

/*
 * This function returns the align_struct with the greatest score. 
 */ 
align_result findMax(const align_result& a, const align_result& b,
                        const align_result& c) {
    if (c.score >= b.score && c.score >= a.score) {
        return c;
    }
    else if (b.score >= c.score && b.score >= a.score) {
        return b;
    }
    else {
        return a;
    }
}

/*
 * This function takes two strings, s and t, and produces an align_result
 * of the highest alignment score and its corresponding instruction str.
 */
align_result align(string s, string t, memo_type &memo) {
    string key = s + "," + t;
    align_result answer, a, b, c;
    int tsize, ssize;

    // If this result is memoized, use the recorded result.
    if (memo.count(key) > 0){
        return memo[key];
    }
    // Handle the base case when both strings have no characters.
    else if (s.length() == 0 && t.length() == 0) {
         return answer;
    }
    /*
     * If s is empty, then fill the instruction string with 't' and increase
     * the answer's score by GAP_SCORE * t_length()
     */
    else if (s.length() == 0) {
        for (unsigned int i = 0; i < t.length(); i++) {
            answer.inst += "t";
            answer.score += GAP_SCORE;
        }
    }
    /*
     * If t is empty, then fill the instruction string with 's' and increase
     * the answer's score by GAP_SCORE * s_length()
     */
    else if (t.length() == 0) {
        for (unsigned int i = 0; i < s.length(); i++) {
            answer.inst += "s";
            answer.score += GAP_SCORE;
        }
    }
    /*
     * If the strings are the same, then fill the instruction string with 
     * '|' and increase the answer's score by MATCHING * s.length()
     */
    else if (s == t) {
        for (unsigned int i = 0; i < s.length(); i++) {
            answer.inst += "|";
            answer.score += MATCHING;
        }
    }
    /*
     * Else if both strings have length 1, fill the instruction string with 
     * '*' and increase the score by MISMATCH
     */
    else if (s.length() == 1 and t.length() == 1) {
        answer.inst += "*";
        answer.score += MISMATCH;
    }
    /*
     * Else, recursively call align on three possible configurations
     * of aligning the strings s and t.
     */ 
    else {
        tsize = t.length() - 1;
        ssize = s.length() - 1;

        a = align("", t.substr(0, 1), memo) + align(s, t.substr(1, tsize), memo);
        b = align(s.substr(0, 1), "", memo) + align(s.substr(1, ssize), t, memo);
        c = align(s.substr(0, 1), t.substr(0, 1), memo) +
               align(s.substr(1, ssize), t.substr(1, tsize), memo);
        answer = findMax(a, b, c);
    }

    // Before returning the calculated align_result object, memoize it.
    memo[key] = answer;
    return answer;
}

/*
 * This is a wrapper function to print the results of align.
 */
void DNA_align(string s, string t) {
    cout << endl << "Calling DNA align on strings: " << s << ", " << t << endl;

    // Create the dictionary to memoize align_result objects.
    memo_type memo;

    align_result answer = align(s, t, memo);
    string ans = answer.inst;
    int score = answer.score;

    // Print string s
    string line1 = "";
    // Print string t 
    string line2 = "";
    // Print description of the relationship between s and t here (* | s t)
    string line3 = "";

    int j = 0;      // Running index in s
    int k = 0;      // Running index in t

    for (unsigned int m = 0; m < ans.length(); m++) {
        // The i is the next element in our instruction string ans
        string i = ans.substr(m, 1);

        // Only in s
        if(i.compare("s") == 0){
            line1 += s[j]; j++;
            line2 += " ";
            line3 += "s";
        }

        // Only in t
        else if (i.compare("t") == 0){
            line1 += " ";
            line2 += t[k]; k++;
            line3 += "t";
        }

        // Mismatch
        else if (i.compare("*") == 0){
            line1 += s[j]; j++;
            line2 += t[k]; k++;
            line3 += "*";
        }

        // Match
        else {
            line1 += s[j]; j++;
            line2 += t[k]; k++;
            line3 += "|";
        }
    }
    cout << line1 << endl << line2 << endl << line3 << endl;
    cout << "Score for this alignment: " << score << endl;
}

int main(){
    DNA_align("",   "a");
    DNA_align("a",   "");
    DNA_align("a", "b");
    DNA_align("a", "a");
    DNA_align("b",  "ba");
    DNA_align("b",  "ab");
    DNA_align("ab", "b");
    DNA_align("ba", "b");
    DNA_align("ab", "ba");
    DNA_align("abc", "ac");
    DNA_align("abc", "adc");
    DNA_align("ACTGGCCGT", "TGACGTAA");
    DNA_align("abracadabra", "avada kedavra");
    return 0;
}
