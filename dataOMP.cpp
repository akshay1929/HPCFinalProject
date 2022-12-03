#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <locale>
#include <set>
#include <sstream>
#include <chrono>
#include <ctime>
#include <omp.h>
using namespace std;

vector<string> getData(string name) {
    vector<string> wordList;
    string line;
    ifstream myfile (name + ".txt");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            transform(line.begin(), line.end(), line.begin(), ::tolower);
            wordList.push_back(line);
        }
        myfile.close();
    }
    else cout << "Unable to open file"; 
    return wordList;
}
void tokenize(std::string const &str, const char delim, std::vector<std::string> &out)
{
    stringstream ss(str);
    string s;
    while (getline(ss, s, delim)) {
        out.push_back(s);
    }
}

int main(int ac, char *av[]) {

    int length = 0;
    if (ac > 1) { // if user entered a value after the prog name, parse it

        length = std::atoi(av[1]);    
    }
    
    vector<string> goodWords = getData("good");
    vector<string> badWords = getData("bad");
    vector<string> racismWords = getData("racism");
    vector<string> article = getData("article" + to_string(length));

    cout << article.size() << '\n';
    int block = 1;
    int good = 0;
    int bad = 0;
    int covid = 0;
    int racism = 0;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    #pragma omp parallel for collapse(2) 
    {
    for (int i = 0; i < article.size(); i++) {
        const char delim = ' ';
        std::vector<std::string> out;
        tokenize(article[i], delim, out);
        #pragma omp parallel for
        for(int j = 0; j < out.size(); j++) {
            //Good word count
            int upper = goodWords.size() - 1;
            for (int lower = 0; lower <= upper;) {
                int mid = lower + (upper - lower) / 2;
                if (out[j] == (goodWords[mid])) {
                    good++;
                    break;
                }
                if (out[j] > (goodWords[mid]))
                    lower = mid + 1;
                else
                    upper = mid - 1;
            }
            
            //Bad word count
            upper = badWords.size() - 1;
            for (int lower = 0; lower <= upper;) {
                int mid = lower + (upper - lower) / 2;
                if (out[j] == (badWords[mid])) {
                    bad++;
                    break;
                }
                if (out[j] > (badWords[mid]))
                    lower = mid + 1;
                else
                    upper = mid - 1;
            }
            
            //Racism word count
            upper = racismWords.size() - 1;
            for (int lower = 0; lower <= upper;) {
                int mid = lower + (upper - lower) / 2;
                if (out[j] == (racismWords[mid])) {
                    racism++;
                    break;
                }
                if (out[j] > (racismWords[mid]))
                    lower = mid + 1;
                else
                    upper = mid - 1;
            }
        }
        block++;
    }
    }
    
    // for (int i = 0; i < article.size(); i += size) {
    //     int good = 0;
    //     int bad = 0;
    //     int covid = 0;
    //     int racism = 0;
    //     #pragma omp parallel for
    //     for (int art = i; art < i + size; art++) {
    //         const char delim = ' ';
    //         std::vector<std::string> out;
    //         tokenize(article[art], delim, out);
    //         #pragma omp parallel for
    //         for(int i = 0; i < out.size(); i++) {
    //             //Good word count
    //             for (int iter = 0; iter < goodWords.size(); iter++) {
    //                 if (out[i] == goodWords[iter]) {
    //                     good++;
    //                 } 
    //             }
    //             //Bad word count
    //             for (int iter = 0; iter < badWords.size(); iter++) {
    //                 if (out[i] == badWords[iter]) {
    //                     bad++;
    //                 } 
    //             }
    //             //Racism word count
    //             for (int iter = 0; iter < racismWords.size(); iter++) {
    //                 if (out[i] == racismWords[iter]) {
    //                     racism++;
    //                 } 
    //             }
    //         }
    //     }
    //     block++;
    // }
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
    
    return 0;
}