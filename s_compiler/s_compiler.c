/**
 * @author Andrea Bocchese
 * @author Ilaria Martinelli
 * @brief Compiler.
 */

#include <unistd.h>
#include <ctype.h>
#include "def.h"
#include "../s_shared/SOL_def.h"
#include "tree.h"
#include "symbol_table.h"
#include "semantic.h"
#include "code_gen.h"


#define YYSTYPE Pnode
extern char *yytext;
extern Value lexval;
extern unsigned int line;
extern FILE *yyin;
extern Pnode root;
extern int yyparse();

int main(int argc, char ** argv){
	int result = 0;
	int c;
	char *ovalue = "s.out";

	while ((c = getopt (argc, argv, "o:")) != -1)
        switch (c){
          	case 'o':
            	ovalue = optarg;
            	break;
           	case '?':
            	if (optopt == 'o')
               		fprintf (stderr, "Option -%c requires an argument.\n", optopt);
             	else if (isprint (optopt))
             		fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            	else
               		fprintf (stderr,"Unknown option character `\\x%x'.\n", optopt);
            	return 1;
           default:
           		abort();
           }

	if (argc-optind == 1){
		FILE *input_file = fopen(argv[optind], "r");
		if (input_file){
    		yyin = input_file;
		}
		else{
			fprintf(stderr,"Can't read input file.\n");
			exit(0);
		}
	}
	else{
		printf("\tUsage: compiler [-o output_name] source_name.sol\n");
		exit(0);
	}
	if((result = yyparse()) == 0){
        //printf("sizeof\nchar:\t%lu\nint:\t%lu\nfloat:\t%lu\nstring:\t%lu\n", sizeof(char), sizeof(int), sizeof(float), sizeof(char *));
        
		treeprint(root, " ");
		Phash_node symtab = create_symbol_table(root, NULL);
        
        printf("## START\n");
        Code code = makecode(S_NOOP);
        sem_program(root, symtab, &code);
        printf("## END\n");
        printf("(((%d)))\n", ((symtab->aux)->num_obj));
	
        FILE *fp = fopen(ovalue, "w");
		if (fp){
    		print_code(fp, &code);
		}
		else{
			fprintf(stderr,"Can't create output file.\n");
		}
 		
    	print_code(stdout, &code);

        destroy_code(&code);
        
	}
	return(result);
}
