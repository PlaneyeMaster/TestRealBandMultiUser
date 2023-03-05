
Any Project that needs to be used by the Multi User Editing Plugin needs to be in a source control repository . For our plugin we will use github 
as a repository 

Follow the steps to create a repository .

Tools Required:
==============
1) Make sure that git bash is installed
2) Install git lfs (https://git-lfs.github.com/)
3) Configure git lfs
   open git bash
   git lfs install
2) Makes sure Python is installed 
3) Install git module for python using the following command 
    3.1) Open git bash console
    3.2) Type the command 
          pip install GitPython
4) All the users should have access permission to the github repositories


Create Git Repository 
=====================

1) Open git bash
2) change directory to the project 
3) git init
4) create a file .gitigonore and add the folders which we want to ignore . Typically we ignore the temporary files/folder . so add the following in .gitignore

# Ignore binary files
Binaries/

# Ignore intermediate files
Intermediate/

# Ignore cached data
DerivedDataCache/

# Ignore temporary data
Saved/


5) Save the file 
6) Add the remaining files
   git add .gitignore
   git add Config
   git add Content
   git add <filename>.uproject 
5) Create a project repository  on GitHub
6) Open Browser
7) go to https://github.com/orgs/PlaneyeMaster/repositories
8) Click New repository
9) Add Repository Name as the Project Name
10) Select the "Public" option
11) Click "Create Repository" 
12) Enable git LFS 
   12.1) Go to your repo settings.
   12.2) Scroll down to the archives section.
   12.3) Check on the checkbox of Include Git LFS objects in archives.
13) Copy the repository URL for eg .
    https://github.com/PlaneyeMaster/nDisplayDemo.git
14) go to the git bash shell
15) git remote add origin https://github.com/PlaneyeMaster/nDisplayDemo.git
16) git push -u origin master
17) If you get any error while pushing large file in excess of 100 MB 
      git lfs track <filename>
      git commit


Cloning Git Repository
======================
Once the repository is created , any user who wants to work on that project needs to have that project on his machine .Follow the steps

1) Create a Folder
2) open git bash
3) change directory to that folder
4) give the command 
   git clone https://github.com/PlaneyeMaster/nDisplayDemo.git
    