/*
 * CSc103 Project 4: Syntax highlighting, part one.
 * See readme.html for details.
 * Please list all references you made use of in order to complete the
 * assignment: your classmates, websites, etc.  Aside from the lecture notes
 * and the book, please list everything.  And remember- citing a source does
 * NOT mean it is okay to COPY THAT SOURCE.  What you submit here **MUST BE
 * YOUR OWN WORK**.
 * References: https://www.youtube.com/watch?v=O-jZJLfXkyk
 * readme.html
 *
 * Finally, please indicate approximately how many hours you spent on this:
 * #hours: 5
 */

#include "fsm.h"
using namespace cppfsm;
#include <vector>
using std::vector;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

// make sure this function returns the old state.  See the header
// file for details.
int cppfsm::updateState(int& state, char c) {
	// TODO:  write this function.
	int oldState = state;
    switch (state) {
        case start:
            if (INSET(c,iddelim))
                state = start;
            else if (INSET(c,ident_st))
                state = scanid;
            else if (c == '/')
                state = readfs;
            else if(INSET(c,num))
                state = scannum;
            else if(c =='"')
                state = strlit;
            break;
        case scanid:
            if (INSET(c,iddelim))
                state = start;
            else if (INSET(c,ident_st))
                state = scanid;
            else if (c =='/')
                state=readfs;
            else if(c == '"')
                state = strlit;
            break;
        case readfs:
            if (INSET(c,ident_st))
                state = scanid;
            else if (c == '/')
                state = comment;
            else if(INSET(c,num))
                state = scannum;
            else if(c == '"')
                state = strlit;
            break;
        case comment:
            break;
        case strlit:
            if (c == '\"')
                state = start;
            else if (c == '\\')
                state = readesc;
            else
                state = strlit;
            break;
        case scannum:
            if (INSET(c,iddelim))
                state = start;
            else if (c == '/')
                state = readfs;
            else if(INSET(c,num))
                state = scannum;
            else
                state = error;
            break;
        case readesc:
            if (INSET(c,escseq))
                state = strlit;
            else
                state = error;
            break;
        case error:
            break;
    }
    return oldState;
}


