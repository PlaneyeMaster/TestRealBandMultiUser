
Any Project that needs to be used by the Multi User Editing Plugin needs to be in a source control repository . For our plugin we will use github 
as a repository 

Follow the steps to create a repository .

Tools Required:
==============
1) Make sure that git bash is installed 
2) Makes sure Python is installed 
3) Install git module for python using the following command 
    3.1) Open git bash console
    3.2) Type the command 
          pip install GitPython

Create Git Repository 
=====================

1) Open git bash
2) change directory to the project 
3) git init
4) git add .
5) Create a project repository  on GitHub
6) Open Browser
7) go to https://github.com/orgs/PlaneyeMaster/repositories
8) Click New repository
9) Add Repository Name as the Project Name
10) Select the "Public" option
11) Click "Create Repository" 
12) Copy the repository URL for eg .
    https://github.com/PlaneyeMaster/nDisplayDemo.git
13) go to the git bash shell
14) git remote add origin https://github.com/PlaneyeMaster/nDisplayDemo.git
15) git push -u origin master
    