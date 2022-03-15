#ifndef SERVER_H
#define SERVER_H

#include <fstream>
#include <string>

using namespace std;

class Movie
{

    public:
        const char* getPath() const;

        bool getCurrentFrame(string &o_lined_frame) const;

    public:
        const char* path = "/home/yonits/Documents/trepointer/Television/someText.txt";
        string frame[5];
};

#endif // SERVER_H