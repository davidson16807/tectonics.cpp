# holy fucking shit that was a lot of work 😩

'''
"pypeg2glsl" is a library for parsing and lexing glsl using the pypeg2 library.

It has the following features:
* pypeg2 grammar rule classes for parsing glsl.
* a "LexicalScope" class for storing type information within glsl lexical scopes
* a "get_expression_type" function for lexing type information from glsl expressions
* various variables storing information about built in glsl types
  (vector_types, matrix_types, built_in_types)

See pypeg2 documentation for more information on usage.
'''

import re
import copy
import warnings

import pypeg2
from pypeg2 import attr, optional, maybe_some, blank, endl

def assert_type(variable, types):
    if not any([isinstance(variable, type_) for type_ in types]):
        raise AssertionError(f'expected {types} but got {variable}')

inline_comment = re.compile('/\*((?!\*/).)*\*/\s*', re.MULTILINE | re.DOTALL)
endline_comment = re.compile('//[^\n]*\s*', re.MULTILINE | re.DOTALL)
include_directive = re.compile('#[^\n]*\s*', re.MULTILINE | re.DOTALL)
int_literal = re.compile(
    '''
    (
        0 [0-7]+ | 
        0x[0-9a-f]+ |
          [0-9]+
    )
    u?
    ''', 
    re.MULTILINE | re.DOTALL | re.VERBOSE | re.IGNORECASE
)
float_literal = re.compile(
    '''
    (
        0x         \.[0-9a-f]+ (e -? \d+)? |
        0x[0-9a-f]+\.[0-9a-f]* (e -? \d+)? |
                     [0-9a-f]+ e -? \d+ | 

           \.\d+ (e -? \d+)? |
        \d+\.\d* (e -? \d+)? | 
        \d        e -? \d+ 
    )
    f?
    ''',
    re.MULTILINE | re.DOTALL | re.VERBOSE | re.IGNORECASE
)
bool_literal = re.compile('true|false')
token = re.compile('[a-zA-Z_]\w*')

'''
"element_attributes" is a list of all attributes 
that can be found within instances of GlslElements
'''
element_attributes = [
    'documentation',
    'qualifiers',
    'type',

    'name',
    'names',

    'parameters',
    'declaration',
    'condition',
    'operation',

    'value',
    'content',
    'else',

    'reference',
    'arguments',
    'attribute',
    'index',
    'operand1',
    'operator',
    'operand2',
    'operand3',

    'comment1',
    'comment2',
    'comment3',
]
'''
"warn_of_invalid_grammar_elements" recursively calls pypeg2.compose() 
on subelements of a glsl element and checks for errors.
If an error occurs, it issues a warning 
stating which subelements are causing problems. 
This makes it very useful for debugging errors where pypeg2.compose()
returns an empty string without explaining why.
'''
def warn_of_invalid_grammar_elements(element):
    if isinstance(element, list):
        for subelement in element:
            warn_of_invalid_grammar_elements(subelement)
    elif isinstance(element, GlslElement):
        try:
            pypeg2.compose(element, type(element))
        except ValueError as error:
            warnings.warn(f'element of type "{type(element)}" is invalid: \n{debug(element, "  ")}')
        for attribute in element_attributes:
            if hasattr(element, attribute):
                subelement = getattr(element, attribute)
                warn_of_invalid_grammar_elements(subelement)

'''
"debug" returns a string representing a variable that is intended to be a GlslElement.
It attempts to provide a string representation of the element using pypeg2.compose()
In the event it is unable to do so, it recurses through element attributes 
to give a description of which subelements are causing problems.
This makes it very useful for debugging errors where pypeg2.compose()
returns an empty string without explaining why.
'''
def debug(element, indent=''):
    if isinstance(element, list):
        if len(element) == 0:
            return indent+'[]'
        subelements = ', \n'.join([
                debug(subelement, indent+"  ")
                for subelement in element
            ])
        return f'{indent}[\n{subelements}\n{indent}]'
    elif isinstance(element, GlslElement):
        try:
            return (f'{indent}pypeg2glsl.{type(element).__name__}<"{pypeg2.compose(element, type(element))}">')
        except ValueError as e:
            header = f'{indent}pypeg2glsl.{type(element).__name__}<ERROR>'
            invalid = '\n'.join([
                    f'{indent}  {attribute}: \n{debug(getattr(element, attribute), indent+"    ")}' 
                    for attribute in element_attributes 
                    if hasattr(element, attribute)
                ])
            return f'{header}\n{invalid}'
    else:
        return indent + repr(element)

'''
"GlslElement" is the parent class of all grammar rule classes within pypeg2glsl
'''
class GlslElement:
    def __init__(self):
        pass

class InvocationExpression(GlslElement):
    def __init__(self, reference='', arguments=None):
        self.reference = reference
        self.arguments = arguments

class AttributeExpression(GlslElement):
    def __init__(self, reference='', attributes=None):
        self.reference = reference
        self.attributes = attributes or []

class UnaryExpression(GlslElement): 
    def __init__(self, operand1 = None, operator = ''):
        self.operand1 = operand1
        self.operator = operator
        
class BinaryExpression(GlslElement): 
    def __init__(self, operand1 = None, operator = '', operand2 = None):
        self.operand1 = operand1
        self.operator = operator
        self.operand2 = operand2
        self.comment1 = ''
        self.comment2 = ''
        self.comment3 = ''

class TernaryExpression(GlslElement): 
    def __init__(self, operand1 = None, operand2 = None, operand3 = None):
        self.operand1 = operand1
        self.operand2 = operand2
        self.operand3 = operand3

class PostIncrementExpression(UnaryExpression): pass
class PreIncrementExpression(UnaryExpression): pass

class MultiplicativeExpression(BinaryExpression): pass
class AdditiveExpression(BinaryExpression): pass
class ShiftExpression(BinaryExpression): pass
class RelationalExpression(BinaryExpression): pass
class EqualityExpression(BinaryExpression): pass
class BitwiseAndExpression(BinaryExpression): pass
class BitwiseXorExpression(BinaryExpression): pass
class BitwiseOrExpression(BinaryExpression): pass
class LogicalAndExpression(BinaryExpression): pass
class LogicalXorExpression(BinaryExpression): pass
class LogicalOrExpression(BinaryExpression): pass

class BracketedExpression(GlslElement): 
    def __init__(self, content=None):
        self.content = content or []
class ParensExpression(GlslElement): 
    def __init__(self, content=None):
        self.content = content or []
class AssignmentExpression(GlslElement): 
    def __init__(self, operand1 = None, operator = '', operand2 = None):
        self.operand1 = operand1
        self.operator = operator
        self.operand2 = operand2
class VariableDeclaration(GlslElement): 
    def __init__(self, type_=None, names=None, value=None, qualifiers=None):
        self.qualifiers = qualifiers or []
        self.type = type_
        self.names = names or []
        self.value = value
class ReturnStatement(GlslElement): 
    def __init__(self, value=None):
        self.value = value

class IfStatement(GlslElement): pass
class WhileStatement(GlslElement): pass
class DoWhileStatement(GlslElement): pass
class ForStatement(GlslElement): pass

class StructureDeclaration(GlslElement): pass
class ParameterDeclaration(GlslElement): 
    def __init__(self, type_=None, name='', qualifiers=None):
        self.qualifiers = qualifiers or []
        self.type = type_
        self.name = name
class FunctionDeclaration(GlslElement): 
    def __init__(self, type_=None, name='', parameters=None, content=None, documentation=None):
        self.documentation = documentation or []
        self.type = type_ or []
        self.name = name
        self.parameters = parameters or []
        self.content = content or []


primary_expression = [
    float_literal, 
    int_literal, 
    bool_literal, 
    token,
    ParensExpression
]

postfix_expression_or_less = [
    AttributeExpression,
    InvocationExpression,
    *primary_expression,
]

PostIncrementExpression.grammar = (
    attr('operand1', postfix_expression_or_less), 
    attr('operator', re.compile('\+\+|--'))
)
PreIncrementExpression.grammar = (
    attr('operator', re.compile('\+\+|--|[!+-]')), 
    attr('operand1', [PostIncrementExpression, *postfix_expression_or_less])
)
unary_expression_or_less = [PreIncrementExpression, PostIncrementExpression, *postfix_expression_or_less]

order_of_operations = [
    (MultiplicativeExpression, re.compile('[*/]')),
    (AdditiveExpression, re.compile('[+-]')),
    (ShiftExpression, re.compile('<<|>>')),
    (RelationalExpression, re.compile('[<>]=?')),
    (EqualityExpression, re.compile('==|\!=')),
    (BitwiseAndExpression, re.compile('&')),
    (BitwiseXorExpression, re.compile('^')),
    (BitwiseOrExpression, re.compile('\|')),
    (LogicalAndExpression, re.compile('&&')),
    (LogicalXorExpression, re.compile('^^')),
    (LogicalOrExpression, re.compile('\|\|'))
]

binary_expression_or_less = [*unary_expression_or_less]
for BinaryExpressionTemp, binary_regex in order_of_operations:
    binary_expression_or_less = [BinaryExpressionTemp, *binary_expression_or_less]
    BinaryExpressionTemp.grammar = (
            attr('operand1', binary_expression_or_less[1:]), 
            attr('comment1', maybe_some([inline_comment, endline_comment])),
            blank,
            attr('operator', binary_regex), 
            blank,
            attr('comment2', maybe_some([inline_comment, endline_comment])),
            attr('operand2', binary_expression_or_less),
            attr('comment3', maybe_some([inline_comment, endline_comment])),
        )

ternary_expression_or_less = [TernaryExpression, *binary_expression_or_less]
TernaryExpression.grammar = (
    attr('operand1', binary_expression_or_less), '?', blank,
    attr('operand2', ternary_expression_or_less), blank, ':', blank,
    attr('operand3', ternary_expression_or_less)
)

BracketedExpression.grammar  = '[', attr('content', ternary_expression_or_less), ']'
ParensExpression.grammar     = '(', attr('content', ternary_expression_or_less), ')'

InvocationExpression.grammar = (
    attr('reference', token),
    '(', attr('arguments', pypeg2.csl(ternary_expression_or_less)), ')'
)
AttributeExpression.grammar = (
    attr('reference', [ InvocationExpression, token, ParensExpression ]),
    attr('attributes', pypeg2.some([ BracketedExpression, ('.', token) ]))
)

AssignmentExpression.grammar = (
    attr('operand1', [ AttributeExpression, token ]), blank,
    attr('operator', re.compile('[*/+-]?=')), blank,
    attr('operand2', [AssignmentExpression, *ternary_expression_or_less])
)

VariableDeclaration.grammar = (
    attr('qualifiers', maybe_some(re.compile('const|highp|mediump|lowp|attribute|uniform|varying'))),
    attr('type', [ AttributeExpression, token ]), blank,
    attr('names', pypeg2.csl(token)),
    attr('value', optional(blank, '=', blank, [AssignmentExpression, *ternary_expression_or_less]))
)

ReturnStatement.grammar = ('return', blank, attr('value', optional(ternary_expression_or_less)))

simple_statement = ([
    re.compile('continue|break|discard'), ReturnStatement,
    VariableDeclaration, AssignmentExpression, InvocationExpression
], ';', endl)
code_block = maybe_some(
    [
        pypeg2.ignore(include_directive),
        inline_comment, 
        endline_comment,
        ForStatement, 
        WhileStatement, 
        DoWhileStatement, 
        IfStatement, 
        simple_statement
    ]
)
compound_statement = ( '{', endl, pypeg2.indent(code_block), '}', endl )

IfStatement.grammar = (
    'if', blank, '(', attr('condition', ternary_expression_or_less), ')', endl,
    attr('content', [compound_statement, simple_statement] ), 
    attr('else', optional('else', blank, [IfStatement, compound_statement, simple_statement])), endl
)
WhileStatement.grammar = (
    'while', blank, '(', attr('condition', ternary_expression_or_less), ')', endl,
    attr('content', [compound_statement, simple_statement] ), endl
)
DoWhileStatement.grammar = (
    'do', endl, attr('content', [compound_statement, simple_statement] ), 
    'while', blank, '(', attr('condition', ternary_expression_or_less), ')', ';', endl, endl
)
ForStatement.grammar = (
    'for', blank, '(', 
        attr('declaration', VariableDeclaration), ';', blank,
        attr('condition', ternary_expression_or_less), ';', blank,
        attr('operation', optional([ AssignmentExpression, PreIncrementExpression, PostIncrementExpression ])), 
    ')', endl,
    attr('content', [compound_statement, simple_statement] ), endl
)

ParameterDeclaration.grammar = (
    attr('qualifiers', maybe_some(re.compile('in|out|inout'), blank)),
    attr('type', [ AttributeExpression, token ]), blank,
    attr('name', token)
)
FunctionDeclaration.grammar = (
    attr('documentation', maybe_some([(inline_comment, endl), endline_comment])),
    attr('type', [ AttributeExpression, token ]), blank, attr('name', token), 
    '(', 
    attr('parameters',  
            optional(endl, pypeg2.indent(ParameterDeclaration, maybe_some(',', endl, ParameterDeclaration)), endl)  
        ), 
    ')', 
    attr('content', compound_statement), endl
)

StructureDeclaration.grammar = (
    'struct', blank, attr('name', token), 
    endl, '{', endl,
    attr('content', pypeg2.indent(maybe_some(VariableDeclaration, ';', endl)) ), 
    '}', ';', endl, endl
)

code = maybe_some(
    [
        StructureDeclaration, FunctionDeclaration, (VariableDeclaration, ';', endl),
        inline_comment, endline_comment,
        pypeg2.ignore(include_directive),
    ]
)

scalar_types = [
    'float', 'int', 'bool'
]
float_vector_types = [
    'vec2', 'vec3', 'vec4', 
]
int_vector_types = [
    'ivec2', 'ivec3', 'ivec4', 
]
bool_vector_types = [
    'bvec2', 'bvec3', 'bvec4', 
]
vector_types = [
    *float_vector_types,
    *int_vector_types,
    *bool_vector_types,
]
float_matrix_types = [
    'mat2',  'mat3',  'mat4',    'mat2x3',  'mat2x4',    'mat3x2',    'mat3x4',   'mat4x2',    'mat4x3', 
]
int_matrix_types = [
    'imat2', 'imat3', 'imat4',   'imat2x3', 'imat2x4',   'imat3x2',   'imat3x4',  'imat4x2',   'imat4x3',
]
bool_matrix_types = [
    'bmat2', 'bmat3', 'bmat4',   'bmat2x3', 'bmat2x4',   'bmat3x2',   'bmat3x4',  'bmat4x2',   'bmat4x3',
]
matrix_types = [
    *float_matrix_types,
    *int_matrix_types,
    *bool_matrix_types,
]
built_in_types = [
    *scalar_types,
    *vector_types,
    *matrix_types,
]
built_in_overloaded_functions = [
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
built_in_function_type_map = {
    'cross': ['vec3'],
    'dot': ['float'],
    'length': ['float'],
    'distance': ['float'],
}
class LexicalScope:
    """ 
    A "LexicalScope" is a conceptually immutable data structure containing 
    dictionaries to access type information for
    variables, functions return values, and data structure attributes
    within a given lexical scope.
    """

    @staticmethod
    def get_local_variable_type_lookups(function_content):
        if not isinstance(function_content, list):
            return {}
        result = { }
        code_block_element_types = [
            IfStatement,
            DoWhileStatement,
            WhileStatement,
            ForStatement,
        ]
        for element in function_content:
            if isinstance(element, ForStatement):
                for name in element.declaration.names:
                    result[name] = element.declaration.type
            if isinstance(element, VariableDeclaration):
                for name in element.names:
                    result[name] = element.type
            elif isinstance(element, ParameterDeclaration):
                result[element.name] = element.type
            elif type(element) in code_block_element_types:
                result.update(LexicalScope.get_local_variable_type_lookups(element.content))
        return result

    @staticmethod
    def get_global_variable_type_lookups(code):
        result = {}
        for element in code:
            if isinstance(element, VariableDeclaration):
                for name in element.names:
                    result[name] = element.type
        return result

    @staticmethod
    def get_attribute_type_lookups(code):
        result = {}
        for element in code:
            if isinstance(element, StructureDeclaration):
                attribute_types = {}
                for declaration in element.content:
                    if isinstance(declaration, VariableDeclaration):
                        for name in declaration.names:
                            result[name] = declaration.type
                result[element.name] = attribute_types
        return result

    @staticmethod
    def get_function_type_lookups(code):
        result = {}
        for element in code:
            if isinstance(element, FunctionDeclaration):
                result[element.name] = element.type
        return result

    def __init__(self, code = []):
        self.variables  = LexicalScope.get_global_variable_type_lookups(code)
        self.functions  = LexicalScope.get_function_type_lookups(code)
        self.attributes = LexicalScope.get_attribute_type_lookups(code)
        
    def get_subscope(self, function):
        """
        returns a new LexicalScope object whose state reflects the type 
        information of variables defined within a local subscope
        """
        result = LexicalScope()
        result.attributes = copy.deepcopy(self.attributes)
        result.functions = copy.deepcopy(self.functions)
        result.variables = {
            **self.variables,
            **LexicalScope.get_local_variable_type_lookups(function.parameters),
            **LexicalScope.get_local_variable_type_lookups(function.content)
        }
        return result
        
    def get_expression_type(self, expression):
        def warn_of_type_deduction_failure(expression, description):
            expression_str = pypeg2.compose(expression, type(expression))
            warnings.warn(f'could not deduce type for {description} in "{expression_str}"')

        assert_type(expression, [str, GlslElement])

        type_ = None
        if isinstance(expression, str):
            # literals
            if float_literal.match(expression):
                type_ = 'float'
            elif int_literal.match(expression):
                type_ = 'int'
            elif bool_literal.match(expression):
                type_ = 'bool'
            # variable reference
            elif expression in self.variables:
                type_ = self.variables[expression]
            else:
                warn_of_type_deduction_failure( expression, f'reference to unknown variable "{expression.reference}"' )
        elif isinstance(expression, InvocationExpression):
            # constructor
            if (expression.reference in built_in_types or 
                expression.reference in self.attributes):
                type_ = [ expression.reference ]
            # function invocation (built-in)
            elif expression.reference in built_in_function_type_map:
                type_ = built_in_function_type_map[expression.reference]
            # function invocation (built-in, overloaded)
            elif expression.reference in built_in_overloaded_functions:
                param_types = [self.get_expression_type(param) for param in expression.arguments]
                vector_param_types = [param_type for param_type in param_types if param_type in vector_types]
                type_ = vector_param_types[0] if len(vector_param_types) > 0 else param_types[0]
            # function invocation (user-defined)
            elif expression.reference in self.functions:
                type_ = self.functions[expression.reference]
            else:
                warn_of_type_deduction_failure( expression, f'call to unknown function "{expression.reference}"' )
        elif isinstance(expression, AttributeExpression):
            type_ = self.get_expression_type(expression.reference)
            for attribute in expression.attributes:
                if isinstance(attribute, str):
                    # vector component access
                    if type_ in vector_types: # likely an attribute of a built-in structure, like a vector
                        #swizzling
                        if len(attribute) > 0:
                            type_ = f'vec{len(attribute)}'
                        #single component
                        else: 
                            type_ = 'float'
                    # attribute access
                    elif (type_ in self.attributes and
                          attribute in self.attributes[type_]):
                        type_ = self.attributes[type_][attribute]
                    else:
                        warn_of_type_deduction_failure( expression, f'''attribute of unknown data structure "{''.join(type_)}"''')
                if isinstance(attribute, BracketedExpression):
                    # matrix column access
                    if type_ in matrix_types:
                        type_ = re.sub('mat(\d)x?', 'vec\\1', type_)
                    # vector component access
                    elif type_ in vector_types:
                        type_ = 'float'
                    # array index access
                    elif (isinstance(type_, AttributeExpression)):
                        type_ = type_.reference
                    else:
                        warn_of_type_deduction_failure( expression, f'''index of non array "{''.join(type_)}"''')
        elif isinstance(expression, ParensExpression):
            type_ = self.get_expression_type(expression.content)
        elif isinstance(expression, UnaryExpression):
            type_ = self.get_expression_type(expression.operand1)
        elif isinstance(expression, BinaryExpression):
            type1 = self.get_expression_type(expression.operand1)
            type2 = self.get_expression_type(expression.operand2)
            vector_type = type1 if type1 in vector_types else type2 if type2 in vector_types else []
            matrix_type = type1 if type1 in matrix_types else type2 if type2 in matrix_types else []
            if vector_type:
                type_ = vector_type
            elif matrix_type:
                type_ = matrix_type
            else:
                expression_str = pypeg2.compose(expression, type(expression))
                operand1_str = pypeg2.compose(expression.operand1, type(expression.operand1))
                operand2_str = pypeg2.compose(expression.operand2, type(expression.operand2))
                if type1 == []:
                    print(f'WARNING: could not deduce type for variable "{operand1_str}" \n\t{expression_str}')
                elif type2 == []:
                    print(f'WARNING: could not deduce type for variable "{operand2_str}" \n\t{expression_str}')
                elif type1 != type2:
                    print(f'WARNING: type mismatch, operation "{expression.operator}" was fed left operand of type "{type1}" and right hand operand of type "{type2}" \n\t{expression_str} ')
                type_ = type1
        elif isinstance(expression, TernaryExpression):
            type1 = self.get_expression_type(expression.operand2)
            type2 = self.get_expression_type(expression.operand3)
            expression_str = pypeg2.compose(expression, type(expression))
            operand1_str = pypeg2.compose(expression.operand2, type(expression.operand2))
            operand2_str = pypeg2.compose(expression.operand3, type(expression.operand3))
            if type1 == []:
                print(f'WARNING: could not deduce type for variable "{operand1_str}" \n\t{expression_str}')
            elif type2 == []:
                print(f'WARNING: could not deduce type for variable "{operand2_str}" \n\t{expression_str}')
            elif type1 != type2:
                expression_str = pypeg2.compose(expression, TernaryExpression)
                print(f'WARNING: type mismatch, ternary operation takes a left hand operand of type "{type1}" and right hand operand of type "{type2} \n\t{expression_str}"')
                print(self.variables)
            type_ = type1
        elif isinstance(expression, FunctionDeclaration):
            type_ = self.functions[expression.name]
        else:
            raise ValueError(f'The grammar rule {type(expression)} has no concept of type and should not have been passed to get_expression_type()')

        assert_type(type_, [str, AttributeExpression])

        return type_
