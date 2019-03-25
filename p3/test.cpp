/*
 * CSc103 Project 3: Game of Life
 * See readme.html for details.
 * Please list all references you made use of in order to complete the
 * assignment: your classmates, websites, etc.  Aside from the lecture notes
 * and the book, please list everything.  And remember- citing a source does
 * NOT mean it is okay to COPY THAT SOURCE.  What you submit here **MUST BE
 * YOUR OWN WORK**.
 * References: Big C++, readme.html,
 * cplusplus.com, geeksforgeeks.org,
 * tutorialspoint.com
 * Finally, please indicate approximately how many hours you spent on this:
 * #hours: 21
 */

#include <cstdio>
#include <stdlib.h> // for exit();
#include <getopt.h> // to parse long arguments.
#include <unistd.h> // sleep
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
#include <fstream>
using namespace std;

static const char* usage =
"Usage: %s [OPTIONS]...\n"
"Text-based version of Conway's game of life.\n\n"
"   --seed,-s     FILE     read start state from FILE.\n"
"   --world,-w    FILE     store current world in FILE.\n"
"   --fast-fw,-f  NUM      evolve system for NUM generations and quit.\n"
"   --help,-h              show this message and exit.\n";

string initfilename = "/tmp/gol-world-current"; /* read initial state from here. */
size_t max_gen = 0; /* if > 0, fast forward to this generation. */
string wfilename =  "/tmp/gol-world-current"; /* write state here */
FILE* fworld = 0; /* handle to file wfilename. */
vector<vector<bool> > world;
vector<vector<bool> > newWorld;

/* see the hints section of the readme: to get the basics working,
 * uncomment this next line, and you'll have a suitable test vector
 * to which you can apply the rules. */
// #define WARMUP 1
/*
 * #ifdef WARMUP
 * vector<vector<bool> > world = {
 * #include "res/bglider-small"
 * };
 * #endif */

/* NOTE: you don't have to write these functions -- this is just how
 * I chose to organize my code. */
int initFromFile(const string& fname); /* read initial state into vectors. */
void mainLoop();
void dumpState(FILE* f,bool omg);
size_t nbrCount(size_t r, size_t c,vector<vector<bool> >& world);
void update();

/* NOTE: you can use a boolean as an index into the following array
 * to translate from bool to the right characters: */
//char text[3] = ".O";

int main(int argc, char *argv[]) {
    // define long options
    static struct option long_opts[] = {
        {"seed",    required_argument, 0, 's'},
        {"world",   required_argument, 0, 'w'},
        {"fast-fw", required_argument, 0, 'f'},
        {"help",    no_argument,       0, 'h'},
        {0,0,0,0}
    };
    // process options:
    char c;
    int opt_index = 0;
    while ((c = getopt_long(argc, argv, "hs:w:f:", long_opts, &opt_index)) != -1) {
        switch (c) {
            case 'h':
                printf(usage,argv[0]);
                return 0;
            case 's':
                initfilename = optarg;
                break;
            case 'w':
                wfilename = optarg;
                break;
            case 'f':
                max_gen = atoi(optarg);
                break;
            case '?':
                printf(usage,argv[0]);
                return 1;
        }
    }
    /* NOTE: at this point wfilename initfilename and max_gen
     * are all set according to the command line. */
    /* If you wrote the initFromFile function, call it here: */
		initFromFile(initfilename);
		mainLoop();
    return 0;
}

int initFromFile(const string& fname){
	FILE* f=fopen(fname.c_str(),"rb");
	if(!f){
		cout<<"Error!";
		exit(1);
	}
	world.push_back(vector<bool>()); //adds new row
	char c;
	size_t row=0;
	while(fread(&c,1,1,f)){
		if(c=='\n'){
			row++;
			world.push_back(vector<bool>());
		} else if(c=='.'){
			world[row].push_back(false);
		} else{
			world[row].push_back(true);
		}
	}
	world.pop_back();
	fclose(f);
	return 0;
}

void mainLoop() {
    /* TODO: write this */
    /* update, write, sleep */
    if (max_gen == 0) {
        /* first mode - make one generation update per second */
			update();
			dumpState(fworld,true);
			sleep(1);
    } else {
        /* second mode - go through generations as fast as you can until
         * max_gen is reached... */
			for(size_t i=0; i<=max_gen; i++){
				if(i!=max_gen){
					update();
					dumpState(fworld,false);
				}else{
					update();
					dumpState(fworld,true);
				}
			}
    }
}

void update(){
	vector<size_t> needsChanging;
	newWorld=world;
	for(size_t i=0; i<world.size(); i++){
		for(size_t j=0; j<world[0].size(); j++){
			/* If a live cell has fewer than 2 neighbors, it dies */
			if(world[i][j]==true && nbrCount(i,j,world)<2){
				needsChanging.push_back(i);
				needsChanging.push_back(j);
			}
			/* If a live cell has more than 3 neighbors, it dies */
			else if(world[i][j]==true && nbrCount(i,j,world)>3){
				needsChanging.push_back(i);
				needsChanging.push_back(j);
			}
			/* If a dead cell has 3 neighbors, it becomes alive */
			else if(world[i][j]==false && nbrCount(i,j,world)==3){
				needsChanging.push_back(i);
				needsChanging.push_back(j);
			}
		}
	}
	/* Changes the value of the cell after knowing what cells
	 * to change. */
	for (size_t i=0; i<needsChanging.size(); i+=2){
		if(world[i][i+1]==false){
			newWorld[i][i+1]=true;
		}
		else{
			newWorld[i][i+1]=false;
		}
	}
}

// Counts the neighbors surrounding a grid
size_t nbrCount(size_t r, size_t c,vector<vector<bool> >& world){
	size_t count = 0;
	size_t rT = world.size();
	size_t cT = world[0].size();
	if(world[(r-1+rT)%rT][(c-1+cT)%cT]==true)
		count++;
	if(world[(r-1+rT)%rT][c]==true)
		count++;
	if(world[(r-1+rT)%rT][(c+1)%cT]==true)
		count++;
	if(world[r][(c-1+cT)%cT]==true)
		count++;
	if(world[r][(c+1)%cT]==true)
		count++;
    if(world[r][c]==true)
		count++;
	if(world[(r+1)%rT][(c-1+cT)%cT]==true)
		count++;
	if(world[(r+1)%rT][c]==true)
		count++;
	if(world[(r+1)%rT][(c+1)%cT]==true)
		count++;
	return count;
}

void dumpState(FILE* f,bool omg){
	f=fopen(wfilename.c_str(),"wb");
	for(size_t i=0; i<newWorld.size(); i++){
		for(size_t j=0; j<newWorld[0].size(); j++){
			if(newWorld[i][j]==true){
				char c = 'O';
				fwrite(&c,1,1,f);
			}
			else{
				char c = '.';
				fwrite(&c,1,1,f);
			}
		}
		char c = '\n';
		fwrite(&c,1,1,f);
	}
	rewind(f);
	if(omg){
		fclose(f);
	}
}