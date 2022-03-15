#ifndef SERVER_H
#define SERVER_H

#include <fstream>
#include <string>

using namespace std;

class Movie
{
    /*                  Ideas to the future:     
        A movie file will enumerate each frame -> user can stop
        and slide within the movie.

        Movie class has to manage the current place in the movie.
    */
    public:
        const char* getPath() const;
        bool hasMoreFrames() const;
        bool getCurrentFrame(string &o_lined_frame) const;

    public:
        const char* m_path = "/home/yonits/Documents/trepointer/Television/someText.txt";
        string m_frame[5];
};

#endif // SERVER_H