//
//  main.cpp
//  sha256
//
//  Created by 蓝猫 on 2019/4/29.
//  Copyright © 2019年 蓝猫. All rights reserved.
//

#include <iostream>

// 文件操作.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
using namespace std;
void executeCMD(const char *cmd, char *result)
{
    char buf_ps[1024];
    char ps[1024] = { 0 };
    FILE *ptr;
    strcpy(ps, cmd);
    if ((ptr = popen(ps, "r")) != NULL)//通过创建一个管道，调用 fork 产生一个子进程，执行一个 shell 以运行命令来开启一个进程。
    {
        while (fgets(buf_ps, 1024, ptr) != NULL)
        {
            //	       可以通过这行来获取shell命令行中的每一行的输出
            //	   	   printf("%s", buf_ps);
            strcat(result, buf_ps);
            if (strlen(result) > 1024)
                break;
        }
        pclose(ptr);
        ptr = NULL;
    }
    else
    {
        printf("popen %s error\n", ps);
    }
}

bool equal(string s1,string s2)
{
    for(int i=0;i<s1.size();i++)
    {
	if(s1[i]==' ')break;
	if(s1[i]!=s2[i])return false;
    }
    return true;
}

int main()
{
    vector<string> v1,v2;
    ifstream in("/home/bluecat/hash_db/filename");//改成模块1生成的文件地址
    ifstream in2("/home/bluecat/hash_db/sha256");//改成模块1生成的文件地址
    string filename;
    string line;
    if (in) // 有该文件
    {
        while (getline(in, line)) // line中不包括每行的换行符
        {
            //cout << line << endl;
            
            char result[1024] = { 0 };
            string cmd="sha256sum ";
	    cmd=cmd+line;
            executeCMD(cmd.data(), result);//      这行是将每一行的输出拼接之后获取到了result字符串中了
     
            string res=result;
            v1.push_back(res);
        }
    }
    else // 没有该文件
    {
        cout << "no such file" << endl;
    }
    if (in2) // 有该文件
    {
        while (getline(in2, line)) // line中不包括每行的换行符
        {
            //cout << line << endl;
	    if(!line.empty())v2.push_back(line);
            
       
        }
    }
    else // 没有该文件
    {
        cout << "no such file" << endl;
    }
    if(v1.size()!=v2.size())
    {
	cout<<"镜像不可信"<<endl;
	return 0;
    }
    for(int i=0;i<v1.size();i++)
    {
	cout<<"old sha256:"<<v2[i]<<endl;
	cout<<"new sha256:"<<v1[i]<<endl;
	//if(v1[i]!=v2[i])
	if(!equal(v1[i],v2[i]))
	{
	    cout<<"镜像不可信"<<endl;
	    return 0;
	    break;
	}
    }
    cout<<"镜像可信"<<endl;
    return 0;
}




