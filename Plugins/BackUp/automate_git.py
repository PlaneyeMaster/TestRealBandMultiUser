import sys
import subprocess
import git

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
#repo_name = sys.argv[1]
#user = sys.argv[2]
#repo = git.Repo('.', search_parent_directories=True)
try:
    #repo = git.Repo("C:\\Projects\\nDisplayDemo", search_parent_directories=True)
    repo = git.Repo('.', search_parent_directories=True)
    if repo is None:
        print("Not a Git Repository")
        exit
    else:
        subprocess.call(["git", "pull"])
        subprocess.call(["git", "submodule", "update", "--init", "--recursive"])
        print("Project Updated")
except git.exc.InvalidGitRepositoryError:
    print("#########")


#is_repo_available=is_git_repo(sys.argv[1],sys.argv[2])
#print(is_repo_available)


print("=============")

exit
try:
    repo = git.Repo('.', search_parent_directories=True)
    print("Location "+repo.working_tree_dir)
    print("Remote: "+repo.remote("origin").url)
except git.exc.InvalidGitRepositoryError:
    print("Invalid Repository")



#is_repo_available = is_git_repo(sys.argv[1])

#if is_repo_available:
#    print("#################### REPO IS AVAILABLE ##################")
#    subprocess.call(["git", "clone", "https://github.com/PlaneyeMaster/%s/%s.git" % (user, repo_name)])
#    subprocess.call(["git", "pull"])
#    subprocess.call(["git", "submodule", "update", "--init", "--recursive"])
#else:
#    print("#################### REPO IS NOT AVAILABLE ##################")
#    repo_name=git.Repo.init(sys.argv[1])
#    subprocess.call(["git", "remote add origin", "https://github.com/PlaneyeMaster/%s/%s.git" % user % repo_name])
#    subprocess.call(["git", "push --tags"])

# subprocess.call(["git", "clone", "https://github.com/user/%s.git" % repo_name])
# subprocess.call(["git", "clone", "https://github.com/%s/%s.git" % user % repo_name])
# subprocess.call(["git", "pull"])
# subprocess.call(["git", "checkout", "origin/main"])
# subprocess.call(["git", "submodule", "update", "--init", "--recursive"])
