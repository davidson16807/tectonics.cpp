import pypeg2
import pypeg2glsl
import pypeg2js

js_math_library_functions = [
    'PI',
    'trunc', 'floor', 'ceil', 'round',
    'sqrt', 'cbrt', 'pow', 'exp', 'log', 'log2',
    'sin', 'cos', 'tan', 
    'asin', 'acos', 'atan', 'atan2', 
    'sinh', 'cosh', 'tanh', 
    'asinh', 'acosh', 'atanh', 
    'max', 'min', 
]
js_glm_library_functions = [
    'radians', 'degrees',
    # 'sin', 'cos', 'tan', 
    # 'asin', 'acos', 'atan', 'atan2', 
    # 'sqrt', 'cbrt', 'pow', 'exp', 'exp2' 'log', 'log2', 'inversesqrt',
    # 'trunc', 'floor', 'ceil', 'round','mod'
    'sqrt', 'fract', 
    'min', 'max', 
    'abs', 'sign', 
    'mix', 'clamp', # 'step', 'smoothstep',
    'length', 'distance', 'dot', 'cross', 'normalize', 
    # 'faceforward', 'reflect', 'refract',
    # 'matrixCompMult', 
    'equal', #'notEqual', 'lessThan', 'lessThanEqual', 'greaterThan', 'greaterThanEqual', 
    'any', 'all', 'not',
    'frexp', 'ldexp',
    'mat2', 'mat3', 'mat4',
    'vec2', 'vec3', 'vec4',
    'uvec2', 'uvec3', 'uvec4',
    'ivec2', 'ivec3', 'ivec4',
    'bvec2', 'bvec3', 'bvec4',
]

def get_default_js_getter(JsClass):
    def _get_js(glsl):
        js = JsClass()
        for attribute in glsl.__dict__:
            setattr(js, attribute, get_js(glsl.__dict__[attribute]))
        return js
    return _get_js

def get_js_postfix_expression(glsl):
    js = pypeg2js.PostfixExpression()
    js.content = []

    expression = glsl.content[0]
    if expression in js_glm_library_functions:
        js.content.append('glm')
        js.content.append(expression)
    elif expression in js_math_library_functions:
        js.content.append('Math')
        js.content.append(expression)
    else:
        js.content.append(get_js(expression))
    for expression in glsl.content[1:]:
        js.content.append(get_js(expression))
    return js

    # is_first = True
    # expression = glsl.content[0]
    # for expression in glsl.content[1:]:
       #  if is_first and expression in js_glm_library_functions:
       #      js.content.append('glm')
       #      js.content.append(expression)
       #  elif is_first and expression in js_math_library_functions:
       #      js.content.append('Math')
       #      js.content.append(expression)
       #  else:
       #      js.content.append(get_js(expression))
    #     is_first = False
    # return js

def get_js_variable_declaration(glsl):
    js = pypeg2js.VariableDeclaration()
    js.qualifiers = ['const' if 'const' in glsl.qualifiers else 'let']
    js.name = glsl.name
    js.value = get_js(glsl.value)
    return js

def get_js_structure_declaration(glsl_structure):
    js_function = pypeg2js.FunctionDeclaration()
    js_function.name = glsl_structure.name
    js_function.parameters = []
    js_function.content = []

    js_empty_invocation = pypeg2js.InvocationExpression()
    js_empty_invocation.content = []
    js_result = pypeg2js.VariableDeclaration()
    js_result.name = 'result'
    js_result.qualifiers = ['let']
    js_result.value = pypeg2js.PostfixExpression();
    js_result.value.content = ['Object', js_empty_invocation]
    js_function.content.append(js_result)

    for glsl_declaration in glsl_structure.content:

        js_parameter = pypeg2js.ParameterDeclaration()
        js_parameter.name = glsl_declaration.name
        js_function.parameters.append(js_parameter)

        js_assignment = pypeg2js.AssignmentExpression()
        js_expression1 = pypeg2js.PostfixExpression()
        js_expression1.content = ['result', glsl_declaration.name]
        js_expression2 = pypeg2js.PostfixExpression()
        js_expression2.content = [glsl_declaration.name]
        js_assignment.operand1 = js_expression1
        js_assignment.operator = '='
        js_assignment.operand2 = js_expression2
        js_function.content.append(js_assignment)


    js_return_value = pypeg2js.PostfixExpression()
    js_return_value.content = ['result']
    js_return = pypeg2js.ReturnStatement()
    js_return.value = js_return_value
    js_function.content.append(js_return)

    return js_function

glsl_js_getter_map = [
    (str,        lambda glsl: glsl),
    (int,        lambda glsl: glsl),
    (type(None), lambda glsl: glsl),

    (pypeg2glsl.PostfixExpression,         get_js_postfix_expression), 
    (pypeg2glsl.PostIncrementExpression,   get_default_js_getter(pypeg2js.PostIncrementExpression)),
    (pypeg2glsl.PreIncrementExpression,    get_default_js_getter(pypeg2js.PreIncrementExpression)),

    (pypeg2glsl.MultiplicativeExpression,  get_default_js_getter(pypeg2js.MultiplicativeExpression)),
    (pypeg2glsl.AdditiveExpression,        get_default_js_getter(pypeg2js.AdditiveExpression)),
    (pypeg2glsl.ShiftExpression,           get_default_js_getter(pypeg2js.ShiftExpression)),
    (pypeg2glsl.RelationalExpression,      get_default_js_getter(pypeg2js.RelationalExpression)),
    (pypeg2glsl.EqualityExpression,        get_default_js_getter(pypeg2js.EqualityExpression)),
    (pypeg2glsl.BitwiseAndExpression,      get_default_js_getter(pypeg2js.BitwiseAndExpression)),
    (pypeg2glsl.BitwiseXorExpression,      get_default_js_getter(pypeg2js.BitwiseXorExpression)),
    (pypeg2glsl.BitwiseOrExpression,       get_default_js_getter(pypeg2js.BitwiseOrExpression)),
    (pypeg2glsl.LogicalAndExpression,      get_default_js_getter(pypeg2js.LogicalAndExpression)),
    (pypeg2glsl.LogicalXorExpression,      get_default_js_getter(pypeg2js.LogicalXorExpression)),
    (pypeg2glsl.LogicalOrExpression,       get_default_js_getter(pypeg2js.LogicalOrExpression)),

    (pypeg2glsl.TernaryExpression,         get_default_js_getter(pypeg2js.TernaryExpression)),

    (pypeg2glsl.BracketedExpression,       get_default_js_getter(pypeg2js.BracketedExpression)),
    (pypeg2glsl.ParensExpression,          get_default_js_getter(pypeg2js.ParensExpression)),
    (pypeg2glsl.InvocationExpression,      get_default_js_getter(pypeg2js.InvocationExpression)),
    (pypeg2glsl.AssignmentExpression,      get_default_js_getter(pypeg2js.AssignmentExpression)),

    (pypeg2glsl.VariableDeclaration,       get_js_variable_declaration),
    (pypeg2glsl.ReturnStatement,           get_default_js_getter(pypeg2js.ReturnStatement)),
    (pypeg2glsl.IfStatement,               get_default_js_getter(pypeg2js.IfStatement)),
    (pypeg2glsl.IfStatementOption,         get_default_js_getter(pypeg2js.IfStatementOption)),
    (pypeg2glsl.WhileStatement,            get_default_js_getter(pypeg2js.WhileStatement)),
    (pypeg2glsl.DoWhileStatement,          get_default_js_getter(pypeg2js.DoWhileStatement)),
    (pypeg2glsl.ForStatement,              get_default_js_getter(pypeg2js.ForStatement)),
    (pypeg2glsl.ParameterDeclaration,      get_default_js_getter(pypeg2js.ParameterDeclaration)),
    (pypeg2glsl.StructureDeclaration,      get_js_structure_declaration),
    (pypeg2glsl.FunctionDeclaration,       get_default_js_getter(pypeg2js.FunctionDeclaration)),

    (pypeg2.List, lambda glsl: [get_js(element) for element in glsl]),
    (list,        lambda glsl: [get_js(element) for element in glsl]),
    (tuple,       lambda glsl: tuple(get_js(element) for element in glsl)),
]

def get_js(glsl):
    for Glsl, _get_js in glsl_js_getter_map:
        if isinstance(glsl, Glsl):
            return _get_js(glsl)
    raise Exception(f'{type(glsl)} not found')




#!/bin/env python3

"""
The command line interface for this script is meant to resemble sed.
You can select a file using the `-f` option.
By default, the script will print out the results of a "dry run". 
You can modify the file in-place using the `-i` option. 
If you want to replace all files in a directory, call like so:
 find . -name *.glsl.c \
     -exec echo {} \; -exec python3 ./replace_goog_base.py -if {} \;
"""
from argparse import ArgumentParser

def convert_file(filename, in_place, verbose=False):
    with open(filename, 'r+') as file:
        text = file.read()
        parsed = pypeg2glsl.parse(text)
        converted = get_js(parsed)
        replaced = pypeg2js.compose(converted)
        if in_place:
            file.seek(0)
            file.write(replaced)
            file.truncate()
        else:
            print(replaced)

if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('-f', '--filename', dest='filename', 
        help='read input from FILE', metavar='FILE')
    parser.add_argument('-i', '--in-place', dest='in_place', 
        help='edit the file in-place', action='store_true')
    parser.add_argument('-v', '--verbose', dest='verbose', 
        help='show debug information', action='store_true')
    args = parser.parse_args()
    main(args.filename, args.in_place, args.verbose)
