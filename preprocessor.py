#!/bin/python3

# IDEA:
# I'm going to wrap alot of weird macros you have to write like the following

# #define FIELD(type, name, def) type name;
# #define DEFAULT(type, name, def) .name = def,
# #define UNPACK(type, name, def) type name = ops.name;
# 
# #define out_FIELDS(X) X(int, n, 1) X(char*, s, "") X(int, indent, 0)
# typedef struct { out_FIELDS(FIELD) } out_Options;
# #define out(...) out_((out_Options){out_FIELDS(DEFAULT) __VA_ARGS__})
# void out_(out_Options ops){
#     out_FIELDS(UNPACK)
# 
#     for(int i = 0; i < n; i++){
#         for(int j = 0; j < indent; j++){
#             printf(" ");
#         }
#         printf("%s\n", s);
#     }
# }

# into 

# @[(int, n, 1), (char*, s, ""), (int, indent, 0)]
# void out(out_Options ops){
#     for(int i = 0; i < n; i++){
#         for(int j = 0; j < indent; j++){
#             printf(" ");
#         }
#         printf("%s\n", s);
#     }
# }

# Only works for structure
#   @[...]
#   typename funcname(){

import sys

args = sys.argv
argc = len(args)

if argc < 2:
    print("no input file: file_name.mc")
    exit(1)

input_file = args[1]


with open(input_file, "r") as f:
    out_file  = ""
    out_file += "#define FIELD(type, name, def) type name;\n"
    out_file += "#define DEFAULT(type, name, def) .name = def,\n"
    out_file += "#define UNPACK(type, name, def) type name = ops.name;\n"

    lines = f.readlines()

    i = 0
    while i < len(lines):
        line = lines[i]
        if len(line) > 0 and line[0] == "@":
            # parse optvars
            assert line[1] == "[", "expect ["
            
            j = 2
            # (type, name, default)
            optvars = []
            while j < len(line) and line[j] != "]":
                assert line[j] == "(", f"expect (, got {line[j]}"
                j += 1
                
                comma = line[j:].find(",") + j
                typ = line[j: comma]
                j = comma + 2
                
                comma = line[j:].find(",") + j
                name = line[j: comma]
                j = comma + 2

                paren = line[j:].find(")") + j
                value = line[j: paren]
                j = paren + 3

                optvars.append((typ, name, value))
            
            # parse func signature line
            assert len(lines) > i + 1, "expected func def, found EOF"
            i += 1

            fundef = lines[i]
            fn = fundef[fundef.find(" ") + 1: fundef.find("(")]
            # write X macros, struct and wrapper macro
            out_file += f"#define {fn}_FIELDS(X) "
            for optvar in optvars:
                out_file += f"X({', '.join(optvar)}) "
            out_file += "\n"

            out_file += f"typedef struct {{ {fn}_FIELDS(FIELD) }} {fn}_Options;\n"
            out_file += f"#define {fn}(" 
            
            req_args = fundef[fundef.find("(") + 1: fundef.find(")")].split(", ")
            for ra in req_args:
                if len(ra) != 0:
                    out_file += ra.split(" ")[1] + ", "


            out_file += f"...) {fn}_(({fn}_Options) {{ {fn}_FIELDS(DEFAULT) __VA_ARGS__ }}" 
       
            for p in range(len(req_args)):
                ra = req_args[p]
                if len(ra) != 0:
                    out_file += ", " + ra.split(" ")[1]


            out_file += ")\n"

            # modify func signature line
            out_file += fundef[:fundef.find(" ")]
            out_file += f" {fn}_({fn}_Options ops"
            if fundef.find(")") - fundef.find("(") - 1 != 0:
                # has args
                out_file += ", "

            out_file += fundef[fundef.find("(") + 1:]


            # add UNPACK macro inside of func
            out_file += f"    {fn}_FIELDS(UNPACK)\n" 

        else:
            out_file += line

        i += 1

with open(input_file[:input_file.find(".")] + ".c", "w") as f:
    f.write(out_file)









