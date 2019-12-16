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

int main()
{
    ifstream in("/home/bluecat/hash_db/filename");//改成模块1生成的文件地址
    ofstream out("/home/bluecat/hash_db/sha256");
    string filename;
    string line;
    if (in) // 有该文件
    {
        while (getline(in, line)) // line中不包括每行的换行符
        {
            cout << line << endl;
            
            char result[1024] = { 0 };
            string cmd="sha256sum ";
	        cmd=cmd+line;
            executeCMD(cmd.data(), result);//      这行是将每一行的输出拼接之后获取到了result字符串中了
            //printf("%s", result);
            //blog.csdn.net/YoungStar70/article/details/70305687
            string res=result;
            cout << res << endl;
            //out << res << endl; // 输入到2.txt中
        }
    }
    else // 没有该文件
    {
        cout << "no such file" << endl;
    }
    
    return 0;
}




