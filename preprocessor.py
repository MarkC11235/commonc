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
#       ...
#   }


# Next Ideas 
# More types of decorators and allowing multiple of them stacked on a function (order may matter)

# @default_args[...] // Will eventually just make this look into the function signature to extract the default args 
# @time              // Times the function and prints to the terminal (maybe add options to redcirect the output)
# ...

# Timer code 

import sys

def parse_default_args_decorator(line: str, fndef: str):
    out_file = ""

    # parse optvars
    assert line[0] == "[", "expect ["

    j = 1
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

    # this is why the function type cannot have multiple words before the func name becasue I split on space
    fn_name = fndef[fndef.find(" ") + 1: fndef.find("(")] # func name 

    # write x macros, 
    out_file += f"#define {fn_name}_fields(_X) "
    for optvar in optvars:
        out_file += f"_X({', '.join(optvar)}) "
    out_file += "\n"
    # options struct,
    out_file += f"typedef struct {{ {fn_name}_fields(field) }} {fn_name}_options;\n"
    # and wrapper macro
    out_file += f"#define {fn_name}(" 

    # extract required args from fn def and write their names as the macro params 
    req_args = fndef[fndef.find("(") + 1: fndef.find(")")].split(", ")
    for ra in req_args:
        if len(ra) != 0:
            out_file += ra.split(" ")[1] + ", "


    out_file += f"...) {fn_name}_(({fn_name}_options) {{ {fn_name}_fields(default_arg) __VA_ARGS__ }}" 
    
    # now write the required aregs into the func call inside the wrapper macro 
    for p in range(len(req_args)):
        ra = req_args[p]
        if len(ra) != 0:
            out_file += ", " + ra.split(" ")[1]

    out_file += ")\n"


    # modify func signature line
    out_file += fndef[:fndef.find(" ")] # stuff before fn name 
    out_file += f" {fn_name}_({fn_name}_options ops"
    if fndef.find(")") - fndef.find("(") - 1 != 0:
        # has args
        out_file += ", "

    out_file += fndef[fndef.find("(") + 1:]


    # add unpack macro inside of func
    out_file += f"    {fn_name}_fields(unpack)\n" 

    return out_file

def process_mc_to_c(input_file: str) -> str:
    out_file  = ""
    # headers
    out_file += "#include <stdio.h>\n#include <stdlib.h>\n#include <time.h>\n\n" # for time decorator
    out_file += "struct timespec start, end; \\\n"
    out_file += "double time_taken_sec;      \\\n"
    out_file += "double time_taken_nsec;     \n"
    out_file += "#define TIME_START clock_gettime(CLOCK_MONOTONIC, &start); \n"
    out_file += "#define TIME_END                                           \\\n"
    out_file += "   clock_gettime(CLOCK_MONOTONIC, &end);                  \\\n"
    out_file += "   time_taken_sec = (end.tv_sec - start.tv_sec);          \\\n"
    out_file += "   time_taken_nsec = (end.tv_nsec - start.tv_nsec) / 1e9; \\\n"
    out_file += "   printf(\"elapsed time: %f seconds\\n\", time_taken_sec + time_taken_nsec);\n"

    # for default args decorator
    out_file += "#define field(type, name, def) type name;\n"
    out_file += "#define default_arg(type, name, def) .name = def,\n"
    out_file += "#define unpack(type, name, def) type name = ops.name;\n"
    
    with open(input_file, "r") as f:
        lines = f.readlines()

        i = 0
        while i < len(lines):
            line = lines[i]
            if len(line) == 0:
                i+=1
                continue

            if line[0] == "@": # found a decorator
                if line[1:13] == "default_args":
                    # parse func signature line
                    assert len(lines) > i + 1, "expected func def, found eof"
                    i += 1
                    fndef = lines[i]
                    
                    out_file += parse_default_args_decorator(line[13:], fndef)
                elif line[1:5] == "time":
                    # TODO: This will fail if there are brackets inside strings that don't match
                    # need to gather the whole function def 
                    stack = []
                    found_opener = False
                    fn = ""
                    while len(stack) != 0 or not(found_opener): # search for closing brackets
                        i+=1
                        next_line = lines[i]
                        for k in range(len(next_line)):
                            if next_line[k] == "{":
                                found_opener = True
                                stack.append("{")
                            elif next_line[k] == "}":
                                stack.pop()
                        fn += next_line
                    fn = fn[:fn.find("{")+1] + "\n    TIME_START" + fn[fn.find("{")+1:fn.rfind("}")] + "    TIME_END\n}\n"

                    out_file += fn
                else:
                    assert False, f"invalid decorator type: {line}"
            else:
                out_file += line

            i += 1

    return out_file


def main():
    args = sys.argv
    argc = len(args)

    if argc < 2:
        print("no input file: file_name.mc")
        exit(1)

    input_file = args[1]

    out_file = process_mc_to_c(input_file)

    with open(input_file[:input_file.find(".")] + ".c", "w") as f:
        f.write(out_file)



main()


