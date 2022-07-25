#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


char result[1000];
long long R[32]; //register
int pc; //program counter

/*******************************************************
 * Function Declaration
 *
 *******************************************************/
char *change_file_ext(char *str);
void mymain(FILE* input);

/*******************************************************
 * Function: main
 *
 * Parameters:
 *  int
 *      argc: the number of argument
 *  char*
 *      argv[]: array of a sting argument
 *
 * Return:
 *  return success exit value
 *
 * Info:
 *  The typical main function in C language.
 *  It reads system arguments from terminal (or commands)
 *  and parse an assembly file(*.s).
 *  Then, it converts a certain instruction into
 *  object code which is basically binary code.
 *
 *******************************************************/
int main(int argc, char *argv[])
{
    FILE *input, *output;
    char *filename;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <*.s>\n", argv[0]);
        fprintf(stderr, "Example: %s sample_input/example?.s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    input = fopen(argv[1], "r");

    if (input == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    filename = strdup(argv[1]); // strdup() is not a standard C library but fairy used a lot.
    if(change_file_ext(filename) == NULL) {
        fprintf(stderr, "'%s' file is not an assembly file.\n", filename);
        exit(EXIT_FAILURE);
    }

    output = fopen(filename, "w");
    if (output == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    mymain(input);

    fprintf(output,"%s\n", result);

    fclose(input);
    fclose(output);
    exit(EXIT_SUCCESS);
}


/*******************************************************
 * Function: change_file_ext
 *
 * Parameters:
 *  char
 *      *str: a raw filename (without path)
 *
 * Return:
 *  return NULL if a file is not an assembly file.
 *  return empty string
 *
 * Info:
 *  This function reads filename and converst it into
 *  object extention name, *.o
 *
 *******************************************************/
char
*change_file_ext(char *str)
{
    char *dot = strrchr(str, '.');

    if (!dot || dot == str || (strcmp(dot, ".s") != 0)) {
        return NULL;
    }

    str[strlen(str) - 1] = 'o';
    return "";
}
/*****************************************************


                        My Code


*****************************************************/

typedef struct { char name[10]; long long value;} datastruct;

typedef struct { char func[10]; char operand[20]; } textstruct;

typedef struct { char name[10]; int textindex;} labelstruct;

char* ftos(FILE*);

void setData(char*);
void setText(char*);
void setLabel(char*, int textindex);

datastruct data[10];
textstruct text[100];
labelstruct label[10];
int datanum=0, insnum=0, labelnum=0;

void show(){
    int i;
    printf("****************** Struct View ******************\n");
    printf("num <%d %d %d>\n", datanum, insnum, labelnum);
    for(i=0; i<datanum; i++){
        printf("data <%s %lld>\n", data[i].name, data[i].value);
    }
    for(i=0; i<insnum; i++){
        printf("text <%s %s>\n", text[i].func, text[i].operand);
    }
    for(i=0; i<labelnum; i++){
        printf("label <%s %d>\n", label[i].name, label[i].textindex);
    }
    printf("*************************************************\n");
    return;
}

void ASSEMtoBIN();

void lui(int rt, long long imm);
void ori(int rt, int rs, long long imm);

void Rformat();
void Iformat();
void Jformat();

long long strToll(char* str);
int labelToaddr(char* str);

void opTobin(int k);
void rTobin(int k);
void iTobin(long long k);
void jTobin(long long k);

/****************************************************/
void mymain(FILE* input){

    char* buffer = ftos(input);
    ////////////find the number of data ins label
    ////////////input right value to each struct
    char* tmp = (char*)malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy(tmp, buffer);
    char* ptr = strtok(tmp, "\n");
    while(ptr != NULL){

        if(!strcmp(ptr, "\t.data")){
            ptr = strtok(NULL, "\n");
            while(strcmp(ptr,"\t.text")){
                setData(ptr);
                datanum++;
                ptr = strtok(NULL, "\n");
            }
        }
        if(!strcmp(ptr, "\t.text")){
            
            ptr = strtok(NULL, "\n");
            while(ptr != NULL){
                if(ptr[strlen(ptr) - 1] == ':'){
                    setLabel(ptr, insnum);
                    labelnum++;
                }
                else{
                    setText(ptr);
                    insnum++;
                }
                ptr = strtok(NULL, "\n");
            }
        }
        ptr = strtok(NULL, "\n");
    }
    //////////////end input value
    //show();
    result[0] = '\0';
    int num;

    
    //int limit = insnum;
    while(pc < insnum){
        ASSEMtoBIN();
    }
    num = datanum*4;
    char* databin = (char*)malloc(sizeof(char)*33);
    int i=0;
    for(i=0; i<32; i++){
        databin[i] = '0';
    }
    databin[32] = '\0';
    int j=1;
    while (num > 0){
        databin[32 - j] = (num % 2) + '0';
        num = num / 2;
        j++;
    }

    num = insnum*4;
    char* textbin = (char*)malloc(sizeof(char)*33);
    for(i=0; i<32; i++){
        textbin[i] = '0';
    }
    textbin[32] = '\0';
    j=1;
    while (num > 0){
        textbin[32 - j] = (num % 2) + '0';
        num = num / 2;
        j++;
    }

    char asdf[1000]={};
    strcat(asdf, textbin);
    strcat(asdf, databin);
    free(databin);
    free(textbin);

    strcat(asdf, result);

    for(i=0; i<datanum; i++){
        num = data[i].value;
        char* valuebin = (char*)malloc(sizeof(char)*33);
        int d;
        for(d=0; d<32; d++){
            valuebin[d] = '0';
        }
        valuebin[32] = '\0';
        j=1;
        while (num > 0){
            valuebin[32 - j] = (num % 2) + '0';
            num = num / 2;
            j++;
        }

        strcat(asdf, valuebin);
        free(valuebin);
    }
    strcpy(result, asdf);

    free(tmp);

    //printf("\nrealnum <%d %d %d>\n", datanum, insnum, labelnum);
    return;
}

char* ftos(FILE* input){
    int lSize;
    char* buffer;
    size_t result;

    // save size of file in ISize
    fseek(input, 0, SEEK_END);
    lSize = ftell(input);
    rewind(input);

    // memory allocate
    buffer = (char*)malloc(sizeof(char) * lSize);
    if (buffer == NULL) {
        fputs("Memory error", stderr);
        exit(2);
    }
    // save file contents in buffer
    result = fread(buffer, 1, lSize, input);
    if (result != lSize) {
        fputs("Reading error", stderr);
        exit(3);
    }

    return buffer;
}



void setData(char* str){
    char value[32] = {};
    char tmp[32] = {};
    strcpy(tmp, str);
    int a = sscanf(tmp, "%s %*s %s", data[datanum].name, value);
    if(a == 1){//if array
        strcpy(data[datanum].name, "");
        sscanf(str, "%*s %s", value);//segfault
    }else{
        int i;
        for(i=0; i<strlen(data[datanum].name); i++)
            if(data[datanum].name[i] == ':')
                data[datanum].name[i] = '\0';
    }
    
    if(value[0] == '0'){//if hex
        data[datanum].value = strtol(value, NULL, 16);
    }else{
        data[datanum].value = strtol(value, NULL, 10);
    }

    return;
}

void setText(char* str){
    sscanf(str, "%s %[^\n]", text[insnum].func, text[insnum].operand);
    return;
}

void setLabel(char* str, int textindex){
    label[labelnum].textindex = textindex;
    sscanf(str, "%[^:]", label[labelnum].name);

    return;
}






void ASSEMtoBIN(){
    if( !( strcmp(text[pc].func, "j") && strcmp(text[pc].func, "jal") )){
        printf("%d %s\n", pc, text[pc].func);
        //sleep(1);
        Jformat();
        return;
    }
    else if( !( strcmp(text[pc].func, "addu") && strcmp(text[pc].func, "and") && strcmp(text[pc].func, "jr") && strcmp(text[pc].func, "nor") && strcmp(text[pc].func, "or") && strcmp(text[pc].func, "sll") && strcmp(text[pc].func, "srl") && strcmp(text[pc].func, "subu") && strcmp(text[pc].func, "sltu") )){
        printf("%d %s\n", pc, text[pc].func);
        //sleep(1);
        Rformat();
        return;
    }
    else if( !( strcmp(text[pc].func, "addiu") && strcmp(text[pc].func, "andi") && strcmp(text[pc].func, "beq") && strcmp(text[pc].func, "lw") && strcmp(text[pc].func, "bne") && strcmp(text[pc].func, "sw") && strcmp(text[pc].func, "sltiu") ) ){
        printf("%d %s\n", pc, text[pc].func);
        //sleep(1);
        Iformat();
        return;
    }
    else{
        printf("%d %s\n", pc, text[pc].func);
        //sleep(1);
        if(!(strcmp(text[pc].func, "la"))){
            pc++;
            char rsstr[11]={};
            char rtstr[11]={};
            int rt=0;
            long long ui=0;
            long long ri=0;
            sscanf(text[pc-1].operand, "%s %s", rtstr, rsstr);
            rt = atoi(rtstr+1);
            int i;
            for(i=0; i<datanum; i++){
                if(!(strcmp(data[i].name, rsstr))){
                    ui = (0x10000000 + 4*i) >> 16;
                    ri = (0x10000000 + 4*i) << 16 >> 16;
                    break;
                }
            }
            if(ri == 0){
                lui(rt, ui);
                return;
            }else{
                printf("!!!!!!!!!!\n");
                lui(rt, ui);
                ori(rt, rt, ri);
                int num;
                for(num=0; num<labelnum; num++){
                    if(pc-1 < label[num].textindex){
                        label[num].textindex += 1;
                    }
                }
                for(num=insnum; num > pc - 1; num--){
                    text[num] = text[num-1];
                }
                pc++;
                insnum++;
                return;
            }
            return;
        }
        if(!(strcmp(text[pc].func, "lui"))){
            pc++;
            int op = 0xf;
            int rs = 0;
            char rtstr[11]={};
            char immstr[11]={};
            sscanf(text[pc-1].operand, "%s %s", rtstr, immstr);
            int rt = atoi(rtstr+1);
            long long imm = strToll(immstr);

            opTobin(op); rTobin(rs); rTobin(rt); iTobin(imm);
            return;
            //R[rt] = imm;
            //printf("lui: op<%d> rs<%d> rt<%d> imm<%lld>\n",op, 0, rt, imm);
        }
        if(!(strcmp(text[pc].func, "ori"))){
            pc++;
            int op = 0xd;
            char rtstr[11]={};
            char rsstr[11]={};
            char immstr[11]={};
            sscanf(text[pc-1].operand, "%s %s %s", rtstr, rsstr, immstr);
            int rt = atoi(rtstr+1);
            int rs = atoi(rsstr+1);
            long long imm = strToll(immstr);

            opTobin(op); rTobin(rs); rTobin(rt); iTobin(imm);
            return;
            //R[rt] = R[rs] | imm;
            //printf("ori: op<%d> rs<%d> rt<%d> imm<%lld>\n",op, rs, rt, imm);
        }  
    }
}





void Iformat(){
    pc++;
    char rsstr[11]={};
    char rtstr[11]={};
    char immstr[11]={};
    int op=0;
    int rs=0;
    int rt=0;
    long long imm=0;
    int branchAddr=0;
    int beq=0;
    int bne=0;
    if(!(strcmp(text[pc-1].func, "addiu"))){ op = 0x9;
        sscanf(text[pc-1].operand, "%s %s %s", rtstr, rsstr, immstr);
        rt = atoi(rtstr+1);
        rs = atoi(rsstr+1);
        imm = strToll(immstr);

        //R[rt] = R[rs] + imm;
    }
    if(!(strcmp(text[pc-1].func, "andi"))){ op = 0xc;
        sscanf(text[pc-1].operand, "%s %s %s", rtstr, rsstr, immstr);
        rt = atoi(rtstr+1);
        rs = atoi(rsstr+1);
        imm = strToll(immstr);

        //R[rt] = R[rs] & imm;
    }
    if(!(strcmp(text[pc-1].func, "beq"))){ op = 0x4;
        sscanf(text[pc-1].operand, "%s %s %s", rsstr, rtstr, immstr);
        rs = atoi(rsstr+1);
        rt = atoi(rtstr+1);
        imm = labelToaddr(immstr);
        imm = imm - pc;

        if(R[rs] == R[rt]){
            beq = 1;
        }
    }
    if(!(strcmp(text[pc-1].func, "bne"))){ op = 0x5;
        sscanf(text[pc-1].operand, "%s %s %s", rsstr, rtstr, immstr);
        rs = atoi(rsstr+1);
        rt = atoi(rtstr+1);
        imm = labelToaddr(immstr);
        imm = imm - pc;

        if(R[rs] != R[rt]){
            bne = 1;
        }
    }
    if(!(strcmp(text[pc-1].func, "sltiu"))){ op = 0xb;
        sscanf(text[pc-1].operand, "%s %s %s", rtstr, rsstr, immstr);
        rt = atoi(rtstr+1);
        rs = atoi(rsstr+1);
        imm = strToll(immstr);

        //R[rt] = R[rs] < imm ? 1 : 0;
    }
    if(!(strcmp(text[pc-1].func, "lw"))){ op = 0x23;
        sscanf(text[pc-1].operand, "%s %[^(]( %s", rtstr, immstr, rsstr);
        rt = atoi(rtstr+1);
        rs = atoi(rsstr+1);
        imm = strToll(immstr);

        //R[rt] = data[R[rs] + imm].value;

    }
    if(!(strcmp(text[pc-1].func, "sw"))){ op = 0x2b;
        sscanf(text[pc-1].operand, "%s %[^(]( %s", rtstr, immstr, rsstr);
        rt = atoi(rtstr+1);
        rs = atoi(rsstr+1);
        imm = strToll(immstr);

        //data[R[rs] + imm].value = R[rt];
    }

    //printf("%s: op<%d> rs<%d> rt<%d> imm<%lld>\n", text[pc-1].func, op, rs, rt, imm);
    if(bne == 1 || beq == 1){

        //printf("R[11] %lld, R[8] %lld\n", R[11], R[8]);
        //pc = pc + branchAddr;
    }
    opTobin(op); rTobin(rs); rTobin(rt); iTobin(imm);

    return;        
}




void Rformat(){
    pc++;
    char rdstr[11]={};
    char rsstr[11]={};
    char rtstr[11]={};
    char shamtstr[11]={};
    int op = 0;
    int rd=0;
    int rs=0;
    int rt=0;
    int shamt=0;
    int func = 0;
    if(!(strcmp(text[pc-1].func, "addu"))){ func = 0x21;
        sscanf(text[pc-1].operand, "%s %s %s", rdstr, rsstr, rtstr);
        rd = atoi(rdstr+1);
        rs = atoi(rsstr+1);
        rt = atoi(rtstr+1);

        //R[rd] = R[rs] + R[rt];
    }
    if(!(strcmp(text[pc-1].func, "sltu"))){ func = 0x2b;
        sscanf(text[pc-1].operand, "%s %s %s", rdstr, rsstr, rtstr);
        rt = atoi(rtstr+1);
        rs = atoi(rsstr+1);
        rd = strToll(rdstr+1);

        //R[rt] = R[rs] < imm ? 1 : 0;
    }
    if(!(strcmp(text[pc-1].func, "and"))){ func = 0x24;
        sscanf(text[pc-1].operand, "%s %s %s", rdstr, rsstr, rtstr);
        rd = atoi(rdstr+1);
        rs = atoi(rsstr+1);
        rt = atoi(rtstr+1);

        //R[rd] = R[rs] & R[rt];
    }
    if(!(strcmp(text[pc-1].func, "jr"))){ func = 0x08;
        sscanf(text[pc-1].operand, "%s", rsstr);
        rs = atoi(rsstr+1);

        //pc = R[rs];
    }
    if(!(strcmp(text[pc-1].func, "nor"))){ func = 0x27;
        sscanf(text[pc-1].operand, "%s %s %s", rdstr, rsstr, rtstr);
        rd = atoi(rdstr+1);
        rs = atoi(rsstr+1);
        rt = atoi(rtstr+1);

        //R[rd] = !(R[rs] | R[rt]);
    }
    if(!(strcmp(text[pc-1].func, "or"))){ func = 0x25;
        sscanf(text[pc-1].operand, "%s %s %s", rdstr, rsstr, rtstr);
        rd = atoi(rdstr+1);
        rs = atoi(rsstr+1);
        rt = atoi(rtstr+1);

        //R[rd] = R[rs] | R[rt];
    }
    if(!(strcmp(text[pc-1].func, "sll"))){ func = 0x00;
        sscanf(text[pc-1].operand,"%s %s %s", rdstr, rtstr, shamtstr);
        rd = atoi(rdstr+1);
        rt = atoi(rtstr+1);
        shamt = atoi(shamtstr);

        //R[rd] = R[rs] << shamt;
    }
    if(!(strcmp(text[pc-1].func, "srl"))){ func = 0x02;
        sscanf(text[pc-1].operand,"%s %s %s", rdstr, rtstr, shamtstr);
        rd = atoi(rdstr+1);
        rt = atoi(rtstr+1);
        shamt = atoi(shamtstr);

        //R[rd] = R[rs] >> shamt;
    }
    if(!(strcmp(text[pc-1].func, "subu"))){ func = 0x23;
        sscanf(text[pc-1].operand, "%s %s %s", rdstr, rsstr, rtstr);
        rd = atoi(rdstr+1);
        rs = atoi(rsstr+1);
        rt = atoi(rtstr+1);

        //R[rd] = R[rs] - R[rt];
    }
    //printf("%s: op<%d> rs<%d> rt<%d> rd<%d> shamt<%d> func<%d>\n", text[pc-1].func, op, rs, rt, rd, shamt, func);
    opTobin(op); rTobin(rs); rTobin(rt); rTobin(rd); rTobin(shamt); opTobin(func);

    return;
}




void Jformat(){
    pc++;
    int op;
    char jumpAddrstr[11]={};
    long long jumpAddr;
    sscanf(text[pc-1].operand, "%s", jumpAddrstr);
    jumpAddr = labelToaddr(jumpAddrstr);
    printf("JUMP !!!! %lld\n", jumpAddr);
    if(!(strcmp(text[pc-1].func, "j"))){ op = 0x2;
        // pc = jumpAddr;
    }
    if(!(strcmp(text[pc-1].func, "jal"))){ op = 0x3;
        // R[31] = pc + 4;
        // pc = jumpAddr;
    }
    /*********************************
    i don't know why +4
    *********************************/
    jumpAddr = jumpAddr*4 + 0x400000;
    jumpAddr = jumpAddr / 4;
    //printf("%s: op<%d> jumpaddr<%lld>\n", text[pc-1].func, op, jumpAddr);
    opTobin(op); jTobin(jumpAddr);

    return;
}








void lui(int rt, long long imm){
    int op = 0xf;
    int rs = 0;

    //R[rt] = imm << 16;
    //printf("lui: op<%d> rs<%d> rt<%d> imm<%lld>\n",op, 0, rt, imm);
    opTobin(op); rTobin(rs); rTobin(rt); iTobin(imm);
    
    return;
}
void ori(int rt, int rs, long long imm){
    int op = 0xd;

    //R[rt] = R[rs] | imm;
    //printf("ori: op<%d> rs<%d> rt<%d> imm<%lld>\n",op, rs, rt, imm);
    opTobin(op); rTobin(rs); rTobin(rt); iTobin(imm);

    return;
}

long long strToll(char* str){
    if(str[0] == '0'){//if hex
        return strtoll(str, NULL, 16);
    }else{
        return strtoll(str, NULL, 10);
    }
}

int labelToaddr(char* str){
    int i;
    for(i=0; i<labelnum; i++){
        if(!(strcmp(label[i].name, str))){
            return label[i].textindex;
        }
    }
    return 0;
}

void opTobin(int k){
    char* bin = (char*)malloc(sizeof(char)*7);
    int i;
    for(i=0; i<7-1; i++){
        bin[i] = '0';
    }
    bin[7-1] = '\0';
    int j=1;
    while (k > 0){
        bin[6 - j] = (k % 2) + '0';
        k = k / 2;
        j++;
    }

    strcat(result, bin);
    free(bin);
    return;
}

void rTobin(int k){
    char* bin = (char*)malloc(sizeof(char)*6);
    int i;
    for(i=0; i<6-1; i++){
        bin[i] = '0';
    }
    bin[6-1] = '\0';
    int j=1;
    while (k > 0){
        bin[5 - j] = (k % 2) + '0';
        k = k / 2;
        j++;
    }

    strcat(result, bin);
    free(bin);
    return;
}

void iTobin(long long k){
    int flag=0;
    if(k<0){
        k = -k;
        flag = 1;
    }
    char* bin = (char*)malloc(sizeof(char)*17);
    int i;
    for(i=0; i<17-1; i++){
        bin[i] = '0';
    }
    bin[17-1] = '\0';
    int j=1;
    while (k > 0){
        bin[16 - j] = (k % 2) + '0';
        k = k / 2;
        j++;
    }
    if(flag == 1){
        for(i=0; i<17-1; i++){
            if(bin[i] == '0'){
                bin[i] = '1';
                continue;
            }
            bin[i] = '0';
        }
        bin[15] += 1;
        for(i=0; i<14; i++){
            if(bin[15-i] == '2'){
                bin[15-i] = '0';
                bin[14-i] += 1;
            }
        }
    }


    strcat(result, bin);
    free(bin);
    return;
}

void jTobin(long long k){
    char* bin = (char*)malloc(sizeof(char)*27);
    int i;
    for(i=0; i<27-1; i++){
        bin[i] = '0';
    }
    bin[27-1] = '\0';
    int j=1;
    while (k > 0){
        printf("!!!!!!!!%lld\n", k);
        bin[26 - j] = (k % 2) + '0';
        k = k / 2;
        j++;
    }
    printf("%s\n", bin);
    strcat(result, bin);
    free(bin);
    return;
}














