from collections import OrderedDict

root = '../../STM32';

with open(root + '/boards.txt') as file:
    boards = file.readlines()

with open(root + '/platform.txt') as file:
    platform = file.readlines()

variants = {}
variant_nucleos = {}
variant_uploads = {}
    
for line in boards:
    line = line.strip()
    
    if not line or line.startswith('#') or line.startswith('menu'):
        continue
        
    (board, data) = line.split('.', 1)
    
    (key, value) = data.split('=', 1)
    
    key = key.strip()
    value = value.strip()
    
    if key.startswith('menu.subboard.'):
        key = key.replace('menu.subboard.', '')
        if '.' not in key:
            continue

        (upload, k) = key.split('.', 1)
        variant_nucleos.setdefault(board + '_' + upload, variants[board].copy())[k] = value
    
    elif key.startswith('menu.upload_method.'):
        key = key.replace('menu.upload_method.', '')
        if '.' not in key or 'MassStorageMethod' in key or 'DFUUploadMethod2' in key:
            continue

        (upload, k) = key.split('.', 1)
        if variant_nucleos:
            for name, keyvalues in variant_nucleos.iteritems():
                variant_uploads.setdefault(name + '_' + upload, keyvalues)[k] = value
        else:
            variant_uploads.setdefault(board + '_' + upload, variants[board].copy())[k] = value
    else:
        variants.setdefault(board, OrderedDict())[key] = value

del variant_uploads['BLACK_F407XX_serialMethod']
del variant_uploads['BLACK_F407XX_STLinkMethod']

for variant in variants.keys():
    upload_exists = False
    for variant_upload in variant_uploads.keys():
        if variant in variant_upload:
            upload_exists = True
            break
    if not upload_exists:
        variant_uploads[variant] = variants[variant]

for (variant, replaces) in variant_uploads.iteritems():
    replaces = replaces.copy()
    replaces['build.project_name'] = '$(VARIANT)/$(PROJECT)/out'
    replaces['object_file'] = '$@'
    replaces['source_file'] = '$<'
    replaces['includes'] = '$(INCLUDES)'
    replaces['object_files'] = '$(OBJECTS)'
    
    replaces['runtime.tools.arm-none-eabi-gcc.path'] = '$(PATH_COMPILER)'
    
    replaces['runtime.ide.version'] = '10802'
    replaces['build.path'] = 'build'
    replaces['build.arch'] = 'STM32GENERIC'
    replaces['runtime.hardware.path'] = root
    replaces['build.system.path'] = root + '/system'
    replaces['build.core.path'] = root + '/cores/arduino'
    replaces['build.variant.path'] = '$(PATH_VARIANT)'
    
    replaces['build.extra_flags_usb'] = '-DMENU_USB_SERIAL'

    with open('makefiles/' + variant, 'w') as file:
        
        for key, value in replaces.iteritems():
            file.write(key + '=' + value + '\n')
        
        for line in platform:
            line = line.strip()
            
            if not line or line.startswith('#'):
                continue
                
            for (key, value) in replaces.iteritems():
                line = line.replace('{' + key + '}', value)
            
            (key, value) = line.split('=', 1)
            
            key = key.strip()
            value = value.strip()
            
            if value or key not in replaces:
                replaces[key] = value
            
            line = line.replace('"build/{archive_file}"', '')
            
            file.write(line + '\n')
