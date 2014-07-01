//http://en.wikipedia.org/wiki/List_of_prime_numbers
#include "schema.h"
//#include "parser.h"

void printSchema(Pschema root, char* father_indent){
    if(root==NULL) return;
    Pschema p;
    
    //printf("[][][][][][]\n");
    //printf("%s\n",schema2format(root));
    
    //--Indent Stuff
    char* my_indent = calloc(1000,sizeof(char));
    my_indent[0]=0;
    strcpy(my_indent, father_indent);
    
    if(root->p2)
        //strcat(my_indent,"    ├");
        strcat(my_indent,"├");
    else
        //strcat(my_indent,"    └");
        strcat(my_indent,"└");
    printf("%s─", my_indent);
    
    //--PRINTING SINGLE NODE
    switch(root->type){
        case SCVECTOR:
            printf("%s [ %s ] [ %d ]", "VECTOR", root->id, root->size);
            break;
        case SCSTRUCT:
            printf("%s [ %s ] [ - ]", "STRUCT", root->id);
            break;
        case SCCHAR:
            printf("%s [ %s ] [ - ]", "CHAR", root->id);
            break;
        case SCINT:
            //printf("\n## %d\n",INT);
            printf("%s [ %s ] [ - ]", "INT", root->id);
            break;
        case SCREAL:
            printf("%s [ %s ] [ - ]", "REAL", root->id);
            break;
        case SCSTRING:
            printf("%s [ %s ] [ - ]", "STRING", root->id);
            break;
        case SCBOOL:
            printf("%s [ %s ] [ - ]", "BOOL", root->id);
            break;
            // case ATTR:
            //     printf(" %s [ - ] [ - ]", "ATTR");
            //     break;
        default:
            printf("ERROR\n");
            break;
    }
    
    //--OTHER INDENT AND RECURSION AND BROTHER CICLE
    printf("\n");
    for(p=root->p1; p != NULL; p = p->p2){
        my_indent[strlen(my_indent)-4]=0;
        strcat(my_indent," |  ");
        if(root->p2 == NULL){
            my_indent[strlen(my_indent)-4]=0;
            strcat(my_indent,"    ");
        }
        printSchema(p, my_indent);
    }
    free(my_indent);
}

void print_sch(Pschema root){
    printSchema(root, " ");
    printf("\n");
}

Pschema new_schema_node(int _type){
    Pschema node = (Pschema) calloc(1,sizeof(Schema));
    node->type = _type;
    return node;
}

int compute_size(Pschema schema){
    switch(schema->type){
        case SCCHAR:
        case SCBOOL:
            return sizeof(char);
            break;
        case SCINT:
            return sizeof(int);
            break;
        case SCREAL:
            return sizeof(float);
            break;
        case SCSTRING:
            return sizeof(char*);
            break;
        case SCVECTOR:
            return (schema->size)*compute_size(schema->p1);
            break;
        case SCSTRUCT:{
            Pschema current = schema->p1;
            int dimension = 0;
            while(current){
                dimension += compute_size(current);
                current = current->p2;
            }
            return dimension;
        }
            break;
        default:
            fprintf(stderr, "%s\n","Some weird schema node in schema");
            return 0;
            break;
    }
}

int are_compatible(Pschema a, Pschema b){
    int ok;
    if (a == NULL && b == NULL) {
        return 1;
    }
    if (a == NULL || b == NULL) {
        return 0;
    }
    switch (a->type) {//CHAR, INT, REAL, STRING, BOOL, STRUCT, VECTOR
        case SCCHAR:
        case SCINT:
        case SCREAL:
        case SCBOOL:
        case SCSTRING:
            return (a->type == b->type);
            break;
        case SCSTRUCT:
            if (b->type == SCSTRUCT) {
                Pschema a_child = a->p1;
                Pschema b_child = b->p1;
                while (a_child != NULL && b_child != NULL) {
                    ok = are_compatible(a_child, b_child);
                    a_child = a_child->p2;
                    b_child = b_child->p2;
                    if(!ok)
                        return 0; //FALSE
                }
                if (a_child != NULL || b_child != NULL) {
                    return 0; //FALSE
                }
                return 1;
            }
            return 0;
            break;
        case SCVECTOR:
            if (a->size != b->size) {
                return 0; //FALSE
            }
            return are_compatible(a->p1, b->p1);
        default:
            printf("default?!\n");
            return 0;
            break;
    }
    //return 0;
}

char * schema2format(Pschema schema){
    char * str, * tmp_str, * tmp2_str; //mallocami
    Pschema tmp_schema;
    switch (schema->type) {
        case SCCHAR:
            //str = (char *)malloc(2*sizeof(char));
            str = "c";
            break;
        case SCINT:
            //str = (char *)malloc(2*sizeof(char));
            str = "i";
            break;
        case SCREAL:
            //str = (char *)malloc(2*sizeof(char));
            str = "r";
            break;
        case SCSTRING:
            //str = (char *)malloc(2*sizeof(char));
            str = "s";
            break;
        case SCBOOL:
            //str = (char *)malloc(2*sizeof(char));
            str = "b";
            break;
        case SCVECTOR:
            tmp_str = schema2format(schema->p1);
            //i=log10(schema->size);
            //str = (char *)calloc((strlen(tmp_str)+i+1+3+1),sizeof(char));
            asprintf(&str, "[%d,%s]", schema->size, tmp_str);
            break;
        case SCSTRUCT:
            tmp_schema = schema->p1;
            tmp_str = schema2format(tmp_schema);
            //str = calloc((strlen(tmp_schema->id)+strlen(tmp_str)+5),sizeof(char));
            //sprintf(str, "(%s:%s", tmp_schema->id, tmp_str);
            asprintf(&str, "(%s:%s", tmp_schema->id, tmp_str);
            
            tmp_schema = tmp_schema->p2;
            while (tmp_schema) {
                tmp_str = schema2format(tmp_schema);
                //printf("--->%s\n", tmp_str);
                /*
                tmp2_str = calloc((strlen(str)+strlen(tmp_str)+1+2),sizeof(char));
                strcpy(tmp2_str, str);
                str = tmp2_str;
                strcat(str, ",");
                strcat(str, (tmp_schema->id? tmp_schema->id: ""));
                strcat(str, ":");
                strcat(str, tmp_str);//
                */
                
                asprintf(&tmp2_str, "%s,%s:%s", str, (tmp_schema->id? tmp_schema->id: ""), tmp_str);
                ///sprintf(str, ",%s:%s", tmp_schema->id, tmp_str);
                str = tmp2_str;
                tmp_schema = tmp_schema->p2;
            }
            asprintf(&tmp2_str, "%s)", str);
            str = tmp2_str;
            //strcat(str, ")");
            //sprintf(str, ")");
            break;
        default:
            str = NULL;
            break;
    }
    return str;
}

char * make_format(Pschema schema){
    char * str, * tmp_str;
    tmp_str = schema2format(schema);
    str = calloc(strlen(tmp_str)+3, sizeof(char));
    sprintf(str, "\"%s\"", tmp_str);
    return str;
}

void destroy_schema(Pschema schema){
    if(schema->p1 != NULL)
        destroy_schema(schema->p1);
    if(schema->p2 != NULL)
        destroy_schema(schema-p2);
    free(schema);
}
