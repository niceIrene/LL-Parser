#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED
#include<iostream>
#include<string>
#include<vector>
#include<set>
#include<fstream>
#include<algorithm>
#include<stdlib.h>

using namespace std;

int asc=65;//65对应‘A“
struct term//非终结符对应的结构体
{
    char ch;
    vector<string> produce;//将产生式分开
    vector<string> produce1;//消除左递归之后
    vector<string> produce2;//提取左公因子之后
    vector<char> first[100];//不同产生式的first集合
    vector<char> follow;
};
struct item
{
    vector<string> str;//输出存储输出
};
class base
{
private:
    char start;
    vector<char> ter;//终结符集
    vector<char> non;//非终结符集
    vector<string> str;//生成式集
    term all[26];
    //构造非终结符struct的过程,里面保存了每个非终结符号的产生式
public:
    base(string file_in);
    ~base(){}
    bool isNon(char ch);
    bool isTer(char ch);
    int getIndex(char ch);//得到在非终结符集中的下标
    int indexInStr(int a,int b,char ch);
    void getFirst(char c);
    void show();
    int isDirect(int a);//接下来的两个是用来消除左递归的函数
    void DirectRemove();
    char getNewNon(int asc);//在从A到Z中选择不属于非终结符集合的字符加入
    int getTer(int a);
    bool getFollow(int big,char c);
    void getLeft();
    void getLeft(char c);
    void makeTable();
    bool isInFirst(int i,char ch);
    bool isInFollow(int i,char ch);
    bool contain(int index,char ch);
};

base::base(string file_in)//构造函数，得到终结符集以及非终结符集
{
    ifstream in;
    in.open(file_in);
    if(!in){cout<<"open file error";exit(1);}
    string temp;
    while(in)
    {
        getline(in,temp,'\n');
        cout<<temp<<endl;
        str.push_back(temp);
    }
    vector<char>::iterator it,it2,it3;
    for(int i=0;i<str.size();i++)
    {
        it=find(non.begin(),non.end(),str[i][0]);
        if(it==non.end())
        non.push_back(str[i][0]);//得到了非终结符集
    }
    for(int i=0;i<str.size();i++)
    {
        for(int j=3;j<str[i].length();j++)
        {
            it2=find(ter.begin(),ter.end(),str[i][j]);//不在已有的终结符集中
            it3=find(non.begin(),non.end(),str[i][j]);//也不属于非终结符集
            if(it2==ter.end()&&it3==non.end()&&str[i][j]!='|')
                ter.push_back(str[i][j]);
        }
    }

    for(int i=0;i<non.size();i++)
   {
        all[i].ch=non[i];
        int index=getIndex(non[i]);//对输入的符号求在非终结符集合中的坐标
        int tmp=3;
        string tmpString;
        while(1)
        {
            if(indexInStr(index,tmp,'|')==0) break;
            tmpString=str[index].substr(tmp,indexInStr(index,tmp,'|')-tmp);
            all[i].produce.push_back(tmpString);
            tmp=indexInStr(index,tmp,'|')+1;
        }
        tmpString=str[index].substr(tmp,str[index].length()-tmp);
        all[i].produce.push_back(tmpString);
    }
    DirectRemove();
    start=non[0];
    getLeft();
    show();
}

bool base::isNon(char ch)//判断是否在非终结符
{
    vector<char>::iterator it;
    it=find(non.begin(),non.end(),ch);
    if(it==non.end()) return false;
    else return true;
}

bool base::isTer(char ch)//判断是否在终结符
{
     vector<char>::iterator it2;
    it2=find(ter.begin(),ter.end(),ch);
    if(it2==ter.end()) return false;
    else return true;
}

char base::getNewNon(int asc)//产生新的非终结符号的函数
{
    char tmp=char(asc);
    while(asc<90)
    {
        if(!isNon(tmp)) return tmp;
        asc++;
        tmp=char(asc);
    }
}

int base::getIndex(char ch)
{
    int result=0;
    for(int i=0;i<non.size();i++)
    {
        if(non[result]==ch) return result;
        result++;
    }
}

int base::indexInStr(int a,int b,char ch)//a代表第几个非终结符号代表的序列,b表示从第几个字符开始向后找
{
    int temp=b;
    for(int i=b;i<str[a].length();i++)
    {
        if(str[a][i]==ch)return temp;//若在返回下标，若不在返回0
        temp++;
    }
    return 0;
}

void base::getFirst(char c)//对每一个非终结符的产生式求first
{
    int index=getIndex(c);
    for(int i=0;i<all[index].produce2.size();i++)//遍历每一条产生式
    {
       int needEmpty=1;bool emp;
       for(int j=0;j<all[index].produce2[i].length();j++)
       {
           emp=0;
           if(isTer(all[index].produce2[i][j])||all[index].produce2[i][j]=='#')//为终结符
                 {all[index].first[i].push_back(all[index].produce2[i][j]);break;}//得到第i条产生式的非终结符集合
           else{
                getFirst(all[index].produce2[i][j]);
                int tmp=getIndex(all[index].produce2[i][j]);
                for(int k=0;k<all[tmp].produce2.size();k++)//遍历该非终结符的所有产生式
                {
                     int indextmp=0;
                     while(all[tmp].first[k][indextmp]!='\0')
                        {
                            if(all[tmp].first[k][indextmp]=='#')
                            {
                                emp=1;indextmp++;continue;
                            }
                            all[index].first[i].push_back(all[tmp].first[k][indextmp]);
                            indextmp++;
                        }
                }
                if(!emp) break;
           }
           needEmpty++;
       }
       if(needEmpty==all[index].produce2[i].length()&&emp) all[index].first[i].push_back('#');
       all[index].first[i].push_back('\0');//放入\0便于操作
    }
}

void base::show()
{
    for(int i=0;i<non.size();i++)
        getFirst(non[i]);
    bool con=1;
    all[0].follow.push_back('$');//对开始符的处理
    while(con==1)
    {
        con=0;
        for(int i=0;i<non.size();i++)
        {
            bool flag=getFollow(non.size(),non[i]);
            if(flag) con=1;
        }
    }
    for(int i=0;i<non.size();i++)
        {
            cout<<all[i].ch<<" 的所有first集合为 "<<endl;
            for(int j=0;j<all[i].produce2.size();j++)
            {
                 int k=0;
                while(all[i].first[j][k]!='\0')
                {
                    cout<<all[i].first[j][k]<<' ';
                    k++;
                }
                cout<<endl;
            }
            cout<<all[i].ch<<" 的follow集合为 "<<endl;
            for(int j=0;j<all[i].follow.size();j++)
                cout<<all[i].follow[j]<<' ';
            cout<<endl;
        }
}

int base::isDirect(int a)//对第a个产生式判断是否为左递归
{
    for(int i=0;i<all[a].produce.size();i++)
         if(all[a].produce[i][0]==all[a].ch) return i;
    return (-1);
}
int base::getTer(int a)//得到用来代替的非终结符
{
    for(int i=0;i<all[a].produce.size();i++)
    {
         if((isTer(all[a].produce[i][0])||isNon(all[a].produce[i][0]))&&all[a].produce[i][0]!='#'&&all[a].produce[i][0]!=all[a].ch)
        {
             return i;
        }
    }
}
void base::DirectRemove()
{
    for(int i=0;i<non.size();i++)
    {
        if(isDirect(i)==-1)
        {
            for(int j=0;j<all[i].produce.size();j++)
        {
            all[i].produce1.push_back(all[i].produce[j]);
        }
        }//没有左递归的时候直接抄过去
        while(isDirect(i)!=-1)//当存在左递归的时候
        {
            int a=isDirect(i);//哪条产生式发生了左递归
            char ch=getNewNon(asc);
            all[non.size()].ch=ch;
            non.push_back(ch);asc++;//新建产生式
            string tmpString=all[i].produce[a].substr(1,all[i].produce[a].length()-1)+ch;
            all[non.size()-1].produce.push_back(tmpString);
            all[non.size()-1].produce.push_back("#");
            tmpString=all[i].produce[getTer(i)];//更改旧的产生式
            tmpString+=ch;
            all[i].produce1.push_back(tmpString);
            break;
        }
    }
}
bool base::contain(int index,char ch)
{
    for(int i=0;i<all[index].follow.size();i++)
        if(all[index].follow[i]==ch)
          return true;
    return false;
}
bool base::getFollow(int big,char c)
{
    bool emp=0;bool change=0;
    int index=getIndex(c);
       for(int i=0;i<all[index].produce2.size();i++)//遍历所有的产生式
      {
        int last=1;//notice :length不包括空字符所占用的空间
        for(int j=0;j<all[index].produce2[i].length();j++)//扫描产生式
        {
            if(isNon(all[index].produce2[i][j])&&last==all[index].produce2[i].length())//当遇到一个排在末尾的非终结符
            {cout<<all[index].produce2[i][j];
                int tmpIndex=getIndex(all[index].produce2[i][j]);
                if(all[index].produce2[i][j]==all[index].ch) continue;//防死锁
                for(int k=0;k<all[index].follow.size();k++)//将C的follow集合加入到该非终结符的follow中
                    {
                        if(contain(tmpIndex,all[index].follow[k])) continue;
                        change=1;
                        all[tmpIndex].follow.push_back(all[index].follow[k]);
                    }
            }
            else if(isNon(all[index].produce2[i][j])&&(isNon(all[index].produce2[i][j+1])||isTer(all[index].produce2[i][j+1])))//该非终结符后面还有符号
                    {
                        int tmpIndex=getIndex(all[index].produce2[i][j]);
                        if(all[index].produce2[i][j]==all[index].ch) continue;//防死锁
                        char ch=all[index].produce2[i][j+1];//后面的非终结符
                        if(isTer(ch)) {if(contain(tmpIndex,ch)) continue;change=1;all[tmpIndex].follow.push_back(ch);}//直接加入
                        else{
                              int a=getIndex(ch);
                              for(int h=0;h<all[a].produce2.size();h++)
                                for(int g=0;g<all[a].first[i].size();g++)
                                    {
                                        if(all[a].first[h][g]=='#') {emp=1;continue;}
                                        if(contain(tmpIndex,all[a].first[h][g])) continue;
                                        change=1;
                                        all[tmpIndex].follow.push_back(all[a].first[h][g]);
                                    }//将first集合加入
                              if(emp){
                                        for(int k=0;k<all[index].follow.size();k++)//将C的follow集合加入到该非终结符的follow中
                                          {
                                              if(contain(tmpIndex,all[index].follow[k]))
                                                 continue;
                                              change=1;
                                              all[tmpIndex].follow.push_back(all[index].follow[k]);
                                          }
                              }
                        }
                    }
            last++;
        }
      }
      return change;
}

void base::getLeft()
{
    for(int i=0;i<non.size();i++)
    {
        getLeft(non[i]);
    }
}

void base::getLeft(char c)
{
   int index=getIndex(c);
   bool left[all[index].produce1.size()];
   for(int i=0;i<10;i++) {left[i]=0;}
   int produce2Index=0;
   for(int i=0;i<all[index].produce1.size();i++)//扫描产生式
   {
       int j=i+1;
       int num=1;//有几个是共享公共子串的
       int arr[10];//记录下标
       for(j;j<all[index].produce1.size();j++)
       {
          if((all[index].produce1[i][0]==all[index].produce1[j][0])&&left[i]==0&&left[j]==0)
               {num++;arr[0]=i;arr[num-1]=j;left[j]=1;}
       }
       if(num>1){
                    char newChar=getNewNon(asc);
                    all[non.size()].ch=newChar;
                    non.push_back(newChar);asc++;//新建产生式
                    for(int j=0;j<num;j++)
                    {
                        string tmp;
                        int c=arr[j];
                        if(all[index].produce1[c].length()==1){all[non.size()-1].produce1.push_back("#");cout<<all[non.size()-1].produce1[1];break;}
                        tmp=all[index].produce1[c].substr(1,all[index].produce1[c].length()-1);
                        all[non.size()-1].produce1.push_back(tmp);cout<<all[non.size()-1].produce1[0];
                    }
                    cout<<produce2Index;
                    int c=arr[0];
                    string tmp=all[index].produce1[i].substr(0,1);
                    tmp+=newChar;
                    all[index].produce2.push_back(tmp);
                    cout<<all[index].produce2[produce2Index];
                    left[i]=1;
                    produce2Index++;break;
       }
       else{
        string tmp=all[index].produce1[i];
        all[index].produce2.push_back(tmp);
        produce2Index++;
        left[i]=1;
       }
   }
}

bool base::isInFirst(int i,char ch)
{
    for(int j=0;j<all[i].produce2.size();j++)
        for(int k=0;k<all[i].first[j].size();k++)
           if(all[i].first[j][k]==ch)
              return true;
    return false;
}
bool base::isInFollow(int i,char ch)
{
    for(int j=0;j<all[i].follow.size();j++)
       if(all[i].follow[j]==ch)
          return true;
    return false;
}
void base::makeTable()
{
   ter.push_back('#');
   ter.push_back('$');
   int col=ter.size();int row=non.size();
   item it[row][col];bool flag[row][col];
   for(int i=0;i<row;i++)
    for(int j=0;j<col;j++)
      flag[i][j]=0;
   for(int i=0;i<row;i++)
    for(int j=0;j<col;j++)
   {
       char ch=ter[j];//现在处理的终结符
       int a;
       if(it[i][j].str.size()>1)
        it[i][j].str.push_back("Conflict");//冲突处理
       else{
           if(isInFirst(i,ch))
       {
           for(int j=0;j<all[i].produce2.size();j++)
              for(int k=0;k<all[i].first[j].size();k++)
                if(all[i].first[j][k]==ch)
                  a=j;
           it[i][j].str.push_back(all[i].produce2[a]);
           flag[i][j]=1;
       }
       if(isInFirst(i,'#')&&isInFollow(i,ch))
       {
           cout<<all[i].ch<<' ';
           for(int j=0;j<all[i].produce2.size();j++)
              for(int k=0;k<all[i].first[j].size();k++)
                if(all[i].first[j][k]=='#')
                    a=j;
           it[i][j].str.push_back(all[i].produce2[a]);
           flag[i][j]=1;
       }
       else it[i][j].str.push_back("Error");
       }
   }
   ofstream output("output.txt");
   char k=' ';
   for(int i=0;i<2;i++)
    output<<k;
   for(int i=0;i<ter.size();i++)
   {
       output<<'|';
       output<<ter[i];
       for(int j=0;j<10;j++)
        output<<k;
   }
   output<<'\n';
   for(int k=0;k<2+11*ter.size();k++)
    output<<'-';
    output<<'\n';
   for(int i=0;i<non.size();i++)
   {
       output<<non[i]<<' ';
       for(int j=0;j<ter.size();j++)
       {
           int need;
           output<<'|';
           if(flag[i][j])
              {output<<all[i].ch<<"->"<<it[i][j].str[0];need=3;}
           else
              {output<<it[i][j].str[0];need=0;}
           for (int k=0;k<11-need-it[i][j].str[0].length();k++)
              output<<" ";
       }
       output<<'\n';
          for(int k=0;k<2+11*ter.size();k++)
            output<<'-';
        output<<'\n';
   }
   output.close();
}

#endif // BASE_H_INCLUDED
