import copy
import config

import re

from collections import OrderedDict

with open(config.arch_dir + "boards.txt") as f:
    boardLines = f.readlines()

boards = OrderedDict()
del_boards = []
tree = OrderedDict()

for line in boardLines:
    line = line.strip()
    if line.startswith("#") or line.startswith("menu") or not line:
        continue
    
    (id, rest) = line.split(".", 1)
    (key, value) = rest.split("=", 1)
    
    if key.startswith("menu.subboard"):
        subline = key[len("menu.subboard") + 1:]
        if not '.' in subline:
            subid = subline
            key = "name"
        else:
            (subid, key) = subline.split(".", 1)
        
        if id in boards:
            boards.setdefault(subid, copy.deepcopy(boards[id]))
            del_boards.append(id)

        if key == 'name':
            if isinstance(tree[parent], str):
                tree[parent] = OrderedDict()
            tree[parent][value] = subid
            
        id = subid
    else:
        if key == 'name':
            parent = value
            tree[value] = id
    
    boards.setdefault(id, {})
    boards[id][key.strip()] = value.strip()
    
for id in set(del_boards):
    del boards[id]

for (id, board) in boards.items():
    variant_dir = board["build.variant"]
    with open(config.arch_dir + "variants/" + variant_dir + "/variant.h", encoding='utf-8') as f:
        variant_lines = f.read().splitlines()
    
    board["variant_lines"] = variant_lines
    
    board["macros"] = {}
    board["pins"] = OrderedDict()
    
    for line in variant_lines:
        values = line.split(" ")
        values = [x for x in values if x]
        
        if line.startswith("#define") and len(values) > 2:
            key = values[1]
            value = values[2]
            if '//' in value:
                value = value[:value.index('//')].strip()
            
            board["macros"][key] = value
        
        if 'VARIANT_PIN_LIST' in line:
            pin_number = 0
        
        if 'PIN(' in line:
            m = re.match(".*PIN\((.*),(.*)\)", line)
            pin = 'P' + m.group(1).strip() + m.group(2).strip()
            board["pins"][pin] = pin_number
            
            pin_number = pin_number + 1
