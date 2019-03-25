/*
 * CSc103 Project 3: Game of Life
 * See readme.html for details.
 * Please list all references you made use of in order to complete the
 * assignment: your classmates, websites, etc.  Aside from the lecture notes
 * and the book, please list everything.  And remember- citing a source does
 * NOT mean it is okay to COPY THAT SOURCE.  What you submit here **MUST BE
 * YOUR OWN WORK**.
 * References:
 * Gene (friend)
 * tutorialspoint.com,cplusplus.com,
 * Finally, please indicate approximately how many hours you spent on this:
 * #hours: 40
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

size_t max_gen = 0; /* if > 0, fast forward to this generation. */
string wfilename =  "/tmp/gol-world-current"; /* write state here */
FILE* fworld = 0; /* handle to file wfilename. */
string initfilename = "/tmp/gol-world-current"; /* read initial state from here. */
vector<vector<bool> > g;
vector<vector<bool> > newWorld;

/* NOTE: you don't have to write these functions -- this is just how
 * I chose to organize my code. */
size_t nbrCount(size_t i, size_t j, const vector<vector<bool> >& g);
void update();
int initFromFile(const string& fname); /* read initial state into vectors. */
void mainLoop();
void dumpState(FILE* f,bool omg);

/* NOTE: you can use a *boolean* as an index into the following array
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
	 * are all set according to the command line: */
	//printf("input file:  %s\n",initfilename.c_str());
	//printf("output file: %s\n",wfilename.c_str());
	//printf("fast forward to generation: %lu\n",max_gen);
	/* TODO: comment out 3 lines above once you see what's in them. */
	/* NOTE: if wfilename == "-" you should write to stdout, not a file
	 * named "-"! */

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
	g.push_back(vector<bool>()); //adds new row
	char c;
	size_t row=0;
	while(fread(&c,1,1,f)){
		if(c=='\n'){
			row++;
			g.push_back(vector<bool>());
		}
		else if(c=='.')
			g[row].push_back(false);
		else
			g[row].push_back(true);
	}
	g.pop_back();
	fclose(f);
	return 0;
}

void mainLoop() {
	/* TODO: write this */
	/* update, write, sleep */
	if (max_gen == 0) {
		/* make one generation update per second */
		update();
        dumpState(fworld,true);
        sleep(1);
	}
	else {
		/* go through generations as fast as you can until
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
	newWorld=g;
	for(size_t i=0; i<g.size(); i++){
		for(size_t j=0; j<g[0].size(); j++){
			/* If a live cell has fewer than 2 neighbors, it dies */
			if(g[i][j]==true && nbrCount(i,j,g)<2){
				needsChanging.push_back(i);
				needsChanging.push_back(j);
			}
			/* If a live cell has more than 3 neighbors, it dies */
			else if(g[i][j]==true && nbrCount(i,j,g)>3){
				needsChanging.push_back(i);
				needsChanging.push_back(j);
			}
			/* If a dead cell has 3 neighbors, it becomes alive */
			else if(g[i][j]==false && nbrCount(i,j,g)==3){
				needsChanging.push_back(i);
				needsChanging.push_back(j);
			}
		}
	}
	/* Changes the value of the cell after knowing what cells
	 * to change. */
	for (size_t i=0; i<needsChanging.size(); i+=2){
		if(g[i][i+1]==false){
			newWorld[i][i+1]=true;
		}
		else{
			newWorld[i][i+1]=false;
		}
	}
}

// Counts the neighbors surrounding a grid
size_t nbrCount(size_t i, size_t j,const vector<vector<bool> >& g){
	size_t count = 0;
	size_t iT = g.size();
	size_t jT = g[0].size();
	if(g[(i-1+iT)%iT][(j-1+jT)%jT]==true)
		count++;
	if(g[(i-1+iT)%iT][j]==true)
		count++;
	if(g[(i-1+iT)%iT][(j+1)%jT]==true)
		count++;
	if(g[i][(j-1+jT)%jT]==true)
		count++;
	if(g[i][(j+1)%jT]==true)
		count++;
    if(g[i][j]==true)
		count++;
	if(g[(i+1)%iT][(j-1+jT)%jT]==true)
		count++;
	if(g[(i+1)%iT][j]==true)
		count++;
	if(g[(i+1)%iT][(j+1)%jT]==true)
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
