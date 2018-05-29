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

int asc=65;//65��Ӧ��A��
struct term//���ս����Ӧ�Ľṹ��
{
    char ch;
    vector<string> produce;//������ʽ�ֿ�
    vector<string> produce1;//������ݹ�֮��
    vector<string> produce2;//��ȡ������֮��
    vector<char> first[100];//��ͬ����ʽ��first����
    vector<char> follow;
};
struct item
{
    vector<string> str;//����洢���
};
class base
{
private:
    char start;
    vector<char> ter;//�ս����
    vector<char> non;//���ս����
    vector<string> str;//����ʽ��
    term all[26];
    //������ս��struct�Ĺ���,���汣����ÿ�����ս���ŵĲ���ʽ
public:
    base(string file_in);
    ~base(){}
    bool isNon(char ch);
    bool isTer(char ch);
    int getIndex(char ch);//�õ��ڷ��ս�����е��±�
    int indexInStr(int a,int b,char ch);
    void getFirst(char c);
    void show();
    int isDirect(int a);//������������������������ݹ�ĺ���
    void DirectRemove();
    char getNewNon(int asc);//�ڴ�A��Z��ѡ�����ڷ��ս�����ϵ��ַ�����
    int getTer(int a);
    bool getFollow(int big,char c);
    void getLeft();
    void getLeft(char c);
    void makeTable();
    bool isInFirst(int i,char ch);
    bool isInFollow(int i,char ch);
    bool contain(int index,char ch);
};

base::base(string file_in)//���캯�����õ��ս�����Լ����ս����
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
        non.push_back(str[i][0]);//�õ��˷��ս����
    }
    for(int i=0;i<str.size();i++)
    {
        for(int j=3;j<str[i].length();j++)
        {
            it2=find(ter.begin(),ter.end(),str[i][j]);//�������е��ս������
            it3=find(non.begin(),non.end(),str[i][j]);//Ҳ�����ڷ��ս����
            if(it2==ter.end()&&it3==non.end()&&str[i][j]!='|')
                ter.push_back(str[i][j]);
        }
    }

    for(int i=0;i<non.size();i++)
   {
        all[i].ch=non[i];
        int index=getIndex(non[i]);//������ķ������ڷ��ս�������е�����
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

bool base::isNon(char ch)//�ж��Ƿ��ڷ��ս��
{
    vector<char>::iterator it;
    it=find(non.begin(),non.end(),ch);
    if(it==non.end()) return false;
    else return true;
}

bool base::isTer(char ch)//�ж��Ƿ����ս��
{
     vector<char>::iterator it2;
    it2=find(ter.begin(),ter.end(),ch);
    if(it2==ter.end()) return false;
    else return true;
}

char base::getNewNon(int asc)//�����µķ��ս���ŵĺ���
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

int base::indexInStr(int a,int b,char ch)//a����ڼ������ս���Ŵ��������,b��ʾ�ӵڼ����ַ���ʼ�����
{
    int temp=b;
    for(int i=b;i<str[a].length();i++)
    {
        if(str[a][i]==ch)return temp;//���ڷ����±꣬�����ڷ���0
        temp++;
    }
    return 0;
}

void base::getFirst(char c)//��ÿһ�����ս���Ĳ���ʽ��first
{
    int index=getIndex(c);
    for(int i=0;i<all[index].produce2.size();i++)//����ÿһ������ʽ
    {
       int needEmpty=1;bool emp;
       for(int j=0;j<all[index].produce2[i].length();j++)
       {
           emp=0;
           if(isTer(all[index].produce2[i][j])||all[index].produce2[i][j]=='#')//Ϊ�ս��
                 {all[index].first[i].push_back(all[index].produce2[i][j]);break;}//�õ���i������ʽ�ķ��ս������
           else{
                getFirst(all[index].produce2[i][j]);
                int tmp=getIndex(all[index].produce2[i][j]);
                for(int k=0;k<all[tmp].produce2.size();k++)//�����÷��ս�������в���ʽ
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
       all[index].first[i].push_back('\0');//����\0���ڲ���
    }
}

void base::show()
{
    for(int i=0;i<non.size();i++)
        getFirst(non[i]);
    bool con=1;
    all[0].follow.push_back('$');//�Կ�ʼ���Ĵ���
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
            cout<<all[i].ch<<" ������first����Ϊ "<<endl;
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
            cout<<all[i].ch<<" ��follow����Ϊ "<<endl;
            for(int j=0;j<all[i].follow.size();j++)
                cout<<all[i].follow[j]<<' ';
            cout<<endl;
        }
}

int base::isDirect(int a)//�Ե�a������ʽ�ж��Ƿ�Ϊ��ݹ�
{
    for(int i=0;i<all[a].produce.size();i++)
         if(all[a].produce[i][0]==all[a].ch) return i;
    return (-1);
}
int base::getTer(int a)//�õ���������ķ��ս��
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
        }//û����ݹ��ʱ��ֱ�ӳ���ȥ
        while(isDirect(i)!=-1)//��������ݹ��ʱ��
        {
            int a=isDirect(i);//��������ʽ��������ݹ�
            char ch=getNewNon(asc);
            all[non.size()].ch=ch;
            non.push_back(ch);asc++;//�½�����ʽ
            string tmpString=all[i].produce[a].substr(1,all[i].produce[a].length()-1)+ch;
            all[non.size()-1].produce.push_back(tmpString);
            all[non.size()-1].produce.push_back("#");
            tmpString=all[i].produce[getTer(i)];//���ľɵĲ���ʽ
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
       for(int i=0;i<all[index].produce2.size();i++)//�������еĲ���ʽ
      {
        int last=1;//notice :length���������ַ���ռ�õĿռ�
        for(int j=0;j<all[index].produce2[i].length();j++)//ɨ�����ʽ
        {
            if(isNon(all[index].produce2[i][j])&&last==all[index].produce2[i].length())//������һ������ĩβ�ķ��ս��
            {cout<<all[index].produce2[i][j];
                int tmpIndex=getIndex(all[index].produce2[i][j]);
                if(all[index].produce2[i][j]==all[index].ch) continue;//������
                for(int k=0;k<all[index].follow.size();k++)//��C��follow���ϼ��뵽�÷��ս����follow��
                    {
                        if(contain(tmpIndex,all[index].follow[k])) continue;
                        change=1;
                        all[tmpIndex].follow.push_back(all[index].follow[k]);
                    }
            }
            else if(isNon(all[index].produce2[i][j])&&(isNon(all[index].produce2[i][j+1])||isTer(all[index].produce2[i][j+1])))//�÷��ս�����滹�з���
                    {
                        int tmpIndex=getIndex(all[index].produce2[i][j]);
                        if(all[index].produce2[i][j]==all[index].ch) continue;//������
                        char ch=all[index].produce2[i][j+1];//����ķ��ս��
                        if(isTer(ch)) {if(contain(tmpIndex,ch)) continue;change=1;all[tmpIndex].follow.push_back(ch);}//ֱ�Ӽ���
                        else{
                              int a=getIndex(ch);
                              for(int h=0;h<all[a].produce2.size();h++)
                                for(int g=0;g<all[a].first[i].size();g++)
                                    {
                                        if(all[a].first[h][g]=='#') {emp=1;continue;}
                                        if(contain(tmpIndex,all[a].first[h][g])) continue;
                                        change=1;
                                        all[tmpIndex].follow.push_back(all[a].first[h][g]);
                                    }//��first���ϼ���
                              if(emp){
                                        for(int k=0;k<all[index].follow.size();k++)//��C��follow���ϼ��뵽�÷��ս����follow��
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
   for(int i=0;i<all[index].produce1.size();i++)//ɨ�����ʽ
   {
       int j=i+1;
       int num=1;//�м����ǹ������Ӵ���
       int arr[10];//��¼�±�
       for(j;j<all[index].produce1.size();j++)
       {
          if((all[index].produce1[i][0]==all[index].produce1[j][0])&&left[i]==0&&left[j]==0)
               {num++;arr[0]=i;arr[num-1]=j;left[j]=1;}
       }
       if(num>1){
                    char newChar=getNewNon(asc);
                    all[non.size()].ch=newChar;
                    non.push_back(newChar);asc++;//�½�����ʽ
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
       char ch=ter[j];//���ڴ�����ս��
       int a;
       if(it[i][j].str.size()>1)
        it[i][j].str.push_back("Conflict");//��ͻ����
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
