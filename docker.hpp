//dockr.hpp
//用到的头文件
#include <sys/wait.h>   // waitpid
#include <sys/mount.h>  // mount
#include <fcntl.h>      // open
#include <unistd.h>     // execv, sethostname, chroot, fchdir
#include <sched.h>      // clone
#include <iostream> 
#include <string>
// C 标准库
#include <cstring>
#define STACK_SIZE (1024 * 1024) // 定义子进程空间大小 1M
namespace bluecat
{
    typedef struct docker_info//一些容器信息
    {
        std::string hostname;//主机名字
        std::string root_dir;//根目录名字
    }docker_info;  
    typedef int process_pid;//增强
    class bluecat_docker
    {
    private:
        docker_info info;//容器信息
        char child_stack[STACK_SIZE];//子进程栈空间大小
        //设置容器主机名字
        void set_hostname()
        {
            sethostname(this->info.hostname.c_str(), this->info.hostname.size());
            //std::cout<<"设置容器主机名字完毕"<<std::endl;
        }
        //设置容器根目录
        void set_rootdir()
        {
            // chdir 系统调用, 切换到某个目录下
            chdir(this->info.root_dir.c_str());
            // chroot 系统调用, 设置根目录, 因为刚才已经切换过当前目录
            //            // 直接使用当前目录作为根目录就可以了
            chroot(".");
        }
        // 挂载 proc 文件系统
        void set_procsys() 
        {
             mount("none", "/proc", "proc", 0, nullptr);
             mount("none", "/sys", "sysfs", 0, nullptr);
 
        }
        void start_bash()
        {
            // 将 C++ std::string 安全的转换为 C 风格的字符串 char *
            std::string bash="/bin/bash";
            char *c_bash=new char[bash.size()+1];//多一个字节存储\0
 
            strcpy(c_bash, bash.c_str());
            char* const child_args[] = { c_bash, NULL   };
            execv(child_args[0], child_args);           // 在子进程中执行 /bin/bash
            delete []c_bash;
            c_bash=nullptr;
        }
    public:
        bluecat_docker(docker_info &d_info):info(d_info)//构造函数
        {
 
        }
        static int set_info(void *args)
        {
            auto bc_docker=static_cast<bluecat_docker*>(args);
            bc_docker->set_hostname();
            bc_docker->set_rootdir();
            bc_docker->set_procsys();
            bc_docker->start_bash();
 
            return 1;
        }
        void start()
        {
            std::cout<<"welcome to bluecat's docker"<<std::endl;
            process_pid child_process=clone(set_info,child_stack+STACK_SIZE,//每个容器是一个进程
                                                               CLONE_NEWUTS| //uts 域名隔离
                                                               CLONE_NEWNS| //Mount namespace
                                                               CLONE_NEWIPC|//进程通信
                                                               CLONE_NEWPID|
                                                               SIGCHLD, // 子进程退出时会发出信号给父进程 避免僵尸进程
                                                               this);//把this传给静态函数
            waitpid(child_process, nullptr, 0); // 等待子进程的退出 避免僵尸进程
            printf("已经退出\n"); 
        }
    };
};

