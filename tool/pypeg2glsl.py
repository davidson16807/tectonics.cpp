# holy fucking shit that was a lot of work 😩

import re

import pypeg2
from pypeg2 import attr, optional, maybe_some, blank, endl

inline_comment = re.compile('/\*((?!\*/).)*\*/', re.MULTILINE | re.DOTALL)
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
    attr('name', token),
    attr('value', optional(blank, '=', blank, [AssignmentExpression, *ternary_expression_or_less]))
)

ReturnStatement.grammar = ('return', blank, attr('value', optional(ternary_expression_or_less)))

simple_statement = ([
	re.compile('continue|break|discard'), ReturnStatement,
	VariableDeclaration, AssignmentExpression, PostfixExpression
], ';', endl)
code_block = maybe_some(
	[
		# inline_comment, 
		# endline_comment,
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
		# inline_comment, endline_comment,
		StructureDeclaration, FunctionDeclaration, (VariableDeclaration, ';', endl)
	]
)
def parse(glsl_text, grammar = glsl):
    sanitized = inline_comment.sub('', glsl_text)
    sanitized = endline_comment.sub('', sanitized)
    return pypeg2.parse(sanitized, grammar)
def compose(glsl_parse_tree, grammar = glsl):
    return pypeg2.compose(glsl_parse_tree, grammar)