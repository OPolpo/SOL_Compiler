#include "sol_io.h"
#include "s_machine.h"

extern Pschema format_root;
extern Pformatted formatted_root;
extern int parse_format();
extern int parse_formatted();


void print_atomic_istack(FILE* stream, char * elem_addr, Pschema elem_type, int on_file){
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
            if(on_file)
                fprintf(stream, "\"%s\"",*(char**)elem_addr);
            else
                fprintf(stream, "%s",*(char**)elem_addr);
            break;
        case SCBOOL:
            fprintf(stream, "%s", (*elem_addr!='0')? "true" : "false");
            break;
        default:
            break;
    }
}

void print_vector(FILE* stream, char * elem_addr, int elem_num, Pschema elem_type, int on_file){
    int elem_dim = compute_size(elem_type);
    int i;
    fprintf(stream, "[");
    for (i=0; i< elem_num; i++){
        if (i!=0) fprintf(stream, ", ");
        switch (elem_type->type) {
            case SCVECTOR:
                print_vector(stream, elem_addr, elem_type->size, elem_type->p1, on_file);
                break;
            case SCSTRUCT:
                print_struct(stream, elem_addr, elem_type->p1, on_file);
                break;
            default:
                print_atomic_istack(stream, elem_addr, elem_type, on_file);
                break;
        }
        elem_addr += (elem_dim);
    }
    fprintf(stream, "]");
}

void print_struct(FILE* stream, char * elem_addr, Pschema elem_type, int on_file){
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
                print_vector(stream, elem_addr, temp->size, temp->p1, on_file);
                break;
            case SCSTRUCT:
                print_struct(stream, elem_addr, temp->p1, on_file);
                break;
            default:
                print_atomic_istack(stream, elem_addr, temp, on_file);
                break;
        }
        elem_addr += compute_size(temp);
        temp = temp->p2;
    }
    fprintf(stream, ")");
}

void basic_wr(FILE* stream, char* format, int on_file){
    parse_format(format);
    switch (format_root->type) {
        case SCCHAR:
            fprintf(stream, "\'%c\'", top_ostack()->inst.cval);
            break;
        case SCINT:
            fprintf(stream, "%d", top_ostack()->inst.ival);
            break;
        case SCREAL:
            fprintf(stream, "%f", top_ostack()->inst.rval);
            break;
        case SCSTRING:
            if(on_file)
                fprintf(stream, "\"%s\"", top_ostack()->inst.sval);
            else
                fprintf(stream, "%s", top_ostack()->inst.sval);
            break;
        case SCBOOL:
            fprintf(stream, "%s", top_ostack()->inst.cval != '0' ? "true" : "false");
            break;
        case SCVECTOR:
            print_vector(stream, top_ostack()->inst.sval, format_root->size, format_root->p1, on_file);
            //pop_istack(top_ostack()->size);
            break;
        case SCSTRUCT:
            print_struct(stream, top_ostack()->inst.sval, format_root->p1, on_file);
            //pop_istack(top_ostack()->size);
            break;
        default:
            break;
    }
    destroy_schema(format_root);
}

void read_atomic_istack(Pformatted elem, char * elem_addr, Pschema elem_type){
    switch (elem_type->type) {
        case SCCHAR:
        case SCBOOL:
            memcpy(elem_addr, &elem->value, sizeof(char));
            break;
        case SCINT:
            memcpy(elem_addr, &elem->value, sizeof(int));
            break;
        case SCREAL:
            memcpy(elem_addr, &elem->value, sizeof(float));
            break;
        case SCSTRING:
            memcpy(elem_addr, &elem->value, sizeof(char*));
            break;
        default:
            break;
    }
}

void basic_read(FILE* stream, Odescr * o_to_lod, Pschema schema){
    //char* str_readed = newmem(1000);
    //fscanf(stream, "%s", str_readed);
    //printf("parse formatted\n");
    parse_formatted(stream);


    //freemem(str_readed, 1000);
    //return 1;
    //printf("formatted2schema");
    Pschema formatted_schema = formatted2schema(formatted_root,NULL);
    
    // print_sch(format_root);
    // printf("schema letto\n");
    //print_sch(formatted_schema);
    // printf("\nfine schema letto\n");

    if(!are_compatible(schema, formatted_schema)){
        char* msg;
        asprintf(&msg,"Read error: schema must be compatible");
        machine_error(msg);
    }
    switch (schema->type) {
        case SCCHAR:
        case SCBOOL:
            o_to_lod->inst.cval = formatted_root->value.cval;
            break;
        case SCINT:
            o_to_lod->inst.ival = formatted_root->value.ival;
            break;
        case SCREAL:
            o_to_lod->inst.rval = formatted_root->value.rval;
            break;
        case SCSTRING:
            o_to_lod->inst.sval = formatted_root->value.sval;
            break;
        case SCVECTOR:
            read_vector(formatted_root->child, o_to_lod->inst.sval, format_root->size, schema->p1);
            break;
        case SCSTRUCT:
            read_struct(formatted_root->child, o_to_lod->inst.sval, schema->p1);
            break;
        default:
            break;
    }
    //printf("destroy\n");
    destroy_schema(formatted_schema);
    destroy_formatted(formatted_root);
}

void read_vector(Pformatted elem, char * elem_addr, int elem_num, Pschema elem_type){
    int elem_dim = compute_size(elem_type);
    int i;
    for (i=0; i< elem_num; i++){
        switch (elem_type->type) {
            case SCVECTOR:
                read_vector(elem->child, elem_addr, elem_type->size, elem_type->p1);
                break;
            case SCSTRUCT:
                read_struct(elem->child, elem_addr, elem_type->p1);
                break;
            default:
                read_atomic_istack(elem, elem_addr, elem_type);
                break;
        }
        elem = elem->brother;
        elem_addr += (elem_dim);
    }
}

void read_struct(Pformatted elem, char * elem_addr, Pschema elem_type){
    Pformatted elem_temp = elem;
    Pschema elem_type_temp = elem_type;
    while (elem_type_temp){
        switch (elem_type_temp->type) {
            case SCVECTOR:
                read_vector(elem_temp->child, elem_addr, elem_type_temp->size, elem_type_temp->p1);
                break;
            case SCSTRUCT:
                read_struct(elem_temp->child, elem_addr, elem_type_temp->p1);
                break;
            default:
                read_atomic_istack(elem_temp, elem_addr, elem_type_temp);
                break;
        }
        elem_addr += compute_size(elem_type_temp);
        elem_temp = elem_temp->brother;
        elem_type_temp = elem_type_temp->p2;
    }
}

Pschema formatted2schema(Pformatted root, char * id){
    Pschema node = NULL;
    Pformatted current_node;
    Pschema current_schema;
    int count=0;
    if (root == NULL) {
        return new_schema_node(-1);
    }
    
    switch (root->type) {
        case F_CHARCONST:
            node = new_schema_node(SCCHAR);
            node->id = id;
            break;
        case F_INTCONST:
            //printf("root is int\n");
            node = new_schema_node(SCINT);
            node->id = id;
            break;
        case F_REALCONST:
            node = new_schema_node(SCREAL);
            node->id = id;
            break;
        case F_STRCONST:
            node = new_schema_node(SCSTRING);
            node->id = id;
            break;
        case F_BOOLCONST:
            //printf("root is bool\n");
            node = new_schema_node(SCBOOL);
            node->id = id;
            break;
        case F_STRUCT:
            //printf("root is struct\n");
            node = new_schema_node(SCSTRUCT);
            node->id = id;
            current_node = root->child;
            current_schema = formatted2schema(current_node, current_node->id);
            node->p1 = current_schema;
            current_node = current_node->brother;
            while (current_node) {
                current_schema->p2 = formatted2schema(current_node, current_node->id);
                current_schema = current_schema->p2;
                current_node = current_node->brother;
            }
            break;
        case F_VECTOR:
            //printf("root is vector\n");
            node = new_schema_node(SCVECTOR);
            node->id = id;
            current_node = root->child;
            current_schema = formatted2schema(current_node, current_node->id);
            current_node = current_node->brother;
            count++;
            //printf("other children\n");
            while (current_node) {
                Pschema next = formatted2schema(current_node, current_node->id);
                if (!are_compatible(next, current_schema)) {
                    machine_error("Vector elements must be of the same type\n");
                }
                destroy_schema(next);
                current_node = current_node->brother;
                count++;
            }
            node->size = count;
            node->p1 = current_schema;
            break;
        default:
            break;
    }
    return node;
}

void destroy_formatted(Pformatted formatted){
    if(formatted->child != NULL)
        destroy_formatted(formatted->child);
    if(formatted->brother != NULL)
        destroy_formatted(formatted->brother);
    free(formatted);
}
