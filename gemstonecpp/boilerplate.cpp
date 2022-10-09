#include "boilerplate.h"
#include <stdio.h>
#include <stdlib.h>
#include <cctype>
namespace Gemstone{

	struct arg_t{
		char arg;
		int indx;
	};

	// only change things here
	static const char * const help[NUM_ARGS][2] = {{"-o", "set the output file"},
												   {"-p", "set the palette"}};
	static const struct arg_t map[NUM_ARGS] =  {{'o', PATH},
												{'p', PALETTE}};
	//
	//

	int args[NUM_ARGS];





	void parseArgs(int argc, const char *argv[]){
		for(int i = 0; i < NUM_ARGS; i++)
			args[i] = EMPTY;

		for(int i = 0; i < argc; i++){
			if(argv[i][0] == '-'){
				char arg = std::tolower(argv[i][1]);
				if(arg == 'h'){
					printHelp();
					exit(0);
				}
				for(int j = 0; j < NUM_ARGS; j++){
					if(map[j].arg == arg){
						args[map[j].indx] = i + 1;
					}
				}
			}
			
		}
	}

	int getArg(int arg){
		return args[arg];
	}

	void printHelp(){
		printf(PROJ " : " DESC "\n");
		for(int i = 0; i < NUM_ARGS; i++){
			printf("%s : %s\n", help[i][0], help[i][1]);		
		}
	}
}
