#!/bin/env python3

import difflib
import sys

import pypeg2 as peg
import pypeg2glsl as glsl
import pypeg2js as js

# attempt to import colorama, for colored diff output
try:
    from colorama import Fore, Back, Style, init
    init()
except ImportError:  # fallback so that the imported classes always exist
    class ColorFallback():
        __getattr__ = lambda self, name: ''
    Fore = Back = Style = ColorFallback()

def assert_type(variable, types):
    if not any([isinstance(variable, type_) for type_ in types]):
        raise AssertionError(f'expected {types} but got {variable}')

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
    def get_js_default_element(glsl_element, scope):
        js_element = JsElement()
        for attribute in glsl.element_attributes:
            if hasattr(glsl_element, attribute):
                setattr(js_element, attribute, get_js(glsl_element.__dict__[attribute], scope))
        return js_element
    return get_js_default_element

def get_js_invocation_expression(glsl_expression, scope):
    js_expression = js.PostfixExpression()
    js_invocation = js.InvocationExpression(
        [get_js(argument, scope) for argument in glsl_expression.arguments]
    )

    glsl_reference = glsl_expression.reference
    if glsl_reference in js_glm_library_functions:
        return js.PostfixExpression(['glm', glsl_reference, js_invocation])
    elif glsl_reference in js_math_library_functions:
        return js.PostfixExpression(['Math', glsl_reference, js_invocation])
    else: 
        return js.PostfixExpression([get_js(glsl_reference, scope), js_invocation])

def get_js_attribute_expression(glsl_expression, scope):
    if isinstance(glsl_expression.reference, glsl.InvocationExpression):
        js_expression = get_js(glsl_expression.reference, scope)
    else:
        js_expression = js.PostfixExpression([get_js(glsl_expression.reference, scope)])

    for attribute in glsl_expression.attributes:
        js_expression.content.append(get_js(attribute, scope))

    return js_expression

def get_js_unary_operator_expression_getter(JsElement):

    get_js_default_operator_expression = get_js_default_element_getter(JsElement)
    def get_js_unary_glm_operator_expression(glsl_operand1, operator, scope):
        type1 = scope.get_expression_type(glsl_operand1)
        assert operator != '!', 'Unary negation for vectors/matrices is not supported by glm-js, cannot safely continue'
        if operator == '++':
            return js.PostfixExpression([
                    js.ParensExpression([get_js(glsl_operand1, scope)]), 
                    peg.parse('["+="]', js.BracketedExpression),
                    js.InvocationExpression(js.PostfixExpression(['glm', *type1, js.InvocationExpression(['1'])]))
                ])
        elif operator == '--':
            return js.PostfixExpression([
                    js.ParensExpression([get_js(glsl_operand1, scope)]), 
                    peg.parse('["-="]', js.BracketedExpression),
                    js.InvocationExpression(js.PostfixExpression(['glm', *type1, js.InvocationExpression(['1'])]))
                ])
        elif operator == '-' and isinstance(glsl_operand1, glsl.PostfixExpression):
            return js.PostfixExpression([
                    *glsl_operand1.content, 
                    peg.parse('["*"]', js.BracketedExpression),
                    peg.parse('(-1)', js.InvocationExpression)
                ])
        elif operator == '-':
            return js.PostfixExpression([
                    js.ParensExpression([get_js(glsl_operand1, scope)]), 
                    peg.parse('["*"]', js.BracketedExpression),
                    peg.parse('(-1)', js.InvocationExpression)
                ])
        elif operator == '+':
            return js.ParensExpression([get_js(glsl_operand1, scope)])
        else:
            raise ValueError(f'Unknown unary operator for vector/matrix, cannot safely continue: \n\t{glsl_operand1.compose()}')


    def get_js_unary_operator_expression(glsl_operator, scope):
        operand1 = glsl_operator.operand1
        operator = glsl_operator.operator
        type1 = scope.get_expression_type(operand1)
        if type1 in glsl.vector_types or type1 in glsl.matrix_types:
            return get_js_unary_glm_operator_expression(operand1, operator, scope)
        else:
            return get_js_default_operator_expression(glsl_operator, scope)

    return get_js_unary_operator_expression


def get_js_binary_operator_expression_getter(JsElement):

    get_js_default_operator_expression = get_js_default_element_getter(JsElement)
    def get_js_binary_glm_operator_expression(glsl_operand1, glsl_operand2, operator, scope):
        if (isinstance(glsl_operand1, glsl.AttributeExpression) or
            isinstance(glsl_operand1, glsl.InvocationExpression)):
            return js.PostfixExpression([
                    *get_js(glsl_operand1, scope).content,
                    js.BracketedExpression(js.PostfixExpression([f"'{operator}'"])), 
                    js.InvocationExpression(get_js(glsl_operand2, scope))
                ])
        elif isinstance(glsl_operand1, str):
            return js.PostfixExpression([
                    glsl_operand1,
                    js.BracketedExpression(js.PostfixExpression([f"'{operator}'"])), 
                    js.InvocationExpression(get_js(glsl_operand2, scope))
                ])
        else:
            return js.PostfixExpression([
                    js.ParensExpression([get_js(glsl_operand1, scope)]), 
                    js.BracketedExpression(js.PostfixExpression([f"'{operator}'"])), 
                    js.InvocationExpression(get_js(glsl_operand2, scope))
                ])
    def get_js_binary_operator_expression(glsl_operator, scope):
        operand1 = glsl_operator.operand1
        operand2 = glsl_operator.operand2
        operator = glsl_operator.operator
        type1 = scope.get_expression_type(operand1)
        type2 = scope.get_expression_type(operand2)
        if type1 in glsl.vector_types:
            return get_js_binary_glm_operator_expression(operand1, operand2, operator, scope)
        elif type1 in glsl.matrix_types:
            return get_js_binary_glm_operator_expression(operand1, operand2, operator, scope)
        elif type2 in glsl.vector_types:
            return get_js_binary_glm_operator_expression(operand2, operand1, operator, scope)
        elif type2 in glsl.matrix_types:
            return get_js_binary_glm_operator_expression(operand2, operand1, operator, scope)
        else:
            return get_js_default_operator_expression(glsl_operator, scope)

    return get_js_binary_operator_expression


def get_js_variable_declaration(glsl_declaration, scope):
    js_declaration = js.VariableDeclaration()
    js_declaration.qualifiers = ['const' if 'const' in glsl_declaration.qualifiers else 'let']
    js_declaration.names = glsl_declaration.names
    js_declaration.value = get_js(glsl_declaration.value, scope)
    return js_declaration


def get_js_structure_declaration(glsl_structure, scope):
    js_function = js.FunctionDeclaration(glsl_structure.name)

    js_object = js.AssociativeListExpression()
    js_object.content = []

    for glsl_declaration in glsl_structure.content:
        js_function.parameters.append(js.ParameterDeclaration(glsl_declaration.name))
        js_object.content.append(js.AttributeDeclaration(
            glsl_declaration.name, 
            js.PostfixExpression([glsl_declaration.name])
        ))

    js_function.content.append(
        js.ReturnStatement(
            js.PostfixExpression([js_object])
        )
    )

    return js_function


def get_js_function_declaration(glsl_function, scope):
    glsl_type_str = peg.compose(glsl_function.type, type(glsl_function.type))
    js_function = js.FunctionDeclaration(glsl_function.name, type_=f'/*{glsl_type_str}*/')
    js_function.documentation = glsl_function.documentation
    local_scope = scope.get_subscope(glsl_function)
    for glsl_parameter in glsl_function.parameters:
        glsl_type_str = peg.compose(glsl_parameter.type, type(glsl_function.type))
        js_function.parameters.append(f'/*{glsl_type_str}*/')
        js_function.parameters.append(js.ParameterDeclaration(glsl_parameter.name))
    for glsl_element in glsl_function.content:
        js_function.content.append(get_js(glsl_element, local_scope))
    return js_function


glsl_js_getter_map = [
    (str,        lambda glsl_element, scope: glsl_element),
    (int,        lambda glsl_element, scope: glsl_element),
    (type(None), lambda glsl_element, scope: glsl_element),

    (glsl.InvocationExpression, get_js_invocation_expression), 
    (glsl.AttributeExpression,  get_js_attribute_expression), 
    (glsl.PostIncrementExpression,   get_js_unary_operator_expression_getter(js.PostIncrementExpression)),
    (glsl.PreIncrementExpression,    get_js_unary_operator_expression_getter(js.PreIncrementExpression)),

    (glsl.MultiplicativeExpression,  get_js_binary_operator_expression_getter(js.MultiplicativeExpression)),
    (glsl.AdditiveExpression,        get_js_binary_operator_expression_getter(js.AdditiveExpression)),
    (glsl.ShiftExpression,           get_js_binary_operator_expression_getter(js.ShiftExpression)),
    (glsl.RelationalExpression,      get_js_binary_operator_expression_getter(js.RelationalExpression)),
    (glsl.EqualityExpression,        get_js_binary_operator_expression_getter(js.EqualityExpression)),
    (glsl.BitwiseAndExpression,      get_js_binary_operator_expression_getter(js.BitwiseAndExpression)),
    (glsl.BitwiseXorExpression,      get_js_binary_operator_expression_getter(js.BitwiseXorExpression)),
    (glsl.BitwiseOrExpression,       get_js_binary_operator_expression_getter(js.BitwiseOrExpression)),
    (glsl.LogicalAndExpression,      get_js_binary_operator_expression_getter(js.LogicalAndExpression)),
    (glsl.LogicalXorExpression,      get_js_binary_operator_expression_getter(js.LogicalXorExpression)),
    (glsl.LogicalOrExpression,       get_js_binary_operator_expression_getter(js.LogicalOrExpression)),

    (glsl.TernaryExpression,         get_js_default_element_getter(js.TernaryExpression)),

    (glsl.BracketedExpression,       get_js_default_element_getter(js.BracketedExpression)),
    (glsl.ParensExpression,          get_js_default_element_getter(js.ParensExpression)),
    (glsl.AssignmentExpression,      get_js_default_element_getter(js.AssignmentExpression)),

    (glsl.VariableDeclaration,       get_js_variable_declaration),
    (glsl.ReturnStatement,           get_js_default_element_getter(js.ReturnStatement)),
    (glsl.IfStatement,               get_js_default_element_getter(js.IfStatement)),
    (glsl.WhileStatement,            get_js_default_element_getter(js.WhileStatement)),
    (glsl.DoWhileStatement,          get_js_default_element_getter(js.DoWhileStatement)),
    (glsl.ForStatement,              get_js_default_element_getter(js.ForStatement)),
    (glsl.ParameterDeclaration,      get_js_default_element_getter(js.ParameterDeclaration)),
    (glsl.StructureDeclaration,      get_js_structure_declaration),
    (glsl.FunctionDeclaration,       get_js_function_declaration),

    (peg.List, lambda glsl_element, scope: [get_js(element, scope) for element in glsl_element]),
    (list,        lambda glsl_element, scope: [get_js(element, scope) for element in glsl_element]),
    (tuple,       lambda glsl_element, scope: tuple(get_js(element, scope) for element in glsl_element)),
]

def get_js(glsl_element, scope):
    assert_type(glsl_element, [str, list, glsl.GlslElement])
    for Glsl, _get_js in glsl_js_getter_map:
        if isinstance(glsl_element, Glsl):
            js_element = _get_js(glsl_element, scope)
            assert_type(js_element, [str, list, js.JsElement])
            return js_element
    raise ValueError(f'support for {type(glsl_element)} not implemented, cannot safely continue')

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

    glsl_code = peg.parse(input_text, glsl.code)
    js_code = get_js(glsl_code, glsl.LexicalScope(glsl_code))
    js.warn_of_invalid_grammar_elements(js_code)
    output_text = peg.compose(js_code, js.code, autoblank = False)

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
    parser.add_argument('-v', '--verbose', dest='verbose', 
        help='show debug information', action='store_true')
    args = parser.parse_args()
    convert_file(
        args.filename, 
        in_place=args.in_place, 
        verbose=args.verbose, 
    )
