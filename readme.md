
## status
| [Windows][win-link] | [Release][release-link]|
| :-----------------: |:-----------------: |
| ![win-badge]        |![release-badge] |


[win-badge]: https://ci.appveyor.com/api/projects/status/mwr2xc7rhgfrxlj3?svg=true "AppVeyor build status"
[win-link]: https://ci.appveyor.com/project/kensou24/akhelper "AppVeyor build status"
[release-link]: https://github.com/kensou24/akhelper/releases "Release status"
[release-badge]: https://img.shields.io/github/v/release/kensou24/akhelper.svg?style=flat-square" "Release status"

## support
[![GitHub Issues](https://img.shields.io/badge/github-issues-red.svg?maxAge=60)](https://github.com/kensou24/akhelper/issues)
[![GitHub Wiki](https://img.shields.io/badge/github-wiki-181717.svg?maxAge=60)](https://github.com/kensou24/akhelper/wiki)




[![Build status](https://ci.appveyor.com/api/projects/status/mwr2xc7rhgfrxlj3?svg=true)](https://ci.appveyor.com/project/kensou24/akhelper)


### what

![演示](https://github.com/kensou24/akhelper/blob/master/images/ak-help.gif)

基于qt5、opencv开发的明日方舟辅助程序.

opencv库进行图像识别，qt5提供界面，识别之后进行按钮点击，逻辑较简单。

从**0.0.4版本**开始，抓图通过adb直接抓取运行程序图片，不再是屏幕抓图，所以可以后台挂机了，上班好助手，**哈哈哈**

### how

#### 0.0.1-0.0.3 版本使用说明

- step1 启动雷电模拟器，分辨率调整为960*540，登录账号，点击到“”开始行动“界面
- step2 将雷电模拟器移动到桌面左上角，【可以调整模拟器位置，在ak界面设置x、y是雷电模拟器的最左上角位置，默认是0，0也就是左上角】
- step3 点击ak.exe启动程序
- step4 将文件夹中图片001.png【开始行动】\002.png【行动结束】\003.png【确认开始行动】导入即可
- step5 点击启动

#### 0.0.4后版本使用说明

- step1 启动雷电模拟器，分辨率调整为960*540，登录账号，点击到“”开始行动“界面
- step2 点击ak.exe启动程序
- step3 将文件夹中图片001.png【开始行动】\002.png【行动结束】\003.png【确认开始行动】导入即可
- step4 点击选择adb.exe所在的位置
- step5 点击查询设备
- step6 点击启动
- 支持最小化，因为已经使用adb直接与Android交互了

### tips

- 使用了开源的appveyor自动构建程序了，可以直接在[下载](https://github.com/kensou24/akhelper/releases) 下载最新版本的执行文件了

- 【推荐使用上面的直接下载】从release目录下面,下载-0.0.1版本，使用最新版本的替换里面的程序即可。比如现在最新的是0.0.3，你需要下载0.0.1版本和0.0.3版本，使用0.0.3版本替换
- 需要先点击到需要挂机的挂卡**，吃好体力,** **点到“开始行动”按钮**，勾选**代理指挥**
- 应该不会存在被封风险，只是识别图像，模拟点击鼠标
- 0.0.1-0.0.3版本使用mumu模拟器测试的时候，应为是模拟鼠标点击，无法正常工作，似乎是模拟器拦截了模拟鼠标点击行为，建议使用雷电模拟器，分辨率设置为 **<u> 960*540 </u>**
- 从0.0.4版本开始支持adb后台挂机了，需要用户手动设置adb.exe的路径，通过adb直接与android系统交互,所以可以后台挂机了,**adb.exe的路径一般在你安装的模拟器目录下面可以找到**
- 如果存在其他需要识别的图片，截图之后，<u>***将需要点击的按钮的位置设置到图片中间点***</u>，很重要，否则即使识别出来也会点击到其他地方，导入到ak即可
- 欢迎加群q 57830127，讨论交流
### 后续

  - 实现挂机次数
  - 实现任务列表
  - 代码开源 [代码已经开源]

  

