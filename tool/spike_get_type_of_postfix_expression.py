import re
import pdb


import pypeg2
import pypeg2glsl
import pypeg2js


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
def get_type_of_glsl_postfix_expression(
        glsl_content, 
        lookups={'variable_types':{}, 'function_types':{}, 'attribute_types':{}}
    ):
    if len(glsl_content) < 1:
        return []

    overloaded_function_types = [
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
        'distance': ['float'],
    }
    elements = [*glsl_content]
    previous = elements.pop(0)
    previous_type = []
    # variable reference
    if isinstance(previous, str) and previous in lookups['variable_types']:
        previous_type = lookups['variable_types'][previous]
    if isinstance(previous, pypeg2glsl.ParensExpression):
        previous_type = get_type_of_glsl_expression(previous.content, lookups)
    while len(elements) > 0:
        current = elements.pop(0)
        if isinstance(current, pypeg2glsl.InvocationExpression):
            # matrix constructor
            if [previous] in matrix_types:
                previous_type = [previous]
            # vector constructor
            elif [previous] in vector_types: 
                previous_type = [previous]
            # structure constructor
            elif previous in lookups['attribute_types']:
                previous_type = [previous]
            # function invocation (built-in)
            elif previous in built_in_function_types:
                previous_type = built_in_function_types[previous]
            # function invocation (built-in, overloaded)
            elif previous in overloaded_function_types:
                param_types = [get_type_of_glsl_expression(param, lookups) for param in current.content]
                vector_param_types = [param_type for param_type in param_types if param_type in vector_types]
                previous_type = vector_param_types[0] if len(vector_param_types) > 0 else param_types[0]
            # function invocation (user-defined)
            elif previous in lookups['function_types']:
                previous_type = lookups['function_types'][previous]
            else:
                print(f'could not invoke unknown function: {previous}(...)')
        if isinstance(current, pypeg2glsl.BracketedExpression):
            # matrix column access
            if previous_type in matrix_types:
                previous_type = [re.sub('mat(\d)x?', 'vec\\1', previous_type[0])]
            # vector component access
            elif previous_type in vector_types:
                previous_type = ['float']
            # array index access
            elif (len(previous_type) > 1 and 
                isinstance(previous_type[1], pypeg2glsl.BracketedExpression)):
                previous_type = previous_type[:-1]
            else:
                print(f'could not access index of non-array: {previous_type}')
        if isinstance(current, str):
            # vector component access
            if previous_type in vector_types: # likely an attribute of a built-in structure, like a vector
                previous_type = ['float']
            # attribute access
            elif (len(previous_type) == 1 and 
                previous_type[0] in lookups['attribute_types'] and
                current in lookups['attribute_types'][previous_type[0]]):
                previous_type = lookups['attribute_types'][previous_type[0]][current]
            else:
                print(f'could not find attribute within unknown structure: {previous_type}')
        previous = current
    return previous_type 

def get_type_of_glsl_expression(
        glsl_expression, 
        lookups={'variable_types':{}, 'function_types':{}, 'attribute_types':{}}
    ):
    if isinstance(glsl_expression, pypeg2glsl.PostfixExpression):
        print('expression:', glsl_expression.content)
        return get_type_of_glsl_postfix_expression(glsl_expression.content, lookups)
    if isinstance(glsl_expression, pypeg2glsl.UnaryExpression):
        return get_type_of_glsl_expression(glsl_expression.operand1, lookups)
    if isinstance(glsl_expression, pypeg2glsl.BinaryExpression):
        type1 = get_type_of_glsl_expression(glsl_expression.operand1, lookups)
        type2 = get_type_of_glsl_expression(glsl_expression.operand2, lookups)
        vector_type = type1 if type1 in vector_types else type2 if type2 in vector_types else []
        matrix_type = type1 if type1 in matrix_types else type2 if type2 in matrix_types else []
        if vector_type:
            return vector_type
        elif matrix_type:
            return matrix_type
        else:
            if type1 != type2:
                print(f'types do not match within binary expression of operator: {glsl_expression.operator}')
            return type1
    if isinstance(glsl_expression, pypeg2glsl.TernaryExpression):
        type1 = get_type_of_glsl_expression(glsl_expression.operand2, lookups)
        type2 = get_type_of_glsl_expression(glsl_expression.operand3, lookups)
        if type1 != type2:
            glsl_expression_str = pypeg2.compose(glsl_expression, pypeg2glsl.TernaryExpression)
            print(f'types do not match within ternary expression: {glsl_expression_str}')
        return type1

def get_js_default_element_getter(JsElement):
    def get_js_default_element(glsl, lookups={}):
        js = JsElement()
        # for attribute in glsl.__dict__:
            # setattr(js, attribute, get_js(glsl.__dict__[attribute]))
        return js
    return get_js_default_element
get_js_default_operator_expression = get_js_default_element_getter(pypeg2glsl.MultiplicativeExpression)
def get_js_vector_operator_expression(glsl_operand1, glsl_operand2, operator):
    return pypeg2js.PostfixExpression([
            *glsl_operand1.content, 
            pypeg2js.BracketedExpression(pypeg2js.PostfixExpression([f"'{operator}'"])), 
            pypeg2js.InvocationExpression(pypeg2js.PostfixExpression(glsl_operand2.content))
        ])
def get_js_binary_operator_expression(
        glsl_operator, 
        lookups={'variable_types':{}, 'function_types':{}, 'attribute_types':{}}
    ):
    operand1 = glsl_operator.operand1
    operand2 = glsl_operator.operand2
    operator = glsl_operator.operator
    type1 = get_type_of_glsl_expression(operand1, lookups)
    type2 = get_type_of_glsl_expression(operand2, lookups)
    print(glsl_operator.operand1.content, type1)
    print(glsl_operator.operand2.content, type2)
    if type1 in glsl_vector_types:
        return get_js_vector_operator_expression(operand1, operand2, operator)
    elif type1 in glsl_matrix_types:
        return get_js_vector_operator_expression(operand1, operand2, operator)
    elif type2 in glsl_vector_types:
        return get_js_vector_operator_expression(operand2, operand1, operator)
    elif type2 in glsl_matrix_types:
        return get_js_vector_operator_expression(operand2, operand1, operator)
    else:
        return get_js_default_operator_expression(glsl_operator, lookups)

test_lookup = {
    'variable_types':{
        'A':['mat3'], 
        'a':['float'], 
        'v':['vec3'], 
        'vs':pypeg2.parse('vec3[5]', pypeg2glsl.PostfixExpression).content,
        'foo':['Foo'],
    },
    'function_types':{
        'F':['mat3'], 
        'f':['float'], 
        'g':['vec3'], 
        'h':pypeg2.parse('vec3[5]', pypeg2glsl.PostfixExpression).content,
        'i':['Foo']
    }, 
    'attribute_types':{
        'Foo': {
            'B':['mat3'], 
            'b':['float'], 
            'u':['vec3'], 
            'us':pypeg2.parse('vec3[5]', pypeg2glsl.PostfixExpression).content,
            'child':['Foo'],
        }
    }
}

def test_postfix(test):
    return get_type_of_glsl_postfix_expression( pypeg2.parse(test, pypeg2glsl.PostfixExpression).content, test_lookup)

def test_generic(test):
    return get_type_of_glsl_expression( pypeg2.parse(test, pypeg2glsl.ternary_expression_or_less), test_lookup)

def test_conversion(test):
    js = get_js_binary_operator_expression( pypeg2.parse(test, pypeg2glsl.MultiplicativeExpression), test_lookup)
    print(type(js), js.content)
    print(js.content[1].content.content)
    return pypeg2.compose(js, type(js))
'''
print(test_postfix('A'))
print(test_postfix('a'))
print(test_postfix('v'))
print(test_postfix('vs'))
print(test_postfix('foo'))
print(test_postfix('F'))
print(test_postfix('f'))
print(test_postfix('g'))
print(test_postfix('h'))
print(test_postfix('i'))
print(test_postfix('vec3'))
print(test_postfix('mat3'))
print(test_postfix('vec3()'))
print(test_postfix('mat3()'))
print(test_postfix('mat3()[0]'))
print(test_postfix('foo.B'))
print(test_postfix('foo.b'))
print(test_postfix('foo.u'))
print(test_postfix('foo.us'))
print(test_postfix('foo.child'))
print(test_postfix('foo.child.B'))
print(test_postfix('foo.child.B[0]'))
print(test_postfix('foo.child.u'))
print(test_postfix('v.x'))
print(test_postfix('v[0]'))
print(test_postfix('vs[0]'))
print(test_postfix('F()'))
print(test_postfix('F()[0]'))
print(test_postfix('f()'))
print(test_postfix('g()'))
print(test_postfix('g().x'))
print(test_postfix('g()[0]'))
print(test_postfix('Foo().child.B[0]'))
print(test_generic('mat3()[0]'))
print(test_generic('(A*a)[0]'))
print(test_generic('-A[0]'))
'''
print(test_conversion('A*v'))
