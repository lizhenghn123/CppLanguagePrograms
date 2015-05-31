#ifndef ZL_SHAREMEMORY_H
#define ZL_SHAREMEMORY_H

/// System V 共享内存C++包装类
class SysShareMemory
{
public:
    SysShareMemory();
    ~SysShareMemory();

public:
    /***
    * 打开一个已经存在的共享内存
    * @path: 用来创建共享内存的路径(包含文件名)，不能为NULL
    */
    bool open(const char* path, int size = 1, int mode = 0604);

    /***
    * 创建一个共享内存
    * @path: 用来创建共享内存的路径(包含文件名)，如果为NULL则由系统选择
    * @mode: 权限模式，其值为S_IRWXU和S_IXUSR等
    * @return: 如果已经存在则返回false，否则返回true
    */
    bool create(const char* path, int size = 1, int mode = 0604);

    /***
    * 关闭已经创建或打开的共享内存，
    * 如果已经没有进程关联到此共享内存，则删除它
    */
    void close();

    /***
    * 关联共享内存，将共享内存映射到进程空间
    * @return: 返回共享内存的地址
    */
    void* attach(int shmflag = 0);

    /***
    * 解除和共享内存的关联，将共享内存从进程空间中移除
    */
    void detach();

    /***
    * 返回共享内存的地址
    */
    void* address()
    {
        return shmAddr_;
    }

    void* address() const
    {
        return shmAddr_;
    }

private:
    int    sgmId_;
    void   *shmAddr_; /** attach的共享内存地址 */
};

#endif /* ZL_SHAREMEMORY_H */