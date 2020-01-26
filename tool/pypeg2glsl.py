# holy fucking shit that was a lot of work 😩

import re
import copy

import pypeg2
from pypeg2 import attr, optional, maybe_some, blank, endl

inline_comment = re.compile('/\*((?!\*/).)*\*/\s*', re.MULTILINE | re.DOTALL)
endline_comment = re.compile('//[^\n]*\s*', re.MULTILINE | re.DOTALL)
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

class UnaryExpression: pass
class BinaryExpression: pass

class PostfixExpression: pass
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

class TernaryExpression: pass
class BracketedExpression: pass
class ParensExpression: pass
class InvocationExpression: pass
class AssignmentExpression: pass
class VariableDeclaration: pass
class ReturnStatement: pass
class IfStatement: pass
class IfStatementOption: pass
class WhileStatement: pass
class DoWhileStatement: pass
class ForStatement: pass
class ParameterDeclaration: pass
class StructureDeclaration: pass
class FunctionDeclaration: pass

primary_expression = [
    float_literal, 
    int_literal, 
    bool_literal, 
    ParensExpression
]

PostfixExpression.grammar = (
    attr('content', ([token, ParensExpression], maybe_some([BracketedExpression, InvocationExpression, ('.', token)])))
)
postfix_expression_or_less = [
    PostfixExpression,
    *primary_expression
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
            attr('operand1', binary_expression_or_less[1:]), blank,
            attr('operator', binary_regex), blank,
            attr('operand2', binary_expression_or_less),
        )

ternary_expression_or_less = [TernaryExpression, *binary_expression_or_less]
TernaryExpression.grammar = (
    attr('operand1', binary_expression_or_less), '?', blank,
    attr('operand2', ternary_expression_or_less), blank, ':', blank,
    attr('operand3', ternary_expression_or_less)
)

BracketedExpression.grammar  = '[', attr('content', ternary_expression_or_less), ']'
ParensExpression.grammar     = '(', attr('content', ternary_expression_or_less), ')'
InvocationExpression.grammar = '(', attr('content', optional(pypeg2.csl((blank, ternary_expression_or_less)))), ')'

AssignmentExpression.grammar = (
    attr('operand1', PostfixExpression), blank,
    attr('operator', re.compile('[*/+-]?=')), blank,
    attr('operand2', [AssignmentExpression, *ternary_expression_or_less])
)

VariableDeclaration.grammar = (
    attr('qualifiers', maybe_some(re.compile('const|highp|mediump|lowp|attribute|uniform|varying'))),
    attr('type', PostfixExpression), blank,
    attr('names', pypeg2.csl(token)),
    attr('value', optional(blank, '=', blank, [AssignmentExpression, *ternary_expression_or_less]))
)

ReturnStatement.grammar = ('return', blank, attr('value', optional(ternary_expression_or_less)))

simple_statement = ([
    re.compile('continue|break|discard'), ReturnStatement,
    VariableDeclaration, AssignmentExpression, PostfixExpression
], ';', endl)
code_block = maybe_some(
    [
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
        attr('operation', optional([AssignmentExpression, PreIncrementExpression, PostIncrementExpression, PostfixExpression])), 
    ')', endl,
    attr('content', [compound_statement, simple_statement] ), endl
)

ParameterDeclaration.grammar = (
    attr('qualifiers', maybe_some(re.compile('in|out|inout'), blank)),
    attr('type', PostfixExpression),
    attr('name', token)
)
FunctionDeclaration.grammar = (
    attr('type', PostfixExpression), blank, attr('name', token), 
    '(', 
    attr('parameters',  
            optional(endl, pypeg2.indent(ParameterDeclaration, maybe_some(',', endl, ParameterDeclaration)), endl)  
        ), 
    ')', endl,
    attr('content', compound_statement), endl
)

StructureDeclaration.grammar = (
    'struct', blank, attr('name', token), 
    endl, '{', endl,
    attr('content', pypeg2.indent(maybe_some(VariableDeclaration, ';', endl)) ), 
    '}', ';', endl, endl
)

glsl = maybe_some(
    [
        inline_comment, endline_comment,
        StructureDeclaration, FunctionDeclaration, (VariableDeclaration, ';', endl)
    ]
)


class LexicalScope:
    """ 
    A "LexicalScope" is a conceptually immutable data structure containing 
    dictionaries to access type information for
    variables, functions return values, and data structure attributes
    within a given lexical scope.
    """

    @staticmethod
    def get_local_variable_type_lookups(content):
        if not isinstance(content, list):
            return {}
        result = { }
        code_block_element_types = [
            IfStatement,
            DoWhileStatement,
            WhileStatement,
            ForStatement,
        ]
        for element in content:
            if isinstance(element, ForStatement):
                for name in element.declaration.names:
                    result[name] = element.declaration.type.content
            if isinstance(element, VariableDeclaration):
                for name in element.names:
                    result[name] = element.type.content
            elif isinstance(element, ParameterDeclaration):
                result[element.name] = element.type.content
            elif type(element) in code_block_element_types:
                result.update(LexicalScope.get_local_variable_type_lookups(element.content))
        return result

    @staticmethod
    def get_global_variable_type_lookups(glsl):
        result = {}
        for element in glsl:
            if isinstance(element, VariableDeclaration):
                for name in element.names:
                    result[name] = element.type.content
        return result

    @staticmethod
    def get_attribute_type_lookups(glsl):
        result = {}
        for element in glsl:
            if isinstance(element, StructureDeclaration):
                attribute_types = {}
                for declaration in element.content:
                    if isinstance(declaration, VariableDeclaration):
                        for name in declaration.names:
                            result[name] = declaration.type.content
                result[element.name] = attribute_types
        return result

    @staticmethod
    def get_function_type_lookups(glsl):
        result = {}
        for element in glsl:
            if isinstance(element, FunctionDeclaration):
                result[element.name] = element.type.content
        return result

    def __init__(self, glsl = []):
        self.variables  = LexicalScope.get_global_variable_type_lookups(glsl)
        self.functions  = LexicalScope.get_function_type_lookups(glsl)
        self.attributes = LexicalScope.get_attribute_type_lookups(glsl)
        
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
        

vector_types = [
    ['vec2'], ['vec3'], ['vec4'], 
    ['ivec2'], ['ivec3'], ['ivec4'],
    ['bvec2'], ['bvec3'], ['bvec4'],
]
matrix_types = [
    ['mat2'],  ['mat3'],  ['mat4'],    ['mat2x3'],  ['mat2x4'],    ['mat3x2'],    ['mat3x4'],   ['mat4x2'],    ['mat4x3'], 
    ['imat2'], ['imat3'], ['imat4'],   ['imat2x3'], ['imat2x4'],   ['imat3x2'],   ['imat3x4'],  ['imat4x2'],   ['imat4x3'],
    ['bmat2'], ['bmat3'], ['bmat4'],   ['bmat2x3'], ['bmat2x4'],   ['bmat3x2'],   ['bmat3x4'],  ['bmat4x2'],   ['bmat4x3'],
]
built_in_types = [
    *vector_types,
    *matrix_types,
    ['float'], ['int'], ['bool']
]

def get_expression_type(expression, scope):
    def _get_postfix_expression_content_type(expression_content, scope):
        if len(expression_content) < 1:
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
        elements = [*expression_content]
        previous = elements.pop(0)
        previous_type = []
        # parentheses
        if isinstance(previous, ParensExpression):
            previous_type = get_expression_type(previous.content, scope)
        # variable reference
        if isinstance(previous, str):
            if previous in scope.variables:
                previous_type = scope.variables[previous]
            elif previous in scope.variables:
                previous_type = scope.variables[previous]
        while len(elements) > 0:
            current = elements.pop(0)
            if isinstance(current, InvocationExpression):
                # constructor (built-in)
                if [previous] in built_in_types:
                    previous_type = [previous]
                # constructor (user-defined)
                elif previous in scope.attributes:
                    previous_type = [previous]
                # function invocation (built-in)
                elif previous in built_in_function_types:
                    previous_type = built_in_function_types[previous]
                # function invocation (built-in, overloaded)
                elif previous in glsl_built_in_overloaded_function_types:
                    param_types = [get_expression_type(param, scope) for param in current.content]
                    vector_param_types = [param_type for param_type in param_types if param_type in vector_types]
                    previous_type = vector_param_types[0] if len(vector_param_types) > 0 else param_types[0]
                # function invocation (user-defined)
                elif previous in scope.functions:
                    previous_type = scope.functions[previous]
                else:
                    expression_str = ''#pypeg2.compose(PostfixExpression(expression_content), PostfixExpression)
                    print(f'WARNING: could not invoke unknown function: "{previous}" \n\t{expression_str}')
                    previous_type = []
            if isinstance(current, BracketedExpression):
                # matrix column access
                if previous_type in matrix_types:
                    previous_type = [re.sub('mat(\d)x?', 'vec\\1', previous_type[0])]
                # vector component access
                elif previous_type in vector_types:
                    previous_type = ['float']
                # array index access
                elif (len(previous_type) > 1 and 
                    isinstance(previous_type[1], BracketedExpression)):
                    previous_type = previous_type[:-1]
                else:
                    expression_str = ''#pypeg2.compose(PostfixExpression(expression_content), PostfixExpression)
                    print(f'WARNING: could not access index of non-array: "{previous_type}" \n\t{expression_str}')
                    previous_type = []
            if isinstance(current, str):
                # vector component access
                if previous_type in vector_types: # likely an attribute of a built-in structure, like a vector
                    previous_type = ['float']
                # attribute access
                elif (len(previous_type) == 1 and 
                    previous_type[0] in scope.attributes and
                    current in scope.attributes[previous_type[0]]):
                    previous_type = scope.attributes[previous_type[0]][current]
                else:
                    expression_str = ''#pypeg2.compose(PostfixExpression(expression_content), PostfixExpression)
                    print(f'WARNING: could not find attribute within unknown data structure: "{previous_type}" \n\t{expression_str}')
                    previous_type = []
            previous = current
        return previous_type 

    if isinstance(expression, str):
        if float_literal.match(expression):
            return ['float']
        if int_literal.match(expression):
            return ['int']
        if bool_literal.match(expression):
            return ['bool']
    if isinstance(expression, PostfixExpression):
        return _get_postfix_expression_content_type(expression.content, scope)
    if isinstance(expression, UnaryExpression):
        return get_expression_type(expression.operand1, scope)
    if isinstance(expression, BinaryExpression):
        type1 = get_expression_type(expression.operand1, scope)
        type2 = get_expression_type(expression.operand2, scope)
        vector_type = type1 if type1 in vector_types else type2 if type2 in vector_types else []
        matrix_type = type1 if type1 in matrix_types else type2 if type2 in matrix_types else []
        if vector_type:
            return vector_type
        elif matrix_type:
            return matrix_type
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
            return type1
    if isinstance(expression, TernaryExpression):
        type1 = get_expression_type(expression.operand2, scope)
        type2 = get_expression_type(expression.operand3, scope)
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
            print(scope.variables)
        return type1
