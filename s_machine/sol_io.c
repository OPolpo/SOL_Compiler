#include "sol_io.h"
#include "s_machine.h"

extern Pschema format_root;
extern Pformat formatted_root;
extern int parse_format();


void print_atomic_istack(FILE* stream, char * elem_addr, Pschema elem_type){
    switch (elem_type->type) {
        case SCCHAR:
            fprintf(stream, "\'%c\'",*elem_addr);
            break;
        case SCINT:
            fprintf(stream, "%d",*(int *)elem_addr);
            break;
        case SCREAL:
            fprintf(stream, "%f",*(float *)elem_addr);
            break;
        case SCSTRING:
            fprintf(stream, "\"%s\"",*(char**)elem_addr);
            break;
        case SCBOOL:
            fprintf(stream, "%s", (*(int *)elem_addr)? "true" : "false");
            break;
        default:
            break;
    }
}

void print_vector(FILE* stream, char * elem_addr, int elem_num, Pschema elem_type){
    int elem_dim = compute_size(elem_type);
    int i;
    fprintf(stream, "[");
    for (i=0; i< elem_num; i++){
        if (i!=0) fprintf(stream, ", ");
        switch (elem_type->type) {
            case SCVECTOR:
                print_vector(stream, elem_addr, elem_type->size, elem_type->p1);
                break;
            case SCSTRUCT:
                print_struct(stream, elem_addr, elem_type->p1);
                break;
            default:
                print_atomic_istack(stream, elem_addr, elem_type);
                break;
        }
        elem_addr += (elem_dim);
    }
    fprintf(stream, "]");
}

void print_struct(FILE* stream, char * elem_addr, Pschema elem_type){
    fprintf(stream, "(");
    int i=0;
    Pschema temp = elem_type;
    while (temp) {
        if (i!=0)
            fprintf(stream,", ");
        else
            i=1;
        fprintf(stream,"%s:", temp->id);
        switch (temp->type) {
            case SCVECTOR:
                print_vector(stream, elem_addr, temp->size, temp->p1);
                break;
            case SCSTRUCT:
                print_struct(stream, elem_addr, temp->p1);
                break;
            default:
                print_atomic_istack(stream, elem_addr, temp);
                break;
        }
        elem_addr += compute_size(temp);
        temp = temp->p2;
    }
    fprintf(stream, ")");
}

void basic_wr(FILE* stream, char* format){
    parse_format(format);
    switch (format_root->type) {
        case SCCHAR:
            fprintf(stream, "\'%c\'",pop_char());
            break;
        case SCINT:
            fprintf(stream, "%d",pop_int());
            break;
        case SCREAL:
            fprintf(stream, "%f",pop_real());
            break;
        case SCSTRING:
            fprintf(stream, "\"%s\"", pop_string());
            break;
        case SCBOOL:
            fprintf(stream, "%s", pop_bool()? "true" : "false");
            break;
        case SCVECTOR:
            print_vector(stream, top_ostack()->inst.sval, format_root->size, format_root->p1);
            pop_istack(top_ostack()->size);
            break;
        case SCSTRUCT:
            print_struct(stream, top_ostack()->inst.sval, format_root->p1);
            pop_istack(top_ostack()->size);
            break;
        default:
            break;
    }
    destroy_schema(format_root);    
}