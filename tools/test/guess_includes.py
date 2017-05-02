import os
import glob
import json
import re
import subprocess
from subprocess import call, check_output

BOARD = "BluePill_F103C8"

DIR_CORES = "../../STM32"
DIR_PROJECTS = "sources/arduino_projects"

def find_examples(dir, list):
    if not os.path.isdir(dir):
        return
    
    subdirs = os.listdir(dir)
    for subdir in subdirs:
        if os.path.exists(dir + '/' + subdir + '/' + subdir + '.ino'):
            list.append(dir + '/' + subdir)

        if os.path.exists(dir + '/' + subdir + '/' + subdir + '.pde'):
            list.append(dir + '/' + subdir)

        find_examples(dir + '/' + subdir, list)

def find_directories():
    dirs = []

    DIR_LIBRARIES = DIR_CORES + "/libraries"
    core_libs = os.listdir(DIR_LIBRARIES)
    for core_lib in core_libs:
        if os.path.exists(DIR_LIBRARIES + "/" + core_lib + '/src'):
            dirs.append(DIR_LIBRARIES + "/" + core_lib + '/src')
        else:
            dirs.append(DIR_LIBRARIES + "/" + core_lib)
        
        find_examples(DIR_LIBRARIES + '/' + core_lib + '/examples', dirs)
        
    return dirs
    
    projects = os.listdir(DIR_PROJECTS)
    for project in projects:
        if os.path.exists(DIR_PROJECTS + '/' + project + '/src'):
            dirs.append(DIR_PROJECTS + '/' + project + '/src')
        else:
            dirs.append(DIR_PROJECTS + '/' + project)

        if os.path.exists(DIR_PROJECTS + '/' + project + '/examples'):
            examples = os.listdir(DIR_PROJECTS + '/' + project + '/examples')

            #TODO use glob.glob()
            for example in examples:
                if not os.path.isdir(DIR_PROJECTS + '/' + project + '/examples/' + example):
                    continue
                if os.path.exists(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + example + '.ino'):
                    dirs.append(DIR_PROJECTS + '/' + project + '/examples/' + example)

                if os.path.exists(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + example + '.pde'):
                    dirs.append(DIR_PROJECTS + '/' + project + '/examples/' + example)

                subexamples = os.listdir(DIR_PROJECTS + '/' + project + '/examples/' + example)
                for subexample in subexamples:
                    if not os.path.isdir(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + subexample):
                        continue

                    if os.path.isdir(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + subexample) and os.path.exists(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + subexample + '/' + subexample + '.ino'):
                        dirs.append(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + subexample)
                    if os.path.isdir(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + subexample) and os.path.exists(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + subexample + '/' + subexample + '.pde'):
                        dirs.append(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + subexample)

                    subsubexamples = os.listdir(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + subexample)
                    for subsubexample in subsubexamples:
                        if os.path.isdir(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + subexample + '/' + subsubexample) and os.path.exists(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + subexample + '/' + subsubexample + '/' + subsubexample + '.ino'):
                            dirs.append(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + subexample + '/' + subsubexample)
                        if os.path.isdir(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + subexample + '/' + subsubexample) and os.path.exists(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + subexample + '/' + subsubexample + '/' + subsubexample + '.pde'):
                            dirs.append(DIR_PROJECTS + '/' + project + '/examples/' + example + '/' + subexample + '/' + subsubexample)

    return dirs

core_includes = os.listdir(DIR_CORES + "/cores/arduino")


def remove_all(lst, key):
    return [val for val in lst if val!=key]

def get_includes(filename):
    file = open(filename, "r")
    includes = []

    if "Printoo" in filename: #TODO remove this when SoftwareSerial is available
        return includes

    declarations = []

    for line in file:
        if '//' in line:
            line = line[:line.index('//')]

        line = line.rstrip()

        declaration_found = re.search('^([a-zA-Z].* .*\\(.*\\).*){$', line)
        if declaration_found:
            func_decl = declaration_found.groups()[0] + ';'
            if func_decl.startswith('SIGNAL'):
                    continue
            if "::" in func_decl[:func_decl.index('(')]:
                    continue
            if "." in func_decl[:func_decl.index('(')]:
                    continue
            if 'void setup(' in func_decl:
                    continue
            if 'void loop(' in func_decl:
                    continue
            declarations.append(func_decl);

        if line.rstrip() == '{':
            combined = previous_line.rstrip() + line
            declaration_found = re.search('^([a-zA-Z].* .*.*\\(.*\\).*){$', combined)
            if declaration_found:
                func_decl = declaration_found.groups()[0] + ';'
                if 'void setup(' in func_decl:
                        continue
                if 'void loop(' in func_decl:
                        continue
                declarations.append(func_decl);


        found = re.search('include.*?"(.*)"', line)
        if found:
            includes.append(found.groups()[0])
        found = re.search('include.*?<(.*)>', line)
        if found:
            includes.append(found.groups()[0])


        previous_line = line


    for ignore_include in core_includes:
        includes = remove_all(includes, ignore_include)
    includes = remove_all(includes, "string.h")
    includes = remove_all(includes, "variant.h")
    includes = remove_all(includes, "time.h")

    if filename.endswith('.ino') or filename.endswith('.pde'):
        with open(filename + '.h', 'w') as file:
            if declarations:
                for include in includes:
                    if include.startswith('avr/'):
                        continue

                    file.write('#include "' + include + '"\n');
            file.write("\n".join(declarations))

    return includes

dirs = find_directories();

header_to_dir = {}

for dir in dirs:
    if 'examples' in dir:
        continue
    #print dir
    headers = [file for file in os.listdir(dir) if file.endswith(('.h', '.hpp'))]
    for header in headers:
        if header in header_to_dir:
            pass
            #print header + ": " + header_to_dir[header] + "=>" + dir
        else:
            header_to_dir[header] = dir

def get_sources(dir, subdir):
    sources = [subdir + file for file in os.listdir(dir) if file.endswith(('.h', '.hpp', '.c', '.cpp', '.ino', '.pde'))]
    for file in os.listdir(dir):
        if os.path.isdir(dir + '/' + file) and file != 'examples':
            sources.extend(get_sources(dir + '/' + file, subdir + file + '/'))
    return sources

def create_includes():
    #print check_output(["find", ".", "-name", "'build-include-*.txt'", "-delete"], stderr=subprocess.STDOUT)

    for dir in dirs:

        sources = get_sources(dir, '')

        dirs_to_include = []

        for source in sources:
            includes = get_includes(dir + '/' + source)

            for include in includes:
                skip = False
                for s in sources:
                    if s == include or include.endswith('/' + s) or s.endswith('/' + include):
                        skip = True

                if skip:
                    continue

                if include in header_to_dir:
                    dirs_to_include.append(header_to_dir[include])

        dirs_to_include = list(set(dirs_to_include))

        if dir in dirs_to_include:
            dirs_to_include.remove(dir)

        add = []

        for dir_to_include in dirs_to_include:
            if os.path.exists(dir_to_include + '/build-include.txt'):
                with open(dir_to_include + '/build-include.txt', 'r') as file:
                    add.extend(file.readline().strip().split(" "))

        dirs_to_include.extend(add)

        dirs_to_include = list(set(dirs_to_include))

        with open(dir + '/build-include.txt', 'w') as file:
            for d in dirs_to_include:
                file.write(d + ' ')

create_includes()

exit()
                
already_compiled = []

def make(directory):
    if (directory in already_compiled):
        return
    print directory

    already_compiled.append(directory)

    #try:
    with open(directory + '/build-include-' + ARCH + '.txt') as f:
            dependencies= f.readline()
            for dependency in dependencies.split():
                make(dependency)
    #except:
    #    print 'DEPENDENCIES NOT CREATED'

    call(['make', 'clean', 'ARCH='+ARCH, 'BOARD='+BOARD, 'PROJECT='+directory])

    try:
        os.mkdir(directory+"/build-"+BOARD)
    except OSError:
        pass

    stdout = open(directory+"/build-"+BOARD+"/stdout.txt", "w")
    stderr = open(directory+"/build-"+BOARD+"/stderr.txt", "w")
    call(['make', 'ARCH='+ARCH, 'BOARD='+BOARD, 'PROJECT='+directory], stdout = stdout, stderr = stderr)
    stdout.close()
    stderr.close()

#create_includes()

#raise X

for dir in dirs:
    make(dir)
#make('sources/arduino_projects/Adafruit_GFX_Library-1.1.5')
#make('sources/arduino_projects/Adafruit_ILI9341-1.0.1/examples/graphicstest')
#make('sources/arduino_projects/LcdProgressBarDouble-1.0.4/examples/DoubleBarPot')
#make('sources/arduino_projects/RTClib-1.2.0')
#make('sources/arduino_projects/Arduino_GUI-1.6.11/examples/01.Basics/Blink')
#make('sources/arduino_projects/SD-1.0.6/src')
#make('sources/arduino_projects/SD-1.0.6/examples/listfiles')
#make('sources/arduino_projects/Cayenne-1.0.1')
#make('sources/arduino_projects/ArduinoJson-5.6.7')
#make('sources/arduino_projects/ArduinoJson-5.6.7/examples/JsonParserExample')
#make('sources/arduino_projects/EMoRo_2560-2.4.1/src')
#make('sources/arduino_projects/Kalman_Filter_Library-1.0.1/examples/MPU6050')
#make('sources/arduino_projects/SparkFun_Graphic_LCD_Serial_Backpack-1.0.1/examples/SparkFunSerialGraphicLCDDemo')
#make('sources/arduino_projects/DHT_sensor_library-1.2.3')
#make('sources/arduino_projects/DHT_sensor_library-1.2.3/examples/DHTtester')
#make('sources/arduino_projects/Adafruit_GPS_Library-1.0.1/examples/leo_locus_erase')
#make('sources/arduino_projects/DimSwitch-1.0.2/examples/DimSwitchTester-ESP-MQTT')
#make('sources/arduino_projects/MFRC522-1.1.8/examples/RFID-Cloner')
#make('sources/arduino_projects/U8glib-1.19.1/examples/HelloWorld')
#make('sources/arduino_projects/Adafruit_CC3000_Library-1.0.3/examples/HTTPServer')
#make('sources/arduino_projects/Adafruit_Fingerprint_Sensor_Library-1.0.0')
#make('sources/arduino_projects/LiquidCrystal_I2C-1.1.2/examples/HelloWorld')
#make('sources/arduino_projects/AccelStepper')

#raise Error

output = []

errors = {}

for dir in dirs:
    #print dir
    error = False
    if not os.path.exists(dir+"/build-"+BOARD+"/stderr.txt"):
        continue

    with open(dir+"/build-"+BOARD+"/stderr.txt", "r") as file:
        for line in file:
            #Find the most relevant error line
            if 'warning: changing start of section ' in line:
                continue
            if 'In function ' in line:
                continue
            if 'In constructor ' in line:
                continue
            if 'In member function ' in line:
                continue
            if 'In file included from ' in line:
                continue
            if 'from sources/' in line:
                continue
            if 'from ./sources/' in line:
                continue
            if 'from <command-line>' in line:
                continue

            if not error: error = line

    #print "ERROR = ", error
    if not error:
        error = "SUCCESS"

    errors[dir] = error.strip()

with open('cache/library_index.json') as file:
    data = json.load(file)

libraries = {}

for library in data["libraries"]:
    libraries[library["name"]] = library

for name, library in libraries.iteritems():
    if "github.com" in library["website"]:
        api = library["website"].replace("github.com", "api.github.com/repos")
        api = api.replace("https", "http")
        if api.endswith(".git"):
            api = api[:-4]
        download_file = "cache/github_api/" + api[len("http://api.github.com/repos")+1:].replace("/", ":")

        if os.path.exists(download_file):
            with open(download_file) as file:
                data = json.load(file)
                libraries[name]["github_api"] = data
                libraries[name]["sort"] = data["forks_count"] + data["watchers_count"] + data["stargazers_count"] + data["subscribers_count"]
                libraries[name]["sort"] = data["stargazers_count"]

    zip_file = os.path.basename(library["url"])
    projects_folder = DIR_PROJECTS + "/" + zip_file[:-4]
    if projects_folder in errors:
        libraries[name]["error"] = errors[projects_folder]
    elif (projects_folder + "/src") in errors:
        libraries[name]["error"] = errors[projects_folder + "/src"]
    else:
        libraries[name]["error"] = "NOT FOUND"

    libraries[name]['example_errors'] = {}
    libraries[name]['projects_folder'] = projects_folder

    for error_folder in errors:
        if 'examples' in error_folder and error_folder.startswith(projects_folder):
            #print projects_folder, error_folder, errors[error_folder]
            libraries[name]['example_errors'][error_folder] = errors[error_folder]

    #if (libraries[name]['author'] == 'Arduino'):
    #    libraries[name]["sort"] = 10000



def sort_library(a):
    if "sort" in libraries[a]:
        return libraries[a]["sort"]
    else:
        return 0

library_names = sorted(libraries, key=sort_library, reverse=True)

for name in library_names:
    if 'sort' in libraries[name]:
        pass
        #print name, libraries[name]["sort"], libraries[name]["website"], libraries[name]["projects_folder"], libraries[name]["error"], libraries[name]["example_errors"]

with open('result.json', 'w') as file:
    file.write(json.dumps({'libraries': libraries, 'library_names': library_names}))


