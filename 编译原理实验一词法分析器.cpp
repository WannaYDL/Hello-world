#include <iostream>
#include <cstring>
#include <cstdio>
#include <map>
#include <stack>
using namespace std;
/*
<begin,1>   <if,2>
<then,3>    <while,4>
<do,5>      <end,6>
<letter(letter|digit)*,10>  <digit digit*,11>
<+,13>      <-,14>
<*,15>      </,16>
<:,17>      <:=,18>
<<,20>      <<>,21>
<<=,22>     <>,23>
<>=,24>     <=,25>
<;,26>      <(,27>
<),28>      <#,0>
map<int, string> mapStudent;

    mapStudent.insert(pair<int, string>(1, "student_one"));

    mapStudent.insert(pair<int, string>(2, "student_two"));

    mapStudent.insert(pair<int, string>(3, "student_three"));

    map<int, string>::iterator iter;

    for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)

       cout<<iter->first<<' '<<iter->second<<endl;

*/

char resourceProject[1000]; //Դ�����Ŵ�
char token[20]={0};         //ÿ��ɨ���ʱ��洢�Ѿ�ɨ�赽�Ľ��
int syn = -1;                //�ֱ���
int pProject = 0;           //Դ����ָ�룬ָ���ɨ��λ��
//map<string,int> CF;
string CF[1000];
int CFM[1000];
int CFcounter=0;
stack<int>FX;

//ȫ�ֱ������ؼ��֣����ֱ���Ϊ�������ż�һ
static char reserveWord[6][10]={
    "begin","if","then","while","do","end"
};

//������������
static char operOrDelimiter[16][5]={
    "+","-","*","/",":",":=","<","<>","<=",">",">=","=",";","(",")","#"
};
//�������������ֱ���
static int operValue[16]={13,14,15,16,17,18,20,21,22,23,24,25,26,27,28,0};
//��ʶ����
static char IDentifierTbl[1000][50] = { "" };

//���ұ�����
int searchReserve(char reserveWorld[][10],char s[])
{
    for(int i=0;i<6;i++){
        if(strcmp(reserveWord[i],s)== 0){
            //�����ҳɹ������ֱ���
            return i+1;
        }
    }
    return -1;
}

//���ҵ����Ľ���������
int searchOper(char operOrDelimiter[][5],char s[])
{
    for(int i=0;i<16;i++){
        if(strcmp(operOrDelimiter[i],s)==0){
            return operValue[i];
        }
    }
    return -1;
}

//�ж��Ƿ�Ϊ��ĸ
bool IsLetter(char letter)
{
    if(letter>='a'&&letter<='z'||letter=='_'){
        return true;
    }
    else
        return false;
}

//�ж��Ƿ�Ϊ����
bool IsDigit(char digit)
{
    if(digit>='0'&&digit<='9'){
        return true;
    }
    else
        return false;
}

//����Ԥ����
void filterResource(char r[],int pProject)
{
    char tempString[1000];//����޳�
    int count =0;
    for(int i=0;i<=pProject;i++){
        if(r[i]!='\n'&&r[i]!='\t'&&r[i]!='\v'&&r[i]!='\r'){
            tempString[count++]=r[i];
        }
    }
    tempString[count] = '\0';
    strcpy(r,tempString);
}

//�����ӳ���
void Scanner(int &syn,char resourceProject[],char token[],int &pProject)
{
    syn=-1;
    int count = 0;
    char ch;
    ch = resourceProject[pProject];
    //�ҵ���һ���ǿ��ַ�
    while(ch==' '){
        pProject++;
        ch = resourceProject[pProject];
    }
    //���token����
    memset(token,'\0',sizeof(token));
    //���ؼ��ֻ��ʶ��
    if(IsLetter(resourceProject[pProject])){
        count = 0;
        token[count++] = resourceProject[pProject];
        pProject++;
        while(IsLetter(resourceProject[pProject])||IsDigit(resourceProject[pProject])){
            token[count++] = resourceProject[pProject];
            pProject++;
        }
        token[count] = '\0';
        syn = searchReserve(reserveWord,token);
        if(syn==-1){
            syn=10;
        }
        return;
    }
    //��ⳣ��
    if(IsDigit(resourceProject[pProject])){
        count = 0;
        token[count++] = resourceProject[pProject];
        pProject++;
        while(IsDigit(resourceProject[pProject])){
            token[count++] = resourceProject[pProject];
            pProject++;
        }
        token[count] = '\0';
        syn= 11;
        return;
    }
    //����������
    if(ch == '+' || ch == '-' || ch == '*' ||
            ch == '/' || ch == ';' || ch == '(' ||
            ch == ')' || ch == '#' || ch == ':' ||
            ch=='<'||ch=='>'||ch == '=' ){
        count = 0;

        if(ch=='<'){
            token[count++] = resourceProject[pProject];
            pProject++;
            if (resourceProject[pProject]=='>'){
                token[count++] = resourceProject[pProject];
                syn=23;pProject++;
            }
            else if(resourceProject[pProject]=='='){
                token[count++] = resourceProject[pProject];
                syn=22;pProject++;
            }
            else{
                syn=20;
            }
        }
        else if(ch=='>'){
            token[count++] = resourceProject[pProject];
            pProject++;
            if(resourceProject[pProject]=='='){
                token[count++] = resourceProject[pProject];
                syn=24;pProject++;
            }
            else{
                syn=23;
            }
        }
        else if(ch==':'){
            token[count++] = resourceProject[pProject];
            pProject++;
            if(resourceProject[pProject]=='='){
                token[count++] = resourceProject[pProject];
                syn=18;pProject++;
            }
            else{
                syn=17;
            }
        }
        else if(ch == '+' || ch == '-' || ch == '*' ||
            ch == '/' || ch == ';' || ch == '(' ||
            ch == ')' || ch == '#' ||'='){
                token[0]=ch;
                token[1]='\0';
                syn=searchOper(operOrDelimiter,token);
                pProject++;
            }
        return;
    }
    if(syn==-1){
        cout<<"���Ŵ���"<<endl;
        return;
    }
}

//��ӡ�ʷ��������
void Print(int syn,int &pProject)
{
    string ch;
    if(syn == 10){
        for(int i=0;i<1000;i++){
            if(strcmp(IDentifierTbl[i],token)==0){
                break;
            }
            if(IDentifierTbl[i]==""){
                strcpy(IDentifierTbl[i],token);
                break;
            }
        }
        //.insert(pair<int, string>(1, "student_one"));
//        cout<<"��ʶ��"<<endl;
//        cout<<"{ "<<token<<", "<<syn<<" }"<<endl;
        ch=token;
        CF[CFcounter++]=ch;CFM[CFcounter-1]=syn;
    }
    else if(syn>=1&&syn<=6){
//        cout<<"�ؼ���"<<endl;
//        cout<<"{ "<<token<<", "<<syn<<" }"<<endl;
        ch=token;
        CF[CFcounter++]=ch;CFM[CFcounter-1]=syn;
    }
    else if(syn==11){
//        cout<<"����"<<endl;
//        cout<<"{ "<<token<<", "<<syn<<" }"<<endl;
        ch=token;
        CF[CFcounter++]=ch;CFM[CFcounter-1]=syn;
    }
    else if((syn>=13&&syn<=18)||(syn>=20&&syn<=28)){
//        cout<<"����������"<<endl;
//        cout<<"{ "<<token<<", "<<syn<<" }"<<endl;
        ch=token;
        CF[CFcounter++]=ch;CFM[CFcounter-1]=syn;
    }
    else if(syn==0){
//        cout<<"{ "<<token<<", "<<syn<<" }"<<endl;
        ch=token;
        CF[CFcounter++]=ch;CFM[CFcounter-1]=syn;
        //break;
    }

    else{
//        cout<<"error"<<endl;
        pProject++;
    }
//    for(int i=0;i<CFcounter;i++)
//        cout<<CF[i]<<"  "<<CFM[i]<<endl;
}
//-----------------------------------------------------------------------------

//�﷨������
int statement(int syn)
{
    int flag = 1;
    int X;
    X = FX.top();
    if(X==31){
        TD31(syn);
    }
    else if(X==32){
        TD32(syn);
    }
    else if(X==33){
        TD33(syn);
    }
    else if(X==34){
        TD34(syn);
    }
    else if(X==35){
        TD35(syn);
    }
    else if(X==36){
        TD36(syn);
    }
    else if(X==37){
        TD37(syn);
    }
    else if(X==38){
        TD38(syn);
    }
    else if(X==39){
        TD39(syn);
    }
    else if(X==syn){
        FX.pop()
    }
    else if(){
        cout<<"error sentence 4 !"<<endl;
        flag=0;
    }
    return flag;
}

int main()
{
    pProject = 0;
    syn=-1;
    char ch;
    int p=1;
    cout<<"������Դ����"<<endl;

    while(scanf("%c",&ch)&&ch!='#'){
        resourceProject[pProject++]=ch;
    }
    resourceProject[pProject]='#';resourceProject[++pProject]='\0';
    cout<<"�������Դ����Ϊ��"<<endl;
    cout<<resourceProject<<endl;
    cout<<"����Ԥ����Ľ��Ϊ��"<<endl;
    filterResource(resourceProject,pProject);
    cout<<resourceProject<<endl;
    pProject=0;
    //while(syn!=0){
    Scanner(syn,resourceProject,token,pProject);
    Print(syn,pProject);
    if(syn!=1){
        cout<<"error sentence 1!"<<endl;
    }
    else{
        Scanner(syn,resourceProject,token,pProject);
        Print(syn,pProject);
        FX.push(0);
        FX.push(31);
        while(!FX.empty()&&p!=0){
            p=statement(syn);
            Scanner(syn,resourceProject,token,pProject);
            Print(syn,pProject);
        }


        if(syn==6){
            Scanner(syn,resourceProject,token,pProject);
            if(syn==0){
                cout<<"success!"<<endl;
            }
            else
                cout<<"error sentence 2 !"<<endl;
        }
        else
            cout<<"error sentence 3 !"<<endl;
    }

    //}

    return 0;
}




