import re

import pypeg2
from pypeg2 import attr, optional, maybe_some, blank, endl

single_quote_string_literal = re.compile("'([^']|\\\\')*'", re.MULTILINE | re.DOTALL)
double_quote_string_literal = re.compile('"([^"]|\\\\")*"', re.MULTILINE | re.DOTALL)
back_tick_string_literal = re.compile('`([^`]|\\\\`)*`', re.MULTILINE | re.DOTALL)
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

'''
"element_attributes" is a list of all attributes 
that can be found within instances of JsElements
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
    'else_',

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
    elif isinstance(element, JsElement):
        try:
            pypeg2.compose(element, type(element))
        except ValueError as error:
            warnings.warn(f'element of type "{type(element)}" is invalid: \n{debug(element, "  ")}')
        for attribute in element_attributes:
            if hasattr(element, attribute):
                subelement = getattr(element, attribute)
                warn_of_invalid_grammar_elements(subelement)

'''
"debug" returns a string representing a variable that is intended to be a JsElement.
It attempts to provide a string representation of the element using pypeg2.compose()
In the event it is unable to do so, it recurses through element attributes 
to give a description of which subelements are causing problems.
This makes it very useful for debugging errors where pypeg2.compose()
returns an empty string without explaining why
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
    elif isinstance(element, JsElement):
        try:
            return (f'{indent}pypeg2js.{type(element).__name__}<"{pypeg2.compose(element, type(element))}">')
        except ValueError as e:
            header = f'{indent}pypeg2js.{type(element).__name__}<ERROR>'
            invalid = '\n'.join([
                    f'{indent}  {attribute}: \n{debug(getattr(element, attribute), indent+"    ")}' 
                    for attribute in element_attributes 
                    if hasattr(element, attribute)
                ])
            return f'{header}\n{invalid}'
    else:
        return indent + repr(element)

class JsElement:
    def __init__(self):
        pass
    def debug(self):
        return f'pypeg2js.{type(self).__name__}<"{pypeg2.compose(self, type(self))}">'

class PostfixExpression(JsElement): 
    def __init__(self, content=None):
        self.content = content or []

class UnaryExpression(JsElement): 
    def __init__(self, operand1 = None, operator = ''):
        self.operand1 = operand1
        self.operator = operator

class BinaryExpression(JsElement): 
    def __init__(self, operand1 = None, operator = '', operand2 = None):
        self.operand1 = operand1
        self.operator = operator
        self.operand2 = operand2

class TernaryExpression(JsElement): 
    def __init__(self, operand1 = None, operand2 = None, operand3 = None):
        self.operand1 = operand1
        self.operand2 = operand2
        self.operand3 = operand3
        self.comment1 = ''
        self.comment2 = ''
        self.comment3 = ''

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

class AttributeDeclaration(JsElement): 
    def __init__(self, name=None, value=None):
        self.name = name
        self.value = value
class AssociativeListExpression(JsElement): 
    def __init__(self, content=None):
        self.content = content or []
class OrderedListExpression(JsElement): 
    def __init__(self, content=None):
        self.content = content or []

class InvocationExpression(JsElement): 
    def __init__(self, content=None):
        self.content = content or []
class BracketedExpression(JsElement): 
    def __init__(self, content=None):
        self.content = content or []
class ParensExpression(JsElement): 
    def __init__(self, content=None):
        self.content = content or []
class AssignmentExpression(JsElement): pass
class VariableDeclaration(JsElement): 
    def __init__(self, names=None, value=None):
        self.names = names or []
        self.value = value
class ReturnStatement(JsElement): 
    def __init__(self, value=None):
        self.value = value

class IfStatement(JsElement): pass
class WhileStatement(JsElement): pass
class DoWhileStatement(JsElement): pass
class ForStatement(JsElement): pass

class ParameterDeclaration(JsElement): 
    def __init__(self, name=''):
        self.name = name
class FunctionDeclaration(JsElement): 
    def __init__(self, name='', parameters=None, content=None, type_=None, documentation=None):
        self.documentation = documentation or []
        self.type = type_ or []
        self.name = name
        self.parameters = parameters or []
        self.content = content or []

primary_expression = [
    single_quote_string_literal,
    double_quote_string_literal,
    back_tick_string_literal,
    float_literal, 
    int_literal, 
    bool_literal, 
    token,
    ParensExpression
]

PostfixExpression.grammar = (
    attr('content', ([OrderedListExpression, AssociativeListExpression, primary_expression, token], maybe_some([BracketedExpression, InvocationExpression, ('.', token)])))
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

AttributeDeclaration.grammar = (
    attr('name', [single_quote_string_literal, double_quote_string_literal, float_literal, int_literal, bool_literal, token]),
    ':',
    attr('value', ternary_expression_or_less)
)
AssociativeListExpression.grammar  = ('{', endl,
    attr('content', pypeg2.indent(optional(AttributeDeclaration, maybe_some(',', endl, AttributeDeclaration)), endl)), 
'}')
OrderedListExpression.grammar= ('[', 
    attr('content'), pypeg2.indent(pypeg2.csl(ternary_expression_or_less)),
']')
BracketedExpression.grammar  = '[', attr('content', ternary_expression_or_less), ']'
InvocationExpression.grammar = '(', attr('content', optional(pypeg2.csl((blank, ternary_expression_or_less)))), ')'
ParensExpression.grammar     = '(', attr('content', ternary_expression_or_less), ')'

AssignmentExpression.grammar = (
    attr('operand1', PostfixExpression), blank,
    attr('operator', re.compile('[*/+-]?=')), blank,
    attr('operand2', [AssignmentExpression, *ternary_expression_or_less])
)

VariableDeclaration.grammar = (
    attr('qualifiers', maybe_some((re.compile('const|var|let'), blank))),
    attr('names', pypeg2.csl(token)),
    attr('value', optional(blank, '=', blank, [AssignmentExpression, *ternary_expression_or_less]))
)

ReturnStatement.grammar = ('return', blank, attr('value', optional(ternary_expression_or_less)))

simple_statement = ([
    re.compile('continue|break|discard'), ReturnStatement,
    VariableDeclaration, AssignmentExpression, PostfixExpression
], optional(';'), endl)
code_block = maybe_some(
    [
        # first try functions: they're harder to parse, but we need 
        # to parse them before comments since they have their own comment docs
        FunctionDeclaration, 
        # then the rest, in no particular order
        ForStatement, 
        WhileStatement, 
        DoWhileStatement, 
        IfStatement, 
        simple_statement,
        # next try standalone comments since they're quick to parse
        (inline_comment, endl, endl), 
        (endline_comment, endl, endl),
        
        # ForStatement, 
        # WhileStatement, 
        # DoWhileStatement, 
        # IfStatement, 
        # FunctionDeclaration, 
        # simple_statement,
        # inline_comment, 
        # endline_comment,
    ]
)
compound_statement = ( '{', endl, pypeg2.indent(code_block), '}', endl )

IfStatement.grammar = (
    'if', blank, '(', attr('condition', ternary_expression_or_less), ')',
    attr('content', [compound_statement, simple_statement] ), 
    attr('else_', optional('else', blank, [IfStatement, compound_statement, simple_statement])), endl
)
WhileStatement.grammar = (
    'while', blank, '(', attr('condition', ternary_expression_or_less), ')', 
    attr('content', [compound_statement, simple_statement] ), endl
)
DoWhileStatement.grammar = (
    'do', blank, attr('content', [compound_statement, simple_statement] ), 
    'while', '(', attr('condition', ternary_expression_or_less), ')', ';', endl
)
ForStatement.grammar = (
    'for', blank, '(', 
        attr('declaration', VariableDeclaration), ';', blank,
        attr('condition', ternary_expression_or_less), ';', blank,
        attr('operation', optional([AssignmentExpression, PreIncrementExpression, PostIncrementExpression, PostfixExpression])), 
    ')', blank,
    attr('content', [compound_statement, simple_statement] ), endl
)

ParameterDeclaration.grammar = (
    attr('name', token) 
)
FunctionDeclaration.grammar = (
    attr('documentation', maybe_some([inline_comment, endline_comment])), 
    attr('type', optional(inline_comment, endl)), 
    'function', blank, attr('name', token), 
    '(', endl,
    attr('parameters',  
            optional(pypeg2.indent(
                optional(blank, inline_comment), blank, ParameterDeclaration, 
                maybe_some(',', endl, optional(blank, inline_comment), blank, ParameterDeclaration) 
            ), endl)
        ), 
    ')', 
    attr('content', compound_statement), endl
)

code = code_block
