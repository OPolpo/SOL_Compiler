#include "s_exec.h"
#include "parser.h"

Scode *prog;
int pc;

void exec(Scode *stat) {
    //print_code_instruction(stat);
    //printf("pc %d, ap %d, op %d, ip %d\n",pc,ap,op,ip);
    
    printf("[%3d] exec %d", pc, stat->op);
    switch (stat->op) {
        case S_PUSH: exec_push(stat->args[0].ival, stat->args[1].ival, pc+1); break;
        case S_GOTO: exec_goto(stat->args[0].ival); break;
        case S_POP: exec_pop(); break;
        case S_NEW: exec_new(stat->args[0].ival); break;
        case S_NEWS: exec_news(stat->args[0].ival); break;
        case S_LDC: exec_ldc(stat->args[0].ival); break;
        case S_LDI: exec_ldi(stat->args[0].ival); break;
        case S_LDS: exec_lds(stat->args[0].sval); break;
        case S_LDR: exec_ldr(stat->args[0].rval); break;
        case S_LOD: exec_lod(stat->args[0].ival, stat->args[1].ival); break;
        case S_CAT: exec_cat(stat->args[0].ival, stat->args[1].ival); break;
        case S_LDA: exec_lda(stat->args[0].ival, stat->args[1].ival); break;
        case S_FDA: exec_fda(stat->args[0].ival); break;
        case S_EIL: exec_eil(stat->args[0].ival); break;
        case S_SIL: exec_sil(stat->args[0].ival); break;
        case S_IXA: exec_ixa(stat->args[0].ival); break;
        case S_STO: exec_sto(stat->args[0].ival, stat->args[1].ival); break;
        case S_IST: exec_ist(); break;
        case S_JMF: exec_jmf(stat->args[0].ival); break;
        case S_JMP: exec_jmp(stat->args[0].ival); break;
        case S_EQU: exec_equ(); break;
        case S_NEQ: exec_neq(); break;
        case S_CGT: exec_cgt(); break;
        case S_IGT: exec_igt(); break;
        case S_RGT: exec_rgt(); break;
        case S_SGT: exec_sgt(); break;
        case S_CGE: exec_cge(); break;
        case S_IGE: exec_ige(); break;
        case S_RGE: exec_rge(); break;
        case S_SGE: exec_sge(); break;
        case S_CLT: exec_clt(); break;
        case S_ILT: exec_ilt(); break;
        case S_RLT: exec_rlt(); break;
        case S_SLT: exec_slt(); break;
        case S_CLE: exec_cle(); break;
        case S_ILE: exec_ile(); break;
        case S_RLE: exec_rle(); break;
        case S_SLE: exec_sle(); break;
        case S_IN: exec_in(); break;
        case S_IPLUS: exec_iplus(); break;
        case S_RPLUS: exec_rplus(); break;
        case S_IMINUS: exec_iminus(); break;
        case S_RMINUS: exec_rminus(); break;
        case S_ITIMES: exec_itimes(); break;
        case S_RTIMES: exec_rtimes(); break;
        case S_IDIV: exec_idiv(); break;
        case S_RDIV: exec_rdiv(); break;
        case S_IUMI: exec_iumi(); break;
        case S_RUMI: exec_rumi(); break;
        case S_NEG: exec_neg(); break;
        case S_WR: exec_wr(stat->args[0].sval); break;
        case S_FWR: exec_fwr(stat->args[0].sval); break;
        case S_RD: exec_rd(stat->args[0].sval); break;
        case S_FRD: exec_frd(stat->args[0].sval); break;
        case S_TOINT: exec_toint(); break;
        case S_TOREAL: exec_toreal(); break;
        case S_READ: exec_read(stat->args[0].ival, stat->args[1].ival, stat->args[2].sval); break;
        case S_FREAD: exec_fread(stat->args[0].ival, stat->args[1].ival, stat->args[2].sval); break;
        case S_WRITE: exec_write(stat->args[0].sval); break;
        case S_FWRITE: exec_fwrite(stat->args[0].sval); break;
        case S_FUNC: exec_func(stat->args[0].ival); break;
        case S_RETURN: exec_return(); break;
        default: machine_error("Unknown operator"); break;
    }
    printf("pc %d, op %d\n",pc,get_next_op());
}

void exec_toint(){
    int n = (float)pop_int();
    push_real(n);
}

void exec_toreal(){
    int n = (int)pop_real();
    push_int(n);
}

void exec_pop(){
    int i,n = top_astack()->numobj;
    for (i=0; i<n; i++) {
        if (top_ostack()->mode == STA) {
            pop_istack(top_ostack()->size);
        }
        pop_ostack();
    }
    pop_astack();
}

void exec_ist(){
    memcpy(under_top_ostack()->inst.sval, &(top_ostack()->inst), top_ostack()->size);
    pop_ostack();
    pop_ostack();
}

void exec_equ(){
    void * n, * m;
    int res = top_ostack()->size == under_top_ostack()->size;
    if (res) {
        if (top_ostack()->mode == EMB){
            n = &(top_ostack()->inst);
            m = &(under_top_ostack()->inst);
        }
        else{
            n = (top_ostack()->inst).sval;
            m = (under_top_ostack()->inst).sval;
        }
        res = memcmp(n, m, top_ostack()->size);
    }
    pop_ostack();
    pop_ostack();
    push_bool(res);
}

void exec_neq(){
    void * n, * m;
    if (top_ostack()->mode == EMB){
        n = &(top_ostack()->inst);
        m = &(under_top_ostack()->inst);
    }
    else{
        n = (top_ostack()->inst).sval;
        m = (under_top_ostack()->inst).sval;
    }
    int res = !memcmp(n, m, top_ostack()->size);
    pop_ostack();
    pop_ostack();
    push_bool(res);
}

void exec_cgt(){
    char n, m;
    n = pop_char();
    m = pop_char();
    push_bool(m>n);
}

void exec_igt(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_bool(m>n);
}

void exec_rgt(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_bool(m>n);
}

void exec_sgt(){
    char * n, * m;
    n = pop_string();
    m = pop_string();
    push_bool(strcmp(m, n) > 0);
}

void exec_cge(){
    char n, m;
    n = pop_char();
    m = pop_char();
    push_bool(m>=n);
}

void exec_ige(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_bool(m>=n);
}

void exec_rge(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_bool(m>=n);
}

void exec_sge(){
    char * n, * m;
    n = pop_string();
    m = pop_string();
    push_bool(strcmp(m, n) >= 0);
}

void exec_clt(){
    char n, m;
    n = pop_char();
    m = pop_char();
    push_bool(m<n);
}

void exec_ilt(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_bool(m<n);
}

void exec_rlt(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_bool(m<n);
    
}

void exec_slt(){
    char * n, * m;
    n = pop_string();
    m = pop_string();
    push_bool(strcmp(m, n) < 0);
}

void exec_cle(){
    char n, m;
    n = pop_char();
    m = pop_char();
    push_bool(m<=n);
}

void exec_ile(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_bool(m<=n);
}

void exec_rle(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_bool(m<=n);
}

void exec_sle(){
    char * n, * m;
    n = pop_string();
    m = pop_string();
    push_bool(strcmp(m, n) <= 0);
}

void exec_in(){
    int found=0, i, n = top_ostack()->size / under_top_ostack()->size;
    char * to_find = (under_top_ostack()->mode == EMB) ? (char *)&(under_top_ostack()->inst) : under_top_ostack()->inst.sval;
    for (i=0; i<n; i++) {
        if (memcmp(top_ostack()->inst.sval, to_find, under_top_ostack()->size)==0) {
            found = 1;
            break;
        }
    }
    pop_ostack();
    pop_ostack();
    push_bool(found);
}

void exec_iplus(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m+n);
}

void exec_rplus(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_real(m+n);
}

void exec_iminus(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m-n);
}

void exec_rminus(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_real(m-n);
}

void exec_itimes(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m*n);
}

void exec_rtimes(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_real(m*n);
}

void exec_idiv(){
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m/n);
}

void exec_rdiv(){
    float n, m;
    n = pop_real();
    m = pop_real();
    push_real(m/n);
}

void exec_iumi(){
    int i = pop_int();
    push_int(-i);
}

void exec_rumi(){
    float r = pop_real();
    push_real(-r);
}

void exec_neg(){
    int n = pop_bool();
    push_bool(!n);
}

void exec_return(){
    pc = top_astack()->raddr;
}

void exec_push(int size, int chain, int raddr){
    printf("push %d %d\n", size, chain);
    Adescr * actual_ar = NULL;
    if(chain >= 0)
        actual_ar = top_astack();
    Adescr * new_ar = push_astack();
    new_ar->numobj = size;
    new_ar->pos_objects = get_next_op();
    new_ar->raddr = raddr;
    int i;
    new_ar->alink = actual_ar;
    for (i=chain; i>0; i--) {
        new_ar->alink = actual_ar->alink;
    }
}

void exec_sto(int env_offset, int oid){
    printf("STO %d %d\n", env_offset, oid);
    Adescr * a_declaration = top_astack();
    int i;
    for (i=env_offset; i>0; i--) {
        printf("NOOOO\n");
        a_declaration = a_declaration->alink; // not sure TODO check
    }
    Odescr * o_to_store = *(get_p2objects(a_declaration->pos_objects) + oid-1);
    memcpy(&(o_to_store->inst), &(top_ostack()->inst), sizeof(Value));
    pop_ostack();
}

void exec_lda(int env_offset, int oid){
    Adescr * a_declaration = top_astack();
    int i;
    for (i=env_offset; i>0; i--) {
        a_declaration = a_declaration->alink; // not sure TODO check
    }
    Odescr * o_to_lod = *(get_p2objects(a_declaration->pos_objects) + oid-1);
    push_ostack();
    memcpy(top_ostack(), o_to_lod, sizeof(Odescr));
}

void exec_cat(int num, int size){
    char * new_inst = push_istack(size);
    char * start = new_inst+size;
    int i, temp_size=0;
    for (i=0; i<num; i++) {
        if(top_ostack()->mode == EMB){
            memcpy(start-top_ostack()->size, &(top_ostack()->inst), top_ostack()->size);
            //temp_size += top_ostack()->size;
        }else{
            memcpy(start-top_ostack()->size, top_ostack()->inst.sval, top_ostack()->size);
            //temp_size += top_ostack()->size;
            move_down_istack(size, top_ostack()->size); // not sure TODO check
        }
        start -= top_ostack()->size;
        pop_ostack();
    }
    //if (temp_size != size) machine_error("exec_cat");
    Odescr * new_obj = push_ostack();
    new_obj->mode = STA;
    new_obj->size = size;
    new_obj->inst.sval = new_inst;
}

void exec_lod(int env_offset, int oid){
    Adescr * a_declaration = top_astack();
    int i;
    for (i=env_offset; i>0; i--) {
        a_declaration = a_declaration->alink; // not sure TODO check
    }
    Odescr * o_to_lod = *(get_p2objects(a_declaration->pos_objects) + oid-1);
    push_ostack();
    memcpy(top_ostack(), o_to_lod, sizeof(Odescr));
    printf("-->%d\n", top_ostack()->inst.ival);
    
    if (top_ostack()->mode == STA) {
        char * i_address = push_istack(top_ostack()->size);
        memcpy(i_address, top_ostack()->inst.sval, top_ostack()->size);
        top_ostack()->inst.sval = i_address;
    }
}

int calc_size(char * f){
    int dim = 0;
    //char * f = format;
    //char subformat[strlen(format)];
    printf("%c\n", f[0]);
    switch (f[0]) {
        case 'c':
        case 'b':
            f++;
            return sizeof(char);
            break;
        case 'i':
            f++;
            return sizeof(int);
            f++;
            break;
        case 'r':
            f++;
            return sizeof(float);
            break;
        case 's':
            f++;
            return sizeof(char *);
            break;
        case '(':
            do{
                do {
                    f++;
                } while( f[0] != ':');
                //printf("%c spero :\n", f[0]);
                f++;
                dim += calc_size(f);
                f++;
            } while (f[0] == ',');
            //printf("%c spero ,\n", f[0]);
            return dim;
            break;
        case '[':
            f++;
            sscanf(f, "%d", &dim);
            do {
                f++;
            } while( f[0] != ',');
            f++;
            return dim * calc_size(f);
        case ')':
            printf(")");
            break;
        case '\0':
            printf("the end");
            break;
        case ']':
            printf("]");
            
            break;
        case ',':
            printf("vaffanculo");
        default:
            break;
    }
    return 0;
}

void exec_read(int oid, int offset, char * format){
    
}

void exec_fread(int oid, int offset, char * format){
    
}

int basic_write(char* format, FILE * stream, char * addr){
    if(addr != NULL){
    //vector
        if(format[0]=='['){
            //printf("vect\n");
            return write_vect(format, stream, top_ostack()->inst.sval);
        }
        //struct
        else if(format[0]=='('){
            //printf("struct\n");
            return write_struct(format, stream, top_ostack()->inst.sval);
        }
        //atomic
        else{
            //printf("atom\n");
            //printf("nella tipologia atomica il formato è %s\n",format);
            if (format[0] == 'c'){
                if(top_ostack()->mode==STA)
                    fprintf(stream, "====>%c<====", *addr);
                else
                    fprintf(stream, "====>%c<====", ((Value *) addr)->cval);
                return 1;
            }
            if (format[0] == 'i'){
                if(top_ostack()->mode==STA)
                    fprintf(stream, "====>%d<====", *((int*)addr));
                else
                    fprintf(stream, "====>\'%d\'<====", ((Value *) addr)->ival);
                return 4;
            }
            if (format[0] == 'r'){
                if(top_ostack()->mode==STA)
                    fprintf(stream, "====>%f<====", *(float*)addr);
                else
                    fprintf(stream, "====>%f<====", ((Value *) addr)->rval);
                return 4;
            }
            if (format[0] == 's'){
                if(top_ostack()->mode==STA)
                    fprintf(stream, "====>\'%s\'<====",  (char**)addr);
                else
                    fprintf(stream, "====>\'%s\'<====", ((Value *) addr)->sval);
                return 8;
            }
            if (format[0] == 'b'){
                if(top_ostack()->mode==STA)
                    fprintf(stream, "====>%s<====", ((char) *addr) =='0' ? "false" : "true");
                else
                    fprintf(stream, "====>%s<====", ((Value *) addr)->cval=='0' ? "false" : "true");
                return 1;
            }
            return 1;
        }
    }
    else{
        return basic_write(format, stream, &(top_ostack()->inst));
    }
}

void exec_write(char* format){
    printf("\n");
    printf("\n");
    basic_write(format, stdout, NULL);
    printf("\n");
    printf("\n");


}

void exec_fwrite(char* format){
    FILE * fp;
    char* file_name = pop_string();
    fp = fopen(file_name, "w");
    if(!fp){
        char* msg;
        asprintf(&msg,"Can't write %s", file_name);
        machine_error(msg);
    }
    // basic_write(format, fp, NULL);
    basic_write(format, stdout, NULL);
    fclose (fp);
}

void exec_rd(char* format){
    
}

void exec_frd(char* format){
    
}

void exec_wr(char* format){
    
}

void exec_fwr(char* format){
    
}

void exec_lds(char* arg1){
    push_string(arg1);
}

void exec_ldr(float arg1){
    push_real(arg1);
}

void exec_ldc(char arg1){
    push_char(arg1);
}

void exec_func(int arg1){
    
}

void exec_goto(int arg1){
    pc = arg1;
}

void exec_jmf(int offset){
    if(!pop_bool())
        pc += offset-1;
}

void exec_jmp(int offset){
    pc += offset-1;
}

void exec_ixa(int size){
    int n = pop_int();
    top_ostack()->inst.sval += (n*size);
}

void exec_eil(int size){
    char * content = top_ostack()->inst.sval;
    pop_ostack();
    Odescr *po;
    po = push_ostack();
    po->mode = EMB;
    po->size = size;
    memcpy(&(po->inst), content, size);
}

void exec_sil(int size){
    char * content = top_ostack()->inst.sval;
    pop_ostack();
    Odescr *po;
    po = push_ostack();
    po->mode = STA;
    po->size = size;
    po->inst.sval = push_istack(size);
    memcpy(po->inst.sval, content, size);
}

void exec_fda(int offset){
    top_ostack()->inst.sval += offset;
}

void exec_ldi(int arg1){
    push_int(arg1);
}

void exec_new(int size){
    Odescr *po;
    po = push_ostack();
    po->mode = EMB;
    po->size = size;
}

void exec_news(int size){
    Odescr *po;
    po = push_ostack();
    po->mode = STA;
    po->size = size;
    po->inst.sval = push_istack(size);
}

int write_vect(char * format, FILE* stream, char* addr){
    //printf(" ENTRO\n");
    int size = 0;
    int i = 0; 
    char str_format[strlen(format)]; // todo
    sscanf(format,"[%d,%s",&size, str_format);
    int element_size = (top_ostack()->size)/size;
    for(i=0;i<size;i++){
        //printf("dimensione elemento %d\n", element_size);
        basic_write(str_format, stream, addr+element_size*i);
    }
    return top_ostack()->size;
}

int write_struct(char * format, FILE* stream, char* addr){
    int size = top_ostack()->size;
    int i = 0;
    while(!(format[i]==0 || format[i] == ':')){
        i++;
    }
    //printf("format %s\n",format+i);


    while(size > 0){
        //printf("%d",size);
        i+=1;
        size -= basic_write(format+i, stream, addr + size);
        while(!(format[i]==0 || format[i] == ':')){
            i++;
        }
        //printf("\nsize residuo: %d\n",size);
        //getc(stdin);
    }
    //printf("%d",top_ostack()->size);
    return top_ostack()->size;
}
