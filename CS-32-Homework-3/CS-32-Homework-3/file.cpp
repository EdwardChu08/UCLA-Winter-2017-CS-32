//
//  file.cpp
//  CS-32-Homework-3
//
//  Created by Edward Chu on 10/2/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//

/*
#include <iostream>
#include <string>
using namespace std;
*/

//Your declarations and implementations would go here

class File{
    public:
        File(string name):m_name(name){};
        virtual ~File()=0;
        string name() const{
            return m_name;
        };
        virtual void open() const = 0;
        virtual void redisplay() const{
            cout << "refresh the screen";
        };
        
    private:
        string m_name;
};

File::~File(){};

class TextMsg : public File{
public:
    TextMsg(string name):File(name){};
    virtual ~TextMsg(){
        cout << "Destroying " << name() << ", a text message" << endl;
    }
    virtual void open() const{
        cout << "open text message";
    }
};

class Video : public File{
public:
    Video(string name, int length):File(name), m_length(length){};
    virtual ~Video(){
        cout << "Destroying" << name() << ", a video" << endl;
    }
    virtual void open() const{
        cout << "play " << m_length << " second video";
    }
    virtual void redisplay() const{
        cout << "replay video";
    }
    
private:
    int m_length;
};

class Picture : public File{
public:
    Picture(string name):File(name){};
    virtual ~Picture(){
        cout << "Destroying the picture" << name() << endl;
    }
    virtual void open() const{
        cout << "show picture";
    }
};


/*
void openAndRedisplay(const File* f)
{
    cout << f->name() << ": ";
    f->open();
    cout << endl << "Redisplay: ";
    f->redisplay();
    cout << endl;
}


int main()
{
    File* files[4];
    files[0] = new TextMsg("fromFred.txt");
    // Videos have a name and running time
    files[1] = new Video("goblin.mpg", 3780);
    files[2] = new Picture("kitten.jpg");
    files[3] = new Picture("baby.jpg");
    //files[4] = new File(0);
    
    for (int k = 0; k < 4; k++)
        openAndRedisplay(files[k]);
    
    // Clean up the files before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
        delete files[k];
}
*/
/*
 fromFred.txt: open text message
 Redisplay: refresh the screen
 goblin.mpg: play 3780 second video
 Redisplay: replay video
 kitten.jpg: show picture
 Redisplay: refresh the screen
 baby.jpg: show picture
 Redisplay: refresh the screen
 Cleaning up.
 Destroying fromFred.txt, a text message
 Destroying goblin.mpg, a video
 Destroying the picture kitten.jpg
 Destroying the picture baby.jpg
*/
