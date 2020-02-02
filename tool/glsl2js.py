#!/bin/env python3

import sys

import pypeg2
import pypeg2glsl
import pypeg2js

# attempt to import colorama, for colored diff output
try:
    from colorama import Fore, Back, Style, init
    init()
except ImportError:  # fallback so that the imported classes always exist
    class ColorFallback():
        __getattr__ = lambda self, name: ''
    Fore = Back = Style = ColorFallback()


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

def get_js_default_element_getter(JsElement):
    def get_js_default_element(glsl, scope):
        js = JsElement()
        for attribute in glsl.__dict__:
            setattr(js, attribute, get_js(glsl.__dict__[attribute], scope))
        return js
    return get_js_default_element


def get_js_postfix_expression(glsl, scope):
    js = pypeg2js.PostfixExpression()
    js.content = []

    is_first = True
    for expression in glsl.content:
        if is_first and expression in js_glm_library_functions:
            js.content.append('glm')
            js.content.append(expression)
        elif is_first and expression in js_math_library_functions:
            js.content.append('Math')
            js.content.append(expression)
        else:
            js.content.append(get_js(expression, scope))
        is_first = False
    return js


def get_js_unary_operator_expression_getter(JsElement):

    get_js_default_operator_expression = get_js_default_element_getter(JsElement)
    def get_js_unary_glm_operator_expression(glsl_operand1, operator, scope):
        type1 = pypeg2glsl.get_expression_type(glsl_operand1, scope)
        assert operator != '!', 'Unary negation for vectors/matrices is not supported by glm-js, cannot safely continue'
        if operator == '++':
            return pypeg2js.PostfixExpression([
                    pypeg2js.ParensExpression([get_js(glsl_operand1, scope)]), 
                    pypeg2.parse('["+="]', pypeg2js.BracketedExpression),
                    pypeg2js.InvocationExpression(pypeg2js.PostfixExpression(['glm', *type1, pypeg2js.InvocationExpression(['1'])]))
                ])
        elif operator == '--':
            return pypeg2js.PostfixExpression([
                    pypeg2js.ParensExpression([get_js(glsl_operand1, scope)]), 
                    pypeg2.parse('["-="]', pypeg2js.BracketedExpression),
                    pypeg2js.InvocationExpression(pypeg2js.PostfixExpression(['glm', *type1, pypeg2js.InvocationExpression(['1'])]))
                ])
        elif operator == '-' and isinstance(glsl_operand1, pypeg2glsl.PostfixExpression):
            return pypeg2js.PostfixExpression([
                    *glsl_operand1.content, 
                    pypeg2.parse('["*"]', pypeg2js.BracketedExpression),
                    pypeg2.parse('(-1)', pypeg2js.InvocationExpression)
                ])
        elif operator == '-':
            return pypeg2js.PostfixExpression([
                    pypeg2js.ParensExpression([get_js(glsl_operand1, scope)]), 
                    pypeg2.parse('["*"]', pypeg2js.BracketedExpression),
                    pypeg2.parse('(-1)', pypeg2js.InvocationExpression)
                ])
        elif operator == '+':
            return pypeg2js.ParensExpression([get_js(glsl_operand1, scope)])
        else:
            raise ValueError(f'Unknown unary operator for vector/matrix, cannot safely continue: \n\t{glsl_operand1.compose()}')


    def get_js_unary_operator_expression(glsl_operator, scope):
        operand1 = glsl_operator.operand1
        operator = glsl_operator.operator
        type1 = pypeg2glsl.get_expression_type(operand1, scope)
        if type1 in pypeg2glsl.vector_types or type1 in pypeg2glsl.matrix_types:
            return get_js_unary_glm_operator_expression(operand1, operator, scope)
        else:
            return get_js_default_operator_expression(glsl_operator, scope)

    return get_js_unary_operator_expression


def get_js_binary_operator_expression_getter(JsElement):

    get_js_default_operator_expression = get_js_default_element_getter(JsElement)
    def get_js_binary_glm_operator_expression(glsl_operand1, glsl_operand2, operator, scope):
        if isinstance(glsl_operand1, pypeg2glsl.PostfixExpression):
            return pypeg2js.PostfixExpression([
                    *get_js(glsl_operand1.content, scope), 
                    pypeg2js.BracketedExpression(pypeg2js.PostfixExpression([f"'{operator}'"])), 
                    pypeg2js.InvocationExpression(get_js(glsl_operand2, scope))
                ])
        else:
            return pypeg2js.PostfixExpression([
                    pypeg2js.ParensExpression([get_js(glsl_operand1, scope)]), 
                    pypeg2js.BracketedExpression(pypeg2js.PostfixExpression([f"'{operator}'"])), 
                    pypeg2js.InvocationExpression(get_js(glsl_operand2, scope))
                ])
    def get_js_binary_operator_expression(glsl_operator, scope):
        operand1 = glsl_operator.operand1
        operand2 = glsl_operator.operand2
        operator = glsl_operator.operator
        type1 = pypeg2glsl.get_expression_type(operand1, scope)
        type2 = pypeg2glsl.get_expression_type(operand2, scope)
        if type1 in pypeg2glsl.vector_types:
            return get_js_binary_glm_operator_expression(operand1, operand2, operator, scope)
        elif type1 in pypeg2glsl.matrix_types:
            return get_js_binary_glm_operator_expression(operand1, operand2, operator, scope)
        elif type2 in pypeg2glsl.vector_types:
            return get_js_binary_glm_operator_expression(operand2, operand1, operator, scope)
        elif type2 in pypeg2glsl.matrix_types:
            return get_js_binary_glm_operator_expression(operand2, operand1, operator, scope)
        else:
            return get_js_default_operator_expression(glsl_operator, scope)

    return get_js_binary_operator_expression


def get_js_variable_declaration(glsl, scope):
    js = pypeg2js.VariableDeclaration()
    js.qualifiers = ['const' if 'const' in glsl.qualifiers else 'let']
    js.names = glsl.names
    js.value = get_js(glsl.value, scope)
    return js


def get_js_structure_declaration(glsl_structure, scope):
    js_function = pypeg2js.FunctionDeclaration(glsl_structure.name)

    js_object = pypeg2js.AssociativeListExpression()
    js_object.content = []

    for glsl_declaration in glsl_structure.content:
        js_function.parameters.append(pypeg2js.ParameterDeclaration(glsl_declaration.name))
        js_object.content.append(pypeg2js.AttributeDeclaration(
            glsl_declaration.name, 
            pypeg2js.PostfixExpression([glsl_declaration.name])
        ))

    js_function.content.append(
        pypeg2js.ReturnStatement(
            pypeg2js.PostfixExpression([js_object])
        )
    )

    return js_function


def get_js_function_declaration(glsl_function, scope):
    glsl_type_str = pypeg2.compose(glsl_function.type, pypeg2glsl.PostfixExpression)
    js_function = pypeg2js.FunctionDeclaration(glsl_function.name, type_=f'/*{glsl_type_str}*/')
    js_function.documentation = glsl_function.documentation
    local_scope = scope.get_subscope(glsl_function)
    for glsl_parameter in glsl_function.parameters:
        glsl_type_str = pypeg2.compose(glsl_parameter.type, pypeg2glsl.PostfixExpression)
        js_function.parameters.append(f'/*{glsl_type_str}*/')
        js_function.parameters.append(pypeg2js.ParameterDeclaration(glsl_parameter.name))
    for glsl_element in glsl_function.content:
        js_function.content.append(get_js(glsl_element, local_scope))
    return js_function


glsl_js_getter_map = [
    (str,        lambda glsl, scope: glsl),
    (int,        lambda glsl, scope: glsl),
    (type(None), lambda glsl, scope: glsl),

    (pypeg2glsl.PostfixExpression,         get_js_postfix_expression), 
    (pypeg2glsl.PostIncrementExpression,   get_js_unary_operator_expression_getter(pypeg2js.PostIncrementExpression)),
    (pypeg2glsl.PreIncrementExpression,    get_js_unary_operator_expression_getter(pypeg2js.PreIncrementExpression)),

    (pypeg2glsl.MultiplicativeExpression,  get_js_binary_operator_expression_getter(pypeg2js.MultiplicativeExpression)),
    (pypeg2glsl.AdditiveExpression,        get_js_binary_operator_expression_getter(pypeg2js.AdditiveExpression)),
    (pypeg2glsl.ShiftExpression,           get_js_binary_operator_expression_getter(pypeg2js.ShiftExpression)),
    (pypeg2glsl.RelationalExpression,      get_js_binary_operator_expression_getter(pypeg2js.RelationalExpression)),
    (pypeg2glsl.EqualityExpression,        get_js_binary_operator_expression_getter(pypeg2js.EqualityExpression)),
    (pypeg2glsl.BitwiseAndExpression,      get_js_binary_operator_expression_getter(pypeg2js.BitwiseAndExpression)),
    (pypeg2glsl.BitwiseXorExpression,      get_js_binary_operator_expression_getter(pypeg2js.BitwiseXorExpression)),
    (pypeg2glsl.BitwiseOrExpression,       get_js_binary_operator_expression_getter(pypeg2js.BitwiseOrExpression)),
    (pypeg2glsl.LogicalAndExpression,      get_js_binary_operator_expression_getter(pypeg2js.LogicalAndExpression)),
    (pypeg2glsl.LogicalXorExpression,      get_js_binary_operator_expression_getter(pypeg2js.LogicalXorExpression)),
    (pypeg2glsl.LogicalOrExpression,       get_js_binary_operator_expression_getter(pypeg2js.LogicalOrExpression)),

    (pypeg2glsl.TernaryExpression,         get_js_default_element_getter(pypeg2js.TernaryExpression)),

    (pypeg2glsl.BracketedExpression,       get_js_default_element_getter(pypeg2js.BracketedExpression)),
    (pypeg2glsl.ParensExpression,          get_js_default_element_getter(pypeg2js.ParensExpression)),
    (pypeg2glsl.InvocationExpression,      get_js_default_element_getter(pypeg2js.InvocationExpression)),
    (pypeg2glsl.AssignmentExpression,      get_js_default_element_getter(pypeg2js.AssignmentExpression)),

    (pypeg2glsl.VariableDeclaration,       get_js_variable_declaration),
    (pypeg2glsl.ReturnStatement,           get_js_default_element_getter(pypeg2js.ReturnStatement)),
    (pypeg2glsl.IfStatement,               get_js_default_element_getter(pypeg2js.IfStatement)),
    (pypeg2glsl.WhileStatement,            get_js_default_element_getter(pypeg2js.WhileStatement)),
    (pypeg2glsl.DoWhileStatement,          get_js_default_element_getter(pypeg2js.DoWhileStatement)),
    (pypeg2glsl.ForStatement,              get_js_default_element_getter(pypeg2js.ForStatement)),
    (pypeg2glsl.ParameterDeclaration,      get_js_default_element_getter(pypeg2js.ParameterDeclaration)),
    (pypeg2glsl.StructureDeclaration,      get_js_structure_declaration),
    (pypeg2glsl.FunctionDeclaration,       get_js_function_declaration),

    (pypeg2.List, lambda glsl, scope: [get_js(element, scope) for element in glsl]),
    (list,        lambda glsl, scope: [get_js(element, scope) for element in glsl]),
    (tuple,       lambda glsl, scope: tuple(get_js(element, scope) for element in glsl)),
]

def get_js(glsl, scope):
    for Glsl, _get_js in glsl_js_getter_map:
        if isinstance(glsl, Glsl):
            return _get_js(glsl, scope)
    raise ValueError(f'support for {type(glsl)} not implemented, cannot safely continue')


"""
The command line interface for this script is meant to resemble sed.
You can select a file using the `-f` option.
By default, the script will print out the results of a "dry run". 
You can modify the file in-place using the `-i` option. 
If you want to replace all files in a directory, call like so:
 find . -name *.glsl.c \
     -exec echo {} \; -exec python3 ./glsl2js.py -if {} \;
"""

def convert_file(input_filename=False, in_place=False, verbose=False):
    def colorize_diff(diff):
        '''
        "colorize_diff" colorizes text output from the difflib library
        for display in the command line
        All credit goes to:
        https://chezsoi.org/lucas/blog/colored-diff-output-with-python.html
        '''
        for line in diff:
            if line.startswith('+'):
                yield Fore.GREEN + line + Fore.RESET
            elif line.startswith('-'):
                yield Fore.RED + line + Fore.RESET
            elif line.startswith('^'):
                yield Fore.BLUE + line + Fore.RESET
            else:
                yield line

    input_text = ''
    if input_filename:
        with open(input_filename, 'r+') as input_file:
            input_text = input_file.read()
    else:
        for line in sys.stdin:
            input_text += line

    glsl = pypeg2.parse(input_text, pypeg2glsl.glsl)
    js = get_js(glsl, pypeg2glsl.LexicalScope(glsl))
    output_text = pypeg2.compose(js, pypeg2js.javascript, autoblank = False)

    if verbose:
        diff = difflib.ndiff(
            input_text.splitlines(keepends=True), 
            output_text.splitlines(keepends=True)
        )
        for line in colorize_diff(diff):
            print(line)
            
    if in_place:
        input_file.seek(0)
        input_file.write(output_text)
        input_file.truncate()
    else:
        print(output_text)

if __name__ == '__main__':
    import argparse

    assert sys.version_info[0] >= 3, "Script must be run with Python 3 or higher"

    parser = argparse.ArgumentParser()
    parser.add_argument('-f', '--filename', dest='filename', 
        help='read input from FILE', metavar='FILE')
    parser.add_argument('-i', '--in-place', dest='in_place', 
        help='edit the file in-place', action='store_true')
    # parser.add_argument('-v', '--verbose', dest='verbose', 
    #     help='show debug information', action='store_true')
    args = parser.parse_args()
    convert_file(
        args.filename, 
        in_place=args.in_place, 
        verbose=False, 
    )
