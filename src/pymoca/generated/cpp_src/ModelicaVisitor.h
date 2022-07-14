
// Generated from Modelica.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "ModelicaParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by ModelicaParser.
 */
class  ModelicaVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by ModelicaParser.
   */
    virtual antlrcpp::Any visitStored_definition(ModelicaParser::Stored_definitionContext *context) = 0;

    virtual antlrcpp::Any visitStored_definition_class(ModelicaParser::Stored_definition_classContext *context) = 0;

    virtual antlrcpp::Any visitClass_definition(ModelicaParser::Class_definitionContext *context) = 0;

    virtual antlrcpp::Any visitClass_prefixes(ModelicaParser::Class_prefixesContext *context) = 0;

    virtual antlrcpp::Any visitClass_type(ModelicaParser::Class_typeContext *context) = 0;

    virtual antlrcpp::Any visitClass_spec_comp(ModelicaParser::Class_spec_compContext *context) = 0;

    virtual antlrcpp::Any visitClass_spec_base(ModelicaParser::Class_spec_baseContext *context) = 0;

    virtual antlrcpp::Any visitClass_spec_enum(ModelicaParser::Class_spec_enumContext *context) = 0;

    virtual antlrcpp::Any visitClass_spec_der(ModelicaParser::Class_spec_derContext *context) = 0;

    virtual antlrcpp::Any visitClass_spec_extends(ModelicaParser::Class_spec_extendsContext *context) = 0;

    virtual antlrcpp::Any visitBase_prefix(ModelicaParser::Base_prefixContext *context) = 0;

    virtual antlrcpp::Any visitEnum_list(ModelicaParser::Enum_listContext *context) = 0;

    virtual antlrcpp::Any visitEnumeration_literal(ModelicaParser::Enumeration_literalContext *context) = 0;

    virtual antlrcpp::Any visitComposition(ModelicaParser::CompositionContext *context) = 0;

    virtual antlrcpp::Any visitLanguage_specification(ModelicaParser::Language_specificationContext *context) = 0;

    virtual antlrcpp::Any visitExternal_function_call(ModelicaParser::External_function_callContext *context) = 0;

    virtual antlrcpp::Any visitElement_list(ModelicaParser::Element_listContext *context) = 0;

    virtual antlrcpp::Any visitElement(ModelicaParser::ElementContext *context) = 0;

    virtual antlrcpp::Any visitRegular_element(ModelicaParser::Regular_elementContext *context) = 0;

    virtual antlrcpp::Any visitReplaceable_element(ModelicaParser::Replaceable_elementContext *context) = 0;

    virtual antlrcpp::Any visitImport_clause(ModelicaParser::Import_clauseContext *context) = 0;

    virtual antlrcpp::Any visitImport_list(ModelicaParser::Import_listContext *context) = 0;

    virtual antlrcpp::Any visitExtends_clause(ModelicaParser::Extends_clauseContext *context) = 0;

    virtual antlrcpp::Any visitConstraining_clause(ModelicaParser::Constraining_clauseContext *context) = 0;

    virtual antlrcpp::Any visitComponent_clause(ModelicaParser::Component_clauseContext *context) = 0;

    virtual antlrcpp::Any visitType_prefix(ModelicaParser::Type_prefixContext *context) = 0;

    virtual antlrcpp::Any visitType_specifier_element(ModelicaParser::Type_specifier_elementContext *context) = 0;

    virtual antlrcpp::Any visitType_specifier(ModelicaParser::Type_specifierContext *context) = 0;

    virtual antlrcpp::Any visitComponent_list(ModelicaParser::Component_listContext *context) = 0;

    virtual antlrcpp::Any visitComponent_declaration(ModelicaParser::Component_declarationContext *context) = 0;

    virtual antlrcpp::Any visitCondition_attribute(ModelicaParser::Condition_attributeContext *context) = 0;

    virtual antlrcpp::Any visitDeclaration(ModelicaParser::DeclarationContext *context) = 0;

    virtual antlrcpp::Any visitModification_class(ModelicaParser::Modification_classContext *context) = 0;

    virtual antlrcpp::Any visitModification_assignment(ModelicaParser::Modification_assignmentContext *context) = 0;

    virtual antlrcpp::Any visitModification_assignment2(ModelicaParser::Modification_assignment2Context *context) = 0;

    virtual antlrcpp::Any visitClass_modification(ModelicaParser::Class_modificationContext *context) = 0;

    virtual antlrcpp::Any visitArgument_list(ModelicaParser::Argument_listContext *context) = 0;

    virtual antlrcpp::Any visitArgument(ModelicaParser::ArgumentContext *context) = 0;

    virtual antlrcpp::Any visitElement_modification_or_replaceable(ModelicaParser::Element_modification_or_replaceableContext *context) = 0;

    virtual antlrcpp::Any visitElement_modification(ModelicaParser::Element_modificationContext *context) = 0;

    virtual antlrcpp::Any visitElement_redeclaration(ModelicaParser::Element_redeclarationContext *context) = 0;

    virtual antlrcpp::Any visitElement_replaceable(ModelicaParser::Element_replaceableContext *context) = 0;

    virtual antlrcpp::Any visitComponent_clause1(ModelicaParser::Component_clause1Context *context) = 0;

    virtual antlrcpp::Any visitComponent_declaration1(ModelicaParser::Component_declaration1Context *context) = 0;

    virtual antlrcpp::Any visitShort_class_definition(ModelicaParser::Short_class_definitionContext *context) = 0;

    virtual antlrcpp::Any visitEquation_block(ModelicaParser::Equation_blockContext *context) = 0;

    virtual antlrcpp::Any visitEquation_section(ModelicaParser::Equation_sectionContext *context) = 0;

    virtual antlrcpp::Any visitStatement_block(ModelicaParser::Statement_blockContext *context) = 0;

    virtual antlrcpp::Any visitAlgorithm_section(ModelicaParser::Algorithm_sectionContext *context) = 0;

    virtual antlrcpp::Any visitEquation_simple(ModelicaParser::Equation_simpleContext *context) = 0;

    virtual antlrcpp::Any visitEquation_if(ModelicaParser::Equation_ifContext *context) = 0;

    virtual antlrcpp::Any visitEquation_for(ModelicaParser::Equation_forContext *context) = 0;

    virtual antlrcpp::Any visitEquation_connect_clause(ModelicaParser::Equation_connect_clauseContext *context) = 0;

    virtual antlrcpp::Any visitEquation_when(ModelicaParser::Equation_whenContext *context) = 0;

    virtual antlrcpp::Any visitEquation_function(ModelicaParser::Equation_functionContext *context) = 0;

    virtual antlrcpp::Any visitEquation(ModelicaParser::EquationContext *context) = 0;

    virtual antlrcpp::Any visitStatement_component_reference(ModelicaParser::Statement_component_referenceContext *context) = 0;

    virtual antlrcpp::Any visitStatement_component_function(ModelicaParser::Statement_component_functionContext *context) = 0;

    virtual antlrcpp::Any visitStatement_break(ModelicaParser::Statement_breakContext *context) = 0;

    virtual antlrcpp::Any visitStatement_return(ModelicaParser::Statement_returnContext *context) = 0;

    virtual antlrcpp::Any visitStatement_if(ModelicaParser::Statement_ifContext *context) = 0;

    virtual antlrcpp::Any visitStatement_for(ModelicaParser::Statement_forContext *context) = 0;

    virtual antlrcpp::Any visitStatement_while(ModelicaParser::Statement_whileContext *context) = 0;

    virtual antlrcpp::Any visitStatement_when(ModelicaParser::Statement_whenContext *context) = 0;

    virtual antlrcpp::Any visitStatement(ModelicaParser::StatementContext *context) = 0;

    virtual antlrcpp::Any visitIf_equation(ModelicaParser::If_equationContext *context) = 0;

    virtual antlrcpp::Any visitIf_statement(ModelicaParser::If_statementContext *context) = 0;

    virtual antlrcpp::Any visitFor_equation(ModelicaParser::For_equationContext *context) = 0;

    virtual antlrcpp::Any visitFor_statement(ModelicaParser::For_statementContext *context) = 0;

    virtual antlrcpp::Any visitFor_indices(ModelicaParser::For_indicesContext *context) = 0;

    virtual antlrcpp::Any visitFor_index(ModelicaParser::For_indexContext *context) = 0;

    virtual antlrcpp::Any visitWhile_statement(ModelicaParser::While_statementContext *context) = 0;

    virtual antlrcpp::Any visitWhen_equation(ModelicaParser::When_equationContext *context) = 0;

    virtual antlrcpp::Any visitWhen_statement(ModelicaParser::When_statementContext *context) = 0;

    virtual antlrcpp::Any visitConnect_clause(ModelicaParser::Connect_clauseContext *context) = 0;

    virtual antlrcpp::Any visitExpression_simple(ModelicaParser::Expression_simpleContext *context) = 0;

    virtual antlrcpp::Any visitExpression_if(ModelicaParser::Expression_ifContext *context) = 0;

    virtual antlrcpp::Any visitSimple_expression(ModelicaParser::Simple_expressionContext *context) = 0;

    virtual antlrcpp::Any visitExpr_add(ModelicaParser::Expr_addContext *context) = 0;

    virtual antlrcpp::Any visitExpr_signed(ModelicaParser::Expr_signedContext *context) = 0;

    virtual antlrcpp::Any visitExpr_exp(ModelicaParser::Expr_expContext *context) = 0;

    virtual antlrcpp::Any visitExpr_or(ModelicaParser::Expr_orContext *context) = 0;

    virtual antlrcpp::Any visitExpr_primary(ModelicaParser::Expr_primaryContext *context) = 0;

    virtual antlrcpp::Any visitExpr_and(ModelicaParser::Expr_andContext *context) = 0;

    virtual antlrcpp::Any visitExpr_rel(ModelicaParser::Expr_relContext *context) = 0;

    virtual antlrcpp::Any visitExpr_not(ModelicaParser::Expr_notContext *context) = 0;

    virtual antlrcpp::Any visitExpr_mul(ModelicaParser::Expr_mulContext *context) = 0;

    virtual antlrcpp::Any visitPrimary_unsigned_number(ModelicaParser::Primary_unsigned_numberContext *context) = 0;

    virtual antlrcpp::Any visitPrimary_string(ModelicaParser::Primary_stringContext *context) = 0;

    virtual antlrcpp::Any visitPrimary_false(ModelicaParser::Primary_falseContext *context) = 0;

    virtual antlrcpp::Any visitPrimary_true(ModelicaParser::Primary_trueContext *context) = 0;

    virtual antlrcpp::Any visitPrimary_function(ModelicaParser::Primary_functionContext *context) = 0;

    virtual antlrcpp::Any visitPrimary_derivative(ModelicaParser::Primary_derivativeContext *context) = 0;

    virtual antlrcpp::Any visitPrimary_initial(ModelicaParser::Primary_initialContext *context) = 0;

    virtual antlrcpp::Any visitPrimary_component_reference(ModelicaParser::Primary_component_referenceContext *context) = 0;

    virtual antlrcpp::Any visitPrimary_output_expression_list(ModelicaParser::Primary_output_expression_listContext *context) = 0;

    virtual antlrcpp::Any visitPrimary_expression_list(ModelicaParser::Primary_expression_listContext *context) = 0;

    virtual antlrcpp::Any visitPrimary_function_arguments(ModelicaParser::Primary_function_argumentsContext *context) = 0;

    virtual antlrcpp::Any visitPrimary_end(ModelicaParser::Primary_endContext *context) = 0;

    virtual antlrcpp::Any visitName(ModelicaParser::NameContext *context) = 0;

    virtual antlrcpp::Any visitComponent_reference_element(ModelicaParser::Component_reference_elementContext *context) = 0;

    virtual antlrcpp::Any visitComponent_reference(ModelicaParser::Component_referenceContext *context) = 0;

    virtual antlrcpp::Any visitFunction_call_args(ModelicaParser::Function_call_argsContext *context) = 0;

    virtual antlrcpp::Any visitFunction_arguments(ModelicaParser::Function_argumentsContext *context) = 0;

    virtual antlrcpp::Any visitNamed_arguments(ModelicaParser::Named_argumentsContext *context) = 0;

    virtual antlrcpp::Any visitNamed_argument(ModelicaParser::Named_argumentContext *context) = 0;

    virtual antlrcpp::Any visitArgument_function(ModelicaParser::Argument_functionContext *context) = 0;

    virtual antlrcpp::Any visitArgument_expression(ModelicaParser::Argument_expressionContext *context) = 0;

    virtual antlrcpp::Any visitOutput_expression_list(ModelicaParser::Output_expression_listContext *context) = 0;

    virtual antlrcpp::Any visitExpression_list(ModelicaParser::Expression_listContext *context) = 0;

    virtual antlrcpp::Any visitArray_subscripts(ModelicaParser::Array_subscriptsContext *context) = 0;

    virtual antlrcpp::Any visitSubscript(ModelicaParser::SubscriptContext *context) = 0;

    virtual antlrcpp::Any visitComment(ModelicaParser::CommentContext *context) = 0;

    virtual antlrcpp::Any visitString_comment(ModelicaParser::String_commentContext *context) = 0;

    virtual antlrcpp::Any visitAnnotation(ModelicaParser::AnnotationContext *context) = 0;


};

