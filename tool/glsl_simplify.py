#!/bin/env python3

"""
"glsl_simplify.py" can be used to remove superfluous mathematical expressions 
within glsl code.
It mostly exists to be used in combination with other scripts that 
automatically generate glsl code, primarily glsl_derivative.py.
This allows us to vastly simplify logic within other scripts that would 
otherwise need to express this simplification logic themselves. 

The command line interface for this script is meant to resemble sed.
You can select a file using the `-f` argument.
By default, the script will print out the results of a "dry run". 
You can modify the file in-place using the `-i` flag. 
You can print a diff between input and output using the `-v` flag.

For basic usage on a single file, call like so:
  python3 ./glsl_standardize.py -f file.glsl.c

If you want to replace all files in a directory, call like so:
 find . -name *.glsl.c \
     -exec echo {} \; -exec python3 ./glsl_standardize.py -if {} \;
"""


import copy
import difflib
import re
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

def assert_type(variable, types):
    if len(types) == 1 and not isinstance(variable, types[0]):
        raise AssertionError(f'expected {types[0]} but got {type(variable)} (value: {variable})')
    if not any([isinstance(variable, type_) for type_ in types]):
        raise AssertionError(f'expected any of {types} but got {type(variable)} (value: {variable})')

def get_1_for_type(type_):
    identity_map ={
        'vec2': peg.parse('vec2(1.f)', glsl.InvocationExpression),
        'vec3': peg.parse('vec3(1.f)', glsl.InvocationExpression),
        'vec4': peg.parse('vec4(1.f)', glsl.InvocationExpression),

        'float': '1.0f',
        'int': '1'
    }
    if type_ in identity_map:
        return identity_map[type_]
    else:
        throw_not_implemented_error(type_element, 'additives identities for types')
    
def get_0_for_type(type_):
    identity_map ={
        'vec2': peg.parse('vec2(0.f)', glsl.InvocationExpression),
        'vec3': peg.parse('vec3(0.f)', glsl.InvocationExpression),
        'vec4': peg.parse('vec4(0.f)', glsl.InvocationExpression),

        'float': '0.0f',
        'int': '0'
    }
    if type_ in identity_map:
        return identity_map[type_]
    else:
        throw_not_implemented_error(type_element, 'multiplicative identities for types')

def get_expression_strings(*expressions):
    return [peg.compose(expression, type(expression)) 
            for expression in expressions]

def get_simplified_multiplicative_expression(element, scope):
    a, b = (
        get_simplified(element.operand1, scope),
        get_simplified(element.operand2, scope)
    )
    if (isinstance(a, glsl.ParensExpression) and 
        isinstance(a.content, glsl.MultiplicativeExpression)):
        a2 = a.content.operand1
        b2 = glsl.MultiplicativeExpression(a.content.operand2, a.content.operator, b)
        # a, b = a2, b2
    if (isinstance(b, glsl.ParensExpression) and 
        isinstance(b.content, glsl.MultiplicativeExpression)):
        b = b.content
    # if (isinstance(b, str) and glsl.float_literal.match(b)):
        # a2 = b
        # b2 = a
        # a, b = a2, b2
    a_str, b_str = get_expression_strings(a, b)
    zero = re.compile('(vec[234])? \(? 0+\.?0*f? \)? $', re.VERBOSE)
    one  = re.compile('(vec[234])? \(? 0*1\.?0*f? \)? $', re.VERBOSE)
    if zero.match(a_str) or zero.match(b_str):
        return get_0_for_type(scope.deduce_type(element))
    elif one.match(a_str):
        return b
    elif one.match(b_str):
        return a
    else:
        return glsl.MultiplicativeExpression(a, element.operator, b)

def get_simplified_additive_expression(element, scope):
    a, b = (
        get_simplified(element.operand1, scope),
        get_simplified(element.operand2, scope)
    )
    if (isinstance(a, glsl.ParensExpression) and 
        isinstance(a.content, glsl.MultiplicativeExpression)):
        a = a.content
    if (isinstance(a, glsl.ParensExpression) and 
        isinstance(a.content, glsl.AdditiveExpression)):
        a2 = a.content.operand1
        b2 = glsl.AdditiveExpression(a.content.operand2, a.content.operator, b)
        a, b = a2, b2
    if (isinstance(b, glsl.ParensExpression) and 
        (isinstance(a.content, glsl.MultiplicativeExpression) or 
         isinstance(a.content, glsl.AdditiveExpression))):
        b = b.content
    a_str, b_str = get_expression_strings(a, b)
    zero = re.compile('(vec[234])? \(? 0+\.?0*f? \)? $', re.VERBOSE)
    if a_str == b_str:
        return glsl.MultiplicativeExpression('2.0f', '*', b_str)
    elif zero.match(a_str):
        return b
    elif zero.match(b_str):
        return a
    else:
        return glsl.AdditiveExpression(a, element.operator, b)

def get_simplified_parens_expression(element, scope):
    if isinstance(element.content, glsl.ParensExpression):
        return get_simplified(element.content, scope)
    else:
        return glsl.ParensExpression(get_simplified(element.content, scope))

def get_simplified_function_declaration(in_element, scope):
    out_element = copy.deepcopy(in_element)
    out_element.content = get_simplified(in_element.content, scope.get_subscope(in_element))
    return out_element

def get_simplified_default_element(in_element, scope):
    out_element = copy.deepcopy(in_element)
    for attribute in glsl.element_attributes:
        if hasattr(in_element, attribute):
            setattr(out_element, attribute, get_simplified(in_element.__dict__[attribute], scope))
    return out_element

def get_simplified(element, scope):
    assert_type(element, [str, list, glsl.GlslElement])
    ''' 
    "get_simplified" is a pure function that 
    transforms an glsl grammar element matching pypeg2glsl.ternary_expression_or_less
    to an equivalent element where some superfluous 
    mathematical expressions have been removed, where able to do so.
    We simplify code in a separate step as it allows us to vastly simplify 
    logic elsewhere in code that would otherwise need to express this 
    simplification logic themselves. 
    '''
    simplification_map = {
        (str,  lambda element, scope: element),
        (list, lambda element, scope: [get_simplified(element, scope) for element in element]),
        (glsl.MultiplicativeExpression,  get_simplified_multiplicative_expression),
        (glsl.AdditiveExpression,        get_simplified_additive_expression),
        (glsl.ParensExpression,          get_simplified_parens_expression),
        (glsl.FunctionDeclaration,       get_simplified_function_declaration),
    }
    for Rule, get_simplification_rule in simplification_map:
        if isinstance(element, Rule):
            simplified = get_simplification_rule(element, scope)
            assert_type(simplified, [str, list, glsl.GlslElement])
            return simplified
    # if no rule is found, run the default simplifier
    return get_simplified_default_element(element, scope)


def convert_glsl(input_glsl):
    ''' 
    "convert_glsl" is a pure function that performs 
    a transformation on a parse tree of glsl as represented by pypeg2glsl,
    then returns a transformed parse tree as output. 
    '''
    output_glsl = get_simplified(input_glsl, glsl.LexicalScope(input_glsl))
    glsl.warn_of_invalid_grammar_elements(output_glsl)
    return output_glsl

def convert_text(input_text):
    ''' 
    "convert_text" is a pure function that performs 
    a transformation on a string containing glsl code,
    then returns transformed output. 
    It may run convert_glsl behind the scenes, 
    and may also perform additional string based transformations,
    such as appending utility functions 
    or performing simple string substitutions 
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
