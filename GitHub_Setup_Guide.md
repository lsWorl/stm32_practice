# GitHub项目上传详细指南

## 1. 初始化本地Git仓库
```bash
git init
```
- 功能：在当前目录创建新的Git仓库
- 说明：会在当前目录创建.git隐藏文件夹，用于存储版本控制信息

## 2. 添加文件到暂存区
```bash
git add .
```
- 功能：将当前目录所有文件添加到暂存区
- 参数说明：
  - `.` 表示当前目录所有文件
  - 也可以指定具体文件路径
- 注意事项：
  - 建议先创建.gitignore文件
  - 排除不需要版本控制的文件

## 3. 提交更改
```bash
git commit -m "Initial commit"
```
- 功能：将暂存区内容提交到本地仓库
- 参数说明：
  - `-m` 后面跟提交信息
  - 提交信息应简洁明确
- 最佳实践：
  - 每次提交只包含一个完整的功能修改
  - 提交信息采用"动词+对象"格式

## 4. 在GitHub创建新仓库
- 登录GitHub网站
- 点击右上角"+"号，选择"New repository"
- 填写仓库信息：
  - Repository name: 仓库名称
  - Description: 项目描述（可选）
  - Public/Private: 选择仓库可见性
  - Initialize this repository with: 不要勾选任何选项
- 点击"Create repository"

## 5. 添加远程仓库地址
```bash
git remote add origin https://github.com/username/repository-name.git
```
- 功能：将本地仓库与远程仓库关联
- 参数说明：
  - `origin` 是远程仓库的默认别名
  - URL可以从GitHub仓库页面复制
- 验证远程仓库：
  ```bash
  git remote -v
  ```

## 6. 推送代码到GitHub
```bash
git push -u origin master
```
- 功能：将本地代码推送到远程仓库
- 参数说明：
  - `-u` 设置上游分支，后续可以直接使用git push
  - `origin` 远程仓库别名
  - `master` 本地分支名称
- 常见问题：
  - 如果远程分支是main而不是master：
    ```bash
    git push -u origin master:main
    ```

## 7. 验证推送结果
```bash
gh repo view
```
- 功能：查看GitHub仓库信息
- 前提条件：
  - 已安装GitHub CLI
  - 已登录GitHub账号
- 替代方案：
  - 直接访问GitHub网站查看

## 8. 后续更新流程
```bash
# 查看当前状态
git status

# 添加修改文件
git add .

# 提交更改
git commit -m "Update message"

# 推送到远程仓库
git push
```

## 9. 常见问题解决

### 9.1 推送时提示权限不足
- 解决方案：
  1. 检查SSH密钥配置
  2. 使用HTTPS方式需要配置个人访问令牌
  3. 确保GitHub账号有仓库的写权限

### 9.2 分支名称冲突
- 解决方案：
  1. 查看本地分支：
     ```bash
     git branch
     ```
  2. 查看远程分支：
     ```bash
     git branch -r
     ```
  3. 重命名本地分支：
     ```bash
     git branch -m old-branch new-branch
     ```

### 9.3 提交历史冲突
- 解决方案：
  1. 拉取最新代码：
     ```bash
     git pull origin main
     ```
  2. 解决冲突后重新提交
  3. 强制推送（慎用）：
     ```bash
     git push -f
     ```

## 10. 最佳实践建议
1. 保持提交历史整洁
2. 使用有意义的提交信息
3. 定期同步远程仓库
4. 使用分支进行功能开发
5. 配置.gitignore文件
6. 使用Git GUI工具辅助操作
