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

char resourceProject[1000]; //源程序存放处
char token[20]={0};         //每次扫描的时候存储已经扫描到的结果
int syn = -1;                //种别码
int pProject = 0;           //源程序指针，指向待扫描位置
//map<string,int> CF;
string CF[1000];
int CFM[1000];
int CFcounter=0;
stack<int>FenXi;

//全局变量，关键字，其种别码为其索引号加一
static char reserveWord[6][10]={
    "begin","if","then","while","do","end"
};

//界符和运算符表
static char operOrDelimiter[16][5]={
    "+","-","*","/",":",":=","<","<>","<=",">",">=","=",";","(",")","#"
};
//界符和运算符的种别码
static int operValue[16]={13,14,15,16,17,18,20,21,22,23,24,25,26,27,28,0};
//标识符表
static char IDentifierTbl[1000][50] = { "" };

//查找保留字
int searchReserve(char reserveWorld[][10],char s[])
{
    for(int i=0;i<6;i++){
        if(strcmp(reserveWord[i],s)== 0){
            //若查找成功返回种别码
            return i+1;
        }
    }
    return -1;
}

//查找单独的界符或运算符
int searchOper(char operOrDelimiter[][5],char s[])
{
    for(int i=0;i<16;i++){
        if(strcmp(operOrDelimiter[i],s)==0){
            return operValue[i];
        }
    }
    return -1;
}

//判断是否为字母
bool IsLetter(char letter)
{
    if(letter>='a'&&letter<='z'||letter=='_'){
        return true;
    }
    else
        return false;
}

//判断是否为数字
bool IsDigit(char digit)
{
    if(digit>='0'&&digit<='9'){
        return true;
    }
    else
        return false;
}

//编译预处理
void filterResource(char r[],int pProject)
{
    char tempString[1000];//存放剔除
    int count =0;
    for(int i=0;i<=pProject;i++){
        if(r[i]!='\n'&&r[i]!='\t'&&r[i]!='\v'&&r[i]!='\r'){
            tempString[count++]=r[i];
        }
    }
    tempString[count] = '\0';
    strcpy(r,tempString);
}

//分析子程序
void Scanner(int &syn,char resourceProject[],char token[],int &pProject)
{
    syn=-1;
    int count = 0;
    char ch;
    ch = resourceProject[pProject];
    //找到第一个非空字符
    while(ch==' '){
        pProject++;
        ch = resourceProject[pProject];
    }
    //清空token数组
    memset(token,'\0',sizeof(token));
    //检测关键字或标识符
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
    //检测常量
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
    //界符或运算符
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
        cout<<"符号错误！"<<endl;
        return;
    }
}

//打印词法分析结果,并保存在CF和CFM中
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
//        cout<<"标识符"<<endl;
//        cout<<"{ "<<token<<", "<<syn<<" }"<<endl;
        ch=token;                                   //token数组转为string类型
        CF[CFcounter++]=ch;CFM[CFcounter-1]=syn;    //保存种别码
    }
    else if(syn>=1&&syn<=6){
//        cout<<"关键字"<<endl;
//        cout<<"{ "<<token<<", "<<syn<<" }"<<endl;
        ch=token;
        CF[CFcounter++]=ch;CFM[CFcounter-1]=syn;
    }
    else if(syn==11){
//        cout<<"常数"<<endl;
//        cout<<"{ "<<token<<", "<<syn<<" }"<<endl;
        ch=token;
        CF[CFcounter++]=ch;CFM[CFcounter-1]=syn;
    }
    else if((syn>=13&&syn<=18)||(syn>=20&&syn<=28)){
//        cout<<"界符或运算符"<<endl;
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
//-----------------------------语法分析函数------------------------------------------------
/*
P   -> begin YC end
31 YC ->YJ YF
32 YJ -> F
33 YF -> ;YJYF
34 F   -> ID :=B
35 B  -> X XF
36 X  -> Y  YE
37 Y  -> ID | NUM | ( B )
38 XF->+X XF|-X XF|ε
39 YE->*YYE|/YYE|ε
*/

//调用第31推导式
int TuiDao31(int &syn)
{
    if(syn==10){
        FenXi.pop();
        FenXi.push(33);
        FenXi.push(32);
        return 1;
    }
    else{
        return 0;
    }
}
//调用第32推导式
int TuiDao32(int &syn)
{
    if(syn==10){
        FenXi.pop();
        FenXi.push(34);
        return 1;
    }
    else
        return 0;
}
//调用第33推导式
int TuiDao33(int &syn)
{
    if(syn==26){
        FenXi.pop();
        FenXi.push(33);
        FenXi.push(32);
        FenXi.push(26);
        return 1;
    }
    else
        return 0;
}
//调用第34推导式
int TuiDao34(int &syn)
{
    if(syn==10){
        FenXi.pop();
        FenXi.push(35);
        FenXi.push(18);
        FenXi.push(10);
        return 1;
    }
    else
        return 0;
}
//调用第35推导式
int TuiDao35(int &syn)
{
    if(syn==11){
        FenXi.pop();
        FenXi.push(38);
        FenXi.push(36);
        return 1;
    }
    else if(syn==27){
        FenXi.pop();
        FenXi.push(38);
        FenXi.push(36);
        return 1;
    }
    else if(syn==10){
        FenXi.pop();
        FenXi.push(38);
        FenXi.push(36);
        return 1;
    }
    else
        return 0;
}
//调用第36推导式
int TuiDao36(int &syn)
{
    if(syn==11){
        FenXi.pop();
        FenXi.push(39);
        FenXi.push(37);
        return 1;
    }
    else if(syn==27){
        FenXi.pop();
        FenXi.push(39);
        FenXi.push(37);
        return 1;
    }
    else if(syn==10){
        FenXi.pop();
        FenXi.push(39);
        FenXi.push(37);
        return 1;
    }
    else
        return 0;
}
//调用第37推导式
int TuiDao37(int &syn)
{
    if(syn==11){
        FenXi.pop();
        FenXi.push(11);
        return 1;
    }
    else if(syn==27){
        FenXi.pop();
        FenXi.push(28);
        FenXi.push(35);
        FenXi.push(27);
        return 1;
    }
    else if(syn==10){
        FenXi.pop();
        FenXi.push(10);
        return 1;
    }
    else
        return 0;
}
//调用第38推导式
int TuiDao38(int &syn)
{
    if(syn==26){
        FenXi.pop();
        return 1;
    }
    else if(syn==13){
        FenXi.pop();
        FenXi.push(38);
        FenXi.push(36);
        FenXi.push(13);
        return 1;
    }
    else if(syn==14){
        FenXi.pop();
        FenXi.push(38);
        FenXi.push(36);
        FenXi.push(14);
        return 1;
    }
    else if(syn==28){
        FenXi.pop();
        return 1;
    }
    else
        return 0;
}
//调用第39推导式
int TuiDao39(int &syn)
{
    if(syn==16){
        FenXi.pop();
        FenXi.push(39);
        FenXi.push(37);
        FenXi.push(16);
        return 1;
    }
    else if(syn==13){
        FenXi.pop();
        return 1;
    }
    else if(syn==14){
        return 1;
    }
    else if(syn==15){
        FenXi.pop();
        FenXi.push(39);
        FenXi.push(37);
        FenXi.push(15);
        return 1;
    }
    else if(syn==28){
        FenXi.pop();
        return 1;
    }
     else if(syn==26){
        FenXi.pop();
        return 1;
    }
    else
        return 0;
}

/*
语法分析器  根据文法FIRST集和FOLLOW集分别调用相应的函数
31至39分别为非终结符的编号
TuiDao函数为某一推导式的调用
*/
int statement(int &syn)
{
    int flag = 1;   //flag用来存放在预测分析表的匹配的状态，成功为1失败为0
    int X;          //X存放当前栈顶值
    X = FenXi.top();
    //cout<<syn<<"  "<<X<<endl;
    //利用预测分析表进行预测分析
    if(X==31){
        flag=TuiDao31(syn);
        //cout<<syn<<"  "<<X<<"  "<<flag<<endl;
    }
    else if(X==32){
        flag=TuiDao32(syn);
        //cout<<syn<<"  "<<X<<"  "<<flag<<endl;
    }
    else if(X==33){
        flag=TuiDao33(syn);
        //cout<<syn<<"  "<<X<<"  "<<flag<<endl;
    }
    else if(X==34){
        flag=TuiDao34(syn);
        //cout<<syn<<"  "<<X<<"  "<<flag<<endl;
    }
    else if(X==35){
        flag=TuiDao35(syn);
        //cout<<syn<<"  "<<X<<"  "<<flag<<endl;
    }
    else if(X==36){
        flag=TuiDao36(syn);
        //cout<<syn<<"  "<<X<<"  "<<flag<<endl;
    }
    else if(X==37){
        flag=TuiDao37(syn);
        //cout<<syn<<"  "<<X<<"  "<<flag<<endl;
    }
    else if(X==38){
        flag=TuiDao38(syn);
        //cout<<syn<<"  "<<X<<"  "<<flag<<endl;
    }
    else if(X==39){
        flag=TuiDao39(syn);
        //cout<<syn<<"  "<<X<<"  "<<flag<<endl;
    }
    //栈顶元素与输入程序的种别码一致时，弹出栈顶元素，匹配成功，更新syn的值，并保存syn
    else if(X==syn){
        //cout<<syn<<"  "<<X<<"  "<<flag<<endl;
        FenXi.pop();
        Scanner(syn,resourceProject,token,pProject);
        Print(syn,pProject);
        //cout<<"now syn"<<syn<<endl;
    }
    else {
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
    cout<<"请输入源程序："<<endl;

    while(scanf("%c",&ch)&&ch!='#'){
        resourceProject[pProject++]=ch;
    }
    resourceProject[pProject]='#';resourceProject[++pProject]='\0';
    cout<<"您输入的源程序为："<<endl;
    cout<<resourceProject<<endl;
    cout<<"编译预处理的结果为："<<endl;
    filterResource(resourceProject,pProject);
    cout<<resourceProject<<endl;
    pProject=0;
    //while(syn!=0){
    Scanner(syn,resourceProject,token,pProject);
    Print(syn,pProject);
    //若输入的第一个种别码不是begin的种别码，直接报错
    if(syn!=1){
        cout<<"error sentence 1!"<<endl;
    }
    else{
        Scanner(syn,resourceProject,token,pProject);
        Print(syn,pProject);
        FenXi.push(0);
        FenXi.push(31);
        //该循环只识别begin后，end前的输入内容，当输入为end时，p的值为0，退出循环
        while(!FenXi.empty()&&p!=0){
            p=statement(syn);
            //cout<<FenXi.top()<<"  "<<p<<"  "<<syn<<endl;
        }
        //begin与end之间若存在end，则可判断该语句为错误语句
        if(syn==6){ //运行过程中碰到end时直接执行该句
            Scanner(syn,resourceProject,token,pProject);
            if(syn==0){
                cout<<"success!"<<endl;
                while(!FenXi.empty()){
                    FenXi.pop();
                }
            }
            else
                cout<<"error sentence 2 !"<<endl;
        }
        else
            cout<<"error sentence 3 !"<<endl;
        //若分析失败将栈内元素全部退出
        while(!FenXi.empty()){
            cout<<FenXi.top()<<endl;
            FenXi.pop();
        }
    }
    return 0;
}

