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

import pypeg2 as peg
import pypeg2glsl as glsl

# attempt to import colorama, for colored diff output
try:
    from colorama import Fore, Back, Style, init
    init()
except ImportError:  # fallback so that the imported classes always exist
    class ColorFallback():
        __getattr__ = lambda self, name: ''
    Fore = Back = Style = ColorFallback()


def throw_not_implemented_error(f, feature='expressions'):
    f_str = peg.compose(f, type(f))
    raise NotImplementedError(f'support for derivatives involving {feature} such as "{f_str}" is not implemented')

def throw_value_error(f, description='invalid expression'):
    f_str = peg.compose(f, type(f))
    raise NotImplementedError(f'{description}, cannot continue safely: \n\t{f_str}')

def get_1_for_type(type_):
    identity_map ={
        'vec2': peg.parse('vec2(1.f)', glsl.NewStyleInvocationExpression),
        'vec3': peg.parse('vec3(1.f)', glsl.NewStyleInvocationExpression),
        'vec4': peg.parse('vec4(1.f)', glsl.NewStyleInvocationExpression),

        'float': '1.0f',
        'int': '1'
    }
    if type_ in identity_map:
        return identity_map[type_]
    else:
        throw_not_implemented_error(type_element, 'additives identities for types')
    
def get_0_for_type(type_):
    identity_map ={
        'vec2': peg.parse('vec2(0.f)', glsl.NewStyleInvocationExpression),
        'vec3': peg.parse('vec3(0.f)', glsl.NewStyleInvocationExpression),
        'vec4': peg.parse('vec4(0.f)', glsl.NewStyleInvocationExpression),

        'float': '0.0f',
        'int': '0'
    }
    if type_ in identity_map:
        return identity_map[type_]
    else:
        throw_not_implemented_error(type_element, 'multiplicative identities for types')

def maybe_wrap(
        expression, 
        unambiguous_expression_types = glsl.unary_expression_or_less
    ):
    '''
    "maybe_wrap" wraps an expression in parentheses 
    and returns the ParensExpression if there is risk of it causing ambiguity,
    otherwise it returns the original unwrapped expression.
    '''
    return ( 
        expression 
        if type(expression) in unambiguous_expression_types 
        or isinstance(expression, str)
        else glsl.ParensExpression(expression)
    )

def get_ddx_max(f, x, scope):
    f_args = f.arguments
    additive_expression_or_less = [
        glsl.AdditiveExpression,
        *glsl.unary_expression_or_less
    ]
    expressions = [
        maybe_wrap(f_args[0], additive_expression_or_less),
        maybe_wrap(get_ddx(f_args[0], x, scope), glsl.binary_expression_or_less),
        maybe_wrap(f_args[1], additive_expression_or_less),
        maybe_wrap(get_ddx(f_args[1], x, scope), glsl.binary_expression_or_less)
    ]
    expression_strings = [
        peg.compose(expression, type(expression))
        for expression in expressions
    ]
    u, dudx, v, dvdx = expression_strings
    u_type = scope.get_expression_type(f_args[0])
    v_type = scope.get_expression_type(f_args[1])
    if (u_type == 'float' and v_type == 'float'):
        return peg.parse(f''' 
            {u} > {v} ? {dudx} : {dvdx}
        ''', glsl.TernaryExpression)
    else:
        throw_not_implemented_error(f, 'calls to component-wise max()')

def get_ddx_min(f, x, scope):
    f_args = f.arguments
    additive_expression_or_less = [
        glsl.AdditiveExpression,
        *glsl.unary_expression_or_less
    ]
    expressions = [
        maybe_wrap(f_args[0], additive_expression_or_less),
        maybe_wrap(get_ddx(f_args[0], x, scope), glsl.binary_expression_or_less),
        maybe_wrap(f_args[1], additive_expression_or_less),
        maybe_wrap(get_ddx(f_args[1], x, scope), glsl.binary_expression_or_less)
    ]
    expression_strings = [
        peg.compose(expression, type(expression))
        for expression in expressions
    ]
    u, dudx, v, dvdx = expression_strings
    u_type = scope.get_expression_type(f_args[0])
    v_type = scope.get_expression_type(f_args[1])
    if (u_type == 'float' and v_type == 'float'):
        return peg.parse(f''' 
            {u} < {v} ? {dudx} : {dvdx}
        ''', glsl.TernaryExpression)
    else:
        throw_not_implemented_error(f, 'calls to component-wise min()')
    
def get_ddx_abs(f, x, scope):
    f_args = f.arguments
    additive_expression_or_less = [
        glsl.AdditiveExpression,
        *glsl.unary_expression_or_less
    ]
    expressions = [
        maybe_wrap(f_args[0], additive_expression_or_less),
        maybe_wrap(get_ddx(f_args[0], x, scope), glsl.binary_expression_or_less)
    ]
    expression_strings = [
        peg.compose(expression, type(expression))
        for expression in expressions
    ]
    u, dudx = expression_strings
    u_type = scope.get_expression_type(f_args[0])
    if (u_type == 'float'):
        return peg.parse(f''' 
            {u} > 0.0f ? {dudx} : -{dudx}
        ''', glsl.TernaryExpression)
    else:
        throw_not_implemented_error(f, 'calls to component-wise abs()')

def get_ddx_sqrt(f, x, scope):
    f_args = f.arguments
    output = glsl.MultiplicativeExpression(
        maybe_wrap(get_ddx(f_args[0], x, scope)),
        '/',
        maybe_wrap(glsl.MultiplicativeExpression('2.0f', '*', f)),
    )
    return output


def get_ddx_dot(f, x, scope):
    if len(f.arguments) != 2:
        throw_value_error(f, 'dot product must have two parameters')
    u = f.arguments[0]
    v = f.arguments[1]
    u_type = scope.get_expression_type(u)
    v_type = scope.get_expression_type(v)

    if (u_type not in glsl.vector_types or 
        v_type not in glsl.vector_types):
        throw_value_error(f, 'dot product must only accept vectors as parameters')
    if (u_type not in glsl.float_vector_types or 
        v_type not in glsl.float_vector_types):
        throw_not_implemented_error(f, 'non-floating point dot products')

    expressions = [
        maybe_wrap( u ),
        maybe_wrap( get_ddx(u, x, scope) ),
        maybe_wrap( v ),
        maybe_wrap( get_ddx(v, x, scope) ),
    ]
    expression_strings = [
        peg.compose(expression, type(expression))
        for expression in expressions
    ]
    '''
    The derivative is defined below. 
    As you can see, the derivative calls the dot product on u, v, dudx, and dvdx
    This means both u, v, dudx, and dvdx must be vectors.
    If both dudx and dvdx are vectors, and u and v are as well,
    then either x is a float or dudx and dvdx are component-wise derivatives,
    since we do not support jacobian derivatives.
    If x is a float, then this is a well supported use case in mathematics.
    If x is a vector though, then we require u and v to be component-wise expressions.
    However is this sensible to the user?

    We start by asking what the user expects in this case.
    They would really want dfdx to be a vector where each component 
    indicates the amount the output of the dot product changes in response to 
    a change in the corresponding component of x. 
    If u(x) and v(x) are defined in a way that mixes components,
    then we would need the jacobian to properly express this.
    However, if u(x) and v(x) are defined only to use component wise expressions, 
    then we could do this using the usual equations, 
    since this would be equivalent to dealing with the jacobian 
    where non-diagonal components are 0.
    So unless we support jacobians, we must mandate that u and v are component-wise only.
    Until then, we can get by with adapting the equation for the dot product
    between vector gradients where non-diagonal elements are 0
    See here: https://math.stackexchange.com/questions/496060/gradient-of-a-dot-product
    assuming Einstein notation:
        d(ai*bj)/dxi = bj*daj/dxi + aj*dbj/dxi
    However we assume daj/dxi = 0 and dbj/dxi = 0 where i!=j, 
    this is equivalent to setting i==j, so it is equivalent to:
        d(ai*bi)/dxi = bi*dai/dxi + ai*dbi/dxi

    Obviously we must make very sure that all other operations are component-wise,
    but how do we enforce this?
    Virtually all glsl arithmetic operators are component-wise on vectors,
    however vector constructors, swizzling, cross products, matrix multiplication,
    and attriute assignment all allow for non-component-wise behavior.
    Fortunately, without constructors or assignments,
    there should be no way for us to take output from the dot product 
    create non-component-wise behavior, 
    since it would amount to turning a float back into a vector.
    '''
    u, dudx, v, dvdx = expression_strings
    return peg.parse(
        f' dot({u}, {dvdx}) + dot({dudx}, {v}) ', 
        glsl.AdditiveExpression
    )

def get_ddx_invocation_expression(f, x, scope):
    dfdu_getter_map = {
        'abs' : get_ddx_abs,
        'min' : get_ddx_min,
        'max' : get_ddx_max,
        'sqrt': get_ddx_sqrt,
        'dot' : get_ddx_dot,
        'normalize': lambda f,x,scope: throw_not_implemented_error(f, 'non-component-wise functions'),
        'cross': lambda f,x,scope: throw_not_implemented_error(f, 'non-component-wise functions'),
    }
    dfdu_name_map = {
        'length': 'normalize',
        'sin': 'cos',
    }
    # supported constructor (constant floating point vector)
    if (f.reference in glsl.float_vector_types and 
        all([isinstance(argument, str) and 
             (glsl.float_literal.match(argument) or 
              glsl.int_literal.match(argument))
             for argument in f.arguments])):
        return glsl.NewStyleInvocationExpression(
            f.reference, ['0.0f' for argument in f.arguments]
        )
    # non-supported constructor (built-in)
    elif f.reference in glsl.built_in_types:
        throw_not_implemented_error(f, 'constructors')
    # non-supported constructor (user-defined)
    elif f.reference in scope.attributes:
        throw_not_implemented_error(f, 'user-defined data structures')
    # function invocation (built-in)
    elif f.reference in dfdu_getter_map:
        return dfdu_getter_map[f.reference](f, x, scope)
    # function invocation (user-defined)
    elif len(f.arguments) > 1:
        throw_not_implemented_error(f, 'user-defined multi-parameter functions')
    elif len(f.arguments) < 1:
        return get_0_for_type(scope.get_expression_type(f))
    else:
        dfdu = dfdu_name_map[f.reference] if f.reference in dfdu_name_map else f'dd{x}_{f.reference}'
        dudx = maybe_wrap(get_ddx(f.arguments[0], x, scope))
        return glsl.MultiplicativeExpression(
            glsl.NewStyleInvocationExpression(dfdu, f.arguments), 
            '*', 
            dudx
        )

def get_ddx_attribute_expression(f, x, scope):
    '''
    Here is where we have to start worrying about type
    monitor the type of every differential.
    Remember: 
    * if x_type is a vector, the return type must also be a vector, du/dXi
    * if f_type is a vector, the return type must also be a vector, dUi/dx
    * if x_type and f_type are both vectors, the return type must be a vector,
      and represent the component-wise derivative, dUi/dXi
    * at no point can you use a jacobian derivative dUi/dXj,
      so functions must be either entirely composed of:
       * derivatives of vectors with respect to scalars, or...
       * derivatives of scalars with respect to vectors
    '''
    f_type = scope.get_expression_type(f)
    x_type = scope.get_expression_type(x)

    # variable reference
    type_ = scope.get_expression_type(f.reference)
    if f.reference == x:
        ddx = get_1_for_type(scope.get_expression_type(f))
    elif isinstance(f.reference, str):
        ddx = f'dd{x}_{f.reference}'
    else:
        ddx  = get_ddx(f.reference, x, scope)

    updated_ddx = None
    for attribute in f.attributes:
        if isinstance(attribute, glsl.BracketedExpression):
            # matrix column access
            if type_ in glsl.matrix_types:
                updated_type = re.sub('mat(\d)x?', 'vec\\1', type_)
                throw_not_implemented_error(f, 'matrix column access')
            # vector component access
            elif type_ in glsl.vector_types:
                updated_type = 'float'
                if (isinstance(attribute.content, str) and 
                    glsl.int_literal.match(attribute.content)):
                    if x_type == 'float':
                        # V(u)[0] -> dVdu[0]
                        updated_ddx = glsl.NewStyleAttributeExpression(ddx, attribute)
                    elif x_type in glsl.float_vector_types:
                        # V(U)[0] -> vec3(dVdU[0], 0.f, 0.f)
                        vecN = type_
                        N = int(vecN[-1])
                        i = int(attribute.content)
                        vecN_params = ['0.0f' for i in range(N)]
                        vecN_params[i] = peg.compose(glsl.NewStyleAttributeExpression(updated_ddx, attribute), glsl.NewStyleAttributeExpression)
                        vecN_params = ','.join(vecN_params)
                        updated_ddx = peg.parse(f'{vecN}({vecN_params})', glsl.NewStyleInvocationExpression)
                    else:
                        throw_not_implemented_error(f, 'component access for non-float derivatives')
                else:
                    throw_not_implemented_error(f, 'variable vector component access')
            # array index access
            elif (isinstance(type_, glsl.NewStyleAttributeExpression)):
                updated_type = [type_]
                throw_not_implemented_error(f, 'array index access')
            else:
                throw_not_implemented_error(f)
        elif isinstance(attribute, str):
            # vector component access
            if type_ in glsl.vector_types: # likely an attribute of a built-in structure, like a vector
                updated_type = 'float'
                if len(attribute) > 1:
                    throw_not_implemented_error(f, 'swizzling')
                if x_type == 'float':
                    # V(u).x -> dVdu.x
                    updated_ddx = glsl.NewStyleAttributeExpression(ddx, attribute)
                elif x_type in glsl.float_vector_types:
                    # V(U).x -> vec3(dVdU[0], 0.f, 0.f)
                    vecN = type_
                    N = int(vecN[-1])
                    i = {
                        'x':0,'y':1,'z':2,'w':3,
                        'r':0,'g':1,'b':2,'a':3,
                        's':0,'t':1,'u':2,'v':3,
                      }[attribute]
                    vecN_params = ['0.0f' for i in range(N)]
                    vecN_params[i] = peg.compose(glsl.NewStyleAttributeExpression(updated_ddx, attribute), glsl.NewStyleAttributeExpression)
                    vecN_params = ','.join(vecN_params)
                    updated_ddx = peg.parse(f'{vecN}({vecN_params})', glsl.NewStyleInvocationExpression)
                else:
                    throw_not_implemented_error(f, 'component access for non-float derivatives')
            # attribute access
            elif (type_ in scope.attributes and
                attribute in scope.attributes[type_]):
                updated_type = scope.attributes[type_][attribute]
                throw_not_implemented_error(f, 'attribute access')
            else:
                throw_not_implemented_error(f)
        type_ = updated_type
        ddx = updated_ddx

    return ddx 

def get_ddx_multiplicative_expression(f, x, scope):
    # programming at its finest, boys 😃
    # multiplication in glsl is always element wise, 
    # so we don't have to worry about managing types

    expressions = [
        #u
        f.operand1, 
        #dudx
        get_ddx(f.operand1, x, scope), 
        #v
        f.operand2, 
        #dvdx
        get_ddx(f.operand2, x, scope), 
    ]

    expressions_wrapped = [
        maybe_wrap(expression) 
        for expression in expressions
    ]

    expression_strings = [ 
        peg.compose(expression, type(expression)) 
        for expression in expressions_wrapped
    ]

    u, dudx, v, dvdx = expression_strings

    if f.operator == '*':
        dfdx = peg.parse(f'{v}*{dudx} + {u}*{dvdx}', glsl.AdditiveExpression)
    elif f.operator == '/':
        dfdx = peg.parse(f'({v}*{dudx} - {u}*{dvdx})/({v}*{v})', glsl.MultiplicativeExpression)
    else:
        throw_value_error(f, f'multiplicative expressions cannot have an operator of "{f.operator}"')

    return dfdx


def get_ddx_additive_expression(f, x, scope):
    # addition is always element-wise, 
    # so we don't have to worry about managing types

    multiplicative_expression_or_less = [
        glsl.MultiplicativeExpression, 
        *glsl.unary_expression_or_less
    ]
    return glsl.AdditiveExpression(
        maybe_wrap(
            get_ddx(f.operand1, x, scope), 
            multiplicative_expression_or_less
        ),
        f.operator,
        maybe_wrap(
            get_ddx(f.operand2, x, scope), 
            multiplicative_expression_or_less
        ),
    )

def get_ddx_parens_expression(f, x, scope):
    return glsl.ParensExpression(
        get_ddx(f.content, x, scope)
    )

def get_ddx_literal(k, x, scope):
    k_type = scope.get_expression_type(k)
    return get_0_for_type(k_type)

def get_ddx_ternary_expression(f, x, scope):
    return glsl.TernaryExpression(
        f.operand1,
        maybe_wrap(get_ddx(f.operand2), glsl.binary_expression_or_less),
        maybe_wrap(get_ddx(f.operand3), glsl.binary_expression_or_less),
    )

def get_ddx_return_statement(f, x, scope):
    return glsl.ReturnStatement(get_ddx(f.value, x, scope))

def get_ddx(f, x, scope):
    ''' 
    "get_ddx_function" is a pure function that 
    transforms an glsl grammar element matching pypeg2glsl.ternary_expression_or_less
    into a glsl parse tree representing the derivative with respect to a given variable. 
    '''
    derivative_map = {
        (str, get_ddx_literal),

        (glsl.NewStyleInvocationExpression, get_ddx_invocation_expression),
        (glsl.NewStyleAttributeExpression, get_ddx_attribute_expression),
        # (glsl.PostIncrementExpression,   ),
        # (glsl.PreIncrementExpression,    ),

        (glsl.MultiplicativeExpression,  get_ddx_multiplicative_expression),
        (glsl.AdditiveExpression,        get_ddx_additive_expression),
        # (glsl.ShiftExpression,           ),
        # (glsl.RelationalExpression,      ),
        # (glsl.EqualityExpression,        ),
        # (glsl.BitwiseAndExpression,      ),
        # (glsl.BitwiseXorExpression,      ),
        # (glsl.BitwiseOrExpression,       ),
        # (glsl.LogicalAndExpression,      ),
        # (glsl.LogicalXorExpression,      ),
        # (glsl.LogicalOrExpression,       ),

        (glsl.TernaryExpression,         get_ddx_ternary_expression),

        (glsl.ParensExpression,          get_ddx_parens_expression),

        (glsl.ReturnStatement,           get_ddx_return_statement)
    }
    for Rule, get_ddx_rule in derivative_map:
        if isinstance(f, Rule):
            dfdx = get_ddx_rule(f, x, scope)
            return dfdx
    raise throw_not_implemented_error(f, type(f).__name__)
    
def get_ddx_type(f_type, x_type, f=None):
    # scalar derivative
    if x_type == 'float' and f_type == 'float':
        return 'float'
    # gradient
    elif (f_type == 'float' and x_type in glsl.vector_types):
        return x_type
    # component-wise scalar derivative
    elif (f_type in glsl.vector_types and x_type == 'float'):
        return f_type
    # either jacobian or component-wise vector derivative,
    # however jacobians have little use in shaders,
    # so we assume it is a component-wise derivative
    elif (f_type in glsl.vector_types and x_type == f_type):
        return f_type

    f_type_str = peg.compose(f_type, type(f_type))
    x_type_str = peg.compose(x_type, type(x_type))
    raise throw_not_implemented_error(f, f'variables of type "{f_type_str}" and "{x_type_str}"')

def get_ddx_function(f, x, scope):
    ''' 
    "get_ddx_function" is a pure function that finds analytic derivatives.
    Given a glsl parse tree representing a glsl function,
    as well as the name of a parameter to that function, 
    and an instance of pypeg2glsl.LexicalScope,
    it returns a glsl parse tree representing the derivative 
    of that function with respect to the parameter. 
    The function works for only a small subset of derivatives 
    but it is intended to fail-loudly. 
    The most important restriction placed on functions is for all 
    vector operations to be **COMPONENT-WISE ONLY**.
    Jacobian derivatives are not currently supported, 
    and support for the dot product currently rests on this assumption.
    This means no vector constructors, swizzling, cross products, 
    matrix multiplication, or attriute assignments.
    '''
    local_scope = scope.get_subscope(f)
    x_type = local_scope.variables[x]

    dfdx = glsl.FunctionDeclaration(
        type_ = get_ddx_type(f.type, x_type, f),
        name  = f'dd{x}_{f.name}',
    )

    # return identity if x not in f
    x_param = [parameter for parameter in f.parameters if parameter.name == x]
    if len(x_param) < 1:
        dfdx.append(
            glsl.ReturnStatement(get_0_for_type(scope.get_expression_type(f)))
        )
        return dfdx

    # create parameters expressing derivatives of other parameters besides x
    dfdx.parameters = []
    for param in f.parameters:
        dfdx.parameters.append(copy.deepcopy(param))
        if param.name != x:
            dfdx.content.append(
                glsl.VariableDeclaration(
                    type_ = copy.deepcopy(param.type),
                    names = [ f'dd{x}_{param.name}' ],
                    value = get_0_for_type(param.type),
                )
            )

    # convert the content of the function
    for statement in f.content:
        if isinstance(statement, glsl.VariableDeclaration):
            dfdx.content.append( copy.deepcopy(statement) )
            dfdx.content.append(
                glsl.VariableDeclaration(
                    type_ = get_ddx_type(statement.type, x_type, statement),
                    names = [ f'dd{x}_{name}' for name in statement.names],
                    value = get_ddx(statement.value, x, local_scope),
                )
            )
        else:
            dfdx.content.append( get_ddx(statement, x, local_scope) )


    return dfdx

def convert_glsl(input_glsl):
    ''' 
    "convert_glsl" is a pure function that performs 
    a transformation on a parse tree of glsl as represented by glsl,
    then returns a transformed parse tree as output. 
    '''
    output_glsl = [
        get_ddx_function(declaration, declaration.parameters[0].name, glsl.LexicalScope(input_glsl)) 
        if isinstance(declaration, glsl.FunctionDeclaration)
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
    input_glsl = peg.parse(input_text, glsl.code)
    output_glsl = convert_glsl(input_glsl)
    output_text = peg.compose(output_glsl, glsl.code, autoblank = False) 
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
