#!/bin/env python3

"""
The command line interface for this script is meant to resemble sed.
You can select a file using the `-f` argument.
By default, the script will print out the results of a "dry run". 
You can modify the file in-place using the `-i` flag. 
You can print a diff between input and output using the `-v` flag.
For basic usage on a single file, call like so:

If you want to replace all files in a directory, call like so:
 find . -name *.glsl.c \
     -exec echo {} \; -exec python3 ./glsl_standardize.py -if {} \;
"""


import difflib
import copy
import sys

import pypeg2
import pypeg2glsl

# attempt to import colorama, for colored diff output
try:
    from colorama import Fore, Back, Style, init
    init()
except ImportError:  # fallback so that the imported classes always exist
    class ColorFallback():
        __getattr__ = lambda self, name: ''
    Fore = Back = Style = ColorFallback()


def throw_not_implemented_error(f, feature='expressions'):
    f_str = pypeg2.compose(f, type(f))
    raise NotImplementedError(f'support for derivatives involving {feature} such as "{f_str}" is not implemented')

def throw_value_error(f, feature='invalid expression'):
    f_str = pypeg2.compose(f, type(f))
    raise NotImplementedError(f'{feature}, cannot continue safely: \n\t{f_str}')

def get_1_for_type(type_):
    identity_map ={
        'vec2': pypeg2.parse('vec2(1.f)', pypeg2glsl.PostfixExpression),
        'vec3': pypeg2.parse('vec3(1.f)', pypeg2glsl.PostfixExpression),
        'vec4': pypeg2.parse('vec4(1.f)', pypeg2glsl.PostfixExpression),

        'float': '1.0f',
        'int': '1'
    }
    type_element = pypeg2glsl.PostfixExpression(type_)
    type_str = pypeg2.compose(type_element, pypeg2glsl.PostfixExpression)
    if type_str in identity_map:
        return identity_map[type_str]
    else:
        throw_not_implemented_error(type_element, 'additives identities for types')
    
def get_0_for_type(type_):
    identity_map ={
        'vec2': pypeg2.parse('vec2(0.f)', pypeg2glsl.PostfixExpression),
        'vec3': pypeg2.parse('vec3(0.f)', pypeg2glsl.PostfixExpression),
        'vec4': pypeg2.parse('vec4(0.f)', pypeg2glsl.PostfixExpression),

        'float': '0.0f',
        'int': '0'
    }
    type_element = pypeg2glsl.PostfixExpression(type_)
    type_str = pypeg2.compose(type_element, pypeg2glsl.PostfixExpression)
    if type_str in identity_map:
        return identity_map[type_str]
    else:
        throw_not_implemented_error(type_element, 'multiplicative identities for types')

def get_wrapped_expression_if_ambigous(
        expression, 
        unambiguous_expression_types = pypeg2glsl.unary_expression_or_less
    ):
    return ( 
        expression 
        if type(expression) in unambiguous_expression_types 
        or isinstance(expression, str)
        else pypeg2glsl.ParensExpression(expression)
    )

def get_ddx_max(f, x, scope):
    f_params = f.content[1].content
    additive_expression_or_less = [
        pypeg2glsl.AdditiveExpression,
        *pypeg2glsl.unary_expression_or_less
    ]
    expressions = [
        get_wrapped_expression_if_ambigous(
            f_params[0], 
            additive_expression_or_less
        ),
        get_wrapped_expression_if_ambigous(
            get_ddx_expression(f_params[0], x, scope),
            pypeg2glsl.binary_expression_or_less,
        ),
        get_wrapped_expression_if_ambigous(
            f_params[1], 
            additive_expression_or_less
        ),
        get_wrapped_expression_if_ambigous(
            get_ddx_expression(f_params[1], x, scope),
            pypeg2glsl.binary_expression_or_less,
        )
    ]
    expression_strings = [
        pypeg2.compose(expression, type(expression))
        for expression in expressions
    ]
    u, dudx, v, dvdx = expression_strings
    u_type = pypeg2glsl.get_expression_type(f_params[0], scope)
    v_type = pypeg2glsl.get_expression_type(f_params[1], scope)
    if (u_type == ['float'] and v_type == ['float']):
        return pypeg2.parse(f''' 
            {u} > {v} ? {dudx} : {dvdx}
        ''', pypeg2glsl.TernaryExpression)
    else:
        throw_not_implemented_error('component-wise max')

def get_ddx_min(f, x, scope):
    f_params = f.content[1].content
    additive_expression_or_less = [
        pypeg2glsl.AdditiveExpression,
        *pypeg2glsl.unary_expression_or_less
    ]
    expressions = [
        get_wrapped_expression_if_ambigous(
            f_params[0], 
            additive_expression_or_less
        ),
        get_wrapped_expression_if_ambigous(
            get_ddx_expression(f_params[0], x, scope),
            pypeg2glsl.binary_expression_or_less,
        ),
        get_wrapped_expression_if_ambigous(
            f_params[1], 
            additive_expression_or_less
        ),
        get_wrapped_expression_if_ambigous(
            get_ddx_expression(f_params[1], x, scope),
            pypeg2glsl.binary_expression_or_less,
        )
    ]
    expression_strings = [
        pypeg2.compose(expression, type(expression))
        for expression in expressions
    ]
    u, dudx, v, dvdx = expression_strings
    u_type = pypeg2glsl.get_expression_type(f_params[0], scope)
    v_type = pypeg2glsl.get_expression_type(f_params[1], scope)
    if (u_type == ['float'] and v_type == ['float']):
        return pypeg2.parse(f''' 
            {u} < {v} ? {dudx} : {dvdx}
        ''', pypeg2glsl.TernaryExpression)
    else:
        throw_not_implemented_error('component-wise min')
    
def get_ddx_abs(f, x, scope):
    f_params = f.content[1].content
    additive_expression_or_less = [
        pypeg2glsl.AdditiveExpression,
        *pypeg2glsl.unary_expression_or_less
    ]
    expressions = [
        get_wrapped_expression_if_ambigous(
            f_params[0], 
            additive_expression_or_less
        ),
        get_wrapped_expression_if_ambigous(
            get_ddx_expression(f_params[0], x, scope),
            pypeg2glsl.binary_expression_or_less,
        )
    ]
    u, dudx = expression_strings
    u_type = pypeg2glsl.get_expression_type(f_params[0], scope)
    if (u_type == ['float']):
        return pypeg2.parse(f''' 
            {u} > 0.0f ? {dudx} : -{dudx}
        ''', pypeg2glsl.TernaryExpression)
    else:
        throw_not_implemented_error('component-wise abs')

def get_ddx_sqrt(f, x, scope):
    f_params = f.content[1].content
    return pypeg2glsl.MultiplicativeExpression(
        get_wrapped_expression_if_ambigous(
            get_ddx_expression(f_params[0], x, scope)
        ),
        '/',
        get_wrapped_expression_if_ambigous(
            pypeg2glsl.MultiplicativeExpression('2.0f', '*', f)
        ),
    )


def get_ddx_dot(f, x, scope):
    if len(f.content) != 2:
        throw_value_error(f, 'dot product must have two parameters')
    u = f.content[1].content[0]
    v = f.content[1].content[1]
    f_type = pypeg2glsl.get_expression_type(f, scope)
    u_type = pypeg2glsl.get_expression_type(u, scope)
    v_type = pypeg2glsl.get_expression_type(v, scope)
    x_type = pypeg2glsl.get_expression_type(x, scope)

    expressions = [
        get_wrapped_expression_if_ambigous( u ),
        get_wrapped_expression_if_ambigous( get_ddx_expression(u, x, scope) ),
        get_wrapped_expression_if_ambigous( v ),
        get_wrapped_expression_if_ambigous( get_ddx_expression(v, x, scope) ),
        # pypeg2glsl.PostfixExpression(f_type),
        # pypeg2glsl.PostfixExpression(u_type),
        # pypeg2glsl.PostfixExpression(v_type),
        # pypeg2glsl.PostfixExpression(x_type),
    ]
    expression_strings = [
        pypeg2.compose(expression, type(expression))
        for expression in expressions
    ]
    if f_type == ['float'] and x_type == ['float']:
        # u, dudx, v, dvdx, f_type, u_type, v_type, x_type = expression_strings
        u, dudx, v, dvdx = expression_strings
        return pypeg2.parse(
            f' dot({u}, {dvdx}) + dot({dudx}, {v}) ', 
            pypeg2glsl.AdditiveExpression
        )
    # elif f_type == ['float'] and x_type in pypeg2glsl.vector_types:
        # u, dudx, v, dvdx, f_type, u_type, v_type, x_type = expression_strings
        # return pypeg2.parse(
        #     f''' 
        #     {x_type}(
        #       dot({u}, {dvdx}) + dot({dudx}, {v}),
        #       dot({u}, {dvdx}) + dot({dudx}, {v}),
        #       dot({u}, {dvdx}) + dot({dudx}, {v})
        #     )
        #     ''', 
        #     pypeg2glsl.AdditiveExpression
        # )
    else:
        throw_not_implemented_error(f, 'jacobians')

def get_ddx_invocation(f, x, scope):
    name = f.content[0]
    params = f.content[1].content
    dfdu_getter_map = {
        'abs' : get_ddx_abs,
        'min' : get_ddx_min,
        'max' : get_ddx_max,
        'sqrt': get_ddx_sqrt,
        'dot' : get_ddx_dot,
    }
    dfdu_name_map = {
        'length': 'normalize',
        'sin': 'cos',
    }
    # constructor (built-in)
    if [name] in pypeg2glsl.built_in_types:
        throw_not_implemented_error(f, 'constructors')
    # constructor (user-defined)
    elif name in scope.attributes:
        throw_not_implemented_error(f, 'custom data structures')
    # function invocation (built-in)
    elif name in dfdu_getter_map:
        return dfdu_getter_map[name](f, x, scope)
    # function invocation (user-defined)
    elif len(params) > 1:
        throw_not_implemented_error(f, 'multi-parameter functions')
    elif len(params) < 1:
        return get_0_for_type(pypeg2glsl.get_expression_type(f, scope))
    else:
        dfdu = dfdu_name_map[name] if name in dfdu_name_map else f'dd{x}_{name}'
        dudx = get_wrapped_expression_if_ambigous(
            get_ddx_expression(params[0], x, scope)
        )
        return pypeg2glsl.MultiplicativeExpression(
            pypeg2glsl.PostfixExpression([
                dfdu, 
                pypeg2glsl.InvocationExpression(params)
            ]), 
            '*', 
            dudx
        )

def get_ddx_postfix_expression(f, x, scope):
            
    dfdx = pypeg2glsl.PostfixExpression()
    # here is where we have to start worrying about type
    # monitor the type of every differential
    f_type = pypeg2glsl.get_expression_type(f, scope)
    x_type = pypeg2glsl.get_expression_type(x, scope)

    elements = copy.deepcopy(f.content)
    current = elements.pop(0)
    current_type = []
    current_ddx = None
    # parentheses
    if isinstance(current, pypeg2glsl.ParensExpression):
        current_type = pypeg2glsl.get_expression_type(current, scope)
        current_ddx = get_ddx_parens_expression(current, x, scope)
    # function invocation
    elif len(elements) > 0 and isinstance(elements[0], pypeg2glsl.InvocationExpression):
        invocation = pypeg2glsl.PostfixExpression([current, elements.pop(0)])
        current_type = pypeg2glsl.get_expression_type(invocation, scope)
        current_ddx  = get_ddx_invocation(invocation, x, scope)
    # variable reference
    elif isinstance(current, str):
        current_type = pypeg2glsl.get_expression_type(
            pypeg2glsl.PostfixExpression([current]), 
            scope
        )
        if current == x:
            current_ddx = get_1_for_type(pypeg2glsl.get_expression_type(f, scope))
        else:
            current_ddx = pypeg2glsl.PostfixExpression([f'dd{x}_{current}'])
    else:
        throw_not_implemented_error(f)

    previous = current
    previous_type = current_type
    previous_ddx = current_ddx
    while len(elements) > 0:
        current = elements.pop(0)
        if isinstance(current, pypeg2glsl.BracketedExpression):
            # matrix column access
            if previous_type in matrix_types:
                current_type = [re.sub('mat(\d)x?', 'vec\\1', previous_type[0])]
                throw_not_implemented_error(f, 'matrix column access')
            # vector component access
            elif previous_type in pypeg2glsl.vector_types:
                current_type = ['float']
                if (isinstance(current.content, str) and 
                    pypeg2glsl.int_literal.match(current.content)):
                    current_ddx = pypeg2glsl.PostfixExpression([current_ddx, current])
                else:
                    throw_not_implemented_error(f, 'vector component access')
            # array index access
            elif (len(previous_type) > 1 and 
                isinstance(previous_type[1], pypeg2glsl.BracketedExpression)):
                current_type = [previous_type[0]]
                throw_not_implemented_error(f, 'array index access')
            else:
                throw_not_implemented_error(f)
        elif isinstance(current, str):
            # vector component access
            if previous_type in pypeg2glsl.vector_types: # likely an attribute of a built-in structure, like a vector
                current_type = ['float']
                current_ddx = pypeg2glsl.PostfixExpression([current_ddx, current])
            # attribute access
            elif (len(previous_type) == 1 and 
                previous_type[0] in scope.attributes and
                current in scope.attributes[previous_type[0]]):
                current_type = scope.attributes[previous_type[0]][current]
                throw_not_implemented_error(f, 'attribute access')
            else:
                throw_not_implemented_error(f)
        previous = current
        previous_type = current_type
        previous_ddx = current_ddx

    return previous_ddx 

def get_ddx_multiplicative_expression(f, x, scope):
    # programming at its finest, boys 😃
    # multiplication in glsl is always element wise, 
    # so we don't have to worry about managing types

    expressions = [
        #u
        f.operand1, 
        #dudx
        get_ddx_expression(f.operand1, x, scope), 
        #v
        f.operand2, 
        #dvdx
        get_ddx_expression(f.operand2, x, scope), 
    ]

    expressions_wrapped = [
        get_wrapped_expression_if_ambigous(expression) 
        for expression in expressions
    ]

    expression_strings = [ 
        pypeg2.compose(expression, type(expression)) 
        for expression in expressions_wrapped
    ]

    u, dudx, v, dvdx = expression_strings

    if f.operator == '*':
        dfdx = pypeg2.parse(f'{v}*{dudx} + {u}*{dvdx}', pypeg2glsl.AdditiveExpression)
    elif f.operator == '/':
        dfdx = pypeg2.parse(f'({v}*{dudx} - {u}*{dvdx})/({v}*{v})', pypeg2glsl.MultiplicativeExpression)
    else:
        throw_value_error(f, f'multiplicative expressions cannot have an operator of "{f.operator}"')

    return dfdx


def get_ddx_additive_expression(f, x, scope):
    # addition is always element-wise, 
    # so we don't have to worry about managing types

    multiplicative_expression_or_less = [
        pypeg2glsl.MultiplicativeExpression, 
        *pypeg2glsl.unary_expression_or_less
    ]
    return pypeg2glsl.AdditiveExpression(
        get_wrapped_expression_if_ambigous(
            get_ddx_expression(f.operand1, x, scope), 
            multiplicative_expression_or_less
        ),
        f.operator,
        get_wrapped_expression_if_ambigous(
            get_ddx_expression(f.operand2, x, scope), 
            multiplicative_expression_or_less
        ),
    )

def get_ddx_parens_expression(f, x, scope):
    return pypeg2glsl.ParensExpression(
        get_ddx_expression(f.content, x, scope)
    )

def get_ddx_literal(k, x, scope):
    k_type = pypeg2glsl.get_expression_type(k, scope)
    return get_0_for_type(k_type)

def get_ddx_expression(f, x, scope):
    ''' 
    "get_ddx_function" is a pure function that 
    transforms an pypeg2glsl grammar element matching pypeg2glsl.ternary_expression_or_less
    into a pypeg2glsl parse tree representing the derivative with respect to a given variable. 
    '''
    derivative_map = {
        (str, get_ddx_literal),

        (pypeg2glsl.PostfixExpression,         get_ddx_postfix_expression), 
        # (pypeg2glsl.PostIncrementExpression,   ),
        # (pypeg2glsl.PreIncrementExpression,    ),

        (pypeg2glsl.MultiplicativeExpression,  get_ddx_multiplicative_expression),
        (pypeg2glsl.AdditiveExpression,        get_ddx_additive_expression),
        # (pypeg2glsl.ShiftExpression,           ),
        # (pypeg2glsl.RelationalExpression,      ),
        # (pypeg2glsl.EqualityExpression,        ),
        # (pypeg2glsl.BitwiseAndExpression,      ),
        # (pypeg2glsl.BitwiseXorExpression,      ),
        # (pypeg2glsl.BitwiseOrExpression,       ),
        # (pypeg2glsl.LogicalAndExpression,      ),
        # (pypeg2glsl.LogicalXorExpression,      ),
        # (pypeg2glsl.LogicalOrExpression,       ),

        # (pypeg2glsl.TernaryExpression,         ),

        (pypeg2glsl.ParensExpression,          get_ddx_parens_expression),
    }
    for Rule, get_ddx_rule in derivative_map:
        if isinstance(f, Rule):
            dfdx = get_ddx_rule(f, x, scope)
            return dfdx
    raise throw_not_implemented_error(f, type(f).__name__)
    
def get_ddx_type(f_type, x_type, f=None):
    # scalar derivative
    if x_type == ['float'] and f_type == ['float']:
        return ['float']
    # gradient
    elif (f_type == ['float'] and x_type in pypeg2glsl.vector_types):
        return x_type
    # component-wise scalar derivative
    elif (f_type in pypeg2glsl.vector_types and x_type == ['float']):
        return f_type
    # either jacobian or component-wise vector derivative,
    # however jacobians have little use in shaders,
    # so we assume it is a component-wise derivative
    elif (f_type in pypeg2glsl.vector_types and x_type == f_type):
        return f_type

    f_type_str = pypeg2.compose(pypeg2glsl.PostfixExpression(f_type), pypeg2glsl.PostfixExpression)
    x_type_str = pypeg2.compose(pypeg2glsl.PostfixExpression(x_type), pypeg2glsl.PostfixExpression)
    raise throw_not_implemented_error(f, f'variables of type "{f_type_str}" and "{x_type_str}"')

def get_ddx_function(f, x, scope):
    ''' 
    "get_ddx_function" is a pure function that finds analytic derivatives.
    Given a pypeg2glsl parse tree representing a glsl function,
    as well as the name of a parameter to that function, 
    and an instance of pypeg2glsl.LexicalScope,
    it returns a pypeg2glsl parse tree representing the derivative 
    of that function with respect to the parameter. 
    The function works for only a small subset of derivatives 
    but it is intended to fail-loudly. 
    '''
    local_scope = scope.get_subscope(f)
    x_type = local_scope.variables[x]

    dfdx = pypeg2glsl.FunctionDeclaration(
        type_ = pypeg2glsl.PostfixExpression(
            get_ddx_type(f.type.content, x_type, f)
        ), 
        name  = f'dd{x}_{f.name}',
    )

    # return identity if x not in f
    x_param = [parameter for parameter in f.parameters if parameter.name == x]
    if len(x_param) < 1:
        dfdx.append(
            pypeg2glsl.ReturnStatement(get_0_for_type(pypeg2glsl.get_expression_type(f, scope)))
        )
        return dfdx

    # create parameters expressing derivatives of other parameters besides x
    dfdx.parameters = []
    for param in f.parameters:
        dfdx.parameters.append(copy.deepcopy(param))
        if param.name != x:
            dfdx.content.append(
                pypeg2glsl.VariableDeclaration(
                    type_ = copy.deepcopy(param.type),
                    names = [ f'dd{x}_{param.name}' ],
                    value = get_0_for_type(param.type.content),
                )
            )

    # convert the content of the function
    for statement in f.content:
        if isinstance(statement, pypeg2glsl.ReturnStatement):
            dfdx.content.append( 
                pypeg2glsl.ReturnStatement(get_ddx_expression(statement.value, x, local_scope))
            )
        elif isinstance(statement, pypeg2glsl.VariableDeclaration):
            value = get_ddx_expression(statement.value, x, local_scope)
            dfdx.content.append( copy.deepcopy(statement) )
            dfdx.content.append(
                pypeg2glsl.VariableDeclaration(
                    type_ = pypeg2glsl.PostfixExpression(get_ddx_type(statement.type.content, x_type, statement)),
                    names = [ f'dd{x}_{name}' for name in statement.names],
                    value = value,
                )
            )
        else:
            statement_str = pypeg2.compose(statement, type(statement))
            throw_not_implemented_error(statement, f'{type(statement)}')

    return dfdx

def convert_glsl(input_glsl):
    ''' 
    "convert_glsl" is a pure function that performs 
    a transformation on a parse tree of glsl as represented by pypeg2glsl,
    then returns a transformed parse tree as output. 
    '''
    output_glsl = [
        get_ddx_function(declaration, declaration.parameters[0].name, pypeg2glsl.LexicalScope(input_glsl)) 
        if isinstance(declaration, pypeg2glsl.FunctionDeclaration)
        else declaration
        for declaration in input_glsl
    ]
    return output_glsl

def convert_text(input_text):
    ''' 
    "convert_text" is a pure function that performs 
    a transformation on a string containing glsl code,
    then returns transformed output. 
    It may run convert_glsl behind the scenes, 
    and may also perform additional string based transformations,
    such as string substitutions or regex replacements
    '''
    input_glsl = pypeg2.parse(input_text, pypeg2glsl.glsl)
    output_glsl = convert_glsl(input_glsl)
    output_text = pypeg2.compose(output_glsl, pypeg2glsl.glsl, autoblank = False) 
    return output_text

def convert_file(input_filename=False, in_place=False, verbose=False):
    ''' 
    "convert_file" performs a transformation on a file containing glsl code
    It may either print out transformed contents or replace the file, 
    depending on the value of `in_place`
    '''
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

    output_text = convert_text(input_text)

    if verbose:
        diff = difflib.ndiff(
            input_text.splitlines(keepends=True), 
            output_text.splitlines(keepends=True)
        )
        for line in colorize_diff(diff):
            print(line)

    if in_place:
        with open(input_filename, 'w') as output_file:
            output_file.write(output_text)
            output_file.truncate()
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
