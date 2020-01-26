#!/bin/env python3

import copy

import pypeg2
import pypeg2glsl
import pypeg2js

class LexicalScope:
    """ 
    A "LexicalScope" is a conceptually immutable data structure containing 
    dictionaries to access type information for
    variables, functions return values, and data structure attributes
    within a given lexical scope.
    """

    @staticmethod
    def get_local_variable_type_lookups(glsl):
        result = { }
        code_block_element_types = [
            pypeg2glsl.IfStatement,
            pypeg2glsl.DoWhileStatement,
            pypeg2glsl.WhileStatement,
            pypeg2glsl.ForStatement,
        ]
        for element in glsl:
            if isinstance(element, pypeg2glsl.VariableDeclaration):
                result[element.name] = element.type.content
            elif isinstance(element, pypeg2glsl.ParameterDeclaration):
                result[element.name] = element.type.content
            elif type(element) in code_block_element_types:
                result.update(LexicalScope.get_local_variable_type_lookups(element.content))
        return result

    @staticmethod
    def get_global_variable_type_lookups(glsl):
        result = {}
        for element in glsl:
            if isinstance(element, pypeg2glsl.VariableDeclaration):
                result[element.name] = element.type.content
        return result

    @staticmethod
    def get_attribute_type_lookups(glsl):
        result = {}
        for element in glsl:
            if isinstance(element, pypeg2glsl.StructureDeclaration):
                attribute_types = {}
                for declaration in element.content:
                    if isinstance(declaration, pypeg2glsl.VariableDeclaration):
                        attribute_types[declaration.name] = declaration.type.content
                result[element.name] = attribute_types
        return result

    @staticmethod
    def get_function_type_lookups(glsl):
        result = {}
        for element in glsl:
            if isinstance(element, pypeg2glsl.FunctionDeclaration):
                result[element.name] = element.type.content
        return result

    def __init__(self, glsl = []):
        self.variables  = LexicalScope.get_global_variable_type_lookups(glsl)
        self.functions  = LexicalScope.get_function_type_lookups(glsl)
        self.attributes = LexicalScope.get_attribute_type_lookups(glsl)
        
    def get_subscope(self, glsl_function):
        """
        returns a new LexicalScope object whose state reflects the type 
        information of variables defined within a local subscope
        """
        result = LexicalScope()
        result.attributes = copy.deepcopy(self.attributes)
        result.functions = copy.deepcopy(self.functions)
        result.variables = {
            **self.variables,
            **LexicalScope.get_local_variable_type_lookups(glsl_function.parameters),
            **LexicalScope.get_local_variable_type_lookups(glsl_function.content)
        }
        return result
        
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


glsl_vector_types = [
    ['vec2'], ['vec3'], ['vec4'], 
    ['ivec2'], ['ivec3'], ['ivec4'],
    ['bvec2'], ['bvec3'], ['bvec4'],
]
glsl_matrix_types = [
    ['mat2'],  ['mat3'],  ['mat4'],    ['mat2x3'],  ['mat2x4'],    ['mat3x2'],    ['mat3x4'],   ['mat4x2'],    ['mat4x3'], 
    ['imat2'], ['imat3'], ['imat4'],   ['imat2x3'], ['imat2x4'],   ['imat3x2'],   ['imat3x4'],  ['imat4x2'],   ['imat4x3'],
    ['bmat2'], ['bmat3'], ['bmat4'],   ['bmat2x3'], ['bmat2x4'],   ['bmat3x2'],   ['bmat3x4'],  ['bmat4x2'],   ['bmat4x3'],
]
glsl_built_in_types = [
    *glsl_vector_types,
    *glsl_matrix_types,
    ['float'], ['int'], ['bool']
]
def get_type_of_glsl_postfix_expression(glsl_content, scope):
    if len(glsl_content) < 1:
        return []

    glsl_built_in_overloaded_function_types = [
        'radians', 'degrees',
        'sin', 'cos', 'tan', 
        'asin', 'acos', 'atan', 'atan2', 
        'sqrt', 'cbrt', 'pow', 'exp', 'exp2' 'log', 'log2', 'inversesqrt',
        'trunc', 'floor', 'ceil', 'round','mod'
        'sqrt', 'fract', 
        'min', 'max', 'clamp',  'step', 'smoothstep',
        'abs', 'sign', 
        'length', 'distance', 'normalize', 
        'faceforward', 'reflect', 'refract',
        'equal', 'notEqual', 'lessThan', 'lessThanEqual', 'greaterThan', 'greaterThanEqual', 
        'any', 'all', 'not',
        'frexp', 'ldexp',
    ]
    built_in_function_types = {
        'cross': ['vec3'],
        'dot': ['float'],
        'length': ['float'],
        'distance': ['float'],
    }
    elements = [*glsl_content]
    previous = elements.pop(0)
    previous_type = []
    # parentheses
    if isinstance(previous, pypeg2glsl.ParensExpression):
        previous_type = get_type_of_glsl_expression(previous.content, scope)
    # variable reference
    if isinstance(previous, str):
        if previous in scope.variables:
            previous_type = scope.variables[previous]
        elif previous in scope.variables:
            previous_type = scope.variables[previous]
    while len(elements) > 0:
        current = elements.pop(0)
        if isinstance(current, pypeg2glsl.InvocationExpression):
            # constructor (built-in)
            if [previous] in glsl_built_in_types:
                previous_type = [previous]
            # constructor (user-defined)
            elif previous in scope.attributes:
                previous_type = [previous]
            # function invocation (built-in)
            elif previous in built_in_function_types:
                previous_type = built_in_function_types[previous]
            # function invocation (built-in, overloaded)
            elif previous in glsl_built_in_overloaded_function_types:
                param_types = [get_type_of_glsl_expression(param, scope) for param in current.content]
                vector_param_types = [param_type for param_type in param_types if param_type in glsl_vector_types]
                previous_type = vector_param_types[0] if len(vector_param_types) > 0 else param_types[0]
            # function invocation (user-defined)
            elif previous in scope.functions:
                previous_type = scope.functions[previous]
            else:
                print(f'could not invoke unknown function: {previous}(...)')
        if isinstance(current, pypeg2glsl.BracketedExpression):
            # matrix column access
            if previous_type in glsl_matrix_types:
                previous_type = [re.sub('mat(\d)x?', 'vec\\1', previous_type[0])]
            # vector component access
            elif previous_type in glsl_vector_types:
                previous_type = ['float']
            # array index access
            elif (len(previous_type) > 1 and 
                isinstance(previous_type[1], pypeg2glsl.BracketedExpression)):
                previous_type = previous_type[:-1]
            else:
                print(f'could not access index of non-array: {previous_type}')
        if isinstance(current, str):
            # vector component access
            if previous_type in glsl_vector_types: # likely an attribute of a built-in structure, like a vector
                previous_type = ['float']
            # attribute access
            elif (len(previous_type) == 1 and 
                previous_type[0] in scope.attributes and
                current in scope.attributes[previous_type[0]]):
                previous_type = scope.attributes[previous_type[0]][current]
            else:
                print(f'could not find attribute within unknown structure: {previous_type}')
        previous = current
    return previous_type 

def get_type_of_glsl_expression(glsl_expression, scope):
    if isinstance(glsl_expression, str):
        if pypeg2glsl.float_literal.match(glsl_expression):
            return ['float']
        if pypeg2glsl.int_literal.match(glsl_expression):
            return ['int']
        if pypeg2glsl.bool_literal.match(glsl_expression):
            return ['bool']
    if isinstance(glsl_expression, pypeg2glsl.PostfixExpression):
        return get_type_of_glsl_postfix_expression(glsl_expression.content, scope)
    if isinstance(glsl_expression, pypeg2glsl.UnaryExpression):
        return get_type_of_glsl_expression(glsl_expression.operand1, scope)
    if isinstance(glsl_expression, pypeg2glsl.BinaryExpression):
        type1 = get_type_of_glsl_expression(glsl_expression.operand1, scope)
        type2 = get_type_of_glsl_expression(glsl_expression.operand2, scope)
        vector_type = type1 if type1 in glsl_vector_types else type2 if type2 in glsl_vector_types else []
        matrix_type = type1 if type1 in glsl_matrix_types else type2 if type2 in glsl_matrix_types else []
        if vector_type:
            return vector_type
        elif matrix_type:
            return matrix_type
        else:
            if type1 != type2:
                glsl_expression_str = pypeg2.compose(glsl_expression, type(glsl_expression))
                print(f'type mismatch: operation "{glsl_expression.operator}" was fed left operand of type "{type1}" and right hand operand of type "{type2}": \n\t{glsl_expression_str} ')
                print(scope.variables)
            return type1
    if isinstance(glsl_expression, pypeg2glsl.TernaryExpression):
        type1 = get_type_of_glsl_expression(glsl_expression.operand2, scope)
        type2 = get_type_of_glsl_expression(glsl_expression.operand3, scope)
        if type1 != type2:
            glsl_expression_str = pypeg2.compose(glsl_expression, pypeg2glsl.TernaryExpression)
            print(f'type mismatch: ternary operation takes a left hand operand of type "{type1}" and right hand operand of type "{type2}: \n\t{glsl_expression_str}"')
            print(scope.variables)
        return type1

def get_js_binary_operator_expression_getter(JsElement):


    get_js_default_operator_expression = get_js_default_element_getter(JsElement)
    def get_js_vector_operator_expression(glsl_operand1, glsl_operand2, operator):
        if isinstance(glsl_operand1, pypeg2glsl.PostfixExpression):
            return pypeg2js.PostfixExpression([
                    *glsl_operand1.content, 
                    pypeg2js.BracketedExpression(pypeg2js.PostfixExpression([f"'{operator}'"])), 
                    pypeg2js.InvocationExpression(get_js(glsl_operand2))
                ])
        if isinstance(glsl_operand1, pypeg2glsl.PostfixExpression):
            return pypeg2js.PostfixExpression([
                    pypeg2js.ParensExpression([glsl_operand1]), 
                    pypeg2js.BracketedExpression(pypeg2js.PostfixExpression([f"'{operator}'"])), 
                    pypeg2js.InvocationExpression(get_js(glsl_operand2))
                ])
    def get_js_binary_operator_expression(glsl_operator, scope):
        operand1 = glsl_operator.operand1
        operand2 = glsl_operator.operand2
        operator = glsl_operator.operator
        type1 = get_type_of_glsl_expression(operand1, scope)
        type2 = get_type_of_glsl_expression(operand2, scope)
        if type1 in glsl_vector_types:
            return get_js_vector_operator_expression(operand1, operand2, operator)
        elif type1 in glsl_matrix_types:
            return get_js_vector_operator_expression(operand1, operand2, operator)
        elif type2 in glsl_vector_types:
            return get_js_vector_operator_expression(operand2, operand1, operator)
        elif type2 in glsl_matrix_types:
            return get_js_vector_operator_expression(operand2, operand1, operator)
        else:
            return get_js_default_operator_expression(glsl_operator, scope)

    return get_js_binary_operator_expression

def get_js_variable_declaration(glsl, scope):
    js = pypeg2js.VariableDeclaration()
    js.qualifiers = ['const' if 'const' in glsl.qualifiers else 'let']
    js.name = glsl.name
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
    (pypeg2glsl.PostIncrementExpression,   get_js_default_element_getter(pypeg2js.PostIncrementExpression)),
    (pypeg2glsl.PreIncrementExpression,    get_js_default_element_getter(pypeg2js.PreIncrementExpression)),

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

def get_js(glsl, scope=None):
    scope = scope or LexicalScope(glsl if isinstance(glsl, list) else []) 
    for Glsl, _get_js in glsl_js_getter_map:
        if isinstance(glsl, Glsl):
            return _get_js(glsl, scope)
    raise Exception(f'{type(glsl)} not found')


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
    convert_file(args.filename, args.in_place, args.verbose)
