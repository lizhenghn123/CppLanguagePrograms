CppLanguagePrograms
===================
包含：

* 一些C++实现的工具库（虽然是在Windows下测试，但其实都是也可以在Linux下使用的，参见：[https://github.com/lizhenghn123/zl_reactor](https://github.com/lizhenghn123/zl_reactor "zl_reactor")）
	* **基本工具库**：
	    * 日志工具（日志优先级、同步日志、异步日志、带色彩的屏幕输出）
	    * 模板单例类
	    * 时间戳类
	    * 计时器类
	    * 日期类
	    * 智能断言（静态断言、动态断言）
	    * ScopeGuard RAII类
	    * STL辅助工具
	    * 字符串辅助工具
	    * File类（本地文件读取、内存数据映像文件操作）
	    * 文件及目录操作工具
	    * 异常类及命名重整还原实现
	    * 进程属性工具（Linux特有）
	    * 多进程master-worker实现（Linux特有）
	* **线程库**：
	    * 信号量 
	    * 锁(互斥、读写、独占、旋转)
	    * 原子计数器
	    * 同步waiter
	    * (固定大小、不限大小）同步队列（先进先出、先进后出、按优先级出入）
	    * TLS
	    * 线程池（组）
	* **网络库**：
	    * 网络相关工具 
	    * socket api的跨平台包装
	    * socket RAII类
	    * socket address类
	    * EventLoop、EventLoopThreadPool
	    * IO MultiPlexing（select\poll\epoll\iocp）
	    * TimerQueue
	    * TcpAcceptor、TcpConnector、TcpConnection、TcpServer
	    * eventfd、signalfd、timerfd（Linux特有）
	    * simple httpserver
	* **通用工具**
	    * base64 编解码
	    * md5
	    * UUID
	    * 字符串多种hash实现
* 用C++编程过程中实现的一些测试代码
	1.  2014-04-21 增加静态绑定、动态绑定测试代码； 
	2.  2014-04-21 增加静态多态、动态多态测试代码； 
	3.  2014-04-21 增加指针常量、常量指针测试代码； 
	4.  2014-04-21 增加名字重整（Name Mangling）测试代码； 
	5.  2014-04-21 增加strlen、sizeof测试代码；  
	6.  2014-04-23 增加GBK、UTF-8转换测试代码；
	7.  2014-04-24 增加Windows及Linux下计时器；
	8.  2014-04-25 增加do-while(0)的使用场景；
	9.  2014-04-28 增加Windows/Linux下的几个计时函数；
	10. 2014-04-28 增加vector、list的插入性能比较；
	11. 2014-04-30 增加以前学习时写的STL容器的使用代码；
	12. 2014-04-30 增加求两个文本文件的差异行；
	13. 2014-04-30 增加C++实现的跨平台字符串模糊匹配；
	14. 2014-07-18 增加google flat buffers(序列化工具)的测试使用；
	15. 2014-09-16 增加Base64编解码功能；
	16. 2014-09-19 增加ByteArray；
	17. 2014-09-23 增加字符串的几种hash函数计算；
	18. 2014-09-24 增加与进程相关的性能统计工具ProcessTimeCounter;
	19. 2014-09-25 增加google gtest库；
	20. 2014-09-30 增加一个侵入式list实现；
	21. 2014-10-01 增加一个RAII类自动释放资源；

Created by lizhenghn@gmail.com 

二〇一四年四月二十一日
