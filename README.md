# Bomb_your_Screen
一个轰炸你的屏幕的小软件

    这算是一个恶搞小程序，如果你的windows用户有管理员权限，那么当你打开软件后，你将发现你鼠标点击的区域就会出现一个桃心，并且不会像正常windows下一样响应
点击事件然后你会发现你无法切换屏幕，当你想用任务管理器来结束掉这个程序时，你会发现你鼠标刚移到任务管理器上，任务管理器就会关闭。

    实现方法其实很简单，这个程序首先对当前屏幕截屏，然后绘制到一个全屏窗口的DC中去，这样会看起来就像什么都没有打开一样，然后用键盘HOOK禁用掉键盘的各种响
应，但是在win7和win10下，键盘HOOK已经无法禁用掉ctrl+alt+del的组合了，要取消掉任务管理器的响应，我想到了两种方法，第一种稍微简单一些，也是本程序使用的方
法，开启一个新的线程，在一个死循环中监视是否出现任务管理器的窗口类(先用spy++检测任务管理器类的基本属性)，如果发现鼠标位置在任务管理器窗口中，那么就关闭掉
任务管理器。第二种方法相对复杂一些（后面做一个版本2），就是直接将Winlogon.exe这个进程挂起（感觉这种方法也相对危险一些，这种方法有很多衍生方法，比如修改
修改Winlogon.exe代码等，担心这样会让系统无法启动，未测试过，毕竟这只是一个恶搞，还是以安全为主），后续如果有什么更好的方法我会提交上来。