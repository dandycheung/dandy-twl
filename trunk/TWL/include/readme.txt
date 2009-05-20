========================================================================
       TINY WINDOW LIBRARY : TWL
========================================================================

TWL 类库，现在只有 wnd.h 和 userWnd.h 两个文件
23:28 2003-11-4

去掉了 wnd.h 中用到 BSTR 的代码
全部使用 ASSERT 宏
启用了 CompactPath 函数
15:45 2003-11-5

添加了 gdi.h
22:19 2003-11-5

gdi.h 中的对象均支持析构时不自动删除 GDI 对象
添加了 mpx.h
16:21 2003-11-6

添加了 thrdmgr.h、msgloop.h、frmbase.h 和 onlyinst.h
16:16 2005-2-3

添加了 str.h、reg.h、trcdbg.h、axhost.h 和 wbhost.h
18:45 2005-3-5

添加了 dlg.h
13:49 2005-4-5

修改了 userwnd.h（this 指针的存储位置）以及其他若干头文件（强制用于 C++ 源程序中）
19:08 2005-4-5

添加了 strary.h 和 tknzr.h
22:09 2005-6-9

修改了 userwnd.h 以支持非自注册窗口类，添加了用于消息转发以及反射的代码
添加了 spltwnd.h、tdc.h、mcr.h、msgmap.h、resutil.h、stdctl.h 和 cmnctl.h
20:35 2005-8-18

增加了 mdc.h、ary.h、lyt.h
修正了一大批文件
原来已经添加入了 thrd.h 和 osinfo.h
21:46 2005-8-25

对 userwnd.h 进行了修改，原来对 WM_NCCREATE 的处理是要接受 this 指针，但现在工作已经
放到了钩子里，所以将其相关代码删除，这样还允许了用户在创建窗口时使用 lParam 参数
13:41 2005-9-11

修改了 spltwnd.h 中如果创建的分隔条窗口初始位置不在父窗口的左上角时导致的定位问题
19:14 2005-9-16

userwnd.h 中去掉了 CUserWindow 类的消息循环方法
msgloop.h 中默认放开了带有消息过滤和空闲处理功能的 CMessageLoop 类
dlg.h 针对非模态对话框作了改进
mpx.h 中修正了 MPX_WM_COMMAND 的注释
21:58 2005-9-19

* msgloop.h 中将需要包含的 ary.h 放到了 namespace 之外
+ scrl.h，对滚动的支持，还有小问题
* userwnd.h 中将挂钩窗口创建的代码独立成了函数，以使其他地方可以方便调用
* mpx.h，修正了若干错误，增加了 TMPX_ 开头的宏，此类宏可以省略消息参数名称
* dlg.h，统一了消息处理函数的原形，将 CDialog 从 CUserWindow 派生
20:35 2005-9-24

+ 添加了 dldll.h，可以像调用常规函数那样调用延迟加载动态库中的函数
23:29 2005-9-26

* 修改了若干头文件，主要统一了原来 ASSERT 宏
* 修改了若干头文件，主要统一了原来窗口类中对模块句柄的使用
* 修改了若干头文件，主要统一了原来资源相关类中对资源模块句柄的使用
+ sysfnt.h 工具类，实现了对系统字体的封装
+ ctrlfnt.h 工具类，用于辅助控件对 WM_GETFONT/WM_SETFONT 消息的处理
+ ccutil.h 工具类，简化对工具栏、复用栏等窗口的创建
+ modutil.h 现为基础类之一，用以获取当前模块的句柄
+ gdiext.h 为若干 GDI API 提供了便利的重载版本
19:41 2005-10-27

* userwnd.h，添加了子类化与去子类化的函数
8:41 2005-11-1

* gdi.h，去除了对 windowsx.h 的依赖
* stdctl.h，去除了对 windowsx.h 的依赖
* cmnctl.h，去除了对 windowsx.h 的依赖
19:44 2005-11-1

* gdiext.h，对原来 API 的调用都用了全局作用域解析符
* str.h，为了兼容 VC 8，将 for 语句中的变量声明提前
17:56 2005-11-4

* dlg.h，使用 HookWindow() 机制会使得 ProcessMessage() 被调用两次，故进行了改动，但是
  出现了显示对话框很慢的情况（仅一次），此现象最早的时候好象出现过
* ary.h，修正了原来删除最后一个元素，其构造函数不能被调用到的问题
17:57 2005-11-17

+ strconv.h，用于在 Ansi 和 Unicode 之间转换的头文件
+ map.h，实现了 TMap<> 模板类
+ lock.h，其中包括了一个临界区的封装以及一个自动进入/离开临界区的辅助类
* trcdbg.h，在发现没有定义 TRACE 宏的情况下将之定义为了 TWLTRACE
* str.h，增加了以宽字节字符串为参数的构造函数（在编译环境定义为窄字节字符集的情况下）
  /以窄字节字符串为参数的构造函数（在编译环境定义为宽字节字符集的情况下）
* scrl.h，修改了其中一个函数的返回值问题
* stdctl.h 和 cmnctl.h，由于 strconv.h 的引入，将原来 _TWL_NO_COM 的强制定义去除了
22:23 2006-12-4

* userwnd.h 和 dlg.h，全面采用 hook 的方式对应对象与句柄
+ dlgtempl.h 和 prop.h，新增，用于支持属性表和属性页
* cmnctl.h，将一个方法的参数增加了缺省值
* tdc.h，改正了 ExtractThreadData 的一个重大问题，并新增了 PeekThreadData 方法
* ext\map.h，增加了一个模板参数，用以提供不同的计算 Hash 的方法
21:29 2006-12-30

* dlg.h，增加了和 MFC 一样的对对话框字体的美化工作
18:38 2006-12-31

* prop.h，对系统进行了动态检测，以使属性表可以在 Windows 95 上显示
12:00 2007-1-1

* userwnd.h，增加了 UnhookWindowCreate() 方法
* dlg.h 和 prop.h，增加了对 UnhookWindowCreate() 方法的调用
19:25 2007-1-5

* strconv.h，修改了一个参数错误
* dldll.h，修改了静态函数指针的声明问题，增强了 Unicode 兼容，去掉了对 4229 号警告的屏蔽
+ iso_for_scope_for_vc6.h，新增；包含此文件可使符合 C++ 标准的 for 语句在 VC6 中正确编译
23:02 2007-1-7

* scrl.h、lock.h、resutil.h、ctrlfnt.h、thrd.h 和 tknzr.h，将原先以 _ 开头的成员变量均
  改成了以 m_ 开头，以保持整个库的一支风格
12:21 2007-1-21

* strconv.h，对转换宏进行了增强，增加了允许传递转换代码页的参数；增强了参数的合法性检查
* lyt.h，一旦 AddControl 的定位方法有某个为相对定位的控件 ID，则不能再使用一次性定位多个
  窗口的机制，对此作了相应更改
22:04 2007-1-29

* str.h，增加了指定起始位置的两个 Find 方法
* ary.h，增加了 InsertAt 方法
* gdiext.h，增加了一个用于公用控件的方法
21:21 2007-11-22

* reg.h、strconv.h、prop.h，将 TWLASSERT 换成了 ASSERT
+ comptr.h、comvar.h，新增，但没有应用测试
00:30 2008-01-28

TODO:
1、目前有了 resutil.h，可以考虑将 dlg.h、gdi.h、str.h 中的资源模块句柄相关的代码整合 -- 完成
2、lyt.h 中，LAYOUT_BORDER 和 LAYOUT_KEEPSIZE 要是真的是一个 ID 怎么办？考虑加入类似
   LAYOUT_LFORCEID、LAYOUT_TFORCEID 这样的参数 -- 完成
3、lyt.h 中添加 RemoveControl()、ShowControls(BOOL bShow) -- 完成

/////////////////////////////////////////////////////////////////////////////
