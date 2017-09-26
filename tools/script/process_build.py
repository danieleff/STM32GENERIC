import os
import json
import subprocess

git_hash = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD']).decode("ascii").strip()

def find_finished_compilations(path):
    ret = []
    for root, dirnames, filenames in os.walk(path):
        for filename in filenames:
            if filename == "info.json":
                ret.append(os.path.join(root, filename))
    return sorted(ret)

def process_stdout(build_path):
    with open(os.path.join(build_path, "stdout.txt")) as f:
        lines = f.readlines()
    
    flash = None
    ram = None
    
    for line in lines:
        if 'Sketch uses' in line:
            flash = line.split(' ')[2]
        if 'Global variables use ' in line:
            ram = line.split(' ')[3]
    
    return (flash, ram)
           
def process_build(info_filename):
    result = {}
    
    with open(info_filename) as f:
        info = json.load(f)
        build_target = info["build_target"]
        sketch_path = info["sketch_path"]
        build_path = info["build_path"]
        git_hash = info["git_hash"]
    
    drive, path = os.path.splitdrive(sketch_path)
    path_parts = os.path.normpath(path).split(os.sep)
        
    library_name = path_parts[path_parts.index("examples") - 1]
    
    library_path = os.path.join(drive, os.sep, *path_parts[:path_parts.index("examples")])
    examples_path = os.path.join(drive, os.sep, *path_parts[:path_parts.index("examples") + 1])
        
    if library_name not in libraries:
    
        libraries[library_name] = {}
        libraries[library_name]["results"] = {}
        libraries[library_name]["examples"] = []
        libraries[library_name]["path"] = library_path
        
        library_file = os.path.join(library_path, "library.properties")
        
        with open(library_file) as f:
            for line in f.readlines():
                key, value = line.split("=")
                libraries[library_name][key] = value.strip();
    
    flash, ram = process_stdout(build_path)
    sketch_path_relative = os.path.dirname(os.path.relpath(sketch_path, examples_path))
    result["sketch_path"] = sketch_path_relative
    result["build_path"] = build_path
    result["library_name"] = library_name
    result["flash"] = flash
    result["ram"] = ram
    
    libraries[library_name]["results"].setdefault(build_target, {})
    libraries[library_name]["results"][build_target][sketch_path_relative] = result
    
    if result["sketch_path"] not in libraries[library_name]["examples"]:
        libraries[library_name]["examples"].append(result["sketch_path"])
    
    build_targets.append(build_target)
    
build_targets = []
libraries = {}

for info_filename in find_finished_compilations(os.path.join("build", git_hash)):
    process_build(info_filename)


json_data = {"core": "STM32GENERIC", "build_targets": sorted(set(build_targets)), "libraries": libraries}

print(json.dumps(json_data, indent=4))

with open("build/latest.json", "w") as f:
    json.dump(json_data, f, indent=4)

with open("build/STM32GENERIC-" + git_hash + ".json", "w") as f:
    json.dump(json_data, f, indent=4)
