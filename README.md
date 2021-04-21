# Database_Project

# 数据库系统的应用
任务就是学怎么用数据库。我选的题目是二手房交易系统。

## 注意事项

1. go mod 需要设置 proxy，我一开始不懂得如何使用，导致出现问题。
可以参考[goproxy中国官网](https://www.goproxy.cn/)的方法设置一下，否则会下不相应的包。


比较通用的方法就是使用下面的命令
```
go env -w GO111MODULE=on 
go env -w GOPROXY=https://www.goproxy.cn/,direct
```



## 参考地址

- [go官方api定义](https://go-zh.org/pkg/database/sql/#Open)
- [官方sql驱动](https://github.com/go-sql-driver/mysql/)

- [go教程一，用go写web应用](https://github.com/astaxie/build-web-application-with-golang/blob/master/zh/05.2.md)
- [go教程二，临时找的](http://www.topgoer.com/%E6%95%B0%E6%8D%AE%E5%BA%93%E6%93%8D%E4%BD%9C/go%E6%93%8D%E4%BD%9Cmysql/mysql%E4%BD%BF%E7%94%A8.html)

为了节约时间，那就找个框架来实现

beego 是一个快速开发 Go 应用的 HTTP 框架，他可以用来快速开发 API、Web 及后端服务等各种应用，是一个 RESTful 的框架，主要设计灵感来源于 tornado、sinatra 和 flask 这三个框架，但是结合了 Go 本身的一些特性（interface、struct 嵌入等）而设计的一个框架。

- [beego官网](https://beego.me/)（但是看起来好像盗版的
- [beego的一个教程](http://www.topgoer.com/beego%E6%A1%86%E6%9E%B6/)