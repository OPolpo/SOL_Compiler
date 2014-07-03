#include "sol_io.h"
#include "s_machine.h"

extern Pschema format_root;
extern Pformatted formatted_root;
extern int parse_format();
extern int parse_formatted();


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
            fprintf(stream, "\'%c\'", top_ostack()->inst.cval);
            break;
        case SCINT:
            fprintf(stream, "%d", top_ostack()->inst.ival);
            break;
        case SCREAL:
            fprintf(stream, "%f", top_ostack()->inst.rval);
            break;
        case SCSTRING:
            fprintf(stream, "\"%s\"", top_ostack()->inst.sval);
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

void basic_read(FILE* stream, int env_offset, int oid, char* format){
    Adescr * a_declaration = top_astack();
    int i;
    for (i=env_offset; i>0; i--) {
        a_declaration = a_declaration->alink; // not sure TODO check
    }
    Odescr * o_to_lod = *(get_p2objects(a_declaration->pos_objects) + oid-1);
    char* str_readed = newmem(1000);
    fscanf(stream, "%s", str_readed);
    parse_format(format);
    
    parse_formatted(str_readed);
    freemem(str_readed, 1000);
    //return 1;
    Pschema formatted_schema = formatted2schema(formatted_root,NULL);
    
    print_sch(format_root);
    print_sch(formatted_schema);
    if(!are_compatible(format_root, formatted_schema)){
        char* msg;
        asprintf(&msg,"Read error: schema must be compatible");
        machine_error(msg);
    }
    switch (format_root->type) {
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
            read_vector(formatted_root->child, o_to_lod->inst.sval, format_root->size, format_root->p1);
            break;
        case SCSTRUCT:
            read_struct(formatted_root->child, o_to_lod->inst.sval, format_root->p1);
            break;
        default:
            break;
    }
    destroy_schema(formatted_schema);
    destroy_schema(format_root);
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
                read_vector(elem_temp, elem_addr, elem_type_temp->size, elem_type_temp->p1);
                break;
            case SCSTRUCT:
                read_struct(elem_temp, elem_addr, elem_type_temp->p1);
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
    switch (root->type) {
        case F_CHARCONST:
            node = new_schema_node(SCCHAR);
            node->id = id;
            break;
        case F_INTCONST:
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
            node = new_schema_node(SCBOOL);
            node->id = id;
            break;
        case F_STRUCT:
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
            node = new_schema_node(SCVECTOR);
            node->id = id;
            current_node = root->child;
            current_schema = formatted2schema(current_node, current_node->id);
            current_node = current_node->brother;
            count++;
            while (current_node) {
                Pschema next = formatted2schema(current_node, current_node->id);
                if (!are_compatible(next, current_schema)) {
                    machine_error("Vector elements must be of the same type\n");
                }
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

