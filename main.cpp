/*
 * main.cpp
 */

#include "tokens.h"
#include "parse.h"
#include <fstream>
using namespace std;

map<string, Value> evars;

void RunTimeError(string msg)
{
    cout << "0: RUNTIME ERROR " << msg << endl;
    exit(1);
}

int
main(int argc, char *argv[])
{
	ifstream file;
	istream *in;
	int linenum = 0;

	if( argc == 1 ) {
		in = &cin;
	}
    
	else if( argc == 2 ) {
		file.open(argv[1]);
		if( file.is_open() == false ) {
			cout << "COULD NOT OPEN " << argv[1] << endl;
			return 1;
		}
		in = &file;
	}

	else {
		cout << "TOO MANY FILENAMES" << endl;
		return 1;
	}

	ParseTree *prog = Prog(in, &linenum);

	if( prog == 0 )
    {
		return 0;
    }
    
    prog->Eval(evars);
    return 0;

    
    /*
	cout << "LEAF COUNT: " << prog->LeafCount() << endl;
	cout << "STRING COUNT: " << prog->StringCount() << endl;
	int idc = prog->IdentCount();
	if( idc ) {
		cout << "IDENT COUNT: " << idc << endl;
		map<string,int> ids;
		prog->GetVars(ids);

		// find max
		int maxval = ids.begin()->second;
		for( const auto& s : ids )
			if( s.second > maxval )
				maxval = s.second;
		bool printed = false;
		for( auto s : ids ) {
			if( s.second != maxval )
				continue;
			if( printed )
				cout << ", ";
			cout << s.first;
			printed = true;
		}
		cout << endl;
	}
	return 0;
    */
}