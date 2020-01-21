import re

import pypeg2
from pypeg2 import attr, optional, maybe_some, blank, endl

single_quote_string_literal = re.compile("'([^']|\\\\')*'", re.MULTILINE | re.DOTALL)
double_quote_string_literal = re.compile('"([^"]|\\\\")*"', re.MULTILINE | re.DOTALL)
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

class PostfixExpression: pass
class PostIncrementExpression: pass
class PreIncrementExpression: pass

class MultiplicativeExpression: pass
class AdditiveExpression: pass
class ShiftExpression: pass
class RelationalExpression: pass
class EqualityExpression: pass
class BitwiseAndExpression: pass
class BitwiseXorExpression: pass
class BitwiseOrExpression: pass
class LogicalAndExpression: pass
class LogicalXorExpression: pass
class LogicalOrExpression: pass

class AttributeDeclaration: pass
class AssociativeListExpression: pass
class OrderedListExpression: pass

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
class FunctionDeclaration: pass

primary_expression = [
    single_quote_string_literal,
    double_quote_string_literal,
    float_literal, 
    int_literal, 
    bool_literal, 
    ParensExpression
]

PostfixExpression.grammar = (
    attr('content', ([OrderedListExpression, AssociativeListExpression, token], maybe_some([BracketedExpression, InvocationExpression, ('.', token)])))
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
for BinaryExpression, binary_regex in order_of_operations:
    binary_expression_or_less = [BinaryExpression, *binary_expression_or_less]
    BinaryExpression.grammar = (
            attr('operand1', binary_expression_or_less[1:]), blank,
            attr('operator', binary_regex), blank,
            attr('operand2', binary_expression_or_less),
        )

ternary_expression_or_less = [TernaryExpression, *binary_expression_or_less]
TernaryExpression.grammar = (
    attr('operand1', binary_expression_or_less), '?', blank,
    attr('operand2', ternary_expression_or_less), ':', blank,
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
ParensExpression.grammar     = '(', attr('content', ternary_expression_or_less), ')'
InvocationExpression.grammar = '(', attr('content', optional(pypeg2.csl((blank, ternary_expression_or_less)))), ')'

AssignmentExpression.grammar = (
    attr('operand1', PostfixExpression), blank,
    attr('operator', re.compile('[*/+-]?=')), blank,
    attr('operand2', [AssignmentExpression, *ternary_expression_or_less])
)

VariableDeclaration.grammar = (
    attr('qualifiers', maybe_some((re.compile('const|var|let'), blank))),
    attr('name', token),
    attr('value', optional(blank, '=', blank, [AssignmentExpression, *ternary_expression_or_less]))
)

ReturnStatement.grammar = ('return', blank, attr('value', optional(ternary_expression_or_less)))

simple_statement = ([
	re.compile('continue|break|discard'), ReturnStatement,
	VariableDeclaration, AssignmentExpression, PostfixExpression
], optional(';'), endl)
code_block = maybe_some(
	[
		# inline_comment, 
		# endline_comment,
		ForStatement, 
		WhileStatement, 
		DoWhileStatement, 
		IfStatement, 
		FunctionDeclaration,
		simple_statement
	]
)
compound_statement = ( '{', endl, pypeg2.indent(code_block), '}', endl )

IfStatement.grammar = (
    'if', blank, '(', attr('condition', ternary_expression_or_less), ')',
	attr('content', [compound_statement, simple_statement] ), 
    attr('else', optional('else', blank, [IfStatement, compound_statement, simple_statement])), endl
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
    'function', blank, attr('name', token), 
    '(', 
    attr('parameters',  
    		optional(pypeg2.indent(ParameterDeclaration, maybe_some(',', blank, ParameterDeclaration)), endl)  
    	), 
    ')', 
    attr('content', compound_statement), endl
)

javascript = code_block
def parse(javascript_text, grammar = javascript):
    sanitized = inline_comment.sub('', javascript_text)
    sanitized = endline_comment.sub('', sanitized)
    return pypeg2.parse(sanitized, grammar)
def compose(javascript_parse_tree, grammar = javascript):
    return pypeg2.compose(javascript_parse_tree, grammar)