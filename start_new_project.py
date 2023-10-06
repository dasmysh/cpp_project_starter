import argparse
import os
import platform
import shutil
from git import Repo, Submodule
import datetime
import re

parser = argparse.ArgumentParser(prog="C++ProjectStarter",
    description="Creates a new C++ project with some default configuration.")
parser.add_argument("project_name")
parser.add_argument("-p", "--path", default="../")
parser.add_argument("-n", "--app_name", default="")
parser.add_argument("--namespace", default="")
parser.add_argument("-r", "--readable_name", default="")
parser.add_argument("-d", "--description", default="")
parser.add_argument("-a", "--author", default="Sebastian Maisch")
parser.add_argument("-e", "--email", default="sebastian.maisch@googlemail.com")
parser.add_argument("-t", "--test_run", action="store_true")
args = parser.parse_args()

test_run = args.test_run
now = datetime.datetime.now()
absolute_path = os.path.abspath(args.path)
project_name = args.project_name
app_name = args.app_name if args.app_name != "" else project_name
namespace = args.namespace if args.namespace != "" else app_name
vcpkg_name = project_name.replace("_", "")
date = now.strftime("%Y.%m.%d")
year = now.strftime("%Y")
readable_name = args.readable_name if args.readable_name != "" else args.project_name
description = args.description if args.description != "" else f"Basic application for {readable_name}."
author = args.author
email = args.email
replace_pattern = "@PROJECT_NAME|@DATE|@YEAR|@APP_NAME|@READABLE_NAME|@DESCRIPTION|@AUTHOR|@AUTHOR_EMAIL|@NAMESPACE|@VCPKG_NAME"
replace_dict = { "@PROJECT_NAME": project_name, "@NAMESPACE": namespace, "@DATE": date, "@YEAR": year, "@APP_NAME": app_name, "@VCPKG_NAME": vcpkg_name,
                "@READABLE_NAME": readable_name, "@DESCRIPTION": description, "@AUTHOR": author, "@AUTHOR_EMAIL": email }


def configure_file(src, dest):
    with open(src) as src_file:
        file_content = src_file.read()
    file_content = re.sub(replace_pattern, lambda m: replace_dict[m.group(0)], file_content)
    with open(dest, "x") as dest_file:
        dest_file.write(file_content)


print(f"Creating new C++ project '{project_name}' in '{absolute_path}'.")

project_path = os.path.join(absolute_path, project_name)
if os.path.isdir(project_path):
    print(f"A directory named {project_path} already exists.")
    exit(1)

print(f"Create directory {project_path}.")
if not test_run: os.mkdir(project_path)

print(f"Initialize git repository {project_path}.")
if not test_run: git_repo = Repo.init(project_path)
print(f"Add submodule vcpkg.")
if not test_run: Submodule.add(git_repo, "vcpkg", "vcpkg", "https://github.com/microsoft/vcpkg.git")

for root, dirnames, filenames in os.walk("./template"):
    if ".git" in root:
        continue

    print(f"Copying contents of {root}.")
    root_abs = os.path.abspath(root)
    root_dest = root

    if "app_name" in root:
        root_dest = root.replace("app_name", app_name)
    root_dest_components = os.path.normpath(root_dest).split(os.sep)
    root_dest_components.remove("template")
    root_dest = os.sep.join(root_dest_components)
    dest_abs = os.path.join(project_path, root_dest)

    if "cmake" in root:
        print(f"Copy {root} to {dest_abs}")
        if not test_run: shutil.copytree(root, dest_abs)
        continue

    if not os.path.isdir(dest_abs):
        if not test_run: os.mkdir(dest_abs)
    for filename in filenames:
        src = os.path.normpath(os.path.join(root_abs, filename))
        if os.path.isfile(src) and not filename == 'start_new_project.py':
            dest = os.path.normpath(os.path.join(dest_abs, filename))
            print(f"Copy {src} to {dest}")
            if not test_run: configure_file(src, dest)

os.system(f"cmake -S {project_path}  --preset=default")
