#!/bin/env python3

import copy
import difflib
import re
import sys

import pypeg2
import pypeg2glsl

"""
The command line interface for this script is meant to resemble sed.
You can select a file using the `-f` option.
By default, the script will print out the results of a "dry run". 
You can modify the file in-place using the `-i` option. 
If you want to replace all files in a directory, call like so:
 find . -name *.glsl.c \
     -exec echo {} \; -exec python3 ./glsl_standardize.py -if {} \;
"""

APPENDED = '''

struct maybe_float_pair
{
    float first; 
    float last; 
    bool  exists; 
};
struct maybe_float
{
    float value;  
    bool  exists; 
};

maybe_float get_distance_along_line_to_union(
    in maybe_float shape1,
    in maybe_float shape2
) {
    return maybe_float(
        !shape1.exists ? shape2.value : !shape2.exists ? shape1.value : min(shape1.value, shape2.value),
        shape1.exists || shape2.exists
    );
}

maybe_float_pair get_distances_along_line_to_union(
    in maybe_float_pair shape1,
    in maybe_float_pair shape2
) {
    return maybe_float_pair(
        !shape1.exists ? shape2.first : !shape2.exists ? shape1.first : min(shape1.first, shape2.first),
        !shape1.exists ? shape2.last  : !shape2.exists ? shape1.last  : max(shape1.last,  shape2.last ),
        shape1.exists || shape2.exists
    );
}

maybe_float_pair get_distances_along_line_to_negation(
    in maybe_float_pair shape1,
    in maybe_float_pair shape2
) {
    return maybe_float_pair(
        shape2.exists || shape1.first < shape2.first ? shape1.first : max(shape2.last,  shape1.first),
        shape2.exists || shape1.last  < shape2.last  ? shape1.last  : max(shape2.first, shape1.last ),
        shape1.exists && (!shape2.exists || shape1.first < shape2.first || shape2.last < shape1.last)
    );
}

maybe_float_pair get_distances_along_line_to_intersection(
    in maybe_float_pair shape1,
    in maybe_float_pair shape2
) {
    float first = shape1.exists && shape2.exists ? max(shape1.first, shape2.first) : 0.f;
    float last  = shape1.exists && shape2.exists ? min(shape1.last,  shape2.last ) : 0.f;
    return maybe_float(first, last, shape1.exists && shape2.exists && first < last);
}

'''     

def convert_glsl(glsl):
    glsl = copy.deepcopy(glsl)
    for function in glsl:
        if isinstance(function, pypeg2glsl.FunctionDeclaration):
            invoked_out_params = []
            for statement in function.content:
                if ( (isinstance(statement, pypeg2glsl.VariableDeclaration)) and
                      isinstance(statement.value, pypeg2glsl.PostfixExpression) and
                      isinstance(statement.value.content[-1], pypeg2glsl.InvocationExpression) and
                      len(statement.value.content) == 2 ):
                    invoked_function_name = statement.value.content[0]
                    invoked_params = statement.value.content[-1]

                    if 'get_distances' in invoked_function_name:
                        invoked_out_param_count = 2
                        statement.type = pypeg2glsl.PostfixExpression(['maybe_float_pair'])
                        invoked_out_params = [invoked_out_params, *invoked_params.content[-invoked_out_param_count:]]
                        invoked_params.content = invoked_params.content[:-invoked_out_param_count]

                    elif 'get_distance' in invoked_function_name and '_plane' not in invoked_function_name:
                        invoked_out_param_count = 1
                        statement.type = pypeg2glsl.PostfixExpression(['maybe_float'])
                        invoked_out_params = [invoked_out_params, *invoked_params.content[-invoked_out_param_count:]]
                        invoked_params.content = invoked_params.content[:-invoked_out_param_count]

            function.content = [
                statement for statement in function.content 
                if not isinstance(statement, pypeg2glsl.VariableDeclaration) or
                   all([name not in invoked_out_params for name in statement.names])
            ]

            out_params = [param for param in function.parameters if param.name in ['entrance', 'exit', 'intersection', 'closest_approach']]

            if len(out_params) == 2:
                function.type = pypeg2glsl.PostfixExpression(['maybe_float_pair'])
                function.name = function.name.replace('_distance_', '_distances_')
                function.parameters = function.parameters[:-2]

                entrance = None
                exit = None
                for statement in function.content:
                    if (isinstance(statement, pypeg2glsl.AssignmentExpression) and
                        statement.operand1.content[0] in ['entrance']):
                        entrance = statement.operand2
                    if (isinstance(statement, pypeg2glsl.AssignmentExpression) and
                        statement.operand1.content[0] in ['exit']):
                        exit = statement.operand2

                assert entrance and exit, 'Must have entrance and exit'
                function.content = [
                    statement for statement in function.content 
                    if not isinstance(statement, pypeg2glsl.AssignmentExpression) or
                       statement.operand1.content[0] not in ['entrance', 'exit']
                   ]
                for statement in function.content:
                    if (isinstance(statement, pypeg2glsl.ReturnStatement) and 
                        '_hits' not in pypeg2.compose(statement, pypeg2glsl.ReturnStatement)):
                        statement.value = pypeg2glsl.PostfixExpression(['maybe_float_pair', pypeg2glsl.InvocationExpression([*([entrance] if entrance else []), *([exit] if exit else []), statement.value])])

            elif len(out_params) == 1:
                function.type = pypeg2glsl.PostfixExpression(['maybe_float'])
                function.name = function.name.replace('_distances_', '_distance_')
                function.parameters = function.parameters[:-1]

                output = None
                for statement in function.content:
                    if (isinstance(statement, pypeg2glsl.AssignmentExpression) and
                        statement.operand1.content[0] in ['entrance', 'intersection', 'closest_approach']):
                        output = statement.operand2

                function.content = [
                    statement for statement in function.content 
                    if not isinstance(statement, pypeg2glsl.AssignmentExpression) or
                       statement.operand1.content[0] not in ['entrance', 'intersection', 'closest_approach']
                   ]
                assert output, 'Must have output'
                for statement in function.content:
                    if (isinstance(statement, pypeg2glsl.ReturnStatement) and 
                        '_hits' not in pypeg2.compose(statement, pypeg2glsl.ReturnStatement)):
                        statement.value = pypeg2glsl.PostfixExpression(['maybe_float', pypeg2glsl.InvocationExpression([*([output] if output else []), statement.value])])

            for param in function.parameters:
                if 'in' not in param.qualifiers and 'out' not in param.qualifiers:
                    param.qualifiers = ['in']
    glsl = [
        statement for statement in glsl 
        if not isinstance(statement, pypeg2glsl.FunctionDeclaration) 
        or not any([keyword in statement.name for keyword in ['union','negation','intersection','relation']])
    ]
    return glsl

def convert_text(input_text):
    input_text = input_text.replace('try_', 'get_')
    input_glsl = pypeg2.parse(input_text, pypeg2glsl.glsl)
    output_glsl = convert_glsl(input_glsl)
    output_text = pypeg2.compose(output_glsl, pypeg2glsl.glsl, autoblank = False) 
    output_text = APPENDED + output_text
    return output_text

def convert_file(input_filename=False, in_place=False, verbose=False):
    input_text = ''
    if input_filename:
        with open(input_filename, 'r+') as input_file:
            input_text = input_file.read()
    else:
        for line in sys.stdin:
            input_text += line

    output_text = convert_text(input_text)

    diff = difflib.ndiff(
        input_text.splitlines(keepends=True), 
        output_text.splitlines(keepends=True)
    )
    for line in diff:
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
    # parser.add_argument('-v', '--verbose', dest='verbose', 
    #     help='show debug information', action='store_true')
    args = parser.parse_args()
    convert_file(
        args.filename, 
        in_place=args.in_place, 
        verbose=False, 
    )
