import sys
import subprocess
import git
import os

def is_git_repo(path,user):
    try:
        _ = git.Repo(path).git_dir
        subprocess.call(["git", "clone", "https://github.com/PlaneyeMaster/%s/%s.git" % (user, path)])
        subprocess.call(["git", "push"])
    except git.exc.InvalidGitRepositoryError:
        print("#################### REPO IS NOT AVAILABLE ##################")
        repo_name=git.Repo.init(path)
        subprocess.call(["git", "remote add origin", "https://github.com/PlaneyeMaster/%s/%s.git" % user % path])
        subprocess.call(["git", "push"])
        return False
    return True

# `git init new_repo`
repo_name = sys.argv[1]
#user = sys.argv[2]
#repo = git.Repo('.', search_parent_directories=True)
try:
    repo = git.Repo(repo_name, search_parent_directories=True)
    #repo = git.Repo('.', search_parent_directories=True)
    if repo is None:
        print("Not a Git Repository")
        exit
    else:
        grepo = git.Repo(repo_name)
        remotes = grepo.remotes
        print(remotes[0].url)
        #remoteurl =  git config --get remote.origin.url
        subprocess.call(["git", "branch", "--set-upstream-to=origin/master", "master"])
        print("Start Sync ....")
        os.chdir(repo_name)
        subprocess.call(["git", "config","--global","-add","safe.directory",repo_name])
        print("##Add Safe directory##")
        print(os.getcwd())
        subprocess.call(["git", "pull","--all"])
        #subprocess.call(["git", "pull","%s main" remotes[0].url)
        subprocess.call(["git", "submodule", "update", "--init", "--recursive"])
        print("Project Updated")        
except git.exc.InvalidGitRepositoryError:
    print("#########")

print("=============")


