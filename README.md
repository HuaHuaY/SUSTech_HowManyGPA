# SUSTech_HowManyGPA(Outdated)
**This repo is outdated. It's designed for old education system jwxt.sustech.edu.cn, not the new tis.sustech.edu.cn.**

SUSTech GPA Viewer

## 运行环境

- 64位Windows操作系统

## 运行

项目无需安装，可直接运行。[下载URL](https://github.com/HuaHuaY/SUSTech_HowManyGPA/releases)

### 介绍

项目是MFC应用程序，使用Visual Studio 2019编译，项目编码为多字节。

程序需要访问网络。

程序不会保存，或向除CAS系统以外的地方发送学生个人CAS账号密码。

程序先向CAS系统post账号密码登录，然后前往教务系统读取成绩信息。

### GPA计算

暂未找到同一节课多次F的案例，未知多次F时，GPA计入多次F，还是只计入一个F，程序代码暂时按仅计入1个F计算。

### 挖坑

- 加上根据课程编号前缀筛选功能
- 加上根据课程编号排序功能
- 减少画面卡顿和闪烁
- 迁移到Qt
- 开发小程序

## 注

本程序内图片来源网络。
