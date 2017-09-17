# Compile from command line

# Example: python run_build.py -v MapleMini_F103CB -s STM32/libraries\HardwareTest\examples\AutomaticCompilation\Basic\Basic.ino -i

import sys
sys.dont_write_bytecode = True

import subprocess
import os
import fnmatch
import shutil
import errno
import argparse
import board
import json
import hashlib

#print(board.boards)
#exit(-1)

hardware_root = os.path.abspath('../../../')
stm32generic_root = os.path.abspath('../../')
arch_root = os.path.join(stm32generic_root, 'STM32')
tools_root = os.path.join(stm32generic_root, 'tools')

arduino_builder_default = "C:\\Program Files (x86)\\Arduino\\arduino-builder.exe"
arm_gcc_path_default = "C:\\Users\\" + str(os.environ.get('USERNAME')) + "\\AppData\\Local\\Arduino15\\packages\\arduino\\tools\\arm-none-eabi-gcc\\4.8.3-2014q1"

git_hash = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD']).decode("ascii").strip()
git_date = subprocess.check_output(['git', 'show', '-s', '--format=%ci']).decode("ascii").strip()

def build(args, build_target, sketch_path, build_path):
    print("Building: ", sketch_path) 
    print("Variant: ", build_target) 
    print("Build path: ", build_path) 
    
    already_compiled = os.path.join(build_path, "already_compiled.txt")
    
    if os.path.exists(already_compiled) and args.ignore_already_compiled:
        return
    
    if os.path.exists(build_path):
        shutil.rmtree(build_path)
    
    os.makedirs(build_path, exist_ok = True)

    command = []
    command.append(args.arduino_builder)
    
    command.append("-hardware")
    command.append(hardware_root)
    
    command.append("-hardware")
    command.append(os.path.dirname(args.arduino_builder) + "/hardware")
    
    command.append("-tools")
    command.append(os.path.dirname(args.arduino_builder) + "/tools-builder")
    
    command.append("-ide-version=10606")
    
    command.append("-fqbn")
    command.append(build_target)
    
    
    #command.append("-warnings=all")
    
    command.append("-verbose")
    
    command.append("-build-path")
    command.append(os.path.abspath(build_path))
    
    command.append("-prefs")
    command.append("runtime.tools.arm-none-eabi-gcc.path=" + args.arm_gcc_path)
    
    command.append(sketch_path)
    
    print(command)
    
    with open(os.path.join(build_path, "stdout.txt"), "w") as stdout, open(os.path.join(build_path, "stderr.txt"), "w") as stderr:
        proc = subprocess.Popen(command, stdout = stdout, stderr = stderr)
        #proc = subprocess.Popen(command)
    
    proc.wait()
    
    for dir in os.listdir(build_path):
        if os.path.isdir(os.path.join(build_path, dir)):
            shutil.rmtree(os.path.join(build_path, dir))
    
    with open(os.path.join(build_path, "disassembly.asm"), "w") as f:
        proc = subprocess.Popen([os.path.join(args.arm_gcc_path, "bin", "arm-none-eabi-objdump"), '-d', os.path.join(build_path, os.path.basename(sketch_path) + ".elf")], stdout = f)
    
    with open(os.path.join(build_path, "nm.txt"), "w") as f:
        proc = subprocess.Popen([os.path.join(args.arm_gcc_path, "bin", "arm-none-eabi-nm"), '-S', '--size-sort', '-t', 'd', os.path.join(build_path, os.path.basename(sketch_path) + ".elf")], stdout = f)
        
    with open(already_compiled, "w") as f:
        f.write("Compilation finished")


def process_stdout(args, build_target, sketch_path, build_path):
    with open(os.path.join(build_path, "stdout.txt")) as f:
        lines = f.readlines()
    
    flash = None
    ram = None
    
    for line in lines:
        if 'Sketch uses' in line:
            flash = line.split(' ')[2]
        if 'Global variables use ' in line:
            ram = line.split(' ')[3]
    
    print("Result: ", flash, ram)
    print("--------")
    return (flash, ram)
            

def find_inos(path):
    ret = []
    for root, dirnames, filenames in os.walk(path):
        for filename in fnmatch.filter(filenames, '*.ino'):
            ret.append(os.path.join(root, filename))
    return ret
    
            
def run(args, build_target, sketch):
    sketch_path = sketch
    
    if not sketch.startswith('/'):
        sketch_path = stm32generic_root + '/' + sketch

    hash = hashlib.sha256(sketch.encode('utf-8')).hexdigest()[:10].strip()
        
    build_path = os.path.join(os.path.abspath("."), "build", build_target.replace(":", "_").replace(",", "_").replace("=", "_"), git_hash, hash)
            
    build(args, build_target, sketch_path, build_path)
    return {"build_path": build_path, "result": process_stdout(args, build_target, sketch_path, build_path)}

def run_manual(args):
    for variant in args.variant:
        for sketch in args.sketch:
            build_target = "STM32GENERIC:STM32:" + variant
            
            result = run(args, build_target, sketch)
            
            key = build_target + "|" + git_hash + "|" + sketch
            results[key] = result

def run_auto(args):
    
    variants = [
        "MapleMini_F103CB:upload_method=STLinkMethod,usb=Disabled",
        "MapleMini_F103CB:upload_method=STLinkMethod,usb=SerialUSB",
        "MapleMini_F103CB:upload_method=DFUUploadMethod2,usb=Disabled",
        "MapleMini_F103CB:upload_method=DFUUploadMethod2,usb=SerialUSB",
        
        "BluePill:upload_method=STLinkMethod,usb=SerialUSB",
        
        "BLACK_F407XX:upload_method=STLinkMethod,usb=Disabled,subboard=BLACK_F407VE",
        "BLACK_F407XX:upload_method=STLinkMethod,usb=SerialUSB,subboard=BLACK_F407VE",
        
        
        "BLACK_F407XX:upload_method=STLinkMethod,usb=SerialUSB,subboard=BLACK_F407VE",
    ]
    
    for variant in board.boards:
        if 'Nucleo' in variant: # Nucleo 64
            if variant[11] == 'R':
                variants.append('NUCLEO_64:subboard=' + variant)
            else:
                variants.append('NUCLEO_144:subboard=' + variant)
                
        if 'DISCOVERY' in variant:
            variants.append(variant)

    for variant in variants:
        inos = []
        inos.extend(find_inos(os.path.join(stm32generic_root, "STM32/libraries/HardwareTest/examples/AutomaticCompilation")))
        inos.extend(find_inos(os.path.join(stm32generic_root, "STM32/libraries/BoardExamples/examples/all_boards")))
        inos.extend(find_inos(os.path.join(stm32generic_root, "STM32/libraries/HardwareTimer/examples")))
        inos.extend(find_inos(os.path.join(stm32generic_root, "STM32/libraries/FreeRTOS/examples")))
        inos.extend(find_inos(os.path.join(stm32generic_root, "STM32/libraries/EEPROM/examples")))
        
        for sketch in inos:
            build_target = "STM32GENERIC:STM32:" + variant
            
            sketch = os.path.relpath(sketch, stm32generic_root)
            
            result = run(args, build_target, sketch)
            
            key = build_target + "|" + git_hash + "|" + sketch
            results[key] = result
    
parser = argparse.ArgumentParser(description='Process some integers.')

parser.add_argument('-v', '--variant', choices = board.boards, action = 'append', default = [])
parser.add_argument('-s', '--sketch', action = 'append', default = [])
parser.add_argument('-b', '--arduino_builder', default = arduino_builder_default)
parser.add_argument('-g', '--arm_gcc_path', default = arm_gcc_path_default)
parser.add_argument('-i', '--ignore_already_compiled', action="store_true")
parser.add_argument('-a', '--auto', action="store_true")
parser.add_argument('-o', '--output')

args = parser.parse_args()

if not args.auto and (not args.variant or not args.sketch):
    print("error: Please specify either -a or -v + -s")
    parser.print_help()
    exit(-1)
    
if not os.path.exists(args.arduino_builder):
    print("error: " + args.arduino_builder + " does not exist")
    print("       please use -b to set")
    exit(-1)
    
if not os.path.exists(args.arduino_builder):
    print("error: " + args.arm_gcc_path + " does not exist")
    print("       please use -g to set")
    exit(-1)

results = {}
if not args.auto:
    run_manual(args)
else:
    run_auto(args)
    
compiler = os.path.basename(os.path.dirname(args.arm_gcc_path)) + " " + os.path.basename(args.arm_gcc_path)

json_data = {"core": "STM32GENERIC", "compiler": compiler, "git_date": git_date, "git_hash": git_hash, "results": results}

with open("build/STM32GENERIC-" + git_hash + ".json", "w") as f:
    json.dump(json_data, f, indent=4)
    
if args.output:
    with open(args.output, "w") as f:
        json.dump(json_data, f, indent=4)
